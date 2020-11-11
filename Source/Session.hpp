/*============================================================================*/
/* Session.hpp - se session                                                   */
/*============================================================================*/
#pragma once

#include "Sessions.h"

namespace Sessions
{
  class Session : public Focal
  {
  public:
    i8    m_i_Session;   // session index
    i8    m_i_NextFree;  // next state in free chain
  
    eSK   m_eSession;    // session kind
    
    eAC   m_eApiFun;     // current / last api function called
    i8    m_iApiLoc;     // last reported location in api function
    eEC   m_eApiError;   // last error code in api function
    eCN   m_eJobClass;   // current / last job
    i8    m_iJobLoc;     // last reported location in job
    i8    m_iSenLoc;     // last reported location in SEN
    
    bool  m_bRunning;    // api call still running?
    bool  m_bSignalled;  // calling thread has been signalled?
    bool  m_bExiting;    // is session exiting ?

    void* m_aStackBase;  // stack base of thread which made api call
    void* m_aStackPtr;   // stack addr of thread which made api call
    
    
    Timer m_oTimer;      // call timer
    i8    m_qTotalUsec;  // total-quanity time in microseconds
    
    i8    m_nBegun;      // number of __begun received
    i8    m_nCancelled;  // number of __cancelled received
    i8    m_nEnded;      // number of __ended received
    i8    m_nExecuted;   // number of __executed received
    
    i8    m_nApiCalls;   // number of api calls so far
    i8    m_nApiErrors;  // number of api call in error so far
    i8    m_nJobNew;     // number of jobs new'ed so far
    i8    m_nJobDelete;  // number of jobs deleted so far
    
    
    i8       m_iPid;             // process which created this session
  
    i8       m_i_Strand;         // which partition this session processes
    DicStack m_oDicStack;        // dictionary stack
    Stack*   m_pJobStack;        // stack of jobs

    TagSet   m_oSsnTagSet;       // session level tags

    Job*     m_pSsnJob;          // job session currently executing
    Tx*      m_pTx;              // current transaction
  
    s4K      m_sToDo;            // prompt to do
      
    Mutex    m_oSsnMutex;        //
    ReqTuple m_xReqTuple;        // request information
    

  private:

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Session(void);
   ~Session(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    bool      bBeginTx(void);
    void      vCommitTx(void);
    void      vContentDump(void);
    void      vEndTx(void);
    void      vFocalOutline(s4K& sOutline);
    eEC       eInit(Stz* pCatalogName, Stz* pSchemaName);
    i8        i_Strand(void);
    void      vStrand(i8 i_Strand);
    DicStack* pPathStack(void);
      
    void      vReport(void);
    
    Tx*       pTx(void);
    void      vTx(Tx* pTx);
  
    void vFreeSession(i8 i_NextFree);
    void vTraceSession(void);
    
    Footer* aFooter(void);
  };
  
  Session* _pSession(void* aAddr);
  Session* _pSession(i8 iAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
