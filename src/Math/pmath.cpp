#include <PML/Core/aligned_vector.h>
#include <PML/Math/pmath.h>
#include <PML/Math/Constants.h>

#include <intrin.h>

// One of parts of 2/pi
#define PIOVERTWO_PART1 1.5707963109016418

// One of parts of 2/pi
#define PIOVERTWO_PART2 1.5893254712295857E-8

// One of parts of 2/pi
#define PIOVERTWO_PART3 6.123233995736766E-17

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
            double mSinCosTable[2*((1UL << EXPBIT) + 1)];

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
                    mSinCosTable[2*i  ] = 0.5*std::sin(i*mSinCosAlphaInv);
                    mSinCosTable[2*i+1] = 0.5*std::cos(i*mSinCosAlphaInv);
                }
            }
        };

        static const IEEE754Format<11, 52> gDouble;

        static constexpr double gSinCosMaxArgChar  =  0.999999*PML_CONST_PIQUATER*std::numeric_limits<char>::max();
        static constexpr double gSinCosMinArgChar  = -0.999999*PML_CONST_PIQUATER*std::numeric_limits<char>::max(); // not lowest
        static constexpr double gSinCosMaxArgShort =  0.999999*PML_CONST_PIQUATER*std::numeric_limits<short>::max();
        static constexpr double gSinCosMinArgShort = -0.999999*PML_CONST_PIQUATER*std::numeric_limits<short>::max();
        static constexpr double gSinCosMaxArgInt   =  0.999999*PML_CONST_PIQUATER*std::numeric_limits<int>::max();
        static constexpr double gSinCosMinArgInt   = -0.999999*PML_CONST_PIQUATER*std::numeric_limits<int>::max();

        template<typename T>
        double sinImpl(double inX)
        {
            bool lIsNegative = std::signbit(inX);
            inX = std::fabs(inX);

            const T lEighth = static_cast<T>(inX*PML_CONST_4OVERPI);
            const T lArgIdx = ((lEighth + 1) >> 1);

            const double lArg_0_45
                = std::fabs(((inX - PIOVERTWO_PART1 * lArgIdx)
                                  - PIOVERTWO_PART2 * lArgIdx)
                                  - PIOVERTWO_PART3 * lArgIdx);

            lIsNegative = (lIsNegative ^ ((lEighth & 7) >= 4));

            const double ln = ((lArg_0_45*gDouble.mSinCosAlpha + gDouble.mRounder) - gDouble.mRounder);

            const double ldt = (lArg_0_45 - ln * gDouble.mSinCosAlphaInv);
            const double ldtSqrd = ldt * ldt;

            double lMulToSin = (2.0 - ldtSqrd);
            double lMulToCos = ldt * (2.0 - pml::constants::Q::_1over3()*ldtSqrd);

            const bool lUseCosSumFormula = (lArgIdx & 1);
            if (lUseCosSumFormula)
            {
                std::swap(lMulToSin, lMulToCos);
                lMulToSin = -lMulToSin;
            }

            auto lIdx = static_cast<int>(ln);
            lIdx += lIdx;

            const double lSinWithoutSign = (gDouble.mSinCosTable[lIdx] * lMulToSin + gDouble.mSinCosTable[lIdx + 1] * lMulToCos);

            return (lIsNegative ? -lSinWithoutSign : +lSinWithoutSign);
        }

        template<typename T>
        double cosImpl(double inX)
        {
            inX = std::fabs(inX);

            const T lEighth = static_cast<T>(inX*PML_CONST_4OVERPI);
            const T lArgIdx = ((lEighth + 1) >> 1);

            const double lArg_0_45
                = std::fabs(((inX - PIOVERTWO_PART1 * lArgIdx)
                                  - PIOVERTWO_PART2 * lArgIdx)
                                  - PIOVERTWO_PART3 * lArgIdx);

            const double ln = ((lArg_0_45*gDouble.mSinCosAlpha + gDouble.mRounder) - gDouble.mRounder);

            const double ldt = (lArg_0_45 - ln * gDouble.mSinCosAlphaInv);
            const double ldtSqrd = ldt * ldt;

            double lMulToCos = (2.0 - ldtSqrd);
            double lMulToSin = ldt * (2.0 - pml::constants::Q::_1over3()*ldtSqrd);

            const bool lUseSinSumFormula = (lArgIdx & 1);
            if (lUseSinSumFormula)
            {
                std::swap(lMulToSin, lMulToCos);
                lMulToSin = -lMulToSin;
            }

            auto lIdx = static_cast<int>(ln);
            lIdx += lIdx;

            const double lCosWithoutSign= (gDouble.mSinCosTable[lIdx + 1] * lMulToCos - gDouble.mSinCosTable[lIdx] * lMulToSin);

            const auto lResidue = (lEighth & 7);

            return (((lResidue < 2) | (lResidue > 5)) ? +lCosWithoutSign : -lCosWithoutSign);
        }
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
        return ((gSinCosMinArgChar  < inX) && (inX < gSinCosMaxArgChar) ) ? sinImpl<char>    (inX):
               ((gSinCosMinArgShort < inX) && (inX < gSinCosMaxArgShort)) ? sinImpl<short>   (inX):
               ((gSinCosMinArgInt   < inX) && (inX < gSinCosMaxArgInt)  ) ? sinImpl<int>     (inX):
                                                                            sinImpl<uint64_t>(inX);
    }

    double cos(double inX)
    {
        return ((gSinCosMinArgChar  < inX) && (inX < gSinCosMaxArgChar) ) ? cosImpl<char>    (inX):
               ((gSinCosMinArgShort < inX) && (inX < gSinCosMaxArgShort)) ? cosImpl<short>   (inX):
               ((gSinCosMinArgInt   < inX) && (inX < gSinCosMaxArgInt)  ) ? cosImpl<int>     (inX):
                                                                            cosImpl<uint64_t>(inX);
    }

}