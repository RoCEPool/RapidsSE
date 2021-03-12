/*============================================================================*/
/* WriteBehind.hpp -- Working WriteBehind Buffer                              */
/*============================================================================*/
#pragma once

namespace STORE
{
  class WriteBehind : public Object
  {
  private:
    Lock m_okLock;
  
    // events
    StreamDef* m_pStreamDef_events;
    RowBuffer* m_pRowBuffer_events;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    WriteBehind(void);
   ~WriteBehind(void) noexcept(false);
  
  public:
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
