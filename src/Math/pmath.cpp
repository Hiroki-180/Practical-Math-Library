#include <PML/Math/pmath.h>

namespace pml {

    namespace {

        union di {
            double _double;
            uint64_t _uint64_t;
        };

        template<int EXPBIT, int FRACTIONBIT>
        struct IEEE754ExpTable final
        {
            const uint64_t mExpBit = EXPBIT;
            const uint64_t mExpAdj = ((1UL << (EXPBIT - 1)) - 1);

            const uint64_t mFractionBit = FRACTIONBIT;

            uint64_t mTable[1UL << EXPBIT];
            const uint64_t mGuide = ((1UL << EXPBIT) - 1);

            const double mRounder  = (3ULL << (FRACTIONBIT - 1));
            const double mAlpha    = (1UL << EXPBIT) / std::log(2.0);
            const double mAlphaInv = std::log(2.0) / (1UL << EXPBIT);

            constexpr IEEE754ExpTable() : mTable()
            {
                for (int i = 0;i < (1UL << EXPBIT);++i)
                {
                    di ldi;
                    ldi._double = std::pow(2.0, i / static_cast<double>(1UL << EXPBIT));
                    mTable[i] = (ldi._uint64_t & ((1ULL << FRACTIONBIT) - 1));
                }
            }
        };

        static const IEEE754ExpTable<11, 52> gExpTblDbl;
    }

    double exp(double inX)
    {
        di ldi;
        ldi._double = inX * gExpTblDbl.mAlpha + gExpTblDbl.mRounder;

        const uint64_t lPower1 = ((ldi._uint64_t >> gExpTblDbl.mExpBit) + gExpTblDbl.mExpAdj) << gExpTblDbl.mFractionBit;
        const uint64_t lPower2 = gExpTblDbl.mTable[ldi._uint64_t & gExpTblDbl.mGuide];

        const double lt = ((ldi._double - gExpTblDbl.mRounder)*gExpTblDbl.mAlphaInv - inX);
        const double lTayorPart = 1 - lt + (3.0000000027955394 - lt)*lt*lt*0.16666666685227835;

        ldi._uint64_t = (lPower1 | lPower2);

        return lTayorPart * ldi._double;
    }
}