/*============================================================================*/
/* Steps_RegIndex.cpp - index[] step group                                    */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Index.hpp"

#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegIndex(void)
{
  vAddStep("index[]"_Stz,      &Step::vRegIndexGet);
  vAddStep("index[].set"_Stz,  &Step::vRegIndexSet);
}

/*----------------------------------------------------------------------------*/
/* index[] -- get index                                                       */
/*----------------------------------------------------------------------------*/
void Step::vRegIndexGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*( m_pJob->pLogicUnit_job() )->m_xpIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index[].set -- set index                                                   */
/*----------------------------------------------------------------------------*/
void Step::vRegIndexSet(void)
{
  ENTRY();
  
  ( m_pJob->pLogicUnit_job() )->m_xpIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
