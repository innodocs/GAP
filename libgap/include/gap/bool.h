/****************************************************************************
**
*A  Ovidiu Podisor
*C  Copyright © 2021 innodocs. All rights reserved.
**
*L  SPDX-License-Identifier: GPL-2.0-or-later
**
**  This file declares the functions handling booleans.
*/

#ifndef LIBGAP_BOOL_H
#define LIBGAP_BOOL_H

#include <iostream>
#include <utility>
#include "gap-system.h"


namespace Gap {

class Bool
{
private: // construction: can't create new objects of type 'Bool'
  Bool() {}

public:
  static constexpr GAP_Obj True  = GAP_True;
  static constexpr GAP_Obj False = GAP_False;
  static constexpr GAP_Obj Fail  = GAP_Fail;
};


} /* namespace Gap */
#endif /* LIBGAP_BOOL_H */