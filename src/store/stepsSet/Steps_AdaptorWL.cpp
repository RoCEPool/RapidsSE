/*============================================================================*/
/* Steps_AdaptorWL.cpp - Steps for AdaptorWL register in LogicUnit            */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LogicUnit.hpp"
//#include "AdaptorWL.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_AdaptorWL(void)
{
  vAddStep("AdaptorWL[].processEventA"_Stz,  &Step::vAdaptorProcessEventA);
  vAddStep("AdaptorWL[].action1"_Stz,  &Step::vAdaptorWLAction1);
  vAddStep("AdaptorWL[].action2"_Stz,  &Step::vAdaptorWLAction2);
  
  vAddStep("AdaptorWL[].delete"_Stz,   &Step::vAdaptorWLDelete);
  vAddStep("AdaptorWL[].new"_Stz,      &Step::vAdaptorWLNew);
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].action0 -- do wireline adaptor action 0                        */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorProcessEventA(void)
{
  ENTRY();

  m_pLogicUnit->m_xpAdaptorWL->vProcessEventA();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].action1 -- do wireline adaptor action 1                        */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLAction1(void)
{
  ENTRY();

  m_pLogicUnit->m_xpAdaptorWL->vAction1();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].action2 -- do wireline adaptor action 2                        */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLAction2(void)
{
  ENTRY();

  m_pLogicUnit->m_xpAdaptorWL->vAction2();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].delete -- delete wire line adaptor                             */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLDelete(void)
{
  ENTRY();

  m_pLogicUnit->m_xpAdaptorWL.vDeleteComponent();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adaptorWL[].new -- create new wire line adaptor                            */
/*----------------------------------------------------------------------------*/
void Step::vAdaptorWLNew(void)
{
  ENTRY();
          
  m_pLogicUnit->m_xpAdaptorWL = new AdaptorWL;

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
