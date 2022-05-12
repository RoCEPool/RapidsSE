/*============================================================================*/
/* EvMgr.hpp - Event Manager                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "OperandWatch.hpp"

namespace STORE
{
  class EvMgr : public Object
  {
  public:
  
    EventHandlers* m_pEventHandlers;

    eEV m_eCurEvent;
  
    i8  m_naEventCount[EV_ENUMS];  // counts of events (operands piped-in)

    OperandWatch* m_pOperandWatch;

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
