/*============================================================================*/
/* Threading.h - includes for Threading namespace                               */
/*============================================================================*/
#pragma once

#include "Datums.h"

#include "PosixThread.hpp"

#if !( (defined(__APPLE__) || defined(__LINUX__)) )
  #include "Asm.h"
#endif

#include "Synch.hpp"


//-----------------------------------------------------------------------------
namespace Threading
{
  void vCheckAssumptions(void);
}

using namespace Threading;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
