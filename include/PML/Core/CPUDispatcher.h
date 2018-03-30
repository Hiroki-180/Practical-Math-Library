#ifndef CORE_CPUDISPATCHER_H
#define CORE_CPUDISPATCHER_H

#pragma once
  
#include <iostream>
#include <vector>
#include <bitset>
#include <array>
#include <string>

namespace pml {
    namespace CPUDispatcher {

        const std::string& getVendor();
        const std::string& getBrand();

        bool isSSE3();
        bool isPCLMULQDQ();
        bool isMONITOR();
        bool isSSSE3();
        bool isFMA();
        bool isCMPXCHG16B();
        bool isSSE41();
        bool isSSE42();
        bool isMOVBE();
        bool isPOPCNT();
        bool isAES();
        bool isXSAVE();
        bool isOSXSAVE();
        bool isAVX();
        bool isF16C();
        bool isRDRAND();

        bool isMSR();
        bool isCX8();
        bool isSEP();
        bool isCMOV();
        bool isCLFSH();
        bool isMMX();
        bool isFXSR();
        bool isSSE();
        bool isSSE2();

        bool isFSGSBASE();
        bool isBMI1();
        bool isHLE();
        bool isAVX2();
        bool isBMI2();
        bool isERMS();
        bool isINVPCID();
        bool isRTM();
        bool isAVX512F();
        bool isRDSEED();
        bool isADX();
        bool isAVX512PF();
        bool isAVX512ER();
        bool isAVX512CD();
        bool isSHA();

        bool isPREFETCHWT1();

        bool isLAHF();
        bool isLZCNT();
        bool isABM();
        bool isSSE4a();
        bool isXOP();
        bool isTBM();

        bool isSYSCALL();
        bool isMMXEXT();
        bool isRDTSCP();
        bool is3DNOWEXT();
        bool is3DNOW();

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
                std::bitset<32> f_81_ECX_;
                std::bitset<32> f_81_EDX_;
                std::vector<std::array<int, 4>> data_;
                std::vector<std::array<int, 4>> extdata_;
            };
        }

    } // CPUDispatcher
} //pml

#endif