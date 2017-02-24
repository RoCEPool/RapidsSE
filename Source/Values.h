/*============================================================================*/
/* Values.h -- Self-Describing Values                                         */
/*============================================================================*/

#pragma once

#include "Magic.h"

#include "V.hpp"

#include "Wsp.hpp"

#include "Value.hpp"
#include "ValWsp.hpp"
#include "ValReg.hpp"

#include "BinVal.hpp"
#include "IntVal.hpp"
#include "IpVal.hpp"
#include "ObjVal.hpp"
#include "PtrVal.hpp"

#include "StrVal.hpp"
#include "StrTpl.hpp"

#include "ValSet.hpp"

typedef StrTpl<32>    s32;
typedef StrTpl<256>   s256;
typedef StrTpl<C_48K> s48K;
typedef StrTpl<C_1M>  s1M;

//-----------------------------------------------------------------------------
namespace Values
{
  void vCheckAssumptions(void);
}

using namespace Values;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
