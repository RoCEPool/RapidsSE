/*============================================================================*/
/* Steps_LogicCompiler.cpp -- LogicCompiler step group                        */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_LogicCompiler(void)
{
  vAddStep("LogicUnit[].delete"_Stz,    &Step::vLogicCompilerDelete);
  vAddStep("LogicUnit[].new"_Stz,       &Step::vLogicCompilerNew);
}

/*----------------------------------------------------------------------------*/
/* LogicCompiler[].delete -- delete logic compiler                            */
/*----------------------------------------------------------------------------*/
void Step::vLogicCompilerDelete(void)
{
  ENTRY();
      
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicCompiler[].new -- create a new logic compiler                         */
/*----------------------------------------------------------------------------*/
void Step::vLogicCompilerNew(void)
{
  ENTRY();
  
  LogicCompiler* pLogicCompiler = new LogicCompiler;

  m_pDataStack->vPushPtr(pLogicCompiler);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
