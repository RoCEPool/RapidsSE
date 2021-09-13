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
  vAddStep("job__Begun::new"_Stz,        &Step::vJob__BegunNew);
  vAddStep("job__Cancel::new"_Stz,       &Step::vJob__CancelNew);
  vAddStep("job__Cancelled::new"_Stz,    &Step::vJob__CancelledNew);
  vAddStep("job__End::new"_Stz,          &Step::vJob__EndNew);
  vAddStep("job__Ended::new"_Stz,        &Step::vJob__EndedNew);
  vAddStep("job__Execute::new"_Stz,      &Step::vJob__ExecuteNew);
  vAddStep("job__Executed::new"_Stz,     &Step::vJob__ExecutedNew);
}

/*----------------------------------------------------------------------------*/
/* job_begun::new -- new a job__begun for this distributed 2pc job            */
/*----------------------------------------------------------------------------*/
void Step::vJob__BegunNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));

  Job* pJobBegun = new Job(pDistJob->m_i_Origin,
                           pDistJob->m_i_Session,
                           pDistJob->m_iCmdNo
                           );
  
  pJobBegun->m_tJobId.vAsI8(pDistJob->iJobId());
  pJobBegun->vParticipantNode(AM::pGrid()->iThisNodeNo());
  pJobBegun->vJobKind(JK_GENERIC);

  pJobBegun->vJobNm("_begun"_Stz);
  
  TagSet* pJobBegunTags = pJobBegun->pJobTags();
  
  Word* pwCmdBegunTags = AM::WORD::pFind("_Begun_cmdTags"_Stz);
  TagSet* pCmdBegunTags = pwCmdBegunTags->pWrdTagSet();
  pJobBegunTags->vCopy(pCmdBegunTags);
  
  pJobBegunTags->vInsert(C_TAG_JOBNM,"_begun"_Stz);
  //pJobBegunTags->vInsert(C_TAG_PATH,"/"_Stz);
  pJobBegunTags->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobBegunTags->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeNo());
  //pJobBegunTags->vInsert(C_TAG_SEEDRUN,"_begun_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobBegun->pJobTags()->vInsert(C_TAG_FLOW,1);

  m_pDataStack->vPushPtr(pJobBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__cancel::new -- new a job__cancel for this distributed 2pc job         */
/*----------------------------------------------------------------------------*/
void Step::vJob__CancelNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Job* pJobCancel = new Job(pDistJob->m_i_Origin,
                            pDistJob->m_i_Session,
                            pDistJob->m_iCmdNo
                            );
  
  pJobCancel->m_tJobId.vAsI8(pDistJob->iJobId());
  pJobCancel->vParticipantNode(AM::pGrid()->iThisNodeNo());
  pJobCancel->vJobKind(JK_GENERIC);

  TagSet* pJobCancelTags = pJobCancel->pJobTags();

  Word* pwCmdCancelTags = AM::WORD::pFind("_Cancel_cmdTags"_Stz);
  TagSet* pCmdCancelTags = pwCmdCancelTags->pWrdTagSet();
  pJobCancelTags->vCopy(pCmdCancelTags);

  pJobCancel->vJobNm("_cancel"_Stz);
  pJobCancel->pJobTags()->vInsert(C_TAG_JOBNM,"_cancel"_Stz);
  //pJobCancel->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobCancel->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobCancel->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeNo());
  //pJobCancel->pJobTags()->vInsert(C_TAG_SEEDRUN,"_cancel_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobCancel->pJobTags()->vInsert(C_TAG_FLOW,1);
    
  m_pDataStack->vPushPtr(pJobCancel);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__cancelled::new -- new a job__cancelled for this distributed 2pc job   */
/*----------------------------------------------------------------------------*/
void Step::vJob__CancelledNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));

  Job* pJobCancelled = new Job(pDistJob->m_i_Origin,
                               pDistJob->m_i_Session,
                               pDistJob->m_iCmdNo
                               );
  
  pJobCancelled->m_tJobId.vAsI8(pDistJob->iJobId());
  pJobCancelled->vParticipantNode(AM::pGrid()->iThisNodeNo());
  pJobCancelled->vJobKind(JK_GENERIC);
  
  TagSet* pJobCancelledTags = pJobCancelled->pJobTags();

  Word* pwCmdCancelledTags = AM::WORD::pFind("_Cancelled_cmdTags"_Stz);
  TagSet* pCmdCancelledTags = pwCmdCancelledTags->pWrdTagSet();
  pJobCancelledTags->vCopy(pCmdCancelledTags);


  
  pJobCancelled->vJobNm("_cancelled"_Stz);
  pJobCancelled->pJobTags()->vInsert(C_TAG_JOBNM,"_cancelled"_Stz);
  //pJobCancelled->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobCancelled->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobCancelled->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeNo());
  //pJobCancelled->pJobTags()->vInsert(C_TAG_SEEDRUN,"_cancelled_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobCancelled->pJobTags()->vInsert(C_TAG_FLOW,1);
  
  m_pDataStack->vPushPtr(pJobCancelled);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__end::new - new a job__end for this distributed 2pc job                */
/*----------------------------------------------------------------------------*/
void Step::vJob__EndNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Job* pJobEnd = new Job(pDistJob->m_i_Origin,
                         pDistJob->m_i_Session,
                         pDistJob->m_iCmdNo
                         );
  
  pJobEnd->m_tJobId.vAsI8(pDistJob->iJobId());
  pJobEnd->vParticipantNode(AM::pGrid()->iThisNodeNo());
  pJobEnd->vJobKind(JK_GENERIC);
    
  TagSet* pJobEndTags = pJobEnd->pJobTags();

  Word* pwCmdEndTags = AM::WORD::pFind("_End_cmdTags"_Stz);
  TagSet* pCmdEndTags = pwCmdEndTags->pWrdTagSet();
  pJobEndTags->vCopy(pCmdEndTags);


  pJobEnd->vJobNm("_end"_Stz);
  pJobEnd->pJobTags()->vInsert(C_TAG_JOBNM,"_end"_Stz);
  //pJobEnd->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobEnd->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobEnd->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeNo());
  //pJobEnd->pJobTags()->vInsert(C_TAG_SEEDRUN,"_end_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobEnd->pJobTags()->vInsert(C_TAG_FLOW,1);
  
  m_pDataStack->vPushPtr(pJobEnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__ended::new -- new a job__ended for this distributed 2pc job           */
/*----------------------------------------------------------------------------*/
void Step::vJob__EndedNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Job* pJobEnded = new Job(pDistJob->m_i_Origin,
                           pDistJob->m_i_Session,
                           pDistJob->m_iCmdNo
                           );
  
  pJobEnded->m_tJobId.vAsI8(pDistJob->iJobId());;
  pJobEnded->vParticipantNode(AM::pGrid()->iThisNodeNo());
  pJobEnded->vJobKind(JK_GENERIC);
  
  
  TagSet* pJobEndedTags = pJobEnded->pJobTags();

  Word* pwCmdEndedTags = AM::WORD::pFind("_Ended_cmdTags"_Stz);
  TagSet* pCmdEndedTags = pwCmdEndedTags->pWrdTagSet();
  pJobEndedTags->vCopy(pCmdEndedTags);


  
  pJobEnded->vJobNm("_ended"_Stz);
  pJobEnded->pJobTags()->vInsert(C_TAG_JOBNM,"_ended"_Stz);
  //pJobEnded->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobEnded->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobEnded->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeNo());
  //pJobEnded->pJobTags()->vInsert(C_TAG_SEEDRUN,"_ended_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobEnded->pJobTags()->vInsert(C_TAG_FLOW,1);
    
  m_pDataStack->vPushPtr(pJobEnded);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__execute::new -- new a job__execute for this distributed 2pc job       */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecuteNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Job* pJobExecute = new Job(pDistJob->m_i_Origin,
                             pDistJob->m_i_Session,
                             pDistJob->m_iCmdNo
                             );
  pJobExecute->m_tJobId.vAsI8(pDistJob->iJobId());
  pJobExecute->vParticipantNode(AM::pGrid()->iThisNodeNo());
  pJobExecute->vJobKind(JK_GENERIC);
  
  
  TagSet* pJobExecuteTags = pJobExecute->pJobTags();

  Word* pwCmdExecuteTags = AM::WORD::pFind("_Execute_cmdTags"_Stz);
  TagSet* pCmdExecuteTags = pwCmdExecuteTags->pWrdTagSet();
  pJobExecuteTags->vCopy(pCmdExecuteTags);


    
  pJobExecute->vJobNm("_execute"_Stz);
  pJobExecute->pJobTags()->vInsert(C_TAG_JOBNM,"_execute"_Stz);
  //pJobExecute->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobExecute->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobExecute->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeNo());
  //pJobExecute->pJobTags()->vInsert(C_TAG_SEEDRUN,"_execute_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobExecute->pJobTags()->vInsert(C_TAG_FLOW,1);
  
  m_pDataStack->vPushPtr(pJobExecute);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__executed::new -- new a job__executed for this distributed 2pc job     */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecutedNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Job* pJobExecuted = new Job(pDistJob->m_i_Origin,
                              pDistJob->m_i_Session,
                              pDistJob->m_iCmdNo
                              );
  pJobExecuted->m_tJobId.vAsI8(pDistJob->iJobId());
  pJobExecuted->vParticipantNode(AM::pGrid()->iThisNodeNo());
  pJobExecuted->vJobKind(JK_GENERIC);
  
  
  
  TagSet* pJobExecutedTags = pJobExecuted->pJobTags();

  Word* pwCmdExecutedTags = AM::WORD::pFind("_Executed_cmdTags"_Stz);
  TagSet* pCmdExecutedTags = pwCmdExecutedTags->pWrdTagSet();
  pJobExecutedTags->vCopy(pCmdExecutedTags);


  
  pJobExecuted->vJobNm("_executed"_Stz);
  pJobExecuted->pJobTags()->vInsert(C_TAG_JOBNM,"_executed"_Stz);
  //pJobExecuted->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobExecuted->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobExecuted->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeNo());
  //pJobExecuted->pJobTags()->vInsert(C_TAG_SEEDRUN,"_executed_run"_Stz);
  
  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobExecuted->pJobTags()->vInsert(C_TAG_FLOW,1);
  
  m_pDataStack->vPushPtr(pJobExecuted);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
