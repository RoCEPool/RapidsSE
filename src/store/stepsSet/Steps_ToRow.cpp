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
#include "TablePtn.hpp"
#include "Table.hpp"

#include "Node_I.hpp"

#include "Job.hpp"
#include "LogicUnit.hpp"

#include "MY.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ToRow(void)
{
  vAddStep("ToKeyHshRow[]"_Stz,               &Step::vToRowFetch);
  vAddStep("ToKeyHshRow[].completed"_Stz,     &Step::vToRowCompleted);
  vAddStep("ToKeyHshRow[].counters@@"_Stz,    &Step::vToRowCountersFetch);
  vAddStep("ToKeyHshRow[].create"_Stz,        &Step::vToRowCreate);
  vAddStep("ToKeyHshRow[].csvToRow"_Stz,      &Step::vToRowCsvToRow);
  vAddStep("ToKeyHshRow[].dangling"_Stz,      &Step::vToRowDangling);
  vAddStep("ToKeyHshRow[].delete"_Stz,        &Step::vToRowDelete);
}

/*----------------------------------------------------------------------------*/
/* ToKeyHshRow[].completed -- full row buffer                                 */
/*----------------------------------------------------------------------------*/
void Step::vToRowCompleted(void)
{
  ENTRY();
  
  ToKeyHshRow* pToRow = * m_pLogicUnit->m_xpToRow;

  RowBuffer* pRowBuffer = pToRow->pFullRowBuffer();
  
  m_pDataStack->vPushPtr(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToKeyHshRow[].counters@@ -- fetch row counters                             */
/*----------------------------------------------------------------------------*/
void Step::vToRowCountersFetch(void)
{
  ENTRY();
  
  ToKeyHshRow* pToRow = *m_pLogicUnit->m_xpToRow;

  m_pDataStack->vPushInt(pToRow->iRowsSkipped());
  m_pDataStack->vPushInt(pToRow->iRowsInvalid());
  m_pDataStack->vPushInt(pToRow->iRowsProcessed());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToKeyHshRow[].csvToRow -- convert given csv buffer to row buffer           */
/*----------------------------------------------------------------------------*/
void Step::vToRowCsvToRow(void)
{
  ENTRY();

  CsvBuffer* pCsvBuffer = _pCsvBuffer(m_pDataStack->pPopPtr(__func__));

  ToKeyHshRow* pToRow = *m_pLogicUnit->m_xpToRow;

  bool b = pToRow->bConvertCsv(pCsvBuffer);

  UNUSED(b);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToKeyHshRow[].create -- create toRow                                       */
/*----------------------------------------------------------------------------*/
void Step::vToRowCreate(void)
{
  ENTRY();
  
  Job* pJob = MY::pJob();
  
  TagSet* pJobTags = pJob->pJobTags();
  
  //pJobTags->vContentDump();

  Format* pFmt = new Format;
  pFmt->vSet(pFmt);
  
  bool b = AM::bFormatSetFromTagSet(pFmt,pJobTags);
  UNUSED(b);

  StreamDef* pStreamDef = m_pLogicUnit->m_xpTable->pStreamDef();

  BucketMap* pBucketMap = m_pLogicUnit->m_xpTable->pBucketMap();


  ToKeyHshRow* pToRow = new ToKeyHshRow(pStreamDef,pBucketMap,pFmt);
      
  m_pLogicUnit->m_xpToRow = pToRow;
  
  delete pFmt;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToKeyHshRow[].dangling -- fetch row buffer                                 */
/*----------------------------------------------------------------------------*/
void Step::vToRowDangling(void)
{
  ENTRY();
  
  if (m_pLogicUnit->m_xpToRow.bIsNull()) {
    m_pDataStack->vPushPtr(nullptr);
  }
  else {
    ToKeyHshRow* pToRow = *m_pLogicUnit->m_xpToRow;
    RowBuffer* pRowBuffer = pToRow->pDanglingRowBuffer();
    m_pDataStack->vPushPtr(pRowBuffer);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToKeyHshRow[].delete -- delete toRow                                       */
/*----------------------------------------------------------------------------*/
void Step::vToRowDelete(void)
{
  ENTRY();
  
  ToKeyHshRow* pToRow = *m_pLogicUnit->m_xpToRow;
  m_pLogicUnit->m_xpToRow = nullptr;
  
  delete pToRow;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToKeyHshRow[] -- fetch toRow                                               */
/*----------------------------------------------------------------------------*/
void Step::vToRowFetch(void)
{
  ENTRY();
  
  ToKeyHshRow* pToRow = * m_pLogicUnit->m_xpToRow;

  m_pDataStack->vPushPtr(pToRow);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
