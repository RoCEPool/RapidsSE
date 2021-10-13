/*============================================================================*/
/* Steps_TinyC.cpp -- TinyC step group                                        */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LogicUnit.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TinyC(void)
{
  vAddStep("TinyC[].compile"_Stz,      &Step::vTinyCCompile);
  vAddStep("TinyC[].delete"_Stz,       &Step::vTinyCDelete);
  vAddStep("TinyC[].new"_Stz,          &Step::vTinyCNew);
}

/*----------------------------------------------------------------------------*/
/* TinyC[].compile -- compile given source using TinyC                        */
/*----------------------------------------------------------------------------*/
void Step::vTinyCCompile(void)
{
  ENTRY();
    
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* TinyC[].delete -- delete TinyC                                             */
/*----------------------------------------------------------------------------*/
void Step::vTinyCDelete(void)
{
  ENTRY();
    
  delete * m_p_TaskLogicUnit->m_xpTinyC;

  m_p_TaskLogicUnit->m_xpTinyC = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TinyC[].new -- new TinyC                                                   */
/*----------------------------------------------------------------------------*/
void Step::vTinyCNew(void)
{
  ENTRY();
    
  m_p_TaskLogicUnit->m_xpTinyC = new TinyC;

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
