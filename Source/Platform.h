/*============================================================================*/
/* Platform.h - includes for Platform namespace                               */
/*============================================================================*/
#pragma once

#include "HashDefs.h"

#include "_types.h"

#include "SystemIncludes.h"
#include "System.hpp"
#include "PosixThread.hpp"

#if !( (defined(__APPLE__) || defined(__LINUX)) )
  #include "Asm.h"
#endif

#include "Synch.hpp"

//-----------------------------------------------------------------------------
namespace Platform
{
  void vCheckAssumptions(void);
}

using namespace Platform;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
