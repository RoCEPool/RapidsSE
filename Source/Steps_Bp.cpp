/*============================================================================*/
/* Steps_Bp.cpp - buffer pool step group                                      */
/*============================================================================*/

#include "Steps.h"

static BPA* gpBPA = nullptr;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Bp(void)
{
  vAddStep("bp::analyze"_Stz,        &Step::vBpAnalyze);
  vAddStep("bp::freeBuffers"_Stz,    &Step::vBpFreeBuffers);
  vAddStep("bp::freePercent"_Stz,    &Step::vBpFreePercent);
  vAddStep("bp::freeSpace"_Stz,      &Step::vBpFreeSpace);
  vAddStep("bp::inUseSpace"_Stz,     &Step::vBpInUseSpace);
  vAddStep("bp::readBufferPool"_Stz ,&Step::vBpReadBufferPool);
  vAddStep("bp::totalSpace"_Stz,     &Step::vBpTotalSpace);
  vAddStep("bp::writeBufferPool"_Stz,&Step::vBpWriteBufferPool);

  vAddStep("bpa"_Stz,                &Step::vBpAnalyze);
  vAddStep("bpr"_Stz,                &Step::vBpReadBufferPool);
  vAddStep("bpw"_Stz,                &Step::vBpWriteBufferPool);
 
  vAddStep("epochDump"_Stz,          &Step::vBpEpochDump);
  vAddStep("epochTake"_Stz,          &Step::vBpEpochTake);
}

/*----------------------------------------------------------------------------*/
/* bp.analyze -- buffer pool analyze                                          */
/*----------------------------------------------------------------------------*/
void Step::vBpAnalyze(void)
{
  ENTRY();
  
  PoolHelper::vPoolAnalyze();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* epochDump -- dump an epoch                                                 */
/*----------------------------------------------------------------------------*/
void Step::vBpEpochDump(void)
{
  ENTRY();
  
  if (! gpBPA)
    gpBPA = new BPA;
  
  s48K sPortrait;
  
  gpBPA->vEpochDump(sPortrait);
  
  vTrace("%s\n"_Stz,sPortrait.szStz());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* epochTake -- take an epoch                                                 */
/*----------------------------------------------------------------------------*/
void Step::vBpEpochTake(void)
{
  ENTRY();
  
  if (! gpBPA)
    gpBPA = new BPA;
  
  gpBPA->vEpochTake();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.readBufferPool -- read the buffers from the file                        */
/*----------------------------------------------------------------------------*/
void Step::vBpReadBufferPool(void)
{
  ENTRY();
  
  PoolHelper::vReadBuffers("../tmp/buffers.dmp"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.writeBufferPool -- write the buffer pool buffers to a file              */
/*----------------------------------------------------------------------------*/
void Step::vBpWriteBufferPool(void)
{
  ENTRY();
  
  PoolHelper::vWritePersistentHeapMem("../tmp/persistentHeapMem.dmp"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::freeBuffers -- push number of free buffers in buffer pool onto stack   */
/*----------------------------------------------------------------------------*/
void Step::vBpFreeBuffers(void)
{
  ENTRY();
  
  i8 nFreeBuffers = gpBufferPool->nFreeCount();
  
  m_pDataStack->vPushInt(nFreeBuffers);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::freePercent -- push free percent of buffer pool onto stack              */
/*----------------------------------------------------------------------------*/
void Step::vBpFreePercent(void)
{
  ENTRY();
  
  i8 iFreePercent = gpBufferPool->iFreePercent();
  
  m_pDataStack->vPushInt(iFreePercent);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::freeSpace -- push free space in buffer pool onto stack                 */
/*----------------------------------------------------------------------------*/
void Step::vBpFreeSpace(void)
{
  ENTRY();
  
  i8 iFreeSpace = gpBufferPool->iFreeSpace();
  i8 iFreeSpaceInPool = PoolHelper::iPoolFree();

  m_pDataStack->vPushInt(iFreeSpace + iFreeSpaceInPool);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::inUseSpace -- push in-use space in buffer pool onto stack              */
/*----------------------------------------------------------------------------*/
void Step::vBpInUseSpace(void)
{
  ENTRY();
  
  i8 iInUseSpace = AM::iRegionSize()
                     - gpBufferPool->iFreeSpace()
                     - PoolHelper::iPoolFree();
  
  m_pDataStack->vPushInt(iInUseSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::totalSpace -- push total space in buffer pool onto stack               */
/*----------------------------------------------------------------------------*/
void Step::vBpTotalSpace(void)
{
  ENTRY();
  
  i8 iTotalSpace = AM::iRegionSize();
  
  m_pDataStack->vPushInt(iTotalSpace);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
