/*============================================================================*/
/* EventHandlers.hpp - Event Handlers                                        */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class EventHandlers : public Object
  {
  public:
  
    Word* m_paEventHandler[EV_ENUMS];  // event handlers

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    EventHandlers(void);
   ~EventHandlers(void) noexcept(false);
      
    Word* pGet(eEV eEvent);
    void  vInitialise(void);
    void  vSet(eEV eEvent, Word* pWord);

    Footer* aFooter(void);
  };

  EventHandlers* _pEventHandlers(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
