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
void Step::vAddSteps_WorkingBuffer(void)
{
  vAddStep("workingBuffer.isLast@"_Stz,         &Task::vWorkingBufferIsLastFetch, false);
  vAddStep("workingBuffer.isLast!"_Stz,         &Task::vWorkingBufferIsLastStore, false);
  vAddStep("workingBuffer::toWorkingBuffer"_Stz, &Task::vWorkingBufferToWorkingBuffer,false);
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

/*----------------------------------------------------------------------------*/
/*    PRIVATE                                                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append a row to a working buffer                                           */
/*----------------------------------------------------------------------------*/
bool Task::bAppendRow(WorkingBuffer* pWorkingBuffer, Entry* pEntry, SEitf* pSEitf)
{
  
  Id xId = pEntry->xId();
  
  if (pSEitf && pSEitf->bRowSeen(xId)) {
    // row already inserted
    //vTrace("Row already inserted %p \n"_Stz, pEntry);
    return true;
  }
  
  Row* aRow = pEntry->aRow();
  i8 nRowSize = pEntry->nRowSize();
  
  if (pWorkingBuffer->bAppendRow(aRow, nRowSize))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* append a row to a working buffer                                           */
/*----------------------------------------------------------------------------*/
bool Task::bAppendRow(FetchBuffer* pFetchBuffer, Entry* pEntry, SEitf* pSEitf)
{
  
  Id xId = pEntry->xId();
  
  if (pSEitf && pSEitf->bRowSeen(xId)) {
    // row already inserted
    //vTrace("Row already inserted %p \n"_Stz, pEntry);
    return true;
  }
  
  Row* aRow = pEntry->aRow();
  
  if (pFetchBuffer->bAppendRow(aRow))
    return true;
  else
    return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
