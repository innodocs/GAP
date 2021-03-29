/****************************************************************************
**
*A  Ovidiu Podisor
*C  Copyright © 2021 innodocs. All rights reserved.
**
*L  SPDX-License-Identifier: GPL-2.0-or-later
**
**  This file declares the base class of the GAP C++ class hierarchy.
*/

#ifndef LIBGAP_OBJ_H
#define LIBGAP_OBJ_H

#include <string>
#include <iostream>

#include "gap-system.h"

namespace Gap {

/****************************************************************************
**
*C  Obj . . . . . . . . . . . . . . . . base class of the GAP class hierarchy
**
**  The class defines the construction/copy/move semantics and rules enabling
**  references to garbage-collected GAP objects from C++ code.
**
**  For the case when the GAP garbage collector will scan the C++ stack,  the
**  copy/move methods are trivial, as nothing needs to be done.  For the case
**  when the application has custom/multiple stacks (e.g. multi-threaded app,
**  etc), a custom callback for marking bags has to be implemented and
**  provided to 'Gap::Init'.
**
*!  TODO: implement alternative management scheme for GAP references based on
*!  std:shared_ptr
*/
class Obj
{
protected:
  GAP_Obj gapObj;

protected: // construction from GAP object reference, non-public
  explicit Obj(GAP_Obj _gapObj);

  static const GAP_Obj getGapObj(const Obj& obj) { return obj.gapObj; }
  template<typename T1, typename T2, typename std::enable_if<std::is_base_of<Obj, T1>::value>::type* = nullptr>
  static const T1 makeObj(const GAP_Obj gapObj) { return T1::template makeObj<T2>(gapObj); }

public:    // construction, assignement (copy, move)
  Obj(const Obj& obj);
  Obj(Obj&& obj) noexcept;

  Obj& operator= (const Obj& obj);
  Obj& operator= (Obj&& obj);

  string toString() const;

public: // operations
  bool operator==(const Obj& opR) const noexcept;

  friend ostream& operator<<(ostream& os, const Obj& op);
};


inline Obj::Obj(GAP_Obj _gapObj)
  : gapObj(_gapObj)
{}

inline Obj::Obj(const Obj& obj)
  : gapObj(obj.gapObj)
{}

inline Obj::Obj(Obj&& obj) noexcept
  : gapObj(obj.gapObj) //std::exchange(obj.gapObj, 0))
{}

Obj& Obj::operator= (const Obj& obj)
{
  if (this != &obj)
    gapObj = obj.gapObj;
  return *this;
}

Obj& Obj::operator= (Obj&& obj)
{
  if (this != &obj)
    gapObj = obj.gapObj;
  return *this;
}

/****************************************************************************
**
*F  <opL> '==' <opR> . . . . . . . . . . . . .test if two rationals are equal
**
**  the '==' operator  returns 'true'  if this object is equal to <opR>,  and
**  'false' otherwise.
*/
inline bool Obj::operator==(const Obj& opR) const noexcept
{
  return GAP_EQ(gapObj, opR.gapObj) == 1;
}


/****************************************************************************
**
*F  toString( <base> ) . . . . . . . . . . . convert this integer to a string
*F  <stream> << <op> . . . . . . . . . . . . . . . . .write integer to stream
*/
inline string Obj::toString() const
{
  return string();
}
inline ostream& operator<<(ostream& os, const Obj& op)
{
  return os;
}


/****************************************************************************
**
*C  Init( <args> ) . . . . . . . . . . . . . . . . .initialise the GAP system
**
**  This function has to be called before any other GAP functions, ideally at
**  or close to the start of your 'main' function.
*/
typedef GAP_CallbackFunc CallbackFunc;

inline void Init(int argc, char *argv[],
                 CallbackFunc markBagsCallback = NULL,
                 CallbackFunc errorCallback = NULL,
                 bool handleSignals = false)
{
  GAP_Initialize(argc, argv, markBagsCallback, errorCallback, handleSignals?1:0);
}

} /* namespace Gap */

#endif /* LIBGAP_OBJ_H */