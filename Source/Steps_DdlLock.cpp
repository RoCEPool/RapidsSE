/*============================================================================*/
/* Steps_DdlLock.cpp - ddl lock step group                                    */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
  
  gpAttachMap->m_oDdlLock.vDropRead();
  
  mpTask->m_iDdlLocked = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock::dropWrite -- drop ddl lock for write                              */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockDropWrite(MP_Task mpTask)
{
  ENTRY("vDdlLockDropWrite"_Stz);
  
  ASSERT(C_HAVE_WRITE_LOCK == mpTask->m_iDdlLocked);
  
  gpAttachMap->m_oDdlLock.vDropWrite();
  
  mpTask->m_iDdlLocked = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock::grabRead -- grab ddl lock for read                                */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockGrabRead(MP_Task mpTask)
{
  ENTRY("vDdlLockGrabRead"_Stz);
  
  ASSERT(! mpTask->m_iDdlLocked);
  
  gpAttachMap->m_oDdlLock.vGrabRead();
  
  mpTask->m_iDdlLocked = C_HAVE_READ_LOCK;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock::grabWrite -- grab ddl lock for write                              */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockGrabWrite(MP_Task mpTask)
{
  ENTRY("vDdlLockGrabWrite"_Stz);
  
  ASSERT(! mpTask->m_iDdlLocked);

  gpAttachMap->m_oDdlLock.vGrabWrite();
  
  mpTask->m_iDdlLocked = C_HAVE_WRITE_LOCK;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock::testDrop -- test for ddl lock and drop if locked by task          */
/*----------------------------------------------------------------------------*/
void Step::vDdlLockTestDrop(MP_Task mpTask)
{
  ENTRY("vDdlLockTestDrop"_Stz);
  
  if (C_HAVE_READ_LOCK == mpTask->m_iDdlLocked) {
    gpAttachMap->m_oDdlLock.vDropRead();
    mpTask->m_iDdlLocked = 0;
  }
  else
  if (C_HAVE_WRITE_LOCK == mpTask->m_iDdlLocked) {
    gpAttachMap->m_oDdlLock.vDropWrite();
    mpTask->m_iDdlLocked = 0;
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
