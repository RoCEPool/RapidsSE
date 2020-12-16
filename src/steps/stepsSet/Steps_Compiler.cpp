/*============================================================================*/
/* Steps_Compiler.cpp - compiler step group                                   */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Compiler(void)
{
  vAddStep("compiler::isCompiling"_Stz,    &Step::vCompilerIsCompiling);
  vAddStep("compiler::isDiscarding"_Stz,   &Step::vCompilerIsDiscarding);
  vAddStep("compiler::isInterpreting"_Stz, &Step::vCompilerIsInterpreting);
  vAddStep("compiler::isPostponing"_Stz,   &Step::vCompilerIsPostponing);
  
  vAddStep("compiler::compiling"_Stz,    &Step::vCompilerCompiling);
  vAddStep("compiler::interpreting"_Stz, &Step::vCompilerInterpreting);
  vAddStep("compiler::postponing"_Stz,   &Step::vCompilerPostponing);

  vAddStep("compiler::current@"_Stz,     &Step::vCompilerCurrentFetch);
  vAddStep("compiler::current!"_Stz,     &Step::vCompilerCurrentStore);
  
  vAddStep("compiler::nested@"_Stz,      &Step::vCompilerNestedFetch);
  vAddStep("compiler::nested!"_Stz,      &Step::vCompilerNestedStore);

  vAddStep("["_Stz,                      &Step::vCompilerInterpreting,    true);
  vAddStep("]"_Stz,                      &Step::vCompilerCompiling);
  
  vAddStep("[["_Stz,                     &Step::vCompilerPostponing,      true);
  vAddStep("]]"_Stz,                     &Step::vCompilerCompiling);
  
  vAddStep("[?"_Stz,                     &Step::vCompilerCondBegin,       true);
  vAddStep("?if"_Stz,                    &Step::vCompilerCondTest);
  vAddStep("?else"_Stz,                  &Step::vCompilerCondElse,        true);
  vAddStep("?]"_Stz,                     &Step::vCompilerCondEnd,         true);
}


/*----------------------------------------------------------------------------*/
/*                           - TEST WHICH MODE -                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compiler::isCompiling -- currently in compiling mode?                      */
/*----------------------------------------------------------------------------*/
void Step::vCompilerIsCompiling(void)
{
  ENTRY();
  
  eCM eMode = m_pCompiler->eMode();
  
  i8 iIsCompiling = (CM_COMPILING == eMode) || (CM_CONDCOMPILING == eMode);
  
  m_pDataStack->vPushInt(iIsCompiling);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::isCompiling -- currently in compiling mode?                      */
/*----------------------------------------------------------------------------*/
void Step::vCompilerIsDiscarding(void)
{
  ENTRY();
  
  eCM eMode = m_pCompiler->eMode();
  
  i8 iIsDiscarding = CM_DISCARDING == eMode;
  
  m_pDataStack->vPushInt(iIsDiscarding);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::isInterpreting -- currently in interpreting mode?                */
/*----------------------------------------------------------------------------*/
void Step::vCompilerIsInterpreting(void)
{
  ENTRY();
  
  eCM eMode = m_pCompiler->eMode();
  
  i8 iIsInterpreting = CM_INTERPRETING == eMode;
  
  m_pDataStack->vPushInt(iIsInterpreting);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::isPostponing -- currently in postponing mode?                    */
/*----------------------------------------------------------------------------*/
void Step::vCompilerIsPostponing(void)
{
  ENTRY();
  
  eCM eMode = m_pCompiler->eMode();
  
  i8 iIsPostponing = CM_POSTPONING == eMode;
  
  m_pDataStack->vPushInt(iIsPostponing);

  EXIT();
}


/*----------------------------------------------------------------------------*/
/*                           - SET WHICH MODE -                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compiler::compiling-- change to compiling mode                             */
/*----------------------------------------------------------------------------*/
void Step::vCompilerCompiling(void)
{
  ENTRY();
  
  m_pCompiler->vMode(CM_COMPILING);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::discarding -- change to discarding mode                          */
/*----------------------------------------------------------------------------*/
void Step::vCompilerDiscarding(void)
{
  ENTRY();
  
  m_pCompiler->vMode(CM_DISCARDING);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::interpreting -- change to interpreting mode                      */
/*----------------------------------------------------------------------------*/
void Step::vCompilerInterpreting(void)
{
  ENTRY();
  
  m_pCompiler->vMode(CM_INTERPRETING);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::postponing -- change to postponing mode                          */
/*----------------------------------------------------------------------------*/
void Step::vCompilerPostponing(void)
{
  ENTRY();
  
  m_pCompiler->vMode(CM_POSTPONING);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/*                        - FETCH / STORE CURRENT WORD -                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compiler::current@ -- fetch current word                                   */
/*----------------------------------------------------------------------------*/
void Step::vCompilerCurrentFetch(void)
{
  ENTRY();
    
  Word* pCurrWord = m_pCompiler->pCurrentWord();
    
//  if (pCurrWord->szWordNm()->bSame("testProc"_Stz))
//    Resources::vDebugPoint();

  m_pDataStack->vPushPtr(pCurrWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::current! -- put current word                                     */
/*----------------------------------------------------------------------------*/
void Step::vCompilerCurrentStore(void)
{
  ENTRY();
  
  Word* pCurrWord = _pWord(m_pDataStack->pPopPtr(__func__));
    
  m_pCompiler->vCurrentWord(pCurrWord);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/*                        - FETCH / STORE NESTED WORD -                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compiler::nested@ -- get nested word                                       */
/*----------------------------------------------------------------------------*/
void Step::vCompilerNestedFetch(void)
{
  ENTRY();
    
  i8 i_Nested = m_pDataStack->iPopInt(__func__);
  
  Word* pNested = m_pCompiler->pNestedWord(i_Nested);
  
  m_pDataStack->vPushPtr(pNested);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::nested! -- set nested word                                       */
/*----------------------------------------------------------------------------*/
void Step::vCompilerNestedStore(void)
{
  ENTRY();
  
  Word* pNested = _pWord(m_pDataStack->pPopPtr(__func__));
  
  i8 i_Nested = m_pDataStack->iPopInt(__func__);
    
  m_pCompiler->vNestedWord(i_Nested,pNested);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*                        - CONDITIONAL COMPILATION -                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* [? -- conditional compilation begin                                        */
/*----------------------------------------------------------------------------*/
void Step::vCompilerCondBegin(void)
{
  ENTRY();
    
  m_pCompiler->vMode(CM_INTERPRETING);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ?if -- conditional compilation test                                        */
/*----------------------------------------------------------------------------*/
void Step::vCompilerCondTest(void)
{
  ENTRY();
  
  i8 iCondTest = m_pDataStack->iPopInt(__func__);
  
  if (iCondTest)
    m_pCompiler->vMode(CM_CONDCOMPILING);
  else
    m_pCompiler->vMode(CM_DISCARDING);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ?else -- conditional compilation else                                      */
/*----------------------------------------------------------------------------*/
void Step::vCompilerCondElse(void)
{
  ENTRY();
   
  eCM eMode = m_pCompiler->eMode();

  if (CM_CONDCOMPILING == eMode)
    m_pCompiler->vMode(CM_DISCARDING);
  else
  if (CM_DISCARDING == eMode)
    m_pCompiler->vMode(CM_CONDCOMPILING);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ?] -- conditional compilation end                                          */
/*----------------------------------------------------------------------------*/
void Step::vCompilerCondEnd(void)
{
  ENTRY();
    
  eCM eMode = m_pCompiler->eMode();

  m_pCompiler->vMode(CM_COMPILING);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
