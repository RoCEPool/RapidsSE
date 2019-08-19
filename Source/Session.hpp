/*============================================================================*/
/* Session.hpp - se session                                                   */
/*============================================================================*/
#pragma once

#include "Sessions.h"

namespace Sessions
{
  class Session : public Object
  {
  public:
    i8       m_iSessionId;       // session id
    i8       m_iPid;             // process which created this session
  
    i8       m_iPartition;       // which partition this session connects to
    DicStack m_oDicStack;        // dictionary stack
    s48K     m_sResponse;        // response
    bool     m_bSessionExiting;  // is session exiting ?
    Stack*   m_pJobStack;        // stack of jobs
    Command* m_pCommand;         // command processor

    Job*     m_pJob;             // job session currently executing
    Tx*      m_pTx;              // current transaction
  
    s4K      m_sToDo;            // prompt to do
  
    SessionState* m_pSessionState; // session state

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Session(i8 iPartition);
    ~Session(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    bool      bBeginTx(void);
    void      vCommitTx(void);
    void      vContentDump(void);
    void      vEndTx(void);
    eEC       eInit(Stz* pCatalogName, Stz* pSchemaName);
    i8        iPartition(void);
    DicStack* pPathStack(void);
      
    void      vReport(void);
    
    i8        iSlot(void);
    Tx*       pTx(void);
    void      vTx(Tx* pTx);
  
    Footer* aFooter(void);
  };
  
  Session* _pSession(void* aAddr);
  Session* _pSession(i8 iAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
