/*============================================================================*/
/* Gadgets.h -- Gadgets namespace                                             */
/*============================================================================*/
#pragma once

#include "Library.h"

//------------------------------------------------------------------------------

namespace Gadgets
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "Birth.hpp"
#include "DecaCnt.hpp"
#include "EnumFlags.hpp"
#include "HV.hpp"
#include "MP.hpp"
#include "NextId.hpp"
#include "Now.hpp"
#include "SeeMe.hpp"
#include "TempName.hpp"
#include "Timer.hpp"

#include "ByteRate.hpp"

#include "SP_Id.hpp"

namespace Tasks { typedef MP<Task,CN_TASK> MP_Task;
  typedef void (*StepFp)(Tasks::MP_Task); }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
