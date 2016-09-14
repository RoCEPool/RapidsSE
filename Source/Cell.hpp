/*============================================================================*/
/* Cell.hpp - a cell in parameter/body area of word                           */
/*============================================================================*/

#pragma once

#include "Dictionaries.h"

namespace Dictionaries
{
  class Cell
  {
  public:
    union {
      StepPtr    m_pStep;
      StepPtr*   m_pCode;
      Cell*      m_pJumpLocn;  // pointer to jump location
    };
  };
  
  Cell* _pCell(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
