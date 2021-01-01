/*============================================================================*/
/* Steps_RegIndex.cpp - index[] step group                                    */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"


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
  
  m_pDataStack->vPushPtr(*pLogicUnit_task()->m_xpIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index[].set -- set index                                                   */
/*----------------------------------------------------------------------------*/
void Step::vRegIndexSet(void)
{
  ENTRY();
  
  pLogicUnit_task()->m_xpIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
