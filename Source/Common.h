/*============================================================================*/
/* Common.h -- Common namespace                                               */
/*============================================================================*/
#pragma once

#include "Values.h"

//-----------------------------------------------------------------------------

namespace Common
{
  void vCheckAssumptions(void);
}

using namespace Common;

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
#include "Magnitudes.hpp"
#include "Process.hpp"
#include "Random.hpp"
#include "Scratch.hpp"
#include "Shell.hpp"
#include "Shm.hpp"
#include "Time_se.hpp"
#include "Timer.hpp"

#include "Since.hpp"

#include "RelationalOps.h"


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
