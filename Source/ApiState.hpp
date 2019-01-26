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
    static void vMarkSenLoc(Session* pSession, i8 iSenLoc);
    static void vMarkSignalled(Session* pSession);

    SessionState* pAllocState(Session* pSession);
    void vDump(void);
    void vFreeState(i8 i_State);
    SessionState* pGetState(i8 i_State);
    void vIncBegun(Session* pSession);
    void vIncCancelled(Session* pSession);
    void vIncExecuted(Session* pSession);
    void vIncEnded(Session* pSession);
    void vMonitorPortrait(s48K& sPortrait);

  private:
    void vApiCall(Session* pSession, eAC eApiCall);
    void vApiError(Session* pSession, eEC eError);
    void vApiJob(Session* pSession, Job* pJob);
    void vApiLoc(Session* pSession, i8 iApiLoc);
    void vApiReturn(Session* pSession);
    void vSenLoc(Session* pSession, i8 iSenLoc);
    void vSignalled(Session* pSession);

    Footer* aFooter(void);
  };
  
  ApiState* _pApiState(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
