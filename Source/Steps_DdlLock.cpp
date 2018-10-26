/*============================================================================*/
/* Steps_DdlLock.cpp - ddl lock step group                                    */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

// ddllock: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_DdlLock(void)
{
  vAddStep("ddlLock::dropRead"_Stz, &Task::vDdlLockDropRead);
  vAddStep("ddlLock::dropWrite"_Stz,&Task::vDdlLockDropWrite);
  vAddStep("ddlLock::grabRead"_Stz, &Task::vDdlLockGrabRead);
  vAddStep("ddlLock::grabWrite"_Stz,&Task::vDdlLockGrabWrite);
  vAddStep("ddlLock::testDrop"_Stz, &Task::vDdlLockTestDrop);
}

/*----------------------------------------------------------------------------*/
/* ddlLock::dropRead -- drop ddl lock for read                                */
/*----------------------------------------------------------------------------*/
void Task::vDdlLockDropRead(MP_Task mpTask)
{
  ENTRY("vDdlLockDropRead"_Stz);
  
  ASSERT(C_HAVE_READ_LOCK == mpTask->m_iDdlLocked);
  
  gpAttachMap->m_xDdlLock.vDropRead();
  
  mpTask->m_iDdlLocked = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock::dropWrite -- drop ddl lock for write                              */
/*----------------------------------------------------------------------------*/
void Task::vDdlLockDropWrite(MP_Task mpTask)
{
  ENTRY("vDdlLockDropWrite"_Stz);
  
  ASSERT(C_HAVE_WRITE_LOCK == mpTask->m_iDdlLocked);
  
  gpAttachMap->m_xDdlLock.vDropWrite();
  
  mpTask->m_iDdlLocked = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock::grabRead -- grab ddl lock for read                                */
/*----------------------------------------------------------------------------*/
void Task::vDdlLockGrabRead(MP_Task mpTask)
{
  ENTRY("vDdlLockGrabRead"_Stz);
  
  ASSERT(! mpTask->m_iDdlLocked);
  
  gpAttachMap->m_xDdlLock.vGrabRead();
  
  mpTask->m_iDdlLocked = C_HAVE_READ_LOCK;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock::grabWrite -- grab ddl lock for write                              */
/*----------------------------------------------------------------------------*/
void Task::vDdlLockGrabWrite(MP_Task mpTask)
{
  ENTRY("vDdlLockGrabWrite"_Stz);
  
  ASSERT(! mpTask->m_iDdlLocked);

  gpAttachMap->m_xDdlLock.vGrabWrite();
  
  mpTask->m_iDdlLocked = C_HAVE_WRITE_LOCK;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock::testDrop -- test for ddl lock and drop if locked by task          */
/*----------------------------------------------------------------------------*/
void Task::vDdlLockTestDrop(MP_Task mpTask)
{
  ENTRY("vDdlLockTestDrop"_Stz);
  
  if (C_HAVE_READ_LOCK == mpTask->m_iDdlLocked) {
    gpAttachMap->m_xDdlLock.vDropRead();
    mpTask->m_iDdlLocked = 0;
  }
  else
  if (C_HAVE_WRITE_LOCK == mpTask->m_iDdlLocked) {
    gpAttachMap->m_xDdlLock.vDropWrite();
    mpTask->m_iDdlLocked = 0;
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
