/*============================================================================*/
/* CodeField.hpp - code field in data area of word header                     */
/*============================================================================*/

#pragma once

#include "Dictionaries.h"

namespace Tasking
{
  typedef NNP<Task,CN_TASK> NNP_Task;
  
  typedef void (*pFUN)(NNP_Task);
}

namespace Dictionaries
{
  class CodeField
  {
  public:
    void (*m_pFunc)(Tasking::NNP_Task nnpTask); // static function of tx to execute
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
