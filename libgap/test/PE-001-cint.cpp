/*
**  PE-001-01-cint.cpp
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

namespace Problem1
{
  /**
   * solution1: brute force loop/filter/add
   * complexity: O(n)
   */
  unsigned long solution1(unsigned long N) {
    unsigned long sum = 0;
    for (int i = 0; i < N; i++) {
      if (i % 3 == 0 || i % 5 == 0) {
        sum += i;
      }
    }

    return sum;
  }

  /**
   * solution2: use sum of series formula: sum(i : 1..n, i) = n*(n+1)/2
   * complexity: O(1)
   */
  unsigned long sumOfSeries(unsigned long start, unsigned long end) {
    if (start > end)
      return 0;
    unsigned long diff = end - start;
    return (diff % 2 == 0)
       ? (diff+1) * ((start+end)/2)
       : ((diff+1)/2) * (start+end);
  }

  unsigned long solution2(unsigned long N) {
    return 3 * sumOfSeries(1, (N-1)/3)
        +  5 * sumOfSeries(1, (N-1)/5)
        - 15 * sumOfSeries(1, (N-1)/15);
  }
};


int main(int argc, char *argv[])
{
  static constexpr unsigned long MAX = 1000000000;
  static constexpr int nrRuns = 10;

  int wMax = log10(MAX)+1;
  int wSum = wMax*2;
  int wTime = 10;

  for (unsigned long max = 10; max <= MAX; max *= 10)
  {
    Instant start, end;
    unsigned long sum = 0;

    start = Instant::now(); {
      for (int i = 0; i < nrRuns; i++) {
        sum = Problem1::solution1(max);
      }
    } end = Instant::now();

    double d = static_cast<double>(Duration::between(start, end).toNanos())
               / (1000000*nrRuns);
    cout << "sol 1 "
         << " | " << setw(wTime) << d
         << " | " << setw(wMax)  << max
         << " | " << setw(wSum)  << sum
         << endl;

    start = Instant::now(); {
      for (int i = 0; i < nrRuns; i++) {
        sum = Problem1::solution2(max);
      }
    } end = Instant::now();

    d = static_cast<double>(Duration::between(start, end).toNanos())
        / (1000000*nrRuns);
    cout << "sol 2 "
         << " | " << setw(wTime) << d
         << " | " << setw(wMax)  << max
         << " | " << setw(wSum)  << sum
         << endl;
   }

   return 0;
}

