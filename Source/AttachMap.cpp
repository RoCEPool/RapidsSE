/*============================================================================*/
/* AttachMap.cpp - map of addresses when process attaches                     */
/*============================================================================*/

#include "StorageEngine.h"

extern BufferPool*  gpBufferPool;

extern Heap*        gpJumboHeap;
extern Heap*        gpMediumHeap;
extern Heap*        gpSmallHeap;
extern Dic*         gpMainDic;

extern TxCollection* gpTxCollection;
extern TxMap*        gpTxMap;
extern Grid*         gpGrid;

extern TaskList*    gpTaskList;
extern SEitfList*   gpSEitfList;
extern Arguments    gxArguments;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* AttachMap::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_ATTACHMAP);
  
  return pPoolCheck<void*>(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void AttachMap::operator delete(void* pBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pBufferAddress);
  
  Heap::vFree(pBuffer);
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
  
  for (i8 iPartition = 0; iPartition < C_MAX_PARTITIONS_PER_NODE; iPartition++) {
    m_iPartition[iPartition] = -1;
  }
  
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

  gpJumboHeap   = m_pJumboHeap;
  gpMediumHeap  = m_pMediumHeap;
  gpSmallHeap   = m_pSmallHeap;
  gpMainDic     = &m_xMainDic;
  gpTxCollection = m_pTxCollection;
  gpTxMap        = m_pTxMap;
  gpGrid         = m_pGrid;
  gpTaskList     = m_pTaskList;
  gpSEitfList    = m_pSEitfList;
}

/*----------------------------------------------------------------------------*/
/* record all the needed globals                                              */
/*----------------------------------------------------------------------------*/
void AttachMap::vRecordGlobals(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);

  m_iPid = Process::iGetPid();
  
  m_xSince.vSet();
  m_xTime.vSet(&m_xSince);
  m_xTime.vUTC(m_sUTC);
  
  m_iRegionSize = gxArguments.m_iRegionSize;
  m_iPartitions = gxArguments.m_nPartitions;
  m_iNodes      = gxArguments.m_nNodes;
  m_iEpoch      = 0;
  
  /*
  i8 iPartitions = 0;
  i8 iTotalPartitions = m_iPartitions * m_iNodes;
  for (i8 iPartition = 0; iPartition < iTotalPartitions; iPartition++) {
    if (iPartition % m_iNodes == gxArguments.m_iNode)
      m_iPartition[iPartitions++] = iPartition;
  }
  ASSERT(iPartitions == m_iPartitions);
  */
  for (i8 iSlot = 0; iSlot < m_iPartitions; iSlot++) {
    m_iPartition[iSlot] = Partition::iSlotToPartition(iSlot);
  }
  
  m_pBufferPool = gpBufferPool;
  
  m_pJumboHeap  = gpJumboHeap;
  m_pMediumHeap = gpMediumHeap;
  m_pSmallHeap  = gpSmallHeap;
  
  m_pTxCollection = gpTxCollection;
  m_pTxMap        = gpTxMap;
  m_pGrid         = gpGrid;
  m_pTaskList     = gpTaskList;
  m_pSEitfList    = gpSEitfList;
  
  m_iStop       = 0;
  m_iQuit       = 0;
}

/*----------------------------------------------------------------------------*/
/* dump this attachmap                                                        */
/*----------------------------------------------------------------------------*/
void AttachMap::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(CN_ATTACHMAP == eClass);
  AttachMap* pAttachMap = reinterpret_cast<AttachMap*>(pObject);
  ASSERT(pAttachMap->eClass() == eClass);
  
  vTrace("%s%p %-16s: ",pTab,pObject,pCN[eClass]);
         
  vTrace("pid %lld utc %s regionSize %lld partitions %lld\n",
         pAttachMap->m_iPid,
         pAttachMap->m_sUTC.a(),
         pAttachMap->m_iRegionSize,
         pAttachMap->m_iPartitions);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* AttachMap::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
