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

		bool isSSE() { return detail::gCPUData.f_1_EDX_[25]; }
		bool isSSE2() { return detail::gCPUData.f_1_EDX_[26]; }
		bool isSSE3() { return detail::gCPUData.f_1_ECX_[0]; }
        bool isSSSE3() { return detail::gCPUData.f_1_ECX_[9]; }
		bool isSSE41() { return detail::gCPUData.f_1_ECX_[19]; }
		bool isSSE42() { return detail::gCPUData.f_1_ECX_[20]; }

        bool isFMA() { return detail::gCPUData.f_1_ECX_[12]; }
        bool isAVX() { return detail::gCPUData.f_1_ECX_[28]; }
        bool isAVX2() { return detail::gCPUData.f_7_EBX_[5]; }

        bool isAVX512F() { return detail::gCPUData.f_7_EBX_[16]; }
        bool isAVX512PF() { return detail::gCPUData.f_7_EBX_[26]; }
        bool isAVX512ER() { return detail::gCPUData.f_7_EBX_[27]; }
        bool isAVX512CD() { return detail::gCPUData.f_7_EBX_[28]; }

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

            support_message("SSE", CPUDispatcher::isSSE());
            support_message("SSE2", CPUDispatcher::isSSE2());
            support_message("SSE3", CPUDispatcher::isSSE3());
			support_message("SSSE3", CPUDispatcher::isSSSE3());
			support_message("SSE4.1", CPUDispatcher::isSSE41());
            support_message("SSE4.2", CPUDispatcher::isSSE42());

			support_message("FMA", CPUDispatcher::isFMA());
			support_message("AVX", CPUDispatcher::isAVX());
			support_message("AVX2", CPUDispatcher::isAVX2());

			support_message("AVX512F", CPUDispatcher::isAVX512F());
			support_message("AVX512PF", CPUDispatcher::isAVX512PF());
			support_message("AVX512ER", CPUDispatcher::isAVX512ER());
			support_message("AVX512CD", CPUDispatcher::isAVX512CD());

        }

    } // CPUDispatcher
} // pml