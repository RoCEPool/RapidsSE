/*============================================================================*/
/* SubReturn.hpp -- Sub-Pipeline Return                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "SubCall.hpp"

namespace STORE
{
  class SubReturn : public Object
  {
  private:
  
    SubCall* m_pSubCall;
  
    // must be last
    Footer m_xFooter;
 
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SubReturn(void);
   ~SubReturn(void) noexcept(false);
  
    static void vRepair(Object* pObject);

    bool bDecRmg(i8& nAcksWereRmg);

    SubCall* pSubCall(void);
    void vSubCall(SubCall* pSubCall);

  private:

    Footer* aFooter(void);
  };
  
  SubReturn* _pSubReturn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
