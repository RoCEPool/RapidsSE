/*============================================================================*/
/* Common.h - includes for Common namespace                                   */
/*============================================================================*/
#pragma once

#include "ThirdParty.h"



#include "NP.hpp"
#include "NNP.hpp"

#include "SysCall.hpp"

#if !( (defined(__APPLE__) || defined(__LINUX__)) )
  #include "Asm.h"
#endif

#include "Atomic.hpp"
#include "Dir.hpp"
#include "Env.hpp"
#include "File.hpp"
#include "Lock.hpp"
#include "Memory.hpp"
#include "Process.hpp"
#include "Random.hpp"
#include "Scratch.hpp"
#include "Shell.hpp"
#include "Shm.hpp"
#include "String.hpp"
#include "Time_se.hpp"
#include "Timer.hpp"

#include "Since.hpp"

#include "RelationalOps.h"


//-----------------------------------------------------------------------------
namespace Common
{
  void vCheckAssumptions(void);
}

using namespace Common;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
