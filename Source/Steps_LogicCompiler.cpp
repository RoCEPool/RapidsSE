/*============================================================================*/
/* Steps_LogicCompiler.cpp -- LogicCompiler step group                        */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_LogicCompiler(void)
{
  vAddStep("LogicCompiler[].compile"_Stz,   &Step::vLogicCompilerCompile);
  vAddStep("LogicCompiler[].delete"_Stz,    &Step::vLogicCompilerDelete);
  vAddStep("LogicCompiler[].new"_Stz,       &Step::vLogicCompilerNew);
}

/*----------------------------------------------------------------------------*/
/* LogicCompiler[].compile -- compile                                         */
/*----------------------------------------------------------------------------*/
void Step::vLogicCompilerCompile(void)
{
  ENTRY();
    
  i8 iResult =
    m_pLogicUnit->m_xpLogicCompiler->iCompile(m_pLogicUnit->m_i_Code,
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
