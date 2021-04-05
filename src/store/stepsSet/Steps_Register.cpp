/*============================================================================*/
/* Steps_Register.cpp -- Register step group                                  */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Node_I.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Register(void)
{
  vAddStep("Register0[]"_Stz,       &Step::vRegister0Get);
  vAddStep("Register0[].get"_Stz,   &Step::vRegister0Get);
  vAddStep("Register0[].set"_Stz,   &Step::vRegister0Set);
  
  vAddStep("Register1[]"_Stz,       &Step::vRegister1Get);
  vAddStep("Register1[].get"_Stz,   &Step::vRegister1Get);
  vAddStep("Register1[].set"_Stz,   &Step::vRegister1Set);
  
  vAddStep("Register2[]"_Stz,       &Step::vRegister2Get);
  vAddStep("Register2[].get"_Stz,   &Step::vRegister2Get);
  vAddStep("Register2[].set"_Stz,   &Step::vRegister2Set);
}

/*----------------------------------------------------------------------------*/
/* Register0[].get -- get the value in register0                              */
/*----------------------------------------------------------------------------*/
void Step::vRegister0Get(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pJob->pLogicUnit_job()->m_xpRegister0);
}

/*----------------------------------------------------------------------------*/
/* Register0[].set -- set the value in register0                              */
/*----------------------------------------------------------------------------*/
void Step::vRegister0Set(void)
{
  ENTRY();
  
  m_pJob->pLogicUnit_job()->m_xpRegister0 = m_pDataStack->pPopPtr(__func__);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Register1[].get -- get the value in register1                              */
/*----------------------------------------------------------------------------*/
void Step::vRegister1Get(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pJob->pLogicUnit_job()->m_xpRegister1);
}

/*----------------------------------------------------------------------------*/
/* Register1[].set -- set the value in register1                              */
/*----------------------------------------------------------------------------*/
void Step::vRegister1Set(void)
{
  ENTRY();
  
  m_pJob->pLogicUnit_job()->m_xpRegister1 = m_pDataStack->pPopPtr(__func__);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Register2[].get -- get the value in register2                              */
/*----------------------------------------------------------------------------*/
void Step::vRegister2Get(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pJob->pLogicUnit_job()->m_xpRegister2);
}

/*----------------------------------------------------------------------------*/
/* Register2[].set -- set the value in register2                              */
/*----------------------------------------------------------------------------*/
void Step::vRegister2Set(void)
{
  ENTRY();
  
  m_pJob->pLogicUnit_job()->m_xpRegister2 = m_pDataStack->pPopPtr(__func__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
