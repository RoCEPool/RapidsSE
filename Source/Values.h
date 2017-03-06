/*============================================================================*/
/* Values.h -- Values namespace                                               */
/*============================================================================*/

#pragma once

#include "Magic.h"

//-----------------------------------------------------------------------------

namespace Values
{
  void vCheckAssumptions(void);
}

using namespace Values;

//-----------------------------------------------------------------------------

#include "V.hpp"

#include "Value.hpp"
#include "ValWsp.hpp"
#include "ValReg.hpp"

#include "BinVal.hpp"
#include "IntVal.hpp"
#include "IpVal.hpp"
#include "ObjVal.hpp"
#include "PtrVal.hpp"

#include "StrVal.hpp"
#include "StrVar.hpp"

#include "ValSet.hpp"

typedef StrVar<32>    s32;
typedef StrVar<256>   s256;
typedef StrVar<C_48K> s48K;  // note this is 48K not 48KB
typedef StrVar<C_1M>  s1M;   // note this is 1M not 1MB

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
