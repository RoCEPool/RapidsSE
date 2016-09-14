/*============================================================================*/
/* CodeField.hpp - code field in data area of word header                     */
/*============================================================================*/

#pragma once

#include "Dictionaries.h"

//namespace Tasks
//{
//  typedef NNP<Task,CN_TASK> NNP_Task;
//  
//}

namespace Dictionaries
{
//  typedef void (*StepPtr)(Tasks::NNP_Task);

  class CodeField
  {
  public:
    void (*m_pFunc)(Tasks::NNP_Task nnpTask); // static function of tx to execute
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
