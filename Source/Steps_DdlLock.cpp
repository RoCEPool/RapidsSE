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
void Step::vDdlLockDropRead(void)
{
  ENTRY("vDdlLockDropRead"_Stz);
  
  ASSERT(LH_HAVE_READ_LOCK == m_pRegSet->m_eDdlLocked);
  
  RASSERT(DL_NONE != m_pTask->pJob()->m_aJobClass->m_eDdlLock);

  AM::vDdlDropRead(m_pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock::dropWrite -- drop ddl lock for write                              */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockDropWrite(void)
{
  ENTRY("vDdlLockDropWrite"_Stz);
  
  ASSERT(LH_HAVE_WRITE_LOCK == m_pRegSet->m_eDdlLocked);
  
  RASSERT(DL_GRAB_WRITE == m_pTask->pJob()->m_aJobClass->m_eDdlLock);
  
  AM::vDdlDropWrite(m_pTask);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock::grabRead -- grab ddl lock for read                                */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockGrabRead(void)
{
  ENTRY("vDdlLockGrabRead"_Stz);
  
  ASSERT(! m_pRegSet->m_eDdlLocked);
  
  RASSERT(DL_NONE != m_pTask->pJob()->m_aJobClass->m_eDdlLock);
  
  AM::vDdlGrabRead(m_pTask);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock::grabWrite -- grab ddl lock for write                              */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockGrabWrite(void)
{
  ENTRY("vDdlLockGrabWrite"_Stz);
  
  ASSERT(! m_pRegSet->m_eDdlLocked);

  RASSERT(DL_GRAB_WRITE == m_pTask->pJob()->m_aJobClass->m_eDdlLock);
  
  AM::vDdlGrabWrite(m_pTask);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock::testDrop -- test for ddl lock and drop if locked by task          */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockTestDrop(void)
{
  ENTRY("vDdlLockTestDrop"_Stz);
  
  if (LH_HAVE_READ_LOCK == m_pRegSet->m_eDdlLocked) {
    
    AM::vDdlDropRead(m_pTask);

  }
  else
  if (LH_HAVE_WRITE_LOCK == m_pRegSet->m_eDdlLocked) {
    
    AM::vDdlDropWrite(m_pTask);

  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
