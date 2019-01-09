/*============================================================================*/
/* Values.h -- Values namespace                                               */
/*============================================================================*/

#pragma once

#include "Datums.h"

//-----------------------------------------------------------------------------

namespace Values
{
  void vCheckAssumptions(void);
}

using namespace Values;

//-----------------------------------------------------------------------------


#include "Value.hpp"
#include "ValWrk.hpp"
#include "ValReg.hpp"

#include "BcdVal.hpp"
#include "BinVal.hpp"
#include "BolVal.hpp"
#include "DteVal.hpp"
#include "FltVal.hpp"
#include "IntVal.hpp"
#include "IvlVal.hpp"
#include "IpVal.hpp"
#include "ObjVal.hpp"
#include "PtrVal.hpp"
#include "StrVal.hpp"
#include "StrVar.hpp"
#include "TmeVal.hpp"
#include "TspVal.hpp"

#include "ValSet.hpp"

#include "ValLib.hpp"

typedef StrVar<32>     s32;
typedef StrVar<256>    s256;
typedef StrVar<C_4K>   s4K;   // note this is 4K not 48KB
typedef StrVar<C_16KB> s16KB; // note this is 16KB
typedef StrVar<C_17KB> s17KB; // note this is 17KB
typedef StrVar<C_48K>  s48K;  // note this is 48K not 48KB
typedef StrVar<C_1M>   s1M;   // note this is 1M not 1MB

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
