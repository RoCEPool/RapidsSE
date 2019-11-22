/*============================================================================*/
/* Steps_JobClass.cpp - jobClass step group                                   */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobClass(void)
{
  vAddStep("jobClass::find"_Stz,   &Step::vJobClassFind);
  
  vAddStep("jobClass.buffer@"_Stz, &Step::vJobClassBuffer);
  vAddStep("jobClass.name"_Stz,    &Step::vJobClassName);
  vAddStep("jobClass.new"_Stz,     &Step::vJobClassNew);
  
  vAddStep("jobClass.seedRun"_Stz, &Step::vJobClassSeedRun);
  vAddStep("jobClass.seedReq"_Stz, &Step::vJobClassSeedReq);
  vAddStep("jobClass.seedRcv"_Stz, &Step::vJobClassSeedRcv);
  vAddStep("jobClass.seedExe"_Stz, &Step::vJobClassSeedExe);
  vAddStep("jobClass.seedEnd"_Stz, &Step::vJobClassSeedEnd);
  vAddStep("jobClass.seedCan"_Stz, &Step::vJobClassSeedCan);
}

/*----------------------------------------------------------------------------*/
/* jobClass::find -- find job class                                           */
/*----------------------------------------------------------------------------*/
void Step::vJobClassFind(void)
{
  ENTRY("vJobClassFind"_Stz);
  
  i8 iJobClass;
  if ( ! m_pDataStack->bPopI8(iJobClass) )
    THROW_DSTACK();
  
  eCN eClass = (eCN)iJobClass;
  
  JobClass* pJobClass = JobClass::pFind(eClass);
  
  m_pDataStack->vPushI8((i8)pJobClass);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobClass.buffer@ -- get JobClass::m_bBuffer                                */
/*----------------------------------------------------------------------------*/
void Step::vJobClassBuffer(void)
{
  ENTRY("vJobClassBuffer"_Stz);
  
  i8 iJobClass_p;
  if ( ! m_pDataStack->bPopI8(iJobClass_p) )
    THROW_DSTACK();
  JobClass* pJobClass = _pJobClass(iJobClass_p);
  
  bool bBuffer = pJobClass->m_bBuffer;
  
  m_pDataStack->vPushBool(bBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobClass.name -- get the name of the job class                             */
/*----------------------------------------------------------------------------*/
void Step::vJobClassName(void)
{
  ENTRY("vJobClassName"_Stz);
  
  i8 iJobClass_p;
  if ( ! m_pDataStack->bPopI8(iJobClass_p) )
    THROW_DSTACK();
  JobClass* pJobClass = _pJobClass(iJobClass_p);
  
  s256 sName(aCN[pJobClass->m_eClass]);
  
  m_pDataStack->vPushS256(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobClass.new -- create a new job by running the new-function               */
/*----------------------------------------------------------------------------*/
void Step::vJobClassNew(void)
{
  ENTRY("vJobClassNew"_Stz);
  
  i8 iJobClass_p;
  if ( ! m_pDataStack->bPopI8(iJobClass_p) )
    THROW_DSTACK();
  JobClass* pJobClass = _pJobClass(iJobClass_p);

  Job* pJob = pJobClass->pNewJob(nullptr);
  pJob->pEnameSet()->vAdd("<path>"_Stz,"/"_Stz);

  m_pDataStack->vPushPointer(pJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobClass.seedRun -- get the run seed of the job class                      */
/*----------------------------------------------------------------------------*/
void Step::vJobClassSeedRun(void)
{
  ENTRY("vJobClassSeedRun"_Stz);
  
  i8 iJobClass_p;
  if ( ! m_pDataStack->bPopI8(iJobClass_p) )
    THROW_DSTACK();
  JobClass* pJobClass = _pJobClass(iJobClass_p);
  
  s256 sSeed((Stz*)pJobClass->m_aSeedRun);
  
  m_pDataStack->vPushS256(sSeed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobClass.seedReq -- get the req seed of the job class                      */
/*----------------------------------------------------------------------------*/
void Step::vJobClassSeedReq(void)
{
  ENTRY("vJobClassSeedReq"_Stz);
  
  i8 iJobClass_p;
  if ( ! m_pDataStack->bPopI8(iJobClass_p) )
    THROW_DSTACK();
  JobClass* pJobClass = _pJobClass(iJobClass_p);
  
  s256 sSeed((Stz*)&pJobClass->m_aSeedReq);
  
  m_pDataStack->vPushS256(sSeed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobClass.seedRcv -- get the rcv seed of the job class                      */
/*----------------------------------------------------------------------------*/
void Step::vJobClassSeedRcv(void)
{
  ENTRY("vJobClassSeedRcv"_Stz);
  
  i8 iJobClass_p;
  if ( ! m_pDataStack->bPopI8(iJobClass_p) )
    THROW_DSTACK();
  JobClass* pJobClass = _pJobClass(iJobClass_p);
  
  s256 sSeed((Stz*)&pJobClass->m_aSeedRcv);
  
  m_pDataStack->vPushS256(sSeed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobClass.seedExe -- get the exe seed of the job class                      */
/*----------------------------------------------------------------------------*/
void Step::vJobClassSeedExe(void)
{
  ENTRY("vJobClassSeedExe"_Stz);
  
  i8 iJobClass_p;
  if ( ! m_pDataStack->bPopI8(iJobClass_p) )
    THROW_DSTACK();
  JobClass* pJobClass = _pJobClass(iJobClass_p);
  
  s256 sSeed((Stz*)&pJobClass->m_aSeedExe);
  
  m_pDataStack->vPushS256(sSeed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobClass.seedEnd -- get the end seed of the job class                      */
/*----------------------------------------------------------------------------*/
void Step::vJobClassSeedEnd(void)
{
  ENTRY("vJobClassSeedEnd"_Stz);
  
  i8 iJobClass_p;
  if ( ! m_pDataStack->bPopI8(iJobClass_p) )
    THROW_DSTACK();
  JobClass* pJobClass = _pJobClass(iJobClass_p);
  
  s256 sSeed((Stz*)&pJobClass->m_aSeedEnd);
  
  m_pDataStack->vPushS256(sSeed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobClass.seedCan -- get the can seed of the job class                      */
/*----------------------------------------------------------------------------*/
void Step::vJobClassSeedCan(void)
{
  ENTRY("vJobClassSeedCan"_Stz);
  
  i8 iJobClass_p;
  if ( ! m_pDataStack->bPopI8(iJobClass_p) )
    THROW_DSTACK();
  JobClass* pJobClass = _pJobClass(iJobClass_p);
  
  s256 sSeed((Stz*)&pJobClass->m_aSeedCan);
  
  m_pDataStack->vPushS256(sSeed);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
