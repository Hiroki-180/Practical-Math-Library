#ifndef CORE_CPUDETECTOR_H
#define CORE_CPUDETECTOR_H

#pragma once

#include <PML/Core/NamespaceMacros.h>
  
#include <iostream>
#include <vector>
#include <bitset>
#include <array>
#include <string>
#include <intrin.h>

PML_NS_BEGIN

/**
* The following class is a copy of example code shown in
*   https://msdn.microsoft.com/ja-jp/library/hskdteyh.aspx .
*/
class CPUInfo final
{  
    class CPUDetector;
  
public:  
    static std::string getVendor(void) { return CPU_Rep.vendor_; }  
    static std::string getBrand(void) { return CPU_Rep.brand_; }  
  
    static bool isSSE3(void) { return CPU_Rep.f_1_ECX_[0]; }  
    static bool isPCLMULQDQ(void) { return CPU_Rep.f_1_ECX_[1]; }  
    static bool isMONITOR(void) { return CPU_Rep.f_1_ECX_[3]; }  
    static bool isSSSE3(void) { return CPU_Rep.f_1_ECX_[9]; }  
    static bool isFMA(void) { return CPU_Rep.f_1_ECX_[12]; }  
    static bool isCMPXCHG16B(void) { return CPU_Rep.f_1_ECX_[13]; }  
    static bool isSSE41(void) { return CPU_Rep.f_1_ECX_[19]; }  
    static bool isSSE42(void) { return CPU_Rep.f_1_ECX_[20]; }  
    static bool isMOVBE(void) { return CPU_Rep.f_1_ECX_[22]; }  
    static bool isPOPCNT(void) { return CPU_Rep.f_1_ECX_[23]; }  
    static bool isAES(void) { return CPU_Rep.f_1_ECX_[25]; }  
    static bool isXSAVE(void) { return CPU_Rep.f_1_ECX_[26]; }  
    static bool isOSXSAVE(void) { return CPU_Rep.f_1_ECX_[27]; }  
    static bool isAVX(void) { return CPU_Rep.f_1_ECX_[28]; }  
    static bool isF16C(void) { return CPU_Rep.f_1_ECX_[29]; }  
    static bool isRDRAND(void) { return CPU_Rep.f_1_ECX_[30]; }  
  
    static bool isMSR(void) { return CPU_Rep.f_1_EDX_[5]; }  
    static bool isCX8(void) { return CPU_Rep.f_1_EDX_[8]; }  
    static bool isSEP(void) { return CPU_Rep.f_1_EDX_[11]; }  
    static bool isCMOV(void) { return CPU_Rep.f_1_EDX_[15]; }  
    static bool isCLFSH(void) { return CPU_Rep.f_1_EDX_[19]; }  
    static bool isMMX(void) { return CPU_Rep.f_1_EDX_[23]; }  
    static bool isFXSR(void) { return CPU_Rep.f_1_EDX_[24]; }  
    static bool isSSE(void) { return CPU_Rep.f_1_EDX_[25]; }  
    static bool isSSE2(void) { return CPU_Rep.f_1_EDX_[26]; }  
  
    static bool isFSGSBASE(void) { return CPU_Rep.f_7_EBX_[0]; }  
    static bool isBMI1(void) { return CPU_Rep.f_7_EBX_[3]; }  
    static bool isHLE(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[4]; }  
    static bool isAVX2(void) { return CPU_Rep.f_7_EBX_[5]; }  
    static bool isBMI2(void) { return CPU_Rep.f_7_EBX_[8]; }  
    static bool isERMS(void) { return CPU_Rep.f_7_EBX_[9]; }  
    static bool isINVPCID(void) { return CPU_Rep.f_7_EBX_[10]; }  
    static bool isRTM(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[11]; }  
    static bool isAVX512F(void) { return CPU_Rep.f_7_EBX_[16]; }  
    static bool isRDSEED(void) { return CPU_Rep.f_7_EBX_[18]; }  
    static bool isADX(void) { return CPU_Rep.f_7_EBX_[19]; }  
    static bool isAVX512PF(void) { return CPU_Rep.f_7_EBX_[26]; }  
    static bool isAVX512ER(void) { return CPU_Rep.f_7_EBX_[27]; }  
    static bool isAVX512CD(void) { return CPU_Rep.f_7_EBX_[28]; }  
    static bool isSHA(void) { return CPU_Rep.f_7_EBX_[29]; }  
  
    static bool isPREFETCHWT1(void) { return CPU_Rep.f_7_ECX_[0]; }  
  
    static bool isLAHF(void) { return CPU_Rep.f_81_ECX_[0]; }  
    static bool isLZCNT(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_ECX_[5]; }  
    static bool isABM(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[5]; }  
    static bool isSSE4a(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[6]; }  
    static bool isXOP(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[11]; }  
    static bool isTBM(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[21]; }  
  
    static bool isSYSCALL(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[11]; }  
    static bool isMMXEXT(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[22]; }  
    static bool isRDTSCP(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[27]; }  
    static bool is3DNOWEXT(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[30]; }  
    static bool is3DNOW(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[31]; }  
  
private:  
    static const CPUDetector CPU_Rep;  
  
    class CPUDetector final
    {  
    public:  
		CPUDetector()
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
};  

// Initialize static member data  
const CPUInfo::CPUDetector CPUInfo::CPU_Rep;

/**
* Print out supported instruction set extensions
*/
void outputCPUInfo(std::ostream& outStream)
{
	outStream << "CPU Information" << std::endl;
	outStream << CPUInfo::getVendor() << std::endl;
	outStream << CPUInfo::getBrand()  << std::endl;
 
	const auto support_message = [&outStream](const std::string& isa_feature, const bool is_supported) {
		outStream << isa_feature << "," << (is_supported ? " supported" : " not supported") << std::endl;
	};

    support_message("3DNOW",       CPUInfo::is3DNOW());
    support_message("3DNOWEXT",    CPUInfo::is3DNOWEXT());
    support_message("ABM",         CPUInfo::isABM());
    support_message("ADX",         CPUInfo::isADX());
    support_message("AES",         CPUInfo::isAES());
    support_message("AVX",         CPUInfo::isAVX());
    support_message("AVX2",        CPUInfo::isAVX2());
    support_message("AVX512CD",    CPUInfo::isAVX512CD());
    support_message("AVX512ER",    CPUInfo::isAVX512ER());
    support_message("AVX512F",     CPUInfo::isAVX512F());
    support_message("AVX512PF",    CPUInfo::isAVX512PF());
    support_message("BMI1",        CPUInfo::isBMI1());
    support_message("BMI2",        CPUInfo::isBMI2());
    support_message("CLFSH",       CPUInfo::isCLFSH());
    support_message("CMPXCHG16B",  CPUInfo::isCMPXCHG16B());
    support_message("CX8",         CPUInfo::isCX8());
    support_message("ERMS",        CPUInfo::isERMS());
    support_message("F16C",        CPUInfo::isF16C());
    support_message("FMA",         CPUInfo::isFMA());
    support_message("FSGSBASE",    CPUInfo::isFSGSBASE());
    support_message("FXSR",        CPUInfo::isFXSR());
    support_message("HLE",         CPUInfo::isHLE());
    support_message("INVPCID",     CPUInfo::isINVPCID());
    support_message("LAHF",        CPUInfo::isLAHF());
    support_message("LZCNT",       CPUInfo::isLZCNT());
    support_message("MMX",         CPUInfo::isMMX());
    support_message("MMXEXT",      CPUInfo::isMMXEXT());
    support_message("MONITOR",     CPUInfo::isMONITOR());
    support_message("MOVBE",       CPUInfo::isMOVBE());
    support_message("MSR",         CPUInfo::isMSR());
    support_message("OSXSAVE",     CPUInfo::isOSXSAVE());
    support_message("PCLMULQDQ",   CPUInfo::isPCLMULQDQ());
    support_message("POPCNT",      CPUInfo::isPOPCNT());
    support_message("PREFETCHWT1", CPUInfo::isPREFETCHWT1());
    support_message("RDRAND",      CPUInfo::isRDRAND());
    support_message("RDSEED",      CPUInfo::isRDSEED());
    support_message("RDTSCP",      CPUInfo::isRDTSCP());
    support_message("RTM",         CPUInfo::isRTM());
    support_message("SEP",         CPUInfo::isSEP());
    support_message("SHA",         CPUInfo::isSHA());
    support_message("SSE",         CPUInfo::isSSE());
    support_message("SSE2",        CPUInfo::isSSE2());
    support_message("SSE3",        CPUInfo::isSSE3());
    support_message("SSE4.1",      CPUInfo::isSSE41());
    support_message("SSE4.2",      CPUInfo::isSSE42());
    support_message("SSE4a",       CPUInfo::isSSE4a());
    support_message("SSSE3",       CPUInfo::isSSSE3());
    support_message("SYSCALL",     CPUInfo::isSYSCALL());
    support_message("TBM",         CPUInfo::isTBM());
    support_message("XOP",         CPUInfo::isXOP());
    support_message("XSAVE",       CPUInfo::isXSAVE());
}

PML_NS_END

#endif