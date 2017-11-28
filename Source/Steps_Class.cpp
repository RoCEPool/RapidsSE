/*============================================================================*/
/* Steps_Class.cpp - class steps group                                        */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern NP_Task         gnpBootTask;

// class: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Class(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddPrimitive("class.name"_Stz,      &vClassName,    false);
}

/*----------------------------------------------------------------------------*/
/* class.name -- translate class no to name                                   */
/*----------------------------------------------------------------------------*/
void Task::vClassName(MP_Task mpTask)
{
  ENTRY("vClassName"_Stz);
  
  i8 iClassNo;
  if ( ! mpTask->m_pDataStack->bPop(iClassNo) )
    THROW_INVALID_DSTACK("class.name"_Stz);
  
  s32 sClassName;
  
  sClassName = aCN[iClassNo];
  
  mpTask->m_pDataStack->vPushS32(sClassName);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
