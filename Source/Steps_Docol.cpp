/*============================================================================*/
/* Steps_Docol.cpp - docol step group                                         */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
void Step::vDocolCompiling(MP_Task mpTask)
{
  ENTRY("vDocolCompiling"_Stz);
  
  mpTask->m_pCompiler->m_bCompiling = true;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ -- enter interpreter state                                               */
/*----------------------------------------------------------------------------*/
void Step::vDocolInterpreting(MP_Task mpTask)
{
  ENTRY("vDocolInterpreting"_Stz);
  
  mpTask->m_pCompiler->m_bCompiling = false;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* docol.isCompiling -- currently in compiling mode?                          */
/*----------------------------------------------------------------------------*/
void Step::vDocolIsCompiling(MP_Task mpTask)
{
  ENTRY("vDocolIsCompiling"_Stz);
  
  i8 iIsCompiling = mpTask->m_pCompiler->m_bCompiling ? 1 : 0;
  
  mpTask->m_pDataStack->vPushI8(iIsCompiling);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* docol.isInterpreting -- currently in interpreting mode?                    */
/*----------------------------------------------------------------------------*/
void Step::vDocolIsInterpreting(MP_Task mpTask)
{
  ENTRY("vDocolIsInterpreting"_Stz);
  
  i8 iIsInterpreting = mpTask->m_pCompiler->m_bCompiling ? 0 : 1;
  
  mpTask->m_pDataStack->vPushI8(iIsInterpreting);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
