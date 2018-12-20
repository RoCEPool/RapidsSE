/*============================================================================*/
/* System.h -- System namespace                                               */
/*============================================================================*/
#pragma once

#include "Assembler.h"

//-----------------------------------------------------------------------------

namespace System
{
  void vCheckAssumptions(void);
}

using namespace System;

//-----------------------------------------------------------------------------

#include "SysCall.hpp"

#include "Atomic.hpp"
#include "Decider.hpp"
#include "Dir.hpp"
#include "Env.hpp"
#include "File.hpp"
#include "Hardware.hpp"
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
