/*============================================================================*/
/* ActionSequence.cpp - Activity Map                                          */
/*============================================================================*/

#include "Maps.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

ActionSequence* gpActionSequence = nullptr;


/*----------------------------------------------------------------------------*/
/* -- LifeCycle --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ActionSequence::operator new(size_t, ActionSequence* pActionSequence)
{
  RASSERT(! gpActionSequence);
  
  return pActionSequence;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ActionSequence::operator delete(void* aAddress)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ActionSequence::ActionSequence(void)
: Map(CN_ACTIONSEQUENCE)
{
  _precondition_(_MAGIC_,CN_ACTIONSEQUENCE,_FOOTER_);
  
  m_okLock.vInitLock(LK_ACTION_JOURNAL);
  
  m_i_Action = 0;
  m_nActions = C_ACTION_SEQUENCE;
  
  for (i8 i_Action = 0; i_Action < m_nActions; i_Action++)
    m_oaAction[i_Action].vInit(i_Action);
  
  _checkInitialised_(CN_ACTIONSEQUENCE);
  
  _postcondition_(_MAGIC_,CN_ACTIONSEQUENCE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ActionSequence::~ActionSequence(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ACTIONSEQUENCE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create or open ActivityMap (called by SE)                                  */
/*----------------------------------------------------------------------------*/
void ActionSequence::vOpenBySE(bool bStarting)
{
#ifdef WITH_ACTION_SEQUENCE
  if (! gpAttachMap->Arg.m_bActionSequence)
    return;
  
  if (gpActionSequence)
    vFatal("ActivityMap::vOpenBySE() -- activity map already opened\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s32 sMapNm("/SE_ActionSequence"_Stz);
  i8 nMapSz = sizeof(ActionSequence);
  
  ActionSequence* pActionSequence = nullptr;
  
  if (bStarting) {
    pActionSequence = (ActionSequence*)Map::aMapCreate(sMapNm,nMapSz);
    if (gpAttachMap->m_bVerbose)
      vTrace("ActionSequence: created new map %s %lldB\n"_Stz,
             sMapNm.aStz(),
             nMapSz);
    gpActionSequence = new(pActionSequence) ActionSequence;
    
  }
  else {
    pActionSequence = (ActionSequence*)Map::aMapExisting(sMapNm,nMapSz);
    if (gpAttachMap->m_bVerbose)
      vTrace("ActionSequence: attached to existing map %s %lldB\n"_Stz,
             sMapNm.aStz(),
             nMapSz);

    gpActionSequence = pActionSequence;
  }
  
#endif
}


/*----------------------------------------------------------------------------*/
/* add a new action to a sequence                                             */
/*----------------------------------------------------------------------------*/
void ActionSequence::vAdd(i8 i_RootAct, i8 iJobId, eCN eClassNo,
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
void ActionSequence::vAdd(Jobs::Job* pJob,
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
  
  pLinked->m_iJobId    = pJob->m_iJobId;
  pLinked->m_eClassNo  = pJob->eClass();
  pLinked->m_iParentId = pJob->m_iParentId;
  pLinked->m_bWasRecv  = pJob->m_bWasReceived;
  
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
void ActionSequence::vAdd(Jobs::Job* pJob, eAN eAction, Stz* szText)
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
  
  pLinked->m_iJobId    = pJob->m_iJobId;
  pLinked->m_eClassNo  = pJob->eClass();
  pLinked->m_iParentId = pJob->m_iParentId;
  pLinked->m_bWasRecv  = pJob->m_bWasReceived;
  
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
void ActionSequence::vAdd(Jobs::Job* pJob, eAN eAction,
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
  
  pLinked->m_iJobId    = pJob->m_iJobId;
  pLinked->m_eClassNo  = pJob->eClass();
  pLinked->m_iParentId = pJob->m_iParentId;
  pLinked->m_bWasRecv  = pJob->m_bWasReceived;
  
  pLinked->m_eAction   = eAction;
  pLinked->m_iArg0     = iArg0;
  pLinked->m_iArg1     = iArg1;

  pLinked->m_iJobId_Arg    = pSystemJob->m_iJobId;
  pLinked->m_eClassNo_Arg  = pSystemJob->eClass();
  pLinked->m_iParentId_Arg = pSystemJob->m_iParentId;
  pLinked->m_bWasRecv_Arg  = pSystemJob->m_bWasReceived;
  
  pLinked->m_aJob     = pJob;
#else
  
#endif
}

/*----------------------------------------------------------------------------*/
/* begin a new action sequence                                                */
/*----------------------------------------------------------------------------*/
i8 ActionSequence::iBegin(i8 iJobId, eCN eClassNo,
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

  return pRoot->m_i_Action;
#else
  return C_UNKNOWN;
#endif
}

/*----------------------------------------------------------------------------*/
/* begin a new action sequence                                                */
/*----------------------------------------------------------------------------*/
void ActionSequence::vRoot(Jobs::Job* pJob)
{
#ifdef WITH_ACTION_SEQUENCE
  if (! gpActionSequence)
    return;

  if (! (pJob->m_bMarkAction ||
         gpAttachMap->m_oMarkAction.bFlagged(pJob->eClass())) )
    return;
  
  Action* pRoot = gpActionSequence->pNewRootAction();
  
  pJob->m_i_ActionRoot = pRoot->m_i_Action;
  
  pRoot->m_iJobId    = pJob->m_iJobId;
  pRoot->m_eClassNo  = pJob->eClass();
  pRoot->m_iParentId = pJob->m_iParentId;
  pRoot->m_bWasRecv  = pJob->m_bWasReceived;
  
  Tsp wTspNow;
  wTspNow.vSetAsNow();
  
  pRoot->m_eAction  = AN_ROOT;
  pRoot->m_iArg0    = wTspNow.iAsI8();
  pRoot->m_iArg1    = 0;
  
  pRoot->m_aJob     = pJob;
#else

#endif
}

/*----------------------------------------------------------------------------*/
/* begin a new action sequence                                                */
/*----------------------------------------------------------------------------*/
void ActionSequence::vEnd(Jobs::Job* pJob)
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
  
  pEnd->m_iJobId    = pJob->m_iJobId;
  pEnd->m_eClassNo  = pJob->eClass();
  pEnd->m_iParentId = pJob->m_iParentId;
  pEnd->m_bWasRecv  = pJob->m_bWasReceived;
  
  Tsp wTspNow;
  wTspNow.vSetAsNow();
  
  pEnd->m_eAction  = AN_END;
  pEnd->m_iArg0    = wTspNow.iAsI8();
  pEnd->m_iArg1    = 0;
  
  pEnd->m_aJob     = pJob;
#else
  
#endif
}

/*----------------------------------------------------------------------------*/
/* write ActivityMap to csv file                                              */
/*----------------------------------------------------------------------------*/
void ActionSequence::vWriteCsv(void)
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
/* get a new mark in ActivityMap (non-locking)                                */
/*----------------------------------------------------------------------------*/
Action* ActionSequence::pAllotAction(void)
{
  _precondition_(_MAGIC_,CN_ACTIONSEQUENCE,_FOOTER_);
  
  vGrabLock();
  
  Action* pAction = &m_oaAction[m_i_Action % m_nActions];
  
  pAction->vReset();
  pAction->m_i_Action = m_i_Action;
  pAction->m_iTick    = Tsc::iRdTscp();
  
  m_i_Action++;
  
  vDropLock();
  
  return pAction;
}

/*----------------------------------------------------------------------------*/
/* drop lock on ActionSequence                                                */
/*----------------------------------------------------------------------------*/
void ActionSequence::vDropLock(void)
{
  _precondition_(_MAGIC_,CN_ACTIONSEQUENCE,_FOOTER_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* find active range                                                          */
/*----------------------------------------------------------------------------*/
void ActionSequence::vFindRange_NL(i8& i_Min, i8& i_Max)
{
  _precondition_(_MAGIC_,CN_ACTIONSEQUENCE,_FOOTER_);
  
  RASSERT(m_okLock.bIsLocked());
  
  // should be a quicker way to do this
  
  i_Min = C_I8_MAX;
  i_Max = C_I8_MIN;
  
  for (i8 i_Action = 0; i_Action < m_nActions; i_Action++) {
    
    if (-1 == m_oaAction[i_Action].m_i_Action)
      return;
    
    if (m_oaAction[i_Action].m_i_Action < i_Min)
      i_Min = m_oaAction[i_Action].m_i_Action;
    
    if (m_oaAction[i_Action].m_i_Action > i_Max)
      i_Max = m_oaAction[i_Action].m_i_Action;
  }
}

/*----------------------------------------------------------------------------*/
/* grab lock on ActionSequence                                                */
/*----------------------------------------------------------------------------*/
void ActionSequence::vGrabLock(void)
{
  _precondition_(_MAGIC_,CN_ACTIONSEQUENCE,_FOOTER_);
  
  m_okLock.vGrabWrite();
}



/*----------------------------------------------------------------------------*/
/* link body into chain given by head                                         */
/*----------------------------------------------------------------------------*/
void ActionSequence::vLinkAction(Action* pRoot, Action* pNewAction)
{
  _precondition_(_MAGIC_,CN_ACTIONSEQUENCE,_FOOTER_);
  
  for (;;) {
    
    i8 iWhatRootLinksTo = * pRoot->m_wLink;
    pNewAction->m_wLink.vSetWithoutTest(iWhatRootLinksTo);
    
    if (pRoot->m_wLink.bTestAndSet(iWhatRootLinksTo,pNewAction->m_i_Action))
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* get a new action (body)                                                    */
/*----------------------------------------------------------------------------*/
Action* ActionSequence::pNewLinkedAction(Action *pRoot)
{
  _precondition_(_MAGIC_,CN_ACTIONSEQUENCE,_FOOTER_);
  
  Action* pLinked = pAllotAction();
  
  pLinked->m_i_Root = pRoot->m_i_Action;
  
  vLinkAction(pRoot,pLinked);

  return pLinked;
}

/*----------------------------------------------------------------------------*/
/* get a new action (root)                                                    */
/*----------------------------------------------------------------------------*/
Action* ActionSequence::pNewRootAction(void)
{
  _precondition_(_MAGIC_,CN_ACTIONSEQUENCE,_FOOTER_);
  
  Action* pRoot = pAllotAction();
  
  pRoot->m_i_Root = pRoot->m_i_Action;
  pRoot->m_wLink.vSetWithoutTest(C_UNKNOWN);

  return pRoot;
}

/*----------------------------------------------------------------------------*/
/* get next action                                                            */
/*----------------------------------------------------------------------------*/
Action* ActionSequence::pNextAction(Action *pAction)
{
  _precondition_(_MAGIC_,CN_ACTIONSEQUENCE,_FOOTER_);
  
  i8 i_NextAction = * pAction->m_wLink;
  
  if (i_NextAction == C_UNKNOWN)
    return nullptr;
  
  Action* pNextAction = &m_oaAction[i_NextAction % m_nActions];;
  
  return pNextAction;
}

/*----------------------------------------------------------------------------*/
/* calculate how full the action sequence is                                  */
/*----------------------------------------------------------------------------*/
i8 ActionSequence::iPercentFull_NL(void)
{
  _precondition_(_MAGIC_,CN_ACTIONSEQUENCE,_FOOTER_);
  
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
    //          m_nActions  is 64
    // then only 0..25 is in use out of 64
    //
    nInUse = (i_Max - i_Min);
  }
  else {
    //
    // Example: .. 25 __ 48 .. [64]
    //          i_Min is 48
    //          i_Max is 25
    //          m_nActions  is 64
    // then 48 .. 63 + 0 ..25 is in use
    //
    nInUse = (m_nActions - i_Min) + (i_Max);
  }
  
  iPercent = (nInUse * 100) / m_nActions;
  
  return iPercent;
}

/*----------------------------------------------------------------------------*/
/* write ActionSequence to a csv file                                         */
/*----------------------------------------------------------------------------*/
void ActionSequence::vWriteToFile(void)
{
  _precondition_(_MAGIC_,CN_ACTIONSEQUENCE,_FOOTER_);
  
  s256 sAsPath("/tmp/ActionSequence.csv"_Stz);
  
  File* pActSeqFile = new File();
  
  bool bOpened = pActSeqFile->bOpenWrite(sAsPath);
  if (! bOpened) {
    vTrace("Unable to open action sequence file: %s\n"_Stz,sAsPath.aChar());
    return;
  }
  
  Now oNow;
  ActivityMap::vMarkInfo("Writing ActionSequence at: %s"_Stz,oNow.sFormatted());
  
  
  // ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼ ▼
  vGrabLock();
  
  i8 i_Min;
  i8 i_Max;
  
  vFindRange_NL(i_Min,i_Max);
  
  for (i8 i_Action = i_Min; i_Action <= i_Max; i_Action++) {

    Action* pAction = &m_oaAction[i_Action % m_nActions];
    
    if (! pAction->bIsRoot())
      continue;

    pAction->vWrite(pActSeqFile);
    
    while ( (pAction = pNextAction(pAction)) )
      pAction->vWrite(pActSeqFile);
  }
  
  vDropLock();
  // ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲ ▲
  
  pActSeqFile->vClose();
  
  vTrace("ActionSequence written to: %s\n"_Stz,sAsPath.aChar());
  
  delete pActSeqFile;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ActionSequence::aFooter(void)
{
  _precondition_(_MAGIC_,CN_ACTIONSEQUENCE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
