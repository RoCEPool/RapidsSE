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
#include "ClassNumbers.hpp"
#include "IntLib.hpp"
#include "i3.hpp"
#include "Line.hpp"
#include "Magnitudes.hpp"
#include "MemLib.hpp"
#include "Null.hpp"
#include "Stz.hpp"
#include "StrLib.hpp"


#include "AxiomaticConstants.h"
#include "LimitsFormats.h"
#include "Structs.h"


#include "Dtm.hpp"

#include "Word.hpp"
#include "DoubleWord.hpp"
#include "QuadWord.hpp"
#include "CacheLine.hpp"
#include "SpaceUnit.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
