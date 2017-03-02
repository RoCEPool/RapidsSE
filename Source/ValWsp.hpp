/*============================================================================*/
/* ValWsp.hpp - workspace for manipulating values                             */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace Values
{
  class ValWsp : public Value
  {
  private:
    WspFxd<C_64K> m_xWsp;
    Footer        m_xFooter;
  
  public:
    void* operator new(size_t);
    ValWsp(void);
    
    void operator =(i8 iInt);
    void operator =(Object* pObject);
    void operator =(char* pStr);
    void operator =(const char* pStr);
    
    i8    iAsInt(void);
    void vAssignBinary(Object* pObject);
  
    char* aWspArea(void);
  
    Footer* pFooter(void);
  };
  
  ValWsp* _pValWsp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
