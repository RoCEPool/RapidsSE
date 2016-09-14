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
  typedef void (*StepPtr)(Tasks::NNP_Task);
}

#include "BodyField.hpp"
#include "Cell.hpp"
#include "CodeField.hpp"
#include "Dictionary.hpp"
#include "DictionaryStack.hpp"
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
