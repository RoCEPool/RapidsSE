/*============================================================================*/
/* AttachMap.cpp - map of addresses when process attaches                     */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap*  gpAttachMap;
extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
AttachMap* StorageEngine::_pAttachMap(void* aAddr)
{
  AttachMap* pAttachMap = reinterpret_cast<AttachMap*>(aAddr);
  
  RASSERT_TYPE(pAttachMap,CN_ATTACHMAP);
  
  return pAttachMap;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* AttachMap::operator new(size_t)
{
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
  
  gpAttachMap = this;
  
  // the attach map must always be the first buffer in the buffer pool
  ASSERT(nBufferNo() == 1_bn);
  
  m_xMainDic.vName("<main>"_Stz);
  
  // create the carry over (which records global pointers restored on restart)
  m_pCarryOver = new CarryOver;
  
  // create the transient heaps
  m_xHeapMem.vInit();
  
  // create the persistent heaps
  m_xHeapMem.Persistent.m_pJumbo  =
    new Heap(CN_JUMBOPERSISTENT,  C_SIZE_JUMBO_OBJECT);
  m_xHeapMem.Persistent.m_pLarge  =
    new Heap(CN_LARGEPERSISTENT,  C_SIZE_LARGE_OBJECT);
  m_xHeapMem.Persistent.m_pMedium =
    new Heap(CN_MEDIUMPERSISTENT, C_SIZE_MEDIUM_OBJECT);
  m_xHeapMem.Persistent.m_pSmall  =
    new Heap(CN_SMALLPERSISTENT,  C_SIZE_SMALL_OBJECT);

  m_bMarkJobAll = false;
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++)
    m_bMarkJob[i_Class] = false;

  _checkInitialised_(CN_ATTACHMAP);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
AttachMap::~AttachMap(void) noexcept(false)
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
  vInfo("UTC: %s; \n"_Stz,     gpAttachMap->m_sUTC.aStz());
  
  vInfo("RegionName   : %s \n"_Stz,     gpAttachMap->m_sRegionName.aStz());
  vInfo("RegionSize   : %lld \n"_Stz,   gpAttachMap->m_iRegionSize);
  vInfo("JumboSize    : %lld \n"_Stz,   gpAttachMap->m_iJumboSize);
  vInfo("SubIngesters : %lld \n"_Stz,   gpAttachMap->m_nSubIngesters);
  vInfo("SubLoaders   : %lld \n"_Stz,   gpAttachMap->m_nSubLoaders);

  vInfo("\n"_Stz);
  vInfo("Epoch          : %lld \n"_Stz, gpAttachMap->m_iEpoch);
  vInfo("BootError      : %lld \n"_Stz, gpAttachMap->m_bBootError);
  vInfo("IndexEntries   : %lld \n"_Stz, gpAttachMap->m_nIndexEntries);
  vInfo("IndexLevels    : %lld \n"_Stz, gpAttachMap->m_nIndexLevels);
  vInfo("IndexOffsets   : %lld \n"_Stz, gpAttachMap->m_nIndexOffsets);
  
  vInfo("Verbose        : %s \n"_Stz, gpAttachMap->m_bVerbose ? "true"_Stz : "false"_Stz);
  vInfo("Tracepoint     : %s \n"_Stz, gpAttachMap->m_bTracepoint ? "true"_Stz : "false"_Stz);
  vInfo("Progress       : %s \n"_Stz, gpAttachMap->m_bProgress ? "true"_Stz : "false"_Stz);
  vInfo("Metadata       : %s \n"_Stz, gpAttachMap->m_bMetadata ? "true"_Stz : "false"_Stz);
  
  vInfo("\n"_Stz);
  vInfo("BufferPool         : %p \n"_Stz, gpAttachMap->m_pBufferPool);
  vInfo("TransientJumboHeap : %p \n"_Stz, gpAttachMap->m_xHeapMem.Transient.m_pJumbo);
  vInfo("TransientMediumHeap: %p \n"_Stz, gpAttachMap->m_xHeapMem.Transient.m_pMedium);
  vInfo("TransientLargeHeap : %p \n"_Stz, gpAttachMap->m_xHeapMem.Transient.m_pLarge);
  vInfo("TransientSmallHeap : %p \n"_Stz, gpAttachMap->m_xHeapMem.Transient.m_pSmall);
  
  vInfo("\n"_Stz);
  vInfo("Total Nodes      : %lld \n"_Stz, gpAttachMap->m_pGrid->m_nNodes);
  vInfo("This node        : %lld \n"_Stz, gpAttachMap->m_pGrid->m_iThisNode);
  
  vInfo("Cluster started  : %s \n"_Stz,  gpAttachMap->m_pGrid->m_bHasStarted ? "true"_Stz : "false"_Stz);
  if (gpAttachMap->m_pGrid->m_bHasStarted)
    vInfo("Start node       : %lld \n"_Stz, gpAttachMap->m_pGrid->m_iStartNode);
  
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
  vInfo("Record_end           : %s\n"_Stz, aRE[gpAttachMap->m_xFormatters.m_eRecordEnd]);
  
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
void AttachMap::vRecordGlobals(Stz* aRegionName,
                               i8 iRegionSize, i8 iJumboSize,
                               i8 nSubIngesters, i8 nSubLoaders)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);

  if (m_bVerbose)
    vTrace("[[mainThread]] Recording globals...\n"_Stz);
  
  m_iPid = Process::iGetPid();
  m_oSince.vNow();
  m_oTime.vSetUTC(&m_oSince);
  m_oTime.vUTC(m_sUTC);
  
  m_sRegionName   = aRegionName;
  m_iRegionSize   = iRegionSize;
  m_iJumboSize    = iJumboSize;
  m_nSubIngesters = nSubIngesters;
  m_nSubLoaders   = nSubLoaders;

  m_pBufferPool = gpBufferPool;
  
  m_iEpoch      = 0;
  
  ASSERT(m_xHeapMem.Transient.m_pJumbo);
  ASSERT(m_xHeapMem.Transient.m_pMedium);
  ASSERT(m_xHeapMem.Transient.m_pLarge);
  ASSERT(m_xHeapMem.Transient.m_pSmall);
  
  ASSERT(m_xHeapMem.Persistent.m_pJumbo);
  ASSERT(m_xHeapMem.Persistent.m_pMedium);
  ASSERT(m_xHeapMem.Persistent.m_pLarge);
  ASSERT(m_xHeapMem.Persistent.m_pSmall);
  
  m_pApiState = new ApiState;
  
  // create the tx sets
  m_pTxSet = new TxSet(gpAttachMap->m_pGrid->iThisNode());
  
  ASSERT(m_pGrid);
    
  ASSERT(m_pWord);


  //ASSERT(m_pStepTiming);
    
  m_nIndexLevels  = 3;
  m_nIndexOffsets = 3;
  //m_nBuffersPending;
  
  m_iStop       = 0;
  m_iQuit       = 0;
  
  
  //m_xDdlLock.vInitLock(LK_DDL);
  //m_xDicLock.vInitLock(LK_DIC);
    
  // initialize the plan number
  m_ziPlanId = m_pGrid->iThisNode() << 48;

  ASSERT( ! m_iSchEnqueueTask );
  ASSERT( ! m_iSchEnqueueTaskWait );
  ASSERT( ! m_iSchGetTask );
  ASSERT( ! m_iSchGetTaskWait );
  
  //m_iMemCopyTotal );
  //m_iMemMoveTotal );
  //m_iMemScribbleTotal );
  //m_iMemSetTotal );
  //m_iMemZeroTotal );

  //m_iaMemCopy[CN_ENUMS];
  //m_iaMemMove[CN_ENUMS];
  //m_xMemScribbl/e[CN_ENUMS];
  //m_iaMemSet[CN_ENUMS];
  //m_iaMemZero[CN_ENUMS];
  
  m_bMarkApiAll = false;
  m_bMarkJobAll = false;
}

/*----------------------------------------------------------------------------*/
/* dump this attachmap                                                        */
/*----------------------------------------------------------------------------*/
void AttachMap::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  AttachMap* pAttachMap = _pAttachMap(pObject);
  
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
/* find api which matches given name                                          */
/*----------------------------------------------------------------------------*/
eAC AttachMap::eFindApi(Base::Stz* aApiNm)
{
  _precondition_(_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  for (i8 i_Api = 0; i_Api < AC_ENUMS; i_Api++)
    if (aApiNm->bSame(aAC[i_Api]))
      return (eAC)i_Api;
  
  return AC_NONE;
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
