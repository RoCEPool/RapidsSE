/*============================================================================*/
/* Steps_RowBuffer.cpp - row buffer step group                                */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

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
  vAddPrimitive("rowBuffer.append", &vRowBufferAppend, false);
  vAddPrimitive("rowBuffer.appendRecord", &vRowBufferAppendRecord, false);
  vAddPrimitive("rowBuffer.dump",   &vRowBufferDump,    false);
  vAddPrimitive("rowBuffer.new",    &vRowBufferNew,    false);
}

/*----------------------------------------------------------------------------*/
/* rowbuffer.append -- attempt to append row onto row buffer                  */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferAppend(NNP_Task nnpTask)
{
ENTRY("vRowBufferAppend");
  
  Object* pPtrRowBuffer;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("rowbuffer.append");
  RowBuffer* pRowBuffer = _pRowBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());

  Object* pPtrTxRef;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("rowbuffer.append");
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  s48K sRecord;
  if ( ! nnpTask->m_pDataStack->bPop(sRecord) )
    THROW_INVALID_DSTACK("rowbuffer.append");

  i8 iLength = sRecord.iLength();
  
  bool b = pRowBuffer->bInsertRow(pTxRef,sRecord.a(),iLength);
  
  if (b) {
    nnpTask->m_pDataStack->vPush((i8)1);
  }
  else {
    nnpTask->m_pDataStack->vPush(sRecord);
    nnpTask->m_pDataStack->vPush((i8)0);
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowbuffer.appendRecord -- attempt to append record onto row buffer         */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferAppendRecord(NNP_Task nnpTask)
{
  ENTRY("vRowBufferAppendRecord");
  
  Object* pPtrRowBuffer;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("rowBuffer.appendRecord");
  RowBuffer* pRowBuffer = _pRowBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  Object* pPtrTxRef;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("rowBuffer.appendRecord");
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  Object* pPtrFile;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("rowBuffer.appendRecord");
  
  File* pFile = _pFile(pPtrFile);
  
  char* aRecord = pFile->pRecord()->pWorkArea();
  
//  i8 iLength = String::iLength(aRecord);
  i8 iLength = pFile->iRecordLength();
  
  bool b = pRowBuffer->bInsertRow(pTxRef,aRecord,iLength);
  
  if (b) {
    nnpTask->m_pDataStack->vPush((i8)1);
  }
  else {
    nnpTask->m_pDataStack->vPush((i8)0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowbuffer.dump -- dump the row buffer                                      */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferDump(NNP_Task nnpTask)
{
  ENTRY("vRowBufferDump");
  
  Object* pPtrRowBuffer;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("rowbuffer.dump");
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
  ENTRY("vRowBufferNew");
  
  RowBuffer* pRowBuffer = new RowBuffer;
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  nnpTask->m_pDataStack->vPush(pRowBuffer);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
