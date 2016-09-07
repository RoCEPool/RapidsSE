/*============================================================================*/
/* Common.h - includes for Common namespace                                   */
/*============================================================================*/
#pragma once

#include "ThirdParty.h"



#include "NP.hpp"
#include "NNP.hpp"


#include "Report.hpp"
#include "SysCall.hpp"

#include "Memory.hpp"
#include "String.hpp"
#include "Today.hpp"
#include "Time_se.hpp"
#include "Random.hpp"
#include "Lock.hpp"
#include "Asm.h"
#include "File.hpp"
#include "Dir.hpp"
#include "Env.hpp"
#include "Shm.hpp"
#include "Process.hpp"

#include "Timer.hpp"
#include "Since.hpp"

#include "Parser.hpp"
#include "Tokeniser.hpp"


//-----------------------------------------------------------------------------
namespace Common
{
  void vCheckAssumptions(void);
}

using namespace Common;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/