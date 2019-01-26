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
  
  m_pSession   = nullptr;
  
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
  
  m_iNextFree  = -1;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SessionState::~SessionState(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONSTATE,_FOOTER_);
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
