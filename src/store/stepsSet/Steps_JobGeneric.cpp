/*============================================================================*/
/* Steps_JobGeneric.cpp - generic job step group                              */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Grid.hpp"
#include "Job.hpp"
#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobGeneric(void)
{
  vAddStep("job_Begun::new"_Stz,        &Step::vJob_BegunNew);
  vAddStep("job_Cancel::new"_Stz,       &Step::vJob_CancelNew);
  vAddStep("job_Cancelled::new"_Stz,    &Step::vJob_CancelledNew);
  vAddStep("job_End::new"_Stz,          &Step::vJob_EndNew);
  vAddStep("job_Ended::new"_Stz,        &Step::vJob_EndedNew);
  vAddStep("job_Execute::new"_Stz,      &Step::vJob_ExecuteNew);
  vAddStep("job_Executed::new"_Stz,     &Step::vJob_ExecutedNew);
}

/*----------------------------------------------------------------------------*/
/* job_begun::new -- new a job_begun for this distributed 2pc job            */
/*----------------------------------------------------------------------------*/
void Step::vJob_BegunNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));

  Job* pJobBegun = new Job("begun"_Stz,
                           pDistJob->m_i_Origin,
                           pDistJob->m_i_Session,
                           pDistJob->m_iCmdNo
                           );
  
  pJobBegun->m_tJobId = pDistJob->tJobId();
  pJobBegun->vParticipantNode(AM::pGrid()->iThisNodeNo());
  pJobBegun->vJobKind(JK_GENERIC);
  
  TagSet* pJobBegunTags = pJobBegun->pJobTags();
  
  Word* pwCmdBegunTags = AM::WORD::pFind("begun_cmdTags"_Stz);
  TagSet* pCmdBegunTags = pwCmdBegunTags->pWrdTagSet();
  pJobBegunTags->vCopy(pCmdBegunTags);
  
  pJobBegunTags->vInsert(C_TAG_JOBNM,"begun"_Stz);
  //pJobBegunTags->vInsert(C_TAG_PATH,"/"_Stz);
  pJobBegunTags->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobBegunTags->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeNo());
  //pJobBegunTags->vInsert(C_TAG_SEEDRUN,"begun::begun"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobBegun->pJobTags()->vInsert(C_TAG_FLOW,1);

  m_pDataStack->vPushPtr(pJobBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_cancel::new -- new a job_cancel for this distributed 2pc job         */
/*----------------------------------------------------------------------------*/
void Step::vJob_CancelNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Job* pJobCancel = new Job("cancel"_Stz,
                            pDistJob->m_i_Origin,
                            pDistJob->m_i_Session,
                            pDistJob->m_iCmdNo
                            );
  
  pJobCancel->m_tJobId = pDistJob->tJobId();
  pJobCancel->vParticipantNode(AM::pGrid()->iThisNodeNo());
  pJobCancel->vJobKind(JK_GENERIC);

  TagSet* pJobCancelTags = pJobCancel->pJobTags();

  Word* pwCmdCancelTags = AM::WORD::pFind("cancel_cmdTags"_Stz);
  TagSet* pCmdCancelTags = pwCmdCancelTags->pWrdTagSet();
  pJobCancelTags->vCopy(pCmdCancelTags);

  pJobCancel->pJobTags()->vInsert(C_TAG_JOBNM,"cancel"_Stz);
  //pJobCancel->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobCancel->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobCancel->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeNo());
  //pJobCancel->pJobTags()->vInsert(C_TAG_SEEDRUN,"cancel::cancel"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobCancel->pJobTags()->vInsert(C_TAG_FLOW,1);
    
  m_pDataStack->vPushPtr(pJobCancel);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_cancelled::new -- new a job_cancelled for this distributed 2pc job   */
/*----------------------------------------------------------------------------*/
void Step::vJob_CancelledNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));

  Job* pJobCancelled = new Job("cancelled"_Stz,
                               pDistJob->m_i_Origin,
                               pDistJob->m_i_Session,
                               pDistJob->m_iCmdNo
                               );
  
  pJobCancelled->m_tJobId = pDistJob->tJobId();
  pJobCancelled->vParticipantNode(AM::pGrid()->iThisNodeNo());
  pJobCancelled->vJobKind(JK_GENERIC);
  
  TagSet* pJobCancelledTags = pJobCancelled->pJobTags();

  Word* pwCmdCancelledTags = AM::WORD::pFind("cancelled_cmdTags"_Stz);
  TagSet* pCmdCancelledTags = pwCmdCancelledTags->pWrdTagSet();
  pJobCancelledTags->vCopy(pCmdCancelledTags);


  
  pJobCancelled->pJobTags()->vInsert(C_TAG_JOBNM,"cancelled"_Stz);
  //pJobCancelled->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobCancelled->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobCancelled->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeNo());
  //pJobCancelled->pJobTags()->vInsert(C_TAG_SEEDRUN,"cancelled::cancelled"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobCancelled->pJobTags()->vInsert(C_TAG_FLOW,1);
  
  m_pDataStack->vPushPtr(pJobCancelled);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_end::new - new a job_end for this distributed 2pc job                */
/*----------------------------------------------------------------------------*/
void Step::vJob_EndNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Job* pJobEnd = new Job("end"_Stz,
                         pDistJob->m_i_Origin,
                         pDistJob->m_i_Session,
                         pDistJob->m_iCmdNo
                         );
  
  pJobEnd->m_tJobId = pDistJob->tJobId();
  pJobEnd->vParticipantNode(AM::pGrid()->iThisNodeNo());
  pJobEnd->vJobKind(JK_GENERIC);
    
  TagSet* pJobEndTags = pJobEnd->pJobTags();

  Word* pwCmdEndTags = AM::WORD::pFind("end_cmdTags"_Stz);
  TagSet* pCmdEndTags = pwCmdEndTags->pWrdTagSet();
  pJobEndTags->vCopy(pCmdEndTags);


  pJobEnd->pJobTags()->vInsert(C_TAG_JOBNM,"end"_Stz);
  //pJobEnd->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobEnd->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobEnd->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeNo());
  //pJobEnd->pJobTags()->vInsert(C_TAG_SEEDRUN,"end::end"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobEnd->pJobTags()->vInsert(C_TAG_FLOW,1);
  
  m_pDataStack->vPushPtr(pJobEnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_ended::new -- new a job_ended for this distributed 2pc job           */
/*----------------------------------------------------------------------------*/
void Step::vJob_EndedNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Job* pJobEnded = new Job("ended"_Stz,
                           pDistJob->m_i_Origin,
                           pDistJob->m_i_Session,
                           pDistJob->m_iCmdNo
                           );
  
  pJobEnded->m_tJobId = pDistJob->tJobId();
  pJobEnded->vParticipantNode(AM::pGrid()->iThisNodeNo());
  pJobEnded->vJobKind(JK_GENERIC);
  
  
  TagSet* pJobEndedTags = pJobEnded->pJobTags();

  Word* pwCmdEndedTags = AM::WORD::pFind("ended_cmdTags"_Stz);
  TagSet* pCmdEndedTags = pwCmdEndedTags->pWrdTagSet();
  pJobEndedTags->vCopy(pCmdEndedTags);


  pJobEnded->pJobTags()->vInsert(C_TAG_JOBNM,"ended"_Stz);
  //pJobEnded->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobEnded->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobEnded->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeNo());
  //pJobEnded->pJobTags()->vInsert(C_TAG_SEEDRUN,"ended::ended"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobEnded->pJobTags()->vInsert(C_TAG_FLOW,1);
    
  m_pDataStack->vPushPtr(pJobEnded);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_execute::new -- new a job_execute for this distributed 2pc job       */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExecuteNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Job* pJobExecute = new Job("execute"_Stz,
                             pDistJob->m_i_Origin,
                             pDistJob->m_i_Session,
                             pDistJob->m_iCmdNo
                             );
  
  pJobExecute->m_tJobId = pDistJob->tJobId();
  pJobExecute->vParticipantNode(AM::pGrid()->iThisNodeNo());
  pJobExecute->vJobKind(JK_GENERIC);
  
  
  TagSet* pJobExecuteTags = pJobExecute->pJobTags();

  Word* pwCmdExecuteTags = AM::WORD::pFind("execute_cmdTags"_Stz);
  TagSet* pCmdExecuteTags = pwCmdExecuteTags->pWrdTagSet();
  pJobExecuteTags->vCopy(pCmdExecuteTags);


  pJobExecute->pJobTags()->vInsert(C_TAG_JOBNM,"execute"_Stz);
  //pJobExecute->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobExecute->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobExecute->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeNo());
  //pJobExecute->pJobTags()->vInsert(C_TAG_SEEDRUN,"execute::execute"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobExecute->pJobTags()->vInsert(C_TAG_FLOW,1);
  
  m_pDataStack->vPushPtr(pJobExecute);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_executed::new -- new a job_executed for this distributed 2pc job     */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExecutedNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Job* pJobExecuted = new Job("executed"_Stz,
                              pDistJob->m_i_Origin,
                              pDistJob->m_i_Session,
                              pDistJob->m_iCmdNo
                              );
  
  pJobExecuted->m_tJobId = pDistJob->tJobId();
  pJobExecuted->vParticipantNode(AM::pGrid()->iThisNodeNo());
  pJobExecuted->vJobKind(JK_GENERIC);
  
  TagSet* pJobExecutedTags = pJobExecuted->pJobTags();

  Word* pwCmdExecutedTags = AM::WORD::pFind("executed_cmdTags"_Stz);
  TagSet* pCmdExecutedTags = pwCmdExecutedTags->pWrdTagSet();
  pJobExecutedTags->vCopy(pCmdExecutedTags);


  pJobExecuted->pJobTags()->vInsert(C_TAG_JOBNM,"executed"_Stz);
  //pJobExecuted->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobExecuted->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobExecuted->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeNo());
  //pJobExecuted->pJobTags()->vInsert(C_TAG_SEEDRUN,"executed::executed"_Stz);
  
  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobExecuted->pJobTags()->vInsert(C_TAG_FLOW,1);
  
  m_pDataStack->vPushPtr(pJobExecuted);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
