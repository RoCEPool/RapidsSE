/*============================================================================*/
/* Steps_Class.cpp - class steps group                                        */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// class: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Class(void)
{
  vAddStep("class::name"_Stz,      &Task::vClassName);
}

/*----------------------------------------------------------------------------*/
/* class::name -- translate class no to name                                  */
/*----------------------------------------------------------------------------*/
void Task::vClassName(MP_Task mpTask)
{
  ENTRY("vClassName"_Stz);
  
  i8 iClassNo;
  if ( ! mpTask->m_pDataStack->bPop(iClassNo) )
    THROW_INVALID_DSTACK("class::name"_Stz);
  
  s32 sClassName;
  
  sClassName = aCN[iClassNo];
  
  mpTask->m_pDataStack->vPushS32(sClassName);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
