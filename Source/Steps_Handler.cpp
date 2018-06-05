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
  vAddPrimitive("Handler::systemEvent"_Stz, &vHandlerSystemEvent, false);
}

/*----------------------------------------------------------------------------*/
/* Handler::systemEvent::list -- call system level event handler              */
/*----------------------------------------------------------------------------*/
void Task::vHandlerSystemEvent(MP_Task mpTask)
{
  ENTRY("vHandlerSystemEvent"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.ack"_Stz);
  Task* pThatTask = _pTask(pTaskObject);

  // correct this task to R35 model
  
  pThatTask->vR35model(true);
  pThatTask->vState(TS_READY);
  
  Handlers::bSystemLevelHandler(pThatTask);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
