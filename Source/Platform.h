/*============================================================================*/
/* Platform.h - includes for Platform namespace                               */
/*============================================================================*/
#pragma once

#include "HashDefs.h"

#include "_types.h"

#include "SystemIncludes.h"
#include "System.hpp"
#include "PosixThread.hpp"

#include "Asm.h"

//-----------------------------------------------------------------------------
namespace Platform
{
  void vCheckAssumptions(void);
}

using namespace Platform;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/