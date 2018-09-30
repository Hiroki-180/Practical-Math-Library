#ifndef CORE_CPUDISPATCHER_H
#define CORE_CPUDISPATCHER_H

#pragma once

/**
* @file
* public header provided by PML.
*
* @brief
* CPU Dispatcher.
*/

#include <ostream>
#include <string>

namespace pml {

    /**
    * @namespace CPUDispatcher
    *
    * @brief
    * Useful functions are provided for runtime CPU dispatching.
    * For instance, isSSEXX()/isAVX()/isAVX2()/isAVX512F() enebles the run-time dispatch of which SIMD verion is supported on your CPU.
    */
    namespace CPUDispatcher {

        /**
        * @brief
        * Getting vender name of runtime CPU, GenuinIntel, ...
        */
        const std::string& getVendor();

        /**
        * @brief
        * Getting brand name of runtime CPU, Intel Core(TM) i7-XXX...
        */
        const std::string& getBrand();

        /**
        * Is SSE supported ?
        */
        bool isSSE();

        /**
        * Is SSE supported ?
        */
        bool isSSE2();

        /**
        * Is SSE2 supported ?
        */
        bool isSSE3();

        /**
        * Is SSE3 supported ?
        */
        bool isSSSE3();

        /**
        * Is SSE4.1 supported ?
        */
        bool isSSE41();

        /**
        * Is SSE4.2 supported ?
        */
        bool isSSE42();

        /**
        * Is FMA supported ?
        */
        bool isFMA();

        /**
        * Is AVX supported ?
        */
        bool isAVX();

        /**
        * Is AVX2 supported ?
        */
        bool isAVX2();

        /**
        * Are standard AVX512 instructions supported ?
        */
        bool isAVX512F();

        /**
        * Is AVX512PF supported ?
        */
        bool isAVX512PF();

        /**
        * Is AVX512ER supported ?
        */
        bool isAVX512ER();

        /**
        * Is AVX512CD supported ?
        */
        bool isAVX512CD();

        /**
        * Output supported instruction set extensions.
        *
        * @param[out] outStream
        * Supported instruction set extensions.
        */
        void outputCPUInfo(std::ostream& outStream);

    } // CPUDispatcher
} //pml

#endif