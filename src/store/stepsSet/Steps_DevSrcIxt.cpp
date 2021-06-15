/*============================================================================*/
/* Steps_DevSrcIxt.cpp -- DevSrcIxt step group                                */
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
#include "CrwBuffer.hpp"
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
void Step::vAddSteps_DevSrcIxt(void)
{
  vAddStep("DevSrcIxt.indexTree!"_Stz,         &Step::vDevSrcIxtIndexTreeStore);
  vAddStep("DevSrcIxt.crwBuffer!"_Stz,         &Step::vDevSrcIxtCrwBufferStore);
}

/*----------------------------------------------------------------------------*/
/* DevSrcIxt.crwBuffer! -- store crwBuffer in DevSrcRow                       */
/*----------------------------------------------------------------------------*/
void Step::vDevSrcIxtCrwBufferStore(void)
{
  ENTRY();

  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));

  ( m_pJob->pLogicUnit_job() )->m_pDevSrcIxt->m_pCrwBuffer = pCrwBuffer;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DevSrcIxt.indexTree! -- store indexTree in src devIxt                      */
/*----------------------------------------------------------------------------*/
void Step::vDevSrcIxtIndexTreeStore(void)
{
  ENTRY();
  
  IndexTree* pIndexTree = _pIndexTree(m_pDataStack->pPopPtr(__func__));

  ( m_pJob->pLogicUnit_job() )->m_pDevSrcIxt->m_pIdxTree = pIndexTree;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
