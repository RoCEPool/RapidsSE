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
  vAddStep("ToRow[]"_Stz,               &Step::vToRowFetch);
  vAddStep("ToRow[].completed"_Stz,     &Step::vToRowCompleted);
  vAddStep("ToRow[].create"_Stz,        &Step::vToRowCreate);
  vAddStep("ToRow[].dangling"_Stz,      &Step::vToRowDangling);
  vAddStep("ToRow[].delete"_Stz,        &Step::vToRowDelete);
  vAddStep("ToRow[].rowCounters@@"_Stz, &Step::vToRowRowCountersFetch);
}

/*----------------------------------------------------------------------------*/
/* ToRow[].completed -- full row buffer                                       */
/*----------------------------------------------------------------------------*/
void Step::vToRowCompleted(void)
{
  ENTRY();
  
  //ToRow* pToRow = *m_pLogicUnit->m_xpToRow;
  ToRow* pToRow = * m_pLogicUnit->m_xpToRow;

  RowBuffer* pRowBuffer = pToRow->pFullRowBuffer();
  
  m_pDataStack->vPushPtr(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToRow[].create -- create toRow                                             */
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


  bool bPtnHash = true;
  ToRow* pToRow = new ToRow(pStreamDef,pBucketMap,pFmt,bPtnHash);
      
  m_pLogicUnit->m_xpToRow = pToRow;
  
  delete pFmt;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToRow[].dangling -- fetch row buffer                                       */
/*----------------------------------------------------------------------------*/
void Step::vToRowDangling(void)
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
/* ToRow[].delete -- delete toRow                                             */
/*----------------------------------------------------------------------------*/
void Step::vToRowDelete(void)
{
  ENTRY();
  
  ToRow* pToRow = *m_pLogicUnit->m_xpToRow;
  m_pLogicUnit->m_xpToRow = nullptr;
  
  delete pToRow;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToRow[] -- fetch toRow                                                     */
/*----------------------------------------------------------------------------*/
void Step::vToRowFetch(void)
{
  ENTRY();
  
  ToRow* pToRow = * m_pLogicUnit->m_xpToRow;

  m_pDataStack->vPushPtr(pToRow);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToRow[].rowCounters@@ -- fetch row counters                                */
/*----------------------------------------------------------------------------*/
void Step::vToRowRowCountersFetch(void)
{
  ENTRY();
  
  ToRow* pToRow = *m_pLogicUnit->m_xpToRow;

  m_pDataStack->vPushInt(pToRow->iRowsSkipped());
  m_pDataStack->vPushInt(pToRow->iRowsInvalid());
  m_pDataStack->vPushInt(pToRow->iRowsProcessed());

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
