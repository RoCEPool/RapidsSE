/*============================================================================*/
/* Steps_Bp.cpp - buffer pool step group                                      */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;
extern BufferPool*     gpBufferPool;

i8 giCountObjects[CN_ENUMS];

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Bp(void)
{
  vAddStep("bp::analyze"_Stz,     &Step::vBpAnalyze);
  vAddStep("bp::dump"_Stz,        &Step::vBpDump);
  vAddStep("bp::freeBuffers"_Stz, &Step::vFreeBuffers);
  vAddStep("bp::freePercent"_Stz, &Step::vFreePercent);
  vAddStep("bp::freeSpace"_Stz,   &Step::vFreeSpace);
  vAddStep("bp::inUseSpace"_Stz,  &Step::vInUseSpace);
  vAddStep("bp::readBufferPool"_Stz,&Step::vBpReadBufferPool);
  vAddStep("bp::snapshot"_Stz,    &Step::vSnapshot);
  vAddStep("bp::totalSpace"_Stz,  &Step::vTotalSpace);
  vAddStep("bp::writeBufferPool"_Stz,&Step::vBpWriteBufferPool);

  vAddStep("bpa"_Stz,             &Step::vBpAnalyze);
  vAddStep("bpd"_Stz,             &Step::vBpDump);
  vAddStep("bpr"_Stz,             &Step::vBpReadBufferPool);
  vAddStep("bpw"_Stz,             &Step::vBpWriteBufferPool);
 
  vAddStep("epochDump"_Stz,       &Step::vBpEpochDump);
  vAddStep("epochTake"_Stz,       &Step::vBpEpochTake);
}

/*----------------------------------------------------------------------------*/
/* bp.analyze -- buffer pool analyze                                          */
/*----------------------------------------------------------------------------*/
void Step::vBpAnalyze(MP_Task mpTask)
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
void Step::vBpDump(MP_Task mpTask)
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
void Step::vBpEpochDump(MP_Task mpTask)
{
  ENTRY("vBpEpochDump"_Stz);
  
  BPA* pBPA = new BPA;
  i8 iSnapshotObjects[CN_ENUMS];
  
  vTrace("\n"_Stz);
  vTrace("%sWords created since last epoch:\n"_Stz,Stz::aTab(1));
  
  pBPA->vWordList(gpAttachMap->m_iEpoch);
  pBPA->vObjectSnapshot(&iSnapshotObjects[0]);
  
  vTrace("\n%sObjects created since last epoch:\n\n"_Stz,Stz::aTab(1));
  
  for (i8 i_Object = 0 ; i_Object < CN_ENUMS; i_Object++) {
    
    eCN eClass = (eCN)i_Object;
    
    if (iSnapshotObjects[eClass] == giCountObjects[eClass])
      continue;
    
    vTrace("%s  %12s (eCN %3lld) %-6lld \n"_Stz,
           Stz::aTab(1),
           aCN[eClass],
           eClass,
           iSnapshotObjects[eClass] - giCountObjects[eClass]
           );
    int iDebug = 1;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* epochTake -- take an epoch                                                 */
/*----------------------------------------------------------------------------*/
void Step::vBpEpochTake(MP_Task mpTask)
{
  ENTRY("vBpEpochTake"_Stz);
  
  // increment the epoch
  gpAttachMap->m_iEpoch++;
  BPA* pBPA = new BPA;
  
  pBPA->vObjectSnapshot(&giCountObjects[0]);
  
  delete pBPA;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.readBufferPool -- read the buffers from the file                        */
/*----------------------------------------------------------------------------*/
void Step::vBpReadBufferPool(MP_Task mpTask)
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
void Step::vBpWriteBufferPool(MP_Task mpTask)
{
  ENTRY("vBpWriteBufferPool"_Stz);
  
  BPA* pBPA = new BPA;
  
  //pBPA->vWriteBuffers("/tmp/buffers.dmp"_Stz);

  pBPA->vWritePersistentHeapMem("/tmp/persistentHeapMem.dmp"_Stz);

  delete pBPA;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::freeBuffers -- push number of free buffers in buffer pool onto stack   */
/*----------------------------------------------------------------------------*/
void Step::vFreeBuffers(MP_Task mpTask)
{
  ENTRY("vFreeBuffers"_Stz);
  
  i8 nFreeBuffers = gpBufferPool->nFreeCount();
  
  mpTask->m_pDataStack->vPushI8(nFreeBuffers);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::freePercent -- push free percent of buffer pool onto stack              */
/*----------------------------------------------------------------------------*/
void Step::vFreePercent(MP_Task mpTask)
{
  ENTRY("vFreePercent"_Stz);
  
  i8 iFreePercent = gpBufferPool->iFreePercent();
  
  mpTask->m_pDataStack->vPushI8(iFreePercent);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::freeSpace -- push free space in buffer pool onto stack                 */
/*----------------------------------------------------------------------------*/
void Step::vFreeSpace(MP_Task mpTask)
{
  ENTRY("vFreeSpace"_Stz);
  
  i8 iFreeSpace = gpBufferPool->iFreeSpace();
  i8 iFreeSpaceInPool = BPA::iPoolFree();

  mpTask->m_pDataStack->vPushI8(iFreeSpace + iFreeSpaceInPool);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::inUseSpace -- push in-use space in buffer pool onto stack              */
/*----------------------------------------------------------------------------*/
void Step::vInUseSpace(MP_Task mpTask)
{
  ENTRY("vInUseSpace"_Stz);
  
  i8 iInUseSpace = gpAttachMap->m_iRegionSize
                     - gpAttachMap->m_iJumboSize
                     - gpBufferPool->iFreeSpace()
                     - BPA::iPoolFree();
  
  mpTask->m_pDataStack->vPushI8(iInUseSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::snapshot -- push snapshot of buffer pool onto stack                    */
/*----------------------------------------------------------------------------*/
void Step::vSnapshot(MP_Task mpTask)
{
  ENTRY("vSnapshot"_Stz);
  
  i8 iCountObjects[CN_ENUMS];
  for (i8 index = 0; index < CN_ENUMS; index++)
    iCountObjects[index] = 0;
  
  bn nBuffer = 0;
  bn nBufferCount = gpBufferPool->nBufferCount() - 1;
  
  for (bn nBuffer = gpBufferPool->nBufferFirst();
       nBuffer <= nBufferCount;
       nBuffer++) {
    
    Buffer* pBuffer = gpBufferPool->pToBuffer(nBuffer);
    iCountObjects[pBuffer->eClass()]++;
  }
  
  gpAttachMap->m_xHeapMem.Transient.m_pSmall->vAnalyse(iCountObjects);
  gpAttachMap->m_xHeapMem.Transient.m_pMedium->vAnalyse(iCountObjects);
  gpAttachMap->m_xHeapMem.Transient.m_pLarge->vAnalyse(iCountObjects);
  gpAttachMap->m_xHeapMem.Transient.m_pJumbo->vAnalyse(iCountObjects);

  gpAttachMap->m_xHeapMem.Persistent.m_pSmall->vAnalyse(iCountObjects);
  gpAttachMap->m_xHeapMem.Persistent.m_pMedium->vAnalyse(iCountObjects);
  gpAttachMap->m_xHeapMem.Persistent.m_pLarge->vAnalyse(iCountObjects);
  gpAttachMap->m_xHeapMem.Persistent.m_pJumbo->vAnalyse(iCountObjects);
  
  for (i8 index = 0; index < CN_ENUMS; index++)
    mpTask->m_pDataStack->vPushI8(iCountObjects[index]);
  
  mpTask->m_pDataStack->vPushI8(CN_ENUMS);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::totalSpace -- push total space in buffer pool onto stack               */
/*----------------------------------------------------------------------------*/
void Step::vTotalSpace(MP_Task mpTask)
{
  ENTRY("vTotalSpace"_Stz);
  
  i8 iTotalSpace = gpAttachMap->m_iRegionSize - gpAttachMap->m_iJumboSize;
  
  mpTask->m_pDataStack->vPushI8(iTotalSpace);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
