/*
**  PE-001-06.cpp
**
*A  Ovidiu Podisor
*C  Copyright © 2021 innodocs. All rights reserved.
**
**  Project Euler Problem 6: The sum of the squares of the first ten natural numbers is
**
**     1^2 + 2^2 + ... + 10^2 = 385
**
**  The square of the sum of the first ten natural numbers is,
**
**     (1 + 2 + ... + 10)^2 = 552 = 3025
**
**  Hence the difference between the sum of the squares of the first ten natural numbers
**  and the square of the sum is 3025 − 385 = 2640.
**
**  Find the difference between the sum of the squares of the first one hundred natural numbers
**  and the square of the sum.
*/

#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

#include "instant.h"
#include "gap/int.h"
using namespace Gap;

namespace Problem6
{

/**
 * solution1: brute force loop/add
 *
 *  complexity: O(n)
 */
template<class T>
T solution1(unsigned long N)
{
  T sum = 0;
  T sumSq = 0;

  for (unsigned long i = 1; i <= N; i++) {
    sum += i;
    sumSq += i*i;
  }

  return sum*sum - sumSq;
}

/**
 * solution2: use sum of series formula:
 *
 *     sum(i : 1..n, i) = n(n+1)/2
 *     sum(i : 1..n, i^2) = n(n+1)(2n+1)/6
 *
 * complexity: O(1)
 */
template<class T>
T solution2(unsigned long N)
{
  T n = N;
  return  ((n * (n + 1) / 2) * (n - 1) / 3) * (3*n + 2) / 2;
}


template<class T, int nrRuns>
void testHarness(unsigned long max,
            int wMax, int wSum, int wTime)
{
  T sum = 0;
  double duration = 0;

  Instant start, end;
  start = Instant::now(); {
   for (int i = 0; i < nrRuns; i++)
     sum = solution1<T>(max);
  } end = Instant::now();

  duration = static_cast<double>(Duration::between(start, end).toNanos())
             / (1000000*nrRuns);
  cout << "sol 1 "
       << " | " << setw(wTime) << duration
       << " | " << setw(wMax)  << max
       << " | " << setw(wSum)  << sum
       << endl;

  start = Instant::now(); {
   for (int i = 0; i < nrRuns; i++)
     sum = solution2<T>(max);
  } end = Instant::now();

  duration = static_cast<double>(Duration::between(start, end).toNanos())
             / (1000000*nrRuns);
  cout << "sol 2 "
       << " | " << setw(wTime) << duration
       << " | " << setw(wMax)  << max
       << " | " << setw(wSum)  << sum
       << endl;
}

}; /* namespace Problem6 */


int main(int argc, char *argv[])
{
  Gap::Init(argc, argv);

  static constexpr unsigned long MAX      = 100000000;//0000000000;
  static constexpr unsigned long MAX_CINT =     10000;

  int wMax = log10(MAX)+3;
  int wSum = wMax*3;
  int wTime = 10;

  cout << endl << "C::Int |||" << endl;
  for (unsigned long max = 10; max <= MAX_CINT; max *= 10)
    Problem6::testHarness<unsigned long, 10>(max, wMax, wSum, wTime);

  cout << endl << "Gap::Int |||" << endl;
  for (unsigned max = 10; max <= MAX; max *= 10)
    Problem6::testHarness<Gap::Int, 10>(max, wMax, wSum, wTime);

  return 0;
}
