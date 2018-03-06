/*============================================================================*/
/* Steps_ResultBuffer.cpp - result buffer step group                          */
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
void Task::vAddPrimitives_ResultBuffer(void)
{
  vAddPrimitive("resultBuffer.isLast@"_Stz,         &vResultBufferIsLastFetch, false);
  vAddPrimitive("resultBuffer.isLast!"_Stz,         &vResultBufferIsLastStore, false);
  vAddPrimitive("resultBuffer::toResultBuffer"_Stz, &vResultBufferToResultBuffer,false);
}

/*----------------------------------------------------------------------------*/
/* resultBuffer.isLast@ -- get if buffer is last                               */
/*----------------------------------------------------------------------------*/
void Task::vResultBufferIsLastFetch(MP_Task mpTask)
{
  ENTRY("vResultBufferIsLastFetch"_Stz);
  
  Object* pPtrResultBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrResultBuffer) )
    THROW_INVALID_DSTACK("resultBuffer.isLast@"_Stz);
  
  ResultBuffer* pResultBuffer = _pResultBuffer(pPtrResultBuffer);
  
  mpTask->m_pDataStack->vPushInt(pResultBuffer->bIsLast());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* resultBuffer.isLast! -- set buffer is last                                  */
/*----------------------------------------------------------------------------*/
void Task::vResultBufferIsLastStore(MP_Task mpTask)
{
  ENTRY("vResultBufferIsLastStore"_Stz);
  
  Object* pPtrResultBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrResultBuffer) )
    THROW_INVALID_DSTACK("resultBuffer.isLast!"_Stz);
  
  ResultBuffer* pResultBuffer = _pResultBuffer(pPtrResultBuffer);
  
  pResultBuffer->vIsLast(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* resultBuffer::toResultBuffer -- convert string to result buffer pointer    */
/*----------------------------------------------------------------------------*/
void Task::vResultBufferToResultBuffer(MP_Task mpTask)
{
  ENTRY("vResultBufferToResultBuffer"_Stz);
  
  s48K sString;
  
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("resultBuffer::toResultBuffer"_Stz);
  
  ResultBuffer* pResultBuffer = reinterpret_cast<ResultBuffer*>(sString.iToI8());
  
  mpTask->m_pDataStack->vPushObject(pResultBuffer);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
