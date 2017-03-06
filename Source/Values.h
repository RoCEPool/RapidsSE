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
#include "StrVbl.hpp"
#include "StrFxd.hpp"

#include "ValSet.hpp"

typedef StrFxd<32>    s32;
typedef StrFxd<256>   s256;
typedef StrFxd<C_48K> s48K;  // note this is 48K not 48KB
typedef StrFxd<C_1M>  s1M;   // note this is 1M not 1MB

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
