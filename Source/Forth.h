/*============================================================================*/
/* Forth.h - includes for Forth namespace                                     */
/*============================================================================*/
#pragma once

#include "Transactions.h"

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
#include "Dic.hpp"
#include "DicPath.hpp"
#include "DicStack.hpp"
#include "GenVar.hpp"
#include "Local.hpp"
#include "Word.hpp"

#include "Stack.hpp"

#include "LocalSet.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
