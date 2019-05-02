/*============================================================================*/
/* Base.h -- Base namespace                                                   */
/*============================================================================*/
#pragma once

//#pragma GCC diagnostic warning "-Wunused-parameter"
//#pragma GCC diagnostic warning "-Wunused-variable"

#include "CompiledWith.h"
#include "MachineTypes.h"
#include "SystemIncludes.h"
#include "SystemResources.h"

//-----------------------------------------------------------------------------

namespace Base
{
  void vCheckAssumptions(void);
  void vTraceWith(void);
}

using namespace Base;

//-----------------------------------------------------------------------------

#include "BitLib.hpp"
#include "ByteLiterals.hpp"
#include "CacheLine.hpp"
#include "ClassNumbers.hpp"
#include "DoubleWord.hpp"
#include "IntLib.hpp"
#include "i3.hpp"
#include "Magnitudes.hpp"
#include "MemLib.hpp"
#include "QuadWord.hpp"
#include "Stz.hpp"
#include "StrLib.hpp"
#include "Word.hpp"

const i8 C_UNKNOWN = -1;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
