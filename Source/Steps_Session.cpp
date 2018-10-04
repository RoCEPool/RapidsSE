/*============================================================================*/
/* Steps_Session.cpp - Session step group                                     */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// session: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Session(void)
{
  vAddStep("session.commandProcessor"_Stz, &Task::vSessionCommandProcessor, false);
  vAddStep("session.dicStack"_Stz,         &Task::vSessionDicStack,         false);
  vAddStep("session.dump"_Stz,             &Task::vSessionDump,             false);
  vAddStep("session.isExiting"_Stz,        &Task::vSessionIsExiting,        false);
  vAddStep("session.pid"_Stz,              &Task::vSessionPid,              false);
  vAddStep("session.slot@"_Stz,            &Task::vSessionSlotFetch,        false);
  vAddStep("session.tx@"_Stz,              &Task::vSessionTxFetch,          false);
  vAddStep("session.tx!"_Stz,              &Task::vSessionTxStore,          false);
}

/*----------------------------------------------------------------------------*/
/* session.commandProcessor -- get SEitf command processor                      */
/*----------------------------------------------------------------------------*/
void Task::vSessionCommandProcessor(MP_Task mpTask)
{
  ENTRY("vSessionDicStack"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("session.dicStack"_Stz);
  
  Session* pSession = _pSession(pPtrSEitf);
  ASSERT(CN_SESSION == pSession->eClass());
  
  mpTask->m_pDataStack->vPushObject(pSession->m_pCommandProcessor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.dicStack -- get SEitf dic stack                                    */
/*----------------------------------------------------------------------------*/
void Task::vSessionDicStack(MP_Task mpTask)
{
  ENTRY("vSessionDicStack"_Stz);
  
  Object* pPtrSession;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSession) )
    THROW_INVALID_DSTACK("session.dicStack"_Stz);
  
  Session* pSession = _pSession(pPtrSession);
  ASSERT(CN_SESSION == pSession->eClass());
  
  mpTask->m_pDataStack->vPushObject(pSession->pPathStack());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.dump -- dump of SE interface                                         */
/*----------------------------------------------------------------------------*/
void Task::vSessionDump(MP_Task mpTask)
{
ENTRY("vSessionDump"_Stz);
  
  Object* pPtrSession;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSession) )
    THROW_INVALID_DSTACK("session.dump"_Stz);
  
  Session* pSession = _pSession(pPtrSession);
  ASSERT(CN_SESSION == pSession->eClass());
  
  pSession->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.isExiting -- is seitf exiting                                        */
/*----------------------------------------------------------------------------*/
void Task::vSessionIsExiting(MP_Task mpTask)
{
  ENTRY("vSessionIsExiting"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("session.isExiting"_Stz);
  
  Session* pSession = _pSession(pPtrSEitf);
  ASSERT(CN_SESSION == pSession->eClass());
  
  mpTask->m_pDataStack->vPushInt(pSession->m_bSessionExiting);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.pid -- get pid of SE interface                                       */
/*----------------------------------------------------------------------------*/
void Task::vSessionPid(MP_Task mpTask)
{
  ENTRY("vSessionPid"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("session.pid"_Stz);
  
  Session* pSession = _pSession(pPtrSEitf);
  ASSERT(CN_SESSION == pSession->eClass());
  
  mpTask->m_pDataStack->vPushInt(pSession->m_iPid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.slot@ -- fetch slot of SE interface                                  */
/*----------------------------------------------------------------------------*/
void Task::vSessionSlotFetch(MP_Task mpTask)
{
  ENTRY("vSessionSlotFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("session.slot@"_Stz);
  
  Session* pSession = _pSession(pPtrSEitf);
  ASSERT(CN_SESSION == pSession->eClass());
  
  mpTask->m_pDataStack->vPushInt(pSession->iSlot());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.tx@ -- fetch tx of SE interface                                      */
/*----------------------------------------------------------------------------*/
void Task::vSessionTxFetch(MP_Task mpTask)
{
  ENTRY("vSessionTxFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("session.tx@"_Stz);
  Session* pSession = _pSession(pPtrSEitf);
  
  Tx* pTx = pSession->pTx();
  
  mpTask->m_pDataStack->vPushObject(pTx);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.tx! -- store tx of SE interface                                      */
/*----------------------------------------------------------------------------*/
void Task::vSessionTxStore(MP_Task mpTask)
{
  ENTRY("vSessionTxStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("session.tx!"_Stz);
  Session* pSession = _pSession(pPtrSEitf);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("session.tx!"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  pSession->vTx(pTx);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
