/*============================================================================*/
/* Steps_JobClass.cpp - jobClass step group                                   */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap* gpAttachMap;

// job: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobClass(void)
{
  vAddStep("jobClass::find"_Stz,   &Task::vJobClassFind);
  
  vAddStep("jobClass.buffer@"_Stz, &Task::vJobClassBuffer);
  vAddStep("jobClass.name"_Stz,    &Task::vJobClassName);
  vAddStep("jobClass.new"_Stz,     &Task::vJobClassNew);
  
  vAddStep("jobClass.seedRun"_Stz, &Task::vJobClassSeedRun);
  vAddStep("jobClass.seedReq"_Stz, &Task::vJobClassSeedReq);
  vAddStep("jobClass.seedRcv"_Stz, &Task::vJobClassSeedRcv);
  vAddStep("jobClass.seedExe"_Stz, &Task::vJobClassSeedExe);
  vAddStep("jobClass.seedEnd"_Stz, &Task::vJobClassSeedEnd);
  vAddStep("jobClass.seedCan"_Stz, &Task::vJobClassSeedCan);
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
  
  JobClass* pJobClass = JobClass::pFind(eClass);
  
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
/* jobClass.name -- get the name of the job class                             */
/*----------------------------------------------------------------------------*/
void Task::vJobClassName(MP_Task mpTask)
{
  ENTRY("vJobClassName"_Stz);
  
  i8 iJobClass_p;
  if ( ! mpTask->m_pDataStack->bPop(iJobClass_p) )
    THROW_INVALID_DSTACK("jobClass.name"_Stz);
  JobClass* pJobClass = _pJobClass(iJobClass_p);
  
  s256 sName((Stz*)&pJobClass->m_aName);
  
  mpTask->m_pDataStack->vPushS256(sName);
  
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
    THROW_INVALID_DSTACK("jobClass.new"_Stz);
  JobClass* pJobClass = _pJobClass(iJobClass_p);

  Object* pArgPtr;
  if ( ! mpTask->m_pDataStack->bPop(pArgPtr) )
    THROW_INVALID_DSTACK("jobClass.new"_Stz);
  Job* pArg = _pJob(pArgPtr);

  Job* pJob = pJobClass->pNewJob(pArg);
  
  mpTask->m_pDataStack->vPushObject(pJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobClass.seedRun -- get the run seed of the job class                      */
/*----------------------------------------------------------------------------*/
void Task::vJobClassSeedRun(MP_Task mpTask)
{
  ENTRY("vJobClassSeedRun"_Stz);
  
  i8 iJobClass_p;
  if ( ! mpTask->m_pDataStack->bPop(iJobClass_p) )
    THROW_INVALID_DSTACK("jobClass.seedRun"_Stz);
  JobClass* pJobClass = _pJobClass(iJobClass_p);
  
  s256 sSeed((Stz*)pJobClass->m_aSeedRun);
  
  mpTask->m_pDataStack->vPushS256(sSeed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobClass.seedReq -- get the req seed of the job class                      */
/*----------------------------------------------------------------------------*/
void Task::vJobClassSeedReq(MP_Task mpTask)
{
  ENTRY("vJobClassSeedReq"_Stz);
  
  i8 iJobClass_p;
  if ( ! mpTask->m_pDataStack->bPop(iJobClass_p) )
    THROW_INVALID_DSTACK("jobClass.seedReq"_Stz);
  JobClass* pJobClass = _pJobClass(iJobClass_p);
  
  s256 sSeed((Stz*)&pJobClass->m_aSeedReq);
  
  mpTask->m_pDataStack->vPushS256(sSeed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobClass.seedRcv -- get the rcv seed of the job class                      */
/*----------------------------------------------------------------------------*/
void Task::vJobClassSeedRcv(MP_Task mpTask)
{
  ENTRY("vJobClassSeedRcv"_Stz);
  
  i8 iJobClass_p;
  if ( ! mpTask->m_pDataStack->bPop(iJobClass_p) )
    THROW_INVALID_DSTACK("jobClass.seedRcv"_Stz);
  JobClass* pJobClass = _pJobClass(iJobClass_p);
  
  s256 sSeed((Stz*)&pJobClass->m_aSeedRcv);
  
  mpTask->m_pDataStack->vPushS256(sSeed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobClass.seedExe -- get the exe seed of the job class                      */
/*----------------------------------------------------------------------------*/
void Task::vJobClassSeedExe(MP_Task mpTask)
{
  ENTRY("vJobClassSeedExe"_Stz);
  
  i8 iJobClass_p;
  if ( ! mpTask->m_pDataStack->bPop(iJobClass_p) )
    THROW_INVALID_DSTACK("jobClass.seedExe"_Stz);
  JobClass* pJobClass = _pJobClass(iJobClass_p);
  
  s256 sSeed((Stz*)&pJobClass->m_aSeedExe);
  
  mpTask->m_pDataStack->vPushS256(sSeed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobClass.seedEnd -- get the end seed of the job class                      */
/*----------------------------------------------------------------------------*/
void Task::vJobClassSeedEnd(MP_Task mpTask)
{
  ENTRY("vJobClassSeedEnd"_Stz);
  
  i8 iJobClass_p;
  if ( ! mpTask->m_pDataStack->bPop(iJobClass_p) )
    THROW_INVALID_DSTACK("jobClass.seedEnd"_Stz);
  JobClass* pJobClass = _pJobClass(iJobClass_p);
  
  s256 sSeed((Stz*)&pJobClass->m_aSeedEnd);
  
  mpTask->m_pDataStack->vPushS256(sSeed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobClass.seedCan -- get the can seed of the job class                      */
/*----------------------------------------------------------------------------*/
void Task::vJobClassSeedCan(MP_Task mpTask)
{
  ENTRY("vJobClassSeedCan"_Stz);
  
  i8 iJobClass_p;
  if ( ! mpTask->m_pDataStack->bPop(iJobClass_p) )
    THROW_INVALID_DSTACK("jobClass.seedCan"_Stz);
  JobClass* pJobClass = _pJobClass(iJobClass_p);
  
  s256 sSeed((Stz*)&pJobClass->m_aSeedCan);
  
  mpTask->m_pDataStack->vPushS256(sSeed);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
