/*============================================================================*/
/* ApiState.hpp - Api State                                                   */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  struct SessionState {
    
    Session* m_pSession;
    
    eAC      m_eApiCall;   // current / last api call
    eCN      m_eJobClass;  // current / lost job
    eEC      m_eError;     // last error code
    bool     m_bRunning;   // call still running
    i8       m_iPosition;  // last reported position
    Timer    m_xTimer;     // call timer
    i8       m_iTotalMicroSecs; // total time in us
    
    i8       m_iNextFree;
  };
  
  class ApiState : public Object
  {
  public:
  
    Lock m_xLock;
    i8   m_nSessionsSoFar;
    i8   m_iFirstFree;
  
    SessionState m_xSessionState[C_MAX_SESSIONS];
  
    Footer m_xFooter;
    
   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    ApiState(void);
    ~ApiState(void);
  
    i8   iAllocState(Session* pSession);
    void vApiCall(Session* pSession, eAC eApiCall);
    void vApiError(Session* pSession, eEC eError);
    void vApiJob(Session* pSession, eCN eJobClass);
    void vApiReturn(Session* pSession);
    void vFreeState(i8 i_State);
    void vSetPosition(Session* pSession, i8 iPosition);

    void vDump(void);
  
    Footer* aFooter(void);
  };
  
  ApiState* _pApiState(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
