/*============================================================================*/
/* System.h -- System namespace                                               */
/*============================================================================*/
#pragma once

#include "Values.h"

//-----------------------------------------------------------------------------

namespace System
{
  void vCheckAssumptions(void);
}

using namespace System;

//-----------------------------------------------------------------------------

#include "SysCall.hpp"

#if !( (defined(__APPLE__) || defined(__LINUX__)) )
  #include "Asm.h"
#endif

#include "Atomic.hpp"
#include "Dir.hpp"
#include "Env.hpp"
#include "File.hpp"
#include "Lock.hpp"
#include "Locks.hpp"
#include "Magnitudes.hpp"
#include "Semaphore.hpp"
#include "OpSys.hpp"
#include "PosixThread.hpp"
#include "Process.hpp"
#include "Random.hpp"
#include "Scratch.hpp"
#include "Shell.hpp"
#include "Shm.hpp"
#include "Synch.hpp"
#include "Time_se.hpp"
#include "Timer.hpp"

#include "Since.hpp"

#include "RelationalOps.h"


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
