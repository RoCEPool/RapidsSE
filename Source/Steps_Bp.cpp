/*============================================================================*/
/* Steps_Bp.cpp - buffer pool step group                                      */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Bp(void)
{
  vAddStep("bp::analyze"_Stz,        &Step::vBpAnalyze);
  vAddStep("bp::dump"_Stz,           &Step::vBpDump);
  vAddStep("bp::freeBuffers"_Stz,    &Step::vBpFreeBuffers);
  vAddStep("bp::freePercent"_Stz,    &Step::vBpFreePercent);
  vAddStep("bp::freeSpace"_Stz,      &Step::vBpFreeSpace);
  vAddStep("bp::inUseSpace"_Stz,     &Step::vBpInUseSpace);
  vAddStep("bp::readBufferPool"_Stz ,&Step::vBpReadBufferPool);
  vAddStep("bp::totalSpace"_Stz,     &Step::vBpTotalSpace);
  vAddStep("bp::writeBufferPool"_Stz,&Step::vBpWriteBufferPool);

  vAddStep("bpa"_Stz,                &Step::vBpAnalyze);
  vAddStep("bpd"_Stz,                &Step::vBpDump);
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
  ENTRY("vBpa"_Stz);
  
  BPA::vPoolAnalyze();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.dump -- dump of objects in the buffer pool                              */
/*----------------------------------------------------------------------------*/
void Step::vBpDump(void)
{
  ENTRY("vBpDump"_Stz);
  
  BPA::vPoolDump();
  
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* epochDump -- dump an epoch                                                 */
/*----------------------------------------------------------------------------*/
void Step::vBpEpochDump(void)
{
  ENTRY("vBpEpochDump"_Stz);
  
  s48K sPortrait;
  
  BPA::vEpochDump(sPortrait);
  
  vTrace("%s\n"_Stz,sPortrait.szStz());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* epochTake -- take an epoch                                                 */
/*----------------------------------------------------------------------------*/
void Step::vBpEpochTake(void)
{
  ENTRY("vBpEpochTake"_Stz);
  
  BPA::vEpochTake();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.readBufferPool -- read the buffers from the file                        */
/*----------------------------------------------------------------------------*/
void Step::vBpReadBufferPool(void)
{
  ENTRY("vBpReadBufferPool"_Stz);
  
  BPA::vReadBuffers("/tmp/buffers.dmp"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.writeBufferPool -- write the buffer pool buffers to a file              */
/*----------------------------------------------------------------------------*/
void Step::vBpWriteBufferPool(void)
{
  ENTRY("vBpWriteBufferPool"_Stz);
  
  BPA::vWritePersistentHeapMem("/tmp/persistentHeapMem.dmp"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::freeBuffers -- push number of free buffers in buffer pool onto stack   */
/*----------------------------------------------------------------------------*/
void Step::vBpFreeBuffers(void)
{
  ENTRY("vBpFreeBuffers"_Stz);
  
  i8 nFreeBuffers = gpBufferPool->nFreeCount();
  
  m_pDataStack->vPushI8(nFreeBuffers);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::freePercent -- push free percent of buffer pool onto stack              */
/*----------------------------------------------------------------------------*/
void Step::vBpFreePercent(void)
{
  ENTRY("vBpFreePercent"_Stz);
  
  i8 iFreePercent = gpBufferPool->iFreePercent();
  
  m_pDataStack->vPushI8(iFreePercent);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::freeSpace -- push free space in buffer pool onto stack                 */
/*----------------------------------------------------------------------------*/
void Step::vBpFreeSpace(void)
{
  ENTRY("vBpFreeSpace"_Stz);
  
  i8 iFreeSpace = gpBufferPool->iFreeSpace();
  i8 iFreeSpaceInPool = BPA::iPoolFree();

  m_pDataStack->vPushI8(iFreeSpace + iFreeSpaceInPool);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::inUseSpace -- push in-use space in buffer pool onto stack              */
/*----------------------------------------------------------------------------*/
void Step::vBpInUseSpace(void)
{
  ENTRY("vBpInUseSpace"_Stz);
  
  i8 iInUseSpace = AM::iRegionSize()
                     - AM::iJumboSize()
                     - gpBufferPool->iFreeSpace()
                     - BPA::iPoolFree();
  
  m_pDataStack->vPushI8(iInUseSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::totalSpace -- push total space in buffer pool onto stack               */
/*----------------------------------------------------------------------------*/
void Step::vBpTotalSpace(void)
{
  ENTRY("vBpTotalSpace"_Stz);
  
  i8 iTotalSpace = AM::iRegionSize()
                     - AM::iJumboSize();
  
  m_pDataStack->vPushI8(iTotalSpace);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
