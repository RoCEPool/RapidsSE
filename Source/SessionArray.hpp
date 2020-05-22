/*============================================================================*/
/* SessionArray.hpp - Session Array                                           */
/*============================================================================*/
#pragma once

namespace Interface
{
  class SessionArray : public Object
  {
  public:
  
    Lock m_okLock;
    i8   m_nSessionsSoFar;
    i8   m_iFirstFree;
  
    Session* m_paSession[C_MAX_SESSIONS];
  
    Footer m_xFooter;
    
   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    SessionArray(void);
   ~SessionArray(void) noexcept(false);
  
    static void vMarkApiLoc(Session* pSession, i8 iApiLoc);
    static void vMarkCall(Session* pSession, eAC eApiCall);
    static void vMarkError(Session* pSession, eEC eError);
    static void vMarkIncBegun(Session* pSession);
    static void vMarkIncCancelled(Session* pSession);
    static void vMarkIncEnded(Session* pSession);
    static void vMarkIncExecuted(Session* pSession);
    static void vMarkJob(Session* pSession, Job* aJob);
    static void vMarkJobLoc(Session* pSession, i8 iJobLoc);
    static void vMarkReturn(Session* pSession);
    static void vMarkSenLoc(Session* pSession, i8 iSenLoc);
    static void vMarkSignalled(Session* pSession);
  
    Session* pAllocSession(eSK eSssion);
    void vDump(void);
    void vFreeSession(Session* pSession);
    Session* pGetSession(i8 i_Session);
    void vMonitorPortrait(s48K& sPortrait);

    
  private:
    void vApiCall(Session* pSession, eAC eApiCall);
    void vApiError(Session* pSession, eEC eError);
    void vApiJob(Session* pSession, Job* pJob);
    void vApiLoc(Session* pSession, i8 iApiLoc);
    void vApiReturn(Session* pSession);
    void vIncBegun(Session* pSession);
    void vIncCancelled(Session* pSession);
    void vIncExecuted(Session* pSession);
    void vIncEnded(Session* pSession);
    void vJobLoc(Session* pSession, i8 iJobLoc);
    void vSenLoc(Session* pSession, i8 iSenLoc);
    void vSignalled(Session* pSession);

    Footer* aFooter(void);
  };
  
  SessionArray* _pSessionArray(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
