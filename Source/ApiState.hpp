/*============================================================================*/
/* ApiState.hpp - Api State                                                   */
/*============================================================================*/
#pragma once

namespace Interface
{
  class ApiState : public Object
  {
  public:
  
    Lock m_oLock;
    i8   m_nSessionsSoFar;
    i8   m_iFirstFree;
  
    SessionState m_oSessionState[C_MAX_SESSIONS];
  
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
    void vSenLoc(Session* pSession, i8 iPosition);
    void vSetSignalled(Job* pJob);
  
    void vIncBegun(i8 i_Session);
    void vIncCancelled(i8 i_Session);
    void vIncExecuted(i8 i_Session);
    void vIncEnded(i8 i_Session);

    void vDump(void);

    Footer* aFooter(void);
  };
  
  ApiState* _pApiState(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
