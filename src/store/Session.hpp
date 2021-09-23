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
  class Session : public Object
  {
  public:
    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // a)
  
    // -- id, name and kind
#ifdef WITH_SSNID
    SsnId m_xSessionId;  // session id in grid
#else
    Gid_t<CN_SESSION> m_xSessionId;  // session id in grid
#endif
    s32   m_sSessionNm;  // name of this session
    eSK   m_eSession;    // session kind

    // -- belongs to
    i8 m_i_Session;  // which session in sessions on this node
    i8 m_iPid;       // which process created this session

    // -- refers to
    Job* m_pSsnJob;  // job currently being executing

    // -- tags
    TagSet m_oSsnTagSet;  // session level tags
  
    // ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?

    i8       m_i_NextFree;    // next state in free chain
    
    i8       m_i_FixedPtn;    // specific partition being processed

    DicStack m_oDicStack;     // dictionary stack
    TagSet   m_oJobTagSet;    // extra tags gleaned from the command
    Input*   m_pInput;        // input processor
    Output*  m_pOutput;       // output processor

    Tx*      m_pTx;           // current transaction
  
    i8       m_iCmdNo;        // the sequence no of the request from that session

  
    Mutex    m_oMutex;        //
    bool     m_bRunning;      // api call still running?
    bool     m_bSignalled;    // calling thread has been signalled?

    void*    m_aStackBase;    // stack base of thread which made api call
    void*    m_aStackPtr;     // stack addr of thread which made api call

    // -- -- -- -- -- --
  
    s4K      m_sToDo;         // prompt to do
    
    bool     m_bHasCascade;   // is there a cascading statement to execute?
    s16KB    m_sCascade;      // the cascade statement
  
    bool     m_bSystemCloser; // is this session the system closer?
  
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

   JobId  m_tLastJobId;  // last job which was run

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
    void vOutline(s4K& sOutline);
    void vFinaliseSession(i8 i_NextFree);
    bool bHasCascade(void);
    void vHasCascade(bool bHasCascade);
    eEC  eInitaliseSession(Stz* pCatalogName, Stz* pSchemaName);
    Job* pJob(void);
    JobId tLastJobId(void);
    void vLastJobId(JobId tLastJobId);
    DicStack* pPathStack(void);
    void vReport(void);
    void vSee(void);
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
