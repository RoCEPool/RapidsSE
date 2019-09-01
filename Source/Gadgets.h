/*============================================================================*/
/* Gadgets.h -- Gadgets namespace                                             */
/*============================================================================*/
#pragma once

#include "Pool.h"

//------------------------------------------------------------------------------

namespace Gadgets
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "Birth.hpp"
#include "DecaCnt.hpp"
#include "EnumFlags.hpp"
#include "FreqDist.hpp"
#include "HV.hpp"
#include "MP.hpp"
#include "NmVal.hpp"
#include "NextId.hpp"
#include "Now.hpp"
#include "SeeMe.hpp"
#include "TempName.hpp"
#include "Tick.hpp"
#include "Timer.hpp"

#include "ByteRate.hpp"
#include "Peruse.hpp"
#include "Numbat.hpp"

#include "SP_Elapsed.hpp"
#include "SP_Footer.hpp"
#include "SP_Header.hpp"
#include "SP_ReqTuple.hpp"
#include "SP_String.hpp"

namespace Tasks { typedef MP<Task,CN_TASK> MP_Task;
  typedef void (*StepFp)(Tasks::MP_Task); }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
