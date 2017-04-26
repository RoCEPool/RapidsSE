/*============================================================================*/
/* Steps_RowBuffer.cpp - row buffer step group                                */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// rowbuffer: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_RowBuffer(void)
{
  vAddPrimitive("rowBuffer.append"_Str, &vRowBufferAppend, false);
  vAddPrimitive("rowBuffer.appendRecord"_Str, &vRowBufferAppendRecord, false);
  vAddPrimitive("rowBuffer.dump"_Str,   &vRowBufferDump,    false);
  vAddPrimitive("rowBuffer.new"_Str,    &vRowBufferNew,    false);
}

/*----------------------------------------------------------------------------*/
/* rowbuffer.append -- attempt to append row onto row buffer                  */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferAppend(NNP_Task nnpTask)
{
ENTRY("vRowBufferAppend"_Str);
  
  Object* pPtrRowBuffer;
  Row* pRow;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("rowbuffer.append"_Str);
  RowBuffer* pRowBuffer = _pRowBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());

  Object* pPtrTxRef;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("rowbuffer.append"_Str);
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  s48K sRecord;
  if ( ! nnpTask->m_pDataStack->bPop(sRecord) )
    THROW_INVALID_DSTACK("rowbuffer.append"_Str);

  i8 iLength = sRecord.iLength();
  
  bool b = pRowBuffer->bInsertRow(pTxRef,sRecord,iLength,&pRow);
  
  if (b) {
    nnpTask->m_pDataStack->vPushInt((i8)1);
  }
  else {
    nnpTask->m_pDataStack->vPush(sRecord);
    nnpTask->m_pDataStack->vPushInt((i8)0);
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowbuffer.appendRecord -- attempt to append record onto row buffer         */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferAppendRecord(NNP_Task nnpTask)
{
  ENTRY("vRowBufferAppendRecord"_Str);
  
  Row* pRow;
  
  Object* pPtrRowBuffer;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("rowBuffer.appendRecord"_Str);
  RowBuffer* pRowBuffer = _pRowBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  Object* pPtrTxRef;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("rowBuffer.appendRecord"_Str);
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  Object* pPtrFile;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("rowBuffer.appendRecord"_Str);
  
  File* pFile = _pFile(pPtrFile);
  
  Str* aRecord = (Str*)pFile->pRecord()->aWspStr();
  
  i8 iLength = pFile->iRecordLength();
  
  bool b = pRowBuffer->bInsertRow(pTxRef,aRecord,iLength,&pRow);
  
  if (b) {
    nnpTask->m_pDataStack->vPushInt((i8)1);
  }
  else {
    nnpTask->m_pDataStack->vPushInt((i8)0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowbuffer.dump -- dump the row buffer                                      */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferDump(NNP_Task nnpTask)
{
  ENTRY("vRowBufferDump"_Str);
  
  Object* pPtrRowBuffer;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("rowbuffer.dump"_Str);
  RowBuffer* pRowBuffer = _pRowBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());

  Row* pRow = pRowBuffer->pFirstRow();
  
  while (pRow) {
    pRow->vContentDump();
    pRow = pRowBuffer->pNextRow(pRow);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowbuffer.new -- new a row buffer                                          */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferNew(NNP_Task nnpTask)
{
  ENTRY("vRowBufferNew"_Str);
  
  RowBuffer* pRowBuffer = new RowBuffer;
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  nnpTask->m_pDataStack->vPushObject(pRowBuffer);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
