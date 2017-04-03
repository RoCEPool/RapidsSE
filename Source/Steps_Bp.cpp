/*============================================================================*/
/* Steps_Bp.cpp - buffer pool step group                                      */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

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

i8 giCountObjects[CN_ENUMS];

// bp: /////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Bp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddPrimitive("_bp_analyze"_Str,     &vBpAnalyze,        false);
  vAddPrimitive("_bp_dump"_Str,        &vBpDump,           false);
  vAddPrimitive("bp.freeBuffers"_Str,  &vFreeBuffers,      false);
  vAddPrimitive("bp.freePercent"_Str,  &vFreePercent,      false);
  vAddPrimitive("bp.freeSpace"_Str,    &vFreeSpace,        false);
  vAddPrimitive("bp.inUseSpace"_Str,   &vInUseSpace,       false);
  vAddPrimitive("bp.snapshot"_Str,     &vSnapshot,         false);
  vAddPrimitive("bp.totalSpace"_Str,   &vTotalSpace,       false);

  vAddPrimitive("bpa"_Str,             &vBpAnalyze,        false);
  vAddPrimitive("bpd"_Str,             &vBpDump,           false);
 
  vAddPrimitive("epochDump"_Str,       &vBpEpochDump,      false);
  vAddPrimitive("epochTake"_Str,       &vBpEpochTake,      false);
}

/*----------------------------------------------------------------------------*/
/* bp.analyze -- buffer pool analyze                                          */
/*----------------------------------------------------------------------------*/
void Task::vBpAnalyze(NNP_Task nnpTask)
{
  ENTRY("vBpa"_Str);
  
  BPA* pBPA = new BPA;
  
  pBPA->vPoolAnalyze();
  
  delete pBPA;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.dump -- dump of objects in the buffer pool                              */
/*----------------------------------------------------------------------------*/
void Task::vBpDump(NNP_Task nnpTask)
{
  ENTRY("vBpDump"_Str);
  
  BPA* pBPA = new BPA;
  
  pBPA->vPoolDump();
  
  delete pBPA;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* epochDump -- dump an epoch                                                 */
/*----------------------------------------------------------------------------*/
void Task::vBpEpochDump(NNP_Task nnpTask)
{
  ENTRY("vBpEpochDump"_Str);
  
  BPA* pBPA = new BPA;
  i8 iCountObjects[CN_ENUMS];
  
  vTrace("\n"_Str);
  vTrace("%sWords created since last epoch:\n"_Str,aSingleTab());
  
  pBPA->vWordList(gpAttachMap->m_iEpoch);
  pBPA->vObjectSnapshot(&iCountObjects[0]);
  
  vTrace("\n%sObjects created since last epoch:\n\n"_Str,aSingleTab());
  
  for (i8 iIndex =0 ; iIndex < CN_ENUMS; iIndex++) {
    if (iCountObjects[iIndex] != giCountObjects[iIndex])
      vTrace("%s  %12s %-6lld \n"_Str, aSingleTab(), aCN[iIndex],
             iCountObjects[iIndex] - giCountObjects[iIndex]);
  }
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* epochTake -- take an epoch                                                 */
/*----------------------------------------------------------------------------*/
void Task::vBpEpochTake(NNP_Task nnpTask)
{
  ENTRY("vBpEpochTake"_Str);
  
  // increment the epoch
  gpAttachMap->m_iEpoch++;
  BPA* pBPA = new BPA;
  
  pBPA->vObjectSnapshot(&giCountObjects[0]);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.freeBuffers -- push number of free buffers in buffer pool onto stack    */
/*----------------------------------------------------------------------------*/
void Task::vFreeBuffers(NNP_Task nnpTask)
{
  ENTRY("vFreePercent"_Str);
  
  i8 iFreeBuffers = gpBufferPool->iFreeCount();
  
  nnpTask->m_pDataStack->vPush(iFreeBuffers);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.freePercent -- push free percent of buffer pool onto stack              */
/*----------------------------------------------------------------------------*/
void Task::vFreePercent(NNP_Task nnpTask)
{
  ENTRY("vFreePercent"_Str);
  
  i8 iFreePercent = gpBufferPool->iFreePercent();
  
  nnpTask->m_pDataStack->vPush(iFreePercent);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.freeSpace -- push free space in buffer pool onto stack                  */
/*----------------------------------------------------------------------------*/
void Task::vFreeSpace(NNP_Task nnpTask)
{
  ENTRY("vFreeSpace"_Str);
  
  i8 iFreeSpace = gpBufferPool->iFreeSpace();
  
  nnpTask->m_pDataStack->vPush(iFreeSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.inUseSpace -- push in-use space in buffer pool onto stack               */
/*----------------------------------------------------------------------------*/
void Task::vInUseSpace(NNP_Task nnpTask)
{
  ENTRY("vInUseSpace"_Str);
  
  i8 iInUseSpace = gpAttachMap->m_iRegionSize
                     - gpAttachMap->m_iJumboSize
                     - gpBufferPool->iFreeSpace();
  
  nnpTask->m_pDataStack->vPush(iInUseSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.snapshot -- push snapshot of buffer pool onto stack                     */
/*----------------------------------------------------------------------------*/
void Task::vSnapshot(NNP_Task nnpTask)
{
  ENTRY("vSnapshot"_Str);
  
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
  ENTRY("vTotalSpace"_Str);
  
  i8 iTotalSpace = gpAttachMap->m_iRegionSize - gpAttachMap->m_iJumboSize;
  
  nnpTask->m_pDataStack->vPush(iTotalSpace);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
