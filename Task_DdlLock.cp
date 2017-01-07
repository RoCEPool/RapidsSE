/*============================================================================*/
/* Task_groups.cpp - task groups                                              */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
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
  vAddPrimitive("ddlLock.dropRead", &vDdlLockDropRead,  false);
  vAddPrimitive("ddlLock.dropWrite",&vDdlLockDropWrite, false);
  vAddPrimitive("ddlLock.grabRead", &vDdlLockGrabRead,  false);
  vAddPrimitive("ddlLock.grabWrite",&vDdlLockGrabWrite, false);
  vAddPrimitive("ddlLock.testDrop", &vDdlLockTestDrop,  false);
}

/*----------------------------------------------------------------------------*/
/* ddlLock.dropRead -- drop ddl lock for read                                 */
/*----------------------------------------------------------------------------*/
void Task::vDdlLockDropRead(NNP_Task nnpTask)
{
  ENTRY("vDdlLockDropRead");
  
  ASSERT(C_HAVE_READ_LOCK == nnpTask->m_iDdlLocked);
  
  gxDdlLock.vDropRead();
  
  nnpTask->m_iDdlLocked = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock.dropWrite -- drop ddl lock for write                               */
/*----------------------------------------------------------------------------*/
void Task::vDdlLockDropWrite(NNP_Task nnpTask)
{
  ENTRY("vDdlLockDropWrite");
  
  ASSERT(C_HAVE_WRITE_LOCK == nnpTask->m_iDdlLocked);
  
  gxDdlLock.vDropWrite();
  
  nnpTask->m_iDdlLocked = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock.grabRead -- grab ddl lock for read                                 */
/*----------------------------------------------------------------------------*/
void Task::vDdlLockGrabRead(NNP_Task nnpTask)
{
  ENTRY("vDdlLockGrabRead");
  
  ASSERT(! nnpTask->m_iDdlLocked);
  
  gxDdlLock.vGrabRead();
  
  nnpTask->m_iDdlLocked = C_HAVE_READ_LOCK;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock.grabWrite -- grab ddl lock for write                               */
/*----------------------------------------------------------------------------*/
void Task::vDdlLockGrabWrite(NNP_Task nnpTask)
{
  ENTRY("vDdlLockGrabWrite");
  
  ASSERT(! nnpTask->m_iDdlLocked);

  gxDdlLock.vGrabWrite();
  
  nnpTask->m_iDdlLocked = C_HAVE_WRITE_LOCK;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock.testDrop -- test for ddl lock and drop if locked by task           */
/*----------------------------------------------------------------------------*/
void Task::vDdlLockTestDrop(NNP_Task nnpTask)
{
  ENTRY("vDdlLockTestDrop");
  
  if (C_HAVE_READ_LOCK == nnpTask->m_iDdlLocked) {
    gxDdlLock.vDropRead();
    nnpTask->m_iDdlLocked = 0;
  }
  else
  if (C_HAVE_WRITE_LOCK == nnpTask->m_iDdlLocked) {
    gxDdlLock.vDropWrite();
    nnpTask->m_iDdlLocked = 0;
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
