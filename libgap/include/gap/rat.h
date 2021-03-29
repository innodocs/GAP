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

public: // operations
  bool operator== (const Rat& opR) const noexcept;
  bool operator<  (const Rat& opR) const noexcept;
  Rat& operator+= (const Rat& opR);
  Rat& operator-= (const Rat& opR);
  Rat& operator*= (const Rat& opR);

  Rat  operator-  () const;

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
  : super(GAP_MakeRat(getGapObj(num), getGapObj(den)))
{}


/****************************************************************************
**
*F  num() . . . . . . . . . . . . . . . . . . . . . . numerator of a rational
*F  den() . . . . . . . . . . . . . . . . . . . . . denumerator of a rational
*/
inline Int Rat::num() const noexcept
{
  return Obj::makeObj<Int, Rat>(NUM_RAT(gapObj));
}
inline Int Rat::den() const noexcept
{
  return Obj::makeObj<Int, Rat>(DEN_RAT(gapObj));
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


} /* namespace GAP */

#endif /* LIBGAP_RAT_H */
