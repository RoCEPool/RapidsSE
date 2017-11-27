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
  
  ASSERT(m_pPersistentJumboHeap);
  ASSERT(m_pPersistentMediumHeap);
  ASSERT(m_pPersistentLargeHeap);
  ASSERT(m_pPersistentSmallHeap);
  
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
  
  m_bIndexSpace   = true;
  m_nIndexEntries = -1;
  m_nIndexLevels  = 3;
  m_nIndexOffsets = m_nIndexLevels - 1;
  
  m_iStop       = 0;
  m_iQuit       = 0;
  
  //m_xLocks
  
  // not used yet
  //m_pBitMap = new BitMap;
  
  // initialize the plan number
  m_zPlanId = m_pGrid->iThisNode() << 48;  
}

/*----------------------------------------------------------------------------*/
/* dump this attachmap                                                        */
/*----------------------------------------------------------------------------*/
void AttachMap::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(CN_ATTACHMAP == eClass);
  AttachMap* pAttachMap = reinterpret_cast<AttachMap*>(pObject);
  ASSERT(pAttachMap->eClass() == eClass);
  
  vTrace("pid %lld utc %s regionSize %lld\n"_Stz,
         pAttachMap->m_iPid,
         pAttachMap->m_sUTC.aChar(),
         pAttachMap->m_iRegionSize);
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
