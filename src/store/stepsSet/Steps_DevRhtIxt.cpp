/*============================================================================*/
/* Steps_DevRhtIxt.cpp -- DevRhtIxt step group                                */
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
void Step::vAddSteps_DevRhtIxt(void)
{
  vAddStep("DevRhtIxt.indexTree!"_Stz,         &Step::vDevRhtIxtIndexTreeStore);
  vAddStep("DevRhtIxt.crwBuffer!"_Stz,         &Step::vDevRhtIxtCrwBufferStore);
}

/*----------------------------------------------------------------------------*/
/* DevRhtIxt.crwBuffer! -- store crwBuffer in DevSrcRow                       */
/*----------------------------------------------------------------------------*/
void Step::vDevRhtIxtCrwBufferStore(void)
{
  ENTRY();

  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));

  ( m_p_TaskLogicUnit )->m_pDevRhtIxt->m_pCrwBuffer = pCrwBuffer;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DevRhtIxt.indexTree! -- store indexTree in src devIxt                      */
/*----------------------------------------------------------------------------*/
void Step::vDevRhtIxtIndexTreeStore(void)
{
  ENTRY();
  
  IndexTree* pIndexTree = _pIndexTree(m_pDataStack->pPopPtr(__func__));

  ( m_p_TaskLogicUnit )->m_pDevRhtIxt->m_pIdxTree = pIndexTree;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
