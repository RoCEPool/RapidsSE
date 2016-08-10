/*============================================================================*/
/* BodyField.hpp - body field in data area of word header                     */
/*============================================================================*/

#pragma once

#include "Dictionaries.h"

namespace Dictionaries
{
  class Cell;
  
  class BodyField
  {
  public:
    Cell* m_pCell; // pointer to (first) cell in parameter/body of word
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
