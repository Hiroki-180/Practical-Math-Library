#include <PML/Core/cross_intrin.h>
#include <PML/Core/aligned_allocator.h>
#include <PML/Core/CPUDispatcher.h>
#include <PML/Math/pmath.h>
#include <PML/Math/constants.h>

#include <assert.h>
#include <type_traits>
#include <cmath>

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

            constexpr IEEE754Format() : mExpTable(), mSinCosTable()
            {
                for (std::size_t i = 0;i < (1UL << EXPBIT);++i)
                {
                    di ldi;
                    ldi._double = std::pow(2.0, i / static_cast<double>(1UL << EXPBIT));
                    mExpTable[i] = (ldi._uint64_t & ((1ULL << FRACTIONBIT) - 1));
                }

                for (std::size_t i = 0;i < ((1UL << EXPBIT)+1);++i)
                {
                    mSinCosTable[2*i  ] = 0.5*std::sin(i*mSinCosAlphaInv);
                    mSinCosTable[2*i+1] = 0.5*std::cos(i*mSinCosAlphaInv);
                }
            }
        };

        static const IEEE754Format<11, 52> gDouble;

        static constexpr double gSinCosMaxArgChar   = 0.999999*PML_CONST_PIQUATER*std::numeric_limits<char>    ::max();
        static constexpr double gSinCosMaxArgShort  = 0.999999*PML_CONST_PIQUATER*std::numeric_limits<short>   ::max();
        static constexpr double gSinCosMaxArgInt    = 0.999999*PML_CONST_PIQUATER*std::numeric_limits<int>     ::max();
        static constexpr double gSinCosMaxArgUInt64 = 0.999999*PML_CONST_PIQUATER*std::numeric_limits<uint64_t>::max();

        /**
        * @param[in] inX
        * Absolute value of the argument.
        *
        * @param[in] inIsNegative
        * Sign of the argument.
        *
        * @return sin((inIsNegative ? -1 : +1)*inX)
        */
        template<typename T>
        typename std::enable_if<std::is_integral<T>::value, double>::type
        sinImpl(double inX, bool inIsNegative)
        {
            assert(inX >= 0.0);

            const T lEighth = static_cast<T>(inX*PML_CONST_4OVERPI);
            const T lArgIdx = ((lEighth + 1) >> 1);

            const double lArg_0_45
                = std::fabs(((inX - PIOVERTWO_PART1 * lArgIdx)
                                  - PIOVERTWO_PART2 * lArgIdx)
                                  - PIOVERTWO_PART3 * lArgIdx);

            inIsNegative = (inIsNegative ^ ((lEighth & 7) >= 4));

            const double ln = ((lArg_0_45*gDouble.mSinCosAlpha + gDouble.mRounder) - gDouble.mRounder);

            const double ldt = (lArg_0_45 - ln * gDouble.mSinCosAlphaInv);
            const double ldtSqrd = ldt * ldt;

            double lMulToSin = (2.0 - ldtSqrd);
            double lMulToCos = ldt * (2.0 - pml::constants::rational(1, 3)*ldtSqrd);

            const bool lUseCosSumFormula = (lArgIdx & 1);
            if (lUseCosSumFormula)
            {
                std::swap(lMulToSin, lMulToCos);
                lMulToSin = -lMulToSin;
            }

            auto lIdx = static_cast<int>(ln);
            lIdx += lIdx;

            const double lSinWithoutSign = (gDouble.mSinCosTable[lIdx] * lMulToSin + gDouble.mSinCosTable[lIdx + 1] * lMulToCos);

            return (inIsNegative ? -lSinWithoutSign : lSinWithoutSign);
        }

        /**
        * @param[in] inX
        * Absolute value of the argument.
        *
        * @return cos((inIsNegative ? -1 : +1)*inX)
        */
        template<typename T>
        typename std::enable_if<std::is_integral<T>::value, double>::type
        cosImpl(double inX)
        {
            assert(inX >= 0.0);

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
            double lMulToSin = ldt * (2.0 - pml::constants::rational(1, 3)*ldtSqrd);

            const bool lUseSinSumFormula = (lArgIdx & 1);
            if (lUseSinSumFormula)
            {
                std::swap(lMulToSin, lMulToCos);
                lMulToSin = -lMulToSin;
            }

            auto lIdx = static_cast<int>(ln);
            lIdx += lIdx;

            const double lCosWithoutSign = (gDouble.mSinCosTable[lIdx + 1] * lMulToCos - gDouble.mSinCosTable[lIdx] * lMulToSin);

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

    void expv(const double* inX, double* outY, std::size_t inSize)
    {
        if (CPUDispatcher::isAVX2())
        {
            const __m256d lExpMin256 = _mm256_set1_pd(-708.39641853226408);
            const __m256d lExpMax256 = _mm256_set1_pd(709.78271289338397);
            const __m256d lAlpha256 = _mm256_set1_pd(gDouble.mExpAlpha);
            const __m256d lAlphaInv256 = _mm256_set1_pd(gDouble.mExpAlphaInv);
            const __m256d lRounder256 = _mm256_set1_pd(gDouble.mRounder);

            const __m256i lExpAdj256 = _mm256_set1_epi64x((int)gDouble.mExpAdj);
            const __m256i lMask256 = _mm256_set1_epi64x((int)gDouble.mGuide);

            const __m256d lLeading = _mm256_set1_pd(1.0);
            const __m256d l2ndFactor = _mm256_set1_pd(3.0000000027955394);
            const __m256d l3rdCoeff = _mm256_set1_pd(0.16666666685227835);

            const std::size_t l256End = (inSize - (inSize & 3));
            for (std::size_t i = 0;i < l256End;i += 4)
            {
                __m256d lX256 = _mm256_load_pd(&inX[i]);
                lX256 = _mm256_max_pd(lExpMin256, _mm256_min_pd(lExpMax256, lX256));

                const __m256d ldi = _mm256_add_pd(_mm256_sub_pd(lAlpha256, lX256), lRounder256);

                auto u = _mm256_castpd_si256(ldi);
                __m256i lPower1 = _mm256_srli_epi64(u /*_mm256_castpd_si256(ldi)*/, 11/*(int)gDouble.mExpBit*/);
                lPower1 = _mm256_add_epi64(lPower1, lExpAdj256);
                lPower1 = _mm256_slli_epi64(lPower1, 52/*(int)gDouble.mFractionBit*/);

                const __m256i lResidue = _mm256_and_si256(_mm256_castpd_si256(ldi), lMask256);
                const __m256i lPower2 = _mm256_i64gather_epi64((const long long*)gDouble.mExpTable, lResidue, 8);

                const __m256d lt = _mm256_sub_pd(_mm256_mul_pd(_mm256_sub_pd(ldi, lRounder256), lAlphaInv256), lX256);
                __m256d lTaylorPart = _mm256_mul_pd(_mm256_sub_pd(lt, l2ndFactor), _mm256_mul_pd(lt, lt));
                lTaylorPart = _mm256_mul_pd(lTaylorPart, l3rdCoeff);
                lTaylorPart = _mm256_add_pd(lLeading, _mm256_sub_pd(lTaylorPart, lt));

                const __m256i lPower = _mm256_or_si256(lPower1, lPower2);

                _mm256_store_pd(outY, _mm256_mul_pd(lTaylorPart, _mm256_castsi256_pd(lPower)));
            }

            for (std::size_t i = l256End; i < inSize; ++i)
            {
                outY[i] = pml::exp(inX[i]);
            }
        }
        else if (CPUDispatcher::isAVX())
        {
            // ToDo.
            for (std::size_t i = 0;i < inSize;++i)
            {
                outY[i] = pml::exp(inX[i]);
            }
        }

        for (std::size_t i = 0;i < inSize;++i)
        {
            outY[i] = pml::exp(inX[i]);
        }
    }

    double sin(double inX)
    {
        const bool lIsNegative = (inX < 0);
        inX = std::fabs(inX);

        return (inX < gSinCosMaxArgChar  ) ? sinImpl<char>    (inX, lIsNegative):
               (inX < gSinCosMaxArgShort ) ? sinImpl<short>   (inX, lIsNegative):
               (inX < gSinCosMaxArgInt   ) ? sinImpl<int>     (inX, lIsNegative):
               (inX < gSinCosMaxArgUInt64) ? sinImpl<uint64_t>(inX, lIsNegative):
                                             std::sin(inX);
    }

    double cos(double inX)
    {
        inX = std::fabs(inX);

        return (inX < gSinCosMaxArgChar  ) ? cosImpl<char>    (inX):
               (inX < gSinCosMaxArgShort ) ? cosImpl<short>   (inX):
               (inX < gSinCosMaxArgInt   ) ? cosImpl<int>     (inX):
               (inX < gSinCosMaxArgUInt64) ? cosImpl<uint64_t>(inX):
                                             std::cos(inX);
    }

} // pml
