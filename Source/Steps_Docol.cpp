/*============================================================================*/
/* Steps_Docol.cpp - docol step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// docol: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Docol(void)
{
  vAddStep("]"_Stz,                     &Task::vDocolCompiling,      false);
  vAddStep("["_Stz,                     &Task::vDocolInterpreting,   true );
  vAddStep("docol.isCompiling"_Stz,     &Task::vDocolIsCompiling,    false);
  vAddStep("docol.isInterpreting"_Stz,  &Task::vDocolIsInterpreting, false);
}

/*----------------------------------------------------------------------------*/
/* ] -- enter compiler state                                                  */
/*----------------------------------------------------------------------------*/
void Task::vDocolCompiling(MP_Task mpTask)
{
  ENTRY("vDocolCompiling"_Stz);
  
  mpTask->m_pCompiler->m_bCompiling = true;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ -- enter interpreter state                                               */
/*----------------------------------------------------------------------------*/
void Task::vDocolInterpreting(MP_Task mpTask)
{
  ENTRY("vDocolInterpreting"_Stz);
  
  mpTask->m_pCompiler->m_bCompiling = false;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* docol.isCompiling -- currently in compiling mode?                          */
/*----------------------------------------------------------------------------*/
void Task::vDocolIsCompiling(MP_Task mpTask)
{
  ENTRY("vDocolIsCompiling"_Stz);
  
  i8 iIsCompiling = mpTask->m_pCompiler->m_bCompiling ? 1 : 0;
  
  mpTask->m_pDataStack->vPushInt(iIsCompiling);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* docol.isInterpreting -- currently in interpreting mode?                    */
/*----------------------------------------------------------------------------*/
void Task::vDocolIsInterpreting(MP_Task mpTask)
{
  ENTRY("vDocolIsInterpreting"_Stz);
  
  i8 iIsInterpreting = mpTask->m_pCompiler->m_bCompiling ? 0 : 1;
  
  mpTask->m_pDataStack->vPushInt(iIsInterpreting);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
