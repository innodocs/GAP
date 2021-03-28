# GAP

<h2>C++ libgap Interface</h2>

The goal of this project is to build a C++ layer for interfacing
with the GAP system (Groups, Algorithms, and Programming [GAP2021]).

From the introduction at www.gap-system.org, "GAP is a system
for computational discrete algebra, with particular emphasis
on Computational Group Theory. GAP provides a programming
language, a library of thousands of functions implementing
algebraic algorithms written in the GAP language as well as
large data libraries of algebraic objects."

Initially this wealth of mathematical algorithms could
only be accessed via the GAP REPL (Read-Eval-Print-Loop).
Subsequently a GAP-to-C compiler was added, which could translate
GAP language programs into C and allow them to be linked into
the GAP kernel, or be built into stand-alone applications.

One glaring ommission was the possibility to access the GAP
functionality from standard programming and language environments
such as `C`, `C++`, `Python` etc.

Recently, the developers have allowed the GAP kernel
functionality to be exported in the form of a library which
can then be linked to various other applications. The API to
the library is essentially a `C` language API, and as such
lacking any form of support for operator overloading,
implicit/explicit conversions to other types, aso, asf.

The goal of this project is to provide such an API for C++.


<h3>Building the System</h3>

* download and build the GAP system, as outlined in the instructions at
  https://www.gap-system.org/Releases/. The latest officially released version
  is 4.11.1, at https://www.gap-system.org/Releases/4.11.1.html

* if the build is successful and you have tested the installation, you need
  to alter one source file, `src/integer.c`. On line `792`, remove
  the `static` keyword:
  
      static Obj StringIntBase(Obj op, int base)

* rebuild the system by re-issuing the make command in the GAP
installation directory:

      make

* when done, install the GAP library

      make install-libgap

* define/alter the following environment variables

    export GAP_DIR="<path to your GAP installation>"
    export CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_PATH:$GAP_DIR/src:$GAP_DIR/gen"

* to test the C++ libgap interface, go to the `test` directory, compile and run
the example programs, e.g.:

  g++ -std=c++17 -I ../include -lgap PE-001-01-mixed.cpp -o PE-001-01-mixed
  ./PE-001-01-mixed
  
If everything went OK, you should see the following output:

Solution | Time (ms) | Max  | Sum
------ | ---------- | ------------------- | -------------
sol 2  |     0.0003 |                  10 |                                     23
sol 2  |     0.0002 |                 100 |                                   2318
sol 2  |     0.0002 |                1000 |                                 233168
sol 2  |     0.0002 |               10000 |                               23331668
sol 2  |     0.0002 |              100000 |                             2333316668
sol 2  |     0.0002 |             1000000 |                           233333166668
sol 2  |     0.0001 |            10000000 |                         23333331666668
sol 2  |     0.0002 |           100000000 |                       2333333316666668
sol 2  |     0.0002 |          1000000000 |                     233333333166666668
sol 2  |     0.0074 |         10000000000 |                   23333333331666666668
sol 2  |     0.0029 |        100000000000 |                 2333333333316666666668
sol 2  |     0.0027 |       1000000000000 |               233333333333166666666668
sol 2  |     0.0025 |      10000000000000 |             23333333333331666666666668
sol 2  |     0.0027 |     100000000000000 |           2333333333333316666666666668
sol 2  |     0.0027 |    1000000000000000 |         233333333333333166666666666668
sol 2  |     0.0027 |   10000000000000000 |       23333333333333331666666666666668
sol 2  |     0.0024 |  100000000000000000 |     2333333333333333316666666666666668
sol 2  |     0.0029 | 1000000000000000000 |   233333333333333333166666666666666668


<h3>Bibliography</h3>

[GAP2021] The GAP Group, *GAP -- Groups, Algorithms, and Programming, Version 4.11.1*; 2021. (https://www.gap-system.org)


