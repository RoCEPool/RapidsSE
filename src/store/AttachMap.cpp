/*============================================================================*/
/* AttachMap.cpp - map of addresses when process attaches                     */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"

#include "eAC.h"

#include "Node.hpp"
#include "Grid.hpp"
#include "AttachMap.hpp"

#include "Session.hpp"
#include "Step.hpp"

#include "ResourceTracker.hpp" //?
#include "TagConstants.h"

extern AttachMap*  gpAttachMap;
extern BufferPool* gpBufferPool;

SubSysFn __AttachMap("AttachMap"_Stz,(fpSELFFN*)AttachMap::vSubSystem,
                     ""_Stz,
                     "BufferPool"_Stz,"HeapMem"_Stz,"Base"_Stz,nullptr);

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
  AttachMap* pAttachMap = (AttachMap*)gpBufferPool->pAllocAttachMapBuffer();
  
  return pPoolCheck<void*>(pAttachMap);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
AttachMap::AttachMap(void)
: Focal(CN_ATTACHMAP,0,C_UNKNOWN)
, m_okDdlLock(LK_DDL,this)
, m_okDicLock(LK_DIC,this)
, m_okTccLock(LK_TCC,this)
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
//  m_xHeapMem.Persistent.m_pTiny  =
//    new Heap(CN_TINYPERSISTENT,   C_SIZE_TINY_OBJECT,   HT_P_TINY);
//  m_xHeapMem.Persistent.m_pSmall  =
//    new Heap(CN_SMALLPERSISTENT,  C_SIZE_SMALL_OBJECT,  HT_P_SMALL);
//  m_xHeapMem.Persistent.m_pMedium =
//    new Heap(CN_MEDIUMPERSISTENT, C_SIZE_MEDIUM_OBJECT, HT_P_MEDIUM);
//  m_xHeapMem.Persistent.m_pLarge  =
//    new Heap(CN_LARGEPERSISTENT,  C_SIZE_LARGE_OBJECT,  HT_P_LARGE);
  
  m_pTxSet = new TxSet;

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
  
  m_pBucketMap = nullptr;
  m_pPtnMap    = nullptr;

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
/* the subsystem function                                                     */
/*----------------------------------------------------------------------------*/
void AttachMap::vSubSystem(SelfFn* aSelfFn, TokenVec* pTokenVec)
{
  _static_
  
  vTrace("AttachMap::vSubSystem\n"_Stz);
  
  if (AM::bAttached())
    AM::vPrintAttachMap();
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
  
  vInfo("RegionName   : %s \n"_Stz,     gpAttachMap->m_xSysArgs.m_sRegionName.szStz());
  vInfo("RegionSize   : %lld \n"_Stz,   gpAttachMap->m_xSysArgs.m_iRegionSize);
  vInfo("Arguments     : %lld \n"_Stz,   gpAttachMap->m_xSysArgs.m_nOperands);

  vInfo("\n"_Stz);
  vInfo("Epoch        : %lld \n"_Stz, gpAttachMap->m_iEpoch);
  vInfo("BootError    : %lld \n"_Stz, gpAttachMap->m_bBootError);
  vInfo("IndexEntries : %lld \n"_Stz, gpAttachMap->m_xSysArgs.m_nIndexEntries);
  vInfo("LvlLevels    : %lld \n"_Stz, gpAttachMap->m_xSysArgs.m_nLvlLevels);
  
  vInfo("Verbose      : %s \n"_Stz, AM::bVerbose() ? "true"_Stz : "false"_Stz);
  vInfo("Tracepoint   : %s \n"_Stz, gpAttachMap->m_xSysArgs.m_bTracepoint ? "true"_Stz : "false"_Stz);
  vInfo("Progress     : %s \n"_Stz, gpAttachMap->m_xSysArgs.m_bProgress ? "true"_Stz : "false"_Stz);
  vInfo("Metadata     : %s \n"_Stz, gpAttachMap->m_xSysArgs.m_bMetadata ? "true"_Stz : "false"_Stz);
  
  vInfo("\n"_Stz);
  vInfo("BufferPool         : %p \n"_Stz, gpAttachMap->m_pBufferPool);
  vInfo("TransientLargeHeap : %p \n"_Stz, gpAttachMap->m_xHeapMem.Transient.m_pLarge);
  vInfo("TransientMediumHeap: %p \n"_Stz, gpAttachMap->m_xHeapMem.Transient.m_pMedium);
  vInfo("TransientSmallHeap : %p \n"_Stz, gpAttachMap->m_xHeapMem.Transient.m_pSmall);
  vInfo("TransientTinyHeap  : %p \n"_Stz, gpAttachMap->m_xHeapMem.Transient.m_pTiny);

  vInfo("\n"_Stz);
  vInfo("Enclosed_by_leading  : %c\n"_Stz,
        gpAttachMap->m_oFormat.m_cEnclosedBy_Leading ? gpAttachMap->m_oFormat.m_cEnclosedBy_Leading : '0');
  vInfo("Enclosed_by_trailing : %c\n"_Stz,
        gpAttachMap->m_oFormat.m_cEnclosedBy_Trailing ? gpAttachMap->m_oFormat.m_cEnclosedBy_Trailing : '0');
  
  vInfo("Escape_char          : %c\n"_Stz,
        gpAttachMap->m_oFormat.m_cEscapeChar ? gpAttachMap->m_oFormat.m_cEscapeChar : '0');
  vInfo("Delimiter            : %c\n"_Stz,
        gpAttachMap->m_oFormat.m_cFieldDelimiter ? gpAttachMap->m_oFormat.m_cFieldDelimiter : '0');
  vInfo("Record_terminator    : %c\n"_Stz,
        gpAttachMap->m_oFormat.m_cRecordTerminator ? gpAttachMap->m_oFormat.m_cRecordTerminator : '0');
  vInfo("Record_end           : %s\n"_Stz, aLE[gpAttachMap->m_oFormat.m_eLineEnd]);
  
  vInfo("Date_component 0     : %s\n"_Stz, aDC[gpAttachMap->m_oFormat.m_eDateComponent[0]]);
  vInfo("Date_component 1     : %s\n"_Stz, aDC[gpAttachMap->m_oFormat.m_eDateComponent[1]]);
  vInfo("Date_component 2     : %s\n"_Stz, aDC[gpAttachMap->m_oFormat.m_eDateComponent[2]]);
  
  vInfo("Date_separator       : %c\n"_Stz,
        gpAttachMap->m_oFormat.m_cDateSeparator ? gpAttachMap->m_oFormat.m_cDateSeparator : '0');
  
  vInfo("\n"_Stz);
  gpAttachMap->m_xSysArgs.m_oArgTagSet.vDumpSet();

  vInfo("\n-- ATTACH MAP END --\n\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* record all the needed arguments                                            */
/*----------------------------------------------------------------------------*/
void AttachMap::vRecordArguments(CmdLineArgs* pArgs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ATTACHMAP,_FOOTER_);

  if (AM::bVerboseProgress())
    vTrace("[[mainThread]] Recording arguments...\n"_Stz);
  
  m_xSysArgs.m_nBootNodes       = pArgs->m_xSysArgs.m_nBootNodes;
  m_xSysArgs.m_iConnectCode     = pArgs->m_xSysArgs.m_iConnectCode;
  m_xSysArgs.m_bDebugIndex      = pArgs->m_xSysArgs.m_bDebugIndex;
  m_xSysArgs.m_bDebugMode       = pArgs->m_xSysArgs.m_bDebugMode;
  m_xSysArgs.m_nDegOfPar        = pArgs->m_xSysArgs.m_nDegOfPar;
  m_xSysArgs.m_bDeveloper       = pArgs->m_xSysArgs.m_bDeveloper;
  m_xSysArgs.m_nDummyLoop       = pArgs->m_xSysArgs.m_nDummyLoop;
  m_xSysArgs.m_nDummySleep      = pArgs->m_xSysArgs.m_nDummySleep;
  m_xSysArgs.m_nDummyThreads    = pArgs->m_xSysArgs.m_nDummyThreads;
  m_xSysArgs.m_nIndexEntries    = pArgs->m_xSysArgs.m_nEntries;
  m_xSysArgs.m_bLog             = pArgs->m_xSysArgs.m_bLog;
  m_xSysArgs.m_nLvlLevels       = pArgs->m_xSysArgs.m_nLvlLevels;
  m_xSysArgs.m_iLvlRatio        = pArgs->m_xSysArgs.m_iLvlRatio;
  m_xSysArgs.m_nMaxLoadErrors   = pArgs->m_xSysArgs.m_nMaxLoadErrors;
  m_xSysArgs.m_nMaxReadyLP      = pArgs->m_xSysArgs.m_nMaxReadyLP;
  m_xSysArgs.m_bMetadata        = pArgs->m_xSysArgs.m_bMetadata;
  m_xSysArgs.m_bMunged          = pArgs->m_xSysArgs.m_bMunged;
  m_xSysArgs.m_sGridName        = pArgs->m_xSysArgs.m_sGridName;
  m_xSysArgs.m_nOperands        = pArgs->m_xSysArgs.m_nOperands;
  m_xSysArgs.m_bProgress        = pArgs->m_xSysArgs.m_bProgress;
  m_xSysArgs.m_sRegionName      = pArgs->m_xSysArgs.m_sRegionName;
  m_xSysArgs.m_iRegionSize      = pArgs->m_xSysArgs.m_iRegionSize;
  m_xSysArgs.m_bRLUDiary        = pArgs->m_xSysArgs.m_bRLUDiary;
  m_xSysArgs.m_bRLUTimer        = pArgs->m_xSysArgs.m_bRLUTimer;
  m_xSysArgs.m_nPtnsPerTable    = pArgs->m_xSysArgs.m_nPtnsPerTable;
  m_xSysArgs.m_nThreads         = pArgs->m_xSysArgs.m_nThreads;
  m_xSysArgs.m_bTracepoint      = pArgs->m_xSysArgs.m_bTracepoint;
  m_xSysArgs.m_bVerbose         = pArgs->m_xSysArgs.m_bVerbose;

  m_xSysArgs.m_oArgTagSet.vInsert(C_TAGSET_NAME,"dbgTagSet"_Stz);
  
  m_xSysArgs.m_oArgTagSet.vCopy(&pArgs->m_xSysArgs.m_oArgTagSet);
  
  
  // ray -- not sure the following should be here
  
  m_iPid = Process::iGetPid();
  
  m_oBirth.vNow();
  m_oNow.vSetNowUTC();
  
  m_pBufferPool = gpBufferPool;
  
  m_iEpoch      = 0;
  
  m_oFormat.m_nMaxErrors = m_xSysArgs.m_nMaxLoadErrors;
  
  ASSERT(m_xHeapMem.Transient.m_pTiny);
  ASSERT(m_xHeapMem.Transient.m_pSmall);
  ASSERT(m_xHeapMem.Transient.m_pMedium);
  ASSERT(m_xHeapMem.Transient.m_pLarge);

  
  ASSERT(m_xHeapMem.Virtual.m_pTiny);
  ASSERT(m_xHeapMem.Virtual.m_pSmall);
  ASSERT(m_xHeapMem.Virtual.m_pMedium);
  ASSERT(m_xHeapMem.Virtual.m_pLarge);

//  ASSERT(m_xHeapMem.Persistent.m_pTiny);
//  ASSERT(m_xHeapMem.Persistent.m_pSmall);
//  ASSERT(m_xHeapMem.Persistent.m_pMedium);
//  ASSERT(m_xHeapMem.Persistent.m_pLarge);
  
    
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
         pAttachMap->m_xSysArgs.m_iRegionSize);
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
  vTrace("  --partitionsPerNode %lld\n"_Stz,AM::nPtnsPerTable());
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
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eGetClassNo()]);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
