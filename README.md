# GAP

<h2>C++ LibGAP Interface</h2>

- [Introduction](#introduction)
- [Using the API](#using-the-api)
- [Building the System](#building-the-system)
- [Bibliography](#bibliography)

<h3>Introduction</h3>

The goal of this project is to build a C++ layer for interfacing
with the GAP system (Groups, Algorithms, and Programming [GAP2021]).

From the introduction at www.gap-system.org:
>GAP is a system
>for computational discrete algebra, with particular emphasis
>on Computational Group Theory. GAP provides a programming
>language, a library of thousands of functions implementing
>algebraic algorithms written in the GAP language as well as
>large data libraries of algebraic objects.

Initially, this wealth of mathematical algorithms could
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


<h3>Using the API</h3>

For a quick test of the system using integers (`Gap::Int`)

- include the `gap/int.h` header file
- call 'Gap::Init' in your `main` method
- define variables using the `Gap::Int` type
- use them as you would use a normal C `int`

For example:

        Gap::Int a = 104324353;
        cout << "a = "       << a  << endl
             << "-a = "      << -a << endl
             << "abs(-a) = " << Gap::Int::abs(-a) << endl << endl
             << "104324353 + 234 = " << a + 234 <<
             << "104324353 - 234 = " << a - 234 <<
             << "104324353 * 234 = " << a * 234 <<
             << "104324353 / 234 = " << a / 234 <<
             << "104324353 % 234 = " << a % 234 <<
             << "104324353 ^ 234 = " << a.pow(234) << endl;

results in the following output:

        a = 104324353
        -a = -104324353
        abs(-a) = 104324353
        104324353 + 234 = 104324587
        104324353 - 234 = 104324119
        104324353 * 234 = 24411898602
        104324353 / 234 = 445830
        104324353 % 234 = 133
        104324353 ^ 234 = 20056437531574057981952138109243120272035835772136155501407486513525283019820859876950928216874550638510649115147553943328455243510012617409046925631617764080101587871423450693093702657019987423978167195849017588530118012826379627671993798099430289319842066431836667357288295161504296037465626456806391955502557906503738998351888078423097526875272901961030958411947390513475699333179431540625925292892644187077864940928003540234015808981400380150840738998889285377502665822410254198659032677124812488970817067303530651617630268095187902503250069800946611729503221896366823530011875366740509797432863048609340478973938414990360255890225573327840409347788279144519908550893814598622574445239499648110191709986087689775767515360998492236485799931595828150397108979908876362318302296201267439936639115771740547801338783553713634077989857112340514530529580963281765273146201321061284947311746892205535651336069199812964897551309047634656440276568006516209146282067975946572622879796158057643905776988444857274430208309644008341300174403443476746469861052743522196931514556653246392795621106010334348066276531909017366324894943612136176193765207380866568024283263180955850589424021662011943001673066945309818151536800094885643423587994190856455109940602564339751824593176787388196450941444679449917769263510242755152156334576560947312934219885664111737032471247010126182562705043337296980522890336720006934895014066689350532272746834371594779332140288884393143668872204981707314066347878709388462947151902791301980204633154630545722211219871585332511306871865764485588005619359544740634628571338426183176037178295423714280566977203126857596670840023151278960030798834910985760394407652870119015911541803092759199775257999789627571566614796134255278926269188175437589391242061569331805490848046378748890026632338831214908337918180574166070464450470314525870382787927033954362983776769

For more examples on how to use integers, rationals and permutions see the `test` folder.


<h3>Building the System</h3>

* download and build the GAP system, as outlined in the instructions at
  https://www.gap-system.org/Releases/. The latest officially released version
  is 4.11.1, at https://www.gap-system.org/Releases/4.11.1.html

* if the build is successful and you have tested the installation, you need
  to alter a few source files in the `src` folder:
  
     * `integer.c`: remove the `static` keyword from `StringIntBase`
     * `rational.c`: remove the `static` keyword from
       `EqRat`, `LtRat`, `SumRat`, `DiffRat`, `AInvRat`, `ProdRat`, `AInvRat`,
       `QuoRat`, `InvRat`, `ModRat`, `PowRat`, `AbsRat` and `SignRat`
       and add the following definition

      Obj GAP_MakeRat(Obj num, Obj den) {
        return MakeRat(num, den); 
      }

* rebuild the system by re-issuing the make command in the GAP
installation directory:

      make

* when done, install the GAP library

      make install-libgap

* define/alter the following environment variables

       export GAP_DIR="path to your GAP installation"
       export CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_PATH:$GAP_DIR/src:$GAP_DIR/gen"

* to test the C++ LibGAP interface, go to the `test` directory, compile and run
the example programs, e.g.:

      g++ -std=c++17 -I ../include -lgap PE-001-mixed.cpp -o PE-001-mixed
      ./PE-001-mixed
  
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


