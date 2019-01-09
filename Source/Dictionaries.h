/*============================================================================*/
/* Dictionaries.h - includes for Dictionaries namespace                       */
/*============================================================================*/
#pragma once

#include "Transactions.h"

//-----------------------------------------------------------------------------

namespace Tasks
{
  typedef MP<Task,CN_TASK> MP_Task;
}

namespace Dictionaries
{
  typedef void (*TaskStep)(Tasks::MP_Task);
}

namespace Dictionaries
{
  void vCheckAssumptions(void);
}

using namespace Dictionaries;

//-----------------------------------------------------------------------------

#include "Cell.hpp"
#include "Dic.hpp"
#include "DicPath.hpp"
#include "DicStack.hpp"
#include "Stack.hpp"

#include "Word.hpp"

#include "RegFrame.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
