/*============================================================================*/
/* AttachMap.cpp - map of addresses when process attaches                     */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap*  gpAttachMap;
extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* -- LifeCycle --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
  
  gpBufferPool->vFree((PoolBuffer*)pAttachMap);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
AttachMap::AttachMap(void)
: Focal(CN_ATTACHMAP,0)
, m_okDdlLock(LK_DDL,nullptr,this)
, m_okDicLock(LK_DIC,nullptr,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP);
  
  gpAttachMap = this;
  
  // the attach map must always be the first buffer in the buffer pool
  //ASSERT(nPoolBufferNo() == 1_bn);
  
  m_iTimePoint = AM::iRdTimePoint();

  
  m_oRootDic.vName("<root>"_Stz);
  
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
/* set index entries                                                          */
/*----------------------------------------------------------------------------*/
void AttachMap::vIndexEntries(i8 nIndexEntries)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);
 
  m_nIndexEntries = nIndexEntries;
}

/*----------------------------------------------------------------------------*/
/* get pid                                                                    */
/*----------------------------------------------------------------------------*/
i8 AttachMap::iPid(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  return m_iPid;
}

/*----------------------------------------------------------------------------*/
/* get next temp name                                                         */
/*----------------------------------------------------------------------------*/
void AttachMap::vNextTempName(s32& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  m_oTempName.vNextName(sName);
}

/*----------------------------------------------------------------------------*/
/* print atachmap                                                             */
/*----------------------------------------------------------------------------*/
void AttachMap::vPrintAttachMap(void)
{
  vInfo("\n****ATTACH MAP****\n\n"_Stz);
  vInfo("Pid: %lld; \n"_Stz,   gpAttachMap->m_iPid);
  vInfo("UTC: %s; \n"_Stz,     gpAttachMap->m_oNow.sz());
  
  vInfo("RegionName   : %s \n"_Stz,     gpAttachMap->Arg.m_sRegionName.aStz());
  vInfo("RegionSize   : %lld \n"_Stz,   gpAttachMap->Arg.m_iRegionSize);
  vInfo("JumboSize    : %lld \n"_Stz,   gpAttachMap->Arg.m_iJumboSize);
  vInfo("SubIngesters : %lld \n"_Stz,   gpAttachMap->Arg.m_nSubIngesters);
  vInfo("SubLoaders   : %lld \n"_Stz,   gpAttachMap->Arg.m_nSubLoaders);

  vInfo("\n"_Stz);
  vInfo("Epoch          : %lld \n"_Stz, gpAttachMap->m_iEpoch);
  vInfo("BootError      : %lld \n"_Stz, gpAttachMap->m_bBootError);
  vInfo("IndexEntries   : %lld \n"_Stz, gpAttachMap->m_nIndexEntries);
  vInfo("IndexOffsets   : %lld \n"_Stz, C_INDEX_LEVELS);
  
  vInfo("Verbose        : %s \n"_Stz, AM::bVerbose() ? "true"_Stz : "false"_Stz);
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
  vInfo("Total Nodes      : %lld \n"_Stz, AM::pGrid()->m_nNodes);
  vInfo("This node        : %lld \n"_Stz, AM::pGrid()->m_iThisNode);
  
  vInfo("Cluster started  : %s \n"_Stz,  AM::pGrid()->m_bHasStarted ? "true"_Stz : "false"_Stz);
  if (AM::pGrid()->m_bHasStarted)
    vInfo("Start node       : %lld \n"_Stz, AM::pGrid()->m_iStartNode);
  
  vInfo("Slots per node   : %lld \n"_Stz, AM::pGrid()->m_nSlotsPerNode);
  vInfo("Total Partitions : %lld \n"_Stz, AM::pGrid()->m_nPartitions);

  vInfo("Partitions on this node : %lld \n"_Stz, AM::pGrid()->m_nPartitions);
  for (i8 iIndex = 0; iIndex < AM::pGrid()->m_nSlotsPerNode; iIndex++)
    vInfo("Slot#%lld      : partition %lld \n"_Stz, iIndex, AM::pGrid()->m_iPartition[iIndex]);
  
  vInfo("\n"_Stz);
  vInfo("Enclosed_by_Leading  : %c\n"_Stz,
        gpAttachMap->m_oFormatters.m_cEnclosedBy_Leading ? gpAttachMap->m_oFormatters.m_cEnclosedBy_Leading : '0');
  vInfo("Enclosed_by_trailing : %c\n"_Stz,
        gpAttachMap->m_oFormatters.m_cEnclosedBy_Trailing ? gpAttachMap->m_oFormatters.m_cEnclosedBy_Trailing : '0');
  
  vInfo("Escape_char          : %c\n"_Stz,
        gpAttachMap->m_oFormatters.m_cEscapeChar ? gpAttachMap->m_oFormatters.m_cEscapeChar : '0');
  vInfo("Delimiter            : %c\n"_Stz,
        gpAttachMap->m_oFormatters.m_cFieldDelimiter ? gpAttachMap->m_oFormatters.m_cFieldDelimiter : '0');
  vInfo("Record_terminator    : %c\n"_Stz,
        gpAttachMap->m_oFormatters.m_cRecordTerminator ? gpAttachMap->m_oFormatters.m_cRecordTerminator : '0');
  vInfo("Record_end           : %s\n"_Stz, aRE[gpAttachMap->m_oFormatters.m_eRecordEnd]);
  
  vInfo("Date component 0     : %s\n"_Stz, aDC[gpAttachMap->m_oFormatters.m_eDateComponent[0]]);
  vInfo("Date component 1     : %s\n"_Stz, aDC[gpAttachMap->m_oFormatters.m_eDateComponent[1]]);
  vInfo("Date component 2     : %s\n"_Stz, aDC[gpAttachMap->m_oFormatters.m_eDateComponent[2]]);
  
  vInfo("Date_separator       : %c\n"_Stz,
        gpAttachMap->m_oFormatters.m_cDateSeparator ? gpAttachMap->m_oFormatters.m_cDateSeparator : '0');

  vInfo("\n****ATTACH MAP END****\n\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* record all the needed arguments                                            */
/*----------------------------------------------------------------------------*/
void AttachMap::vRecordArguments(
                                 bool bDeveloper,
                                 i8   nDummyLoop,
                                 i8   nDummySleep,
                                 i8   nDummyThreads,
                                 i8   nIndexEntries,
                                 i8   iJumboSize,
                                 bool bLog,
                                 i8   nMaxLoadErrors,
                                 i8   nMaxReadyLP,
                                 bool bMetadata,
                                 i8   nNodes,
                                 bool bProgress,
                                 Stz* aRegionName,
                                 i8   iRegionSize,
                                 i8   nSlotsPerNode,
                                 i8   nSubIngesters,
                                 i8   nSubLoaders,
                                 i8   nThreads,
                                 bool bTracepoint,
                                 bool bVerbose
                                 )
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);

  if (m_bVerbose)
    vTrace("[[mainThread]] Recording arguments...\n"_Stz);
  
  m_iPid = Process::iGetPid();
  
  m_oBirth.vNow();
  m_oNow.vSetNowUTC();
  
  Arg.m_bDeveloper       = bDeveloper;
  Arg.m_nDummyLoop       = nDummyLoop;
  Arg.m_nDummySleep      = nDummySleep;
  Arg.m_nDummyThreads    = nDummyThreads;
  Arg.m_iJumboSize       = iJumboSize;
  Arg.m_bLog             = bLog;
  Arg.m_nMaxLoadErrors   = nMaxLoadErrors;
  Arg.m_nMaxReadyLP      = nMaxReadyLP;
  Arg.m_nNodes           = nNodes;
  Arg.m_sRegionName      = aRegionName;
  Arg.m_iRegionSize      = iRegionSize;
  Arg.m_nSlotsPerNode    = nSlotsPerNode;
  Arg.m_nSubIngesters    = nSubIngesters;
  Arg.m_nSubInserters    = 0;
  Arg.m_nSubLoaders      = nSubLoaders;
  Arg.m_nThreads         = nThreads;
  
  // these are args
  m_nIndexEntries        = nIndexEntries;
  m_bMetadata            = bMetadata;
  m_bProgress            = bProgress;
  m_bTracepoint          = bTracepoint;
  m_bVerbose             = bVerbose;

  
  
  m_pBufferPool = gpBufferPool;
  
  m_iEpoch      = 0;
  
  m_oFormatters.m_nMaxErrors = Arg.m_nMaxLoadErrors;
  
  ASSERT(m_xHeapMem.Transient.m_pJumbo);
  ASSERT(m_xHeapMem.Transient.m_pMedium);
  ASSERT(m_xHeapMem.Transient.m_pLarge);
  ASSERT(m_xHeapMem.Transient.m_pSmall);
  
  ASSERT(m_xHeapMem.Persistent.m_pJumbo);
  ASSERT(m_xHeapMem.Persistent.m_pMedium);
  ASSERT(m_xHeapMem.Persistent.m_pLarge);
  ASSERT(m_xHeapMem.Persistent.m_pSmall);
  
  
  //ASSERT(m_pGrid);
    
  //ASSERT(m_pWord);


  //ASSERT(m_pStepTiming);
    
  //m_nBuffersPending;
  
  m_iStop       = 0;
  m_iQuit       = 0;
}

/*----------------------------------------------------------------------------*/
/* dump this attachmap                                                        */
/*----------------------------------------------------------------------------*/
void AttachMap::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  AttachMap* pAttachMap = _pAttachMap(pObject);
  
  vTrace("pid %lld utc %s regionSize %lld\n"_Stz,
         pAttachMap->m_iPid,
         pAttachMap->m_oNow.sz(),
         pAttachMap->Arg.m_iRegionSize);
}

/*----------------------------------------------------------------------------*/
/* print arguments                                                            */
/*----------------------------------------------------------------------------*/
void AttachMap::vPrintStartupInfo(void)
{
  _precondition_(_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  vTrace("Args:\n"_Stz);
  
  vTrace("  --name %s\n"_Stz,m_pGrid->pThisNode()->aNodeName()->aChar());
  vTrace("  --region %s\n"_Stz,Arg.m_sRegionName.aChar());
  vTrace("  --size %lld\n"_Stz,Arg.m_iRegionSize);
  vTrace("  --partitionsPerNode %lld\n"_Stz,m_pGrid->m_nSlotsPerNode);

}

/*----------------------------------------------------------------------------*/
/* find api which matches given name                                          */
/*----------------------------------------------------------------------------*/
eAC AttachMap::eFindApi(Stz* aApiNm)
{
  _precondition_(_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  for (i8 i_Api = 0; i_Api < AC_ENUMS; i_Api++)
    if (aApiNm->bSame(aAC[i_Api]))
      return (eAC)i_Api;
  
  return AC_NONE;
}

/*----------------------------------------------------------------------------*/
/* focal outline                                                              */
/*----------------------------------------------------------------------------*/
void AttachMap::vFocalOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eClass()]);
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
