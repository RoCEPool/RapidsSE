/*============================================================================*/
/* Steps_JobReceiver.cpp - job_receiver step group                            */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobReceiver(void)
{
  vAddStep("job_receiver.connNo@"_Stz, &Step::vJob_ReceiverConnNoFetch);
  vAddStep("job_receiver.connNo!"_Stz, &Step::vJob_ReceiverConnNoStore);
  vAddStep("job_receiver.node@"_Stz,   &Step::vJob_ReceiverNodeFetch);
  vAddStep("job_receiver.node!"_Stz,   &Step::vJob_ReceiverNodeStore);
}

/*----------------------------------------------------------------------------*/
/* job_receiver.connNo@ -- fetch connNo from job_receiver                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_ReceiverConnNoFetch(MP_Task mpTask)
{
  ENTRY("vJob_ReceiverConnNoFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_Receiver.connNo@"_Stz);
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJobPtr);
  
  i8 iConnNo = pJob_Receiver->iConnNo();
  mpTask->m_pDataStack->vPushI8(iConnNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_receiver.connNo! -- store connNo into job_receiver                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_ReceiverConnNoStore(MP_Task mpTask)
{
  ENTRY("vJob_ReceiverConnNoStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_receiver.connNo!"_Stz);
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJobPtr);

  i8 iConnNo;
  if ( ! mpTask->m_pDataStack->bPop(iConnNo) )
    THROW_INVALID_DSTACK("job_receiver.connNo!"_Stz);
  
  pJob_Receiver->vConnNo(iConnNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_receiver.node@ -- fetch node from job_receiver                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_ReceiverNodeFetch(MP_Task mpTask)
{
  ENTRY("vJob_ReceiverNodeFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_Receiver.node@"_Stz);
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJobPtr);
  
  Node* pNode = pJob_Receiver->pNode();
  mpTask->m_pDataStack->vPushObject(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_receiver.node! -- store node into job_receiver                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_ReceiverNodeStore(MP_Task mpTask)
{
  ENTRY("vJob_ReceiverNodeStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_receiver.node!"_Stz);
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJobPtr);
  
  Object* pNodePtr;
  if ( ! mpTask->m_pDataStack->bPop(pNodePtr) )
    THROW_INVALID_DSTACK("job_receiver.node!"_Stz);
  Node* pNode = _pNode(pNodePtr);
  
  pJob_Receiver->vNode(pNode);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
