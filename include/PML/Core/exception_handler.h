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
#include <ostream>

/**
* @def
* Macro to throw std::nested_exception by std::throw_with_nested( TYPE(MESSAGE with info. of __FILE__ and __LINE__) ).
* PML always throws exceptions using this macro if TYPE has a Single String Argument constructor.
* PML throws only inheritances of std::exception as exception and does not throw it's original implementations of exceptions.
*/
#define PML_THROW_WITH_NESTED( TYPE, MESSAGE ) pml::detail::throw_with_nested_SSA<TYPE>(MESSAGE, __FILE__, __LINE__)

/**
* @def
* Macro to begin a try-brock.
* This macro should be used with PML_CATCH_END_AND_PRINT(OFSTREAM) or PML_CATCH_END_AND_THROW(TYPE, MESSAGE).
*/
#define PML_CATCH_BEGIN try{

/**
* @def
* Macro to finish a try-brock and output error messges to OSTREAM which must be an inheritance of std::ofstream.
* std::logic_error, std::runtime_error, and other exceptions are distinguished in message.
* This macro should be used with PML_CATCH_BEGIN.
*/
#define PML_CATCH_END_AND_OUTPUT( OSTREAM )   } catch(...) { pml::detail::output_exceptions(OSTREAM); }

/**
* @def
* Macro to finish a try-brock which aggregates error message of nested exceptions and throws a single std::runtime_error with aggregated one.
* This macro should be used with PML_CATCH_BEGIN.
*/
#define PML_CATCH_END_AND_AGGREGATE   } catch(...) { pml::detail::aggregate_exceptions(); }

/**
* @def
* Macro to finish a try-brock which calls std::throw_with_nested(TYPE(MESSAGE)) if something exceptions are catched.
* This macro should be used with PML_CATCH_BEGIN.
*/
#define PML_CATCH_END_AND_THROW( TYPE, MESSAGE )   } catch(...) { PML_THROW_WITH_NESTED( TYPE, MESSAGE ); }


namespace pml {
    namespace detail {

        /**
        * @brief
        * Throws std::nested_exception by std::throw_with_nested( E(inMessage with info. of inExceptionFileName and inLine) )
        * Templated exception E must have a Single String Argument constructor.
        *
        * @param[in] inMessage
        * Error message.
        *
        * @param[in] inExceptionFileName
        * File name throwing the present exception.
        *
        * @param[in] inLine
        * Line number where exception occured in inExceptionFileName.
        */
        template<typename E>
        [[noreturn]] void throw_with_nested_SSA(
            std::string&& inMessage,
            char const* inExceptionFileName,
            std::size_t inLine)
        {
            std::throw_with_nested(
                E(std::string{} + inExceptionFileName + "(" + std::to_string(inLine) + "): " + inMessage + " "));
        };

        /**
        * @brief
        * Output error message of current exceptions.
        *
        * @param[in] inOstream
        * Out stream.
        */
        void output_exceptions(std::ostream& inOstream);

        /**
        * @brief
        * Aggregate error message of current exceptions.
        */
        void aggregate_exceptions();

    } // detail
} // pml

#endif