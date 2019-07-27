/*============================================================================*/
/* Base.h -- Base namespace                                                   */
/*============================================================================*/
#pragma once

//#pragma GCC diagnostic warning "-Wunused-parameter"
//#pragma GCC diagnostic warning "-Wunused-variable"

#include "CompiledWith.h"
#include "MachineTypes.h"
#include "SystemIncludes.h"

//-----------------------------------------------------------------------------

namespace Base
{
  void vCheckAssumptions(void);
}

using namespace Base;

//-----------------------------------------------------------------------------

#include "UsefulConstants.h"
#include "AxiomaticConstants.h"
#include "ForwardClasses.h"
//#include "AM.hpp"

#include "BitLib.hpp"
#include "ByteLiterals.hpp"
#include "i3.hpp"
#include "Null.hpp"


#include "Structs.h"





/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
