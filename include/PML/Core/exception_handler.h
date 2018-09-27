#ifndef CORE_EXCEPTION_HANDLER_H
#define CORE_EXCEPTION_HANDLER_H

#pragma once

/**
* @file
* public header provided by PML.
*
* @brief
* Useful macros for handling exceptions.
*/

#include <exception>
#include <fstream>

/**
* @def
* Throw std::nested_exception by std::throw_with_nested( TYPE(MESSAGE with info. of __FILE and __LINE__) ).
* PML throw exceptions using this macro if TYPE has a single string argument constructor.
*/
#define PML_THROW_WITH_NESTED( TYPE, MESSAGE ) pml::detail::throw_with_nested<TYPE>(MESSAGE, __FILE__, __LINE__)

/**
* @def
* Macro to begin the try-brock.
* This macro should be used with PML_CATCH_END_AND_PRINT(OFSTREAM) or PML_CATCH_END_AND_THROW(TYPE, MESSAGE).
*/
#define PML_CATCH_BEGIN try{

/**
* @def
* Macro to finish the try-brock and output error messges to OFSTREAM.
* std::logic_error, std::runtime_error, and other exceptions are distinguished in message.
* This macro should be used with PML_CATCH_BEGIN.
*/
#define PML_CATCH_END_AND_PRINT( OFSTREAM )   } catch(...) { pml::detail::print_exception(OFSTREAM); }

/**
* @def
* Macro to finish the try-brock and add new exception by std::throw_with_nested(TYPE(MESSAGE)).
* This macro should be used with PML_CATCH_BEGIN.
*/
#define PML_CATCH_END_AND_THROW( TYPE, MESSAGE )   } catch(...) { PML_THROW_WITH_NESTED( TYPE, MESSAGE ); }


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

#endif