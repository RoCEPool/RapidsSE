/*============================================================================*/
/* Steps_Session.cpp - Session step group                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Session(void)
{
  vAddStep("session::this"_Stz,      &Step::vSessionThis);
  
  vAddStep("session.dicStack@"_Stz,  &Step::vSessionDicStackFetch);
  vAddStep("session.dump"_Stz,       &Step::vSessionDump);
  vAddStep("session.tagSet@"_Stz,    &Step::vSessionTagSetFetch);
  vAddStep("session.isExiting"_Stz,  &Step::vSessionIsExiting);
  vAddStep("session.pid"_Stz,        &Step::vSessionPid);
  vAddStep("session.tablePtn@"_Stz,  &Step::vSessionTablePtnFetch);
  vAddStep("session.todo@"_Stz,      &Step::vSessionToDoFetch);
  vAddStep("session.todo!"_Stz,      &Step::vSessionToDoStore);
  vAddStep("session.tx@"_Stz,        &Step::vSessionTxFetch);
  vAddStep("session.tx!"_Stz,        &Step::vSessionTxStore);
}

/*----------------------------------------------------------------------------*/
/* session.dicStack@ -- get session dic stack                                 */
/*----------------------------------------------------------------------------*/
void Step::vSessionDicStackFetch(void)
{
  ENTRY();
  
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
  ENTRY();
  
  Session* pSession = _pSession(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_SESSION == pSession->eClass());
  
  pSession->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.tagSet@ -- fetch ename-set of session semantic                     */
/*----------------------------------------------------------------------------*/
void Step::vSessionTagSetFetch(void)
{
  ENTRY();
  
  Session* pSession = _pSession(m_pDataStack->pPopPtr(__func__));
  
  TagSet* pSsnTagSet = & pSession->m_oSsnTagSet;
  
  m_pDataStack->vPushPtr(pSsnTagSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.isExiting -- is session exiting                                    */
/*----------------------------------------------------------------------------*/
void Step::vSessionIsExiting(void)
{
  ENTRY();
  
  Session* pSession = _pSession(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_SESSION == pSession->eClass());
  
  m_pDataStack->vPushInt(pSession->m_bExiting);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.pid -- get pid of SE session                                       */
/*----------------------------------------------------------------------------*/
void Step::vSessionPid(void)
{
  ENTRY();
  
  Session* pSession = _pSession(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_SESSION == pSession->eClass());
  
  m_pDataStack->vPushInt(pSession->m_iPid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.tablePtn@ -- fetch tablePtn of SE session                           */
/*----------------------------------------------------------------------------*/
void Step::vSessionTablePtnFetch(void)
{
  ENTRY();
  
  Session* pSession = _pSession(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_SESSION == pSession->eClass());
  
  m_pDataStack->vPushInt(pSession->i_Strand());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session::this -- get current session                                       */
/*----------------------------------------------------------------------------*/
void Step::vSessionThis(void)
{
  ENTRY();
 
  Job* pJob = m_pTask->m_pJob;

  i8 iSessionId = pJob->m_xReqTuple.m_i_Session;
  
  Session* pSession = nullptr;
  
  if (iSessionId != C_UNKNOWN)
    pSession = AM::pSessionArray()->pGetSession(iSessionId);
  
  m_pDataStack->vPushPtr(pSession);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.todo@ -- fetch todo of SE session                                  */
/*----------------------------------------------------------------------------*/
void Step::vSessionToDoFetch(void)
{
  ENTRY();
  
  Session* pSession = _pSession(m_pDataStack->pPopPtr(__func__));

  m_pDataStack->vPushStr(pSession->m_sToDo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.toDo! -- store todo of SE session                                  */
/*----------------------------------------------------------------------------*/
void Step::vSessionToDoStore(void)
{
  ENTRY();
  
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
  ENTRY();
  
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
  ENTRY();
  
  Session* pSession = _pSession(m_pDataStack->pPopPtr(__func__));
  
  Tx* pTx = _pTx(m_pDataStack->pPopPtr(__func__));
  
  pSession->vTx(pTx);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
