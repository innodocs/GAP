/****************************************************************************
**
*A  Ovidiu Podisor
*C  Copyright © 2021 innodocs. All rights reserved.
**
*L  SPDX-License-Identifier: GPL-2.0-or-later
**
**  This file declares the functions handling integers.
*/

#ifndef LIBGAP_INT_H
#define LIBGAP_INT_H

#include <string>
#include <iostream>

extern "C" {
#include "integer.h"
}
#include "exception.h"
#include "obj.h"


namespace Gap {

/****************************************************************************
**
*C Gap::Int . . . . . . . . . . . . . . . . . . . . . . . .GAP integers class
*/
class Int : public Obj
{
protected: // construction from GAP object reference, non-public
  typedef Obj super;
  explicit Int(const GAP_Obj gapObj) : super(gapObj) {}

private: friend class Obj; // allow construction from other classes in hierarchy
  //template<typename T, typename std::enable_if<std::is_base_of<Obj, T>::value>::type* = nullptr>
  static const Int apply(const GAP_Obj gapObj) { return Int(gapObj); }

public: // construction, conversion
  Int(const GAP_UInt* limbs, GAP_Int size);

  Int(const GAP_Int8 i = 0);
  //explicit Int(const GAP_Int i);
  //explicit Int(const GAP_UInt i);
  //explicit Int(const GAP_UInt8 i);

  explicit (operator GAP_Int)()   const;
  explicit (operator GAP_UInt)()  const;
  explicit (operator GAP_Int8)()  const;
  explicit (operator GAP_UInt8)() const;

  int size() const noexcept;
  string toString(const int base=10) const;

public: // runtime type test
  static bool isInt(const Int& obj);
  static bool isSmallInt(const Int& obj);
  static bool isLargeInt(const Int& obj);

public: // properties
  static bool isNeg (const Int& i) noexcept;
         bool isNeg () const noexcept;
  static bool isPos (const Int& i) noexcept;
         bool isPos () const noexcept;
  static bool isOdd (const Int& i) noexcept;
         bool isOdd () const noexcept;
  static bool isEven(const Int& i) noexcept;
         bool isEven() const noexcept;
  static int  sign(const Int& i);
         int  sign() const;

public: // operations
  bool operator== (const Int& opR) const noexcept;
  bool operator<  (const Int& opR) const noexcept;
  Int& operator+= (const Int& opR);
  Int& operator-= (const Int& opR);
  Int& operator*= (const Int& opR);
  Int& operator/= (const Int& opR);
  Int& operator%= (const Int& opR);

  Int  operator-  () const;

  static Int pow(const Int& opL, const Int& opR);
         Int pow(const Int& opR) const;
  static Int abs(const Int& op);
         Int abs() const;
  static Int mod(const Int& opL, const Int& opR);
         Int mod(const Int& opR) const;
  static Int invMod(const Int& base, const Int& mod);
         Int invMod(const Int& mod) const;
  static Int gcd(const Int& opL, const Int& opR);
  static Int lcm(const Int& opL, const Int& opR);
  static Int binomial(const Int& n, const Int& k);

  friend ostream& operator<<(ostream& os, const Int& i);
};


/****************************************************************************
**
*F  Int(<limbs>, <size>) . . . . . . . . . . . . .create a new integer object
**
**  Construct an integer object from the limbs at which <limbs> points. The
**  absolute value of <size> determines the number of limbs. If <size> is
**  zero, then `INTOBJ_INT(0)` is returned. Otherwise, the sign of the
**  returned integer object is determined by the sign of <size>.
**
**  Note that GAP automatically reduces and normalized the integer object,
**  i.e., it will discard any leading zeros; and if the integer fits into a
**  small integer, it will be returned as such.
*/
inline Int::Int(const GAP_UInt* limbs, GAP_Int size)
  : Int(MakeObjInt(limbs, size))
{}

/****************************************************************************
**
*F  Int(i) . . . . . . . . . . . . . create a new integer object from a C int
*F  Int(i) . . . . . . . . .create a new integer object from a C unsigned int
*F  Int(i) . . . . . . . . . . . . .create a new integer object from a C byte
*F  Int(i) . . . . . . . . create a new integer object from a C unsigned byte
**
**  The following constructors convert Int, UInt or Int8 respectively into a
**  GAP integer, either an immediate, small integer if possible or otherwise
**  a new GAP bag with TNUM T_INTPOS or T_INTNEG.
**
*!  All but one of these constructors have been disabled,  reason being that
*!  attempting to initialize a GAP integer from a C int literal will confuse
*!  the compiler, as it will use different implicit conversions of the 'int'
*!  literal which will then match different constructors  -  so the compiler
*!  will not know which constructor to select; unless you absolutely need to
*!  build 'Gap::Int' from various 'int' types, and are willing to disambigu-
*!  ate the calls, leave it as is.
*/
inline Int::Int(const GAP_Int8  i) : Int(ObjInt_Int8(i))  {}
#if 0
inline Int::Int(const GAP_Int   i) : Int(ObjInt_Int(i))   {}
inline Int::Int(const GAP_UInt  i) : Int(ObjInt_UInt(i))  {}
inline Int::Int(const GAP_UInt8 i) : Int(ObjInt_UInt8(i)) {}
#endif


/****************************************************************************
**
**  The following functions convert a GAP integer into an Int, UInt, Int8 or
**  UInt8 if it is in range. Otherwise it gives an error.
**
*!  these need to be re-worked, re-written to raise C++ exceptions, no point
*!  in braking into a GAP loop.
*/
inline Int::operator GAP_Int()   const  { return Int_ObjInt(gapObj);   }
inline Int::operator GAP_UInt()  const  { return UInt_ObjInt(gapObj);  }
inline Int::operator GAP_Int8()  const  { return Int8_ObjInt(gapObj);  }
inline Int::operator GAP_UInt8() const  { return UInt8_ObjInt(gapObj); }


/****************************************************************************
**
*F  size() . . . . . . . . . . . . . . . . .number of limbs of integer object
**
**  'size' returns the number of limbs in a large integer object.
*/
inline int Int::size() const noexcept
{
  return SIZE_INT(gapObj);
}


/****************************************************************************
**
*F  toString( <base> ) . . . . . . . . . . . convert this integer to a string
*F  <stream> << <op> . . . . . . . . . . . . . . . . .write integer to stream
*/
inline string Int::toString(const int base) const
{
  GAP_Obj gs = StringIntBase(gapObj, base);
  return string(GAP_CSTR_STRING(gs), GAP_LenString(gs));
}

inline ostream& operator<<(ostream& os, const Int& i)
{
  int base = 10;
  if      (os.flags() & ios::oct) base = 8;
  else if (os.flags() & ios::hex) base = 16;

  os << GAP_CSTR_STRING(StringIntBase(i.gapObj, base));
  return os;
}


/****************************************************************************
**
*F  isSmallInt( <obj> )  . . . . . . . . . .test if GAP object is a small int
*F  isLargeInt( <obj> )  . . . . . . . . . .test if GAP object is a large int
*F  isInt( <obj> ) . . . . . . . . . . . . . . . test if GAP object is an int
**
**  'isSmallInt' returns 'true' if 'obj' is large positive or negative integer
**  object, and 'false' for all other kinds of objects.
**
**  'isLargeInt' returns 'true' if 'obj' is large positive or negative integer
**  object, and 'false' for all other kinds of objects.
**
**  'isInt' returns 'true' if 'obj' is either a large or an immediate integer
**  object, and 'false' for all other kinds of objects.
*/
inline bool Int::isSmallInt(const Int& obj) {
  return IS_INTOBJ(obj.gapObj) == 1;
}
inline bool Int::isLargeInt(const Int& obj) {
  return IS_LARGEINT(obj.gapObj) == 1;
}
inline bool Int::isInt(const Int& obj) {
  return IS_INT(obj.gapObj) == 1;
}


/****************************************************************************
**
*F  isNeg(<int>) . . . . . . . . . . . . . .check whether integer is negative
*F  isPos(<int>) . . . . . . . . . . . . . .check whether integer is positive
**
**  'isNeg' returns 'true' if 'i' is a negative large or immediate integer
**  object, and 'false' otherwise.
**
**  'isPos' returns 'true' if 'i' is a positive large or immediate integer
**  object, and 'false' otherwise.
*/
inline bool Int::isNeg() const noexcept {
  return IS_NEG_INT(gapObj) == 1;
}
inline bool Int::isNeg(const Int& i) noexcept {
  return i.isNeg();
}

inline bool Int::isPos() const noexcept {
  return IS_POS_INT(gapObj) == 1;
}
inline bool Int::isPos(const Int& i) noexcept {
  return i.isPos();
}


/****************************************************************************
**
*F  isOdd(<int>) . . . . . . . . . . . . . . . . check whether integer is odd
*F  isEven(<int>). . . . . . . . . . . . . . . .check whether integer is even
**
**  'isOdd' returns 'true' if <int> is an odd large or immediate integer object
**  and 'false' otherwise.
**
**  'isEven' returns 'true' if <int> is an even large or immediate integer
**  object, and 'false' otherwise.
*/
inline bool Int::isOdd() const noexcept {
  return IS_ODD_INT(gapObj) == 1;
}
inline bool Int::isOdd(const Int& i) noexcept {
  return i.isOdd();
}

inline bool Int::isEven() const noexcept {
  return IS_ODD_INT(gapObj) == 0;
}
inline bool Int::isEven(const Int& i) noexcept {
  return i.isEven();
}


/****************************************************************************
**
*F  <opL> '==' <opR> . . . . . . . . . . . . . test if two integers are equal
**
**  the '==' operator returns 'true' if this integer is equal to <opR>, and
**  'false' otherwise
*/
inline bool Int::operator==(const Int& opR) const noexcept
{
  return EqInt(gapObj, opR.gapObj);
}


/****************************************************************************
**
*F  <opL> '<'  <opR> . . . . . . . . .test if an integer is less than another
**
**  the '<' operator returns 'true' if this integer is strictly less than the
**  integer <opR> and 'false' otherwise.
*/
inline bool Int::operator<(const Int& opR) const noexcept
{
  return LtInt(gapObj, opR.gapObj) == 1;
}


/****************************************************************************
**
*F  <opL> += <opR>. . . . . . . . . . . . . . . . . . . . . increment integer
*F  <opL> + <opR> . . . . . . . . . . . . . . . . . . . . sum of two integers
**
**  the '+=' operator adds <opR> to <opL>.
**  the '+' operator returns the sum of integers <opL> and <opR>.
*/
inline Int& Int::operator+=(const Int& opR)
{
  *this = Int(GAP_SumInt(gapObj, opR.gapObj));
  return *this;
}

// passing lhs by value helps optimize chained a+b+c
// otherwise, both parameters may be const references
inline Int operator+(Int opL, const Int& opR)
{
  opL += opR; // reuse compound assignment
  return opL; // return the result by value (uses move constructor)
}


/****************************************************************************
**
*F  <opL> -= <opR>. . . . . . . . . . . . . . . . . . . . . decrement integer
*F  <opL> - <opR> . . . . . . . . . . . . . . . . .difference of two integers
*F  - <op>  . . . . . . . . . . . . . . . . . .additive inverse of an integer
**
**  the '-=' operator subtracts <opR> from <opL>.
**  the '-' operator returns the difference of integers <opL> and <opR>.
**  the unary '-' operator returns the additive inverse of <op>.
*/
inline Int& Int::operator-=(const Int& opR)
{
  *this = Int(GAP_DiffInt(gapObj, opR.gapObj));
  return *this;
}
inline Int operator-(Int opL, const Int& opR)
{
  opL -= opR;
  return opL;
}
inline Int Int::operator-() const
{
  return Int(AInvInt(gapObj));
}

/****************************************************************************
**
*F  <opL> *= <opR>. . . . . . . . . . . . . . . . . . . . . .multiply integer
*F  <opL> * <opR> . . . . . . . . . . . . . . .multiplication of two integers
**
**  the '*=' multiplies the integer <opL> by <opR>.
**  the '*' operator returns the product of integers <opL> and <opR>.
*/
inline Int& Int::operator*=(const Int& opR)
{
  *this = Int(GAP_ProdInt(gapObj, opR.gapObj));
  return *this;
}
inline Int operator*(Int opL, const Int& opR)
{
  opL *= opR;
  return opL;
}


/****************************************************************************
**
*F  <opL> /= <opR>. . . . . . . . . . . . . . . . . . . . . . .divide integer
*F  <opL> / <opR> . . . . . . . . . . . . . . . . . .division of two integers
**
**  the '/=' operator divides the integer <opL> by <opR>.
**  the '/' operator returns the quotient of integers <opL> and <opR>.
*/
inline Int& Int::operator/=(const Int& opR)
{
  *this = Int(QuoInt(gapObj, opR.gapObj));
  return *this;
}
inline Int operator/(Int opL, const Int& opR)
{
  opL /= opR;
  return opL;
}


/****************************************************************************
**
*F  <opL> %= <opR>. . . . . . . . . . . . . . . . . . . . . division reminder
*F  <opL> % <opR> . . . . . . . . . . . . . . . . . .reminder of two integers
**
**  the '%=' operator yields the reminder of the division of <opL> by <opR>.
**  the '%' operator returns the reminder of integers <opL> and <opR>.
*/
inline Int& Int::operator%=(const Int& opR)
{
  *this = Int(GAP_RemInt(gapObj, opR.gapObj));
  return *this;
}
inline Int operator%(Int opL, const Int& opR)
{
  opL %= opR;
  return opL;
}


/****************************************************************************
**
*F  pow( <opL>, <opR> ) . . . . . . . . . . . . . . . . . power of an integer
**
**  'pow' returns the <opR>-th (an integer) power of the integer <opL>.
*/
//Obj PowInt(Obj opL, Obj opR);
inline Int Int::pow(const Int& opR) const
{
  return Int(PowInt(gapObj, opR.gapObj));
}
inline Int Int::pow(const Int& opL, const Int& opR)
{
  return opL.pow(opR);
}


/****************************************************************************
**
*F  abs( <op> ) . . . . . . . . . . . . . . . . .absolute value of an integer
**
**  'abs' returns the absolute value of the integer <op>.
*/
inline Int Int::abs() const
{
  return Int(AbsInt(gapObj));
}
inline Int Int::abs(const Int& op)
{
  return op.abs();
}


/****************************************************************************
**
*F  sign() . . . . . . . . . . . . . . . . . . . . . . . . sign of an integer
**
**  'sign' returns the sign of the integer <op>.
*/
inline int Int::sign() const
{
  GAP_Obj sign = SignInt(gapObj);
  if      (sign == INTOBJ_INT(0))   return 0;
  else if (sign == INTOBJ_INT(1))   return 1;
  else if (sign == INTOBJ_INT(-1))  return -1;
  else
    throw FailedOpException("Int::sign(): bad sign");
}
inline int Int::sign(const Int& op)
{
  return op.sign();
}


/****************************************************************************
**
*F  mod( <opL>, <opR> ) . . . . representative of residue class of an integer
**
**  'mod' returns the smallest positive representative of the residue class
**  of the integer <opL> modulo the integer <opR>.
*/
inline Int Int::mod(const Int& opR) const
{
  return Int(ModInt(gapObj, opR.gapObj));
}
inline Int Int::mod(const Int& opL, const Int& opR)
{
  return opL.mod(opR);
}


/****************************************************************************
**
*F  invMod( <base>, <mod> ) . . . . mult. inverse of an integer modulo another
*/
inline Int Int::invMod(const Int& mod) const
{
  return Int(InverseModInt(gapObj, mod.gapObj));
}
inline Int Int::invMod(const Int& base, const Int& mod)
{
  return base.invMod(mod);
}


/****************************************************************************
**
*F  gcd( <opL>, <opR> ) . . . . . . . . . . . . . . . . . gcd of two integers
**
**  'gcd' returns the gcd of the two integers <opL> and <opR>.
*/
inline Int Int::gcd(const Int& opL, const Int& opR)
{
  return Int(GcdInt(opL.gapObj, opR.gapObj));
}


/****************************************************************************
**
*F  lcm( <opL>, <opR> ) . . . . . . . . . . . . . . . . . lcm of two integers
**
**  'lcm' returns the lcm of the two integers <opL> and <opR>.
*/
inline Int Int::lcm(const Int& opL, const Int& opR)
{
  return Int(LcmInt(opL.gapObj, opR.gapObj));
}


/****************************************************************************
**
*F  binomial(<n>, <k>) . . . . . . . . . binomial coefficient of two integers
**
**  'binomial' returns the binomial coefficient of integers <n> and <k>.
*/
inline Int Int::binomial(const Int& n, const Int& k)
{
  return Int(BinomialInt(n.gapObj, k.gapObj));
}


} /* namespace GAP */

#endif /* LIBGAP_INT_H */
