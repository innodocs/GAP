/*
**  rational-pi.cpp
**
*A  Ovidiu Podisor
*C  Copyright © 2021 innodocs. All rights reserved.
**
**  Compute 'pi'
*/

#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

#include "instant.h"
#include "gap/rat.h"
using namespace Gap;

namespace Pi {

/**
 *  Madhava / Gregory–Leibniz series
 *
 *     1 - 1/3 + 1/5 - 1/7 ... = pi/4
 *
 *  https://en.wikipedia.org/wiki/Leibniz_formula_for_%CF%80
 */
Gap::Rat seriesMGL(unsigned long N)
{
  Gap::Rat sum = 1;
  for (unsigned long i = 1; i < N; i++) {
    if (i % 2 == 0)
      sum += Gap::Rat(1, 2*i+1);
    else
      sum -= Gap::Rat(1, 2*i+1);
  }

  return 4*sum;
}

/**
 * Borwein, Bailey, Plouffe series
 *
 * https://en.wikipedia.org/wiki/Bailey%E2%80%93Borwein%E2%80%93Plouffe_formula
 */
Rat seriesBBP(unsigned long N)
{
  Rat sum = 0;
  for (unsigned long i = 0; i < N; i++) {
    sum += Rat(1, Gap::Int::pow(16, i))
         //* (Rat(4, 8*i+1) - Rat(2, 8*i+4) - Rat(1, 8*i+5) - Rat(1, 8*i+6));
         * Rat(120*i*i + 151*i + 47,
               Gap::Int::pow(i, 4)*512 + Gap::Int::pow(i, 3)*1024 + (712*i*i + 194*i + 15));
  }

  return sum;
}

struct prDec {
  const Gap::Rat& r; const int prec;
  prDec(const Gap::Rat& _r, const int _prec) : r(_r), prec(_prec) {}
  friend ostream& operator<<(ostream& os, const prDec& pr);
};
ostream& operator<<(ostream& os, const prDec& pr)
{
  Gap::Rat r = pr.r;
  int prec = pr.prec;

  Gap::Int quo = r.num() / r.den();
  Gap::Rat rem = r - quo;

  if (r.isNeg()) {
    os << '-';
    prec--;
  }
  os << quo << '.';

  for (int i = 0; i < prec; i++) {
    rem *= 10;
    quo = rem.num() / rem.den();
    os << quo;

    rem -= quo;
  }

  return os;
}

template<int nrRuns>
void testHarness(const unsigned long max,
            int wMax, int wSum, int wTime)
{
  Gap::Rat sum = 0;
  double duration = 0;

  Instant start, end;
  start = Instant::now(); {
   for (int i = 0; i < nrRuns; i++) {
     sum = seriesMGL(max);
   }
  } end = Instant::now();

  duration = static_cast<double>(Duration::between(start, end).toNanos())
             / (1000000*nrRuns);
  cout << "Pi-MGL"
       << " | " << setw(wTime) << duration
       << " | " << setw(wMax)  << max
       << " | " << "  "        << prDec(sum, wSum-2)
       << endl;

  if (max > 32768)
    return;
  start = Instant::now(); {
   for (int i = 0; i < nrRuns; i++) {
     sum = seriesBBP(max);
   }
  } end = Instant::now();

  duration = static_cast<double>(Duration::between(start, end).toNanos())
             / (1000000*nrRuns);
  cout << "Pi-BBP"
       << " | " << setw(wTime) << duration
       << " | " << setw(wMax)  << max
       << " | " << "  "        << prDec(sum, wSum-2)
       << endl;
}

}; /* namespace Pi */


int main(int argc, char *argv[])
{
  Gap::Init(argc, argv);

  static constexpr unsigned long MAX = 1000000;

  int wMax  = log10(MAX)+1;
  int wSum  = 50;
  int wTime = 14;

  for (unsigned long max = 2; max <= MAX; max *= 2)
    Pi::testHarness<1>(max, wMax, wSum, wTime);

  //int prec = 32768;
  //cout << Pi::prDec(Pi::seriesBBP(prec), prec);

  return 0;
}
