/*============================================================================*/
/* Steps_DdlLock.cpp - ddl lock step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_DdlLock(void)
{
  vAddStep("ddlLock::dropRead"_Stz, &Step::vDdlLockDropRead);
  vAddStep("ddlLock::dropWrite"_Stz,&Step::vDdlLockDropWrite);
  vAddStep("ddlLock::grabRead"_Stz, &Step::vDdlLockGrabRead);
  vAddStep("ddlLock::grabWrite"_Stz,&Step::vDdlLockGrabWrite);
  vAddStep("ddlLock::testDrop"_Stz, &Step::vDdlLockTestDrop);
}

/*----------------------------------------------------------------------------*/
/* ddlLock::dropRead -- drop ddl lock for read                                */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockDropRead(MP_Task mpTask)
{
  ENTRY("vDdlLockDropRead"_Stz);
  
  ASSERT(C_HAVE_READ_LOCK == mpTask->m_iDdlLocked);
  
  RASSERT(DL_NONE != mpTask->pJob()->m_pJobClass->m_eDdlLock);

  AM::pDdlLock()->vDropRead();
  
  mpTask->m_iDdlLocked = 0;
  
  ActLog::vAdd(mpTask->pJob(),AN_DDL_DROPPED_READ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock::dropWrite -- drop ddl lock for write                              */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockDropWrite(MP_Task mpTask)
{
  ENTRY("vDdlLockDropWrite"_Stz);
  
  ASSERT(C_HAVE_WRITE_LOCK == mpTask->m_iDdlLocked);
  
  RASSERT(DL_GRAB_WRITE == mpTask->pJob()->m_pJobClass->m_eDdlLock);
  
  AM::pDdlLock()->vDropWrite();
  
  mpTask->m_iDdlLocked = 0;

  ActLog::vAdd(mpTask->pJob(),AN_DDL_DROPPED_WRITE);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock::grabRead -- grab ddl lock for read                                */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockGrabRead(MP_Task mpTask)
{
  ENTRY("vDdlLockGrabRead"_Stz);
  
  ASSERT(! mpTask->m_iDdlLocked);
  
  RASSERT(DL_NONE != mpTask->pJob()->m_pJobClass->m_eDdlLock);

  ActLog::vAdd(mpTask->pJob(),AN_DDL_GRAB_READ);
  AM::pDdlLock()->vGrabRead();
  mpTask->m_iDdlLocked = C_HAVE_READ_LOCK;
  ActLog::vAdd(mpTask->pJob(),AN_DDL_GRABBED_READ);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock::grabWrite -- grab ddl lock for write                              */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockGrabWrite(MP_Task mpTask)
{
  ENTRY("vDdlLockGrabWrite"_Stz);
  
  ASSERT(! mpTask->m_iDdlLocked);

  RASSERT(DL_GRAB_WRITE == mpTask->pJob()->m_pJobClass->m_eDdlLock);
  
  ActLog::vAdd(mpTask->pJob(),AN_DDL_GRAB_WRITE);
  AM::pDdlLock()->vGrabWrite();
  mpTask->m_iDdlLocked = C_HAVE_WRITE_LOCK;
  ActLog::vAdd(mpTask->pJob(),AN_DDL_GRABBED_WRITE);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock::testDrop -- test for ddl lock and drop if locked by task          */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockTestDrop(MP_Task mpTask)
{
  ENTRY("vDdlLockTestDrop"_Stz);
  
  if (C_HAVE_READ_LOCK == mpTask->m_iDdlLocked) {
    AM::pDdlLock()->vDropRead();
    mpTask->m_iDdlLocked = 0;
    ActLog::vAdd(mpTask->pJob(),AN_DDL_DROPPED_READ);
  }
  else
  if (C_HAVE_WRITE_LOCK == mpTask->m_iDdlLocked) {
    AM::pDdlLock()->vDropWrite();
    mpTask->m_iDdlLocked = 0;
    ActLog::vAdd(mpTask->pJob(),AN_DDL_DROPPED_WRITE);
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
