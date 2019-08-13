/*============================================================================*/
/* Base.h -- Base namespace                                                   */
/*============================================================================*/
#pragma once

//#pragma GCC diagnostic warning "-Wunused-parameter"
//#pragma GCC diagnostic warning "-Wunused-variable"

#include "CompiledWith.h"
#include "MachineTypes.h"
#include "SystemIncludes.h"
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
#include "i3.hpp"
#include "Null.hpp"
#include "Structs.h"
#include "UsefulConstants.h"
#include "WordAsBytes.h"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
