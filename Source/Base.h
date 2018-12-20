/*============================================================================*/
/* Base.h -- Base namespace                                                   */
/*============================================================================*/
#pragma once

//#pragma GCC diagnostic warning "-Wunused-parameter"
//#pragma GCC diagnostic warning "-Wunused-variable"

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
#include "BitLib.hpp"
#include "ByteLiterals.hpp"
#include "ClassNumbers.hpp"
#include "IntLib.hpp"
#include "i3.hpp"
#include "Magnitudes.hpp"
#include "MemLib.hpp"
#include "Stz.hpp"
#include "StrLib.hpp"

const i8 C_UNKNOWN = -1;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
