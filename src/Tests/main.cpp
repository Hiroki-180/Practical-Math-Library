#include "stdafx.h"

#include <gtest/gtest.h>

#include <iostream>

int main(int argc, char** argv) 
{
    std::cout << "Unit tests of PML are written with GoogleTest, ";
    std::cout << "which has the following BSD 3 - Clause that must be duplicated in its entirety:" << std::endl << std::endl;

    std::cout << "   GoogleTest, 2005, Google Inc.All rights reserved." << std::endl << std::endl;

    std::cout << "   Redistributionand use in sourceand binary forms, with or without modification," << std::endl;
    std::cout << "   permitted provided that the following conditions are met :" << std::endl << std::endl;

    std::cout << "     * Redistributions of source code must retain the above copyright notice, " << std::endl;
    std::cout << "       this list of conditionsand the following disclaimer." << std::endl;
    std::cout << "     * Redistributions in binary form must reproduce the above copyright notice, " << std::endl;
    std::cout << "       this list of conditionsand the following disclaimer in the documentation " << std::endl;
    std::cout << "       and /or other materials provided with the distribution." << std::endl;
    std::cout << "     * Neither the name of Google Inc.nor the names of its contributors may be " << std::endl;
    std::cout << "       used to endorse or promote products derived from this software without " << std::endl;
    std::cout << "       specific prior written permission." << std::endl << std::endl;

    std::cout << "   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS" << std::endl;
    std::cout << "   \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT" << std::endl;
    std::cout << "   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR" << std::endl;
    std::cout << "   A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT" << std::endl;
    std::cout << "   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL," << std::endl;
    std::cout << "   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO," << std::endl;
    std::cout << "   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;" << std::endl;
    std::cout << "   OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, " << std::endl;
    std::cout << "   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR " << std::endl;
    std::cout << "   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF " << std::endl;
    std::cout << "   ADVISED OF THE POSSIBILITY OF SUCH DAMAGE." << std::endl << std::endl;

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}