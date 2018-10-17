/*============================================================================*/
/* ApiState.cpp - ApiState                                                    */
/*============================================================================*/

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ApiState* StorageEngine::_pApiState(void* aAddr)
{
  ApiState* pArg = reinterpret_cast<ApiState*>(aAddr);
  
  RASSERT( ! pArg || CN_APISTATE == pArg->eClass() );
 
  return pArg;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ApiState::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = HeapMem::pAllocate(CN_APISTATE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ApiState::operator delete(void* aAddress)
{
  ApiState* pApiState = reinterpret_cast<ApiState*>(aAddress);
  
  HeapMem::vFree(pApiState);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ApiState::ApiState(void)
: Object(CN_APISTATE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  m_xLock.vInitLock(LK_API_STATE);
  m_nSessionsSoFar = 0;
  m_iFirstFree = 0;

  for (i8 i_State = 0; i_State < C_MAX_SESSIONS; i_State++) {
    
    xState[i_State].m_pSession  = nullptr;
    
    xState[i_State].m_eApiCall  = AC_NONE;
    xState[i_State].m_eJobClass = CN_NONE;
    xState[i_State].m_eError    = EC_NONE;
    xState[i_State].m_bRunning  = false;
    xState[i_State].m_xTimer.vInit();

    xState[i_State].m_iNextFree = i_State + 1;
  }
  xState[C_MAX_SESSIONS - 1].m_iNextFree = -1;
  
  m_iFirstFree = 0;

  _checkInitialised_(CN_APISTATE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ApiState::~ApiState(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allocate an api state entry                                                */
/*----------------------------------------------------------------------------*/
i8 ApiState::iAllocState(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);

  m_xLock.vGrabWrite();
  
  RASSERT(-1 != m_iFirstFree);
  
  i8 i_State = m_iFirstFree;
  
  m_iFirstFree = xState[i_State].m_iNextFree;
  
  xState[i_State].m_iNextFree = -1;
  xState[i_State].m_pSession  = pSession;

  m_nSessionsSoFar++;
  
  m_xLock.vDropWrite();

  return i_State;
}

/*----------------------------------------------------------------------------*/
/* mark call of an api                                                        */
/*----------------------------------------------------------------------------*/
void ApiState::vApiCall(Session* pSession, eAC eApiCall)
{
  i8 i_State = pSession->m_iApiState;
  
  xState[i_State].m_bRunning = true;
  xState[i_State].m_eApiCall = eApiCall;
  
  xState[i_State].m_xTimer.vStart();
  
  pSession->m_nApiCalls++;
}

/*----------------------------------------------------------------------------*/
/* mark error return of an api                                                */
/*----------------------------------------------------------------------------*/
void ApiState::vApiError(Session* pSession, eEC eError)
{
  i8 i_State = pSession->m_iApiState;
  
  xState[i_State].m_bRunning = false;
  xState[i_State].m_eError   = eError;
  
  xState[i_State].m_xTimer.vUpdate();

  pSession->m_nApiErrors++;
}

/*----------------------------------------------------------------------------*/
/* mark job of an api                                                         */
/*----------------------------------------------------------------------------*/
void ApiState::vApiJob(Session* pSession, eCN eJobClass)
{
  i8 i_State = pSession->m_iApiState;
  
  xState[i_State].m_eJobClass = eJobClass;
  
  pSession->m_nJobNew++;
}

/*----------------------------------------------------------------------------*/
/* mark return of an api                                                      */
/*----------------------------------------------------------------------------*/
void ApiState::vApiReturn(Session* pSession)
{
  i8 i_State = pSession->m_iApiState;
  
  xState[i_State].m_bRunning = false;
  
  xState[i_State].m_xTimer.vUpdate();
}

/*----------------------------------------------------------------------------*/
/* feee an api state entry                                                    */
/*----------------------------------------------------------------------------*/
void ApiState::vFreeState(i8 i_State)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  m_xLock.vGrabWrite();
  
  xState[i_State].m_pSession = nullptr;
  
  xState[i_State].m_eApiCall  = AC_NONE;
  xState[i_State].m_eJobClass = CN_NONE;
  xState[i_State].m_eError    = EC_NONE;
  xState[i_State].m_bRunning  = false;
  
  xState[i_State].m_xTimer.vReset();

  xState[i_State].m_iNextFree = m_iFirstFree;

  m_iFirstFree = i_State;
  
  m_xLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void ApiState::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  m_xLock.vGrabRead();

  vTrace("ApiState %p SessionsSoFar %lld:\n"_Stz,this,m_nSessionsSoFar);

  for (i8 i_State = 0; i_State < C_MAX_SESSIONS; i_State++) {
    
    if (xState[i_State].m_pSession) {
      
      Session* pSession = xState[i_State].m_pSession;
      
      if (xState[i_State].m_bRunning)
        xState[i_State].m_xTimer.vUpdate();

      vTrace("  %lld: session %p, call %s, job %s, error %s, %s, time %lldms\n"_Stz,
             i_State,
             xState[i_State].m_pSession,
             aAC[xState[i_State].m_eApiCall],
             aCN[xState[i_State].m_eJobClass],
             aEC[xState[i_State].m_eError],
             xState[i_State].m_bRunning ? "running" : "completed",
             xState[i_State].m_xTimer.iMilliseconds()
             );
      vTrace("     #calls %lld, #errors %lld, #jobNew %lld, #jobDelete %lld\n"_Stz,
             pSession->m_nApiCalls,
             pSession->m_nApiErrors,
             pSession->m_nJobNew,
             pSession->m_nJobDelete);
    }
    else {
      vTrace("  %lld: nextfree %lld\n"_Stz,
             i_State,
             xState[i_State].m_iNextFree);
    }
  }
  
  vTrace("  firstFree %lld\n"_Stz,m_iFirstFree);
  
  m_xLock.vDropRead();
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

_aFooter_(ApiState);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
