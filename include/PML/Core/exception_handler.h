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
#include <type_traits>

/**
* @def
* Macro to throw std::nested_exception by std::throw_with_nested( TYPE(MESSAGE with info. of __FILE__ and __LINE__) ).
* PML always throws exceptions using this macro if TYPE has a Single String Argument constructor.
* PML throws only inheritances of std::exception if exceptions occured and does not throw it's original implementations.
*/
#define PML_THROW_WITH_NESTED( TYPE, MESSAGE ) pml::detail::throw_with_nested_SSA<TYPE>(MESSAGE, __FILE__, __LINE__)

/**
* @def
* Macro to begin a try-brock.
* This macro should be used with PML_CATCH_END_AND_OUTPUT, PML_CATCH_END_AND_AGGREGATE, or PML_CATCH_END_AND_THROW.
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

        template<typename E>
        std::enable_if_t<std::is_polymorphic<E>::value>
            rethrow_if_nested_ptr(const E& inException)
        {
            const auto *p = dynamic_cast<const std::nested_exception*>(std::addressof(inException));

            if (p && p->nested_ptr()) {
                p->rethrow_nested();
            }
        }

        /**
        * @brief
        * Output error message of std::exceptions.
        *
        * @param[in] inException
        * Nested exceptions.
        *
        * @param[in] inoutStream
        * Stream.
        *
        * @param[in] inIsFirstCall
        * Is the current call first one in recursion of nested_exception unrolling.
        */
        inline void output_std_exceptions(
            const std::exception& inException,
            std::ostream& inoutStream,
            bool inIsFirstCall)
        {
            try {
                if (inIsFirstCall) {
                    throw;
                }

                inoutStream << inException.what() << std::endl;
                pml::detail::rethrow_if_nested_ptr(inException);
            }
            catch (const std::logic_error& e) {
                inoutStream << "Logic error: ";
                output_std_exceptions(e, inoutStream, false);
            }
            catch (const std::runtime_error& e) {
                inoutStream << "Runtime error: ";
                output_std_exceptions(e, inoutStream, false);
            }
            catch (const std::bad_exception& e) {
                inoutStream << "Bad exception error: ";
                output_std_exceptions(e, inoutStream, false);
            }
            catch (const std::bad_alloc& e) {
                inoutStream << "Bad alloc error: ";
                output_std_exceptions(e, inoutStream, false);
            }
            catch (const std::bad_cast& e) {
                inoutStream << "Bad cast error: ";
                output_std_exceptions(e, inoutStream, false);
            }
            catch (const std::bad_typeid& e) {
                inoutStream << "Bad typeid error: ";
                output_std_exceptions(e, inoutStream, false);
            }
            catch (const std::bad_weak_ptr& e) {
                inoutStream << "Bad weak pointer error: ";
                output_std_exceptions(e, inoutStream, false);
            }
#if defined(__cplusplus) && __cplusplus >= 201703L
            catch (const std::bad_optional_access& e) {
                inoutStream << "Bad optional access error: ";
                output_std_exceptions(e, inoutStream, false);
            }
            catch (const std::bad_variant_access& e) {
                inoutStream << "Bad variant access error: ";
                output_std_exceptions(e, inoutStream, false);
            }
#endif
            catch (const std::exception& e) {
                inoutStream << "Error: ";
                output_std_exceptions(e, inoutStream, false);
            }
        }

        /**
        * @brief
        * Output error message of current exceptions.
        *
        * @param[in] inoutStream
        * Stream.
        */
        inline void output_exceptions(std::ostream& inoutStream)
        {
            try {
                throw;
            }
            catch (std::exception& e) {
                output_std_exceptions(e, inoutStream, true);
            }
            catch (...) {
                inoutStream << "PML never throws unstandard exceptions." << std::endl;
            }
        }

        /**
        * @brief
        * Aggregate error message of std::exceptions.
        *
        * @param[in] inException
        * Nested exceptions.
        *
        * @param[in] inAggregatedMessage
        * Recursively aggregated message.
        *
        * @param[in] inIsFirstCall
        * Is the current call first one in recursion of nested_exception unrolling.
        */
        inline void aggregate_std_exceptions(
            const std::exception& inException,
            std::string& inAggregatedMessage,
            bool inIsFirstCall)
        {
            try {
                if (inIsFirstCall) {
                    throw;
                }

                inAggregatedMessage += std::string(std::string{} +" " + inException.what());
                pml::detail::rethrow_if_nested_ptr(inException);
            }
            catch (const std::exception& e) {
                aggregate_std_exceptions(e, inAggregatedMessage, false);
            }
        }

        /**
        * @brief
        * Aggregate error message of current exceptions.
        */
        inline void aggregate_exceptions()
        {
            try {
                throw;
            }
            catch (std::exception& e) {
                std::string lAggregatedMessage("");
                aggregate_std_exceptions(e, lAggregatedMessage, true);
                throw std::runtime_error(lAggregatedMessage);
            }
            catch (...) {
                std::string lAggregatedMessage("");
                lAggregatedMessage = "PML never throws unstandard exceptions.";
                throw std::runtime_error(lAggregatedMessage);
            }
        }

    } // detail
} // pml

#endif