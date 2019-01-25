/*============================================================================*/
/* ApiState.hpp - Api State                                                   */
/*============================================================================*/
#pragma once

namespace Interface
{
  struct SessionState {
    
    Session* m_pSession;
    
    eAC      m_eApiCall;   // current / last api call
    void*    m_aStackBase; // stack base of thread which made api call
    void*    m_aStackPtr;  // stack addr of thread which made api call

    i8       m_iApiLoc;    // last reported location in api call
    eCN      m_eJobClass;  // current / last job
    i8       m_iJobSeq;    // job seq
    eEC      m_eError;     // last error code
    bool     m_bRunning;   // call still running
    bool     m_bSignalled; // calling thread has been signalled?
    i8       m_iPosition;  // last reported position in SEN
    Timer    m_oTimer;     // call timer
    i8       m_iTotalMicroSecs; // total time in us
    
    i8       m_iNextFree;
  };
  
  class ApiState : public Object
  {
  public:
  
    Lock m_oLock;
    i8   m_nSessionsSoFar;
    i8   m_iFirstFree;
  
    SessionState m_xSessionState[C_MAX_SESSIONS];
  
    Footer m_xFooter;
    
   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    ApiState(void);
    ~ApiState(void) noexcept(false);
  
    static void vMarkApiLoc(Session* pSession, i8 iApiLoc);
    static void vMarkCall(Session* pSession, eAC eApiCall);
    static void vMarkError(Session* pSession, eEC eError);
    static void vMarkJob(Session* pSession, Job* aJob);
    static void vMarkReturn(Session* pSession);
  
    i8   iAllocState(Session* pSession);
    void vApiCall(Session* pSession, eAC eApiCall);
    void vApiError(Session* pSession, eEC eError);
    void vApiJob(Session* pSession, Job* pJob);
    void vApiReturn(Session* pSession);
    void vFreeState(i8 i_State);
    void vMonitorPortrait(s48K& sPortrait);
    void vApiLoc(Session* pSession, i8 iApiLoc);
    void vSetPosition(Session* pSession, i8 iPosition);
    void vSetSignalled(Job* pJob);

    void vDump(void);
  
    Footer* aFooter(void);
  };
  
  ApiState* _pApiState(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
