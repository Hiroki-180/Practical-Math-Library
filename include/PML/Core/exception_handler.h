#ifndef CORE_EXCEPTION_HANDLER_H
#define CORE_EXCEPTION_HANDLER_H

/**
* @file public header provided by PML.
*
* @brief Useful macros for handling exceptions.
*/

#include <exception>
#include <stdexcept>
#include <ostream>
#include <type_traits>

/**
* @def
* Macro to throw std::nested_exception by std::throw_with_nested( ETYPE(MESSAGE with info. of __FILE__ and __LINE__, ...) ).
* PML athrows exceptions using this macro if it's ctor accepts an message.
* PML throws only inheritances of std::exception when exceptions occured and does not throw something original implementations.
*/
#define PML_THROW_WITH_NESTED(ETYPE, MESSAGE, ...) \
pml::detail::throw_with_nested_wrapper<ETYPE>(__FILE__, __LINE__, MESSAGE, __VA_ARGS__)

/**
* @def
* Macro to begin a try-brock.
* This macro should be used with PML_CATCH_END.
*/
#define PML_CATCH_BEGIN try{

/**
* @def
* Macro to finish a try-brock and output error messges to OSTREAM which must be an inheritance of std::ofstream.
* std::logic_error, std::runtime_error, and other exceptions are distinguished in message.
* This macro should be used with PML_CATCH_BEGIN.
*/
#define PML_CATCH_END(OSTREAM)   } catch(...) { pml::detail::output_exceptions(OSTREAM); }

/**
* @def
* Macro to finish a try-brock which calls std::throw_with_nested(TYPE(MESSAGE)) if something exceptions are catched.
* This macro should be used with PML_CATCH_BEGIN.
* Macro to throw std::nested_exception by std::throw_with_nested when an exception occurred in OPERATION.
*/
#define PML_CATCH_END_AND_THROW(ETYPE, MESSAGE, ...)   } catch (...) { PML_THROW_WITH_NESTED(ETYPE, MESSAGE, __VA_ARGS__); }

/**
* @def
* Macro to throw std::nested_exception by std::throw_with_nested when an exception occurred in OPERATION.
*/
#define PML_HOOK(OPERATION, ETYPE, MESSAGE, ...)            \
[&]()                                                       \
{                                                           \
    try{                                                    \
        return OPERATION;                                   \
    }                                                       \
    catch(...){                                             \
        PML_THROW_WITH_NESTED(ETYPE, MESSAGE, __VA_ARGS__); \
    }                                                       \
}()

namespace pml {
    namespace detail {

        /**
        * @brief
        * Throws std::nested_exception by std::throw_with_nested( E(inMessage with info. of inFileName and inLine) )
        *
        * @param[in] E
        * Exception type.
        * The first argument of the ctor must be a string literal and error message.
        *
        * @param[in] inFileName
        * File name throwing the present exception.
        *
        * @param[in] inLine
        * Line number where exception occured in inExceptionFileName.
        *
        * @param[in] inMessage
        * Error message.
        *
        * @param[in] inArgs
        * Second and other arguments of the ctor of E.
        */
        template<typename E, typename ...Args>
        [[noreturn]]
        void throw_with_nested_wrapper(
            char const* inFileName,
            std::size_t inLine,
            const std::string& inMessage,
            Args&&... inArgs)
        {
            const std::string info{ "l." + std::to_string(inLine) + " in " + inFileName + ": " + inMessage + " " };

            std::throw_with_nested(E(info, std::forward<Args>(inArgs)...));
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
            try
            {
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
                inoutStream << "Something STL Error: ";
                output_std_exceptions(e, inoutStream, false);
            }
            catch(...) {
                inoutStream << "None-STL Error.";
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
    } // detail
} // pml

#endif