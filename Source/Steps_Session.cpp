/*============================================================================*/
/* Steps_Session.cpp - Session step group                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Session(void)
{
  vAddStep("session.commandProcessor"_Stz, &Step::vSessionCommand);
  vAddStep("session.dicStack"_Stz,         &Step::vSessionDicStack);
  vAddStep("session.dump"_Stz,             &Step::vSessionDump);
  vAddStep("session.isExiting"_Stz,        &Step::vSessionIsExiting);
  vAddStep("session.pid"_Stz,              &Step::vSessionPid);
  vAddStep("session.slot@"_Stz,            &Step::vSessionSlotFetch);
  vAddStep("session.tx@"_Stz,              &Step::vSessionTxFetch);
  vAddStep("session.tx!"_Stz,              &Step::vSessionTxStore);
}

/*----------------------------------------------------------------------------*/
/* session.commandProcessor -- get session command processor                  */
/*----------------------------------------------------------------------------*/
void Step::vSessionCommand(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vSessionDicStack"_Stz);
  
  Object* pPtrSession;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrSession) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Session* pSession = _pSession(pPtrSession);
  ASSERT(CN_SESSION == pSession->eClass());
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pSession->m_pInput);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.dicStack -- get session dic stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vSessionDicStack(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vSessionDicStack"_Stz);
  
  Object* pPtrSession;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrSession) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Session* pSession = _pSession(pPtrSession);
  ASSERT(CN_SESSION == pSession->eClass());
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pSession->pPathStack());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.dump -- dump of SE interface                                       */
/*----------------------------------------------------------------------------*/
void Step::vSessionDump(MP_Task mpTask, Forth* pForth)
{
ENTRY("vSessionDump"_Stz);
  
  Object* pPtrSession;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrSession) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Session* pSession = _pSession(pPtrSession);
  ASSERT(CN_SESSION == pSession->eClass());
  
  pSession->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.isExiting -- is session exiting                                    */
/*----------------------------------------------------------------------------*/
void Step::vSessionIsExiting(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vSessionIsExiting"_Stz);
  
  Object* pPtrSession;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrSession) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Session* pSession = _pSession(pPtrSession);
  ASSERT(CN_SESSION == pSession->eClass());
  
  mpTask->m_pForth->m_pDataStack->vPushI8(pSession->m_bSessionExiting);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.pid -- get pid of SE session                                       */
/*----------------------------------------------------------------------------*/
void Step::vSessionPid(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vSessionPid"_Stz);
  
  Object* pPtrSession;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrSession) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Session* pSession = _pSession(pPtrSession);
  ASSERT(CN_SESSION == pSession->eClass());
  
  mpTask->m_pForth->m_pDataStack->vPushI8(pSession->m_iPid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.slot@ -- fetch slot of SE session                                  */
/*----------------------------------------------------------------------------*/
void Step::vSessionSlotFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vSessionSlotFetch"_Stz);
  
  Object* pPtrSession;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrSession) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Session* pSession = _pSession(pPtrSession);
  ASSERT(CN_SESSION == pSession->eClass());
  
  mpTask->m_pForth->m_pDataStack->vPushI8(pSession->iSlot());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.tx@ -- fetch tx of SE session                                      */
/*----------------------------------------------------------------------------*/
void Step::vSessionTxFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vSessionTxFetch"_Stz);
  
  Object* pPtrSession;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrSession) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Session* pSession = _pSession(pPtrSession);
  
  Tx* pTx = pSession->pTx();
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pTx);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.tx! -- store tx of SE session                                      */
/*----------------------------------------------------------------------------*/
void Step::vSessionTxStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vSessionTxStore"_Stz);
  
  Object* pPtrSession;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrSession) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Session* pSession = _pSession(pPtrSession);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTx) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Tx* pTx = _pTx(pPtrTx);
  
  pSession->vTx(pTx);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
