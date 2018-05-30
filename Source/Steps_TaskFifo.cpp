/*============================================================================*/
/* Steps_TaskFifo.cpp - task fifo step group                                  */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// TaskFifo: ///////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TaskFifo(void)
{
  vAddPrimitive("TaskFifo.append"_Stz,   &vTaskFifoAppend,    false);
  vAddPrimitive("TaskFifo.dump"_Stz,     &vTaskFifoDump,      false);
  vAddPrimitive("TaskFifo.init"_Stz,     &vTaskFifoInit,      false);
  vAddPrimitive("TaskFifo::new"_Stz,     &vTaskFifoNew,       false);
}

/*----------------------------------------------------------------------------*/
/* TaskFifo.append -- TaskQueue append                                       */
/*----------------------------------------------------------------------------*/
void Task::vTaskFifoAppend(MP_Task mpTask)
{
ENTRY("vTaskFifoAppend"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("TaskFifo.append"_Stz);
  
  ASSERT(CN_TASKFIFO == pObject->eClass());
  
  TaskFifo* pTaskFifo = static_cast<TaskFifo*>(pObject);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("TaskFifo.append"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  pTaskFifo->vAppend(pThatTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TaskFifo.dump -- TaskQueue dump                                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskFifoDump(MP_Task mpTask)
{
ENTRY("vTaskFifoDump"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("TaskFifo.dump"_Stz);
  
  ASSERT(CN_TASKFIFO == pObject->eClass());
  
  TaskFifo* pTaskFifo = static_cast<TaskFifo*>(pObject);
  
  pTaskFifo->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TaskFifo.init -- TaskFifo init                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskFifoInit(MP_Task mpTask)
{
ENTRY("vTaskFifoInit"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("TaskFifo.init"_Stz);
  
  ASSERT(CN_TASKFIFO == pObject->eClass());
  
  TaskFifo* pTaskFifo = static_cast<TaskFifo*>(pObject);
  
  new(pTaskFifo) TaskFifo;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TaskFifo::new -- create new TaskQueue                                      */
/*----------------------------------------------------------------------------*/
void Task::vTaskFifoNew(MP_Task mpTask)
{
  ENTRY("vTaskFifoNew"_Stz);

  TaskFifo* pTaskFifo = new TaskFifo;
  
  mpTask->m_pDataStack->vPushObject(pTaskFifo);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
