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
  
  vAddPrimitive("jobClass.buffer@"_Stz, &vJobClassBuffer,  false);
  vAddPrimitive("jobClass.new"_Stz,     &vJobClassNew,     false);
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
  
  eCN eClass = (eCN)iJobClass;
  
  JobClass* pJobClass = gxJobCatalog.pFindJob(eClass);
  
  mpTask->m_pDataStack->vPushInt((i8)pJobClass);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobClass.buffer@ -- get JobClass::m_bBuffer                                */
/*----------------------------------------------------------------------------*/
void Task::vJobClassBuffer(MP_Task mpTask)
{
  ENTRY("vJobClassBuffer"_Stz);
  
  i8 iJobClass_p;
  if ( ! mpTask->m_pDataStack->bPop(iJobClass_p) )
    THROW_INVALID_DSTACK("jobClass.buffer@"_Stz);
  JobClass* pJobClass = _pJobClass(iJobClass_p);
  
  bool bBuffer = pJobClass->m_bBuffer;
  
  mpTask->m_pDataStack->vPushInt((i8)bBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobClass.new -- create a new job by running the new-function               */
/*----------------------------------------------------------------------------*/
void Task::vJobClassNew(MP_Task mpTask)
{
  ENTRY("vJobClassNew"_Stz);
  
  i8 iJobClass_p;
  if ( ! mpTask->m_pDataStack->bPop(iJobClass_p) )
    THROW_INVALID_DSTACK("jobClass::find"_Stz);
  JobClass* pJobClass = _pJobClass(iJobClass_p);

  Object* pArgPtr;
  if ( ! mpTask->m_pDataStack->bPop(pArgPtr) )
    THROW_INVALID_DSTACK("job.acksRemaining!"_Stz);
  Job* pArg = _pJob(pArgPtr);

  Job* pJob = pJobClass->pNewJob(pArg);
  
  mpTask->m_pDataStack->vPushObject(pJob);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
