/*============================================================================*/
/* Steps_RowCounts.cpp - RowCounts step group for client statement statistics */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RowCounts(void)
{
  vAddStep("rowCounts[]"_Stz,                &Step::vRowCountsGet);
  
  vAddStep("rowCounts[].delete"_Stz,         &Step::vRowCountsDelete);
  vAddStep("rowCounts[].isNull"_Stz,         &Step::vRowCountsIsNull);
  vAddStep("rowCounts[].new"_Stz,            &Step::vRowCountsNew);

  vAddStep("rowCounts[].addProcessed"_Stz,   &Step::vRowCountsAddProcessed);
  vAddStep("rowCounts[].addSkipped"_Stz,     &Step::vRowCountsAddSkipped);
  vAddStep("rowCounts[].addInvalid"_Stz,     &Step::vRowCountsAddInvalid);
  vAddStep("rowCounts[].addInserted"_Stz,    &Step::vRowCountsAddInserted);
  vAddStep("rowCounts[].addUpdated"_Stz,     &Step::vRowCountsAddUpdated);
  vAddStep("rowCounts[].addDeleted"_Stz,     &Step::vRowCountsAddDeleted);

  vAddStep("rowCounts[].getCounters@@"_Stz,  &Step::vRowCountsGetAllCounts);
  vAddStep("rowCounts[].numCounters"_Stz,    &Step::vRowCountsGetNumCounters);
}

/*----------------------------------------------------------------------------*/
/* rowCounts[].delete -- delete the rowCounts register                        */
/*----------------------------------------------------------------------------*/
void Step::vRowCountsDelete(void)
{
  ENTRY();
  
  RowCounts* pRowCounts = _pRowCounts(*m_pLogicUnit->m_xpRowCounts);
  
  delete pRowCounts;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCounts[] -- get rowCounts register                                      */
/*----------------------------------------------------------------------------*/
void Step::vRowCountsGet(void)
{
  ENTRY();
  
  RowCounts* pRowCounts = _pRowCounts(*m_pLogicUnit->m_xpRowCounts);
  
  m_pDataStack->vPushPtr(pRowCounts);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCounts[].isNull -- is the rowCounts register null?                      */
/*----------------------------------------------------------------------------*/
void Step::vRowCountsIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpRowCounts.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCounts[].new -- new a rowCounts register                                */
/*----------------------------------------------------------------------------*/
void Step::vRowCountsNew(void)
{
  ENTRY();

  ASSERT(m_pLogicUnit->m_xpRowCounts.bIsNull());
  m_pLogicUnit->m_xpRowCounts = new RowCounts;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCounts[].addProcessed -- accumulate number of rows processed            */
/*----------------------------------------------------------------------------*/
void Step::vRowCountsAddProcessed(void)
{
  ENTRY();
  
  i8 nRows = m_pDataStack->iPopInt(__func__);
  ASSERT(nRows >= 0);

  ASSERT(!m_pLogicUnit->m_xpRowCounts.bIsNull());
  RowCounts* pRowCounts = _pRowCounts(*m_pLogicUnit->m_xpRowCounts);
  pRowCounts->vAddRowsProcessed(nRows);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCounts[].addSkipped -- accumulate number of rows skipped                */
/*----------------------------------------------------------------------------*/
void Step::vRowCountsAddSkipped(void)
{
  ENTRY();

  i8 nRows = m_pDataStack->iPopInt(__func__);
  ASSERT(nRows >= 0);

  ASSERT(!m_pLogicUnit->m_xpRowCounts.bIsNull());
  RowCounts* pRowCounts = _pRowCounts(*m_pLogicUnit->m_xpRowCounts);
  pRowCounts->vAddRowsSkipped(nRows);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCounts[].addInvalid -- accumulate number of rows deemed invalid         */
/*----------------------------------------------------------------------------*/
void Step::vRowCountsAddInvalid(void)
{
  ENTRY();

  i8 nRows = m_pDataStack->iPopInt(__func__);
  ASSERT(nRows >= 0);

  ASSERT(!m_pLogicUnit->m_xpRowCounts.bIsNull());
  RowCounts* pRowCounts = _pRowCounts(*m_pLogicUnit->m_xpRowCounts);
  pRowCounts->vAddRowsInvalid(nRows);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCounts[].addInserted -- accumulate number of rows inserted              */
/*----------------------------------------------------------------------------*/
void Step::vRowCountsAddInserted(void)
{
  ENTRY();

  i8 nRows = m_pDataStack->iPopInt(__func__);
  ASSERT(nRows >= 0);

  ASSERT(!m_pLogicUnit->m_xpRowCounts.bIsNull());
  RowCounts* pRowCounts = _pRowCounts(*m_pLogicUnit->m_xpRowCounts);
  pRowCounts->vAddRowsInserted(nRows);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCounts[].addUpdated -- accumulate number of rows updated                */
/*----------------------------------------------------------------------------*/
void Step::vRowCountsAddUpdated(void)
{
  ENTRY();

  i8 nRows = m_pDataStack->iPopInt(__func__);
  ASSERT(nRows >= 0);

  ASSERT(!m_pLogicUnit->m_xpRowCounts.bIsNull());
  RowCounts* pRowCounts = _pRowCounts(*m_pLogicUnit->m_xpRowCounts);
  pRowCounts->vAddRowsUpdated(nRows);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCounts[].addDeleted -- accumulate number of rows deleted                */
/*----------------------------------------------------------------------------*/
void Step::vRowCountsAddDeleted(void)
{
  ENTRY();

  i8 nRows = m_pDataStack->iPopInt(__func__);
  ASSERT(nRows >= 0);

  ASSERT(!m_pLogicUnit->m_xpRowCounts.bIsNull());
  RowCounts* pRowCounts = _pRowCounts(*m_pLogicUnit->m_xpRowCounts);
  pRowCounts->vAddRowsDeleted(nRows);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCounts[].getCounters@@ -- push all counter values                       */
/*----------------------------------------------------------------------------*/
void Step::vRowCountsGetAllCounts(void)
{
  ENTRY();

  ASSERT(!m_pLogicUnit->m_xpRowCounts.bIsNull());
  RowCounts* pRowCounts = _pRowCounts(*m_pLogicUnit->m_xpRowCounts);

  // These are pushed in reverse order so they can be added to the __affectedRows infoset
  m_pDataStack->vPushInt(pRowCounts->nRowsDeleted());
  m_pDataStack->vPushInt(pRowCounts->nRowsUpdated());
  m_pDataStack->vPushInt(pRowCounts->nRowsInserted());
  m_pDataStack->vPushInt(pRowCounts->nRowsInvalid());
  m_pDataStack->vPushInt(pRowCounts->nRowsSkipped());
  m_pDataStack->vPushInt(pRowCounts->nRowsProcessed());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCounts[].getNumCounters -- push the number of counter values collected  */
/*----------------------------------------------------------------------------*/
void Step::vRowCountsGetNumCounters(void)
{
  ENTRY();

  m_pDataStack->vPushInt(6);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
