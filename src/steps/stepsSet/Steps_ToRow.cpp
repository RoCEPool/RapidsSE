/*============================================================================*/
/* Steps_ToRow.cpp - csv to row convertor                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ToRow(void)
{
  vAddStep("toRow[].completed"_Stz,   &Step::vToRowRegCompleted);
  vAddStep("toRow[].convert"_Stz,     &Step::vToRowRegConvert);
  vAddStep("toRow[].create"_Stz,      &Step::vToRowRegCreate);
  vAddStep("toRow[].dangling"_Stz,    &Step::vToRowRegDangling);
  vAddStep("toRow[].delete"_Stz,      &Step::vToRowRegDelete);
}

/*----------------------------------------------------------------------------*/
/* toRow[].completed -- full row buffer                                       */
/*----------------------------------------------------------------------------*/
void Step::vToRowRegCompleted(void)
{
  ENTRY();
  
  ToRow* pToRow = *m_pLogicUnit->m_xpToRow;
  
  RowBuffer* pRowBuffer = pToRow->pFullRowBuffer();
  
  m_pDataStack->vPushPtr(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRow[].convert -- convert csv buffer                                      */
/*----------------------------------------------------------------------------*/
void Step::vToRowRegConvert(void)
{
  ENTRY();

  ToRow* pToRow = *m_pLogicUnit->m_xpToRow;
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pLogicUnit->m_xpOperandA);
  CsvMeta* pCsvMeta = pCsvBuffer->pCsvMeta();

  char* azCsvChunk  = (char*)pCsvBuffer->ayDataArea();
  
  bool bResult = pToRow->bConvertCsv(azCsvChunk,pCsvMeta);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRow[].create -- create toRow                                             */
/*----------------------------------------------------------------------------*/
void Step::vToRowRegCreate(void)
{
  ENTRY();
  
  Job* pJob = m_pTask->pJob();
  
  TagSet* pJobTags = pJob->pJobTags();
  
  //pJobTags->vContentDump();

  Format* pFmt = new Format;
  pFmt->vSet(pFmt);
  
  bool b = pFmt->bSetFormat(pJobTags);

  StreamDef* pStreamDef = m_pLogicUnit->m_xpTable->pStreamDef();
  BucketMap* pBucketMap = m_pLogicUnit->m_xpTable->pBucketMap();

  bool bPtnHash = true;
  ToRow* pToRow = new ToRow(pStreamDef,pBucketMap,pFmt,bPtnHash);
      
  m_pLogicUnit->m_xpToRow = pToRow;
  
  delete pFmt;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRow[].dangling -- fetch row buffer                                       */
/*----------------------------------------------------------------------------*/
void Step::vToRowRegDangling(void)
{
  ENTRY();
  
  if (m_pLogicUnit->m_xpToRow.bIsNull()) {
    m_pDataStack->vPushPtr(nullptr);
  }
  else {
    ToRow* pToRow = *m_pLogicUnit->m_xpToRow;
    RowBuffer* pRowBuffer = pToRow->pDanglingRowBuffer();
    m_pDataStack->vPushPtr(pRowBuffer);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRow[].delete -- delete toRow                                             */
/*----------------------------------------------------------------------------*/
void Step::vToRowRegDelete(void)
{
  ENTRY();
  
  ToRow* pToRow = *m_pLogicUnit->m_xpToRow;
  m_pLogicUnit->m_xpToRow = nullptr;
  
  delete pToRow;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
