/*============================================================================*/
/* Steps_DevSrcRow.cpp -- DevSrcRow step group                                */
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
void Step::vAddSteps_DevSrcRow(void)
{
  vAddStep("DevSrcRow.streamDef!"_Stz,         &Step::vDevSrcRowStreamDefStore);
}

/*----------------------------------------------------------------------------*/
/* DevSrcRow.streamDef! -- store streamDef in src devRow                      */
/*----------------------------------------------------------------------------*/
void Step::vDevSrcRowStreamDefStore(void)
{
  ENTRY();
  
  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  ( m_pJob->pLogicUnit_job() )->m_pDevSrcRow->m_pStreamDef = pStreamDef;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
