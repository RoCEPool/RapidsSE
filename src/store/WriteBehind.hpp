/*============================================================================*/
/* WriteBehind.hpp -- Working WriteBehind Buffer                              */
/*============================================================================*/
#pragma once

#include "eTI.h"

namespace STORE { class NamedBuffer; }

namespace STORE
{
  class WriteBehind : public Object
  {
  private:
    Lock m_okLock;
  
    NamedBuffer* m_apNamedBuffer[TI_ENUMS];

    // events
    StreamDef*   m_pStreamDef_events;
    NamedBuffer* m_pNamedBuffer_events;
  

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    WriteBehind(void);
   ~WriteBehind(void) noexcept(false);
  
  public:
    Row* pAllotRow(eTI eTable);
    void vDropLock(void);
    void vFlush(void);
    void vGrabLock(void);
  
  
    void vDepositEvent(i8 iVargsBegin, ...);

  private:
    Row* pAllotEventRow(void);
    Footer* aFooter(void);
  };

  WriteBehind* _pWriteBehind(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
