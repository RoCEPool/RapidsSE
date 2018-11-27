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

#include "Pmc.h"

#include "Atomic.hpp"

#include "Decider.hpp"
#include "Dir.hpp"
#include "Env.hpp"
#include "File.hpp"
#include "HwCounters.hpp"
#include "Lock.hpp"
#include "OpSys.hpp"
#include "Pipe.hpp"
#include "PosixThread.hpp"
#include "Process.hpp"
#include "Random.hpp"
#include "Scratch.hpp"
#include "Semaphore.hpp"
#include "Shell.hpp"
#include "Shm.hpp"
#include "Shm.hpp"
#include "Socket.hpp"
#include "Synch.hpp"
#include "Time_se.hpp"
#include "Timer.hpp"

#include "Since.hpp"
#include "Now.hpp"

#include "RelationalOps.h"


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
