/*============================================================================*/
/* IpVal.hpp - instruction pointer value                                      */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace Dictionaries
{
  class Cell;
}

namespace Values
{
  class IpVal : public Value
  {
  private:
    Dictionaries::Cell* m_pPtr;
        
  public:
    void* operator new(size_t);
    
    IpVal(void);
    
    void operator =(Dictionaries::Cell* pPtr);
    
    i8      nSize(void) const;
    Dictionaries::Cell* pIpPtr(void) const;
    
    void vToCommand(char* aWorkspace);
    void vToString(char* aWorkspace);
  };
  
  IpVal* _pIpVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
