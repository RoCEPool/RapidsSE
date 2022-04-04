/*============================================================================*/
/* Steps_SendWL.cpp - Steps for SendWL register in LogicUnit            */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LogicUnit.hpp"
//#include "SendWL.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SendWL(void)
{
  vAddStep("SendWL[].processEventA"_Stz,  &Step::vSendProcessEventA);
  vAddStep("SendWL[].processEventB"_Stz,  &Step::vSendProcessEventB);
  vAddStep("SendWL[].action1"_Stz,  &Step::vSendWLAction1);
  vAddStep("SendWL[].action2"_Stz,  &Step::vSendWLAction2);

  vAddStep("SendWL[].delete"_Stz,   &Step::vSendWLDelete);
  vAddStep("SendWL[].new"_Stz,      &Step::vSendWLNew);
}

/*----------------------------------------------------------------------------*/
/* sendWL[].processEventA -- do wireline recv action A                  */
/*----------------------------------------------------------------------------*/
void Step::vSendProcessEventA(void)
{
  ENTRY();

  m_pLogicUnit->m_xpSendWL->vProcessEventA();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sendWL[].processEventB -- do wireline recv action B                  */
/*----------------------------------------------------------------------------*/
void Step::vSendProcessEventB(void)
{
  ENTRY();

  //m_pLogicUnit->m_xpSendWL->vProcessEventB();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sendWL[].action1 -- do wireline recv action 1                        */
/*----------------------------------------------------------------------------*/
void Step::vSendWLAction1(void)
{
  ENTRY();

  m_pLogicUnit->m_xpSendWL->vAction1();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sendWL[].action2 -- do wireline recv action 2                        */
/*----------------------------------------------------------------------------*/
void Step::vSendWLAction2(void)
{
  ENTRY();

  m_pLogicUnit->m_xpSendWL->vAction2();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sendWL[].delete -- delete wire line recv                             */
/*----------------------------------------------------------------------------*/
void Step::vSendWLDelete(void)
{
  ENTRY();

  m_pLogicUnit->m_xpSendWL.vDeleteComponent();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sendWL[].new -- create new wire line recv                            */
/*----------------------------------------------------------------------------*/
void Step::vSendWLNew(void)
{
  ENTRY();

  m_pLogicUnit->m_xpSendWL = new SendWL;

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

