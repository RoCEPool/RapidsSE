/*============================================================================*/
/* EvMgr.hpp - Event Manager                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class EvMgr : public Object
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    EvMgr(void);
   ~EvMgr(void) noexcept(false);
        
    Footer* aFooter(void);
  };

  EvMgr* _pEvMgr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
