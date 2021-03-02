/*============================================================================*/
/* Steps_ToRow.cpp - csv to row convertor                                     */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "RowBuffer.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "ReadApi.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"

#include "Node_I.hpp"

#include "Response.hpp"
#include "Result.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ToRow(void)
{
  vAddStep("toRow[].completed"_Stz,     &Step::vToRowCompleted);
  vAddStep("toRow[].convert"_Stz,       &Step::vToRowConvert);
  vAddStep("toRow[].create"_Stz,        &Step::vToRowCreate);
  vAddStep("toRow[].dangling"_Stz,      &Step::vToRowDangling);
  vAddStep("toRow[].delete"_Stz,        &Step::vToRowDelete);
  vAddStep("toRow[].rowCounters@@"_Stz, &Step::vToRowRowCountersFetch);
}

/*----------------------------------------------------------------------------*/
/* toRow[].completed -- full row buffer                                       */
/*----------------------------------------------------------------------------*/
void Step::vToRowCompleted(void)
{
  ENTRY();
  
  //ToRow* pToRow = *( m_pJob->pLogicUnit_job() )->m_xpToRow;
  ToRow* pToRow = * ( m_pJob->pLogicUnit_job() )->m_xpToRow;

  RowBuffer* pRowBuffer = pToRow->pFullRowBuffer();
  
  m_pDataStack->vPushPtr(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRow[].convert -- convert csv buffer                                      */
/*----------------------------------------------------------------------------*/
void Step::vToRowConvert(void)
{
  ENTRY();

  ToRow* pToRow = *( m_pJob->pLogicUnit_job() )->m_xpToRow;
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*( m_pJob->pLogicUnit_job() )->m_xpOperandA);
  CsvMeta* pCsvMeta = pCsvBuffer->pCsvMeta();

  char* azCsvChunk  = (char*)pCsvBuffer->ayDataArea();
  
  bool bResult = pToRow->bConvertCsv(azCsvChunk,pCsvMeta);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRow[].create -- create toRow                                             */
/*----------------------------------------------------------------------------*/
void Step::vToRowCreate(void)
{
  ENTRY();
  
  Job* pJob = m_pJob;
  
  TagSet* pJobTags = pJob->pJobTags();
  
  //pJobTags->vContentDump();

  Format* pFmt = new Format;
  pFmt->vSet(pFmt);
  
  bool b = AM::bFormatSetFromTagSet(pFmt,pJobTags);

  StreamDef* pStreamDef = ( m_pJob->pLogicUnit_job() )->m_xpTable->pStreamDef();
  BucketMap* pBucketMap = ( m_pJob->pLogicUnit_job() )->m_xpTable->pBucketMap();

  bool bPtnHash = true;
  ToRow* pToRow = new ToRow(pStreamDef,pBucketMap,pFmt,bPtnHash);
      
  ( m_pJob->pLogicUnit_job() )->m_xpToRow = pToRow;
  
  delete pFmt;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRow[].dangling -- fetch row buffer                                       */
/*----------------------------------------------------------------------------*/
void Step::vToRowDangling(void)
{
  ENTRY();
  
  if (( m_pJob->pLogicUnit_job() )->m_xpToRow.bIsNull()) {
    m_pDataStack->vPushPtr(nullptr);
  }
  else {
    ToRow* pToRow = *( m_pJob->pLogicUnit_job() )->m_xpToRow;
    RowBuffer* pRowBuffer = pToRow->pDanglingRowBuffer();
    m_pDataStack->vPushPtr(pRowBuffer);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRow[].delete -- delete toRow                                             */
/*----------------------------------------------------------------------------*/
void Step::vToRowDelete(void)
{
  ENTRY();
  
  ToRow* pToRow = *( m_pJob->pLogicUnit_job() )->m_xpToRow;
  ( m_pJob->pLogicUnit_job() )->m_xpToRow = nullptr;
  
  delete pToRow;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRow[].rowCounters@@ -- fetch row counters                                */
/*----------------------------------------------------------------------------*/
void Step::vToRowRowCountersFetch(void)
{
  ENTRY();
  
  ToRow* pToRow = *( m_pJob->pLogicUnit_job() )->m_xpToRow;

  m_pDataStack->vPushInt(pToRow->iRowsSkipped());
  m_pDataStack->vPushInt(pToRow->iRowsInvalid());
  m_pDataStack->vPushInt(pToRow->iRowsProcessed());

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
