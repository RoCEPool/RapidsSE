/*============================================================================*/
/* Steps_Compiler.cpp - compiler step group                                   */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Compiler(void)
{
  vAddStep("compiler::current"_Stz,  &Step::vCompilerCurrent);
}

/*----------------------------------------------------------------------------*/
/* compiler::current -- push word being compiled                              */
/*----------------------------------------------------------------------------*/
void Step::vCompilerCurrent(Forth* pForth)
{
  ENTRY("vCompilerCurrent"_Stz);
  
  Word* pWord = pForth->m_pTask->m_pCompiler->m_pCurrent;
  
  pForth->m_pDataStack->vPushPointer(pWord);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
