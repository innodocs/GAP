/****************************************************************************
**
*A  Ovidiu Podisor
*C  Copyright © 2021 innodocs. All rights reserved.
**
*L  SPDX-License-Identifier: GPL-2.0-or-later
**
**  This file imports all low-level declarations from the GAP system which are
**  are needed by the C++ LibGap interface/library, and provides name aliases
**  for disamiguating name clashes.
*/

#ifndef LIBGAP_GAP_SYSTEM_H
#define LIBGAP_GAP_SYSTEM_H

extern "C" {

#include "system.h"
#include "libgap-api.h"

}

/**
 * systen.h
 */
typedef ::Char GAP_Char;
typedef ::UChar GAP_UChar;

typedef ::Int1 GAP_Int1;
typedef ::Int2 GAP_Int2;
typedef ::Int4 GAP_Int4;
typedef ::Int8 GAP_Int8;

typedef ::UInt1 GAP_UInt1;
typedef ::UInt2 GAP_UInt2;
typedef ::UInt4 GAP_UInt4;
typedef ::UInt8 GAP_UInt8;

typedef ::Int GAP_Int;
typedef ::UInt GAP_UInt;

typedef ::Obj GAP_Obj;


/****************************************************************************
**
*S  GAP_Vars . . . . . . . . . . . . . . . . . . . . . . . GAP_Vars structure
**
**  Code which uses the GAP API exposed by the 'libgap-api' header file should
**  sandwich any such calls between uses of the GAP_Enter() and GAP_Leave()
**  macro as follows:
**
**    int ok = GAP_Enter();
**    if (ok) {
**      ... // any number of calls to GAP APIs
**    }
**    GAP_Leave();
**
**  In C++, this can be better achieved with a class which calls 'GAP_Enter'
**  in the constructor, 'GAP_Leave' in the destructor, and then place a local
**  variable (guard) of this type in each section of code which uses the GAP
**  API.
**
**  The 'GAP_VARS' macro defines such a guard variable, so the above code is
**  reduced to writing
**
**    GAP_VARS
**
**  before starting to declare GAP API variables in a block of code.
**
**  This is not needed in most cases as the C and C++ stacks are one and the
**  same. If you need this support in your application, define 'SYS_MARK_TOS'
*/
#ifdef LIBGAP_MARK_TOS

struct GAP_Vars {
  GAP_Vars()    { int ok = GAP_Enter(); }
  ~GAP_Vars()   { GAP_Leave(); }
};

#define GAP_VARS    GAP_Vars __gap_vars_def;

#else

#define GAP_VARS    do {} while(0);

#endif /* LIBGAP_MARK_TOS */

#endif /* LIBGAP_GAP_SYSTEM_H */

