/*============================================================================*/
/* Steps_DdlLock.cpp - ddl lock step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_DdlLock(void)
{
  vAddStep("DdlLock[].dropRead"_Stz,  &Step::vDdlLockDropRead);
  vAddStep("DdlLock[].dropWrite"_Stz, &Step::vDdlLockDropWrite);
  vAddStep("DdlLock[].grabRead"_Stz,  &Step::vDdlLockGrabRead);
  vAddStep("DdlLock[].grabWrite"_Stz, &Step::vDdlLockGrabWrite);
  vAddStep("DdlLock[].none"_Stz,      &Step::vDdlLockNone);
  vAddStep("DdlLock[].testDrop"_Stz,  &Step::vDdlLockTestDrop);
}

/*----------------------------------------------------------------------------*/
/* DdlLock[].dropRead -- drop ddl lock for read                               */
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
/* DdlLock[].dropWrite -- drop ddl lock for write                             */
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
/* DdlLock[].grabRead -- grab ddl lock for read                               */
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
/* DdlLock[].grabWrite -- grab ddl lock for write                             */
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
/* DdlLock[].none -- check for no ddl lock                                    */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockNone(void)
{
  ENTRY("vDdlLockNone"_Stz);

  if (LH_HAVE_NO_LOCK != m_pRegSet->m_eDdlLocked)
    THROW("DdlLock[].none : register still has ddl locked"_Stz);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DdlLock[].testDrop -- test for ddl lock and drop if locked by task         */
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
