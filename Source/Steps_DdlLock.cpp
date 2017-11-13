/*============================================================================*/
/* Steps_DdlLock.cpp - ddl lock step group                                    */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

// ddllock: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_DdlLock(void)
{
  vAddPrimitive("ddlLock::dropRead"_Stz, &vDdlLockDropRead,  false);
  vAddPrimitive("ddlLock::dropWrite"_Stz,&vDdlLockDropWrite, false);
  vAddPrimitive("ddlLock::grabRead"_Stz, &vDdlLockGrabRead,  false);
  vAddPrimitive("ddlLock::grabWrite"_Stz,&vDdlLockGrabWrite, false);
  vAddPrimitive("ddlLock::testDrop"_Stz, &vDdlLockTestDrop,  false);
}

/*----------------------------------------------------------------------------*/
/* ddlLock.dropRead -- drop ddl lock for read                                 */
/*----------------------------------------------------------------------------*/
void Task::vDdlLockDropRead(MP_Task mpTask)
{
  ENTRY("vDdlLockDropRead"_Stz);
  
  ASSERT(C_HAVE_READ_LOCK == mpTask->m_iDdlLocked);
  
  gxDdlLock.vDropRead();
  
  mpTask->m_iDdlLocked = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock.dropWrite -- drop ddl lock for write                               */
/*----------------------------------------------------------------------------*/
void Task::vDdlLockDropWrite(MP_Task mpTask)
{
  ENTRY("vDdlLockDropWrite"_Stz);
  
  ASSERT(C_HAVE_WRITE_LOCK == mpTask->m_iDdlLocked);
  
  gxDdlLock.vDropWrite();
  
  mpTask->m_iDdlLocked = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock.grabRead -- grab ddl lock for read                                 */
/*----------------------------------------------------------------------------*/
void Task::vDdlLockGrabRead(MP_Task mpTask)
{
  ENTRY("vDdlLockGrabRead"_Stz);
  
  ASSERT(! mpTask->m_iDdlLocked);
  
  gxDdlLock.vGrabRead();
  
  mpTask->m_iDdlLocked = C_HAVE_READ_LOCK;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock.grabWrite -- grab ddl lock for write                               */
/*----------------------------------------------------------------------------*/
void Task::vDdlLockGrabWrite(MP_Task mpTask)
{
  ENTRY("vDdlLockGrabWrite"_Stz);
  
  ASSERT(! mpTask->m_iDdlLocked);

  gxDdlLock.vGrabWrite();
  
  mpTask->m_iDdlLocked = C_HAVE_WRITE_LOCK;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock.testDrop -- test for ddl lock and drop if locked by task           */
/*----------------------------------------------------------------------------*/
void Task::vDdlLockTestDrop(MP_Task mpTask)
{
  ENTRY("vDdlLockTestDrop"_Stz);
  
  if (C_HAVE_READ_LOCK == mpTask->m_iDdlLocked) {
    gxDdlLock.vDropRead();
    mpTask->m_iDdlLocked = 0;
  }
  else
  if (C_HAVE_WRITE_LOCK == mpTask->m_iDdlLocked) {
    gxDdlLock.vDropWrite();
    mpTask->m_iDdlLocked = 0;
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
