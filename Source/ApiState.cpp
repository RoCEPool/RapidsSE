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
  
  for (i8 i_State = 0; i_State < C_MAX_SESSIONS; i_State++) {
    
    xState[i_State].m_pSession  = nullptr;
    
    xState[i_State].m_eApiCall  = AC_NONE;
    xState[i_State].m_eJobClass = CN_NONE;
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

  RASSERT(-1 != m_iFirstFree);
  
  i8 i_State = m_iFirstFree;
  
  m_iFirstFree = xState[i_State].m_iNextFree;
  
  xState[i_State].m_iNextFree = -1;
  xState[i_State].m_pSession  = pSession;

  return i_State;
}

/*----------------------------------------------------------------------------*/
/* mark call of an api                                                        */
/*----------------------------------------------------------------------------*/
void ApiState::vApiCall(Session* pSession, eAC eApiCall)
{
}

/*----------------------------------------------------------------------------*/
/* mark job of an api                                                         */
/*----------------------------------------------------------------------------*/
void ApiState::vApiJob(Session* pSession, eCN eJobClass)
{
}

/*----------------------------------------------------------------------------*/
/* mark return of an api                                                      */
/*----------------------------------------------------------------------------*/
void ApiState::vApiReturn(Session* pSession)
{
}

/*----------------------------------------------------------------------------*/
/* feee an api state entry                                                    */
/*----------------------------------------------------------------------------*/
void ApiState::vFreeState(i8 i_State)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  xState[i_State].m_pSession = nullptr;
  
  xState[i_State].m_eApiCall  = AC_NONE;
  xState[i_State].m_eJobClass = CN_NONE;
  xState[i_State].m_bRunning  = false;
  xState[i_State].m_xTimer.vReset();

  xState[i_State].m_iNextFree = m_iFirstFree;

  m_iFirstFree = i_State;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void ApiState::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  vTrace("ApiState %p:\n"_Stz,this);

  for (i8 i_State = 0; i_State < C_MAX_SESSIONS; i_State++) {
    
    if (xState[i_State].m_pSession) {
      
      if (xState[i_State].m_bRunning)
        xState[i_State].m_xTimer.vUpdate();

      vTrace("  %lld: session %p, call %s, job %s, %srunnning, time %lldms\n"_Stz,
             i_State,
             xState[i_State].m_pSession,
             aAC[xState[i_State].m_eApiCall],
             aCN[xState[i_State].m_eJobClass],
             xState[i_State].m_bRunning ? "" : "not ",
             xState[i_State].m_xTimer.iMilliseconds()
             );
    }
    else {
      vTrace("  %lld: nextfree %lld\n"_Stz,
             i_State,
             xState[i_State].m_iNextFree);
    }
  }
    

  vTrace("  firstFree %lld\n"_Stz,m_iFirstFree);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

_aFooter_(ApiState);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
