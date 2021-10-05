/*============================================================================*/
/* Steps_DevRhtRow.cpp -- DevRhtRow step group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

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
void Step::vAddSteps_DevRhtRow(void)
{
  vAddStep("DevRhtRow.rowBuffer!"_Stz,          &Step::vDevRhtRowRowBufferStore);
  vAddStep("DevRhtRow.streamDef!"_Stz,          &Step::vDevRhtRowStreamDefStore);
}

/*----------------------------------------------------------------------------*/
/* DevRhtRow.rowBuffer! -- store rowBuffer in DevLftRow                       */
/*----------------------------------------------------------------------------*/
void Step::vDevRhtRowRowBufferStore(void)
{
  ENTRY();

  RowBuffer* pRowBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));

  ( m_p_TaskLogicUnit )->m_pDevRhtRow->m_pRowBuffer = pRowBuffer;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DevRhtRow.streamDef! -- store streamDef in src devRow                      */
/*----------------------------------------------------------------------------*/
void Step::vDevRhtRowStreamDefStore(void)
{
  ENTRY();
  
  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  ( m_p_TaskLogicUnit )->m_pDevRhtRow->m_pStreamDef = pStreamDef;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
