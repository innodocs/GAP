/****************************************************************************
**
*A  Ovidiu Podisor
*C  Copyright © 2021 innodocs. All rights reserved.
**
*L  SPDX-License-Identifier: GPL-2.0-or-later
**
**  This file declares the support for exception handling for the LibGAP API.
*/

#ifndef LIBGAP_EXCEPTION_H
#define LIBGAP_EXCEPTION_H

#include <stdexcept>
#include <sstream>
#include <tuple>

#include "../tupleprint.h"
#include "../typestring.h" // support for templates parameterized by string literals
#define _gap_ts typestring_is

namespace Gap {

/****************************************************************************
**
*E  Error . . . . . . . . . . . . . base class for the GAP C++ API exceptions
**
*/
class Error : public runtime_error
{
public:
  Error() : runtime_error("GAPError") {}
protected:
  Error(const char *tid) : runtime_error(tid) {}
};

template<typename TS, typename ...TArgs> struct ErrorT : public Error
{
  tuple<const TArgs...> args;

  ErrorT(const TArgs&... _args) : Error(TS::data()), args(_args...) {}

  const char* what() const noexcept {
    static string s;
    ostringstream os; os << "(" << Error::what() << "): " << hex << args;
    s = std::move(os.str()); return s.c_str();
  }
};

/****************************************************************************
**
*E  FailedOpException . . . . . . . . . . . . . . .raised on failed operation
**
*/
using FailedOpException = ErrorT<_gap_ts("FailedOpException"), const char*>;

} /* namespace Gap */

#endif /* LIBGAP_EXCEPTION_H */