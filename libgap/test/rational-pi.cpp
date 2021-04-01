/*
**  rational-pi.cpp
**
*A  Ovidiu Podisor
*C  Copyright © 2021 innodocs. All rights reserved.
**
**  Compute 'pi' using the series
**
**     1 - 1/3 + 1/5 - 1/7 ... = pi/4
**
*/

#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

#include "instant.h"
#include "gap/rat.h"

namespace Pi {

Gap::Rat piSeries(unsigned long N)
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

string toDecStr(const Gap::Rat& r, int prec)
{
  Gap::Int quo = r.num() / r.den();
  Gap::Rat rem = r - quo;

  string s(quo.toString());
  s += ".";

  for (int i = 0; i < prec; i++)
  {
    rem *= 10;
    quo = rem.num() / rem.den();
    s += quo.toString();

    rem -= quo;
  }

  return s;
}


template<int nrRuns>
void testHarness(const unsigned long max,
            int wMax, int wSum, int wTime)
{
  Gap::Rat sum = 0;

  Instant start, end;
  start = Instant::now(); {
   for (int i = 0; i < nrRuns; i++) {
     sum = piSeries(max);
   }
  } end = Instant::now();

  double d = static_cast<double>(Duration::between(start, end).toNanos())
             / (1000000*nrRuns);
  cout << "Pi  "
       << " | " << setw(wTime) << d
       << " | " << setw(wMax)  << max
       << " | " << setw(wSum)  << toDecStr(sum, wSum)
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

  return 0;
}
