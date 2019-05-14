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
#include "Compress.hpp"
#include "DecaCnt.hpp"
#include "Dir.hpp"
#include "Env.hpp"
#include "File.hpp"
#include "Hardware.hpp"
#include "HwCounters.hpp"
#include "Lock.hpp"
#include "MemTest.hpp"
#include "Mutex.hpp"
#include "NmVal.hpp"
#include "OpSys.hpp"
#include "Pipe.hpp"
#include "Pthread.hpp"
#include "Process.hpp"
#include "Random.hpp"
#include "Shell.hpp"
#include "Shm.hpp"
#include "Shm.hpp"
#include "Since.hpp"
#include "Socket.hpp"
#include "TempName.hpp"
#include "Tsc.hpp"

#include "Time_se.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
