/*============================================================================*/
/* sRefactor.hpp -- temporary class for refactoring s                         */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace Values
{
  class sRefactor : public Value
  {
  protected:
    i8   m_nDataBytes; // size of the data area (may not be length of the string)
    // ..dataArea..
  
  public:
    sRefactor(eCN eClass);
  
    char* pDataArea(void);
  
    char* a(void);
    char* z(void);

    Footer* pFooter(void);
  
  };

  StrVal* _pStrVal(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
