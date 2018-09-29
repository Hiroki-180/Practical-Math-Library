#include <PML/Core/exception_handler.h>
#include <iostream>
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

        void print_std_exception(
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
                print_std_exception(e, inOstream, false);
            }
            catch (const std::runtime_error& e) {
                inOstream << "Runtime error: ";
                print_std_exception(e, inOstream, false);
            }
            catch (const std::bad_exception& e) {
                inOstream << "Bad exception error: ";
                print_std_exception(e, inOstream, false);
            }
            catch (const std::bad_alloc& e) {
                inOstream << "Bad alloc error: ";
                print_std_exception(e, inOstream, false);
            }
            catch (const std::bad_cast& e) {
                inOstream << "Bad cast error: ";
                print_std_exception(e, inOstream, false);
            }
            catch (const std::bad_typeid& e) {
                inOstream << "Bad typeid error: ";
                print_std_exception(e, inOstream, false);
            }
            catch (const std::bad_weak_ptr& e) {
                inOstream << "Bad weak pointer error: ";
                print_std_exception(e, inOstream, false);
            }
#if defined(__cplusplus) && __cplusplus >= 201703L
            catch (const std::bad_optional_access& e) {
                inOstream << "Bad optional access error: ";
                print_std_exception(e, inOstream, false);
            }
            catch (const std::bad_variant_access& e) {
                inOstream << "Bad variant access error: ";
                print_std_exception(e, inOstream, false);
            }
#endif
            catch (const std::exception& e) {
                inOstream << "Error: ";
                print_std_exception(e, inOstream, false);
            }
        }

        void print_exception(std::ostream& inOstream)
        {
            try {
                throw;
            }
            catch (std::exception& e) {
                print_std_exception(e, inOstream, true);
            }
            catch (...) {
                inOstream << "PML never throws not standard exceptions." << std::endl;
            }
        }

    } // detail
} // pml
