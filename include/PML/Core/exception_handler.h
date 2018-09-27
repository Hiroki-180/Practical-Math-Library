#ifndef CORE_EXCEPTION_HANDLER_H
#define CORE_EXCEPTION_HANDLER_H

#pragma once

/**
* @file
* public header provided by PML.
*
* @brief
* Macros for handling exceptions.
*/

#include <exception>
#include <fstream>

namespace pml {
    namespace detail {

        template<class E>
        [[noreturn]] void throw_with_nested(
            std::string&& inMessage,
            char const* inFileName,
            std::size_t inLine)
        {
            std::throw_with_nested(
                E(std::string{} + inFileName + "(" + std::to_string(inLine) + "): " + inMessage + " "));
        };

        void print_std_exception(
            const std::exception& inException,
            std::ofstream& inFile,
            bool inIsFirstCall);

        void print_exception(std::ofstream& inFile);

    } // detail
} // pml

#define PML_THROW_WITH_NESTED( TYPE, MESSAGE ) pml::detail::throw_with_nested<TYPE>(MESSAGE, __FILE__, __LINE__)

#define PML_CATCH_BEGIN try{

#define PML_CATCH_END_AND_PRINT( OFSTREAM )   } catch(...) { pml::detail::print_exception(OFSTREAM); }

#endif