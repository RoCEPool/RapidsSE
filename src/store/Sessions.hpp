/*============================================================================*/
/* Sessions.hpp - Session Array                                           */
/*============================================================================*/
#pragma once

#include "eAC.h"
#include "eSK.h"

namespace STORE
{
  class Sessions : public Object
  {
  public:
  
    Lock m_okLock;
    i8   m_nSessionsSoFar;
    i8   m_iFirstFree;
  
    Session* m_pInternalSession;

    Session* m_paSession[C_MAX_SESSIONS];
  
    SkipListNL* m_pSsnSkipList; // skip list of sessions (ordered by session id)

    Footer m_xFooter;
    
   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Sessions(void);
   ~Sessions(void) noexcept(false);
  
    static void vMarkApiLoc(Session* pSession, i8 iApiLoc);
    static void vMarkCall(Session* pSession, eAC eApiCall);
    static void vMarkError(Session* pSession, eEC eError);
    static void vMarkIncBegun(Session* pSession);
    static void vMarkIncCancelled(Session* pSession);
    static void vMarkIncEnded(Session* pSession);
    static void vMarkIncExecuted(Session* pSession);
    static void vMarkReturn(Session* pSession);
    static void vMarkSenLoc(Session* pSession, i8 iSenLoc);
    static void vMarkSignalled(Session* pSession);
  
    void     vAllocInternal(void);
    Session* pAllocSession(eSK eSssion);
  
    void vDump(void);
    void vFreeSession(Session* pSession);
    Session* pGetSession(i8 i_Session);
  
    Session* pInternalSession(void);

    
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
  
  Sessions* _pSessions(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
