/*============================================================================*/
/* AttachMap.cpp - map of addresses when process attaches                     */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap*  gpAttachMap;
extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* AttachMap::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  AttachMap* pAttachMap = (AttachMap*)gpBufferPool->pAllocAttachMapBuffer();
  
  return pPoolCheck<void*>(pAttachMap);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void AttachMap::operator delete(void* pAttachMapAddress)
{
  AttachMap* pAttachMap = static_cast<AttachMap*>(pAttachMapAddress);
  
  gpBufferPool->vFree((Buffer*)pAttachMap);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
AttachMap::AttachMap(void)
: Buffer(CN_ATTACHMAP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP);
  
  // the attach map must always be the first buffer in the buffer pool
  ASSERT(nBufferNo() == 1_bn);
  
  m_xMainDic.vName("<main>"_Stz);
  
  _checkInitialised_(CN_ATTACHMAP);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
AttachMap::~AttachMap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get all the needed globals                                                 */
/*----------------------------------------------------------------------------*/
void AttachMap::vGetGlobals(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* record all the needed globals                                              */
/*----------------------------------------------------------------------------*/
void AttachMap::vRecordGlobals(Stz* aRegionName, i8 iRegionSize, i8 iJumboSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);

  if (m_bVerbose)
    vTrace("[[mainThread]] Recording globals...\n"_Stz);
  
  m_iPid = Process::iGetPid();
  
  m_xSince.vSet();
  m_xTime.vSet(&m_xSince);
  m_xTime.vUTC(m_sUTC);
  
  m_sRegionName = aRegionName;
  m_iRegionSize = iRegionSize;
  m_iJumboSize  = iJumboSize;
  m_iEpoch      = 0;
  
  m_pBufferPool = gpBufferPool;
  
  ASSERT(m_pTransientJumboHeap);
  ASSERT(m_pTransientMediumHeap);
  ASSERT(m_pTransientLargeHeap);
  ASSERT(m_pTransientSmallHeap);
  
  ASSERT(m_pCarryOver->m_pPersistentJumboHeap);
  ASSERT(m_pCarryOver->m_pPersistentMediumHeap);
  ASSERT(m_pCarryOver->m_pPersistentLargeHeap);
  ASSERT(m_pCarryOver->m_pPersistentSmallHeap);
  
  // create the tx sets
  m_pTxSet = new TxSet(gpAttachMap->m_pGrid->iThisNode());
  
  ASSERT(m_pGrid );
  
  m_pPartitionScheduler = new PartitionScheduler;
  m_pServerScheduler = new ServerScheduler;
  
  ASSERT(m_pWord);
  ASSERT(m_pTaskList);
  ASSERT(m_pThreadList);
  ASSERT(m_pPlanList);  
  ASSERT(m_pStepArray);
  ASSERT(m_pSEitfList);
  
  //m_nMaxQueue = 0;
  m_nIndexLevels  = 3;
  m_nIndexOffsets = 3;
  //m_nSplitRatio   = 2;
  //m_nBuffersPending;
  
  m_iStop       = 0;
  m_iQuit       = 0;
  
  //m_xLocks
  
  //m_xDdlLock.vInitLock(LK_DDL);
  //m_xDicLock.vInitLock(LK_DIC);
  
  // not used yet
  //m_pBitMap = new BitMap;
  
  // initialize the plan number
  m_zPlanId = m_pGrid->iThisNode() << 48;

  ASSERT( ! m_iSchEnqueueTask );
  ASSERT( ! m_iSchEnqueueTaskWait );
  ASSERT( ! m_iSchGetTask );
  ASSERT( ! m_iSchGetTaskWait );
  
  //m_iMemCopyBytes );
  //m_iMemMoveBytes );
  //m_iMemScribbleBytes );
  //m_iMemSetBytes );
  //m_iMemZeroBytes );

  //m_xMemCopy[CN_ENUMS];
  //m_xMemMove[CN_ENUMS];
  //m_xMemScribbl/e[CN_ENUMS];
  //m_xMemSet[CN_ENUMS];
  //m_xMemZero[CN_ENUMS];
  
}

/*----------------------------------------------------------------------------*/
/* dump this attachmap                                                        */
/*----------------------------------------------------------------------------*/
void AttachMap::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_ATTACHMAP == eClass);
  AttachMap* pAttachMap = reinterpret_cast<AttachMap*>(pObject);
  RASSERT(pAttachMap->eClass() == eClass);
  
  vTrace("pid %lld utc %s regionSize %lld\n"_Stz,
         pAttachMap->m_iPid,
         pAttachMap->m_sUTC.aChar(),
         pAttachMap->m_iRegionSize);
}

/*----------------------------------------------------------------------------*/
/* print arguments                                                            */
/*----------------------------------------------------------------------------*/
void AttachMap::vPrintStartupInfo(void)
{
  _precondition_(_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  vTrace("Args:\n"_Stz);
  
  vTrace("  --name %s\n"_Stz,m_pGrid->pThisNode()->aNodeName()->aChar());
  vTrace("  --region %s\n"_Stz,m_sRegionName.aChar());
  vTrace("  --size %lld\n"_Stz,m_iRegionSize);
  vTrace("  --partitionsPerNode %lld\n"_Stz,m_pGrid->m_nSlots);

}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* AttachMap::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
