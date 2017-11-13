/*============================================================================*/
/* Steps_RowBuffer.cpp - row buffer step group                                */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// rowbuffer: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_RowBuffer(void)
{
  vAddPrimitive("rowBuffer.append"_Stz, &vRowBufferAppend, false);
  vAddPrimitive("rowBuffer.appendRecord"_Stz, &vRowBufferAppendRecord, false);
  vAddPrimitive("rowBuffer.dump"_Stz,   &vRowBufferDump,    false);
  vAddPrimitive("rowBuffer::new"_Stz,    &vRowBufferNew,    false);
}

/*----------------------------------------------------------------------------*/
/* rowbuffer.append -- attempt to append row onto row buffer                  */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferAppend(MP_Task mpTask)
{
ENTRY("vRowBufferAppend"_Stz);
  
  Object* pPtrRowBuffer;
  Entry* pRow;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("rowbuffer.append"_Stz);
  EntryBuffer* pRowBuffer = (EntryBuffer*)_pEntryBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());

  Object* pPtrTx;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("rowbuffer.append"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  s48K sRecord;
  if ( ! mpTask->m_pDataStack->bPop(sRecord) )
    THROW_INVALID_DSTACK("rowbuffer.append"_Stz);

  i8 iLength = sRecord.iLength();
  
  // create working row
  EntWrk xRowWrk;
  xRowWrk.vTxId(pTx->iDistTxId());
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
  ENTRY("vRowBufferAppendRecord"_Stz);
  
  Entry* pRow;
  
  Object* pPtrRowBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("rowBuffer.appendRecord"_Stz);
  EntryBuffer* pRowBuffer = (EntryBuffer*)_pEntryBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  Object* pPtrTx;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("rowBuffer.appendRecord"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  Object* pPtrFile;
  if ( ! mpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("rowBuffer.appendRecord"_Stz);
  
  File* pFile = _pFile(pPtrFile);
  
  Stz* aRecord = (Stz*)pFile->pRecord()->aWspStr();
  
  i8 iLength = pFile->iRecordLength();
  
  // create working row
  EntWrk xRowWrk;
  xRowWrk.vTxId(pTx->iDistTxId());
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
  ENTRY("vRowBufferDump"_Stz);
  
  Object* pPtrRowBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("rowbuffer.dump"_Stz);
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
  ENTRY("vRowBufferNew"_Stz);
  
  EntryBuffer* pRowBuffer = new EntryBuffer;
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  mpTask->m_pDataStack->vPushObject(pRowBuffer);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
