/*============================================================================*/
/* Steps_DevLftRow.cpp -- DevLftRow step group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "EachRowBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"
#include "StageSpec.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_DevLftRow(void)
{
  vAddStep("DevLftRow.deleteEachRowBuffer"_Stz, &Step::vDevLftRowDeleteEachRowBuffer);
  vAddStep("DevLftRow.eachRowBuffer!"_Stz,      &Step::vDevLftRowEachRowBufferStore);
  vAddStep("DevLftRow.row!"_Stz,                &Step::vDevLftRowRowStore);
  vAddStep("DevLftRow.rowBuffer!"_Stz,          &Step::vDevLftRowRowBufferStore);
  vAddStep("DevLftRow.streamDef!"_Stz,          &Step::vDevLftRowStreamDefStore);
}

/*----------------------------------------------------------------------------*/
/* DevLftRow.deleteEachRowBuffer -- delete eachRowBuffer in DevLftRow         */
/*----------------------------------------------------------------------------*/
void Step::vDevLftRowDeleteEachRowBuffer(void)
{
  ENTRY();

  EachRowBuffer* pEachRowBuffer = ( m_pJob->pLogicUnit_job() )->m_pDevLftRow->m_pEachRowBuffer;
  delete pEachRowBuffer;

  ( m_pJob->pLogicUnit_job() )->m_pDevLftRow->m_pEachRowBuffer = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DevLftRow.eachRowBuffer! -- store eachRowBuffer in DevLftRow               */
/*----------------------------------------------------------------------------*/
void Step::vDevLftRowEachRowBufferStore(void)
{
  ENTRY();

  EachRowBuffer* pEachRowBuffer = _pEachRowBuffer(m_pDataStack->pPopPtr(__func__));

  ( m_pJob->pLogicUnit_job() )->m_pDevLftRow->m_pEachRowBuffer = pEachRowBuffer;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DevLftRow.row! -- store row in DevLftRow                                   */
/*----------------------------------------------------------------------------*/
void Step::vDevLftRowRowStore(void)
{
  ENTRY();

  EachRowBuffer* pEachRowBuffer = _pEachRowBuffer(m_pDataStack->pPopPtr(__func__));

  ( m_pJob->pLogicUnit_job() )->m_pDevLftRow->m_aRow = pEachRowBuffer->pRowAddress(0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DevLftRow.rowBuffer! -- store rowBuffer in DevLftRow                       */
/*----------------------------------------------------------------------------*/
void Step::vDevLftRowRowBufferStore(void)
{
  ENTRY();

  RowBuffer* pRowBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));

  ( m_pJob->pLogicUnit_job() )->m_pDevLftRow->m_pRowBuffer = pRowBuffer;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DevLftRow.streamDef! -- store streamDef in src devRow                      */
/*----------------------------------------------------------------------------*/
void Step::vDevLftRowStreamDefStore(void)
{
  ENTRY();

  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  ( m_pJob->pLogicUnit_job() )->m_pDevLftRow->m_pStreamDef = pStreamDef;

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
