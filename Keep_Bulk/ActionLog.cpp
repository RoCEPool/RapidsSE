/*============================================================================*/
/* ActionLog.cpp - Activity ShmLog                                          */
/*============================================================================*/

#include "Logging.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

ActionLog* gpActionSequence = nullptr;


/*----------------------------------------------------------------------------*/
/* -- LifeCycle --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ActionLog::operator new(size_t, ActionLog* pActionSequence)
{
  RASSERT(! gpActionSequence);
  
  return pActionSequence;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ActionLog::operator delete(void* aAddress)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ActionLog::ActionLog(void)
: ShmLog(CN_ACTIONLOG)
{
  _precondition_(_MAGIC_,CN_ACTIONLOG,_FOOTER_);
    
  for (i8 i_Entry = 0; i_Entry < m_nEntries; i_Entry++)
    m_oaAction[i_Entry].vInit(i_Entry);
  
  _checkInitialised_(CN_ACTIONLOG);
  
  _postcondition_(_MAGIC_,CN_ACTIONLOG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ActionLog::~ActionLog(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ACTIONLOG,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create or open Log (called by SE)                                          */
/*----------------------------------------------------------------------------*/
void ActionLog::vOpenBySE(bool bStarting)
{
#ifdef WITH_ACTION_SEQUENCE
  if (! gpAttachMap->Arg.m_bActionSequence)
    return;
  
  if (gpActionSequence)
    vFatal("Log::vOpenBySE() -- activity map already opened\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s32 sMapNm("/SE_ActionSequence"_Stz);
  i8 nMapSz = sizeof(ActionLog);
  
  ActionLog* pActionSequence = nullptr;
  
  if (bStarting) {
    pActionSequence = (ActionLog*)ShmLog::aOpenCreate(sMapNm,nMapSz);
    if (gpAttachMap->m_bVerbose)
      vTrace("ActionLog: created new map %s %lldB\n"_Stz,
             sMapNm.aStz(),
             nMapSz);
    gpActionSequence = new(pActionSequence) ActionLog;
    
  }
  else {
    pActionSequence = (ActionLog*)ShmLog::aOpenExisting(sMapNm,nMapSz);
    if (gpAttachMap->m_bVerbose)
      vTrace("ActionLog: attached to existing map %s %lldB\n"_Stz,
             sMapNm.aStz(),
             nMapSz);

    gpActionSequence = pActionSequence;
  }
  
#endif
}


/*----------------------------------------------------------------------------*/
/* add a new action to a sequence                                             */
/*----------------------------------------------------------------------------*/
void ActionLog::vAdd(i8 i_RootAct, i8 iJobId, eCN eClassNo,
                          eAN eAction, i8 iArg0, i8 iArg1)
{
#ifdef WITH_ACTION_SEQUENCE
  if (! gpActionSequence)
    return;
  
  if (! gpAttachMap->m_oMarkAction.bFlagged(eClassNo))
    return;
  
  if (C_UNKNOWN == i_RootAct)
    return;

  Action* pRoot = &gpActionSequence->m_oaAction[i_RootAct];
  
  Action* pAdded = gpActionSequence->pNewLinkedAction(pRoot);
  
  pAdded->m_iJobId   = iJobId;
  pAdded->m_eClassNo = eClassNo;
  pAdded->m_eAction  = eAction;
  pAdded->m_iArg0    = iArg0;
  pAdded->m_iArg1    = iArg1;
  
#else

#endif
}

/*----------------------------------------------------------------------------*/
/* add a new action to a sequence                                             */
/*----------------------------------------------------------------------------*/
void ActionLog::vAdd(Jobs::Job* pJob,
                          eAN eAction, i8 iArg0, i8 iArg1)
{
#ifdef WITH_ACTION_SEQUENCE
  if (! gpActionSequence)
    return;
  
  if (! (pJob->m_bMarkAction ||
         gpAttachMap->m_oMarkAction.bFlagged(pJob->eClass())) )
    return;
  
  if (C_UNKNOWN == pJob->m_i_ActionRoot)
    return;
  
  Action* pRoot = &gpActionSequence->m_oaAction[pJob->m_i_ActionRoot];
  Action* pLinked = gpActionSequence->pNewLinkedAction(pRoot);
  
  pLinked->m_iJobId     = pJob->m_iJobId;
  pLinked->m_eClassNo   = pJob->eClass();
  pLinked->m_iParentId  = pJob->m_iParentId;
  pLinked->m_bWasRecv   = pJob->m_bWasReceived;
  pLinked->m_i_Activity = pJob->m_i_Activity;

  pLinked->m_eAction  = eAction;
  pLinked->m_iArg0    = iArg0;
  pLinked->m_iArg1    = iArg1;
  
  pLinked->m_aJob     = pJob;
#else
  
#endif
}

/*----------------------------------------------------------------------------*/
/* add a new action to a sequence                                             */
/*----------------------------------------------------------------------------*/
void ActionLog::vAdd(Jobs::Job* pJob, eAN eAction, Stz* szText)
{
#ifdef WITH_ACTION_SEQUENCE
  if (! gpActionSequence)
    return;
  
  if (! (pJob->m_bMarkAction ||
         gpAttachMap->m_oMarkAction.bFlagged(pJob->eClass())) )
    return;
  
  if (C_UNKNOWN == pJob->m_i_ActionRoot)
    return;
  
  Action* pRoot = &gpActionSequence->m_oaAction[pJob->m_i_ActionRoot];
  
  Action* pLinked = gpActionSequence->pNewLinkedAction(pRoot);
  
  pLinked->m_iJobId     = pJob->m_iJobId;
  pLinked->m_eClassNo   = pJob->eClass();
  pLinked->m_iParentId  = pJob->m_iParentId;
  pLinked->m_bWasRecv   = pJob->m_bWasReceived;
  pLinked->m_i_Activity = pJob->m_i_Activity;

  pLinked->m_eAction  = eAction;
  StrLib::vCopyLen(pLinked->m_caText,
                   szText->aChar(),
                   sizeof(pLinked->m_caText) - 1);
  
  pLinked->m_aJob     = pJob;
#else
  
#endif
}

/*----------------------------------------------------------------------------*/
/* add a new action to a sequence                                             */
/*----------------------------------------------------------------------------*/
void ActionLog::vAdd(Jobs::Job* pJob, eAN eAction,
                          Jobs::Job* pSystemJob,i8 iArg0, i8 iArg1)
{
#ifdef WITH_ACTION_SEQUENCE
  if (! gpActionSequence)
    return;
  
  if (! (pJob->m_bMarkAction ||
         gpAttachMap->m_oMarkAction.bFlagged(pJob->eClass())) )
    return;
  
  if (C_UNKNOWN == pJob->m_i_ActionRoot)
    return;
  
  Action* pRoot = &gpActionSequence->m_oaAction[pJob->m_i_ActionRoot];
  
  Action* pLinked = gpActionSequence->pNewLinkedAction(pRoot);
  
  pLinked->m_iJobId     = pJob->m_iJobId;
  pLinked->m_eClassNo   = pJob->eClass();
  pLinked->m_iParentId  = pJob->m_iParentId;
  pLinked->m_bWasRecv   = pJob->m_bWasReceived;
  pLinked->m_i_Activity = pJob->m_i_Activity;

  pLinked->m_eAction   = eAction;
  pLinked->m_iArg0     = iArg0;
  pLinked->m_iArg1     = iArg1;

  pLinked->m_iJobId_Arg     = pSystemJob->m_iJobId;
  pLinked->m_eClassNo_Arg   = pSystemJob->eClass();
  pLinked->m_iParentId_Arg  = pSystemJob->m_iParentId;
  pLinked->m_bWasRecv_Arg   = pSystemJob->m_bWasReceived;
  pLinked->m_i_Activity_Arg = pSystemJob->m_i_Activity;

  pLinked->m_aJob     = pJob;
#else
  
#endif
}

/*----------------------------------------------------------------------------*/
/* begin a new action sequence                                                */
/*----------------------------------------------------------------------------*/
i8 ActionLog::iBegin(i8 iJobId, eCN eClassNo,
                          eAN eAction, i8 iArg0, i8 iArg1)
{
#ifdef WITH_ACTION_SEQUENCE
  if (! gpActionSequence)
    return C_UNKNOWN;
  
  if (! gpAttachMap->m_oMarkAction.bFlagged(eClassNo))
    return C_UNKNOWN;

  Action* pRoot = gpActionSequence->pNewRootAction();
  pRoot->m_iJobId   = iJobId;
  pRoot->m_eClassNo = eClassNo;
  pRoot->m_eAction  = eAction;
  pRoot->m_iArg0    = iArg0;
  pRoot->m_iArg1    = iArg1;
  pRoot->m_wLink.vSetWithoutTest(C_UNKNOWN);

  return pRoot->m_i_Entry;
#else
  return C_UNKNOWN;
#endif
}

/*----------------------------------------------------------------------------*/
/* begin a new action sequence                                                */
/*----------------------------------------------------------------------------*/
void ActionLog::vRoot(Jobs::Job* pJob)
{
#ifdef WITH_ACTION_SEQUENCE
  if (! gpActionSequence)
    return;

  if (! (pJob->m_bMarkAction ||
         gpAttachMap->m_oMarkAction.bFlagged(pJob->eClass())) )
    return;
  
  Action* pRoot = gpActionSequence->pNewRootAction();
  
  pJob->m_i_ActionRoot = pRoot->m_i_Entry;
  
  pRoot->m_iJobId     = pJob->m_iJobId;
  pRoot->m_eClassNo   = pJob->eClass();
  pRoot->m_iParentId  = pJob->m_iParentId;
  pRoot->m_bWasRecv   = pJob->m_bWasReceived;
  pRoot->m_i_Activity = pJob->m_i_Activity;
  
  Now xNow;
  
  pRoot->m_eAction  = AN_ROOT;
  pRoot->m_iArg0    = xNow.iTsp();
  pRoot->m_iArg1    = 0;
  
  pRoot->m_aJob     = pJob;
#else

#endif
}

/*----------------------------------------------------------------------------*/
/* begin a new action sequence                                                */
/*----------------------------------------------------------------------------*/
void ActionLog::vEnd(Jobs::Job* pJob)
{
#ifdef WITH_ACTION_SEQUENCE
  if (! gpActionSequence)
    return;
  
  if (! (pJob->m_bMarkAction ||
         gpAttachMap->m_oMarkAction.bFlagged(pJob->eClass())) )
    return;
  
  if (C_UNKNOWN == pJob->m_i_ActionRoot)
    return;
  
  Action* pRoot = &gpActionSequence->m_oaAction[pJob->m_i_ActionRoot];
  Action* pEnd = gpActionSequence->pNewLinkedAction(pRoot);
  
  pEnd->m_iJobId     = pJob->m_iJobId;
  pEnd->m_eClassNo   = pJob->eClass();
  pEnd->m_iParentId  = pJob->m_iParentId;
  pEnd->m_bWasRecv   = pJob->m_bWasReceived;
  pEnd->m_i_Activity = pJob->m_i_Activity;
  
  Now xNow;

  pEnd->m_eAction  = AN_END;
  pEnd->m_iArg0    = xNow.iTsp();
  pEnd->m_iArg1    = 0;
  
  pEnd->m_aJob     = pJob;
#else
  
#endif
}

/*----------------------------------------------------------------------------*/
/* write Log to csv file                                              */
/*----------------------------------------------------------------------------*/
void ActionLog::vWriteCsv(void)
{
#ifdef WITH_ACTIVITY_MAP
  if (! gpActionSequence)
    return;
  
  gpActionSequence->vWriteToFile();
#endif
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get a new mark in Log (non-locking)                                */
/*----------------------------------------------------------------------------*/
Action* ActionLog::pAllotAction(void)
{
  _precondition_(_MAGIC_,CN_ACTIONLOG,_FOOTER_);
  
  vGrabLock();
  
  Action* pAction = &m_oaAction[m_i_Entry % m_nEntries];
  
  pAction->vReset();
  pAction->m_i_Entry = m_i_Entry;
  pAction->m_iTick    = Tick::iRdTscp();
  
  m_i_Entry++;
  
  vDropLock();
  
  return pAction;
}

/*----------------------------------------------------------------------------*/
/* find active range                                                          */
/*----------------------------------------------------------------------------*/
void ActionLog::vFindRange_NL(i8& i_Min, i8& i_Max)
{
  _precondition_(_MAGIC_,CN_ACTIONLOG,_FOOTER_);
  
  RASSERT(m_okLock.bIsLocked());
  
  // should be a quicker way to do this
  
  i_Min = C_I8_MAX;
  i_Max = C_I8_MIN;
  
  for (i8 i_Entry = 0; i_Entry < m_nEntries; i_Entry++) {
    
    if (-1 == m_oaAction[i_Entry].m_i_Entry)
      return;
    
    if (m_oaAction[i_Entry].m_i_Entry < i_Min)
      i_Min = m_oaAction[i_Entry].m_i_Entry;
    
    if (m_oaAction[i_Entry].m_i_Entry > i_Max)
      i_Max = m_oaAction[i_Entry].m_i_Entry;
  }
}

/*----------------------------------------------------------------------------*/
/* link body into chain given by head                                         */
/*----------------------------------------------------------------------------*/
void ActionLog::vLinkAction(Action* pRoot, Action* pNewAction)
{
  _precondition_(_MAGIC_,CN_ACTIONLOG,_FOOTER_);
  
  for (;;) {
    
    i8 iWhatRootLinksTo = * pRoot->m_wLink;
    pNewAction->m_wLink.vSetWithoutTest(iWhatRootLinksTo);
    
    if (pRoot->m_wLink.bTestAndSet(iWhatRootLinksTo,pNewAction->m_i_Entry))
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* get a new action (body)                                                    */
/*----------------------------------------------------------------------------*/
Action* ActionLog::pNewLinkedAction(Action *pRoot)
{
  _precondition_(_MAGIC_,CN_ACTIONLOG,_FOOTER_);
  
  Action* pLinked = pAllotAction();
  
  pLinked->m_i_Root = pRoot->m_i_Entry;
  
  vLinkAction(pRoot,pLinked);

  return pLinked;
}

/*----------------------------------------------------------------------------*/
/* get a new action (root)                                                    */
/*----------------------------------------------------------------------------*/
Action* ActionLog::pNewRootAction(void)
{
  _precondition_(_MAGIC_,CN_ACTIONLOG,_FOOTER_);
  
  Action* pRoot = pAllotAction();
  
  pRoot->m_i_Root = pRoot->m_i_Entry;
  pRoot->m_wLink.vSetWithoutTest(C_UNKNOWN);

  return pRoot;
}

/*----------------------------------------------------------------------------*/
/* get next action                                                            */
/*----------------------------------------------------------------------------*/
Action* ActionLog::pNextAction(Action *pAction)
{
  _precondition_(_MAGIC_,CN_ACTIONLOG,_FOOTER_);
  
  i8 i_NextAction = * pAction->m_wLink;
  
  if (i_NextAction == C_UNKNOWN)
    return nullptr;
  
  Action* pNextAction = &m_oaAction[i_NextAction % m_nEntries];;
  
  return pNextAction;
}

/*----------------------------------------------------------------------------*/
/* calculate how full the action sequence is                                  */
/*----------------------------------------------------------------------------*/
i8 ActionLog::iPercentFull_NL(void)
{
  _precondition_(_MAGIC_,CN_ACTIONLOG,_FOOTER_);
  
  RASSERT(m_okLock.bIsLocked());
  
  i8 nInUse = 0;
  i8 iPercent = 0;
  
  i8 i_Min;
  i8 i_Max;
  vFindRange_NL(i_Min,i_Max);
  
  if (i_Min < i_Max) {
    //
    // Example: 0 .. 25 __ [64]
    //          i_Min is 0
    //          i_Max is 48
    //          m_nEntries  is 64
    // then only 0..25 is in use out of 64
    //
    nInUse = (i_Max - i_Min);
  }
  else {
    //
    // Example: .. 25 __ 48 .. [64]
    //          i_Min is 48
    //          i_Max is 25
    //          m_nEntries  is 64
    // then 48 .. 63 + 0 ..25 is in use
    //
    nInUse = (m_nEntries - i_Min) + (i_Max);
  }
  
  iPercent = (nInUse * 100) / m_nEntries;
  
  return iPercent;
}

/*----------------------------------------------------------------------------*/
/* write ActionLog to a csv file                                         */
/*----------------------------------------------------------------------------*/
void ActionLog::vWriteToFile(void)
{
  _precondition_(_MAGIC_,CN_ACTIONLOG,_FOOTER_);
  
  s256 sAsPath("/tmp/ActionLog.csv"_Stz);
  
  File* pActSeqFile = new File();
  
  bool bOpened = pActSeqFile->bOpenWrite(sAsPath);
  if (! bOpened) {
    vTrace("Unable to open action sequence file: %s\n"_Stz,sAsPath.aChar());
    return;
  }
  
  Now oNow;
  Log::vNote("Writing ActionLog at: %s"_Stz,oNow.sz());
  
  
  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  vGrabLock();
  
  i8 i_Min;
  i8 i_Max;
  
  vFindRange_NL(i_Min,i_Max);
  
  for (i8 i_Entry = i_Min; i_Entry <= i_Max; i_Entry++) {

    Action* pAction = &m_oaAction[i_Entry % m_nEntries];
    
    if (! pAction->bIsRoot())
      continue;

    pAction->vWrite(pActSeqFile);
    
    while ( (pAction = pNextAction(pAction)) )
      pAction->vWrite(pActSeqFile);
  }
  
  vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲
  
  pActSeqFile->vClose();
  
  vTrace("ActionLog written to: %s\n"_Stz,sAsPath.aChar());
  
  delete pActSeqFile;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ActionLog::aFooter(void)
{
  _precondition_(_MAGIC_,CN_ACTIONLOG);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
