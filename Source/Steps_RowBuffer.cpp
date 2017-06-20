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
  vAddPrimitive("rowBuffer::new"_Str,    &vRowBufferNew,    false);
}

/*----------------------------------------------------------------------------*/
/* rowbuffer.append -- attempt to append row onto row buffer                  */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferAppend(MP_Task mpTask)
{
ENTRY("vRowBufferAppend"_Str);
  
  Object* pPtrRowBuffer;
  Entry* pRow;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("rowbuffer.append"_Str);
  EntryBuffer* pRowBuffer = (EntryBuffer*)_pEntryBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());

  Object* pPtrTxRef;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("rowbuffer.append"_Str);
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  s48K sRecord;
  if ( ! mpTask->m_pDataStack->bPop(sRecord) )
    THROW_INVALID_DSTACK("rowbuffer.append"_Str);

  i8 iLength = sRecord.iLength();
  
  // create working row
  EntWrk xRowWrk;
  xRowWrk.vTxId(pTxRef->iTxId());
  xRowWrk.vData(sRecord, iLength);
  
  bool b = pRowBuffer->bInsertRow(xRowWrk,&pRow);
  
  if (b) {
    mpTask->m_pDataStack->vPushInt((i8)1);
  }
  else {
    mpTask->m_pDataStack->vPushS48K(sRecord);
    mpTask->m_pDataStack->vPushInt((i8)0);
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowbuffer.appendRecord -- attempt to append record onto row buffer         */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferAppendRecord(MP_Task mpTask)
{
  ENTRY("vRowBufferAppendRecord"_Str);
  
  Entry* pRow;
  
  Object* pPtrRowBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("rowBuffer.appendRecord"_Str);
  EntryBuffer* pRowBuffer = (EntryBuffer*)_pEntryBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  Object* pPtrTxRef;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("rowBuffer.appendRecord"_Str);
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  Object* pPtrFile;
  if ( ! mpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("rowBuffer.appendRecord"_Str);
  
  File* pFile = _pFile(pPtrFile);
  
  Str* aRecord = (Str*)pFile->pRecord()->aWspStr();
  
  i8 iLength = pFile->iRecordLength();
  
  // create working row
  EntWrk xRowWrk;
  xRowWrk.vTxId(pTxRef->iTxId());
  xRowWrk.vData(aRecord, iLength);
  
  bool b = pRowBuffer->bInsertRow(xRowWrk,&pRow);
  
  if (b) {
    mpTask->m_pDataStack->vPushInt((i8)1);
  }
  else {
    mpTask->m_pDataStack->vPushInt((i8)0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowbuffer.dump -- dump the row buffer                                      */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferDump(MP_Task mpTask)
{
  ENTRY("vRowBufferDump"_Str);
  
  Object* pPtrRowBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("rowbuffer.dump"_Str);
  EntryBuffer* pRowBuffer = (EntryBuffer*)_pEntryBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());

  Entry* pRow = pRowBuffer->pFirstRow();
  
  while (pRow) {
    pRow->vContentDump();
    pRow = pRowBuffer->pNextRow(pRow);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowbuffer::new -- new a row buffer                                          */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferNew(MP_Task mpTask)
{
  ENTRY("vRowBufferNew"_Str);
  
  EntryBuffer* pRowBuffer = new EntryBuffer;
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  mpTask->m_pDataStack->vPushObject(pRowBuffer);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
