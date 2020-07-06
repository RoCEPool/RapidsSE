/*============================================================================*/
/* Base.h -- Base namespace                                                   */
/*============================================================================*/
#pragma once

//#pragma GCC diagnostic warning "-Wunused-parameter"
//#pragma GCC diagnostic warning "-Wunused-variable"

#define EndDefine

#define _static_ EndDefine

#include "CompiledWith.h"
#include "MachineTypes.h"
#include "SystemIncludes.h"
#include "UsefulConstants.h"
#include "UserStrSizes.h"
#include "Namespaces.h"

//------------------------------------------------------------------------------

namespace Base
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------
#include "AxiomaticConstants.h"
#include "BitLib.hpp"
#include "ByteLiterals.hpp"
#include "CommonTypes.h"
#include "DebugFlags.h"
#include "Gid.h"
#include "i3.hpp"
#include "Null.hpp"
#include "WordAsBytes.h"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
