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
  vAddStep("DevLftRow.streamDef!"_Stz,          &Step::vDevLftRowStreamDefStore);
  vAddStep("DevLftRow.deleteEachRowBuffer"_Stz, &Step::vDevLftRowDeleteEachRowBuffer);
  vAddStep("DevLftRow.eachRowBuffer!"_Stz,      &Step::vDevLftRowEachRowBufferStore);
  vAddStep("DevLftRow.rowBuffer!"_Stz,          &Step::vDevLftRowRowBufferStore);
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
/* DevLftRow.rowBuffer! -- store rowBuffer in DevLftRow                       */
/*----------------------------------------------------------------------------*/
void Step::vDevLftRowRowBufferStore(void)
{
  ENTRY();

  RowBuffer* pRowBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));

  ( m_pJob->pLogicUnit_job() )->m_pDevLftRow->m_pRowBuffer = pRowBuffer;

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
