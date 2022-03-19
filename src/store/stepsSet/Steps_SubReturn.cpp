/*============================================================================*/
/* Steps_SubReturn.cpp -- SubReturn step group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LogicUnit.hpp"
#include "SubReturn.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SubReturn(void)
{
  vAddStep("SubReturn[]"_Stz,     &Step::vSubReturnGet);
  vAddStep("SubReturn[].get"_Stz, &Step::vSubReturnGet);
  vAddStep("SubReturn[].set"_Stz, &Step::vSubReturnSet);
}

/*----------------------------------------------------------------------------*/
/* SubReturn[].get -- get subReturn                                           */
/*----------------------------------------------------------------------------*/
void Step::vSubReturnGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpSubReturn);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubReturn[].set -- set subRetrun                                           */
/*----------------------------------------------------------------------------*/
void Step::vSubReturnSet(void)
{
  ENTRY();

  SubReturn* pSubReturn = _pSubReturn(m_pDataStack->pPopPtr(__func__));

  m_pLogicUnit->m_xpSubReturn = pSubReturn;

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
