/*============================================================================*/
/* Task_Bp.cpp - task buffer pool group                                       */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern Heap*           gpJumboHeap;

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
  
  vAddPrimitive("_bp_analyze",      &vBpAnalyze,        false);
  vAddPrimitive("_bp_dump",         &vBpdump,           false);
  vAddPrimitive("bp.freeBuffers",  &vFreeBuffers,      false);
  vAddPrimitive("bp.freePercent",  &vFreePercent,      false);
  vAddPrimitive("bp.freeSpace",    &vFreeSpace,        false);
  vAddPrimitive("bp.inUseSpace",   &vInUseSpace,       false);
  vAddPrimitive("bp.snapshot",     &vSnapshot,         false);
  vAddPrimitive("bp.totalSpace",   &vTotalSpace,       false);
}

/*----------------------------------------------------------------------------*/
/* bp.analyze -- buffer pool analyze                                          */
/*----------------------------------------------------------------------------*/
void Task::vBpAnalyze(NNP_Task nnpTask)
{
  ENTRY("vBpa");
  
  BPA* pBPA = new BPA;
  
  pBPA->vAnalyze();
  
  delete pBPA;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.dump -- dump of objects in the buffer pool                              */
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
/* bp.freeBuffers -- push number of free buffers in buffer pool onto stack    */
/*----------------------------------------------------------------------------*/
void Task::vFreeBuffers(NNP_Task nnpTask)
{
  ENTRY("vFreePercent");
  
  i8 iFreeBuffers = gpBufferPool->iFreeCount();
  
  nnpTask->m_pDataStack->vPush(iFreeBuffers);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.freePercent -- push free percent of buffer pool onto stack              */
/*----------------------------------------------------------------------------*/
void Task::vFreePercent(NNP_Task nnpTask)
{
  ENTRY("vFreePercent");
  
  i8 iFreePercent = gpBufferPool->iFreePercent();
  
  nnpTask->m_pDataStack->vPush(iFreePercent);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.freeSpace -- push free space in buffer pool onto stack                  */
/*----------------------------------------------------------------------------*/
void Task::vFreeSpace(NNP_Task nnpTask)
{
  ENTRY("vFreeSpace");
  
  i8 iFreeSpace = gpBufferPool->iFreeSpace();
  
  nnpTask->m_pDataStack->vPush(iFreeSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.inUseSpace -- push in-use space in buffer pool onto stack               */
/*----------------------------------------------------------------------------*/
void Task::vInUseSpace(NNP_Task nnpTask)
{
  ENTRY("vInUseSpace");
  
  i8 iInUseSpace = gxArguments.m_iRegionSize
                     - gxArguments.m_iJumboSize
                     - gpBufferPool->iFreeSpace();
  
  nnpTask->m_pDataStack->vPush(iInUseSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.snapshot -- push snapshot of buffer pool onto stack                     */
/*----------------------------------------------------------------------------*/
void Task::vSnapshot(NNP_Task nnpTask)
{
  ENTRY("vSnapshot");
  
  i8 iCountObjects[CN_ENUMS];
  for (i8 index = 0; index < CN_ENUMS; index++)
    iCountObjects[index] = 0;
  
  bn nBuffer = 0;
  bn nBufferCount = gpBufferPool->nBuffers() - 1;
  
  for (bn nBuffer = gpBufferPool->nBufferFirst();
       nBuffer <= nBufferCount;
       nBuffer++) {
    
    Buffer* pBuffer = gpBufferPool->pToBuffer(nBuffer);
    iCountObjects[pBuffer->eClass()]++;
  }
  
  gpSmallHeap->vAnalyse(iCountObjects);
  gpMediumHeap->vAnalyse(iCountObjects);
  gpJumboHeap->vAnalyse(iCountObjects);

  for (i8 index = 0; index < CN_ENUMS; index++)
    nnpTask->m_pDataStack->vPush(iCountObjects[index]);
  
  nnpTask->m_pDataStack->vPush(CN_ENUMS);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.totalSpace -- push total space in buffer pool onto stack                */
/*----------------------------------------------------------------------------*/
void Task::vTotalSpace(NNP_Task nnpTask)
{
  ENTRY("vTotalSpace");
  
  i8 iTotalSpace = gxArguments.m_iRegionSize - gxArguments.m_iJumboSize;
  
  nnpTask->m_pDataStack->vPush(iTotalSpace);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
