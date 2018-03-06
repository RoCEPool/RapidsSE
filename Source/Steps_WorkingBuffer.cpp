/*============================================================================*/
/* Steps_WorkingBuffer.cpp - working buffer step group                        */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern BufferPool*     gpBufferPool;
extern AttachMap*      gpAttachMap;

// indexbuffer: ////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_WorkingBuffer(void)
{
  vAddPrimitive("workingBuffer.isLast@"_Stz,         &vWorkingBufferIsLastFetch, false);
  vAddPrimitive("workingBuffer.isLast!"_Stz,         &vWorkingBufferIsLastStore, false);
  vAddPrimitive("workingBuffer::toWorkingBuffer"_Stz, &vWorkingBufferToWorkingBuffer,false);
}

/*----------------------------------------------------------------------------*/
/* workingBuffer.isLast@ -- get if buffer is last                             */
/*----------------------------------------------------------------------------*/
void Task::vWorkingBufferIsLastFetch(MP_Task mpTask)
{
  ENTRY("vWorkingBufferIsLastFetch"_Stz);
  
  Object* pPtrWorkingBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrWorkingBuffer) )
    THROW_INVALID_DSTACK("workingBuffer.isLast@"_Stz);
  
  WorkingBuffer* pWorkingBuffer = _pWorkingBuffer(pPtrWorkingBuffer);
  
  mpTask->m_pDataStack->vPushInt(pWorkingBuffer->bIsLast());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* workingBuffer.isLast! -- set buffer is last                                */
/*----------------------------------------------------------------------------*/
void Task::vWorkingBufferIsLastStore(MP_Task mpTask)
{
  ENTRY("vWorkingBufferIsLastStore"_Stz);
  
  Object* pPtrWorkingBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrWorkingBuffer) )
    THROW_INVALID_DSTACK("workingBuffer.isLast!"_Stz);
  
  WorkingBuffer* pWorkingBuffer = _pWorkingBuffer(pPtrWorkingBuffer);
  
  pWorkingBuffer->vIsLast(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* workingBuffer::toWorkingBuffer -- convert string to working buffer pointer */
/*----------------------------------------------------------------------------*/
void Task::vWorkingBufferToWorkingBuffer(MP_Task mpTask)
{
  ENTRY("vWorkingBufferToWorkingBuffer"_Stz);
  
  s48K sString;
  
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("workingBuffer::toWorkingBuffer"_Stz);
  
  WorkingBuffer* pWorkingBuffer = reinterpret_cast<WorkingBuffer*>(sString.iToI8());
  
  mpTask->m_pDataStack->vPushObject(pWorkingBuffer);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
