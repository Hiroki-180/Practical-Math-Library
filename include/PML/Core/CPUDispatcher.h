#ifndef CORE_CPUDISPATCHER_H
#define CORE_CPUDISPATCHER_H

#pragma once
  
#include <iostream>
#include <vector>
#include <bitset>
#include <array>
#include <string>

namespace pml {

    /**
    * @namespace CPUDispatcher
    *
    * @brief
    * Useful functions are prepared for CPU dispatching.
    * For instance, isSSE() / isSSE2() / isSSE3() / isSSE41() / isSSE42() / isAVX() / isAVX512F() enebles the run-time dispatch of which SIMD verion is supported on your CPU.
    */
    namespace CPUDispatcher {

        const std::string& getVendor();
        const std::string& getBrand();

		bool isSSE();
		bool isSSE2();
		bool isSSE3();
        bool isSSSE3();
        bool isSSE41();
        bool isSSE42();

		bool isFMA();
		bool isAVX();
		bool isAVX2();
        
		bool isAVX512F();
        bool isAVX512PF();
        bool isAVX512ER();
        bool isAVX512CD();

        void outputCPUInfo(std::ostream& outStream);

        namespace detail {

            class CPUData final
            {
            public:
                CPUData();

                int nIds_;
                int nExIds_;
                std::string vendor_;
                std::string brand_;
                bool isIntel_;
                bool isAMD_;
                std::bitset<32> f_1_ECX_;
                std::bitset<32> f_1_EDX_;
                std::bitset<32> f_7_EBX_;
                std::bitset<32> f_7_ECX_;
                std::vector<std::array<int, 4>> data_;
                std::vector<std::array<int, 4>> extdata_;
            };
        }

    } // CPUDispatcher
} //pml

#endif