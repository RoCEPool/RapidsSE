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
  
  m_oLock.vInitLock(LK_API_STATE);
  m_nSessionsSoFar = 0;
  m_iFirstFree = 0;

  for (i8 i_Session = 0; i_Session < C_MAX_SESSIONS; i_Session++) {
    SessionState* pState = &m_oSessionState[i_Session];
    pState->m_iNextFree = i_Session + 1;
  }
  m_oSessionState[C_MAX_SESSIONS - 1].m_iNextFree = -1;
  
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
/* mark job of an api                                                         */
/*----------------------------------------------------------------------------*/
void ApiState::vMarkJob(Session* pSession, Job* pJob)
{
  gpAttachMap->m_pApiState->vApiJob(pSession,pJob);
}

/*----------------------------------------------------------------------------*/
/* mark location of an api                                                    */
/*----------------------------------------------------------------------------*/
void ApiState::vMarkApiLoc(Session* pSession, i8 iApiLoc)
{
  gpAttachMap->m_pApiState->vApiLoc(pSession,iApiLoc);
}

/*----------------------------------------------------------------------------*/
/* mark return of an api                                                      */
/*----------------------------------------------------------------------------*/
void ApiState::vMarkReturn(Session* pSession)
{
  gpAttachMap->m_pApiState->vApiReturn(pSession);
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
  
  m_oLock.vGrabWrite();
  
  RASSERT(-1 != m_iFirstFree);
  
  i8 i_State = m_iFirstFree;
  SessionState* pState = &m_oSessionState[i_State];
  
  m_iFirstFree = pState->m_iNextFree;
  
  pState->m_iNextFree = -1;
  pState->m_pSession  = pSession;
  pState->m_i_State   = i_State;
  
  m_nSessionsSoFar++;
  
  m_oLock.vDropWrite();
  
  return pState;
}

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
  pState->m_iJobLoc   = pJob->m_iJobLoc;

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
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void ApiState::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  m_oLock.vGrabRead();
  
  vTrace("ApiState %p SessionsSoFar %lld:\n"_Stz,this,m_nSessionsSoFar);
  
  for (i8 i_Session = 0; i_Session < C_MAX_SESSIONS; i_Session++) {
    SessionState* pState = &m_oSessionState[i_Session];
    Session* pSession = pState->m_pSession;
    if (pSession)
      pState->vTraceState();
  }
  
  vTrace("  firstFree %lld\n"_Stz,m_iFirstFree);
  vTrace("  DdlLock %llx\n"_Stz,gpAttachMap->m_oDdlLock.uLockWord());
  
  m_oLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* free an api state entry                                                    */
/*----------------------------------------------------------------------------*/
void ApiState::vFreeState(i8 i_Session)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  SessionState* pState = &m_oSessionState[i_Session];
  
  m_oLock.vGrabWrite();
  
  pState->vInit();
  m_iFirstFree = i_Session;
  
  m_oLock.vDropWrite();
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
/* get a portrait of the queues                                               */
/*----------------------------------------------------------------------------*/
void ApiState::vMonitorPortrait(s48K& sPortrait)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE);
  
  s48K sTasksPortrait;
  
  sPortrait = ""_Stz;
  sPortrait.vConcat("ApiState:\n"_Stz);
  
  m_oLock.vGrabRead();
  
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
  
  m_oLock.vDropRead();

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
/* set signalled                                                              */
/*----------------------------------------------------------------------------*/
void ApiState::vSetSignalled(Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);

  Session* pSession = pJob->m_pSession;
  
  SessionState* pState = pSession->m_pSessionState;

  RASSERT( (-1 == pJob->m_i_State) ||
          (pJob->m_iJobLoc == pState->m_iJobLoc) );

  pState->m_bSignalled = true;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(ApiState);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
