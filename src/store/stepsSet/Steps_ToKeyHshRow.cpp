/*============================================================================*/
/* Steps_ToKeyHshRow.cpp - csv to row convertor                               */
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
void Step::vAddSteps_ToKeyHshRow(void)
{
  vAddStep("ToKeyHshRow[]"_Stz,               &Step::vToKeyHshRowFetch);
  vAddStep("ToKeyHshRow[].completed"_Stz,     &Step::vToKeyHshRowCompleted);
  vAddStep("ToKeyHshRow[].counters@@"_Stz,    &Step::vToKeyHshRowCountersFetch);
  vAddStep("ToKeyHshRow[].csvToRow"_Stz,      &Step::vToKeyHshRowCsvToRow);
  vAddStep("ToKeyHshRow[].dangling"_Stz,      &Step::vToKeyHshRowDangling);
  vAddStep("ToKeyHshRow[].delete"_Stz,        &Step::vToKeyHshRowDelete);
  vAddStep("ToKeyHshRow[].new"_Stz,           &Step::vToKeyHshRowNew);
}

/*----------------------------------------------------------------------------*/
/* ToKeyHshRow[].completed -- fetch full/complete row buffer                  */
/*----------------------------------------------------------------------------*/
void Step::vToKeyHshRowCompleted(void)
{
  ENTRY();
  
  ToKeyHshRow* pToKeyHshRow = * m_pLogicUnit->m_xpToKeyHshRow;

  RowBuffer* pRowBuffer = pToKeyHshRow->pFullRowBuffer();
  
  m_pDataStack->vPushPtr(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToKeyHshRow[].counters@@ -- fetch row counters                             */
/*----------------------------------------------------------------------------*/
void Step::vToKeyHshRowCountersFetch(void)
{
  ENTRY();
  
  ToKeyHshRow* pToKeyHshRow = *m_pLogicUnit->m_xpToKeyHshRow;

  m_pDataStack->vPushInt(pToKeyHshRow->iRowsSkipped());
  m_pDataStack->vPushInt(pToKeyHshRow->iRowsInvalid());
  m_pDataStack->vPushInt(pToKeyHshRow->iRowsProcessed());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToKeyHshRow[].csvToRow -- convert given csv buffer to row buffer           */
/*----------------------------------------------------------------------------*/
void Step::vToKeyHshRowCsvToRow(void)
{
  ENTRY();

  CsvBuffer* pCsvBuffer = _pCsvBuffer(m_pDataStack->pPopPtr(__func__));

  ToKeyHshRow* pToKeyHshRow = *m_pLogicUnit->m_xpToKeyHshRow;

  bool b = pToKeyHshRow->bConvertCsv(pCsvBuffer);

  UNUSED(b);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToKeyHshRow[].dangling -- fetch dangling row buffer                        */
/*----------------------------------------------------------------------------*/
void Step::vToKeyHshRowDangling(void)
{
  ENTRY();
  
  if (m_pLogicUnit->m_xpToKeyHshRow.bIsNull()) {
    m_pDataStack->vPushPtr(nullptr);
  }
  else {
    ToKeyHshRow* pToKeyHshRow = *m_pLogicUnit->m_xpToKeyHshRow;
    RowBuffer* pRowBuffer = pToKeyHshRow->pDanglingRowBuffer();
    m_pDataStack->vPushPtr(pRowBuffer);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToKeyHshRow[].delete -- delete ToKeyHshRow                                 */
/*----------------------------------------------------------------------------*/
void Step::vToKeyHshRowDelete(void)
{
  ENTRY();
  
  ToKeyHshRow* pToKeyHshRow = *m_pLogicUnit->m_xpToKeyHshRow;
  m_pLogicUnit->m_xpToKeyHshRow = nullptr;
  
  delete pToKeyHshRow;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToKeyHshRow[] -- fetch ToKeyHshRow                                         */
/*----------------------------------------------------------------------------*/
void Step::vToKeyHshRowFetch(void)
{
  ENTRY();
  
  ToKeyHshRow* pToKeyHshRow = * m_pLogicUnit->m_xpToKeyHshRow;

  m_pDataStack->vPushPtr(pToKeyHshRow);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToKeyHshRow[].new -- new ToKeyHshRow                                       */
/*----------------------------------------------------------------------------*/
void Step::vToKeyHshRowNew(void)
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


  ToKeyHshRow* pToKeyHshRow = new ToKeyHshRow(pStreamDef,pBucketMap,pFmt);
      
  m_pLogicUnit->m_xpToKeyHshRow = pToKeyHshRow;
  
  delete pFmt;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
