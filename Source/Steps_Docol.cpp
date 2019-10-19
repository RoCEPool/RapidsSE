/*============================================================================*/
/* Steps_Docol.cpp - docol step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Docol(void)
{
  vAddStep("]"_Stz,                     &Step::vDocolCompiling);
  vAddStep("["_Stz,                     &Step::vDocolInterpreting,   true );
  vAddStep("docol.isCompiling"_Stz,     &Step::vDocolIsCompiling);
  vAddStep("docol.isInterpreting"_Stz,  &Step::vDocolIsInterpreting);
}

/*----------------------------------------------------------------------------*/
/* ] -- enter compiler state                                                  */
/*----------------------------------------------------------------------------*/
void Step::vDocolCompiling(Forth* pForth)
{
  ENTRY("vDocolCompiling"_Stz);
  
  pForth->m_pTask->m_pCompiler->m_bCompiling = true;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ -- enter interpreter state                                               */
/*----------------------------------------------------------------------------*/
void Step::vDocolInterpreting(Forth* pForth)
{
  ENTRY("vDocolInterpreting"_Stz);
  
  pForth->m_pTask->m_pCompiler->m_bCompiling = false;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* docol.isCompiling -- currently in compiling mode?                          */
/*----------------------------------------------------------------------------*/
void Step::vDocolIsCompiling(Forth* pForth)
{
  ENTRY("vDocolIsCompiling"_Stz);
  
  i8 iIsCompiling = pForth->m_pTask->m_pCompiler->m_bCompiling ? 1 : 0;
  
  pForth->m_pDataStack->vPushI8(iIsCompiling);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* docol.isInterpreting -- currently in interpreting mode?                    */
/*----------------------------------------------------------------------------*/
void Step::vDocolIsInterpreting(Forth* pForth)
{
  ENTRY("vDocolIsInterpreting"_Stz);
  
  i8 iIsInterpreting = pForth->m_pTask->m_pCompiler->m_bCompiling ? 0 : 1;
  
  pForth->m_pDataStack->vPushI8(iIsInterpreting);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
