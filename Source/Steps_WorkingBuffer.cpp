/*============================================================================*/
/* Steps_WorkingBuffer.cpp - working buffer step group                        */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap*  gpAttachMap;
extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_WorkingBuffer(void)
{
  vAddStep("workingBuffer.isLast@"_Stz,          &Step::vWorkingBufferIsLastFetch);
  vAddStep("workingBuffer.isLast!"_Stz,          &Step::vWorkingBufferIsLastStore);
  vAddStep("workingBuffer.rows"_Stz,             &Step::vWorkingBufferRows);
  vAddStep("workingBuffer.show"_Stz,             &Step::vWorkingBufferShow);
  vAddStep("workingBuffer::toWorkingBuffer"_Stz, &Step::vWorkingBufferToWorkingBuffer);
}

/*----------------------------------------------------------------------------*/
/* workingBuffer.isLast@ -- get if buffer is last                             */
/*----------------------------------------------------------------------------*/
void Step::vWorkingBufferIsLastFetch(MP_Task mpTask)
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
void Step::vWorkingBufferIsLastStore(MP_Task mpTask)
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
/* workingBuffer.rows -- get number of rows in working buffer                 */
/*----------------------------------------------------------------------------*/
void Step::vWorkingBufferRows(MP_Task mpTask)
{
  ENTRY("vWorkingBufferShow"_Stz);
  
  Object* pPtrWorkingBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrWorkingBuffer) )
    THROW_INVALID_DSTACK("workingBuffer.isLast@"_Stz);
  
  WorkingBuffer* pWorkingBuffer = _pWorkingBuffer(pPtrWorkingBuffer);
 
  mpTask->m_pDataStack->vPushInt(pWorkingBuffer->nWorkingRows());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* workingBuffer.show -- show info about a working buffer                     */
/*----------------------------------------------------------------------------*/
void Step::vWorkingBufferShow(MP_Task mpTask)
{
  ENTRY("vWorkingBufferShow"_Stz);
  
  Object* pPtrWorkingBuffer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrWorkingBuffer) )
    THROW_INVALID_DSTACK("workingBuffer.show"_Stz);
  
  WorkingBuffer* pWorkingBuffer = _pWorkingBuffer(pPtrWorkingBuffer);

  pWorkingBuffer->vShow();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* workingBuffer::toWorkingBuffer -- convert string to working buffer pointer */
/*----------------------------------------------------------------------------*/
void Step::vWorkingBufferToWorkingBuffer(MP_Task mpTask)
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
bool Task::bAppendRow(FetchBuffer* pFetchBuffer, Entry* pEntry, Job_FetchRows* pJob_FetchRows)
{
  
  Id xId = pEntry->xId();
  
  if (pJob_FetchRows && pJob_FetchRows->bRowSeen(xId)) {
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
