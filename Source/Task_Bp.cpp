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

// bp: /////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Bp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddPrimitive("bpa",             &vBpa,              false);
  vAddPrimitive("bpdump",          &vBpdump,           false);
  vAddPrimitive("freeBuffers",     &vFreeBuffers,      false);
  vAddPrimitive("freePercent",     &vFreePercent,      false);
  vAddPrimitive("freeSpace",       &vFreeSpace,        false);
  vAddPrimitive("inUseSpace",      &vInUseSpace,       false);
  vAddPrimitive("totalSpace",      &vTotalSpace,       false);
}

/*----------------------------------------------------------------------------*/
/* bpa -- buffer pool analyze                                                 */
/*----------------------------------------------------------------------------*/
void Task::vBpa(NNP_Task nnpTask)
{
  ENTRY("vBpa");
  
  BPA* pBPA = new BPA;
  
  pBPA->vAnalyze();
  
  delete pBPA;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bpdump -- dump of objects in the buffer pool                               */
/*----------------------------------------------------------------------------*/
void Task::vBpdump(NNP_Task nnpTask)
{
  ENTRY("vBpdump");
  
  BPA* pBPA = new BPA;
  
  pBPA->vDump();
  
  delete pBPA;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* freeBuffers -- push number of free buffers in buffer pool onto stack       */
/*----------------------------------------------------------------------------*/
void Task::vFreeBuffers(NNP_Task nnpTask)
{
  ENTRY("vFreePercent");
  
  i8 iFreeBuffers = gpBufferPool->iFreeCount();
  
  nnpTask->m_xDataStack.vPush(iFreeBuffers);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* freePercent -- push free percent of buffer pool onto stack                 */
/*----------------------------------------------------------------------------*/
void Task::vFreePercent(NNP_Task nnpTask)
{
  ENTRY("vFreePercent");
  
  i8 iFreePercent = gpBufferPool->iFreePercent();
  
  nnpTask->m_xDataStack.vPush(iFreePercent);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* freeSpace -- push free space in buffer pool onto stack                     */
/*----------------------------------------------------------------------------*/
void Task::vFreeSpace(NNP_Task nnpTask)
{
  ENTRY("vFreeSpace");
  
  i8 iFreeSpace = gpBufferPool->iFreeSpace();
  
  nnpTask->m_xDataStack.vPush(iFreeSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* inUseSpace -- push in-use space in buffer pool onto stack                  */
/*----------------------------------------------------------------------------*/
void Task::vInUseSpace(NNP_Task nnpTask)
{
  ENTRY("vInUseSpace");
  
  i8 iInUseSpace = gpBufferPool->iRegionSize() - gpBufferPool->iFreeSpace();
  
  nnpTask->m_xDataStack.vPush(iInUseSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TotalSpace -- push total space in buffer pool onto stack                   */
/*----------------------------------------------------------------------------*/
void Task::vTotalSpace(NNP_Task nnpTask)
{
  ENTRY("vTotalSpace");
  
  i8 iTotalSpace = gpBufferPool->iRegionSize();
  
  nnpTask->m_xDataStack.vPush(iTotalSpace);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
