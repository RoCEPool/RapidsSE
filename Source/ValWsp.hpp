/*============================================================================*/
/* ValWsp.hpp - workspace for manipulating values                             */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace Values
{
  class ValWsp : public Value,
                 public Wsp<C_64K>
  {
  private:
  
  public:
    void* operator new(size_t);
    ValWsp(void);
    
    void operator =(i8 iInt);
    void operator =(Object* pObject);
    void operator =(char* pStr);
    void operator =(const char* pStr);
    
    void vAssignBinary(Object* pObject);
  };
  
  ValWsp* _pValWsp(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
