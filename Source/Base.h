/*============================================================================*/
/* Base.h -- Base namespace                                                   */
/*============================================================================*/
#pragma once

//#pragma GCC diagnostic warning "-Wunused-parameter"
//#pragma GCC diagnostic warning "-Wunused-variable"

#include "CompiledWith.h"
#include "MachineTypes.h"
#include "SystemIncludes.h"

//------------------------------------------------------------------------------

namespace Base
{
  void vCheckAssumptions(void);
}

using namespace Base;

//------------------------------------------------------------------------------

#include "AxiomaticConstants.h"
#include "BitLib.hpp"
#include "ByteLiterals.hpp"
#include "ForwardClasses.h"
#include "i3.hpp"
#include "Null.hpp"
#include "Structs.h"
#include "Tick.hpp"
#include "UsefulConstants.h"
#include "WordAsBytes.h"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
