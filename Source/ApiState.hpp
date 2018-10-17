/*============================================================================*/
/* ApiState.hpp - Api State                                                   */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class ApiState : public Object
  {
  public:
  
    Lock m_xLock;
    i8   m_nSessionsSoFar;
    i8   m_iFirstFree;
  
    struct {
      Session* m_pSession;
      
      eAC      m_eApiCall;   // current / last api call
      eCN      m_eJobClass;  // current / lost job
      eEC      m_eError;     // last error code
      bool     m_bRunning;   // call still running
      Timer    m_xTimer;     // call timer

      i8       m_iNextFree;
      
    } xState[C_MAX_SESSIONS];
  
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

    void vDump(void);
  
    Footer* aFooter(void);
  };
  
  ApiState* _pApiState(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
