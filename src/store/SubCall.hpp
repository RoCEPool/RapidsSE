/*============================================================================*/
/* SubCall.hpp -- Sub-Pipeline Call                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class SubCall : public Object
  {
  private:

    // must be last
    Footer m_xFooter;
 
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SubCall(void);
   ~SubCall(void) noexcept(false);
  
    static void vRepair(Object* pObject);

  private:

    Footer* aFooter(void);
  };
  
  SubCall* _pSubCall(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
