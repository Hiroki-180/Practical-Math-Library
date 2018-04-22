#include <PML/Core/cross_intrin.h>
#include <PML/Core/CPUDispatcher.h>

namespace pml {
    namespace CPUDispatcher {

        namespace detail {

            CPUData::CPUData()
                : nIds_{ 0 },
                nExIds_{ 0 },
                isIntel_{ false },
                isAMD_{ false },
                f_1_ECX_{ 0 },
                f_1_EDX_{ 0 },
                f_7_EBX_{ 0 },
                f_7_ECX_{ 0 },
                f_81_ECX_{ 0 },
                f_81_EDX_{ 0 },
                data_{},
                extdata_{}
            {
                //int cpuInfo[4] = {-1};
                std::array<int, 4> cpui;

                // Calling __cpuid with 0x0 as the function_id argument
                // gets the number of the highest valid function ID.
                __cpuid(cpui.data(), 0);
                nIds_ = cpui[0];

                for (int i = 0; i <= nIds_; ++i)
                {
                    __cpuidex(cpui.data(), i, 0);
                    data_.push_back(cpui);
                }

                // Capture vendor string
                char vendor[0x20];
                memset(vendor, 0, sizeof(vendor));
                *reinterpret_cast<int*>(vendor) = data_[0][1];
                *reinterpret_cast<int*>(vendor + 4) = data_[0][3];
                *reinterpret_cast<int*>(vendor + 8) = data_[0][2];
                vendor_ = vendor;
                if (vendor_ == "GenuineIntel")
                {
                    isIntel_ = true;
                }
                else if (vendor_ == "AuthenticAMD")
                {
                    isAMD_ = true;
                }

                // load bitset with flags for function 0x00000001
                if (nIds_ >= 1)
                {
                    f_1_ECX_ = data_[1][2];
                    f_1_EDX_ = data_[1][3];
                }

                // load bitset with flags for function 0x00000007
                if (nIds_ >= 7)
                {
                    f_7_EBX_ = data_[7][1];
                    f_7_ECX_ = data_[7][2];
                }

                // Calling __cpuid with 0x80000000 as the function_id argument
                // gets the number of the highest valid extended ID.
                __cpuid(cpui.data(), 0x80000000);
                nExIds_ = cpui[0];

                char brand[0x40];
                memset(brand, 0, sizeof(brand));

                for (int i = 0x80000000; i <= nExIds_; ++i)
                {
                    __cpuidex(cpui.data(), i, 0);
                    extdata_.push_back(cpui);
                }

                // load bitset with flags for function 0x80000001
                if (nExIds_ >= 0x80000001)
                {
                    f_81_ECX_ = extdata_[1][2];
                    f_81_EDX_ = extdata_[1][3];
                }

                // Interpret CPU brand string if reported
                if (nExIds_ >= 0x80000004)
                {
                    memcpy(brand, extdata_[2].data(), sizeof(cpui));
                    memcpy(brand + 16, extdata_[3].data(), sizeof(cpui));
                    memcpy(brand + 32, extdata_[4].data(), sizeof(cpui));
                    brand_ = brand;
                }
            };

            static const CPUData gCPUData;

        } // detail

        const std::string& getVendor() { return detail::gCPUData.vendor_; }
        const std::string& getBrand() { return detail::gCPUData.brand_; }

        bool isSSE3() { return detail::gCPUData.f_1_ECX_[0]; }
        bool isPCLMULQDQ() { return detail::gCPUData.f_1_ECX_[1]; }
        bool isMONITOR() { return detail::gCPUData.f_1_ECX_[3]; }
        bool isSSSE3() { return detail::gCPUData.f_1_ECX_[9]; }
        bool isFMA() { return detail::gCPUData.f_1_ECX_[12]; }
        bool isCMPXCHG16B() { return detail::gCPUData.f_1_ECX_[13]; }
        bool isSSE41() { return detail::gCPUData.f_1_ECX_[19]; }
        bool isSSE42() { return detail::gCPUData.f_1_ECX_[20]; }
        bool isMOVBE() { return detail::gCPUData.f_1_ECX_[22]; }
        bool isPOPCNT() { return detail::gCPUData.f_1_ECX_[23]; }
        bool isAES() { return detail::gCPUData.f_1_ECX_[25]; }
        bool isXSAVE() { return detail::gCPUData.f_1_ECX_[26]; }
        bool isOSXSAVE() { return detail::gCPUData.f_1_ECX_[27]; }
        bool isAVX() { return detail::gCPUData.f_1_ECX_[28]; }
        bool isF16C() { return detail::gCPUData.f_1_ECX_[29]; }
        bool isRDRAND() { return detail::gCPUData.f_1_ECX_[30]; }

        bool isMSR() { return detail::gCPUData.f_1_EDX_[5]; }
        bool isCX8() { return detail::gCPUData.f_1_EDX_[8]; }
        bool isSEP() { return detail::gCPUData.f_1_EDX_[11]; }
        bool isCMOV() { return detail::gCPUData.f_1_EDX_[15]; }
        bool isCLFSH() { return detail::gCPUData.f_1_EDX_[19]; }
        bool isMMX() { return detail::gCPUData.f_1_EDX_[23]; }
        bool isFXSR() { return detail::gCPUData.f_1_EDX_[24]; }
        bool isSSE() { return detail::gCPUData.f_1_EDX_[25]; }
        bool isSSE2() { return detail::gCPUData.f_1_EDX_[26]; }

        bool isFSGSBASE() { return detail::gCPUData.f_7_EBX_[0]; }
        bool isBMI1() { return detail::gCPUData.f_7_EBX_[3]; }
        bool isHLE() { return (detail::gCPUData.isIntel_ && detail::gCPUData.f_7_EBX_[4]); }
        bool isAVX2() { return detail::gCPUData.f_7_EBX_[5]; }
        bool isBMI2() { return detail::gCPUData.f_7_EBX_[8]; }
        bool isERMS() { return detail::gCPUData.f_7_EBX_[9]; }
        bool isINVPCID() { return detail::gCPUData.f_7_EBX_[10]; }
        bool isRTM() { return (detail::gCPUData.isIntel_ && detail::gCPUData.f_7_EBX_[11]); }
        bool isAVX512F() { return detail::gCPUData.f_7_EBX_[16]; }
        bool isRDSEED() { return detail::gCPUData.f_7_EBX_[18]; }
        bool isADX() { return detail::gCPUData.f_7_EBX_[19]; }
        bool isAVX512PF() { return detail::gCPUData.f_7_EBX_[26]; }
        bool isAVX512ER() { return detail::gCPUData.f_7_EBX_[27]; }
        bool isAVX512CD() { return detail::gCPUData.f_7_EBX_[28]; }
        bool isSHA() { return detail::gCPUData.f_7_EBX_[29]; }

        bool isPREFETCHWT1() { return detail::gCPUData.f_7_ECX_[0]; }

        bool isLAHF() { return detail::gCPUData.f_81_ECX_[0]; }
        bool isLZCNT() { return (detail::gCPUData.isIntel_ && detail::gCPUData.f_81_ECX_[5]); }
        bool isABM() { return (detail::gCPUData.isAMD_ && detail::gCPUData.f_81_ECX_[5]); }
        bool isSSE4a() { return (detail::gCPUData.isAMD_ && detail::gCPUData.f_81_ECX_[6]); }
        bool isXOP() { return (detail::gCPUData.isAMD_ && detail::gCPUData.f_81_ECX_[11]); }
        bool isTBM() { return (detail::gCPUData.isAMD_ && detail::gCPUData.f_81_ECX_[21]); }

        bool isSYSCALL() { return (detail::gCPUData.isIntel_ && detail::gCPUData.f_81_EDX_[11]); }
        bool isRDTSCP() { return (detail::gCPUData.isIntel_ && detail::gCPUData.f_81_EDX_[27]); }
        bool isMMXEXT() { return (detail::gCPUData.isAMD_ && detail::gCPUData.f_81_EDX_[22]); }
        bool is3DNOWEXT() { return (detail::gCPUData.isAMD_ && detail::gCPUData.f_81_EDX_[30]); }
        bool is3DNOW() { return (detail::gCPUData.isAMD_ && detail::gCPUData.f_81_EDX_[31]); }

        /**
        * Print out supported instruction set extensions
        */
        void outputCPUInfo(std::ostream& outStream)
        {
            outStream << "CPU Information" << std::endl;
            outStream << CPUDispatcher::getVendor() << std::endl;
            outStream << CPUDispatcher::getBrand() << std::endl;

            const auto support_message = [&outStream](const std::string& isa_feature, bool is_supported) {
                outStream << isa_feature << "," << (is_supported ? " supported" : " not supported") << std::endl;
            };

            support_message("3DNOW", CPUDispatcher::is3DNOW());
            support_message("3DNOWEXT", CPUDispatcher::is3DNOWEXT());
            support_message("ABM", CPUDispatcher::isABM());
            support_message("ADX", CPUDispatcher::isADX());
            support_message("AES", CPUDispatcher::isAES());
            support_message("AVX", CPUDispatcher::isAVX());
            support_message("AVX2", CPUDispatcher::isAVX2());
            support_message("AVX512CD", CPUDispatcher::isAVX512CD());
            support_message("AVX512ER", CPUDispatcher::isAVX512ER());
            support_message("AVX512F", CPUDispatcher::isAVX512F());
            support_message("AVX512PF", CPUDispatcher::isAVX512PF());
            support_message("BMI1", CPUDispatcher::isBMI1());
            support_message("BMI2", CPUDispatcher::isBMI2());
            support_message("CLFSH", CPUDispatcher::isCLFSH());
            support_message("CMPXCHG16B", CPUDispatcher::isCMPXCHG16B());
            support_message("CX8", CPUDispatcher::isCX8());
            support_message("ERMS", CPUDispatcher::isERMS());
            support_message("F16C", CPUDispatcher::isF16C());
            support_message("FMA", CPUDispatcher::isFMA());
            support_message("FSGSBASE", CPUDispatcher::isFSGSBASE());
            support_message("FXSR", CPUDispatcher::isFXSR());
            support_message("HLE", CPUDispatcher::isHLE());
            support_message("INVPCID", CPUDispatcher::isINVPCID());
            support_message("LAHF", CPUDispatcher::isLAHF());
            support_message("LZCNT", CPUDispatcher::isLZCNT());
            support_message("MMX", CPUDispatcher::isMMX());
            support_message("MMXEXT", CPUDispatcher::isMMXEXT());
            support_message("MONITOR", CPUDispatcher::isMONITOR());
            support_message("MOVBE", CPUDispatcher::isMOVBE());
            support_message("MSR", CPUDispatcher::isMSR());
            support_message("OSXSAVE", CPUDispatcher::isOSXSAVE());
            support_message("PCLMULQDQ", CPUDispatcher::isPCLMULQDQ());
            support_message("POPCNT", CPUDispatcher::isPOPCNT());
            support_message("PREFETCHWT1", CPUDispatcher::isPREFETCHWT1());
            support_message("RDRAND", CPUDispatcher::isRDRAND());
            support_message("RDSEED", CPUDispatcher::isRDSEED());
            support_message("RDTSCP", CPUDispatcher::isRDTSCP());
            support_message("RTM", CPUDispatcher::isRTM());
            support_message("SEP", CPUDispatcher::isSEP());
            support_message("SHA", CPUDispatcher::isSHA());
            support_message("SSE", CPUDispatcher::isSSE());
            support_message("SSE2", CPUDispatcher::isSSE2());
            support_message("SSE3", CPUDispatcher::isSSE3());
            support_message("SSE4.1", CPUDispatcher::isSSE41());
            support_message("SSE4.2", CPUDispatcher::isSSE42());
            support_message("SSE4a", CPUDispatcher::isSSE4a());
            support_message("SSSE3", CPUDispatcher::isSSSE3());
            support_message("SYSCALL", CPUDispatcher::isSYSCALL());
            support_message("TBM", CPUDispatcher::isTBM());
            support_message("XOP", CPUDispatcher::isXOP());
            support_message("XSAVE", CPUDispatcher::isXSAVE());
        }

    } // CPUDispatcher
} // pml