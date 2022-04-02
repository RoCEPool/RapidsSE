/*============================================================================*/
/* Steps_AnswerWL.cpp - Steps for AnswerWL register in LogicUnit            */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LogicUnit.hpp"
//#include "AnswerWL.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_AnswerWL(void)
{
  vAddStep("AnswerWL[].processEventA"_Stz,  &Step::vAnswerProcessEventA);
  vAddStep("AnswerWL[].processEventB"_Stz,  &Step::vAnswerProcessEventB);
  vAddStep("AnswerWL[].action1"_Stz,  &Step::vAnswerWLAction1);
  vAddStep("AnswerWL[].action2"_Stz,  &Step::vAnswerWLAction2);

  vAddStep("AnswerWL[].delete"_Stz,   &Step::vAnswerWLDelete);
  vAddStep("AnswerWL[].new"_Stz,      &Step::vAnswerWLNew);
}

/*----------------------------------------------------------------------------*/
/* answerWL[].processEventA -- do wireline recv action A                  */
/*----------------------------------------------------------------------------*/
void Step::vAnswerProcessEventA(void)
{
  ENTRY();

  m_pLogicUnit->m_xpAnswerWL->vProcessEventA();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* answerWL[].processEventB -- do wireline recv action B                  */
/*----------------------------------------------------------------------------*/
void Step::vAnswerProcessEventB(void)
{
  ENTRY();

  //m_pLogicUnit->m_xpAnswerWL->vProcessEventB();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* answerWL[].action1 -- do wireline recv action 1                        */
/*----------------------------------------------------------------------------*/
void Step::vAnswerWLAction1(void)
{
  ENTRY();

  m_pLogicUnit->m_xpAnswerWL->vAction1();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* answerWL[].action2 -- do wireline recv action 2                        */
/*----------------------------------------------------------------------------*/
void Step::vAnswerWLAction2(void)
{
  ENTRY();

  m_pLogicUnit->m_xpAnswerWL->vAction2();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* answerWL[].delete -- delete wire line recv                             */
/*----------------------------------------------------------------------------*/
void Step::vAnswerWLDelete(void)
{
  ENTRY();

  m_pLogicUnit->m_xpAnswerWL.vDeleteComponent();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* answerWL[].new -- create new wire line recv                            */
/*----------------------------------------------------------------------------*/
void Step::vAnswerWLNew(void)
{
  ENTRY();

  m_pLogicUnit->m_xpAnswerWL = new AnswerWL;

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
