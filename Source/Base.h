/*============================================================================*/
/* Base.h -- Base namespace                                                   */
/*============================================================================*/
#pragma once

#include "HashDefs.h"
#include "MachineTypes.h"
#include "SystemIncludes.h"

//-----------------------------------------------------------------------------

namespace Base
{
  void vCheckAssumptions(void);
}

using namespace Base;

//-----------------------------------------------------------------------------

#include "Addressing.hpp"
#include "BinaryNumbers.h"
#include "ByteLiterals.hpp"
#include "ClassNumbers.hpp"
#include "Hardware.hpp"
#include "MemLib.hpp"
#include "Str.hpp"
#include "StrLib.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
