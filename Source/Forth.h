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
  typedef void (*TaskStep)(Tasks::MP_Task);
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
#include "Register.hpp"
#include "ValWrk.hpp"
#include "Stack.hpp"
#include "Word.hpp"

#include "RegFrame.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
