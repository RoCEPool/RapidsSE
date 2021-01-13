/*============================================================================*/
/* Steps_SenCompiler.cpp - compiler step group                                */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SqlCompiler(void)
{
  vAddStep("compiler::isCompiling"_Stz,    &Step::vSqlCompilerIsCompiling);
  vAddStep("compiler::isDiscarding"_Stz,   &Step::vSqlCompilerIsDiscarding);
  vAddStep("compiler::isInterpreting"_Stz, &Step::vSqlCompilerIsInterpreting);
  vAddStep("compiler::isPostponing"_Stz,   &Step::vSqlCompilerIsPostponing);
  
  vAddStep("compiler::compiling"_Stz,      &Step::vSqlCompilerCompiling);
  vAddStep("compiler::interpreting"_Stz,   &Step::vSqlCompilerInterpreting);
  vAddStep("compiler::postponing"_Stz,     &Step::vSqlCompilerPostponing);

  vAddStep("compiler::current@"_Stz,       &Step::vSqlCompilerCurrentFetch);
  vAddStep("compiler::current!"_Stz,       &Step::vSqlCompilerCurrentStore);
  
  vAddStep("compiler::nested@"_Stz,        &Step::vSqlCompilerNestedFetch);
  vAddStep("compiler::nested!"_Stz,        &Step::vSqlCompilerNestedStore);

  
  vAddStep("["_Stz,     &Step::vSqlCompilerInterpreting,    true);
  vAddStep("]"_Stz,     &Step::vSqlCompilerCompiling);
  
  vAddStep("[["_Stz,    &Step::vSqlCompilerPostponing,      true);
  vAddStep("]]"_Stz,    &Step::vSqlCompilerCompiling);
  
  vAddStep("[?"_Stz,    &Step::vSqlCompilerCondBegin,       true);
  vAddStep("?if"_Stz,   &Step::vSqlCompilerCondTest);
  vAddStep("?else"_Stz, &Step::vSqlCompilerCondElse,        true);
  vAddStep("?]"_Stz,    &Step::vSqlCompilerCondEnd,         true);
}


/*----------------------------------------------------------------------------*/
/*                           - TEST WHICH MODE -                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compiler::isCompiling -- currently in compiling mode?                      */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerIsCompiling(void)
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
void Step::vSqlCompilerIsDiscarding(void)
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
void Step::vSqlCompilerIsInterpreting(void)
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
void Step::vSqlCompilerIsPostponing(void)
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
void Step::vSqlCompilerCompiling(void)
{
  ENTRY();
  
  m_pCompiler->vMode(CM_COMPILING);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::discarding -- change to discarding mode                          */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerDiscarding(void)
{
  ENTRY();
  
  m_pCompiler->vMode(CM_DISCARDING);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::interpreting -- change to interpreting mode                      */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerInterpreting(void)
{
  ENTRY();
  
  m_pCompiler->vMode(CM_INTERPRETING);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::postponing -- change to postponing mode                          */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerPostponing(void)
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
void Step::vSqlCompilerCurrentFetch(void)
{
  ENTRY();
    
  Word* pCurrWord = m_pCompiler->pCurrentWord();
    
//  if (pCurrWord->szWordNm()->bSame("testProc"_Stz))
//    BASE::vDebugPoint();

  m_pDataStack->vPushPtr(pCurrWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::current! -- put current word                                     */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerCurrentStore(void)
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
void Step::vSqlCompilerNestedFetch(void)
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
void Step::vSqlCompilerNestedStore(void)
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
void Step::vSqlCompilerCondBegin(void)
{
  ENTRY();
    
  m_pCompiler->vMode(CM_INTERPRETING);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ?if -- conditional compilation test                                        */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerCondTest(void)
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
void Step::vSqlCompilerCondElse(void)
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
void Step::vSqlCompilerCondEnd(void)
{
  ENTRY();
    
  eCM eMode = m_pCompiler->eMode();

  m_pCompiler->vMode(CM_COMPILING);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
