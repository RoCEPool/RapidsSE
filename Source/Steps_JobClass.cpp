/*============================================================================*/
/* Steps_jobClass.cpp - jobClass step group                                   */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap* gpAttachMap;
extern JobCatalog gxJobCatalog;

// job: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_JobClass(void)
{
  vAddPrimitive("jobClass::find"_Stz,   &vJobClassFind,    false);
}

/*----------------------------------------------------------------------------*/
/* jobClass::find -- find job class                                           */
/*----------------------------------------------------------------------------*/
void Task::vJobClassFind(MP_Task mpTask)
{
  ENTRY("vJobClassFind"_Stz);
  
  i8 iJobClass;
  if ( ! mpTask->m_pDataStack->bPop(iJobClass) )
    THROW_INVALID_DSTACK("jobClass::find"_Stz);
  
  JobClass* pJobClass = gxJobCatalog.pFindJob((eCN)iJobClass);
  
  mpTask->m_pDataStack->vPushInt((i8)pJobClass);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
