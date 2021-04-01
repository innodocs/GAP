/****************************************************************************
**
*A  Ovidiu Podisor
*C  Copyright © 2021 innodocs. All rights reserved.
**
*L  SPDX-License-Identifier: GPL-2.0-or-later
**
**  This file declares the functions handling rationals.
*/

#ifndef LIBGAP_RAT_H
#define LIBGAP_RAT_H

#include <string>
#include <iostream>

extern "C" {
#include "rational.h"
}
#include "exception.h"
#include "obj.h"
#include "int.h"


namespace Gap {

/****************************************************************************
**
*C Gap::Rat . . . . . . . . . . . . . . . . . . . . . . . GAP rationals class
*/
class Rat : public Obj
{
protected: // construction from GAP object reference, non-public
  typedef Obj super;
  explicit Rat(const GAP_Obj gapObj) : super(gapObj) {}

public: // construction, conversion
  Rat(const GAP_Int8 i = 0);
  Rat(const Int& num);
  Rat(const Int& num, const Int& den);

  Int num() const noexcept;
  Int den() const noexcept;

  string toString(const int base=10) const;

public: // runtime type test
  static bool isRat(const Rat& obj);

public: // properties
  static bool isNeg (const Rat& op) noexcept;
         bool isNeg () const noexcept;
  static bool isPos (const Rat& op) noexcept;
         bool isPos () const noexcept;
  static int  sign(const Rat& op);
         int  sign() const;

public: // operations
  bool operator== (const Rat& opR) const noexcept;
  bool operator<  (const Rat& opR) const noexcept;
  Rat& operator+= (const Rat& opR);
  Rat& operator-= (const Rat& opR);
  Rat& operator*= (const Rat& opR);
  Rat& operator/= (const Rat& opR);

  Rat  operator-  () const;

  static Rat pow(const Rat& opL, const Int& opR);
         Rat pow(const Int& opR) const;
  static Rat abs(const Rat& op);
         Rat abs() const;
  static Int mod(const Rat& opL, const Int& opR);
         Int mod(const Int& opR) const;
  static Rat inv(const Rat& base, const Rat& mod);
         Rat inv(const Rat& mod) const;

  friend ostream& operator<<(ostream& os, const Rat& i);
};


/****************************************************************************
**
*F  Rat( <int> ) . . . . .create a new rational object from an integer object
*F  Rat( <num>, <den>) . . . . .create a new rational object from num and den
**
*/
inline Rat::Rat(const GAP_Int8 i)
  : Rat(Int(i))
{}
inline Rat::Rat(const Int& i)
  : Rat(i, 1)
{}
inline Rat::Rat(const Int& num, const Int& den)
  : super(GAP_MakeRat(unapply(num), unapply(den)))
{}


/****************************************************************************
**
*F  num() . . . . . . . . . . . . . . . . . . . . . . numerator of a rational
*F  den() . . . . . . . . . . . . . . . . . . . . . denumerator of a rational
*/
inline Int Rat::num() const noexcept
{
  return Obj::apply<Int, Rat>(NUM_RAT(gapObj));
}
inline Int Rat::den() const noexcept
{
  return Obj::apply<Int, Rat>(DEN_RAT(gapObj));
}

/****************************************************************************
**
*F  toString( <base> ) . . . . . . . . . . .convert this rational to a string
*F  <stream> << <op> . . . . . . . . . . . . . . . . write rational to stream
**
*/
inline string Rat::toString(const int base) const
{
  return num().toString(base) + " / " + den().toString(base);
}

inline ostream& operator<<(ostream& os, const Rat& op)
{
  int base = 10;
  if      (os.flags() & ios::oct) base = 8;
  else if (os.flags() & ios::hex) base = 16;

  os << op.toString(base);
  return os;
}


/****************************************************************************
**
*F  isRat( <val> ) . . . . . . . . . . . . . test if GAP object is a rational
**
*  'IsRat' returns  'true' if  the  value <val> is  a  rational and  'false'
**  otherwise.
*/
inline bool Rat::isRat(const Rat& val)
{
  GAP_Obj gapObj = val.gapObj;
  return ( TNUM_OBJ(gapObj) == T_RAT || IS_INT(gapObj)  );
}

/****************************************************************************
**
*F  isNeg( <op> ) . . . . . . . . . . . . .check whether rational is negative
*F  isPos(< op> ) . . . . . . . . . . . . .check whether rational is positive
*F  sign() . . . . . . . . . . . . . . . . . . . . . . . . sign of a rational
**
**  'isNeg' returns 'true' if <op> is a negative rational object, and 'false'
**  otherwise.
**
**  'isPos' returns 'true' if <op> is a positive rational object, and 'false'
**  otherwise.
**
**  'sign' returns the absolute value of the rational <op>.
*/
inline bool Rat::isNeg() const noexcept
{
  return num().isNeg();
}
inline bool Rat::isNeg(const Rat& op) noexcept
{
  return op.isNeg();
}

inline bool Rat::isPos() const noexcept
{
  return num().isPos();
}
inline bool Rat::isPos(const Rat& op) noexcept
{
  return op.isPos();
}

inline int Rat::sign() const
{
  GAP_Obj sign = SignRat(gapObj);
  if      (sign == INTOBJ_INT(0))   return 0;
  else if (sign == INTOBJ_INT(1))   return 1;
  else if (sign == INTOBJ_INT(-1))  return -1;
  else
    throw FailedOpException("Rat::sign(): bad sign");
}
inline int Rat::sign(const Rat& op)
{
  return op.sign();
}


/****************************************************************************
**
*F  <opL> '==' <opR> . . . . . . . . . . . . .test if two rationals are equal
**
**  the '==' operator returns 'true' if this rational is equal to <opR>, and
**  'false' otherwise.
*/
inline bool Rat::operator==(const Rat& opR) const noexcept
{
  return EqRat(gapObj, opR.gapObj);
}


/****************************************************************************
**
*F  <opL> '<' <opR> . . . . . . . . . test if a rational is less than another
**
**  the '<'  operator returns  'true' if this rational is  strictly less than
**  rational <opR> and 'false' otherwise.
*/
inline bool Rat::operator<(const Rat& opR) const noexcept
{
  return LtRat(gapObj, opR.gapObj) == 1;
}


/****************************************************************************
**
*F  <opL> += <opR>. . . . . . . . . . . . . . . . . . . . .increment rational
*F  <opL> + <opR> . . . . . . . . . . . . . . . . . . . .sum of two rationals
**
**  the '+=' operator adds <opR> to <opL>.
**  the '+' operator returns the sum of rationals <opL> and <opR>.
**
**  Either operand may also be an integer. The sum is reduced.
*/
inline Rat& Rat::operator+=(const Rat& opR)
{
  *this = Rat(SumRat(gapObj, opR.gapObj));
  return *this;
}
inline Rat operator+(Rat opL, const Rat& opR)
{
  opL += opR;
  return opL;
}


/****************************************************************************
**
*F  <opL> -= <opR>. . . . . . . . . . . . . . . . . . . . .decrement rational
*F  <opL> - <opR> . . . . . . . . . . . . . . . . .difference of two raionals
*F  - <op>  . . . . . . . . . . . . . . . . . .additive inverse of a rational
**
**  the '-=' operator subtracts <opR> from <opL>.
**  the '-' operator returns the difference of rationals <opL> and <opR>.
**  the unary '-' operator returns the additive inverse of <op>.
**
**  Either operand may also be an integer. The difference is reduced.
*/
inline Rat& Rat::operator-=(const Rat& opR)
{
  *this = Rat(DiffRat(gapObj, opR.gapObj));
  return *this;
}
inline Rat operator-(Rat opL, const Rat& opR)
{
  opL -= opR;
  return opL;
}
inline Rat Rat::operator-() const
{
  return Rat(AInvRat(gapObj));
}


/****************************************************************************
**
*F  <opL> *= <opR>. . . . . . . . . . . . . . . . . . . . . multiply rational
*F  <opL> * <opR> . . . . . . . . . . . . . . multiplication of two rationals
**
**  the '*=' multiplies the rational <opL> by <opR>.
**  the '*' operator returns the product of rationals <opL> and <opR>.
**
**  Either operand may also be an integer. The product is reduced.
*/
inline Rat& Rat::operator*=(const Rat& opR)
{
  *this = Rat(ProdRat(gapObj, opR.gapObj));
  return *this;
}
inline Rat operator*(Rat opL, const Rat& opR)
{
  opL *= opR;
  return opL;
}


/****************************************************************************
**
*F  <opL> /= <opR>. . . . . . . . . . . . . . . . . . . . . . divide rational
*F  <opL> / <opR> . . . . . . . . . . . . . . . . . division of two rationals
**
**  the '/=' operator divides the rational <opL> by <opR>.
**  the '/' operator returns the quotient of rationals <opL> and <opR>.
*/
inline Rat& Rat::operator/=(const Rat& opR)
{
  *this = Rat(QuoRat(gapObj, opR.gapObj));
  return *this;
}
inline Rat operator/(Rat opL, const Rat& opR)
{
  opL /= opR;
  return opL;
}


/****************************************************************************
**
*F  pow( <opL>, <opR> ) . . . . . . . . .raise a rational to an integer power
**
**  'pow' raises the rational <opL> to the power given by the integer <opR>.
**  The power is reduced.
*/
inline Rat Rat::pow(const Int& opR) const
{
  return Rat(PowRat(gapObj, unapply(opR)));
}
inline Rat Rat::pow(const Rat& opL, const Int& opR)
{
  return opL.pow(opR);
}


/****************************************************************************
**
*F  abs( <op> ) . . . . . . . . . . . . . . . . .absolute value of a rational
**
**  'abs' returns the absolute value of the rational <op>.
*/
inline Rat Rat::abs() const
{
  return Rat(AbsRat(gapObj));
}
inline Rat Rat::abs(const Rat& op)
{
  return op.abs();
}


/****************************************************************************
**
*F  mod( <opL>, <opR> ) . . . . . . . . . . remainder of fraction mod integer
**
**  'mod' returns the remainder of the fraction <opL> modulo the integer <n>.
**  The remainder is always an integer.
**
**  '<r>  / <s> mod  <n>' yields  the remainder of   the fraction '<p> / <q>'
**  modulo  the  integer '<n>',  where '<p> / <q>' is  the  reduced  form  of
**  '<r>  / <s>'.
**
**  The modular remainder of $r / s$ mod $n$ is defined to be the integer $k$
**  in $0 .. n-1$ such that $p = k q$ mod $n$, where $p = r / gcd(r, s)$  and
**  $q = s / gcd(r, s)$. In particular, $1  /  s$  mod  $n$  is  the  modular
**  inverse of $s$ modulo $n$, whenever $s$ and $n$ are relatively prime.
**
**  Note that  the  remainder  will  not  exist  if  $s / gcd(r, s)$  is  not
**  relatively prime to $n$. Note that $4 / 6$ mod $32$ does  exist  (and  is
**  $22$), even though $6$ is not invertible modulo  $32$,  because  the  $2$
**  cancels.
**
**  Another possible  definition of $r/s$ mod $n$  would be  a rational $t/s$
**  such that $0 \<= t/s \< n$ and $r/s - t/s$ is a multiple of $n$.  This is
**  rarely needed while computing modular inverses is very useful.
*/
inline Int Rat::mod(const Int& opR) const
{
  return apply<Int, Rat>(ModRat(gapObj, unapply(opR)));
}
inline Int Rat::mod(const Rat& opL, const Int& opR)
{
  return opL.mod(opR);
}


} /* namespace GAP */

#endif /* LIBGAP_RAT_H */
