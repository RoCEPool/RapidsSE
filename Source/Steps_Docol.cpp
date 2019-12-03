/*============================================================================*/
/* Steps_Docol.cpp - docol step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Docol(void)
{
  vAddStep("]"_Stz,                       &Step::vDocolCompiling);
  vAddStep("["_Stz,                       &Step::vDocolInterpreting,   true);
  
  vAddStep("docol::currentWord@"_Stz,     &Step::vDocolCurrentWordFetch);
  vAddStep("docol::currentWord!"_Stz,     &Step::vDocolCurrentWordStore);

  vAddStep("docol::isCompiling"_Stz,      &Step::vDocolIsCompiling);
  vAddStep("docol::isInterpreting"_Stz,   &Step::vDocolIsInterpreting);
}

/*----------------------------------------------------------------------------*/
/* ] -- enter compiler state                                                  */
/*----------------------------------------------------------------------------*/
void Step::vDocolCompiling(void)
{
  ENTRY(); // ENTRY("vDocolCompiling"_Stz);
  
  m_pCompiler->vCompiling(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* docol::currentWord@ -- get current word                                    */
/*----------------------------------------------------------------------------*/
void Step::vDocolCurrentWordFetch(void)
{
  ENTRY(); // ENTRY("vDocolCurrentWordFetch"_Stz);
  
  Word* pCurrWord = m_pCompiler->pCurrentWord();
  
  m_pDataStack->vPushPtr(pCurrWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* docol::currentWord! -- put current word                                    */
/*----------------------------------------------------------------------------*/
void Step::vDocolCurrentWordStore(void)
{
  ENTRY(); // ENTRY("vDocolCurrentWordStore"_Stz);
  
  Word* pCurrWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  m_pCompiler->vCurrentWord(pCurrWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ -- enter interpreter state                                               */
/*----------------------------------------------------------------------------*/
void Step::vDocolInterpreting(void)
{
  ENTRY(); // ENTRY("vDocolInterpreting"_Stz);
  
  m_pCompiler->vCompiling(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* docol::isCompiling -- currently in compiling mode?                         */
/*----------------------------------------------------------------------------*/
void Step::vDocolIsCompiling(void)
{
  ENTRY(); // ENTRY("vDocolIsCompiling"_Stz);
  
  i8 iIsCompiling = m_pCompiler->bCompiling() ? 1 : 0;
  
  m_pDataStack->vPushInt(iIsCompiling);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* docol::isInterpreting -- currently in interpreting mode?                   */
/*----------------------------------------------------------------------------*/
void Step::vDocolIsInterpreting(void)
{
  ENTRY(); // ENTRY("vDocolIsInterpreting"_Stz);
  
  i8 iIsInterpreting = m_pCompiler->bCompiling() ? 0 : 1;
  
  m_pDataStack->vPushInt(iIsInterpreting);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
