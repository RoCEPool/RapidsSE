/*============================================================================*/
/* Steps_SubCall.cpp -- SubCall step group                                    */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LogicUnit.hpp"
#include "SubCall.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SubCall(void)
{
  vAddStep("SubCall[]"_Stz,     &Step::vSubCallGet);
  vAddStep("SubCall[].get"_Stz, &Step::vSubCallGet);
  vAddStep("SubCall[].set"_Stz, &Step::vSubCallSet);
}

/*----------------------------------------------------------------------------*/
/* SubCall[].get -- get subcall                                               */
/*----------------------------------------------------------------------------*/
void Step::vSubCallGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpSubCall);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubCall[].set -- set subcall                                               */
/*----------------------------------------------------------------------------*/
void Step::vSubCallSet(void)
{
  ENTRY();

  SubCall* pSubCall = _pSubCall(m_pDataStack->pPopPtr(__func__));

  m_pLogicUnit->m_xpSubCall = pSubCall;

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
