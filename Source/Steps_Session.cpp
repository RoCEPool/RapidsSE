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
  vAddStep("session.todo@"_Stz,            &Step::vSessionToDoFetch);
  vAddStep("session.todo!"_Stz,            &Step::vSessionToDoStore);
  vAddStep("session.tx@"_Stz,              &Step::vSessionTxFetch);
  vAddStep("session.tx!"_Stz,              &Step::vSessionTxStore);
}

/*----------------------------------------------------------------------------*/
/* session.commandProcessor -- get session command processor                  */
/*----------------------------------------------------------------------------*/
void Step::vSessionCommand(void)
{
  ENTRY(); // ENTRY("vSessionCommand"_Stz);
  
  Session* pSession = _pSession(m_pDataStack->pPopPtr(__func__));
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
  
  Session* pSession = _pSession(m_pDataStack->pPopPtr(__func__));
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
  
  Session* pSession = _pSession(m_pDataStack->pPopPtr(__func__));
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
  
  Session* pSession = _pSession(m_pDataStack->pPopPtr(__func__));
  
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
  
  Session* pSession = _pSession(m_pDataStack->pPopPtr(__func__));
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
  
  Session* pSession = _pSession(m_pDataStack->pPopPtr(__func__));
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
  
  Session* pSession = _pSession(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_SESSION == pSession->eClass());
  
  m_pDataStack->vPushInt(pSession->iSlot());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.todo@ -- fetch todo of SE session                                  */
/*----------------------------------------------------------------------------*/
void Step::vSessionToDoFetch(void)
{
  ENTRY(); // ENTRY("vSessionToDoFetch"_Stz);
  
  Session* pSession = _pSession(m_pDataStack->pPopPtr(__func__));

  m_pDataStack->vPushStr(pSession->m_sToDo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.toDo! -- store todo of SE session                                  */
/*----------------------------------------------------------------------------*/
void Step::vSessionToDoStore(void)
{
  ENTRY(); // ENTRY("vSessionTxStore"_Stz);
  
  Session* pSession = _pSession(m_pDataStack->pPopPtr(__func__));
  
  s4K sToDo;
  m_pDataStack->vPopStr(sToDo,__func__);

  pSession->m_sToDo = sToDo;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.tx@ -- fetch tx of SE session                                      */
/*----------------------------------------------------------------------------*/
void Step::vSessionTxFetch(void)
{
  ENTRY(); // ENTRY("vSessionTxFetch"_Stz);
  
  Session* pSession = _pSession(m_pDataStack->pPopPtr(__func__));
  
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
  
  Session* pSession = _pSession(m_pDataStack->pPopPtr(__func__));
  
  Tx* pTx = _pTx(m_pDataStack->pPopPtr(__func__));
  
  pSession->vTx(pTx);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
