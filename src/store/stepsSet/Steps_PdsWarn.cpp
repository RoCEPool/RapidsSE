/*============================================================================*/
/* Steps_PdsWarn.cpp - PdsWarn step group                                     */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"

#include "LglPtnMap.hpp"

#include "LogicUnit.hpp"
#include "Index.hpp"
#include "Table.hpp"
#include "Node.hpp"
#include "Grid.hpp"
#include "Op.hpp"
#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PdsWarn(void)
{
  vAddStep("PdsWarn[].delete"_Stz,          &Step::vPdsWarnDelete);
  vAddStep("PdsWarn[].set"_Stz,             &Step::vPdsWarnSet);
}

/*----------------------------------------------------------------------------*/
/* PdsWarn[].delete -- delete the buffer                                       */
/*----------------------------------------------------------------------------*/
void Step::vPdsWarnDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_pLogicUnit->m_xpPdsWarn,nullptr);

  m_pLogicUnit->m_xpPdsWarn = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsWarn[].set -- set operand                                                */
/*----------------------------------------------------------------------------*/
void Step::vPdsWarnSet(void)
{
  ENTRY();

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  ASSERT(! pOperand || m_pLogicUnit->m_xpPdsWarn.bIsNull());

  m_pLogicUnit->m_xpPdsWarn = _pPds(pOperand);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
