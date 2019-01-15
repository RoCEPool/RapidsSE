/*============================================================================*/
/* Session.hpp - se session                                                   */
/*============================================================================*/
#pragma once

#include "Interface.h"

namespace Interface
{
  static const i8 C_MAX_SESSIONS = 1024;
  
  class Session : public Object
  {
  public:
    i8       m_iPid;             // process which created this session
    i8       m_iPartition;       // which partition this session connects to
    DicStack m_oDicStack;        // dictionary stack
    Job*     m_pJob;             // job session currently executing
    s48K     m_sResponse;        // response
    Tx*      m_pTx;              // current transaction
    bool     m_bSessionExiting;  // is session exiting ?
    Stack*   m_pJobStack;        // stack of jobs
  
    i8       m_iApiState;        // api state entry
    i8       m_nApiCalls;        // number of api calls so far
    i8       m_nApiErrors;       // number of api call in error so far
    i8       m_nJobNew;          // number of jobs new'ed so far
    i8       m_nJobDelete;       // number of jobs deleted so far

    PerformanceTrace* m_pPerformanceTrace;  // performance Trace
    CommandProcessor* m_pCommandProcessor;  // command processor

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
    
    PerformanceTrace* pPerformanceTrace(void);
    void vPerformanceTrace(PerformanceTrace* pPerformanceTrace);
  
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
