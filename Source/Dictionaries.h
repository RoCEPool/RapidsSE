/*============================================================================*/
/* Dictionaries.h - includes for Dictionaries namespace                       */
/*============================================================================*/

#pragma once

#include "Transactions.h"

namespace Tasks
{
  typedef NNP<Task,CN_TASK> NNP_Task;
  
}
namespace Dictionaries
{
  typedef void (*TaskStep)(Tasks::NNP_Task);
}

#include "Cell.hpp"
#include "Dic.hpp"
#include "DicPath.hpp"
#include "Word.hpp"

//-----------------------------------------------------------------------------
namespace Dictionaries
{
  void vCheckAssumptions(void);
}

using namespace Dictionaries;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
