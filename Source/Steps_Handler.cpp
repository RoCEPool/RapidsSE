/*============================================================================*/
/* Steps_Handler.cpp - event handler step group                               */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap*  gpAttachMap;

// Handler: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Handler(void)
{
  vAddPrimitive("Handler::spawnedTask"_Stz, &vHandlerSpawnedTask, false);
}

/*----------------------------------------------------------------------------*/
/* Handler::spawnedTask -- call spawned task handler                          */
/*----------------------------------------------------------------------------*/
void Task::vHandlerSpawnedTask(MP_Task mpTask)
{
  ENTRY("vHandlerSystemEvent"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.ack"_Stz);
  Task* pThatTask = _pTask(pTaskObject);

  // correct this task to R35 model
  
  pThatTask->vR35model(true);
  pThatTask->vState(TS_READY);
  
  Handlers::bSpawnedTaskHandler(pThatTask);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
