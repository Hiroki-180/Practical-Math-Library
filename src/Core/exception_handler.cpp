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
            std::ostream& inOfstream,
            bool inIsFirstCall)
        {
            try {
                if (inIsFirstCall) {
                    throw;
                }

                inOfstream << inException.what() << std::endl;
                pml::detail::rethrow_if_nested_ptr(inException);
            }
            catch (const std::logic_error& e) {
                inOfstream << "Logic error: ";
                print_std_exception(e, inOfstream, false);
            }
            catch (const std::runtime_error& e) {
                inOfstream << "Runtime error: ";
                print_std_exception(e, inOfstream, false);
            }
            catch (const std::bad_exception& e) {
                inOfstream << "Bad exception error: ";
                print_std_exception(e, inOfstream, false);
            }
            catch (const std::bad_alloc& e) {
                inOfstream << "Bad alloc error: ";
                print_std_exception(e, inOfstream, false);
            }
            catch (const std::bad_cast& e) {
                inOfstream << "Bad cast error: ";
                print_std_exception(e, inOfstream, false);
            }
            catch (const std::bad_typeid& e) {
                inOfstream << "Bad typeid error: ";
                print_std_exception(e, inOfstream, false);
            }
            catch (const std::bad_weak_ptr& e) {
                inOfstream << "Bad weak pointer error: ";
                print_std_exception(e, inOfstream, false);
            }
#if defined(__cplusplus) && __cplusplus >= 201703L
            catch (const std::bad_optional_access& e) {
                inOfstream << "Bad optional access error: ";
                print_std_exception(e, inOfstream, false);
            }
            catch (const std::bad_variant_access& e) {
                inOfstream << "Bad variant access error: ";
                print_std_exception(e, inOfstream, false);
            }
#endif
            catch (const std::exception& e) {
                inOfstream << "Error: ";
                print_std_exception(e, inOfstream, false);
            }
        }

        void print_exception(std::ostream& inOfstream)
        {
            try {
                throw;
            }
            catch (std::exception& e) {
                print_std_exception(e, inOfstream, true);
            }
            catch (...) {
                inOfstream << "Unknown errro occured in PML." << std::endl;
            }
        }

    } // detail
} // pml
