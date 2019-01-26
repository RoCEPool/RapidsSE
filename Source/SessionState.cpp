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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONSTATE,_FOOTER_);
 
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
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void SessionState::vTraceState(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONSTATE,_FOOTER_);
  
  if (m_bRunning)
    m_oTimer.vUpdate();
  
  i8 nBytes;
  if (m_aStackBase >= m_aStackPtr)
    nBytes = nSub(m_aStackBase,m_aStackPtr);
  else
    nBytes = nSub(m_aStackPtr,m_aStackBase);
  
  vTrace("  %lld: session %p base %llx ptr %llx (%lldB)\n"
         "     apiFun %s apiLoc %lld job %s jobLoc %lld senLoc %lld \n"
         "     %s %s apiError %s time %lldus\n"
         "     dist2PC: begun %lld executed %lld cancelled %lld ended %lld\n"
         "     #calls %lld, #errors %lld, #jobNew %lld #jobDelete %lld total %lldus\n"_Stz,
         
         m_i_State,
         m_pSession,
         m_aStackBase,
         m_aStackPtr,
         nBytes,
         
         aAC[m_eApiFun],
         m_iApiLoc,
         aCN[m_eJobClass],
         m_iJobLoc,
         m_iSenLoc,
         
         m_bRunning   ? "running"   : "completed",
         m_bSignalled ? "signalled" : "not signalled",
         aEC[m_eApiError],
         m_oTimer.iMicroseconds(),
         
         m_nBegun,
         m_nExecuted,
         m_nCancelled,
         m_nEnded,
         
         m_nApiCalls,
         m_nApiErrors,
         m_nJobNew,
         m_nJobDelete,
         m_iTotalMsec
         );
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(SessionState);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
