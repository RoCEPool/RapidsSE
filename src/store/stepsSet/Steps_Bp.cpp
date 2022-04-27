/*============================================================================*/
/* Steps_Bp.cpp - buffer pool step group                                      */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Bp(void)
{
  vAddStep("bp::freePoolBuffers"_Stz,    &Step::vBpFreePoolBuffers);
  vAddStep("bp::freePercent"_Stz,    &Step::vBpFreePercent);
  vAddStep("bp::freeSpace"_Stz,      &Step::vBpFreeSpace);
  vAddStep("bp::inUseSpace"_Stz,     &Step::vBpInUseSpace);
  vAddStep("bp::readBufferPool"_Stz ,&Step::vBpReadBufferPool);
  vAddStep("bp::totalSpace"_Stz,     &Step::vBpTotalSpace);
  vAddStep("bp::writeBufferPool"_Stz,&Step::vBpWriteBufferPool);

  vAddStep("bpr"_Stz,                &Step::vBpReadBufferPool);
  vAddStep("bpw"_Stz,                &Step::vBpWriteBufferPool);
}

/*----------------------------------------------------------------------------*/
/* bp.readBufferPool -- read the buffers from the file                        */
/*----------------------------------------------------------------------------*/
void Step::vBpReadBufferPool(void)
{
  ENTRY();
  
  PersistenceHelper::vReadBuffers("../tmp/buffers.dmp"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp.writeBufferPool -- write the buffer pool buffers to a file              */
/*----------------------------------------------------------------------------*/
void Step::vBpWriteBufferPool(void)
{
  ENTRY();
  
  PersistenceHelper::vWritePersistentHeaps("../tmp/persistentHeaps.dmp"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bp::freePoolBuffers -- push number of free buffers in buffer pool onto stack   */
/*----------------------------------------------------------------------------*/
void Step::vBpFreePoolBuffers(void)
{
  ENTRY();
  
  i8 nFreePoolBuffers = gpBufferPool->nFreeCount();
  
  m_pDataStack->vPushInt(nFreePoolBuffers);
  
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
