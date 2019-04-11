/*============================================================================*/
/* Forth.h - includes for Forth namespace                                     */
/*============================================================================*/
#pragma once

#include "Commands.h"

//-----------------------------------------------------------------------------

namespace Tasks
{
  typedef MP<Task,CN_TASK> MP_Task;
}

namespace Forth
{
  typedef void (*StepFp)(Tasks::MP_Task);
}

namespace Forth
{
  void vCheckAssumptions(void);
}

using namespace Forth;

//-----------------------------------------------------------------------------

#include "Cell.hpp"
#include "Compiler.hpp"
#include "Dic.hpp"
#include "DicPath.hpp"
#include "DicStack.hpp"
#include "GenVar.hpp"
#include "Local.hpp"
#include "LocalSet.hpp"
#include "Routine.hpp"
#include "ForthWord.hpp"

#include "Stack.hpp"


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
