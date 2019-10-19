/*============================================================================*/
/* Steps_JobReceiver.cpp - job_receiver step group                            */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobReceiver(void)
{
  vAddStep("job_receiver::jobReceived"_Stz, &Step::vJob_ReceiverJobReceived);
  
  vAddStep("job_receiver.connNo@"_Stz,     &Step::vJob_ReceiverConnNoFetch);
  vAddStep("job_receiver.connNo!"_Stz,     &Step::vJob_ReceiverConnNoStore);
  vAddStep("job_receiver.node@"_Stz,       &Step::vJob_ReceiverNodeFetch);
  vAddStep("job_receiver.node!"_Stz,       &Step::vJob_ReceiverNodeStore);
}

/*----------------------------------------------------------------------------*/
/* job_receiver.connNo@ -- fetch connNo from job_receiver                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_ReceiverConnNoFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_ReceiverConnNoFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJobPtr);
  
  i8 iConnNo = pJob_Receiver->iConnNo();
  mpTask->m_pForth->m_pDataStack->vPushI8(iConnNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_receiver.jobReceived -- job has been recieved                          */
/*----------------------------------------------------------------------------*/
void Step::vJob_ReceiverJobReceived(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_ReceiverJobReceived"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job* pJob = _pJob(pJobPtr);

  if (CN_JOB__EXECUTED == pJob->eClass()) {
    Job__Executed* pJob__Executed = _pJob__Executed(pJob);
    Stz* szResponse = pJob__Executed->szResponse();
    if (szResponse->iLength())
      AM::vLogSvc(SL_INF,mpTask->m_pJob,VA_SVC_RESPONSE_JOB,
                  "%s >>%s<<"_Stz, //!format-LogSvc
                  aCN[pJob->eClass()],
                  szResponse);
    EXIT();
    return;
  }

  AM::vLogSvc(SL_INF,mpTask->m_pJob,VA_SVC_RECEIVED_JOB,
              "%s"_Stz, //!format-LogSvc
              aCN[pJob->eClass()]);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_receiver.connNo! -- store connNo into job_receiver                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_ReceiverConnNoStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_ReceiverConnNoStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJobPtr);
  
  i8 iConnNo;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iConnNo) )
    THROW_DSTACK();
  
  pJob_Receiver->vConnNo(iConnNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_receiver.node@ -- fetch node from job_receiver                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_ReceiverNodeFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_ReceiverNodeFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJobPtr);
  
  Node* pNode = pJob_Receiver->pNode();
  mpTask->m_pForth->m_pDataStack->vPushPointer(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_receiver.node! -- store node into job_receiver                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_ReceiverNodeStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_ReceiverNodeStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJobPtr);
  
  Object* pNodePtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pNodePtr) )
    THROW_DSTACK();
  Node* pNode = _pNode(pNodePtr);
  
  pJob_Receiver->vNode(pNode);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
