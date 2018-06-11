/*============================================================================*/
/* Steps_Queues.cpp - queues step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// seitf: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Queues(void)
{
  vAddPrimitive("queues::QueuesExecutingLength"_Stz, &vQueuesExecutingLength, false);
  vAddPrimitive("queues::QueuesReadyLength"_Stz,     &vQueuesReadyLength,     false);
  vAddPrimitive("queues::QueuesReadySlotLength"_Stz, &vQueuesReadySlotLength, false);
  vAddPrimitive("queues::QueuesSleepingLength"_Stz,  &vQueuesSleepingLength,  false);
  vAddPrimitive("queues::QueuesWaitingLength"_Stz,   &vQueuesWaitingLength,   false);
}

/*----------------------------------------------------------------------------*/
/* queues::QueuesExecutingLength -- get the executing queue length            */
/*----------------------------------------------------------------------------*/
void Task::vQueuesExecutingLength(MP_Task mpTask)
{
  ENTRY("vQueuesExecutingLength"_Stz);
  
  i8 iSlot;
  
  if ( !  mpTask->m_pDataStack->bPop(iSlot) )
    THROW_INVALID_DSTACK("queues::QueuesExecutingLength"_Stz);
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  mpTask->m_pDataStack->vPushInt(pQueues->nExecutingQueueTasks());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queues::QueuesReadyLength -- get the ready queue length                    */
/*----------------------------------------------------------------------------*/
void Task::vQueuesReadyLength(MP_Task mpTask)
{
  ENTRY("vQueuesReadyLength"_Stz);
  
  i8 iSlot;
  
  if ( !  mpTask->m_pDataStack->bPop(iSlot) )
    THROW_INVALID_DSTACK("queues::QueuesReadyLength"_Stz);
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  mpTask->m_pDataStack->vPushInt(pQueues->nReadyQueueTasks());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queues::QueuesReadySlotLength -- get the ready slot queue length           */
/*----------------------------------------------------------------------------*/
void Task::vQueuesReadySlotLength(MP_Task mpTask)
{
  ENTRY("vQueuesReadySlotLength"_Stz);
  
  i8 iSlot;
  
  if ( !  mpTask->m_pDataStack->bPop(iSlot) )
    THROW_INVALID_DSTACK("queues::QueuesReadySlotLength"_Stz);
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  mpTask->m_pDataStack->vPushInt(pQueues->nReadySlotQueueTasks(iSlot));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queues::QueuesSleepingLength -- get the sleeping queue length              */
/*----------------------------------------------------------------------------*/
void Task::vQueuesSleepingLength(MP_Task mpTask)
{
  ENTRY("vQueuesSleepingLength"_Stz);
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  mpTask->m_pDataStack->vPushInt(pQueues->nSleepingQueueTasks());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queues::QueuesWaitingLength -- get the sleeping queue length              */
/*----------------------------------------------------------------------------*/
void Task::vQueuesWaitingLength(MP_Task mpTask)
{
  ENTRY("vQueuesWaitingLength"_Stz);
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  mpTask->m_pDataStack->vPushInt(pQueues->nQWaitingQueueTasks());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
