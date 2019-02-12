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
i8 ActionSequence::iAdd(i8 i_RootAct, i8 iJobId, i8 iAction, i8 iArg0, i8 iArg1)
{
#ifdef WITH_ACTION_SEQUENCE
  if (! gpActionSequence)
    return -1;
  
  RASSERT(i_RootAct > -1);

  Action* pRoot = &gpActionSequence->m_oaAction[i_RootAct];
  
  Action* pAdded = gpActionSequence->pNewAction(pRoot);
  
  pAdded->m_iJobId  = iJobId;
  pAdded->m_iAction = iAction;
  pAdded->m_iArg0   = iArg0;
  pAdded->m_iArg1   = iArg1;
  
  return pAdded->m_i_Action;
#else
  return -1;
#endif
}

/*----------------------------------------------------------------------------*/
/* begin a new action sequence                                                */
/*----------------------------------------------------------------------------*/
i8 ActionSequence::iBegin(i8 iJobId, i8 iAction, i8 iArg0, i8 iArg1)
{
#ifdef WITH_ACTION_SEQUENCE
  if (! gpActionSequence)
    return C_UNKNOWN;
  
  Action* pRoot = gpActionSequence->pNewRoot();
  pRoot->m_iJobId  = iJobId;
  pRoot->m_iAction = iAction;
  pRoot->m_iArg0   = iArg0;
  pRoot->m_iArg1   = iArg1;

  return pRoot->m_i_Action;
#else
  return C_UNKNOWN;
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
  _precondition_(_MAGIC_,CN_ACTIVITYMAP,_FOOTER_);
  
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
Action* ActionSequence::pNewAction(Action *pRootAction)
{
  _precondition_(_MAGIC_,CN_ACTIONSEQUENCE,_FOOTER_);
  
  Action* pAction = pAllotAction();
  
  pAction->m_i_Root = pRootAction->m_i_Action;
  
  vLinkAction(pRootAction,pAction);

  return pAction;
}

/*----------------------------------------------------------------------------*/
/* get a new action (root)                                                    */
/*----------------------------------------------------------------------------*/
Action* ActionSequence::pNewRoot(void)
{
  _precondition_(_MAGIC_,CN_ACTIONSEQUENCE,_FOOTER_);
  
  Action* pRootAction = pAllotAction();
  
  pRootAction->m_i_Root = pRootAction->m_i_Action;
  
  return pRootAction;
}

/*----------------------------------------------------------------------------*/
/* get a new action (body)                                                    */
/*----------------------------------------------------------------------------*/
Action* ActionSequence::pNextAction(Action *pAction)
{
  _precondition_(_MAGIC_,CN_ACTIONSEQUENCE,_FOOTER_);
  
  i8 i_NextAction = * pAction->m_wLink;
  
  if (i_NextAction == -1)
    return nullptr;
  
  Action* pNextAction = &m_oaAction[i_NextAction % m_nActions];;
  
  return pNextAction;
}

/*----------------------------------------------------------------------------*/
/* calculate how full the activity map is                                     */
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
    //          i_MinMark is 0
    //          i_Maxmark is 48
    //          m_nMarks  is 64
    // then only 0..25 is in use out of 64
    //
    nInUse = (i_Max - i_Min);
  }
  else {
    //
    // Example: .. 25 __ 48 .. [64]
    //          i_MinMark is 48
    //          i_Maxmark is 25
    //          m_nMarks  is 64
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
  
  i8 iTotalDelta = 0;
  
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
  
  i8 i_MinMark;
  i8 i_MaxMark;
  
  vFindRange_NL(i_MinMark,i_MaxMark);
  
  for (i8 i_Mark = i_MinMark + 1; i_Mark <= i_MaxMark; i_Mark++) {

    Action* pAction = &m_oaAction[i_MinMark % m_nActions];
    
    if (! pAction->bIsRoot())
      continue;

    pAction->vWrite(pActSeqFile);
    
    while (pAction = pNextAction(pAction))
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
