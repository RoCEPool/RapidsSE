/*============================================================================*/
/* Values.h -- Self-Describing Values                                         */
/*============================================================================*/

#pragma once

#include "Magic.h"

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
#include "StrFxd.hpp"

#include "ValSet.hpp"

typedef StrFxd<32>    s32;
typedef StrFxd<256>   s256;
typedef StrFxd<C_48K> s48K;
typedef StrFxd<C_1M>  s1M;

//-----------------------------------------------------------------------------
namespace Values
{
  void vCheckAssumptions(void);
}

using namespace Values;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
