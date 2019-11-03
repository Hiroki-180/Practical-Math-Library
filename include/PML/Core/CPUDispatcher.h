#ifndef CORE_CPUDISPATCHER_H
#define CORE_CPUDISPATCHER_H

/**
* @file public header provided by PML.
*
* @brief CPU Dispatcher.
*/

#include <PML/Core/cross_intrin.h>

#include <ostream>
#include <string>
#include <vector>
#include <array>
#include <bitset>

#ifndef _MSC_VER
#include <cpuid.h>
#endif

namespace pml {

    /**
    * @namespace CPUDispatcher
    *
    * @brief Useful functions are provided for the runtime CPU dispatching.
    *        For instance, isSSEXX()/isAVX()/isAVX2()/isAVX512F() enebles the run-time dispatch of which SIMD verion is supported on your CPU.
    */
    class CPUDispatcher final
    {
        class CPUData final
        {
        public:
            int nIds_;
            int nExIds_;

            std::string vendor_;
            std::string brand_;
            std::bitset<32> f_1_ECX_;
            std::bitset<32> f_1_EDX_;
            std::bitset<32> f_7_EBX_;

            std::vector<std::array<int, 4>> data_;
            std::vector<std::array<int, 4>> extdata_;
            std::size_t mOptimalAlignment;

            CPUData(const CPUData&)            = delete;
            CPUData(CPUData&&)                 = delete;
            CPUData& operator=(const CPUData&) = delete;
            CPUData& operator=(CPUData&&)      = delete;

            ~CPUData() = default;

            CPUData()
                : nIds_{ 0 },
                nExIds_{ 0 },
                f_1_ECX_{ 0 },
                f_1_EDX_{ 0 },
                f_7_EBX_{ 0 },
                data_{},
                extdata_{},
                mOptimalAlignment(16)
            {
#ifdef _MSC_VER
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
                *reinterpret_cast<int*>(vendor    ) = data_[0][1];
                *reinterpret_cast<int*>(vendor + 4) = data_[0][3];
                *reinterpret_cast<int*>(vendor + 8) = data_[0][2];
                vendor_ = vendor;

                // load bitset with flags for function 0x00000001
                if (nIds_ >= 1)
                {
                    f_1_ECX_ = data_[1][2];
                    f_1_EDX_ = data_[1][3];
                }

                // load bitset with flags for function 0x00000007
                if (nIds_ >= 7){
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
#else
                {
                    union {
                        char text[16];
                        uint32_t reg[4];
                    } vender = { .text = {0} };
                    uint32_t idmax = 0;

                    // id=0: idmax=eax, VenderID:ebx.edx.ecx
                    __cpuid(0, idmax, vender.reg[0], vender.reg[2], vender.reg[1]);

                    char vendor[0x20];
                    memset(vendor, 0, sizeof(vendor));
                    *reinterpret_cast<int*>(vendor) = vender.reg[0];
                    *reinterpret_cast<int*>(vendor + 4) = vender.reg[1];
                    *reinterpret_cast<int*>(vendor + 8) = vender.reg[2];
                    vendor_ = vendor;
                }

                {
                    union {
                        struct {
                            uint8_t step : 4;
                            uint8_t model : 4;
                            uint8_t family : 4;
                            uint8_t type : 2;
                            uint8_t pad1 : 2;
                            uint8_t emodel : 4;
                            uint8_t efamily : 8;
                            uint8_t pad2 : 4;
                        };
                        uint32_t reg;
                    } eax = { .reg = 1 };

                    uint32_t ebx = 0;
                    __cpuid(1, eax.reg, ebx, f_1_ECX_, f_1_EDX_);
                }

                {
                    union {
                        struct {
                            uint8_t prefetchwt1 : 1;
                            uint8_t avx512vbmi : 1;
                            uint32_t reserved : 30;
                        };
                        uint32_t reg;
                    } ecx = { .reg = 0 };

                    uint32_t edx = 0;
                    uint32_t eax = 7;

                    __cpuid(7, eax, f_7_EBX_, ecx.reg, edx);
                }
#endif
                mOptimalAlignment = f_7_EBX_[16] ? 64 :
                                    f_7_EBX_[ 5] ? 32 :
                                    f_1_ECX_[28] ? 32 :
                                                   16;
            }
        };

        static const CPUData& getCPUData()
        {
            static const CPUData cpuData;

            return cpuData;
        }

    public:

        /**
        * @brief Getting vender name of runtime CPU, GenuinIntel, etc.
        */
        static const std::string& getVendor()
        {
            return getCPUData().vendor_;
        }

        /**
        * @brief Getting brand name of runtime CPU, Intel Core(TM) i7-XXX, etc.
        */
        static const std::string& getBrand()
        {
            return getCPUData().brand_;
        }

        /**
        * @brief Is SSE supported ?
        */
        static bool isSSE()
        {
            return getCPUData().f_1_EDX_[25];
        }

        /**
        * @brief Is SSE2 supported ?
        */
        static bool isSSE2()
        {
            return getCPUData().f_1_EDX_[26];
        }

        /**
        * @brief Is SSE3 supported ?
        */
        static bool isSSE3()
        {
            return getCPUData().f_1_ECX_[0];
        }

        /**
        * @brief Is SSSE3 supported ?
        */
        static bool isSSSE3()
        {
            return getCPUData().f_1_ECX_[9];
        }

        /**
        * @brief Is SSE4.1 supported ?
        */
        static bool isSSE41()
        {
            return getCPUData().f_1_ECX_[19];
        }

        /**
        * @brief Is SSE4.2 supported ?
        */
        static bool isSSE42()
        {
            return getCPUData().f_1_ECX_[20];
        }

        /**
        * @brief Is FMA supported ?
        */
        static bool isFMA()
        {
            return getCPUData().f_1_ECX_[12];
        }

        /**
        * @brief Is AVX supported ?
        */
        static bool isAVX()
        {
            return getCPUData().f_1_ECX_[28];
        }

        /**
        * @brief Is AVX2 supported ?
        */
        static bool isAVX2()
        {
            return getCPUData().f_7_EBX_[5];
        }

        /**
        * @brief Are standard AVX512 instructions supported ?
        */
        static bool isAVX512F()
        {
            return getCPUData().f_7_EBX_[16];
        }

        /**
        * @brief Is AVX512PF supported ?
        */
        static bool isAVX512PF()
        {
            return getCPUData().f_7_EBX_[26];
        }

        /**
        * @brief Is AVX512ER supported ?
        */
        static bool isAVX512ER()
        {
            return getCPUData().f_7_EBX_[27];
        }

        /**
        * @brief Is AVX512CD supported ?
        */
        static bool isAVX512CD()
        {
            return getCPUData().f_7_EBX_[28];
        }

        /**
        * @brief Get proper memory alignment size for the optimal SIMD of the runtime CPU .
        */
        static std::size_t getOptimalAlignment()
        {
            return getCPUData().mOptimalAlignment;
        }

        /**
        * @brief Output supported instruction set extensions of the runtime CPU architecture.
        *
        * @param[out] outStream
        * Supported instruction set extensions.
        */
        static void outputCPUInfo(std::ostream& outStream)
        {
            outStream << "CPU Information" << std::endl;
            outStream << CPUDispatcher::getVendor() << std::endl;
            outStream << CPUDispatcher::getBrand() << std::endl;

            const auto support_message = [&outStream](const std::string& isa_feature, bool is_supported) {
                outStream << isa_feature << ":" << (is_supported ? " supported" : " not supported") << std::endl;
            };

            support_message("SSE   ", CPUDispatcher::isSSE());
            support_message("SSE2  ", CPUDispatcher::isSSE2());
            support_message("SSE3  ", CPUDispatcher::isSSE3());
            support_message("SSSE3 ", CPUDispatcher::isSSSE3());
            support_message("SSE4.1", CPUDispatcher::isSSE41());
            support_message("SSE4.2", CPUDispatcher::isSSE42());

            support_message("FMA   ", CPUDispatcher::isFMA());
            support_message("AVX   ", CPUDispatcher::isAVX());
            support_message("AVX2  ", CPUDispatcher::isAVX2());

            support_message("AVX512F ", CPUDispatcher::isAVX512F());
            support_message("AVX512PF", CPUDispatcher::isAVX512PF());
            support_message("AVX512ER", CPUDispatcher::isAVX512ER());
            support_message("AVX512CD", CPUDispatcher::isAVX512CD());
        }
    
    }; // CPUDispatcher
} //pml

#endif