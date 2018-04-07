#include <PML/Core/aligned_vector.h>
#include <PML/Math/pmath.h>
#include <PML/Math/Constants.h>

#include <intrin.h>

// One of parts of 2/pi
#define PART1_PIOVERTWO 1.5707963109016418

// One of parts of 2/pi
#define PART2_PIOVERTWO 1.5893254712295857E-8

// One of parts of 2/pi
#define PART3_PIOVERTWO 6.123233995736766E-17

namespace pml {

    namespace {

        /**
        * @union
        * Union of double and uint64_t.
        * This class makes bit manipulations simple.
        *
        * @param _double
        *   Value as double type.
        * @param _uint64_t
        *   Value as uint64_t type.
        */
        union di {
            double _double;
            uint64_t _uint64_t;
        };

        /**
        * @brief
        * IEEE754 format class.
        * This class represents various values related to the IEEE754 format.
        *
        * For instance, EXPBIT = 11 and FRACTIONBIT = 52 generates the following IEEE754 double format
        *
        *      | 1bit sign |---- 11bit exponet ----|-------- 52bit fraction --------|,
        *
        * which correspond to (sign)*2^{exponent - adjustment}*(1.fraction) with adjustment = (2^{10}-1).
        * The value of the adjustment is given by IEEE754Format<11, 52>::mExpAdj.
        */
        template<uint64_t EXPBIT, uint64_t FRACTIONBIT>
        struct IEEE754Format final
        {
            const uint64_t mExpBit = EXPBIT;
            const uint64_t mExpAdj = ((1UL << (EXPBIT - 1)) - 1);
            const uint64_t mFractionBit = FRACTIONBIT;
            const double mRounder = (3ULL << (FRACTIONBIT - 1));

            const double mExpAlpha    = (1UL << EXPBIT) / std::log(2.0);
            const double mExpAlphaInv = std::log(2.0) / (1UL << EXPBIT);
            uint64_t mExpTable[1UL << EXPBIT];
            const uint64_t mGuide = ((1UL << EXPBIT) - 1);

            const double mSinCosAlpha = ((1UL << EXPBIT)* PML_CONST_4OVERPI);
            const double mSinCosAlphaInv = (PML_CONST_PIQUATER / (1UL << EXPBIT));
            double mSinTable[(1UL << EXPBIT) + 1];
            double mCosTable[(1UL << EXPBIT) + 1];

            constexpr IEEE754Format() : mExpTable()
            {
                for (int i = 0;i < (1UL << EXPBIT);++i)
                {
                    di ldi;
                    ldi._double = std::pow(2.0, i / static_cast<double>(1UL << EXPBIT));
                    mExpTable[i] = (ldi._uint64_t & ((1ULL << FRACTIONBIT) - 1));
                }

                for (int i = 0;i < ((1UL << EXPBIT)+1);++i)
                {
                    mSinTable[i] = std::sin(i*mSinCosAlphaInv);
                    mCosTable[i] = std::cos(i*mSinCosAlphaInv);
                }
            }
        };

        static const double gOne[2] = { 1.0,1.0 };
        static const double gTaylorCoeff[2] = { 0.5, 1.0 / 6.0 };

        static const IEEE754Format<11, 52> gDouble;
    }

    double exp(double inX)
    {
        if (inX <= -708.39641853226408)
        {
            return 0;
        }

        if (inX >= 709.78271289338397)
        {
            return std::numeric_limits<double>::infinity();
        }

        di ldi;
        ldi._double = inX * gDouble.mExpAlpha + gDouble.mRounder;

        const uint64_t lPower1 = ((ldi._uint64_t >> gDouble.mExpBit) + gDouble.mExpAdj) << gDouble.mFractionBit;
        const uint64_t lPower2 = gDouble.mExpTable[ldi._uint64_t & gDouble.mGuide];

        const double lt = ((ldi._double - gDouble.mRounder)*gDouble.mExpAlphaInv - inX);
        const double lTayorPart = 1 - lt + (3.0000000027955394 - lt)*lt*lt*0.16666666685227835;

        ldi._uint64_t = (lPower1 | lPower2);

        return lTayorPart * ldi._double;
    }

    double sin(double inX)
    {
        double lSign = 1.0;
        if (std::signbit(inX))
        {
            lSign = -1.0;
            inX = -inX;
        }

        const auto lEighth = static_cast<uint64_t>(inX*PML_CONST_4OVERPI);
        const uint64_t lArgIdx = ((lEighth + 1) >> 1);

        const double lArg_0_45
            = std::fabs(((inX - PART1_PIOVERTWO * lArgIdx)
                              - PART2_PIOVERTWO * lArgIdx)
                              - PART3_PIOVERTWO * lArgIdx);

        lSign = (((lEighth & 7) < 4) ? lSign : -lSign);

        const double ln = ((lArg_0_45*gDouble.mSinCosAlpha + gDouble.mRounder) - gDouble.mRounder);

        const double ldt = (lArg_0_45 - ln * gDouble.mSinCosAlphaInv);
        const double ldtSqrd = ldt * ldt;        
        
        double lMulToSin = (1.0 - 0.5*ldtSqrd);
        double lMulToCos = ldt * (1.0 - pml::constants::Q::_1Over6()*ldtSqrd);
        
        const bool lUseCosSumFormula = (lArgIdx & 1);
        if (lUseCosSumFormula)
        {
            std::swap(lMulToSin, lMulToCos);
            lMulToSin = -lMulToSin;
        }

        return lSign * (gDouble.mSinTable[(int)(ln)] * lMulToSin + gDouble.mCosTable[(int)(ln)] * lMulToCos);
    }

    double cos(double inX)
    {
        inX = std::fabs(inX);

        const auto lEighth = static_cast<uint64_t>(inX*PML_CONST_4OVERPI);
        const uint64_t lArgIdx = ((lEighth + 1) >> 1);

        const double lArg_0_45
            = std::fabs(((inX - PART1_PIOVERTWO * lArgIdx)
                              - PART2_PIOVERTWO * lArgIdx)
                              - PART3_PIOVERTWO * lArgIdx);

        const uint64_t lResidu = (lEighth & 7);
        const double lSign = (((lResidu < 2) | (lResidu > 5)) ? +1 : -1);

        const double ln = ((lArg_0_45*gDouble.mSinCosAlpha + gDouble.mRounder) - gDouble.mRounder);

        const double ldt = (lArg_0_45 - ln * gDouble.mSinCosAlphaInv);
        const double ldtSqrd = ldt * ldt;

        double lMulToCos = (1.0 - 0.5*ldtSqrd);
        double lMulToSin = ldt * (1.0 - pml::constants::Q::_1Over6()*ldtSqrd);

        const bool lUseSinSumFormula = (lArgIdx & 1);
        if (lUseSinSumFormula)
        {
            std::swap(lMulToSin, lMulToCos);
            lMulToSin = -lMulToSin;
        }

        return lSign * (gDouble.mCosTable[(int)(ln)] * lMulToCos - gDouble.mSinTable[(int)(ln)] * lMulToSin);
    }

}