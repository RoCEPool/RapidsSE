/*============================================================================*/
/* Steps_JobReceiver.cpp - job_receiver step group                            */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap* gpAttachMap;

// job: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobReceiver(void)
{
  vAddStep("job_receiver.connNo@"_Stz, &Task::vJob_ReceiverConnNoFetch);
  vAddStep("job_receiver.connNo!"_Stz, &Task::vJob_ReceiverConnNoStore);
  vAddStep("job_receiver.socket@"_Stz, &Task::vJob_ReceiverSocketFetch);
  vAddStep("job_receiver.socket!"_Stz, &Task::vJob_ReceiverSocketStore);

}

/*----------------------------------------------------------------------------*/
/* job_receiver.connNo@ -- fetch connNo from job_receiver                     */
/*----------------------------------------------------------------------------*/
void Task::vJob_ReceiverConnNoFetch(MP_Task mpTask)
{
  ENTRY("vJob_ReceiverConnNoFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_Receiver.connNo@"_Stz);
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJobPtr);
  
  i8 iConnNo = pJob_Receiver->iConnNo();
  mpTask->m_pDataStack->vPushInt(iConnNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_receiver.connNo! -- store connNo into job_receiver                     */
/*----------------------------------------------------------------------------*/
void Task::vJob_ReceiverConnNoStore(MP_Task mpTask)
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
/* job_receiver.socket@ -- fetch socket from job_receiver                     */
/*----------------------------------------------------------------------------*/
void Task::vJob_ReceiverSocketFetch(MP_Task mpTask)
{
  ENTRY("vJob_ReceiverSocketFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_Receiver.socket@"_Stz);
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJobPtr);
  
  Socket* pSocket = pJob_Receiver->pSocket();
  mpTask->m_pDataStack->vPushObject(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_receiver.socket! -- store socket into job_receiver                     */
/*----------------------------------------------------------------------------*/
void Task::vJob_ReceiverSocketStore(MP_Task mpTask)
{
  ENTRY("vJob_ReceiverSocketStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_receiver.socket!"_Stz);
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJobPtr);
  
  Object* pSocketPtr;
  if ( ! mpTask->m_pDataStack->bPop(pSocketPtr) )
    THROW_INVALID_DSTACK("job_receiver.socket!"_Stz);
  Socket* pSocket = _pSocket(pSocketPtr);
  
  pJob_Receiver->vSocket(pSocket);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
