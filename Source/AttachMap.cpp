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
/* print atachmap                                                             */
/*----------------------------------------------------------------------------*/
void AttachMap::vPrintAttachMap(void)
{
  vInfo("\n****ATTACH MAP****\n\n"_Stz);
  vInfo("Pid: %lld; \n"_Stz,   gpAttachMap->m_iPid);
  vInfo("UTC: %s; \n"_Stz,     gpAttachMap->m_sUTC.aStr());
  
  vInfo("RegionName : %s \n"_Stz,     gpAttachMap->m_sRegionName.aStr());
  vInfo("RegionSize : %lld \n"_Stz,   gpAttachMap->m_iRegionSize);
  vInfo("JumboSize  : %lld \n"_Stz,   gpAttachMap->m_iJumboSize);
  
  vInfo("\n"_Stz);
  vInfo("Epoch          : %lld \n"_Stz, gpAttachMap->m_iEpoch);
  vInfo("IndexEntries   : %lld \n"_Stz, gpAttachMap->m_nIndexEntries);
  vInfo("IndexLevels    : %lld \n"_Stz, gpAttachMap->m_nIndexLevels);
  vInfo("IndexOffsets   : %lld \n"_Stz, gpAttachMap->m_nIndexOffsets);
  vInfo("MaxQueue       : %lld \n"_Stz, gpAttachMap->m_nMaxQueue);
  vInfo("SplitRatio     : %lld \n"_Stz, gpAttachMap->m_nSplitRatio);
  
  vInfo("Verbose        : %s \n"_Stz, gpAttachMap->m_bVerbose ? "true"_Stz : "false"_Stz);
  vInfo("Tracepoint     : %s \n"_Stz, gpAttachMap->m_bTracepoint ? "true"_Stz : "false"_Stz);
  vInfo("Progress       : %s \n"_Stz, gpAttachMap->m_bProgress ? "true"_Stz : "false"_Stz);
  vInfo("Metadata       : %s \n"_Stz, gpAttachMap->m_bMetadata ? "true"_Stz : "false"_Stz);
  
  vInfo("\n"_Stz);
  vInfo("BufferPool         : %p \n"_Stz, gpAttachMap->m_pBufferPool);
  vInfo("TransientJumboHeap : %p \n"_Stz, gpAttachMap->m_pTransientJumboHeap);
  vInfo("TransientMediumHeap: %p \n"_Stz, gpAttachMap->m_pTransientMediumHeap);
  vInfo("TransientLargeHeap : %p \n"_Stz, gpAttachMap->m_pTransientLargeHeap);
  vInfo("TransientSmallHeap : %p \n"_Stz, gpAttachMap->m_pTransientSmallHeap);
  
  vInfo("\n"_Stz);
  vInfo("Total Nodes      : %lld \n"_Stz, gpAttachMap->m_pGrid->m_nNodes);
  vInfo("This node        : %lld \n"_Stz, gpAttachMap->m_pGrid->m_iThisNode);
  
  vInfo("Cluster started  : %s \n"_Stz,  gpAttachMap->m_pGrid->m_bHasStarted ? "true"_Stz : "false"_Stz);
  if (gpAttachMap->m_pGrid->m_bHasStarted)
    vInfo("Start node       : %lld \n"_Stz, gpAttachMap->m_pGrid->m_iStartNodeR35);
  
  vInfo("Slots per node   : %lld \n"_Stz, gpAttachMap->m_pGrid->m_nSlots);
  vInfo("Total Partitions : %lld \n"_Stz, gpAttachMap->m_pGrid->m_nPartitions);

  vInfo("Partitions on this node : %lld \n"_Stz, gpAttachMap->m_pGrid->m_nPartitions);
  for (i8 iIndex = 0; iIndex < gpAttachMap->m_pGrid->m_nSlots; iIndex++)
    vInfo("Slot#%lld      : partition %lld \n"_Stz, iIndex, gpAttachMap->m_pGrid->m_iPartition[iIndex]);
  
  vInfo("\n"_Stz);
  vInfo("Enclosed_by_Leading  : %c\n"_Stz,
        gpAttachMap->m_xFormatters.m_cEnclosedBy_Leading ? gpAttachMap->m_xFormatters.m_cEnclosedBy_Leading : '0');
  vInfo("Enclosed_by_trailing : %c\n"_Stz,
        gpAttachMap->m_xFormatters.m_cEnclosedBy_Trailing ? gpAttachMap->m_xFormatters.m_cEnclosedBy_Trailing : '0');
  
  vInfo("Escape_char          : %c\n"_Stz,
        gpAttachMap->m_xFormatters.m_cEscapeChar ? gpAttachMap->m_xFormatters.m_cEscapeChar : '0');
  vInfo("Delimiter            : %c\n"_Stz,
        gpAttachMap->m_xFormatters.m_cFieldDelimiter ? gpAttachMap->m_xFormatters.m_cFieldDelimiter : '0');
  vInfo("Record_terminator    : %c\n"_Stz,
        gpAttachMap->m_xFormatters.m_cRecordTerminator ? gpAttachMap->m_xFormatters.m_cRecordTerminator : '0');
  vInfo("Record_ends          : %s\n"_Stz, aRE[gpAttachMap->m_xFormatters.m_eRecordEnds]);
  
  vInfo("Date component 0     : %s\n"_Stz, aDC[gpAttachMap->m_xFormatters.m_eDateComponent[0]]);
  vInfo("Date component 1     : %s\n"_Stz, aDC[gpAttachMap->m_xFormatters.m_eDateComponent[1]]);
  vInfo("Date component 2     : %s\n"_Stz, aDC[gpAttachMap->m_xFormatters.m_eDateComponent[2]]);
  
  vInfo("Date_separator       : %c\n"_Stz,
        gpAttachMap->m_xFormatters.m_cDateSeparator ? gpAttachMap->m_xFormatters.m_cDateSeparator : '0');

  vInfo("\n****ATTACH MAP END****\n\n"_Stz);
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
    
  ASSERT(m_pWord);


  ASSERT(m_pPlanList);  
  ASSERT(m_pStepArray);
  
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
