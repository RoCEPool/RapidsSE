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

#include "StrVal.hpp"  // temp

#include "ValSet.hpp"

#include "StrTpl.hpp"

#include "s32.hpp"
#include "s256.hpp"
#include "s48K.hpp"
#include "s1M.hpp"

//-----------------------------------------------------------------------------
namespace Values
{
  void vCheckAssumptions(void);
}

using namespace Values;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
