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
void Step::vDdlLockDropRead(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDdlLockDropRead"_Stz);
  
  ASSERT(LH_HAVE_READ_LOCK == pForth->m_pTask->m_eDdlLocked);
  
  RASSERT(DL_NONE != pForth->m_pTask->pJob()->m_aJobClass->m_eDdlLock);

  AM::vDdlDropRead(* mpTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock::dropWrite -- drop ddl lock for write                              */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockDropWrite(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDdlLockDropWrite"_Stz);
  
  ASSERT(LH_HAVE_WRITE_LOCK == pForth->m_pTask->m_eDdlLocked);
  
  RASSERT(DL_GRAB_WRITE == pForth->m_pTask->pJob()->m_aJobClass->m_eDdlLock);
  
  AM::vDdlDropWrite(* mpTask);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock::grabRead -- grab ddl lock for read                                */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockGrabRead(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDdlLockGrabRead"_Stz);
  
  ASSERT(! pForth->m_pTask->m_eDdlLocked);
  
  RASSERT(DL_NONE != pForth->m_pTask->pJob()->m_aJobClass->m_eDdlLock);
  
  AM::vDdlGrabRead(* mpTask);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock::grabWrite -- grab ddl lock for write                              */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockGrabWrite(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDdlLockGrabWrite"_Stz);
  
  ASSERT(! pForth->m_pTask->m_eDdlLocked);

  RASSERT(DL_GRAB_WRITE == pForth->m_pTask->pJob()->m_aJobClass->m_eDdlLock);
  
  AM::vDdlGrabWrite(* mpTask);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock::testDrop -- test for ddl lock and drop if locked by task          */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockTestDrop(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDdlLockTestDrop"_Stz);
  
  if (LH_HAVE_READ_LOCK == pForth->m_pTask->m_eDdlLocked) {
    
    AM::vDdlDropRead(* mpTask);

  }
  else
  if (LH_HAVE_WRITE_LOCK == pForth->m_pTask->m_eDdlLocked) {
    
    AM::vDdlDropWrite(* mpTask);

  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
