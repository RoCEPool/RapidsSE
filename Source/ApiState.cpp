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
ApiState* Interface::_pApiState(void* aAddr)
{
  ApiState* pApiState = reinterpret_cast<ApiState*>(aAddr);
  
  RASSERT_TYPE(pApiState,CN_APISTATE);
 
  return pApiState;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ApiState::operator new(size_t)
{
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
  
  m_okLock.vInitLock(LK_API_STATE);
  m_nSessionsSoFar = 0;
  m_iFirstFree = 0;

  for (i8 i_State = 0; i_State < C_MAX_SESSIONS; i_State++) {
    SessionState* pState = &m_oSessionState[i_State];
    pState->m_i_State = i_State;
    pState->m_i_NextFree = i_State + 1;
  }
  m_oSessionState[C_MAX_SESSIONS - 1].m_i_NextFree = -1;
  
  m_iFirstFree = 0;

  _checkInitialised_(CN_APISTATE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ApiState::~ApiState(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* mark location of an api                                                    */
/*----------------------------------------------------------------------------*/
void ApiState::vMarkApiLoc(Session* pSession, i8 iApiLoc)
{
  gpAttachMap->m_pApiState->vApiLoc(pSession,iApiLoc);
}

/*----------------------------------------------------------------------------*/
/* mark call of an api                                                        */
/*----------------------------------------------------------------------------*/
void ApiState::vMarkCall(Session* pSession, eAC eApiCall)
{
  gpAttachMap->m_pApiState->vApiCall(pSession,eApiCall);
}

/*----------------------------------------------------------------------------*/
/* mark error of an api                                                       */
/*----------------------------------------------------------------------------*/
void ApiState::vMarkError(Session* pSession, eEC eError)
{
  gpAttachMap->m_pApiState->vApiError(pSession,eError);
}

/*----------------------------------------------------------------------------*/
/* mark another __begun job has been received                                 */
/*----------------------------------------------------------------------------*/
void ApiState::vMarkIncBegun(Session* pSession)
{
  gpAttachMap->m_pApiState->vIncBegun(pSession);
}

/*----------------------------------------------------------------------------*/
/* mark another __cancelled job has been received                             */
/*----------------------------------------------------------------------------*/
void ApiState::vMarkIncCancelled(Session* pSession)
{
  gpAttachMap->m_pApiState->vIncCancelled(pSession);
}

/*----------------------------------------------------------------------------*/
/* mark another __ended job has been received                                 */
/*----------------------------------------------------------------------------*/
void ApiState::vMarkIncEnded(Session* pSession)
{
  gpAttachMap->m_pApiState->vIncEnded(pSession);
}

/*----------------------------------------------------------------------------*/
/* mark another __executed job has been received                              */
/*----------------------------------------------------------------------------*/
void ApiState::vMarkIncExecuted(Session* pSession)
{
  gpAttachMap->m_pApiState->vIncExecuted(pSession);
}

/*----------------------------------------------------------------------------*/
/* mark job of an api                                                         */
/*----------------------------------------------------------------------------*/
void ApiState::vMarkJob(Session* pSession, Job* pJob)
{
  gpAttachMap->m_pApiState->vApiJob(pSession,pJob);
}

/*----------------------------------------------------------------------------*/
/* mark job location of an api                                                */
/*----------------------------------------------------------------------------*/
void ApiState::vMarkJobLoc(Session* pSession, i8 iJobLoc)
{
  gpAttachMap->m_pApiState->vJobLoc(pSession,iJobLoc);
}

/*----------------------------------------------------------------------------*/
/* mark return of an api                                                      */
/*----------------------------------------------------------------------------*/
void ApiState::vMarkReturn(Session* pSession)
{
  gpAttachMap->m_pApiState->vApiReturn(pSession);
}

/*----------------------------------------------------------------------------*/
/* mark sen location of an api                                                */
/*----------------------------------------------------------------------------*/
void ApiState::vMarkSenLoc(Session* pSession, i8 iSenLoc)
{
  gpAttachMap->m_pApiState->vApiLoc(pSession,iSenLoc);
}

/*----------------------------------------------------------------------------*/
/* mark that job has had its mutex signalled                                  */
/*----------------------------------------------------------------------------*/
void ApiState::vMarkSignalled(Session* pSession)
{
  gpAttachMap->m_pApiState->vSignalled(pSession);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allocate an api state entry                                                */
/*----------------------------------------------------------------------------*/
SessionState* ApiState::pAllocState(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  m_okLock.vGrabWrite();
  
  RASSERT(-1 != m_iFirstFree);
  
  i8 i_State = m_iFirstFree;
  SessionState* pState = &m_oSessionState[i_State];
  
  m_iFirstFree = pState->m_i_NextFree;
    
  pState->vAllocState(pSession);
  
  m_nSessionsSoFar++;
  
  m_okLock.vDropWrite();
  
  return pState;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void ApiState::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  m_okLock.vGrabRead();
  
  vTrace("ApiState %p SessionsSoFar %lld:\n"_Stz,this,m_nSessionsSoFar);
  
  for (i8 i_Session = 0; i_Session < C_MAX_SESSIONS; i_Session++) {
    SessionState* pState = &m_oSessionState[i_Session];
    Session* pSession = pState->m_pSession;
    if (pSession)
      pState->vTraceState();
  }
  
  vTrace("  firstFree %lld\n"_Stz,m_iFirstFree);
  vTrace("  DdlLock %llx\n"_Stz,gpAttachMap->m_okDdlLock.uLockWord());
  
  m_okLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* free an api state entry                                                    */
/*----------------------------------------------------------------------------*/
void ApiState::vFreeState(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  SessionState* pState = pSession->m_pSessionState;
  
  i8 i_FreedState = pState->m_i_State;
  
  m_okLock.vGrabWrite();
  
  pState->vFreeState(m_iFirstFree);
  
  m_iFirstFree = i_FreedState;
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* get an api state entry                                                     */
/*----------------------------------------------------------------------------*/
SessionState* ApiState::pGetState(i8 i_State)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  SessionState* pState = &m_oSessionState[i_State];
  
  return pState;
}

/*----------------------------------------------------------------------------*/
/* get a portrait of the apistate vector                                      */
/*----------------------------------------------------------------------------*/
void ApiState::vMonitorPortrait(s48K& sPortrait)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE);
  
  s48K sTasksPortrait;
  
  sPortrait = ""_Stz;
  sPortrait.vConcat("ApiState:\n"_Stz);
  
  m_okLock.vGrabRead();
  
  for (i8 i_Session = 0; i_Session < C_MAX_SESSIONS; i_Session++) {
    
    SessionState* aSessionState = &m_oSessionState[i_Session];
    if (! aSessionState->m_pSession)
      continue;
    
    if (aSessionState->m_bRunning)
      aSessionState->m_oTimer.vUpdate();
    
    sPortrait.vCatSprint("  [%2lld] #calls %lld job %s call %s (%s); "
                         "loc %lld seq %lld pos %lld\n"_Stz,
                         
                         i_Session,
                         
                         aSessionState->m_nApiCalls,
                         aCN[aSessionState->m_eJobClass],
                         aAC[aSessionState->m_eApiFun],
                         aSessionState->m_bRunning ? "running" : "completed",
                         
                         aSessionState->m_iApiLoc,
                         aSessionState->m_iJobLoc,
                         aSessionState->m_iSenLoc
                         
                         );
  }
  
  m_okLock.vDropRead();
  
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* mark call of an api                                                        */
/*----------------------------------------------------------------------------*/
void ApiState::vApiCall(Session* pSession, eAC eApiCall)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  SessionState* pState = pSession->m_pSessionState;

  pState->m_eApiFun    = eApiCall;
  pState->m_iApiLoc    = 0;
  pState->m_eApiError  = EC_NONE;
  pState->m_bRunning   = true;
  pState->m_bSignalled = false;

  pState->m_aStackBase = Pthread::aStackBase();
  pState->m_aStackPtr  = Pthread::aStackPointer();

  pState->m_eJobClass  = CN_NONE;
  pState->m_iJobLoc  = 0;
  pState->m_iSenLoc  = 0;
  
  pState->m_oTimer.vStart();
  
  pState->m_nApiCalls++;
}

/*----------------------------------------------------------------------------*/
/* mark error return of an api                                                */
/*----------------------------------------------------------------------------*/
void ApiState::vApiError(Session* pSession, eEC eError)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  SessionState* pState = pSession->m_pSessionState;

  pState->m_bRunning  = false;
  pState->m_eApiError = eError;
  
  pState->m_oTimer.vUpdate();
  pState->m_iTotalMsec += pState->m_oTimer.iMicroseconds();

  pState->m_nApiErrors++;
}

/*----------------------------------------------------------------------------*/
/* mark job of an api                                                         */
/*----------------------------------------------------------------------------*/
void ApiState::vApiJob(Session* pSession, Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  SessionState* pState = pSession->m_pSessionState;

  pState->m_eJobClass = pJob->eClass();

  pState->m_nJobNew++;
  
  pJob->m_i_State = pState->m_i_State;
}

/*----------------------------------------------------------------------------*/
/* set last API location                                                      */
/*----------------------------------------------------------------------------*/
void ApiState::vApiLoc(Session* pSession, i8 iLocation)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  if (! pSession)
    return;
  
  SessionState* pState = pSession->m_pSessionState;
  
  pState->m_iApiLoc = iLocation;
}

/*----------------------------------------------------------------------------*/
/* mark return of an api                                                      */
/*----------------------------------------------------------------------------*/
void ApiState::vApiReturn(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  SessionState* pState = pSession->m_pSessionState;

  pState->m_bRunning = false;
  pState->m_oTimer.vUpdate();
  pState->m_iTotalMsec += pState->m_oTimer.iMicroseconds();

//  if (xState[i_Session].m_eApiFun == AC_FetchNextBuffer)
//    vTrace("Timer start: %lld; Timer update: %lld Time: %lldus\n"_Stz,
//           pState->m_oTimer.iStart(),
//           pState->m_oTimer.iUpdate(),
//           pState->m_oTimer.iMicroseconds());
}

/*----------------------------------------------------------------------------*/
/* increment number of __begun received                                       */
/*----------------------------------------------------------------------------*/
void ApiState::vIncBegun(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  SessionState* pState = pSession->m_pSessionState;
  
  pState->m_nBegun++;
}

/*----------------------------------------------------------------------------*/
/* increment number of __cancelled received                                   */
/*----------------------------------------------------------------------------*/
void ApiState::vIncCancelled(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  SessionState* pState = pSession->m_pSessionState;

  pState->m_nCancelled++;
}

/*----------------------------------------------------------------------------*/
/* increment number of __ended received                                       */
/*----------------------------------------------------------------------------*/
void ApiState::vIncEnded(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  SessionState* pState = pSession->m_pSessionState;

  pState->m_nEnded++;
}

/*----------------------------------------------------------------------------*/
/* increment number of executed received                                      */
/*----------------------------------------------------------------------------*/
void ApiState::vIncExecuted(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  SessionState* pState = pSession->m_pSessionState;

  pState->m_nExecuted++;
}

/*----------------------------------------------------------------------------*/
/* set last job location                                                      */
/*----------------------------------------------------------------------------*/
void ApiState::vJobLoc(Session* pSession, i8 iJobLoc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  SessionState* pState = pSession->m_pSessionState;
  
  pState->m_iJobLoc = iJobLoc;
}

/*----------------------------------------------------------------------------*/
/* set last SEN location                                                      */
/*----------------------------------------------------------------------------*/
void ApiState::vSenLoc(Session* pSession, i8 iSenLoc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  SessionState* pState = pSession->m_pSessionState;

  pState->m_iSenLoc = iSenLoc;
}

/*----------------------------------------------------------------------------*/
/* set that job has had its mutex signalled                                   */
/*----------------------------------------------------------------------------*/
void ApiState::vSignalled(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  SessionState* pState = pSession->m_pSessionState;

  pState->m_bSignalled = true;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(ApiState);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
