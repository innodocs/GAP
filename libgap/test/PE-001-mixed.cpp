/*
**  PE-001-01-mixed.cpp
**
*A  Ovidiu Podisor
*C  Copyright © 2021 innodocs. All rights reserved.
**
**  Project Euler Problem 1
**
**  If we list all the natural numbers below 10 that are multiples of 3 or 5,
**  we get 3, 5, 6 and 9. The sum of these multiples is 23.
**
**  Find the sum of all the multiples of 3 or 5 below 1000.
*/

#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

#include "instant.h"
#include "gap/int.h"
using namespace Gap;

namespace Problem1
{
/**
 * solution2: use sum of series formula: sum(i : 1..n, i) = n*(n+1)/2
 * complexity: O(1)
 */
template<class T>
T sumOfSeries(unsigned long start, unsigned long end)
{
 if (start > end)
   return 0;

 T diff = end - start;
 return (diff % 2 == 0)
    ?  (diff+1)    * ((start+end)/2)
    : ((diff+1)/2) *  (start+end);
}

template<class T>
T solution2(unsigned long N)
{
  return 3 * sumOfSeries<T>(1, (N-1)/3)
      +  5 * sumOfSeries<T>(1, (N-1)/5)
      - 15 * sumOfSeries<T>(1, (N-1)/15);
}

template<class T, int nrRuns>
void testHarness(const unsigned long max,
            int wMax, int wSum, int wTime)
{
  T sum = 0;

  Instant start, end;
  start = Instant::now(); {
   for (int i = 0; i < nrRuns; i++) {
     sum = solution2<T>(max);
   }
  } end = Instant::now();

  double d = static_cast<double>(Duration::between(start, end).toNanos())
             / (1000000*nrRuns);
  cout << "sol 2 "
       << " | " << setw(wTime) << d
       << " | " << setw(wMax)  << max
       << " | " << setw(wSum)  << sum
       << endl;
}

}; /* namespace Problem1 */


int main(int argc, char *argv[])
{
  Gap::Init(argc, argv);

  static constexpr unsigned long MAX      = 1000000000000000000;
  static constexpr unsigned long MAX_CINT =          1000000000;

  int wMax = log10(MAX)+1;
  int wSum = wMax*2;
  int wTime = 10;

  for (unsigned long max = 10; max <= MAX; max *= 10)
  {
    if (max <= MAX_CINT)
      Problem1::testHarness<unsigned long, 10>(max, wMax, wSum, wTime);
    else
      Problem1::testHarness<Gap::Int, 10>(max, wMax, wSum, wTime);
  }

  return 0;
}
