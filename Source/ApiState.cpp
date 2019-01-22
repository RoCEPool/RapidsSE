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
    
    SessionState* aState = &m_xSessionState[i_Session];
    
    aState->m_pSession   = nullptr;
    
    aState->m_eApiCall   = AC_NONE;
    aState->m_aStackBase = nullptr;
    aState->m_aStackPtr  = nullptr;
    aState->m_iLocation  = 0;
    aState->m_eJobClass  = CN_NONE;
    aState->m_iJobSeq    = 0;
    aState->m_eError     = EC_NONE;
    aState->m_bRunning   = false;
    aState->m_bSignalled = false;
    aState->m_iPosition  = 0;
    aState->m_oTimer.vInit();
    aState->m_iTotalMicroSecs = 0;

    aState->m_iNextFree = i_Session + 1;
  }
  m_xSessionState[C_MAX_SESSIONS - 1].m_iNextFree = -1;
  
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allocate an api state entry                                                */
/*----------------------------------------------------------------------------*/
i8 ApiState::iAllocState(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);

  m_oLock.vGrabWrite();
  
  RASSERT(-1 != m_iFirstFree);
  
  i8 i_Session = m_iFirstFree;
  SessionState* aState = &m_xSessionState[i_Session];

  m_iFirstFree = aState->m_iNextFree;
  
  aState->m_iNextFree = -1;
  aState->m_pSession  = pSession;

  m_nSessionsSoFar++;
  
  m_oLock.vDropWrite();

  return i_Session;
}

/*----------------------------------------------------------------------------*/
/* mark call of an api                                                        */
/*----------------------------------------------------------------------------*/
void ApiState::vApiCall(Session* pSession, eAC eApiCall)
{
  i8 i_Session = pSession->m_iApiState;
  
  SessionState* aState = &m_xSessionState[i_Session];

  aState->m_eApiCall   = eApiCall;
  aState->m_aStackBase = Pthread::aStackBase();
  aState->m_aStackPtr  = Pthread::aStackPointer();
  aState->m_iLocation  = 0;
  aState->m_eJobClass  = CN_NONE;
  aState->m_eError     = EC_NONE;
  aState->m_bRunning   = true;
  aState->m_bSignalled = false;
  aState->m_iPosition  = 0;
  
  aState->m_oTimer.vStart();
  
  pSession->m_nApiCalls++;
}

/*----------------------------------------------------------------------------*/
/* mark error return of an api                                                */
/*----------------------------------------------------------------------------*/
void ApiState::vApiError(Session* pSession, eEC eError)
{
  i8 i_Session = pSession->m_iApiState;
  SessionState* aState = &m_xSessionState[i_Session];

  aState->m_bRunning = false;
  aState->m_eError   = eError;
  
  aState->m_oTimer.vUpdate();
  
  aState->m_iTotalMicroSecs += aState->m_oTimer.iMicroseconds();

  pSession->m_nApiErrors++;
}

/*----------------------------------------------------------------------------*/
/* mark job of an api                                                         */
/*----------------------------------------------------------------------------*/
void ApiState::vApiJob(Session* pSession, Job* pJob)
{
  i8 i_Session = pSession->m_iApiState;
  SessionState* aState = &m_xSessionState[i_Session];

  aState->m_eJobClass = pJob->eClass();
  aState->m_iJobSeq   = pJob->m_iJobSeq;

  pSession->m_nJobNew++;
  
  pJob->m_iSession = i_Session;
}

/*----------------------------------------------------------------------------*/
/* mark return of an api                                                      */
/*----------------------------------------------------------------------------*/
void ApiState::vApiReturn(Session* pSession)
{
  i8 i_Session = pSession->m_iApiState;
  SessionState* aState = &m_xSessionState[i_Session];

  aState->m_bRunning = false;
  aState->m_oTimer.vUpdate();
  aState->m_iTotalMicroSecs += aState->m_oTimer.iMicroseconds();

//  if (xState[i_Session].m_eApiCall == AC_FetchNextBuffer)
//    vTrace("Timer start: %lld; Timer update: %lld Time: %lldus\n"_Stz,
//           aState->m_oTimer.iStart(),
//           aState->m_oTimer.iUpdate(),
//           aState->m_oTimer.iMicroseconds());
}

/*----------------------------------------------------------------------------*/
/* feee an api state entry                                                    */
/*----------------------------------------------------------------------------*/
void ApiState::vFreeState(i8 i_Session)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_APISTATE,_FOOTER_);
  
  SessionState* aState = &m_xSessionState[i_Session];

  if (aState->m_pSession->m_nJobNew != aState->m_pSession->m_nJobDelete) {
    vTrace("**************************************************\n"_Stz);
    vTrace("Session being deleted has not deleted all its jobs\n"_Stz);
    vDump();
    vTrace("**************************************************\n"_Stz);
  }
  
  m_oLock.vGrabWrite();
  aState->m_pSession = nullptr;
  
  aState->m_eApiCall   = AC_NONE;
  aState->m_aStackBase = nullptr;
  aState->m_aStackPtr  = nullptr;
  aState->m_iLocation  = 0;
  aState->m_eJobClass  = CN_NONE;
  aState->m_eError     = EC_NONE;
  aState->m_bRunning   = false;
  aState->m_bSignalled = false;
  aState->m_iPosition  = 0;

  aState->m_oTimer.vReset();
  aState->m_iTotalMicroSecs = 0;

  aState->m_iNextFree = m_iFirstFree;

  m_iFirstFree = i_Session;
  
  m_oLock.vDropWrite();
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
    
    SessionState* aSessionState = &m_xSessionState[i_Session];
    if (! aSessionState->m_pSession)
      continue;

    Session* pSession = aSessionState->m_pSession;

    if (aSessionState->m_bRunning)
      aSessionState->m_oTimer.vUpdate();

    sPortrait.vCatSprint("  [%2lld] #calls %lld job %s call %s (%s); "
                         "loc %lld seq %lld pos %lld\n"_Stz,
                        
                         i_Session,
                         
                         pSession->m_nApiCalls,
                         aCN[aSessionState->m_eJobClass],
                         aAC[aSessionState->m_eApiCall],
                         aSessionState->m_bRunning ? "running" : "completed",

                         aSessionState->m_iLocation,
                         aSessionState->m_iJobSeq,
                         aSessionState->m_iPosition

                         );
  }
  
  m_oLock.vDropRead();

}

/*----------------------------------------------------------------------------*/
/* set last location                                                          */
/*----------------------------------------------------------------------------*/
void ApiState::vSetLocation(Session* pSession, i8 iLocation)
{
  i8 i_Session = pSession->m_iApiState;
  SessionState* aState = &m_xSessionState[i_Session];
  
  aState->m_iLocation = iLocation;
}

/*----------------------------------------------------------------------------*/
/* set last position                                                          */
/*----------------------------------------------------------------------------*/
void ApiState::vSetPosition(Session* pSession, i8 iPosition)
{
  i8 i_Session = pSession->m_iApiState;
  SessionState* aState = &m_xSessionState[i_Session];
  
  aState->m_iPosition = iPosition;
}

/*----------------------------------------------------------------------------*/
/* set signalled                                                              */
/*----------------------------------------------------------------------------*/
void ApiState::vSetSignalled(Job* pJob)
{
  Session* pSession = pJob->m_pSession;
  
  i8 i_Session = pSession->m_iApiState;
  
  RASSERT( (-1 == pJob->m_iSession) ||
          (pJob->m_iSession == i_Session) );
  
  SessionState* aState = &m_xSessionState[i_Session];

  RASSERT( (-1 == pJob->m_iSession) ||
          (pJob->m_iJobSeq == aState->m_iJobSeq) );

  aState->m_bSignalled = true;
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
    
    SessionState* aSessionState = &m_xSessionState[i_Session];
    
    if (aSessionState->m_pSession) {
      
      Session* pSession = aSessionState->m_pSession;
      
      if (aSessionState->m_bRunning)
        aSessionState->m_oTimer.vUpdate();
      
      i8 nBytes;
      
      if (aSessionState->m_aStackBase >= aSessionState->m_aStackPtr)
        nBytes = nSub(aSessionState->m_aStackBase,aSessionState->m_aStackPtr);
      else
        nBytes = nSub(aSessionState->m_aStackPtr,aSessionState->m_aStackBase);

      vTrace("  %lld: session %p, call %s, base %llx, ptr %llx (%lldB), loc %lld, job %s seq %lld\n"
             "     error %s, %s, %s, pos %lld, time %lldus total %lldus\n"
             "     #calls %lld, #errors %lld, #jobNew %lld, #jobDelete %lld\n"_Stz,
             i_Session,
             pSession,
             aAC[aSessionState->m_eApiCall],
             aSessionState->m_aStackBase,
             aSessionState->m_aStackPtr,
             nBytes,

             aSessionState->m_iLocation,
             aCN[aSessionState->m_eJobClass],
             aSessionState->m_iJobSeq,
             aEC[aSessionState->m_eError],
             aSessionState->m_bRunning ? "running" : "completed",
             aSessionState->m_bSignalled ? "signalled" : "not signalled",
             aSessionState->m_iPosition,
             aSessionState->m_oTimer.iMicroseconds(),
             aSessionState->m_iTotalMicroSecs,
             pSession->m_nApiCalls,
             pSession->m_nApiErrors,
             pSession->m_nJobNew,
             pSession->m_nJobDelete);
    }
//    else {
//      vTrace("  %lld: nextfree %lld\n"_Stz,
//             i_Session,
//             aState->m_iNextFree);
//    }
  }
  
  vTrace("  firstFree %lld\n"_Stz,m_iFirstFree);
  vTrace("  DdlLock %llx\n"_Stz,gpAttachMap->m_oDdlLock.uLockWord());

  m_oLock.vDropRead();
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

_aFooter_(ApiState);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
