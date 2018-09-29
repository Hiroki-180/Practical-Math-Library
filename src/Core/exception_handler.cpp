#include <PML/Core/exception_handler.h>
#include <ostream>
#include <type_traits>

namespace pml {
    namespace detail {

        template<typename E>
        std::enable_if_t<std::is_polymorphic<E>::value>
        rethrow_if_nested_ptr(const E& inException)
        {
            const auto *p = dynamic_cast<const std::nested_exception*>(std::addressof(inException));

            if (p && p->nested_ptr()){
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
        * @param[in] inOstream
        * Out stream.
        *
        * @param[in] inIsFirstCall
        * Is the current call first one in recursion of nested_exception unrolling.
        */
        void output_std_exceptions(
            const std::exception& inException,
            std::ostream& inOstream,
            bool inIsFirstCall)
        {
            try {
                if (inIsFirstCall) {
                    throw;
                }

                inOstream << inException.what() << std::endl;
                pml::detail::rethrow_if_nested_ptr(inException);
            }
            catch (const std::logic_error& e) {
                inOstream << "Logic error: ";
                output_std_exceptions(e, inOstream, false);
            }
            catch (const std::runtime_error& e) {
                inOstream << "Runtime error: ";
                output_std_exceptions(e, inOstream, false);
            }
            catch (const std::bad_exception& e) {
                inOstream << "Bad exception error: ";
                output_std_exceptions(e, inOstream, false);
            }
            catch (const std::bad_alloc& e) {
                inOstream << "Bad alloc error: ";
                output_std_exceptions(e, inOstream, false);
            }
            catch (const std::bad_cast& e) {
                inOstream << "Bad cast error: ";
                output_std_exceptions(e, inOstream, false);
            }
            catch (const std::bad_typeid& e) {
                inOstream << "Bad typeid error: ";
                output_std_exceptions(e, inOstream, false);
            }
            catch (const std::bad_weak_ptr& e) {
                inOstream << "Bad weak pointer error: ";
                output_std_exceptions(e, inOstream, false);
            }
#if defined(__cplusplus) && __cplusplus >= 201703L
            catch (const std::bad_optional_access& e) {
                inOstream << "Bad optional access error: ";
                output_std_exceptions(e, inOstream, false);
            }
            catch (const std::bad_variant_access& e) {
                inOstream << "Bad variant access error: ";
                output_std_exceptions(e, inOstream, false);
            }
#endif
            catch (const std::exception& e) {
                inOstream << "Error: ";
                output_std_exceptions(e, inOstream, false);
            }
        }

        void output_exceptions(std::ostream& inOstream)
        {
            try {
                throw;
            }
            catch (std::exception& e) {
                output_std_exceptions(e, inOstream, true);
            }
            catch (...) {
                inOstream << "PML never throws unstandard exceptions." << std::endl;
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
        void aggregate_std_exceptions(
            const std::exception& inException,
            std::string& inAggregatedMessage,
            bool inIsFirstCall)
        {
            try {
                if (inIsFirstCall) {
                    throw;
                }

                inAggregatedMessage += std::string(std::string{} + " " + inException.what());
                pml::detail::rethrow_if_nested_ptr(inException);
            }
            catch (const std::exception& e) {
                aggregate_std_exceptions(e, inAggregatedMessage, false);
            }
        }

        void aggregate_exceptions()
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
