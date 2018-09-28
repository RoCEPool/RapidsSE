/*============================================================================*/
/* Steps_job.cpp - job step group                                             */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap* gpAttachMap;

// job: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_JobReceiver(void)
{
  vAddPrimitive("job_receiver.connNo@"_Stz, &vJobReceiverConnNoFetch,   false);
  vAddPrimitive("job_receiver.connNo!"_Stz, &vJobReceiverConnNoStore,   false);
  vAddPrimitive("job_receiver.socket@"_Stz, &vJobReceiverSocketFetch,   false);
  vAddPrimitive("job_receiver.socket!"_Stz, &vJobReceiverSocketStore,   false);

}

/*----------------------------------------------------------------------------*/
/* job_receiver.connNo@ -- fetch connNo from job_receiver                     */
/*----------------------------------------------------------------------------*/
void Task::vJobReceiverConnNoFetch(MP_Task mpTask)
{
  ENTRY("vJobReceiverConnNoFetch"_Stz);
  
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
void Task::vJobReceiverConnNoStore(MP_Task mpTask)
{
  ENTRY("vJobReceiverConnNoStore"_Stz);
  
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
void Task::vJobReceiverSocketFetch(MP_Task mpTask)
{
  ENTRY("vJobReceiverSocketFetch"_Stz);
  
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
void Task::vJobReceiverSocketStore(MP_Task mpTask)
{
  ENTRY("vJobReceiverFetchStore"_Stz);
  
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
