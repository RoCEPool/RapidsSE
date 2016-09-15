/*============================================================================*/
/* Cell.hpp -- Cell in Data Area of Routine Word                              */
/*============================================================================*/

#pragma once

#include "Dictionaries.h"

namespace Dictionaries
{
  class Cell
  {
  public:
    union {
      DoesStep m_pDoesStep; // pointer the "entry step" of a word
      Cell*    m_pJumpTo;   // pointer to another cell within same routine
    };
  };
  
  Cell* _pCell(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/