/*============================================================================*/
/* ValWsp.hpp - workspace for manipulating values                             */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace Values
{
  class ValWsp : public Value,
                 public WspFxd<C_64K>
  {
  private:
  
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    ValWsp(void);
    
    void operator =(i8 iInt);
    void operator =(Object* pObject);
    void operator =(char* pStr);
    void operator =(const char* pStr);
    
    i8    iAsInt(void);
    void vAssignBinary(Object* pObject);
  
    Footer* pFooter(void);
  };
  
  ValWsp* _pValWsp(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
