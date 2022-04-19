/*============================================================================*/
/* AttachMap.cpp - map of addresses when process attaches                     */
/*============================================================================*/

#include "BASE.h"

#include "eAC.h"

#include "CmdLineArgs.hpp"


#include "Node.hpp"
#include "Grid.hpp"
#include "AttachMap.hpp"
#include "Numbering.hpp"
#include "Jobs.hpp"
#include "InternalTables.hpp"
#include "Session.hpp"
#include "Step.hpp"

#include "Resources.hpp" //?
#include "TagConstants.h"
#include "BucketMap.hpp"

extern AttachMap*  gpAttachMap;
extern BufferPool* gpBufferPool;

SubSysFn __AttachMap("AttachMap"_Stz,(fpSELFFN*)AttachMap::vSubSystem,
                     ""_Stz,
                     "BufferPool"_Stz,"Heaps"_Stz,"Base"_Stz,nullptr);

/*----------------------------------------------------------------------------*/
/* -- LifeCycle --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,AttachMap,CN_ATTACHMAP)
__vDeleteFunction(AttachMap,CN_ATTACHMAP)
__aFooterFunction(AttachMap)

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* AttachMap::operator new(size_t)
{
  AttachMap* pAttachMap = (AttachMap*)gpBufferPool->pAllocOtherBuffer();
  
  return pPoolCheck<void*>(pAttachMap,__func__);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
AttachMap::AttachMap(bool bNoFree)
: Object(CN_ATTACHMAP)
, m_xHeaps(bNoFree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP);
  
  gpAttachMap = this;
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vTrace("Created attach map %p: sizeof(AttachMap)=%lldB\n"_Stz,
          gpAttachMap,
          sizeof(AttachMap));
  
  m_uAttachMapState = C_CREATED;
  
  // the attach map must always be the first buffer in the buffer pool
  //ASSERT(nPoolBufferNo() == 1_bn);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 
  // m_oBirth
  // m_oNow
  // m_iPid
  m_oSince.vStart();
  m_bSystemOpen = true;
  m_iTimePoint  = AM::iRdTickingDevice();
      
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // create the heaps
  m_xHeaps.vInit();

  m_pFileMap        = new FileMap();
  m_pBucketMap      = nullptr;
  m_pDicRoot        = new Dic("__dicRoot"_Stz);
#ifdef WITH_HASHMAP
  m_pHashMap        = new HashMap();
#endif
  m_pInternalTables = new InternalTables;
  m_pJobs           = new Jobs;
  m_pNumbering      = new Numbering;
  m_pLglPtnMap         = nullptr;
  m_bReapEnabled    = true;
  m_pStarter        = new Starter;
  m_pTxSet          = new TxSet;

  m_pTraceFile      = nullptr;
  
  Step::vAddStep("\\word"_Stz,&Step::vWord);
  AM::vRootWord(m_pDicRoot->pFindWord("\\word"_Stz));

  Step::vAddStep("rootConstant"_Stz,nullptr);
  AM::vRootConstant(m_pDicRoot->pFindWord("rootConstant"_Stz));
  

  _checkInitialised_(CN_ATTACHMAP);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
AttachMap::~AttachMap(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* -- LifeCycle --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
/* generate next command no                                                   */
/*----------------------------------------------------------------------------*/
i8 AttachMap::iCmdNoNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);

  i8 iCmdNo = ++ m_ixCmdNo;
  
  return iCmdNo;
}

/*----------------------------------------------------------------------------*/
/* dump this attachmap                                                        */
/*----------------------------------------------------------------------------*/
void AttachMap::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  UNUSED(eClass);
  UNUSED(aTab);

  AttachMap* pAttachMap = _pAttachMap(pObject);
  
  vTrace("sepid %lld utc %s regionSize %lld\n"_Stz,
         pAttachMap->m_iSePid,
         pAttachMap->m_oNow.sz(),
         pAttachMap->m_xSysArgs.m_iRegionSize);
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
 
  m_xSysArgs.m_nIndexEntries = nIndexEntries;
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
/* outline                                                                    */
/*----------------------------------------------------------------------------*/
void AttachMap::vOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eGetClassNo()]);
}

/*----------------------------------------------------------------------------*/
/* get pid                                                                    */
/*----------------------------------------------------------------------------*/
i8 AttachMap::iSePid(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  return m_iSePid;
}


/*----------------------------------------------------------------------------*/
/* print atachmap                                                             */
/*----------------------------------------------------------------------------*/
void AttachMap::vPrintAttachMap(void)
{
  vTrace("\n-- ATTACH MAP --\n\n"_Stz);
  vTrace("SePid: %lld; \n"_Stz,   gpAttachMap->m_iSePid);
  vTrace("UTC: %s; \n"_Stz,     gpAttachMap->m_oNow.sz());
  
  vTrace("RegionName   : %s \n"_Stz,     gpAttachMap->m_xSysArgs.m_sRegionName.szStz());
  vTrace("RegionSize   : %lld \n"_Stz,   gpAttachMap->m_xSysArgs.m_iRegionSize);
  vTrace("Arguments     : %lld \n"_Stz,   gpAttachMap->m_xSysArgs.m_nOperands);

  vTrace("\n"_Stz);
  vTrace("Epoch        : %lld \n"_Stz, gpAttachMap->m_iEpoch);
  vTrace("BootError    : %lld \n"_Stz, gpAttachMap->m_bBootError);
  vTrace("IndexEntries : %lld \n"_Stz, gpAttachMap->m_xSysArgs.m_nIndexEntries);
  vTrace("LvlLevels    : %lld \n"_Stz, gpAttachMap->m_xSysArgs.m_nLvlLevels);
  
  vTrace("Tracepoint   : %s \n"_Stz, gpAttachMap->m_xSysArgs.m_bTracepoint ? "true"_Stz : "false"_Stz);
  vTrace("Progress     : %s \n"_Stz, gpAttachMap->m_xSysArgs.m_bProgress ? "true"_Stz : "false"_Stz);
  vTrace("Metadata     : %s \n"_Stz, gpAttachMap->m_xSysArgs.m_bMetadata ? "true"_Stz : "false"_Stz);
  
  vTrace("\n"_Stz);
  vTrace("BufferPool         : %p \n"_Stz, gpAttachMap->m_pBufferPool);
  vTrace("TransientDataHeap  : %p \n"_Stz, gpAttachMap->m_xHeaps.Transient.m_pData);
  vTrace("TransientLargeHeap : %p \n"_Stz, gpAttachMap->m_xHeaps.Transient.m_pLarge);
  vTrace("TransientMediumHeap: %p \n"_Stz, gpAttachMap->m_xHeaps.Transient.m_pMedium);
  vTrace("TransientSmallHeap : %p \n"_Stz, gpAttachMap->m_xHeaps.Transient.m_pSmall);
  vTrace("TransientTinyHeap  : %p \n"_Stz, gpAttachMap->m_xHeaps.Transient.m_pTiny);

  vTrace("\n"_Stz);
  vTrace("Enclosed_by_leading  : %c\n"_Stz,
        gpAttachMap->m_oFormat.m_cEnclosedBy_Leading ? gpAttachMap->m_oFormat.m_cEnclosedBy_Leading : '0');
  vTrace("Enclosed_by_trailing : %c\n"_Stz,
        gpAttachMap->m_oFormat.m_cEnclosedBy_Trailing ? gpAttachMap->m_oFormat.m_cEnclosedBy_Trailing : '0');
  
  vTrace("Escape_char          : %c\n"_Stz,
        gpAttachMap->m_oFormat.m_cEscapeChar ? gpAttachMap->m_oFormat.m_cEscapeChar : '0');
  vTrace("Delimiter            : %c\n"_Stz,
        gpAttachMap->m_oFormat.m_cFieldDelimiter ? gpAttachMap->m_oFormat.m_cFieldDelimiter : '0');
  vTrace("Record_terminator    : %c\n"_Stz,
        gpAttachMap->m_oFormat.m_cRecordTerminator ? gpAttachMap->m_oFormat.m_cRecordTerminator : '0');
  vTrace("Record_end           : %s\n"_Stz, aLE[gpAttachMap->m_oFormat.m_eLineEnd]);
  
  vTrace("Date_component 0     : %s\n"_Stz, aDC[gpAttachMap->m_oFormat.m_eDateComponent[0]]);
  vTrace("Date_component 1     : %s\n"_Stz, aDC[gpAttachMap->m_oFormat.m_eDateComponent[1]]);
  vTrace("Date_component 2     : %s\n"_Stz, aDC[gpAttachMap->m_oFormat.m_eDateComponent[2]]);
  
  vTrace("Date_separator       : %c\n"_Stz,
        gpAttachMap->m_oFormat.m_cDateSeparator ? gpAttachMap->m_oFormat.m_cDateSeparator : '0');
  
  vTrace("\n"_Stz);
  gpAttachMap->m_xSysArgs.m_oArgTags.vDumpSet();

  vTrace("\n-- ATTACH MAP END --\n\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* print arguments                                                            */
/*----------------------------------------------------------------------------*/
void AttachMap::vPrintStartupInfo(void)
{
  _precondition_(_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  vTrace("\nArgs:\n"_Stz);
  
  vTrace("  --name %s\n"_Stz,m_pGrid->pThisNode()->szNodeNm()->azChar());
  vTrace("  --region %s\n"_Stz,m_xSysArgs.m_sRegionName.azChar());
  vTrace("  --size %lld\n"_Stz,m_xSysArgs.m_iRegionSize);
  vTrace("  --partitionsPerNode %lld\n"_Stz,AM::PTNMAP::nPtnsPerNode());
}

/*----------------------------------------------------------------------------*/
/* get reap enabled?                                                          */
/*----------------------------------------------------------------------------*/
bool AttachMap::bReapEnabled(void)
{
  _precondition_(_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  return m_bReapEnabled;
}

/*----------------------------------------------------------------------------*/
/* set reap enabled?                                                          */
/*----------------------------------------------------------------------------*/
void AttachMap::vReapEnabled(bool bReapEnabled)
{
  _precondition_(_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  m_bReapEnabled = bReapEnabled;
}

/*----------------------------------------------------------------------------*/
/* drop reap lock                                                             */
/*----------------------------------------------------------------------------*/
void AttachMap::vReapLockDrop(void)
{
  _precondition_(_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  m_oReapLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab reap lock                                                             */
/*----------------------------------------------------------------------------*/
void AttachMap::vReapLockGrab(void)
{
  _precondition_(_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  m_oReapLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* record all the needed arguments                                            */
/*----------------------------------------------------------------------------*/
void AttachMap::vRecordArguments(CmdLineArgs* pCmdLineArgs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);

  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vTrace("[[mainThread]] Recording arguments...\n"_Stz);
  
  m_xSysArgs.m_nDefinedNodes    = pCmdLineArgs->m_xSysArgs.m_nDefinedNodes;
  m_xSysArgs.m_iConnectCode     = pCmdLineArgs->m_xSysArgs.m_iConnectCode;
  m_xSysArgs.m_bDebugIndex      = pCmdLineArgs->m_xSysArgs.m_bDebugIndex;
  m_xSysArgs.m_bDebugMode       = pCmdLineArgs->m_xSysArgs.m_bDebugMode;
  m_xSysArgs.m_bDerefMetrics    = pCmdLineArgs->m_xSysArgs.m_bDerefMetrics;
  m_xSysArgs.m_bDeveloper       = pCmdLineArgs->m_xSysArgs.m_bDeveloper;
  m_xSysArgs.m_nDummyLoop       = pCmdLineArgs->m_xSysArgs.m_nDummyLoop;
  m_xSysArgs.m_nDummySleep      = pCmdLineArgs->m_xSysArgs.m_nDummySleep;
  m_xSysArgs.m_nDummyThreads    = pCmdLineArgs->m_xSysArgs.m_nDummyThreads;
  m_xSysArgs.m_sGridName        = pCmdLineArgs->m_xSysArgs.m_sGridName;
  m_xSysArgs.m_nIndexEntries    = pCmdLineArgs->m_xSysArgs.m_nEntries;
  m_xSysArgs.m_bJobMetrics      = pCmdLineArgs->m_xSysArgs.m_bJobMetrics;
  m_xSysArgs.m_bLockTracking    = pCmdLineArgs->m_xSysArgs.m_bLockTracking;
  m_xSysArgs.m_nLvlLevels       = pCmdLineArgs->m_xSysArgs.m_nLvlLevels;
  m_xSysArgs.m_iLvlRatio        = pCmdLineArgs->m_xSysArgs.m_iLvlRatio;
  m_xSysArgs.m_nMaxLoadErrors   = pCmdLineArgs->m_xSysArgs.m_nMaxLoadErrors;
  m_xSysArgs.m_nMaxReadyLP      = pCmdLineArgs->m_xSysArgs.m_nMaxReadyLP;
  m_xSysArgs.m_bMetadata        = pCmdLineArgs->m_xSysArgs.m_bMetadata;
  m_xSysArgs.m_nMetricsSleep    = pCmdLineArgs->m_xSysArgs.m_nMetricsSleep;
  m_xSysArgs.m_bMunged          = pCmdLineArgs->m_xSysArgs.m_bMunged;
  m_xSysArgs.m_nOperands        = pCmdLineArgs->m_xSysArgs.m_nOperands;
  m_xSysArgs.m_nOperandYield    = pCmdLineArgs->m_xSysArgs.m_nOperandYield;
  m_xSysArgs.m_iOptimiserLvl    = pCmdLineArgs->m_xSysArgs.m_iOptimiserLvl;
  m_xSysArgs.m_bProgress        = pCmdLineArgs->m_xSysArgs.m_bProgress;
  m_xSysArgs.m_nPtnsPerNode     = pCmdLineArgs->m_xSysArgs.m_nPtnsPerNode;
  m_xSysArgs.m_sRegionName      = pCmdLineArgs->m_xSysArgs.m_sRegionName;
  m_xSysArgs.m_iRegionSize      = pCmdLineArgs->m_xSysArgs.m_iRegionSize;
  m_xSysArgs.m_bRLUDiary        = pCmdLineArgs->m_xSysArgs.m_bRLUDiary;
  m_xSysArgs.m_bRLUTimer        = pCmdLineArgs->m_xSysArgs.m_bRLUTimer;
  m_xSysArgs.m_bShmLock         = pCmdLineArgs->m_xSysArgs.m_bShmLock;
  m_xSysArgs.m_bSysMetrics      = pCmdLineArgs->m_xSysArgs.m_bSysMetrics;
  m_xSysArgs.m_nThreads         = pCmdLineArgs->m_xSysArgs.m_nThreads;
  m_xSysArgs.m_bTracepoint      = pCmdLineArgs->m_xSysArgs.m_bTracepoint;
  m_xSysArgs.m_bVerbose         = pCmdLineArgs->m_xSysArgs.m_bVerbose;

  m_xSysArgs.m_oArgTags.vInsert(C_TAGSET_NAME,"dbgTagSet"_Stz);
  
  m_xSysArgs.m_oArgTags.vCopy(&pCmdLineArgs->m_xSysArgs.m_oArgTags);
  
  
  // ray -- not sure the following should be here
  
  m_iSePid = Process::iGetPid();
  
  m_oBirth.vNow();
  m_oNow.vSetNowUTC();
  
  m_pBufferPool = gpBufferPool;
  
  m_iEpoch      = 0;
  
  m_oFormat.m_nMaxErrors = m_xSysArgs.m_nMaxLoadErrors;
  
  ASSERT(m_xHeaps.Transient.m_pTiny);
  ASSERT(m_xHeaps.Transient.m_pSmall);
  ASSERT(m_xHeaps.Transient.m_pMedium);
  ASSERT(m_xHeaps.Transient.m_pLarge);
  ASSERT(m_xHeaps.Transient.m_pData);

  
  ASSERT(m_xHeaps.Virtual.m_pTiny);
  ASSERT(m_xHeaps.Virtual.m_pSmall);
  ASSERT(m_xHeaps.Virtual.m_pMedium);
  ASSERT(m_xHeaps.Virtual.m_pLarge);

//  ASSERT(m_xHeaps.Persistent.m_pTiny);
//  ASSERT(m_xHeaps.Persistent.m_pSmall);
//  ASSERT(m_xHeaps.Persistent.m_pMedium);
//  ASSERT(m_xHeaps.Persistent.m_pLarge);
  
    
  m_iStop       = 0;
  m_iQuit       = 0;
}

/*----------------------------------------------------------------------------*/
/* get session array                                                          */
/*----------------------------------------------------------------------------*/
Sessions* AttachMap::pSessions(void)
{
  return m_pSessions;
}

/*----------------------------------------------------------------------------*/
/* the subsystem function                                                     */
/*----------------------------------------------------------------------------*/
void AttachMap::vSubSystem(TokenVec* pTokenVec)
{
  _static_
  UNUSED(pTokenVec);

  vTrace("AttachMap::vSubSystem\n"_Stz);
  
  if (AM::bAttached())
    AM::vPrintAttachMap();
}

/*----------------------------------------------------------------------------*/
/* add sys metrics to chain                                                   */
/*----------------------------------------------------------------------------*/
void AttachMap::vSysMetricsAdd(SysMetrics* pSysMetrics)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);

  pSysMetrics->vNext(m_pSysMetricsChain);
  
  m_pSysMetricsChain = pSysMetrics;
}

/*----------------------------------------------------------------------------*/
/* get sys metrics chain                                                      */
/*----------------------------------------------------------------------------*/
SysMetrics* AttachMap::pSysMetricsChain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);

  return m_pSysMetricsChain;
}



/*----------------------------------------------------------------------------*/
/* get trace file                                                             */
/*----------------------------------------------------------------------------*/
File* AttachMap::pTraceFile(void)
{
  _precondition_(_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  return m_pTraceFile;
}

/*----------------------------------------------------------------------------*/
/* set trace file                                                             */
/*----------------------------------------------------------------------------*/
void AttachMap::vTraceFile(File* pTraceFile)
{
  _precondition_(_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  m_pTraceFile = pTraceFile;
}

/*----------------------------------------------------------------------------*/
/* drop trace lock                                                            */
/*----------------------------------------------------------------------------*/
void AttachMap::vTraceLockDrop(void)
{
  _precondition_(_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  m_oTraceLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab trace lock                                                            */
/*----------------------------------------------------------------------------*/
void AttachMap::vTraceLockGrab(void)
{
  _precondition_(_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  m_oTraceLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* drop traceMulti lock                                                       */
/*----------------------------------------------------------------------------*/
void AttachMap::vTraceMultiLockDrop(void)
{
  _precondition_(_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  m_oTraceMultiLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab traceMulti lock                                                       */
/*----------------------------------------------------------------------------*/
void AttachMap::vTraceMultiLockGrab(void)
{
  _precondition_(_MAGIC_,CN_ATTACHMAP,_FOOTER_);
  
  m_oTraceMultiLock.vGrabWrite();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
