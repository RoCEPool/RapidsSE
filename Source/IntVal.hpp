/*============================================================================*/
/* IntVal.hpp - integer value                                                 */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace Values
{
  class IntVal : public Value
  {
  private:
    i8 m_iInt;
    
  public:
    void* operator new(size_t);
    
    IntVal(void);
    
    void operator =(i8 iVal);
    
    i8 iSize(void) const;
    i8 iInt(void) const;
    
    void vToCommand(char* aWorkspace);
    void vToString(char* aWorkspace);
  };
  
  IntVal* _pIntVal(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/