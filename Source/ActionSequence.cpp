/*============================================================================*/
/* ActionSequence.cpp - Activity Map                                           */
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
