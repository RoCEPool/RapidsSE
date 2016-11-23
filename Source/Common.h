/*============================================================================*/
/* Common.h - includes for Common namespace                                   */
/*============================================================================*/
#pragma once

#include "ThirdParty.h"



#include "NP.hpp"
#include "NNP.hpp"

#include "Report.hpp"
#include "SysCall.hpp"

#include "Asm.h"
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
#include "Parser.hpp"
#include "Tokeniser.hpp"

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
