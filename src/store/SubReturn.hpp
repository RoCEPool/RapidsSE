/*============================================================================*/
/* SubReturn.hpp -- Sub-Pipeline Return                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class SubReturn : public Object
  {
  private:

    // must be last
    Footer m_xFooter;
 
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SubReturn(void);
   ~SubReturn(void) noexcept(false);
  
    static void vRepair(Object* pObject);
  
  private:

    Footer* aFooter(void);
  };
  
  SubReturn* _pSubReturn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
