/*============================================================================*/
/* Common.h - includes for Common namespace                                   */
/*============================================================================*/
#pragma once

#include "Platform.h"

#include "Types.h"

#include "_bytes.hpp"
#include "_debug.h"

#include "Footer.hpp"

#include "s.hpp"
#include "s32.hpp"
#include "s256.hpp"
#include "s16K.hpp"


#include "Error.hpp"
#include "NP.hpp"
#include "NNP.hpp"


#include "Asm.h"

#include "Report.hpp"
#include "SysCall.hpp"

#include "Constants.h"
#include "Memory.hpp"
#include "String.hpp"
#include "Today.hpp"
#include "Time_se.hpp"
#include "System.hpp"
#include "PosixThread.hpp"
#include "Random.hpp"
#include "Lock.hpp"
#include "Asm.h"
#include "File.hpp"
#include "Dir.hpp"
#include "Env.hpp"
#include "Shm.hpp"
#include "xxhash.hpp"
#include "process.hpp"
#include "prompt.hpp"

#include "Mersenne.hpp"
#include "Timer.hpp"
#include "Since.hpp"

//-----------------------------------------------------------------------------
namespace Common
{
  void vCheckAssumptions(void);
}

using namespace Common;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/