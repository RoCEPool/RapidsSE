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
  vAddStep("session.dicStack@"_Stz,        &Step::vSessionDicStackFetch);
  vAddStep("session.dump"_Stz,             &Step::vSessionDump);
  vAddStep("session.enameSet@"_Stz,        &Step::vSessionEnameSetFetch);
  vAddStep("session.isExiting"_Stz,        &Step::vSessionIsExiting);
  vAddStep("session.pid"_Stz,              &Step::vSessionPid);
  vAddStep("session.slot@"_Stz,            &Step::vSessionSlotFetch);
  vAddStep("session.tx@"_Stz,              &Step::vSessionTxFetch);
  vAddStep("session.tx!"_Stz,              &Step::vSessionTxStore);
}

/*----------------------------------------------------------------------------*/
/* session.commandProcessor -- get session command processor                  */
/*----------------------------------------------------------------------------*/
void Step::vSessionCommand(void)
{
  ENTRY(); // ENTRY("vSessionCommand"_Stz);
  
  Object* pPtrSession; m_pDataStack->vPopPtr(pPtrSession,__func__);
  
  Session* pSession = _pSession(pPtrSession);
  ASSERT(CN_SESSION == pSession->eClass());
  
  m_pDataStack->vPushPtr(pSession->m_pInput);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.dicStack@ -- get session dic stack                                 */
/*----------------------------------------------------------------------------*/
void Step::vSessionDicStackFetch(void)
{
  ENTRY(); // ENTRY("vSessionDicStackFetch"_Stz);
  
  Object* pPtrSession; m_pDataStack->vPopPtr(pPtrSession,__func__);
  
  Session* pSession = _pSession(pPtrSession);
  ASSERT(pSession);
  ASSERT(CN_SESSION == pSession->eClass());
  
  m_pDataStack->vPushPtr(pSession->pPathStack());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.dump -- dump of SE interface                                       */
/*----------------------------------------------------------------------------*/
void Step::vSessionDump(void)
{
  ENTRY(); // ENTRY("vSessionDump"_Stz);
  
  Object* pPtrSession; m_pDataStack->vPopPtr(pPtrSession,__func__);
  
  Session* pSession = _pSession(pPtrSession);
  ASSERT(CN_SESSION == pSession->eClass());
  
  pSession->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.enameSet@ -- fetch ename-set of session semantic                   */
/*----------------------------------------------------------------------------*/
void Step::vSessionEnameSetFetch(void)
{
  ENTRY(); // ENTRY("vSessionEnameSetFetch"_Stz);
  
  Object* pPtrSession; m_pDataStack->vPopPtr(pPtrSession,__func__);
  Session* pSession = _pSession(pPtrSession);
  
  EnameSet* pEnameSet = pSession->m_pSemantic->pEnameSet();
  
  m_pDataStack->vPushPtr(pEnameSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.isExiting -- is session exiting                                    */
/*----------------------------------------------------------------------------*/
void Step::vSessionIsExiting(void)
{
  ENTRY(); // ENTRY("vSessionIsExiting"_Stz);
  
  Object* pPtrSession; m_pDataStack->vPopPtr(pPtrSession,__func__);
  
  Session* pSession = _pSession(pPtrSession);
  ASSERT(CN_SESSION == pSession->eClass());
  
  m_pDataStack->vPushInt(pSession->m_bSessionExiting);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.pid -- get pid of SE session                                       */
/*----------------------------------------------------------------------------*/
void Step::vSessionPid(void)
{
  ENTRY(); // ENTRY("vSessionPid"_Stz);
  
  Object* pPtrSession; m_pDataStack->vPopPtr(pPtrSession,__func__);
  
  Session* pSession = _pSession(pPtrSession);
  ASSERT(CN_SESSION == pSession->eClass());
  
  m_pDataStack->vPushInt(pSession->m_iPid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.slot@ -- fetch slot of SE session                                  */
/*----------------------------------------------------------------------------*/
void Step::vSessionSlotFetch(void)
{
  ENTRY(); // ENTRY("vSessionSlotFetch"_Stz);
  
  Object* pPtrSession; m_pDataStack->vPopPtr(pPtrSession,__func__);
  
  Session* pSession = _pSession(pPtrSession);
  ASSERT(CN_SESSION == pSession->eClass());
  
  m_pDataStack->vPushInt(pSession->iSlot());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.tx@ -- fetch tx of SE session                                      */
/*----------------------------------------------------------------------------*/
void Step::vSessionTxFetch(void)
{
  ENTRY(); // ENTRY("vSessionTxFetch"_Stz);
  
  Object* pPtrSession; m_pDataStack->vPopPtr(pPtrSession,__func__);
  Session* pSession = _pSession(pPtrSession);
  
  Tx* pTx = pSession->pTx();
  
  m_pDataStack->vPushPtr(pTx);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.tx! -- store tx of SE session                                      */
/*----------------------------------------------------------------------------*/
void Step::vSessionTxStore(void)
{
  ENTRY(); // ENTRY("vSessionTxStore"_Stz);
  
  Object* pPtrSession; m_pDataStack->vPopPtr(pPtrSession,__func__);
  Session* pSession = _pSession(pPtrSession);
  
  Object* pPtrTx; m_pDataStack->vPopPtr(pPtrTx,__func__);
  Tx* pTx = _pTx(pPtrTx);
  
  pSession->vTx(pTx);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
