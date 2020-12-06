/*============================================================================*/
/* Steps_ToRow.cpp - csv to row convertor                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ToRow(void)
{
  vAddStep("toRow[].completed"_Stz,   &Step::vToRsbRegCompleted);
  vAddStep("toRow[].convert"_Stz,     &Step::vToRsbRegConvert);
  vAddStep("toRow[].create"_Stz,      &Step::vToRsbRegCreate);
  vAddStep("toRow[].dangling"_Stz,    &Step::vToRsbRegDangling);
  vAddStep("toRow[].delete"_Stz,      &Step::vToRsbRegDelete);
}

/*----------------------------------------------------------------------------*/
/* toRow[].fullRsbBuffer -- full row buffer                                   */
/*----------------------------------------------------------------------------*/
void Step::vToRsbRegCompleted(void)
{
  ENTRY();
  
  ToRow* pToRow = *m_pLogicUnit->m_xpToRow;
  
  RowBuffer* pRsbBuffer = pToRow->pFullRsbBuffer();
  
  m_pDataStack->vPushPtr(pRsbBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRow[].convert -- convert csv buffer                                      */
/*----------------------------------------------------------------------------*/
void Step::vToRsbRegConvert(void)
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
void Step::vToRsbRegCreate(void)
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
/* toRow[].danglingRsbBuffer -- fetch row buffer                              */
/*----------------------------------------------------------------------------*/
void Step::vToRsbRegDangling(void)
{
  ENTRY();
  
  if (m_pLogicUnit->m_xpToRow.bIsNull()) {
    m_pDataStack->vPushPtr(nullptr);
  }
  else {
    ToRow* pToRow = *m_pLogicUnit->m_xpToRow;
    RowBuffer* pRsbBuffer = pToRow->pDanglingRsbBuffer();
    m_pDataStack->vPushPtr(pRsbBuffer);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRow[].delete -- delete toRow                                             */
/*----------------------------------------------------------------------------*/
void Step::vToRsbRegDelete(void)
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
