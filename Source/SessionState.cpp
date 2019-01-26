/*============================================================================*/
/* ApiState.cpp - ApiState                                                    */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SessionState* Interface::_pSessionState(void* aAddr)
{
  SessionState* pSessionState = reinterpret_cast<SessionState*>(aAddr);
  
  RASSERT_TYPE(pSessionState,CN_SESSIONSTATE);
  
  return pSessionState;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SessionState::operator new(size_t)
{
  Object* pObject = HeapMem::pAllocate(CN_SESSIONSTATE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SessionState::operator delete(void* aAddr)
{
  SessionState* pSessionState = reinterpret_cast<SessionState*>(aAddr);
  
  HeapMem::vFree(pSessionState);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SessionState::SessionState(void)
: Object(CN_SESSIONSTATE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONSTATE,_FOOTER_);
  
  vInit();
  
  _checkInitialised_(CN_SESSIONSTATE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SESSIONSTATE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SessionState::~SessionState(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONSTATE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
void SessionState::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONSTATE);
 
  m_pSession   = nullptr;
  m_i_State    = -1;
  
  m_eApiFun    = AC_NONE;
  m_iApiLoc    = 0;
  m_eApiError  = EC_NONE;
  m_bRunning   = false;
  m_bSignalled = false;
  
  m_aStackBase = nullptr;
  m_aStackPtr  = nullptr;
  
  m_eJobClass  = CN_NONE;
  m_iJobLoc    = 0;
  m_iSenLoc    = 0;
  
  m_oTimer.vInit();
  m_iTotalMsec = 0;
  
  m_nBegun     = 0;
  m_nExecuted  = 0;
  m_nCancelled = 0;
  m_nEnded     = 0;
  
  m_nApiCalls  = 0;
  m_nApiErrors = 0;
  m_nJobNew    = 0;
  m_nJobDelete = 0;

  m_iNextFree  = -1;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SessionState::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONSTATE);
  
  return &m_xFooter;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
