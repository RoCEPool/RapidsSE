/*============================================================================*/
/* SessionArray.cpp - SessionArray                                                    */
/*============================================================================*/

#include "Interface.h"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SessionArray* Interface::_pSessionArray(void* aAddr)
{
  SessionArray* pSessionArray = reinterpret_cast<SessionArray*>(aAddr);
  
  RASSERT_TYPE(pSessionArray,CN_SESSIONARRAY);
 
  return pSessionArray;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SessionArray::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_SESSIONARRAY);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SessionArray::operator delete(void* aAddress)
{
  SessionArray* pSessionArray = reinterpret_cast<SessionArray*>(aAddress);
  
  HeapMem::vFree(pSessionArray);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SessionArray::SessionArray(void)
: Object(CN_SESSIONARRAY)
, m_okLock(LK_API_STATE,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONARRAY,_FOOTER_);
  
  m_nSessionsSoFar = 0;
  m_iFirstFree = 0;

  for (i8 i_Session = 0; i_Session < C_MAX_SESSIONS; i_Session++) {
    Session* pSession = new Session;
    
    m_paSession[i_Session] = pSession;
    pSession->m_i_Session  = i_Session;
    pSession->m_xReqTuple.m_i_Session = i_Session;
    pSession->m_i_NextFree = i_Session + 1;
  }
  m_paSession[C_MAX_SESSIONS - 1]->m_i_NextFree = -1;
  
  m_iFirstFree = 0;

  _checkInitialised_(CN_SESSIONARRAY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SESSIONARRAY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SessionArray::~SessionArray(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONARRAY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* mark location of an api                                                    */
/*----------------------------------------------------------------------------*/
void SessionArray::vMarkApiLoc(Session* pSession, i8 iApiLoc)
{
  AM::pSessionArray()->vApiLoc(pSession,iApiLoc);
}

/*----------------------------------------------------------------------------*/
/* mark call of an api                                                        */
/*----------------------------------------------------------------------------*/
void SessionArray::vMarkCall(Session* pSession, eAC eApiCall)
{
  AM::pSessionArray()->vApiCall(pSession,eApiCall);
}

/*----------------------------------------------------------------------------*/
/* mark error of an api                                                       */
/*----------------------------------------------------------------------------*/
void SessionArray::vMarkError(Session* pSession, eEC eError)
{
  AM::pSessionArray()->vApiError(pSession,eError);
}

/*----------------------------------------------------------------------------*/
/* mark another __begun job has been received                                 */
/*----------------------------------------------------------------------------*/
void SessionArray::vMarkIncBegun(Session* pSession)
{
  AM::pSessionArray()->vIncBegun(pSession);
}

/*----------------------------------------------------------------------------*/
/* mark another __cancelled job has been received                             */
/*----------------------------------------------------------------------------*/
void SessionArray::vMarkIncCancelled(Session* pSession)
{
  AM::pSessionArray()->vIncCancelled(pSession);
}

/*----------------------------------------------------------------------------*/
/* mark another __ended job has been received                                 */
/*----------------------------------------------------------------------------*/
void SessionArray::vMarkIncEnded(Session* pSession)
{
  AM::pSessionArray()->vIncEnded(pSession);
}

/*----------------------------------------------------------------------------*/
/* mark another __executed job has been received                              */
/*----------------------------------------------------------------------------*/
void SessionArray::vMarkIncExecuted(Session* pSession)
{
  AM::pSessionArray()->vIncExecuted(pSession);
}

/*----------------------------------------------------------------------------*/
/* mark job of an api                                                         */
/*----------------------------------------------------------------------------*/
void SessionArray::vMarkJob(Session* pSession, Job* pJob)
{
  AM::pSessionArray()->vApiJob(pSession,pJob);
}

/*----------------------------------------------------------------------------*/
/* mark job location of an api                                                */
/*----------------------------------------------------------------------------*/
void SessionArray::vMarkJobLoc(Session* pSession, i8 iJobLoc)
{
  AM::pSessionArray()->vJobLoc(pSession,iJobLoc);
}

/*----------------------------------------------------------------------------*/
/* mark return of an api                                                      */
/*----------------------------------------------------------------------------*/
void SessionArray::vMarkReturn(Session* pSession)
{
  AM::pSessionArray()->vApiReturn(pSession);
}

/*----------------------------------------------------------------------------*/
/* mark sen location of an api                                                */
/*----------------------------------------------------------------------------*/
void SessionArray::vMarkSenLoc(Session* pSession, i8 iSenLoc)
{
  AM::pSessionArray()->vApiLoc(pSession,iSenLoc);
}

/*----------------------------------------------------------------------------*/
/* mark that job has had its mutex signalled                                  */
/*----------------------------------------------------------------------------*/
void SessionArray::vMarkSignalled(Session* pSession)
{
  AM::pSessionArray()->vSignalled(pSession);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allocate a session                                                         */
/*----------------------------------------------------------------------------*/
Session* SessionArray::pAllocSession(eSK eSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONARRAY,_FOOTER_);
  
  m_okLock.vGrabWrite();
  
  RASSERT(-1 != m_iFirstFree);
  
  i8 i_Session = m_iFirstFree;
  Session* pSession = m_paSession[i_Session];
  
  m_iFirstFree = pSession->m_i_NextFree;
  
  m_nSessionsSoFar++;

  pSession->m_eSession = eSession;
  
  m_okLock.vDropWrite();
  
  return pSession;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void SessionArray::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONARRAY,_FOOTER_);
  
  m_okLock.vGrabRead();
  
  vTrace("\n"_Stz);
  vTrace("SessionArray\n"_Stz);
  vTrace("------------\n"_Stz);

  vTrace("%p: SessionsSoFar %lld:\n"_Stz,
         this,
         m_nSessionsSoFar);
  
  i8 iSessions = 0;
  for (i8 i_Session = 0; i_Session < C_MAX_SESSIONS; i_Session++) {
    
    if (iSessions == m_nSessionsSoFar)
      break;
    
    Session* pSession = m_paSession[i_Session];
    if (pSession)
      pSession->vTraceSession();
    
    iSessions++;
  }
  
  vTrace("  firstFree %lld\n"_Stz,m_iFirstFree);
  vTrace("  DdlLock %llx\n"_Stz,AM::pDdlLock()->uLockWord());
  
  m_okLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* free a session                                                             */
/*----------------------------------------------------------------------------*/
void SessionArray::vFreeSession(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONARRAY,_FOOTER_);
  
  i8 i_FreedSession = pSession->m_i_Session;
  
  m_okLock.vGrabWrite();
  
  pSession->m_eSession = SK_NONE;

  pSession->vFreeSession(m_iFirstFree);
  
  m_iFirstFree = i_FreedSession;
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* get a session                                                              */
/*----------------------------------------------------------------------------*/
Session* SessionArray::pGetSession(i8 i_Session)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONARRAY,_FOOTER_);
  
  ASSERT(i_Session >= 0 && i_Session < C_MAX_SESSIONS);
  Session* pSession = m_paSession[i_Session];
  
  return pSession;
}

/*----------------------------------------------------------------------------*/
/* get a portrait of the SessionArray vector                                  */
/*----------------------------------------------------------------------------*/
void SessionArray::vMonitorPortrait(s48K& sPortrait)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONARRAY);
  
  s48K sTasksPortrait;
  
  sPortrait = ""_Stz;
  sPortrait.vConcat("SessionArray:\n"_Stz);
  
  m_okLock.vGrabRead();
  
  for (i8 i_Session = 0; i_Session < C_MAX_SESSIONS; i_Session++) {
    
    Session* pSession = m_paSession[i_Session];
    if (! pSession)
      continue;
    
    if (pSession->m_bRunning)
      pSession->m_oTimer.vUpdate();
    
    sPortrait.vCatSprint("  [%2lld] #calls %lld job %s call %s (%s); "
                         "loc %lld seq %lld pos %lld\n"_Stz,
                         
                         i_Session,
                         
                         pSession->m_nApiCalls,
                         aCN[pSession->m_eJobClass],
                         aAC[pSession->m_eApiFun],
                         pSession->m_bRunning ? "running" : "completed",
                         
                         pSession->m_iApiLoc,
                         pSession->m_iJobLoc,
                         pSession->m_iSenLoc
                         
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
void SessionArray::vApiCall(Session* pSession, eAC eApiCall)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONARRAY,_FOOTER_);

  pSession->m_eApiFun    = eApiCall;
  pSession->m_iApiLoc    = 0;
  pSession->m_eApiError  = EC_NONE;
  pSession->m_bRunning   = true;
  pSession->m_bSignalled = false;

  pSession->m_aStackBase = PsxThd::aGetStackBase();
  pSession->m_aStackPtr  = PsxThd::aGetStackPointer();

  pSession->m_eJobClass  = CN_NONE;
  pSession->m_iJobLoc  = 0;
  pSession->m_iSenLoc  = 0;
  
  pSession->m_oTimer.vStart();
  
  pSession->m_nApiCalls++;
}

/*----------------------------------------------------------------------------*/
/* mark error return of an api                                                */
/*----------------------------------------------------------------------------*/
void SessionArray::vApiError(Session* pSession, eEC eError)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONARRAY,_FOOTER_);

  pSession->m_bRunning  = false;
  pSession->m_eApiError = eError;
  
  pSession->m_oTimer.vUpdate();
  pSession->m_qTotalUsec += pSession->m_oTimer.iMicroseconds();

  pSession->m_nApiErrors++;
}

/*----------------------------------------------------------------------------*/
/* mark job of an api                                                         */
/*----------------------------------------------------------------------------*/
void SessionArray::vApiJob(Session* pSession, Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONARRAY,_FOOTER_);

  pSession->m_eJobClass = pJob->eClass();

  pSession->m_nJobNew++;
  
  pJob->m_xReqTuple.m_i_Session = pSession->m_i_Session;
}

/*----------------------------------------------------------------------------*/
/* set last API location                                                      */
/*----------------------------------------------------------------------------*/
void SessionArray::vApiLoc(Session* pSession, i8 iLocation)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONARRAY,_FOOTER_);
  
  if (! pSession)
    return;
  
  pSession->m_iApiLoc = iLocation;
}

/*----------------------------------------------------------------------------*/
/* mark return of an api                                                      */
/*----------------------------------------------------------------------------*/
void SessionArray::vApiReturn(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONARRAY,_FOOTER_);
  
  pSession->m_bRunning = false;
  pSession->m_oTimer.vUpdate();
  pSession->m_qTotalUsec += pSession->m_oTimer.iMicroseconds();

//  if (xState[i_Session].m_eApiFun == AC_FetchNextBuffer)
//    vTrace("Timer start: %lld; Timer update: %lld Time: %lldus\n"_Stz,
//           pState->m_oTimer.iStart(),
//           pState->m_oTimer.iUpdate(),
//           pState->m_oTimer.iMicroseconds());
}

/*----------------------------------------------------------------------------*/
/* increment number of __begun received                                       */
/*----------------------------------------------------------------------------*/
void SessionArray::vIncBegun(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONARRAY,_FOOTER_);
  
  pSession->m_nBegun++;
}

/*----------------------------------------------------------------------------*/
/* increment number of __cancelled received                                   */
/*----------------------------------------------------------------------------*/
void SessionArray::vIncCancelled(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONARRAY,_FOOTER_);

  pSession->m_nCancelled++;
}

/*----------------------------------------------------------------------------*/
/* increment number of __ended received                                       */
/*----------------------------------------------------------------------------*/
void SessionArray::vIncEnded(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONARRAY,_FOOTER_);

  pSession->m_nEnded++;
}

/*----------------------------------------------------------------------------*/
/* increment number of executed received                                      */
/*----------------------------------------------------------------------------*/
void SessionArray::vIncExecuted(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONARRAY,_FOOTER_);

  pSession->m_nExecuted++;
}

/*----------------------------------------------------------------------------*/
/* set last job location                                                      */
/*----------------------------------------------------------------------------*/
void SessionArray::vJobLoc(Session* pSession, i8 iJobLoc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONARRAY,_FOOTER_);
  
  pSession->m_iJobLoc = iJobLoc;
}

/*----------------------------------------------------------------------------*/
/* set last SEN location                                                      */
/*----------------------------------------------------------------------------*/
void SessionArray::vSenLoc(Session* pSession, i8 iSenLoc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONARRAY,_FOOTER_);

  pSession->m_iSenLoc = iSenLoc;
}

/*----------------------------------------------------------------------------*/
/* set that job has had its mutex signalled                                   */
/*----------------------------------------------------------------------------*/
void SessionArray::vSignalled(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONARRAY,_FOOTER_);

  pSession->m_bSignalled = true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(SessionArray);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
