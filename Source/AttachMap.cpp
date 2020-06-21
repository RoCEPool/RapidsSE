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
: Focal(CN_ATTACHMAP,0,C_UNKNOWN)
, m_okDdlLock(LK_DDL,this)
, m_okDicLock(LK_DIC,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP);
  
  gpAttachMap = this;
  
  if (AM::bVerboseProgress())
    vInfo("Created attach map %p: sizeof(AttachMap)=%lldB\n"_Stz,
          gpAttachMap,
          sizeof(AttachMap));
  
  m_uAttachMapState = C_CREATED;
  
  // the attach map must always be the first buffer in the buffer pool
  //ASSERT(nPoolBufferNo() == 1_bn);
  
  m_iTimePoint = AM::iRdTimePoint();
  
  // create the carry over (which records global pointers restored on restart)
  m_pCarryOver = new CarryOver;
  
  // create the transient heaps
  m_xHeapMem.vInit();
  
  Lexicon* pLexicon = new Lexicon("__lexicon"_Stz);
  m_pLexiconArticle = pLexicon->pArticle();
  
  m_pRootDic = new Dic("__dicRoot"_Stz);
  
  m_pRootArcticle = m_pRootDic->pArticle();
  
  m_pLexiconArticle->vAppend(m_pRootArcticle);
  
  Step::vAddStep("word"_Stz,&Step::vWord);
  AM::vRootWord(m_pRootDic->pFindWord("word"_Stz));

  Step::vAddStep("rootConstant"_Stz,nullptr);
  AM::vRootConstant(m_pRootDic->pFindWord("rootConstant"_Stz));
  
  // create the persistent heaps
  m_xHeapMem.Persistent.m_pSmall  =
    new Heap(CN_SMALLPERSISTENT,  C_SIZE_SMALL_OBJECT);
  m_xHeapMem.Persistent.m_pMedium =
    new Heap(CN_MEDIUMPERSISTENT, C_SIZE_MEDIUM_OBJECT);
  m_xHeapMem.Persistent.m_pLarge  =
    new Heap(CN_LARGEPERSISTENT,  C_SIZE_LARGE_OBJECT);

  _checkInitialised_(CN_ATTACHMAP);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Lexicon* pSubLex;
  
  pSubLex = new Lexicon("__ddlLock"_Stz);
  m_pLexiconArticle->vAppend(pSubLex->pArticle());

  pSubLex = new Lexicon("__grid"_Stz);
  m_pLexiconArticle->vAppend(pSubLex->pArticle());
  
  pSubLex = new Lexicon("__queues"_Stz);
  m_pLexiconArticle->vAppend(pSubLex->pArticle());
  
  pSubLex = new Lexicon("__tennants"_Stz);
  m_pLexiconArticle->vAppend(pSubLex->pArticle());

  pSubLex = new Lexicon("__threads"_Stz);
  m_pLexiconArticle->vAppend(pSubLex->pArticle());
  
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
 
  Arg.m_nIndexEntries = nIndexEntries;
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
  vInfo("\n-- ATTACH MAP --\n\n"_Stz);
  vInfo("Pid: %lld; \n"_Stz,   gpAttachMap->m_iPid);
  vInfo("UTC: %s; \n"_Stz,     gpAttachMap->m_oNow.sz());
  
  vInfo("RegionName   : %s \n"_Stz,     gpAttachMap->Arg.m_sRegionName.szStz());
  vInfo("RegionSize   : %lld \n"_Stz,   gpAttachMap->Arg.m_iRegionSize);
  vInfo("SubIngesters : %lld \n"_Stz,   gpAttachMap->Arg.m_nSubIngesters);
  vInfo("SubLoaders   : %lld \n"_Stz,   gpAttachMap->Arg.m_nSubLoaders);

  vInfo("\n"_Stz);
  vInfo("Epoch        : %lld \n"_Stz, gpAttachMap->m_iEpoch);
  vInfo("BootError    : %lld \n"_Stz, gpAttachMap->m_bBootError);
  vInfo("IndexEntries : %lld \n"_Stz, gpAttachMap->Arg.m_nIndexEntries);
  vInfo("LvlLevels    : %lld \n"_Stz, gpAttachMap->Arg.m_nLvlLevels);
  
  vInfo("Verbose      : %s \n"_Stz, AM::bVerbose() ? "true"_Stz : "false"_Stz);
  vInfo("Tracepoint   : %s \n"_Stz, gpAttachMap->Arg.m_bTracepoint ? "true"_Stz : "false"_Stz);
  vInfo("Progress     : %s \n"_Stz, gpAttachMap->Arg.m_bProgress ? "true"_Stz : "false"_Stz);
  vInfo("Metadata     : %s \n"_Stz, gpAttachMap->Arg.m_bMetadata ? "true"_Stz : "false"_Stz);
  
  vInfo("\n"_Stz);
  vInfo("BufferPool         : %p \n"_Stz, gpAttachMap->m_pBufferPool);
  vInfo("TransientLargeHeap : %p \n"_Stz, gpAttachMap->m_xHeapMem.Transient.m_pLarge);
  vInfo("TransientMediumHeap: %p \n"_Stz, gpAttachMap->m_xHeapMem.Transient.m_pMedium);
  vInfo("TransientSmallHeap : %p \n"_Stz, gpAttachMap->m_xHeapMem.Transient.m_pSmall);
  vInfo("TransientTinyHeap  : %p \n"_Stz, gpAttachMap->m_xHeapMem.Transient.m_pTiny);

  /*
  vInfo("\n"_Stz);
  vInfo("Total Nodes      : %lld \n"_Stz, AM::pGrid()->nDefinedNodes());
  vInfo("This node        : %lld \n"_Stz, AM::pGrid()->m_iThisNode);
  
  vInfo("Cluster started  : %s \n"_Stz,  AM::pGrid()->m_bHasStarted ? "true"_Stz : "false"_Stz);
  if (AM::pGrid()->m_bHasStarted)
    vInfo("Start node       : %lld \n"_Stz, AM::pGrid()->m_iStartNode);
  
  vInfo("Slots per node   : %lld \n"_Stz, AM::pGrid()->nSlotsPerNode());
  vInfo("Total Partitions : %lld \n"_Stz, AM::pGrid()->nPartitions());
  
  vInfo("Partitions on this node : %lld \n"_Stz, AM::pGrid()->nPartitions());
  for (i8 iIndex = 0; iIndex < AM::pGrid()->nSlotsPerNode(); iIndex++)
    vInfo("Slot#%lld      : partition %lld \n"_Stz, iIndex, AM::pGrid()->m_iPartition[iIndex]);
  */

  vInfo("\n"_Stz);
  vInfo("Enclosed_by_leading  : %c\n"_Stz,
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
  
  vInfo("Date_component 0     : %s\n"_Stz, aDC[gpAttachMap->m_oFormatters.m_eDateComponent[0]]);
  vInfo("Date_component 1     : %s\n"_Stz, aDC[gpAttachMap->m_oFormatters.m_eDateComponent[1]]);
  vInfo("Date_component 2     : %s\n"_Stz, aDC[gpAttachMap->m_oFormatters.m_eDateComponent[2]]);
  
  vInfo("Date_separator       : %c\n"_Stz,
        gpAttachMap->m_oFormatters.m_cDateSeparator ? gpAttachMap->m_oFormatters.m_cDateSeparator : '0');
  
  vInfo("\n"_Stz);
  gpAttachMap->Arg.m_oDbgTagSet.vDumpSet();

  vInfo("\n-- ATTACH MAP END --\n\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* record all the needed arguments                                            */
/*----------------------------------------------------------------------------*/
void AttachMap::vRecordArguments(Args* pArgs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);

  if (AM::bVerboseProgress())
    vTrace("[[mainThread]] Recording arguments...\n"_Stz);
  
  Arg.m_bDebugMode       = pArgs->m_bDebugMode;
  Arg.m_bDeveloper       = pArgs->m_bDeveloper;
  Arg.m_nDummyLoop       = pArgs->m_nDummyLoop;
  Arg.m_nDummySleep      = pArgs->m_nDummySleep;
  Arg.m_nDummyThreads    = pArgs->m_nDummyThreads;
  Arg.m_nIndexEntries    = pArgs->m_nEntries;
  Arg.m_bLog             = pArgs->m_bLog;
  Arg.m_nLvlLevels       = pArgs->m_nLvlLevels;
  Arg.m_iLvlRatio        = pArgs->m_iLvlRatio;
  Arg.m_nMaxLoadErrors   = pArgs->m_nMaxLoadErrors;
  Arg.m_nMaxReadyLP      = pArgs->m_nMaxReadyLP;
  Arg.m_bMetadata        = pArgs->m_bMetadata;
  Arg.m_bMunged          = pArgs->m_bMunged;
  Arg.m_nNodes           = pArgs->m_nNodes;
  Arg.m_bProgress        = pArgs->m_bProgress;
  Arg.m_sRegionName      = pArgs->m_sRegionName;
  Arg.m_iRegionSize      = pArgs->m_iRegionSize;
  Arg.m_nSlotsPerNode    = pArgs->m_nSlotsPerNode;
  Arg.m_nSubIngesters    = pArgs->m_nSubIngesters;
  Arg.m_nSubInserters    = 0;
  Arg.m_nSubLoaders      = pArgs->m_nSubLoaders;
  Arg.m_nThreads         = pArgs->m_nThreads;
  Arg.m_bTracepoint      = pArgs->m_bTracepoint;
  Arg.m_bVerbose         = pArgs->m_bVerbose;

  Arg.m_oDbgTagSet.vInsert(C_TAGSET_NAME,"dbgTagSet"_Stz);
  
  Arg.m_oDbgTagSet.vCopy(&pArgs->m_oArgTagSet);
  
  
  // ray -- not sure the following should be here
  
  m_iPid = Process::iGetPid();
  
  m_oBirth.vNow();
  m_oNow.vSetNowUTC();
  
  m_pBufferPool = gpBufferPool;
  
  m_iEpoch      = 0;
  
  m_oFormatters.m_nMaxErrors = Arg.m_nMaxLoadErrors;
  
  ASSERT(m_xHeapMem.Transient.m_pMedium);
  ASSERT(m_xHeapMem.Transient.m_pLarge);
  ASSERT(m_xHeapMem.Transient.m_pSmall);
  
  ASSERT(m_xHeapMem.Persistent.m_pMedium);
  ASSERT(m_xHeapMem.Persistent.m_pLarge);
  ASSERT(m_xHeapMem.Persistent.m_pSmall);
  
    
  m_iStop       = 0;
  m_iQuit       = 0;
}

/*----------------------------------------------------------------------------*/
/* get session array                                                          */
/*----------------------------------------------------------------------------*/
SessionArray* AttachMap::pSessionArray(void)
{
  return m_pSessionArray;
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
  
  vTrace("\nArgs:\n"_Stz);
  
  vTrace("  --name %s\n"_Stz,m_pGrid->pThisNode()->szNodeNm()->azChar());
  vTrace("  --region %s\n"_Stz,Arg.m_sRegionName.azChar());
  vTrace("  --size %lld\n"_Stz,Arg.m_iRegionSize);
  vTrace("  --partitionsPerNode %lld\n"_Stz,m_pGrid->nSlotsPerNode());

}

/*----------------------------------------------------------------------------*/
/* find api which matches given name                                          */
/*----------------------------------------------------------------------------*/
void AttachMap::vAttachMapState(u8 uState)
{
  _precondition_(_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  m_uAttachMapState = uState;
}

/*----------------------------------------------------------------------------*/
/* find api which matches given name                                          */
/*----------------------------------------------------------------------------*/
u8 AttachMap::uAttachMapState(void)
{
  _precondition_(_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  return m_uAttachMapState;
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
