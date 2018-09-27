#include <PML/Core/exception_handler.h>

namespace pml {
    namespace detail {

        void print_std_exception(
            const std::exception& inException,
            std::ofstream& inFile,
            bool inIsFirstCall)
        {
            try {
                if (inIsFirstCall) {
                    throw;
                }

                inFile << inException.what() << std::endl;
                std::rethrow_if_nested(inException);
            }
            catch (const std::logic_error& e) {
                inFile << "Logic error: ";
                print_std_exception(e, inFile, false);
            }
            catch (const std::runtime_error& e) {
                inFile << "Runtime error: ";
                print_std_exception(e, inFile, false);
            }
            catch (const std::exception& e) {
                inFile << "Error: ";
                print_std_exception(e, inFile, false);
            }
            catch (...) {}
        }

        void print_exception(std::ofstream& inFile)
        {
            try {
                throw;
            }
            catch (std::exception& e) {
                print_std_exception(e, inFile, true);
            }
            catch (...) {
                inFile << "Unknown errro occured in PML." << std::endl;
            }
        }

    } // detail
} // pml
