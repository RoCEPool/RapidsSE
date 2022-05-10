/*============================================================================*/
/* Steps_SenCompiler.cpp - compiler step group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SenCompiler(void)
{
  vAddStep("senCompiler::isCompiling"_Stz,    &Step::vSenCompilerIsCompiling);
  vAddStep("senCompiler::isDiscarding"_Stz,   &Step::vSenCompilerIsDiscarding);
  vAddStep("senCompiler::isInterpreting"_Stz, &Step::vSenCompilerIsInterpreting);
  vAddStep("senCompiler::isPostponing"_Stz,   &Step::vSenCompilerIsPostponing);
  
  vAddStep("senCompiler::compiling"_Stz,      &Step::vSenCompilerCompiling);
  vAddStep("senCompiler::interpreting"_Stz,   &Step::vSenCompilerInterpreting);
  vAddStep("senCompiler::postponing"_Stz,     &Step::vSenCompilerPostponing);

  vAddStep("senCompiler::current@"_Stz,       &Step::vSenCompilerCurrentFetch);
  vAddStep("senCompiler::current!"_Stz,       &Step::vSenCompilerCurrentStore);
  vAddStep("senCompiler::locals@"_Stz,        &Step::vSenCompilerLocalsFetch);
  vAddStep("senCompiler::nested@"_Stz,        &Step::vSenCompilerNestedFetch);
  vAddStep("senCompiler::nested!"_Stz,        &Step::vSenCompilerNestedStore);

  
  vAddStep("["_Stz,     &Step::vSenCompilerInterpreting,    true);
  vAddStep("]"_Stz,     &Step::vSenCompilerCompiling);
  
  vAddStep("[["_Stz,    &Step::vSenCompilerPostponing,      true);
  vAddStep("]]"_Stz,    &Step::vSenCompilerCompiling);
  
  vAddStep("[?"_Stz,    &Step::vSenCompilerCondBegin,       true);
  vAddStep("?if"_Stz,   &Step::vSenCompilerCondTest);
  vAddStep("?else"_Stz, &Step::vSenCompilerCondElse,        true);
  vAddStep("?]"_Stz,    &Step::vSenCompilerCondEnd,         true);
}


/*----------------------------------------------------------------------------*/
/*                           - TEST WHICH MODE -                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* senCompiler::isCompiling -- currently in compiling mode?                   */
/*----------------------------------------------------------------------------*/
void Step::vSenCompilerIsCompiling(void)
{
  ENTRY();
  
  eCM eMode = m_pSenCompiler->eMode();
  
  i8 iIsCompiling = (CM_COMPILING == eMode) || (CM_CONDCOMPILING == eMode);
  
  m_pDataStack->vPushInt(iIsCompiling);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* senCompiler::isCompiling -- currently in compiling mode?                   */
/*----------------------------------------------------------------------------*/
void Step::vSenCompilerIsDiscarding(void)
{
  ENTRY();
  
  eCM eMode = m_pSenCompiler->eMode();
  
  i8 iIsDiscarding = CM_DISCARDING == eMode;
  
  m_pDataStack->vPushInt(iIsDiscarding);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* senCompiler::isInterpreting -- currently in interpreting mode?             */
/*----------------------------------------------------------------------------*/
void Step::vSenCompilerIsInterpreting(void)
{
  ENTRY();
  
  eCM eMode = m_pSenCompiler->eMode();
  
  i8 iIsInterpreting = CM_INTERPRETING == eMode;
  
  m_pDataStack->vPushInt(iIsInterpreting);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* senCompiler::isPostponing -- currently in postponing mode?                 */
/*----------------------------------------------------------------------------*/
void Step::vSenCompilerIsPostponing(void)
{
  ENTRY();
  
  eCM eMode = m_pSenCompiler->eMode();
  
  i8 iIsPostponing = CM_POSTPONING == eMode;
  
  m_pDataStack->vPushInt(iIsPostponing);

  EXIT();
}


/*----------------------------------------------------------------------------*/
/*                           - SET WHICH MODE -                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* senCompiler::compiling-- change to compiling mode                          */
/*----------------------------------------------------------------------------*/
void Step::vSenCompilerCompiling(void)
{
  ENTRY();
  
  m_pSenCompiler->vMode(CM_COMPILING);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* senCompiler::discarding -- change to discarding mode                       */
/*----------------------------------------------------------------------------*/
void Step::vSenCompilerDiscarding(void)
{
  ENTRY();
  
  m_pSenCompiler->vMode(CM_DISCARDING);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* senCompiler::interpreting -- change to interpreting mode                   */
/*----------------------------------------------------------------------------*/
void Step::vSenCompilerInterpreting(void)
{
  ENTRY();
  
  m_pSenCompiler->vMode(CM_INTERPRETING);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* senCompiler::postponing -- change to postponing mode                       */
/*----------------------------------------------------------------------------*/
void Step::vSenCompilerPostponing(void)
{
  ENTRY();
  
  m_pSenCompiler->vMode(CM_POSTPONING);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/*                        - FETCH / STORE CURRENT WORD -                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* senCompiler::current@ -- fetch current word                                */
/*----------------------------------------------------------------------------*/
void Step::vSenCompilerCurrentFetch(void)
{
  ENTRY();
    
  Word* pCurrWord = m_pSenCompiler->pCurrentWord();
    
//  if (pCurrWord->szWordNm()->bSame("testProc"_Stz))
//    BASE::vDebugPoint();

  m_pDataStack->vPushPtr(pCurrWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* senCompiler::current! -- put current word                                  */
/*----------------------------------------------------------------------------*/
void Step::vSenCompilerCurrentStore(void)
{
  ENTRY();
  
  Word* pCurrWord = _pWord(m_pDataStack->pPopPtr(__func__));
    
  m_pSenCompiler->vCurrentWord(pCurrWord);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/*                        - FETCH / STORE NESTED WORD -                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* senCompiler::locals@ -- get number of locals                               */
/*----------------------------------------------------------------------------*/
void Step::vSenCompilerLocalsFetch(void)
{
  ENTRY();
    
  i8 nLocals = m_pSenCompiler->nLocals();
  
  m_pDataStack->vPushInt(nLocals);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* senCompiler::nested@ -- get nested word                                    */
/*----------------------------------------------------------------------------*/
void Step::vSenCompilerNestedFetch(void)
{
  ENTRY();
    
  i8 i_Nested = m_pDataStack->iPopInt(__func__);
  
  Word* pNested = m_pSenCompiler->pNestedWord(i_Nested);
  
  m_pDataStack->vPushPtr(pNested);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* senCompiler::nested! -- set nested word                                    */
/*----------------------------------------------------------------------------*/
void Step::vSenCompilerNestedStore(void)
{
  ENTRY();
  
  Word* pNested = _pWord(m_pDataStack->pPopPtr(__func__));
  
  i8 i_Nested = m_pDataStack->iPopInt(__func__);
    
  m_pSenCompiler->vNestedWord(i_Nested,pNested);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*                        - CONDITIONAL COMPILATION -                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* [? -- conditional compilation begin                                        */
/*----------------------------------------------------------------------------*/
void Step::vSenCompilerCondBegin(void)
{
  ENTRY();
    
  m_pSenCompiler->vMode(CM_INTERPRETING);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ?if -- conditional compilation test                                        */
/*----------------------------------------------------------------------------*/
void Step::vSenCompilerCondTest(void)
{
  ENTRY();
  
  i8 iCondTest = m_pDataStack->iPopInt(__func__);
  
  if (iCondTest)
    m_pSenCompiler->vMode(CM_CONDCOMPILING);
  else
    m_pSenCompiler->vMode(CM_DISCARDING);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ?else -- conditional compilation else                                      */
/*----------------------------------------------------------------------------*/
void Step::vSenCompilerCondElse(void)
{
  ENTRY();
   
  eCM eMode = m_pSenCompiler->eMode();

  if (CM_CONDCOMPILING == eMode)
    m_pSenCompiler->vMode(CM_DISCARDING);
  else
  if (CM_DISCARDING == eMode)
    m_pSenCompiler->vMode(CM_CONDCOMPILING);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ?] -- conditional compilation end                                          */
/*----------------------------------------------------------------------------*/
void Step::vSenCompilerCondEnd(void)
{
  ENTRY();
    
  eCM eMode = m_pSenCompiler->eMode();
  UNUSED(eMode);

  m_pSenCompiler->vMode(CM_COMPILING);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
