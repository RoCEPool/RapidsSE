/*============================================================================*/
/* Platform.h - includes for Platform namespace                               */
/*============================================================================*/
#pragma once

#include "Datums.h"

#include "System.hpp"
#include "PosixThread.hpp"

#if !( (defined(__APPLE__) || defined(__LINUX__)) )
  #include "Asm.h"
#endif

#include "Synch.hpp"
#include "Addressing.hpp"

#include "ByteLiterals.hpp"

//-----------------------------------------------------------------------------
namespace Platform
{
  void vCheckAssumptions(void);
}

using namespace Platform;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
