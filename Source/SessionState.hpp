/*============================================================================*/
/* SessionState.hpp - Session State                                           */
/*============================================================================*/
#pragma once

namespace Interface
{
  class SessionState : public Object
  {
  public:
    i8    m_i_State;     // session state index
  
    Session* m_pSession; // will always be set unless state is free
  
    eAC   m_eApiFun;     // current / last api function called
    i8    m_iApiLoc;     // last reported location in api function
    eEC   m_eApiError;   // last error code in api function
    bool  m_bRunning;    // api call still running?
    bool  m_bSignalled;  // calling thread has been signalled?

    void* m_aStackBase;  // stack base of thread which made api call
    void* m_aStackPtr;   // stack addr of thread which made api call

    eCN   m_eJobClass;   // current / last job
    i8    m_iJobLoc;     // last reported location in job
    i8    m_iSenLoc;     // last reported location in SEN

    Timer m_oTimer;      // call timer
    i8    m_iTotalMsec;  // total time in us
    
    i8    m_nBegun;      // number of __begun received
    i8    m_nCancelled;  // number of __cancelled received
    i8    m_nEnded;      // number of __ended received
    i8    m_nExecuted;   // number of __executed received
  
    i8    m_nApiCalls;   // number of api calls so far
    i8    m_nApiErrors;  // number of api call in error so far
    i8    m_nJobNew;     // number of jobs new'ed so far
    i8    m_nJobDelete;  // number of jobs deleted so far

    i8    m_iNextFree;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    SessionState(void);
    ~SessionState(void) noexcept(false);
  
    void vIncBegun(void);
    void vIncCancelled(void);
    void vIncExecuted(void);
    void vIncEnded(void);

    void vInit(void);
    void vTraceState(void);
  
    Footer* aFooter(void);
  };
  
  SessionState* _pSessionState(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
