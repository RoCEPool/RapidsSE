/*============================================================================*/
/* Platform.h - includes for Platform namespace                               */
/*============================================================================*/
#pragma once

#include "Datums.h"

#include "PosixThread.hpp"

#if !( (defined(__APPLE__) || defined(__LINUX__)) )
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
