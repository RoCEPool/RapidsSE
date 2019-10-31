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
void Step::vDocolCompiling(void)
{
  ENTRY("vDocolCompiling"_Stz);
  
  m_pCompiler->m_bCompiling = true;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ -- enter interpreter state                                               */
/*----------------------------------------------------------------------------*/
void Step::vDocolInterpreting(void)
{
  ENTRY("vDocolInterpreting"_Stz);
  
  m_pCompiler->m_bCompiling = false;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* docol.isCompiling -- currently in compiling mode?                          */
/*----------------------------------------------------------------------------*/
void Step::vDocolIsCompiling(void)
{
  ENTRY("vDocolIsCompiling"_Stz);
  
  i8 iIsCompiling = m_pCompiler->m_bCompiling ? 1 : 0;
  
  m_pDataStack->vPushI8(iIsCompiling);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* docol.isInterpreting -- currently in interpreting mode?                    */
/*----------------------------------------------------------------------------*/
void Step::vDocolIsInterpreting(void)
{
  ENTRY("vDocolIsInterpreting"_Stz);
  
  i8 iIsInterpreting = m_pCompiler->m_bCompiling ? 0 : 1;
  
  m_pDataStack->vPushI8(iIsInterpreting);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
