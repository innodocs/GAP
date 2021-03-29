# GAP

<h2>C++ LibGAP Interface</h2>

- [Introduction](#introduction)
  
- [Using the API](#using-the-api)
    - [Integers](#integers)
    - [Rationals](#rationals)
  
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

<h4>Integers</h4>

- include the `gap/int.h` header file
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

For another example in `test/PE-001-gap.cpp`, the `GAP` version of `sumOfSeries`
was derived from the original C-`int` version simply by replacing `unsigned long`
with `Gap::Int`.

    Gap::Int sumOfSeries(unsigned long start, unsigned long end) {
      if (start > end)
        return 0;

      Gap::Int diff = end - start;
      return (diff % 2 == 0)
             ?  (diff+1)    * ((start+end)/2)
             : ((diff+1)/2) *  (start+end);
    }

<h4>Rationals</h4>

- include the `gap/rat.h` header file
- define variables using the `Gap::Rat` type
- use them as you would use a normal C `int`

For example:

    Gap::Rat piSeries(unsigned long N) {
      Gap::Rat sum = 1;
      for (unsigned long i = 1; i < N; i++) {
        if (i % 2 == 0)
          sum += Gap::Rat(1, 2*i+1);
        else
          sum -= Gap::Rat(1, 2*i+1);
      }
      return 4*sum;
    }

Solution | Time (ms) | Iterations  | Pi Approximation
------ | ---------- | ------------------- | -------------
Pi   |      0.021 |       2 | 2.66666666666666666666666666666666666666666666666666
Pi   |      0.005 |       4 | 2.89523809523809523809523809523809523809523809523809
Pi   |      0.005 |       8 | 3.01707181707181707181707181707181707181707181707181
Pi   |      0.017 |      16 | 3.07915339419742616127046502490958431369330483839942
Pi   |      0.053 |      32 | 3.11035027369868591662673207601446144110965763351191
Pi   |      0.053 |      64 | 3.12596860697328775602217776200598767849546447755739
Pi   |      0.123 |     128 | 3.13378027278998953473003255638715434914075010785843
Pi   |      0.292 |     256 | 3.13768641849067022844211661176356555725817534854694
Pi   |      0.646 |     512 | 3.13963953045242950601273919533185964951400618752302
Pi   |      1.615 |    1024 | 3.14061609132262360456156417980421812566680794945537
Pi   |      5.209 |    2048 | 3.14110437236889706024576601425238367601846888563834
Pi   |     18.150 |    4096 | 3.14134851296843121699868460238517784434856826312530
Pi   |     79.024 |    8192 | 3.14147058327774798580505951831088148952440084231846
Pi   |    196.469 |   16384 | 3.14153161843360008188123949350136313742017582717285
Pi   |    609.564 |   32768 | 3.14156213601167534388999271247525956150829377972076
Pi   |   2311.240 |   65536 | 3.14157739480073162664106282491079398384655305652476
Pi   |   9388.620 |  131072 | 3.14158502419526209948494583771722125853127842857039
Pi   |  37613.500 |  262144 | 3.14158883889252762734043119084152414983304540330895
Pi   | 146685.000 |  524288 | 3.14159074624116042769736685924842136955697129342016
Pi   | 131777.000 |  524288 | 3.14159074624116042769736685924842136955697129342016


<h3>Building the System</h3>

* download and build the GAP system, as outlined in the instructions at
  https://www.gap-system.org/Releases/. The latest officially released version
  is 4.11.1, at https://www.gap-system.org/Releases/4.11.1.html

* if the build is successful and you have tested the installation, you need
  to alter a few source files in the `src` folder:
  
     * `integer.c`: remove the `static` keyword from `StringIntBase`
     * `rational.c`: remove the `static` keyword from
       `EqRat`, `LtRat`, `SumRat`, `DiffRat`, `AInvRat`, `ProdRat`, and add the following
       definition

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


