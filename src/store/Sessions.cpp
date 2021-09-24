/*============================================================================*/
/* Sessions.cpp - Sessions                                                    */
/*============================================================================*/

#include "BASE.h"

#include "Session.hpp"
#include "Sessions.hpp"
#include "Job.hpp"

#include "PsxThd.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Sessions* STORE::_pSessions(void* aAddr)
{
  Sessions* pSessions = reinterpret_cast<Sessions*>(aAddr);
  
  RASSERT_TYPE(pSessions,CN_SESSIONS);
 
  return pSessions;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Sessions::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_SESSIONS);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Sessions::operator delete(void* aAddress)
{
  Sessions* pSessions = reinterpret_cast<Sessions*>(aAddress);
  
  Heaps::vFree(pSessions);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Sessions::Sessions(void)
: Object(CN_SESSIONS)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);
  
  m_nSessionsSoFar = 0;
  m_iFirstFree = 0;

  m_pInternalSession = nullptr;

  m_pSsnSkipList = new SkipListNL(C_UNQIUE_KEY, STORE::bSsnCompare_id);

  for (i8 i_Session = 0; i_Session < C_MAX_SESSIONS; i_Session++) {
    Session* pSession = new Session;
    
    m_paSession[i_Session] = pSession;
    pSession->m_i_Session  = i_Session;
    pSession->m_i_NextFree = i_Session + 1;
  }
  m_paSession[C_MAX_SESSIONS - 1]->m_i_NextFree = -1;
  
  m_iFirstFree = 0;
  

  _checkInitialised_(CN_SESSIONS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Sessions::~Sessions(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* mark location of an api                                                    */
/*----------------------------------------------------------------------------*/
void Sessions::vMarkApiLoc(Session* pSession, i8 iApiLoc)
{
  AM::pSessions()->vApiLoc(pSession,iApiLoc);
}

/*----------------------------------------------------------------------------*/
/* mark call of an api                                                        */
/*----------------------------------------------------------------------------*/
void Sessions::vMarkCall(Session* pSession, eAC eApiCall)
{
  AM::pSessions()->vApiCall(pSession,eApiCall);
}

/*----------------------------------------------------------------------------*/
/* mark error of an api                                                       */
/*----------------------------------------------------------------------------*/
void Sessions::vMarkError(Session* pSession, eEC eError)
{
  AM::pSessions()->vApiError(pSession,eError);
}

/*----------------------------------------------------------------------------*/
/* mark another __begun job has been received                                 */
/*----------------------------------------------------------------------------*/
void Sessions::vMarkIncBegun(Session* pSession)
{
  AM::pSessions()->vIncBegun(pSession);
}

/*----------------------------------------------------------------------------*/
/* mark another __cancelled job has been received                             */
/*----------------------------------------------------------------------------*/
void Sessions::vMarkIncCancelled(Session* pSession)
{
  AM::pSessions()->vIncCancelled(pSession);
}

/*----------------------------------------------------------------------------*/
/* mark another __ended job has been received                                 */
/*----------------------------------------------------------------------------*/
void Sessions::vMarkIncEnded(Session* pSession)
{
  AM::pSessions()->vIncEnded(pSession);
}

/*----------------------------------------------------------------------------*/
/* mark another __executed job has been received                              */
/*----------------------------------------------------------------------------*/
void Sessions::vMarkIncExecuted(Session* pSession)
{
  AM::pSessions()->vIncExecuted(pSession);
}

/*----------------------------------------------------------------------------*/
/* mark return of an api                                                      */
/*----------------------------------------------------------------------------*/
void Sessions::vMarkReturn(Session* pSession)
{
  AM::pSessions()->vApiReturn(pSession);
}

/*----------------------------------------------------------------------------*/
/* mark sen location of an api                                                */
/*----------------------------------------------------------------------------*/
void Sessions::vMarkSenLoc(Session* pSession, i8 iSenLoc)
{
  AM::pSessions()->vApiLoc(pSession,iSenLoc);
}

/*----------------------------------------------------------------------------*/
/* mark that job has had its mutex signalled                                  */
/*----------------------------------------------------------------------------*/
void Sessions::vMarkSignalled(Session* pSession)
{
  AM::pSessions()->vSignalled(pSession);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allocate an internal session                                               */
/*----------------------------------------------------------------------------*/
void Sessions::vAllocInternal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);
  
  m_okLock.vGrabWrite();
  
  RASSERT(-1 != m_iFirstFree);
  RASSERT(! m_pInternalSession);

  i8 i_Session = m_iFirstFree;
  m_pInternalSession = m_paSession[i_Session];
  
  m_iFirstFree = m_pInternalSession->m_i_NextFree;
  
  m_nSessionsSoFar++;

  m_pInternalSession->m_eSession = SK_INTERNAL;
  
#ifdef WITH_SSN_SKIPLIST
  bool bInserted = m_pSsnSkipList->bInsert(m_pInternalSession);
  RASSERT(bInserted);
#endif
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* allocate a session                                                         */
/*----------------------------------------------------------------------------*/
Session* Sessions::pAllocSession(eSK eSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);
  
  m_okLock.vGrabWrite();
  
  RASSERT(-1 != m_iFirstFree);
  
  i8 i_Session = m_iFirstFree;
  Session* pSession = m_paSession[i_Session];
  
  m_iFirstFree = pSession->m_i_NextFree;
  
  m_nSessionsSoFar++;

  pSession->m_eSession = eSession;
  
#ifdef WITH_SSN_SKIPLIST
  bool bInserted = m_pSsnSkipList->bInsert(pSession);
  RASSERT(bInserted);
#endif
  
  m_okLock.vDropWrite();
  
  return pSession;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Sessions::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);
  
  m_okLock.vGrabRead();
  
  vTrace("\n"_Stz);
  vTrace("Sessions\n"_Stz);
  vTrace("------------\n"_Stz);

  vTrace("%p: DataBuffersSoFar %lld:\n"_Stz,
         this,
         m_nSessionsSoFar);
  
  i8 iDataBuffers = 0;
  for (i8 i_Session = 0; i_Session < C_MAX_SESSIONS; i_Session++) {
    
    if (iDataBuffers == m_nSessionsSoFar)
      break;
    
    Session* pSession = m_paSession[i_Session];
    if (pSession)
      pSession->vTraceSession();
    
    iDataBuffers++;
  }
  
  vTrace("  firstFree %lld\n"_Stz,m_iFirstFree);
  vTrace("  DdlLock %llx\n"_Stz,AM::MASTERLOCK::uDdlLockWord());
  
  m_okLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* free a session                                                             */
/*----------------------------------------------------------------------------*/
void Sessions::vFreeSession(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);
  
  i8 i_FreedSession = pSession->m_i_Session;
  
  m_okLock.vGrabWrite();
  
  pSession->m_eSession = SK_NONE;

  pSession->vFinaliseSession(m_iFirstFree);
  
  m_iFirstFree = i_FreedSession;
  
#ifdef WITH_SSN_SKIPLIST
  bool bRemoved = m_pSsnSkipList->bRemove(pSession);
  RASSERT(bRemoved);
#endif
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* get a session                                                              */
/*----------------------------------------------------------------------------*/
Session* Sessions::pGetSession(i8 i_Session)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);
  
  ASSERT(i_Session >= 0 && i_Session < C_MAX_SESSIONS);
  Session* pSession = m_paSession[i_Session];
  
  return pSession;
}

/*----------------------------------------------------------------------------*/
/* find a session in skip list                                                */
/*----------------------------------------------------------------------------*/
Session* Sessions::pFindSession(SsnId xSessionId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);
  
  Session* pSession = nullptr;
  
  bool bMatch = false;
  auto bfCmpByKey = [xSessionId, &bMatch](void* aSsnScan) {
    
    Session* pSsnScan   = _pSession(aSsnScan);

    if (! pSsnScan)
      return false;

    // do the id's match?
    SsnId tSsnId = xSessionId;
    //if (*const_cast<SsnId*>(&xSessionId) == pSsnScan->m_xSessionId)
    if (tSsnId == pSsnScan->m_xSessionId)
      bMatch = true;
    else
      bMatch = false;

    if (tSsnId > pSsnScan->m_xSessionId)
      return true;
    else
      return false;
  };

  pSession = _pSession(m_pSsnSkipList->pFindByKey(bfCmpByKey));
  
  return pSession;
}

/*----------------------------------------------------------------------------*/
/* get the internal session                                                   */
/*----------------------------------------------------------------------------*/
Session* Sessions::pInternalSession(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);
  
  RASSERT(m_pInternalSession);

  return m_pInternalSession;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* mark call of an api                                                        */
/*----------------------------------------------------------------------------*/
void Sessions::vApiCall(Session* pSession, eAC eApiCall)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);

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
void Sessions::vApiError(Session* pSession, eEC eError)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);

  pSession->m_bRunning  = false;
  pSession->m_eApiError = eError;
  
  pSession->m_oTimer.vUpdate();
  pSession->m_qTotalUsec += pSession->m_oTimer.iMicroseconds();

  pSession->m_nApiErrors++;
}

/*----------------------------------------------------------------------------*/
/* mark job of an api                                                         */
/*----------------------------------------------------------------------------*/
void Sessions::vApiJob(Session* pSession, Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);

  pSession->m_eJobClass = pJob->eGetClassNo();

  pSession->m_nJobNew++;
  
  pJob->m_i_Session = pSession->m_i_Session;
}

/*----------------------------------------------------------------------------*/
/* set last API location                                                      */
/*----------------------------------------------------------------------------*/
void Sessions::vApiLoc(Session* pSession, i8 iLocation)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);
  
  if (! pSession)
    return;
  
  pSession->m_iApiLoc = iLocation;
}

/*----------------------------------------------------------------------------*/
/* mark return of an api                                                      */
/*----------------------------------------------------------------------------*/
void Sessions::vApiReturn(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);
  
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
void Sessions::vIncBegun(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);
  
  pSession->m_nBegun++;
}

/*----------------------------------------------------------------------------*/
/* increment number of __cancelled received                                   */
/*----------------------------------------------------------------------------*/
void Sessions::vIncCancelled(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);

  pSession->m_nCancelled++;
}

/*----------------------------------------------------------------------------*/
/* increment number of __ended received                                       */
/*----------------------------------------------------------------------------*/
void Sessions::vIncEnded(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);

  pSession->m_nEnded++;
}

/*----------------------------------------------------------------------------*/
/* increment number of executed received                                      */
/*----------------------------------------------------------------------------*/
void Sessions::vIncExecuted(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);

  pSession->m_nExecuted++;
}

/*----------------------------------------------------------------------------*/
/* set last job location                                                      */
/*----------------------------------------------------------------------------*/
void Sessions::vJobLoc(Session* pSession, i8 iJobLoc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);
  
  pSession->m_iJobLoc = iJobLoc;
}

/*----------------------------------------------------------------------------*/
/* set last SEN location                                                      */
/*----------------------------------------------------------------------------*/
void Sessions::vSenLoc(Session* pSession, i8 iSenLoc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);

  pSession->m_iSenLoc = iSenLoc;
}

/*----------------------------------------------------------------------------*/
/* set that job has had its mutex signalled                                   */
/*----------------------------------------------------------------------------*/
void Sessions::vSignalled(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSIONS,_FOOTER_);

  pSession->m_bSignalled = true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Sessions);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
