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
void Step::vJob_ReceiverConnNoFetch(void)
{
  ENTRY(); // ENTRY("vJob_ReceiverConnNoFetch"_Stz);
  
  Object* pJobPtr; m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJobPtr);
  
  i8 iConnNo = pJob_Receiver->iConnNo();
  m_pDataStack->vPushInt(iConnNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_receiver.jobReceived -- job has been recieved                          */
/*----------------------------------------------------------------------------*/
void Step::vJob_ReceiverJobReceived(void)
{
  ENTRY(); // ENTRY("vJob_ReceiverJobReceived"_Stz);
  
  Object* pJobPtr; m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job* pJob = _pJob(pJobPtr);

//  if (CN_JOB__EXECUTED == pJob->eClass()) {
//    Job__Executed* pJob__Executed = _pJob__Executed(pJob);
//    Stz* szResponse = pJob__Executed->szResponse();
//    if (szResponse->iLength())
//      AM::vLogSvc(SL_INF,m_pTask->m_pJob,VA_SVC_RESPONSE_JOB,
//                  "%s >>%s<<"_Stz, //!format-LogSvc
//                  aCN[pJob->eClass()],
//                  szResponse);
//    EXIT();
//    return;
//  }

  AM::vLogSvc(SL_INF,m_pTask->m_pJob,VA_SVC_RECEIVED_JOB,
              "%s"_Stz, //!format-LogSvc
              aCN[pJob->eClass()]);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_receiver.connNo! -- store connNo into job_receiver                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_ReceiverConnNoStore(void)
{
  ENTRY(); // ENTRY("vJob_ReceiverConnNoStore"_Stz);
  
  Object* pJobPtr; m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJobPtr);
  
  i8 iConnNo = m_pDataStack->iPopInt(__func__);
  
  pJob_Receiver->vConnNo(iConnNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_receiver.node@ -- fetch node from job_receiver                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_ReceiverNodeFetch(void)
{
  ENTRY(); // ENTRY("vJob_ReceiverNodeFetch"_Stz);
  
  Object* pJobPtr; m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJobPtr);
  
  Node* pNode = pJob_Receiver->pNode();
  m_pDataStack->vPushPtr(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_receiver.node! -- store node into job_receiver                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_ReceiverNodeStore(void)
{
  ENTRY(); // ENTRY("vJob_ReceiverNodeStore"_Stz);
  
  Object* pJobPtr; m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJobPtr);
  
  Object* pNodePtr; m_pDataStack->vPopPtr(pNodePtr,__func__);
  Node* pNode = _pNode(pNodePtr);
  
  pJob_Receiver->vNode(pNode);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
