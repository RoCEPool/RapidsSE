/*============================================================================*/
/* Steps_DevLftIxt.cpp -- DevLftIxt step group                                */
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
void Step::vAddSteps_DevLftIxt(void)
{
  vAddStep("DevLftIxt.indexTree!"_Stz,         &Step::vDevLftIxtIndexTreeStore);
  vAddStep("DevLftIxt.crwBuffer@"_Stz,         &Step::vDevLftIxtCrwBufferFetch);
  vAddStep("DevLftIxt.crwBuffer!"_Stz,         &Step::vDevLftIxtCrwBufferStore);
}

/*----------------------------------------------------------------------------*/
/* DevLftIxt.crwBuffer@ -- fetch crwBuffer in DevSrcRow                       */
/*----------------------------------------------------------------------------*/
void Step::vDevLftIxtCrwBufferFetch(void)
{
  ENTRY();

  CrwBuffer* pCrwBuffer = ( m_pLogicUnit )->m_pDevLftIxt->m_pCrwBuffer;
  m_pDataStack->vPushPtr(pCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DevLftIxt.crwBuffer! -- store crwBuffer in DevSrcRow                       */
/*----------------------------------------------------------------------------*/
void Step::vDevLftIxtCrwBufferStore(void)
{
  ENTRY();

  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));

  ( m_pLogicUnit )->m_pDevLftIxt->m_pCrwBuffer = pCrwBuffer;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DevLftIxt.indexTree! -- store indexTree in src devIxt                      */
/*----------------------------------------------------------------------------*/
void Step::vDevLftIxtIndexTreeStore(void)
{
  ENTRY();
  
  IndexTree* pIndexTree = _pIndexTree(m_pDataStack->pPopPtr(__func__));

  ( m_pLogicUnit )->m_pDevLftIxt->m_pIdxTree = pIndexTree;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
