/*============================================================================*/
/* Steps_LogicCompiler.cpp -- LogicCompiler step group                        */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_LogicCompiler(void)
{
  vAddStep("LogicUnit[].compile"_Stz,   &Step::vLogicCompilerCompile);
  vAddStep("LogicUnit[].delete"_Stz,    &Step::vLogicCompilerDelete);
  vAddStep("LogicUnit[].new"_Stz,       &Step::vLogicCompilerNew);
}

/*----------------------------------------------------------------------------*/
/* LogicCompiler[].compile -- compile                                         */
/*----------------------------------------------------------------------------*/
void Step::vLogicCompilerCompile(void)
{
  ENTRY();
    
  m_pLogicUnit->m_xpLogicCompiler->vCompile(m_pLogicUnit->m_i_Code,
                                            m_pLogicUnit->m_eaCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicCompiler[].delete -- delete logic compiler                            */
/*----------------------------------------------------------------------------*/
void Step::vLogicCompilerDelete(void)
{
  ENTRY();
  
  LogicCompiler* pLogicCompiler = * m_pLogicUnit->m_xpLogicCompiler;
  m_pLogicUnit->m_xpLogicCompiler = nullptr;
  
  delete pLogicCompiler;
      
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicCompiler[].new -- create a new logic compiler                         */
/*----------------------------------------------------------------------------*/
void Step::vLogicCompilerNew(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpLogicCompiler = new LogicCompiler;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
