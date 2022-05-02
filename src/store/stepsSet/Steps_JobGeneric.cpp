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

#include "Api.hpp"

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
/* job_begun::new -- new a job_begun for this distributed 2pc job             */
/*----------------------------------------------------------------------------*/
void Step::vJob_BegunNew(void)
{
  ENTRY();
  
  //Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  //Job* pJobBegun = new Job("begun"_Stz,pDistJob->tJobId());
  
  i8 iDistJobId = m_pDataStack->iPopInt(__func__);
  JobId tDistJobId;
  tDistJobId = iDistJobId;
  Job* pJobBegun = Api::pNewGenericJob("begun"_Stz,tDistJobId);

  //pJobBegun->vParticipantNodeId(AM::pGrid()->iThisNodeId());
  //pJobBegun->vJobKind(JK_GENERIC);
  
  //TagSet* pJobBegunTags = pJobBegun->pJobTags();
  
  //Word* pwCmdBegunTags = AM::WORD::pFind("begun_cmdTags"_Stz);
  //TagSet* pCmdBegunTags = pwCmdBegunTags->pWrdTagSet();
  //pJobBegunTags->vCopy(pCmdBegunTags);
  
  //pJobBegunTags->vInsert(C_TAG_JOBNM,"begun"_Stz);
  //pJobBegunTags->vInsert(C_TAG_DISTID,tDistJobId.iAsI8());
  //pJobBegunTags->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeId());

  //if (pDistJob->bHasTag(C_TAG_FLOW))
  //  pJobBegun->pJobTags()->vInsert(C_TAG_FLOW,1);

  TagSet* pJobTagSet = pJobBegun->pJobTags();
  RASSERT(pJobTagSet->szValue(C_TAG_ENTRYPOINT)->bSame("begun:begun"_Stz));
  
  pJobBegun->vEntryPoint("begun:begun"_Stz);
  
  m_pDataStack->vPushPtr(pJobBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_cancel::new -- new a job_cancel for this distributed 2pc job           */
/*----------------------------------------------------------------------------*/
void Step::vJob_CancelNew(void)
{
  ENTRY();
  
  //Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  //Job* pJobCancel = new Job("cancel"_Stz,pDistJob->tJobId());
  
  i8 iDistJobId = m_pDataStack->iPopInt(__func__);
  JobId tDistJobId;
  tDistJobId = iDistJobId;
  Job* pJobCancel = Api::pNewGenericJob("cancel"_Stz,tDistJobId);

  //pJobCancel->vParticipantNodeId(AM::pGrid()->iThisNodeId());
  //pJobCancel->vJobKind(JK_GENERIC);

  //TagSet* pJobCancelTags = pJobCancel->pJobTags();

  //Word* pwCmdCancelTags = AM::WORD::pFind("cancel_cmdTags"_Stz);
  //TagSet* pCmdCancelTags = pwCmdCancelTags->pWrdTagSet();
  //pJobCancelTags->vCopy(pCmdCancelTags);

  //pJobCancel->pJobTags()->vInsert(C_TAG_JOBNM,"cancel"_Stz);
  //pJobCancelTags->vInsert(C_TAG_DISTID,tDistJobId.iAsI8());
  //pJobCancelTags->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeId());

  //if (pDistJob->bHasTag(C_TAG_FLOW))
  //  pJobCancel->pJobTags()->vInsert(C_TAG_FLOW,1);
    
  TagSet* pJobTagSet = pJobCancel->pJobTags();
  RASSERT(pJobTagSet->szValue(C_TAG_ENTRYPOINT)->bSame("cancel:cancel"_Stz));
  
  pJobCancel->vEntryPoint("cancel:cancel"_Stz);
  
  m_pDataStack->vPushPtr(pJobCancel);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_cancelled::new -- new a job_cancelled for this distributed 2pc job     */
/*----------------------------------------------------------------------------*/
void Step::vJob_CancelledNew(void)
{
  ENTRY();
  
  //Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  //Job* pJobCancelled = new Job("cancelled"_Stz,pDistJob->tJobId());

  i8 iDistJobId = m_pDataStack->iPopInt(__func__);
  JobId tDistJobId;
  tDistJobId = iDistJobId;
  Job* pJobCancelled = Api::pNewGenericJob("cancelled"_Stz,tDistJobId);


  //pJobCancelled->vParticipantNodeId(AM::pGrid()->iThisNodeId());
  //pJobCancelled->vJobKind(JK_GENERIC);
  
  //TagSet* pJobCancelledTags = pJobCancelled->pJobTags();

  //Word* pwCmdCancelledTags = AM::WORD::pFind("cancelled_cmdTags"_Stz);
  //TagSet* pCmdCancelledTags = pwCmdCancelledTags->pWrdTagSet();
  //pJobCancelledTags->vCopy(pCmdCancelledTags);


  
  //pJobCancelled->pJobTags()->vInsert(C_TAG_JOBNM,"cancelled"_Stz);
  //pJobCancelledTags->vInsert(C_TAG_DISTID,tDistJobId.iAsI8());
  //pJobCancelledTags->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeId());

  //if (pDistJob->bHasTag(C_TAG_FLOW))
  //  pJobCancelled->pJobTags()->vInsert(C_TAG_FLOW,1);
  
  TagSet* pJobTagSet = pJobCancelled->pJobTags();
  RASSERT(pJobTagSet->szValue(C_TAG_ENTRYPOINT)->bSame("cancelled:cancelled"_Stz));
  
  pJobCancelled->vEntryPoint("cancelled:cancelled"_Stz);
  
  m_pDataStack->vPushPtr(pJobCancelled);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_end::new - new a job_end for this distributed 2pc job                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_EndNew(void)
{
  ENTRY();
  
  //Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  //Job* pJobEnd = new Job("end"_Stz,pDistJob->tJobId());
  
  i8 iDistJobId = m_pDataStack->iPopInt(__func__);
  JobId tDistJobId;
  tDistJobId = iDistJobId;
  Job* pJobEnd = Api::pNewGenericJob("end"_Stz,tDistJobId);

  //pJobEnd->vParticipantNodeId(AM::pGrid()->iThisNodeId());
  //pJobEnd->vJobKind(JK_GENERIC);
    
  //TagSet* pJobEndTags = pJobEnd->pJobTags();

  //Word* pwCmdEndTags = AM::WORD::pFind("end_cmdTags"_Stz);
  //TagSet* pCmdEndTags = pwCmdEndTags->pWrdTagSet();
  //pJobEndTags->vCopy(pCmdEndTags);


  //pJobEnd->pJobTags()->vInsert(C_TAG_JOBNM,"end"_Stz);
  //pJobEndTags->vInsert(C_TAG_DISTID,tDistJobId.iAsI8());
  //pJobEndTags->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeId());

  //if (pDistJob->bHasTag(C_TAG_FLOW))
  //  pJobEnd->pJobTags()->vInsert(C_TAG_FLOW,1);
  
  TagSet* pJobTagSet = pJobEnd->pJobTags();
  RASSERT(pJobTagSet->szValue(C_TAG_ENTRYPOINT)->bSame("end:end"_Stz));
  
  pJobEnd->vEntryPoint("end:end"_Stz);
  
  m_pDataStack->vPushPtr(pJobEnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_ended::new -- new a job_ended for this distributed 2pc job             */
/*----------------------------------------------------------------------------*/
void Step::vJob_EndedNew(void)
{
  ENTRY();
  
  //Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  //Job* pJobEnded = new Job("ended"_Stz,pDistJob->tJobId());
  
  i8 iDistJobId = m_pDataStack->iPopInt(__func__);
  JobId tDistJobId;
  tDistJobId = iDistJobId;
  Job* pJobEnded = Api::pNewGenericJob("ended"_Stz,tDistJobId);

  //pJobEnded->vParticipantNodeId(AM::pGrid()->iThisNodeId());
  //pJobEnded->vJobKind(JK_GENERIC);
  
  //TagSet* pJobEndedTags = pJobEnded->pJobTags();

  //Word* pwCmdEndedTags = AM::WORD::pFind("ended_cmdTags"_Stz);
  //TagSet* pCmdEndedTags = pwCmdEndedTags->pWrdTagSet();
  //pJobEndedTags->vCopy(pCmdEndedTags);


  //pJobEnded->pJobTags()->vInsert(C_TAG_JOBNM,"ended"_Stz);
  //pJobEndedTags->vInsert(C_TAG_DISTID,tDistJobId.iAsI8());
  //pJobEndedTags->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeId());

  //if (pDistJob->bHasTag(C_TAG_FLOW))
  //  pJobEnded->pJobTags()->vInsert(C_TAG_FLOW,1);
    
  TagSet* pJobTagSet = pJobEnded->pJobTags();
  RASSERT(pJobTagSet->szValue(C_TAG_ENTRYPOINT)->bSame("ended:ended"_Stz));
  
  pJobEnded->vEntryPoint("ended:ended"_Stz);
  
  m_pDataStack->vPushPtr(pJobEnded);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_execute::new -- new a job_execute for this distributed 2pc job         */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExecuteNew(void)
{
  ENTRY();
  
  //Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  //Job* pJobExecute = new Job("execute"_Stz,pDistJob->tJobId());
  
  i8 iDistJobId = m_pDataStack->iPopInt(__func__);
  JobId tDistJobId;
  tDistJobId = iDistJobId;
  Job* pJobExecute = Api::pNewGenericJob("execute"_Stz,tDistJobId);

  //pJobExecute->vParticipantNodeId(AM::pGrid()->iThisNodeId());
  //pJobExecute->vJobKind(JK_GENERIC);
  
  

  //Word* pwCmdExecuteTags = AM::WORD::pFind("execute_cmdTags"_Stz);
  //TagSet* pCmdExecuteTags = pwCmdExecuteTags->pWrdTagSet();
  //pJobExecuteTags->vCopy(pCmdExecuteTags);


  //pJobExecute->pJobTags()->vInsert(C_TAG_JOBNM,"execute"_Stz);
  //pJobExecuteTags->vInsert(C_TAG_DISTID,tDistJobId.iAsI8());
  //pJobExecuteTags->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeId());

  //if (pDistJob->bHasTag(C_TAG_FLOW))
  //  pJobExecute->pJobTags()->vInsert(C_TAG_FLOW,1);
  
  TagSet* pJobTagSet = pJobExecute->pJobTags();
  RASSERT(pJobTagSet->szValue(C_TAG_ENTRYPOINT)->bSame("execute:execute"_Stz));

  pJobExecute->vEntryPoint("execute:execute"_Stz);
  
  m_pDataStack->vPushPtr(pJobExecute);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_executed::new -- new a job_executed for this distributed 2pc job       */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExecutedNew(void)
{
  ENTRY();
  
  //Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  //Job* pJobExecuted = new Job("executed"_Stz,pDistJob->tJobId());
  
  i8 iDistJobId = m_pDataStack->iPopInt(__func__);
  JobId tDistJobId;
  tDistJobId = iDistJobId;
  Job* pJobExecuted = Api::pNewGenericJob("executed"_Stz,tDistJobId);

  //pJobExecuted->vParticipantNodeId(AM::pGrid()->iThisNodeId());
  //pJobExecuted->vJobKind(JK_GENERIC);
  
  //TagSet* pJobExecutedTags = pJobExecuted->pJobTags();

  //Word* pwCmdExecutedTags = AM::WORD::pFind("executed_cmdTags"_Stz);
  //TagSet* pCmdExecutedTags = pwCmdExecutedTags->pWrdTagSet();
  //pJobExecutedTags->vCopy(pCmdExecutedTags);


  //pJobExecuted->pJobTags()->vInsert(C_TAG_JOBNM,"executed"_Stz);
  //pJobExecutedTags->vInsert(C_TAG_DISTID,tDistJobId.iAsI8());
  //pJobExecutedTags->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNodeId());
  
  //if (pDistJob->bHasTag(C_TAG_FLOW))
  //  pJobExecuted->pJobTags()->vInsert(C_TAG_FLOW,1);
  
  TagSet* pJobTagSet = pJobExecuted->pJobTags();
  RASSERT(pJobTagSet->szValue(C_TAG_ENTRYPOINT)->bSame("executed:executed"_Stz));
  
  pJobExecuted->vEntryPoint("executed:executed"_Stz);
  
  m_pDataStack->vPushPtr(pJobExecuted);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
