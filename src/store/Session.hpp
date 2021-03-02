/*============================================================================*/
/* Session.hpp - se session                                                   */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eAC.h"
#include "eSK.h"

namespace STORE { class Output; class StreamDef; }

namespace STORE
{
  class Session : public Loggable
  {
  public:
    i8       m_i_Session;     // session index
    i8       m_i_NextFree;    // next state in free chain
    
    eSK      m_eSession;      // session kind
    i8       m_iPid;          // process which created this session
    i8       m_i_FixedPtn;    // specific partition being processed

    DicStack m_oDicStack;     // dictionary stack
    TagSet   m_oSsnTagSet;    // session level tags
    Input*   m_pInput;        // input processor
    Output*  m_pOutput;       // output processor

    Tx*      m_pTx;           // current transaction
    ReqTuple m_xReqTuple;     // request information
    Job*     m_pSsnJob;       // job currently being executing
    Mutex    m_oMutex;     //
    bool     m_bRunning;      // api call still running?
    bool     m_bSignalled;    // calling thread has been signalled?

    void*    m_aStackBase;    // stack base of thread which made api call
    void*    m_aStackPtr;     // stack addr of thread which made api call

    // -- -- -- -- -- --
  
    s4K      m_sToDo;         // prompt to do
    
    bool     m_bHasCascade;   // is there a cascading statement to execute?
    s16KB    m_sCascade;      // the cascade statement
  
    // -- -- -- -- -- --

    Timer m_oTimer;      // call timer
    i8    m_qTotalUsec;  // total-quanity time in microseconds
  
    eAC   m_eApiFun;     // current / last api function called
    i8    m_iApiLoc;     // last reported location in api function
    eEC   m_eApiError;   // last error code in api function
    eCN   m_eJobClass;   // current / last job
    i8    m_iJobLoc;     // last reported location in job
    i8    m_iSenLoc;     // last reported location in SEN
      
    i8    m_nBegun;      // number of __begun received
    i8    m_nCancelled;  // number of __cancelled received
    i8    m_nEnded;      // number of __ended received
    i8    m_nExecuted;   // number of __executed received
      
    i8    m_nApiCalls;   // number of api calls so far
    i8    m_nApiErrors;  // number of api call in error so far
    i8    m_nJobNew;     // number of jobs new'ed so far
    i8    m_nJobDelete;  // number of jobs deleted so far
      
  private:


    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Session(void);
   ~Session(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    bool bBeginTx(void);
    Stz* szCascade(void);
    void vCascade(Stz* szCascade);
    void vCommitTx(void);
    void vContentDump(void);
    void vEndTx(void);
    i8   i_FixedPtn(void);
    void vFixedPtn(i8 i_FixedPtn);
    void vFocalOutline(s4K& sOutline);
    void vFreeSession(i8 i_NextFree);
    bool bHasCascade(void);
    void vHasCascade(bool bHasCascade);
    eEC  eInitSession(Stz* pCatalogName, Stz* pSchemaName);
    Job* pJob(void);
    DicStack* pPathStack(void);
    void vReport(void);
    void vTraceSession(void);
    Tx*  pTx(void);
    void vTx(Tx* pTx);
  
    Footer* aFooter(void);
  };
  
  Session* _pSession(void* aAddr);
  Session* _pSession(i8 iAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
