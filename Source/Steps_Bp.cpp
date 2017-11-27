/*============================================================================*/
/* Steps_Bp.cpp - buffer pool step group                                      */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;

i8 giCountObjects[CN_ENUMS];

// bp: /////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Bp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddPrimitive("bp::analyze"_Stz,     &vBpAnalyze,        false);
  vAddPrimitive("bp::dump"_Stz,        &vBpDump,           false);
  vAddPrimitive("bp::freeBuffers"_Stz, &vFreeBuffers,      false);
  vAddPrimitive("bp::freePercent"_Stz, &vFreePercent,      false);
  vAddPrimitive("bp::freeSpace"_Stz,   &vFreeSpace,        false);
  vAddPrimitive("bp::inUseSpace"_Stz,  &vInUseSpace,       false);
  vAddPrimitive("bp::readBufferPool"_Stz,&vBpReadBufferPool,false);
  vAddPrimitive("bp::snapshot"_Stz,    &vSnapshot,         false);
  vAddPrimitive("bp::totalSpace"_Stz,  &vTotalSpace,       false);
  vAddPrimitive("bp::writeBufferPool"_Stz,&vBpWriteBufferPool,false);

  vAddPrimitive("bpa"_Stz,             &vBpAnalyze,        false);
  vAddPrimitive("bpd"_Stz,             &vBpDump,           false);
  vAddPrimitive("bpr"_Stz,             &vBpReadBufferPool, false);
  vAddPrimitive("bpw"_Stz,             &vBpWriteBufferPool,false);
 
  vAddPrimitive("epochDump"_Stz,       &vBpEpochDump,      false);
  vAddPrimitive("epochTake"_Stz,       &vBpEpochTake,      false);
}

/*----------------------------------------------------------------------------*/
/* bp.analyze -- buffer pool analyze                                          */
/*----------------------------------------------------------------------------*/
void Task::vBpAnalyze(MP_Task mpTask)
{
  ENTRY("vBpa"_Stz);
  
  BPA* pBPA = new BPA;
  
  pBPA->vPoolAnalyze();
  
  delete pBPA;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.dump -- dump of objects in the buffer pool                              */
/*----------------------------------------------------------------------------*/
void Task::vBpDump(MP_Task mpTask)
{
  ENTRY("vBpDump"_Stz);
  
  BPA* pBPA = new BPA;
  
  pBPA->vPoolDump();
  
  delete pBPA;
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* epochDump -- dump an epoch                                                 */
/*----------------------------------------------------------------------------*/
void Task::vBpEpochDump(MP_Task mpTask)
{
  ENTRY("vBpEpochDump"_Stz);
  
  BPA* pBPA = new BPA;
  i8 iCountObjects[CN_ENUMS];
  
  vTrace("\n"_Stz);
  vTrace("%sWords created since last epoch:\n"_Stz,aSingleTab());
  
  pBPA->vWordList(gpAttachMap->m_iEpoch);
  pBPA->vObjectSnapshot(&iCountObjects[0]);
  
  vTrace("\n%sObjects created since last epoch:\n\n"_Stz,aSingleTab());
  
  for (i8 iIndex =0 ; iIndex < CN_ENUMS; iIndex++) {
    if (iCountObjects[iIndex] != giCountObjects[iIndex])
      vTrace("%s  %12s %-6lld \n"_Stz, aSingleTab(), aCN[iIndex],
             iCountObjects[iIndex] - giCountObjects[iIndex]);
  }
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.readBufferPool -- read the buffers from the file                        */
/*----------------------------------------------------------------------------*/
void Task::vBpReadBufferPool(MP_Task mpTask)
{
  ENTRY("vBpReadBufferPool"_Stz);
  
  BPA* pBPA = new BPA;
  
  pBPA->vReadBuffers("/tmp/buffers.dmp"_Stz);
  
  delete pBPA;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.writeBufferPool -- write the buffer pool buffers to a file              */
/*----------------------------------------------------------------------------*/
void Task::vBpWriteBufferPool(MP_Task mpTask)
{
  ENTRY("vBpWriteBufferPool"_Stz);
  
  BPA* pBPA = new BPA;
  
  //pBPA->vWriteBuffers("/tmp/buffers.dmp"_Stz);

  pBPA->vWritePersistentHeaps("/tmp/persistentHeaps.dmp"_Stz);

  delete pBPA;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* epochTake -- take an epoch                                                 */
/*----------------------------------------------------------------------------*/
void Task::vBpEpochTake(MP_Task mpTask)
{
  ENTRY("vBpEpochTake"_Stz);
  
  // increment the epoch
  gpAttachMap->m_iEpoch++;
  BPA* pBPA = new BPA;
  
  pBPA->vObjectSnapshot(&giCountObjects[0]);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::freeBuffers -- push number of free buffers in buffer pool onto stack    */
/*----------------------------------------------------------------------------*/
void Task::vFreeBuffers(MP_Task mpTask)
{
  ENTRY("vFreePercent"_Stz);
  
  i8 iFreeBuffers = gpBufferPool->iFreeCount();
  
  mpTask->m_pDataStack->vPushInt(iFreeBuffers);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::freePercent -- push free percent of buffer pool onto stack              */
/*----------------------------------------------------------------------------*/
void Task::vFreePercent(MP_Task mpTask)
{
  ENTRY("vFreePercent"_Stz);
  
  i8 iFreePercent = gpBufferPool->iFreePercent();
  
  mpTask->m_pDataStack->vPushInt(iFreePercent);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::freeSpace -- push free space in buffer pool onto stack                  */
/*----------------------------------------------------------------------------*/
void Task::vFreeSpace(MP_Task mpTask)
{
  ENTRY("vFreeSpace"_Stz);
  
  i8 iFreeSpace = gpBufferPool->iFreeSpace();
  
  mpTask->m_pDataStack->vPushInt(iFreeSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::inUseSpace -- push in-use space in buffer pool onto stack               */
/*----------------------------------------------------------------------------*/
void Task::vInUseSpace(MP_Task mpTask)
{
  ENTRY("vInUseSpace"_Stz);
  
  i8 iInUseSpace = gpAttachMap->m_iRegionSize
                     - gpAttachMap->m_iJumboSize
                     - gpBufferPool->iFreeSpace();
  
  mpTask->m_pDataStack->vPushInt(iInUseSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::snapshot -- push snapshot of buffer pool onto stack                     */
/*----------------------------------------------------------------------------*/
void Task::vSnapshot(MP_Task mpTask)
{
  ENTRY("vSnapshot"_Stz);
  
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
  
  gpAttachMap->m_pTransientSmallHeap->vAnalyse(iCountObjects);
  gpAttachMap->m_pTransientMediumHeap->vAnalyse(iCountObjects);
  gpAttachMap->m_pTransientLargeHeap->vAnalyse(iCountObjects);
  gpAttachMap->m_pTransientJumboHeap->vAnalyse(iCountObjects);

  gpAttachMap->m_pPersistentSmallHeap->vAnalyse(iCountObjects);
  gpAttachMap->m_pPersistentMediumHeap->vAnalyse(iCountObjects);
  gpAttachMap->m_pPersistentLargeHeap->vAnalyse(iCountObjects);
  gpAttachMap->m_pPersistentJumboHeap->vAnalyse(iCountObjects);
  
  for (i8 index = 0; index < CN_ENUMS; index++)
    mpTask->m_pDataStack->vPushInt(iCountObjects[index]);
  
  mpTask->m_pDataStack->vPushInt(CN_ENUMS);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::totalSpace -- push total space in buffer pool onto stack                */
/*----------------------------------------------------------------------------*/
void Task::vTotalSpace(MP_Task mpTask)
{
  ENTRY("vTotalSpace"_Stz);
  
  i8 iTotalSpace = gpAttachMap->m_iRegionSize - gpAttachMap->m_iJumboSize;
  
  mpTask->m_pDataStack->vPushInt(iTotalSpace);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
