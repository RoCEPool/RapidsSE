/*============================================================================*/
/* Steps_ToRsb.cpp - csv to row convertor                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ToRsb(void)
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
  
  ToRsb* pToRsb = *m_pLogicUnit->m_xpToRsb;
  
  RowBuffer* pRsbBuffer = pToRsb->pFullRsbBuffer();
  
  m_pDataStack->vPushPtr(pRsbBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRow[].convert -- convert csv buffer                                      */
/*----------------------------------------------------------------------------*/
void Step::vToRsbRegConvert(void)
{
  ENTRY();

  ToRsb* pToRsb = *m_pLogicUnit->m_xpToRsb;
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pLogicUnit->m_xpOperandA);
  CsvMeta* pCsvMeta = pCsvBuffer->pCsvMeta();

  char* azCsvChunk  = (char*)pCsvBuffer->ayDataArea();
  
  bool bResult = pToRsb->bConvertCsv(azCsvChunk,pCsvMeta);
  
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
  ToRsb* pToRsb = new ToRsb(pStreamDef,pBucketMap,pFmt,bPtnHash);
      
  m_pLogicUnit->m_xpToRsb = pToRsb;
  
  delete pFmt;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRow[].danglingRsbBuffer -- fetch row buffer                              */
/*----------------------------------------------------------------------------*/
void Step::vToRsbRegDangling(void)
{
  ENTRY();
  
  if (m_pLogicUnit->m_xpToRsb.bIsNull()) {
    m_pDataStack->vPushPtr(nullptr);
  }
  else {
    ToRsb* pToRsb = *m_pLogicUnit->m_xpToRsb;
    RowBuffer* pRsbBuffer = pToRsb->pDanglingRsbBuffer();
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
  
  ToRsb* pToRsb = *m_pLogicUnit->m_xpToRsb;
  m_pLogicUnit->m_xpToRsb = nullptr;
  
  delete pToRsb;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
