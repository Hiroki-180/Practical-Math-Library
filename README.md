# Practical Math Library
This library aims to provide useful functions or classes for IT practitioners.


## Current Status
Current development is mainly for use in 32-bit applications.
Also, current version supports only the Visual Studio 2017 Community compiler.
64-bit development is future target.

## Contents
 - Constants
    - ~~mathematical constants~~
    - ~~physical constants~~

 - Differentiations
    - ~~1st/2nd/3rd/4th derivatives with 1st/2nd/3rd/4th/5th accuracy by forward/central/backward finite difference~~

 - Integrations
    - ~~Trapezoidal rules~~
    - ~~Gaussian quadratures~~
    - ~~Double exponential formulas~~
    
 - Interporations and Extrapolations
    - ~~linear, 2nd/3rd order spline~~
    - ~~monotonic ones~~
    - ~~Hermite polynomial~~
    - ~~bilinear/bicubic~~

 - One dimensional Root findings
    - ~~Newton~~
    - ~~globally convergent Newton~~
    - ~~Brent-Dekker~~
    
 - STL Container Utilities
    - ~~calculations by SIMD~~
    - ~~nearest~~
    - ~~histogram~~
    
 - Special Functions
    - ~~Legendre~~
    - ~~physicists/probabilistic Hermite~~
    - ~~Laguerre~~
 
 - Probability Distributions
    - ~~Standard Normal~~
    - ~~Poisson~~
    - ~~chi-squared~~
    - ~~Noncentral chi-squared~~
    - ~~exponential~~
    - ~~arcsine~~
    - ~~beta~~
    - ~~gamma~~
    - ~~Laplace~~
    - ~~Erlang~~
    - ~~Weibull~~
    - ~~Benktander I/II~~
    - ~~Pareto~~
    - ~~Nakagami~~
    - ~~Gumbel~~
    - ~~logistics~~
    - ~~K~~
    - ~~exponentially modified Gaussian~~
 
 - Stochastic processes
    - ~~Brownian~~
    - ~~2d/3d binomial/trinomial Brownian tree~~
    - ~~Log-normal~~
    - ~~Bessel~~
    - ~~squared Bessel~~
    - ~~gamma~~
    - ~~Ornstein-Uhlenbeck~~
    - ~~CEV~~
    - ~~SABR~~
    
 - Two dimensional Copulas
    - ~~independence/perfect dependence/perfect anti-dependence~~
    - ~~Gaussian~~
    - ~~Student t~~
    - ~~Clayton~~
    - ~~Gumbel~~
    - ~~Frank~~
    - ~~Joe~~
    - ~~Ali-Mikhail-Haq~~
    - ~~empilical~~

## Exceptions
This library throws “PMLException” with detailed error descriptions when input is incorrect.

## Tests
 - Unit tests are written with [GoogleTest](https://github.com/google/googletest), which has the following license that must be duplicated in its entirety, per its terms:
```
 Copyright 2005, Google Inc.  All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are
 met:

     * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above
 copyright notice, this list of conditions and the following disclaimer
 in the documentation and/or other materials provided with the
 distribution.
     * Neither the name of Google Inc. nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```

 - The lib files gtest.lib and gtest_main.lib are generated from [this revision](https://github.com/google/googletest/commit/dfa853b63d17c787914b663b50c2095a0c5b706e) using the Visual Studio 2017 Community compiler.
 - These tests show examples of use of this library.
