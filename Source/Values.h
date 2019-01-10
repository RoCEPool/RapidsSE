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

#include "BcdVal.hpp"
#include "BinVal.hpp"
#include "BolVal.hpp"
#include "DteVal.hpp"
#include "FltVal.hpp"
#include "IntVal.hpp"
#include "IvlVal.hpp"
#include "IpVal.hpp"
#include "PtrVal.hpp"
#include "StrVal.hpp"
#include "TmeVal.hpp"
#include "TspVal.hpp"
#include "ValSet.hpp"

#include "String_t.hpp"

typedef String_t<32>     s32;
typedef String_t<256>    s256;
typedef String_t<C_4K>   s4K;   // note this is 4K not 48KB
typedef String_t<C_16KB> s16KB; // note this is 16KB
typedef String_t<C_17KB> s17KB; // note this is 17KB
typedef String_t<C_48K>  s48K;  // note this is 48K not 48KB
typedef String_t<C_1M>   s1M;   // note this is 1M not 1MB

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
