/*============================================================================*/
/* AttachMap.cpp - map of addresses when process attaches                     */
/*============================================================================*/

#include "StorageEngine.h"

extern BufferPool*  gpBufferPool;

extern Heap*        gpJumboHeap;
extern Heap*        gpMediumHeap;
extern Heap*        gpSmallHeap;
extern Heap*        gpLargeHeap;
extern Dic*         gpMainDic;

extern TxSet*        gpTxSet;
extern Grid*         gpGrid;
extern PartitionScheduler*    gpPartitionScheduler;
extern ServerScheduler*    gpServerScheduler;

extern TaskList*    gpTaskList;
extern PlanList*    gpPlanList;
extern SEitfList*   gpSEitfList;

extern bool         gbVerbose;
extern bool         gbTracepoint;
extern bool         gbProgress;
extern bool         gbMetadata;

extern bool         gbIndexSpace;
extern i8           gnIndexEntries;
extern i8           gnIndexLevels;
extern i8           gnIndexOffsets;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* AttachMap::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  AttachMap* pAttachMap = (AttachMap*)gpBufferPool->pAllocBuffer();
  
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

  gpJumboHeap   = m_pJumboHeap;
  gpMediumHeap  = m_pMediumHeap;
  gpLargeHeap   = m_pLargeHeap;
  gpSmallHeap   = m_pSmallHeap;
  gpMainDic     = &m_xMainDic;
  gpTxSet        = m_pTxSet;
  gpGrid         = m_pGrid;
  gpPartitionScheduler = m_pPartitionScheduler;
  gpServerScheduler    = m_pServerScheduler;
  gpTaskList     = m_pTaskList;
  gpPlanList     = m_pPlanList;
  gpSEitfList    = m_pSEitfList;
  gbVerbose      = m_bVerbose;
  gbTracepoint   = m_bTracepoint;
  gbProgress     = m_bProgress;
  gbMetadata     = m_bMetadata;
  
  gbIndexSpace   = m_bIndexSpace;
  gnIndexEntries = m_nIndexEntries;
  gnIndexLevels  = m_nIndexLevels;
  gnIndexOffsets = gnIndexLevels - 1;
}

/*----------------------------------------------------------------------------*/
/* record all the needed globals                                              */
/*----------------------------------------------------------------------------*/
void AttachMap::vRecordGlobals(Stz* aRegionName, i8 iRegionSize, i8 iJumboSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);

  if (gbVerbose)
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
  
  m_pJumboHeap  = gpJumboHeap;
  m_pMediumHeap = gpMediumHeap;
  m_pLargeHeap  = gpLargeHeap;
  m_pSmallHeap  = gpSmallHeap;
  
  m_pTxSet        = gpTxSet;
  m_pGrid         = gpGrid;
  m_pPartitionScheduler = gpPartitionScheduler;
  m_pServerScheduler    = gpServerScheduler;
  m_pTaskList     = gpTaskList;
  m_pPlanList     = gpPlanList;
  m_pSEitfList    = gpSEitfList;
  m_bVerbose      = gbVerbose;
  m_bProgress     = gbProgress;
  m_bMetadata     = gbMetadata;
  
  m_bIndexSpace   = true;
  m_nIndexEntries = -1;
  m_nIndexLevels  = 3;
  m_nIndexOffsets = m_nIndexLevels - 1;
  
  m_iStop       = 0;
  m_iQuit       = 0;
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
