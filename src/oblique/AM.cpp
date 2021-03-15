/*============================================================================*/
/* AM.cpp -- AttachMap helper                                                 */
/*============================================================================*/

#include "BASE.h"


#include "eAK.h"
#include "eBO.h"
#include "eCC.h"
#include "eCK.h"
#include "eCP.h"
#include "eCX.h"
#include "eDL.h"
#include "eDS.h"
#include "eED.h"
#include "eFN.h"
#include "eIK.h"
#include "eJF.h"
#include "eLG.h"
#include "eMM.h"
#include "eMT.h"
#include "eNE.h"
#include "eOC.h"
#include "eOK.h"
#include "eOP.h"
#include "eOR.h"
#include "ePF.h"
#include "ePP.h"
#include "ePO.h"
#include "ePY.h"
#include "ePX.h"
#include "eRG.h"
#include "eRO.h"
#include "eRU.h"
#include "eST.h"
#include "eTP.h"
#include "eTT.h"
#include "eUK.h"

#include "CmdLineArgs.hpp"

#include "IndexDef.hpp"
#include "Marker.hpp"

#include "RowBuffer.hpp"

#include "Pot.hpp"
#include "InfoSet.hpp"
#include "StreamSet.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"

#include "Crw.hpp"

#include "PtnMap.hpp"

#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"

#include "CrvBuffer.hpp"
#include "ReadApi.hpp"
#include "TablePtn.hpp"
#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "Table.hpp"

#include "Node.hpp"
#include "Node_I.hpp"
#include "Grid.hpp"

#include "Arg.hpp"
#include "RelOp.hpp"
#include "Predicate.hpp"
#include "PredicateVec.hpp"
#include "AccessPath.hpp"

#include "Api.hpp"
#include "AttachMap.hpp"
#include "Constants.hpp"
#include "Engine.hpp"
#include "PipelineSpec.hpp"
#include "Job.hpp"
#include "JobList.hpp"
#include "Jobs.hpp"
#include "Session.hpp"
#include "Sessions.hpp"
#include "XmitOperand.hpp"
#include "Objects.hpp"
#include "Output.hpp"

#include "TagConstants.h"


#include "LogEntry.hpp"
#include "Log.hpp"
#include "LogicUnit.hpp"

#include "Pipeline.hpp"

#include "Op.hpp"


#include "EnumGroups.h"

#include "Step.hpp"
#include "StreamDef.hpp"

#include "SP_Word.hpp"

#include "TraceLock.hpp"
#include "WriteBehind.hpp"

extern CmdLineArgs   gxCmdLineArgs;

extern AttachMap*    gpAttachMap;
extern BufferPool*   gpBufferPool;
extern File*         gpTraceFile;
extern Header*       gaSeShmRegion;
extern Index*        gpReadApiIndex;
extern Log*          gpLog;

extern SizeHeap      gxaSizeHeap[];

static Stz*  RAPIDSSE_VERSION_STRING = "0.6.0 alpha"_Stz;
static i8    RAPIDSSE_VERSION_NUMBER = 0x000600;

static Stz*  RAPIDSSE_FORMAT_STRING  = "0.6.0"_Stz;
static i8    RAPIDSSE_FORMAT_NUMBER  = 0x000600;


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                   Demon                                    */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* pipe given row buffer to named demon                                       */
/*----------------------------------------------------------------------------*/
void AM::DEMON::vPipeTo(Stz* szDemonNm, RowBuffer* pRowBuffer)
{

  // for reference: sen code from : OP_toStatDemon_Profile \handler
  
//  PipeIn[]
//  "__statDemon" find dup if
//    word.object@
//    pipeline.pipeout
//    nullptr to PipeIn[]
//  else
//    discard discard
//    PipeIn[].delete
//  then

  Dic* pRootDic = AM::pDicRoot();
  Word* pDemonWord = pRootDic->pFindWord(szDemonNm);
  Job* pPipelineJob = _pJob(pDemonWord->pObject());
  Pipeline* pPipeline = pPipelineJob->m_pPipeline;
  Op* pOp0 = pPipeline->pOp0();

  Task* pThisTask = AM::pThisTLSTask();
  
  pOp0->vPipeOut_thisOp(pRowBuffer,pThisTask);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                    Task                                    */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* get task dic stack                                                         */
/*----------------------------------------------------------------------------*/
DicStack* AM::TASK::pDicStack(void)
{
  Task* pTask = pThisTLSTask();
  if (! pTask)
    return nullptr;
  
  DicStack* pDicStack = pTask->pDicStack();
  
  return pDicStack;
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                   Thread                                   */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* delete thread                                                              */
/*----------------------------------------------------------------------------*/
void AM::THREAD::vDeleteThread(Thread* pThread)
{
  _static_
  
  WriteBehind* pWriteBehind = AM::THREAD::pWriteBehind(pThread);
  pThread->vObject0(nullptr);

  delete pWriteBehind;
  
  delete pThread;
}

/*----------------------------------------------------------------------------*/
/* new thread                                                                 */
/*----------------------------------------------------------------------------*/
Thread* AM::THREAD::pNewThread(void)
{
  _static_
  
  Thread* pThread = new Thread;

  //WriteBehind* pWriteBehind = new WriteBehind;
  
  //pThread->vObject0(pWriteBehind);
  
  return pThread;
}

/*----------------------------------------------------------------------------*/
/* get write behind of current thread                                         */
/*----------------------------------------------------------------------------*/
WriteBehind* AM::THREAD::pWriteBehind(void)
{
  _static_
  
  Thread* pThread = AM::pThisTLSThread();
  
  if (! pThread)
    return nullptr;
  
  WriteBehind* pWriteBehind = AM::THREAD::pWriteBehind(pThread);
  
  return pWriteBehind;
}

/*----------------------------------------------------------------------------*/
/* get write behind of given thread (lazy creation of write behind)           */
/*----------------------------------------------------------------------------*/
WriteBehind* AM::THREAD::pWriteBehind(Thread* pThread)
{
  _static_
  
  WriteBehind* pWriteBehind = _pWriteBehind(pThread->pObject0());
  
  if (! pWriteBehind) {
    pWriteBehind = new WriteBehind;
    pThread->vObject0(pWriteBehind);
  }
  
  return pWriteBehind;
}




/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*----------------------------------------------------------------------------*/
/* has the system been fully started?                                         */
/*----------------------------------------------------------------------------*/
bool AM::bHasStarted(void)
{
  if (! gpAttachMap || ! gpAttachMap->m_pGrid)
    return false;
  
  return gpAttachMap->m_pGrid->bHasStarted();
}



/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

Stz*  AM::szVersionString(void) { return RAPIDSSE_VERSION_STRING; }
i8    AM::iVersionNumber(void)  { return RAPIDSSE_VERSION_NUMBER; }

Stz*  AM::szFormatString(void)  { return RAPIDSSE_FORMAT_STRING;  }
i8    AM::iFormatNumber(void)   { return RAPIDSSE_FORMAT_NUMBER;  }

// Arg
bool AM::bDebugIndex(void)    { return gpAttachMap->m_xSysArgs.m_bDebugIndex;        }
bool AM::bDebugMode(void)     { return gpAttachMap->m_xSysArgs.m_bDebugMode;         }
i8   AM::nDegOfPar(void)      { return gpAttachMap->m_xSysArgs.m_nDegOfPar;          }
bool AM::bDeveloper(void)     { return gpAttachMap->m_xSysArgs.m_bDeveloper;         }
i8   AM::nDummyLoop(void)     { return gpAttachMap->m_xSysArgs.m_nDummyLoop;         }
i8   AM::nDummySleep(void)    { return gpAttachMap->m_xSysArgs.m_nDummySleep;        }
i8   AM::nDummyThreads(void)  { return gpAttachMap->m_xSysArgs.m_nDummyThreads;      }
bool AM::bLog(void)           { return gpAttachMap->m_xSysArgs.m_bLog;               }
i8   AM::nMaxLoadErrors(void) { return gpAttachMap->m_xSysArgs.m_nMaxLoadErrors;     }
i8   AM::nMaxReadyLP(void)    { return gpAttachMap->m_xSysArgs.m_nMaxReadyLP;        }
bool AM::bMunged(void)        { return gpAttachMap->m_xSysArgs.m_bMunged;            }
Stz* AM::szRegionName(void)   { return gpAttachMap->m_xSysArgs.m_sRegionName.szStz();}
bool AM::bRLUDiary(void)      { return gpAttachMap->m_xSysArgs.m_bRLUDiary;          }
bool AM::bRLUTimer(void)      { return gpAttachMap->m_xSysArgs.m_bRLUTimer;          }
i8   AM::iRegionSize(void)    { return gpAttachMap->m_xSysArgs.m_iRegionSize;        }
i8   AM::nThreads(void)       { return gpAttachMap->m_xSysArgs.m_nThreads;           }

i8   AM::nPtnsPerTable(void) {
  return gpAttachMap->m_pPtnMap->nPtnsPerTable();
}

TagSet* AM::pDbgTagSet(void)  { return & gpAttachMap->m_xSysArgs.m_oArgTagSet;       }

bool AM::bDbgTag(Stz* szTagNm) {
  
  if (! AM::bDebugMode())
    return false;
  
  bool bHasDbgTag = gpAttachMap->m_xSysArgs.m_oArgTagSet.bHasName(szTagNm);
  
  return bHasDbgTag;
}

Stz* AM::szCommand(void)      { return gxCmdLineArgs.m_xSysArgs.m_sCommand.szStz();             }
Stz* AM::szInitial(void)      { return gxCmdLineArgs.m_xSysArgs.m_sInitial.szStz();             }

// file names
Stz* AM::szLogRegion(void)    { return gpAttachMap->m_sLogRegion;             }
Stz* AM::szLogCsvFile(void)   { return gpAttachMap->m_sLogCsvFile;            }
Stz* AM::szLogTxtFile(void)   { return gpAttachMap->m_sLogTxtFile;            }

//
Sessions* AM::pSessions(void) { return gpAttachMap->m_pSessions;  }


bool AM::bBootError(void)     { return gpAttachMap->m_bBootError;             }
i8   AM::nBuckets(void)       { return C_MAX_BUCKETS;                         }
BufferPool* AM::pBufferPool(void) { return gpAttachMap->m_pBufferPool;        }
ClassFlags* AM::pClassFlags(void)  { return & gpAttachMap->m_oClassFlags;      }
MasterLock* AM::pDdlLock(void)      { return & gpAttachMap->m_oLocks.m_oDdlMasterLock;  }
JobList* AM::pDemonJobs(void) {
  return gpAttachMap->m_pJobs->m_pDemonJobs;                                }
MasterLock* AM::pDicLock(void)      { return & gpAttachMap->m_oLocks.m_oDicMasterLock;            }
i8   AM::iElapsedSsec(void)   { return gpAttachMap->m_oBirth.iElapsedSsec();  }
i8   AM::iEpoch(void)         { return gpAttachMap->m_iEpoch;                 }
Format* AM::pFmt(void)        { return & gpAttachMap->m_oFormat;          }
Grid* AM::pGrid(void)         { return gpAttachMap->m_pGrid;                  }
Heaps* AM::pHeaps(void)   { return & gpAttachMap->m_xHeaps;             }
i8   AM::nIndexEntries(void)  { return gpAttachMap->m_xSysArgs.m_nIndexEntries;      }
JobList* AM::pLocalJobs(void) {
  return gpAttachMap->m_pJobs->m_pLocalJobs;                                }
Locks* AM::pLocks(void) { return & gpAttachMap->m_oLocks;   }
bool AM::bLogging(void)       { return gpLog ? true : false;                  }
i8   AM::iNextEpoch(void)     { return ++ gpAttachMap->m_iEpoch;              }
Stz* AM::szNodeNm(void)       {
  return gpAttachMap->m_pGrid->pThisNode()->szNodeNm();  }
JobList* AM::pOriginatingJobs(void) {
  return gpAttachMap->m_pJobs->m_pOriginatingJobs; }
JobList* AM::pParticipatingJobs(void) {
  return gpAttachMap->m_pJobs->m_pParticipatingJobs; }
i8   AM::iPid(void)           { return gpAttachMap->m_iPid;                   }
i8   AM::nConnectedNodes(void){ return gpAttachMap->m_pGrid->nConnectedNodes(); }
i8   AM::nBootNodes(void)  { return gpAttachMap->m_pGrid->nBootNodes(); }
i8   AM::nNodes(void)  { return gpAttachMap->m_pGrid->nNodes(); }
// newcode
std::list<Node*>::const_iterator AM::itBeginNodes(void) {return  gpAttachMap->m_pGrid->itBeginNodes(); }
std::list<Node*>::const_iterator AM::itEndNodes(void) {return  gpAttachMap->m_pGrid->itEndNodes(); }

Article* AM::pLexiconArticle(void) { return gpAttachMap->m_pLexiconArticle;    }
void AM::vProcessExit(void)   { Process::vExitFailure();                      }
bool AM::bProgress(void)      { return gpAttachMap->m_xSysArgs.m_bProgress;              }
bool AM::bMetadata(void)      { return gpAttachMap->m_xSysArgs.m_bMetadata;          }

i8   AM::iNextTxId(void)      { return gpAttachMap->m_pTxSet->iNextTxId();    }

i8   AM::nPtnsInGrid(void) {
  return gpAttachMap->m_pPtnMap->nPtnsInGrid();
}
Schedule* AM::pSchedule(void)     { return gpAttachMap->m_pSchedule;                }
Resources* AM::pResources(void) {
  return & gpAttachMap->m_oResources; }
Article* AM::pArcticleRoot(void)     { return gpAttachMap->m_pArcticleRoot;    }
Word* AM::pConstantRoot(void) { return gpAttachMap->m_pConstantRoot;          }
Dic* AM::pDicRoot(void)       { return gpAttachMap->m_pDicRoot;               }
Word* AM::pWordRoot(void)     { return gpAttachMap->m_pWordRoot;              }
Statistics* AM::pStatistics(void)   { return & gpAttachMap->m_oStatistics;    }
Steps* AM::pSteps(void)   { return gpAttachMap->m_pSteps;      }

MasterLock* AM::pTccLock(void)      { return & gpAttachMap->m_oLocks.m_oTccMasterLock;            }
Word* AM::pTenants(void)      { return gpAttachMap->m_pTenants;               }
i8   AM::iThisNodeNo(void)    { return gpAttachMap->m_pGrid->iThisNode();     }
void AM::vThreadBegins(void)  { ++ gpAttachMap->m_pSchedule->m_ixThreads; }
i8   AM::nThreadsBegun(void)  { return * gpAttachMap->m_pSchedule->m_ixThreads; }
bool AM::bTracepoint(void)    { return gpAttachMap->m_xSysArgs.m_bTracepoint;        }
TxSet* AM::pTxSet(void)       { return gpAttachMap->m_pTxSet;                 }

void AM::vXmitNode(Op* pOp, i8 i_XmitNde) {
  pOp->vXmitNode(i_XmitNde);
}
bool AM::bVerbose(void)       { return gpAttachMap->m_xSysArgs.m_bVerbose;           }
bool AM::bVerboseProgress(void)  {
  if (gxCmdLineArgs.m_xSysArgs.m_bVerbose || gxCmdLineArgs.m_xSysArgs.m_bProgress)
    return true;
  else
    return false;
}

//
void AM::vAckWaitingTask(Task* pTaskWaiting, Task* pTaskAcking) {
  IGNORE_RETURN(Scheduler::bTaskCross_AckWaiting(pTaskWaiting,pTaskAcking));
}
void AM::vAddConstant(Stz* szName,Stz* szValue) {
  Constants::vAddConstant(szName,szValue);
}
void AM::vAttachMap(Header* aHeader) {
  gpAttachMap = static_cast<AttachMap*>(aHeader);
}

Queue* AM::pOutputQueue(Session* pSession) {
    
  return pSession->m_pOutput->m_pOutputQueue;
}


void AM::vBootError(bool bBootError) {
  gpAttachMap->m_bBootError = bBootError;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
void AM::vDeleteOperand(Operand* pOperand)
{
  
  eCN eClass = pOperand->eGetClassNo();
  
  switch(eClass) {
      
    case CN_CRWBUFFER: {
      CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
      
      // permament crw buffers not to be deleted
      if (pCrwBuffer->bIsPermanent())
        break;
      
      delete pCrwBuffer;
      break;
    }
      
    case CN_CSVBUFFER: {
      CsvBuffer* pCsvBuffer = _pCsvBuffer(pOperand);
      delete pCsvBuffer;
      break;
    }
 
    case CN_ENTBUFFER: {
      // should never be called for permanent ent buffers
      EntBuffer* pEntBuffer = _pEntBuffer(pOperand);
      RASSERT(! pEntBuffer->bIsPermanent());
      delete pEntBuffer;
      break;
    }
      
    case CN_INDEXDEF: {
      IndexDef* pIndexDef = _pIndexDef(pOperand);
      delete pIndexDef;
      break;
    }
      
    case CN_INFOSET: {
      InfoSet* pInfoSet = _pInfoSet(pOperand);
      delete pInfoSet;
      break;
    }
      
    case CN_MARKER: {
      Marker* pMarker = _pMarker(pOperand);
      delete pMarker;
      break;
    }
      
    case CN_PROFILE: {
      Profile* pProfile = _pProfile(pOperand);
      delete pProfile;
      break;
    }
      
    case CN_ROWBUFFER: {
      RowBuffer* pRowBuffer = _pRowBuffer(pOperand);
      delete pRowBuffer;
      break;
    }
      
    case CN_STREAMDEF: {
      StreamDef* pStreamDef = _pStreamDef(pOperand);
      delete pStreamDef;
      break;
    }
      
    case CN_STREAMSET: {
      StreamSet* pStreamSet = _pStreamSet(pOperand);
      delete pStreamSet;
      break;
    }
      
    default: {
      RASSERT(0);
    }
  }
}
void AM::vDropTaskLock(Task* pTask) {
  pTask->vDropTaskLock();
}
void AM::vRootConstant(Word* pConstantRoot) {
  gpAttachMap->m_pConstantRoot = pConstantRoot;
}
void AM::vRootWord(Word* pWordRoot) {
  gpAttachMap->m_pWordRoot = pWordRoot;
}
void AM::vGrabTaskLock(Task* pTask) {
  pTask->vGrabTaskLock();
}
void AM::vIndexEntries(i8 nIndexEntries) {
  gpAttachMap->m_xSysArgs.m_nIndexEntries = nIndexEntries;
}
void AM::vNextTempName(s32& sName) {
  gpAttachMap->m_oTempName.vNextName(sName);
}
void AM::vSetAcksRmg(Task* pTask, i8 nAcks) {
  pTask->vAcksRmg(nAcks);
}
void AM::vSteps(Steps* pSteps) {
  gpAttachMap->m_pSteps = pSteps;
}
void AM::vTenants(Word *pTenants) {
  gpAttachMap->m_pTenants = pTenants;
}
void AM::vTriggerBidingTask(Task* pTaskBiding, Task* pTriggeringTask) {
  Scheduler::vTaskCross_TriggerBiding(pTaskBiding,pTriggeringTask);
}

//
bool AM::bAttached(void) {
  if (gpAttachMap)
    return true;
  else
    return false;
}

void AM::vAppendJobTags(Job* pJob, TagSet* pTagSet)
{
  pJob->pJobTags()->vCopy(pTagSet);
}

eCN AM::eJobClass(Job* pJob)
{
  return pJob->eGetClassNo();
}

Stz* AM::szJobNm(Job* pJob)
{
  return pJob->szJobNm();
}

Stz* AM::szJobOpNm(Job* pJob)
{
  if (pJob->m_pOp)
    return pJob->m_pOp->szOpNm();
  else
    return nullptr;
}

Stz* AM::szJobSeed(Job* pJob)
{
  return pJob->szSeed();
}


Session* AM::pJobSession(Job* pJob)
{
  i8 iSessionId = pJob->m_xReqTuple.m_i_Session;
  
  Session* pSession = nullptr;
  
  if (iSessionId != C_UNKNOWN)
    pSession = AM::pSessions()->pGetSession(iSessionId);

  return pSession;
}


TagSet* AM::pJobTags(Job* pJob)
{
  return pJob->pJobTags();
}

bool AM::bJobWasReceived(Job* pJob)
{
  return pJob->bWasReceived();
}


void AM::vDumpApi(void) {
  pSessions()->vDump();
}
Task* AM::pExecutingTask(Job* pJob) {
  Task* pTask = pJob->pTask();
  return pTask;
}
bool AM::bJobIsForeign(i8 iJobId) {
  bool bForeign = Job::bIsForeign(iJobId);
  return bForeign;
}
void AM::vLogArguments(void) {
  gxCmdLineArgs.vLog();
}

i8 AM::nLvlLevels(void) {
  return gpAttachMap->m_xSysArgs.m_nLvlLevels;
}
i8 AM::iLvlRatio(void) {
  return gpAttachMap->m_xSysArgs.m_iLvlRatio;
}

bool AM::bParseCmd(TokenVec* pTknVec, TagSet* pExtraTagSet)
{
  return CmdHelper::bParseCmd(pTknVec,pExtraTagSet);
}


Job* AM::pPipelineJob(void)
{
  
  Job* pOpJob = AM::pTaskJob();
  
  Job* pPipelineJob = pOpJob->m_pOp->pPipeline()->pPipelineJob();
  return pPipelineJob;
}



void AM::vPoolAnalyze(void)   { return PoolHelper::vPoolAnalyze(); }

eEC AM::ePopulateJob(Job* pJob, TagSet* pTagSet)
{
  eEC eError = pJob->ePopulateJob(pTagSet);

  return eError;
}

void AM::vPrintArguments(void) {
  gxCmdLineArgs.vPrint();
}
void AM::vPrintAttachMap(void) {
  gpAttachMap->vPrintAttachMap();
}
void AM::vPrintStartupInfo(void) {
  gxCmdLineArgs.vPrintStartupInfo();
}

PtnMap* AM::pPtnMap(void) {
  return gpAttachMap->m_pPtnMap;
}



i8 AM::nRndSkipLevel(void) {
  
  i8 nLvlLevels = AM::nLvlLevels();
  i8 iLvlRatio  = AM::iLvlRatio();

  i8 nLevels;
  if (AM::bDeveloper()) {
    Task* pTask = Task::pThisTLSTask();
    nLevels =
      RndLib::iSkipLevel(nLvlLevels,
                         iLvlRatio,
                         pTask->pJob()->pLogicUnit_job()->m_i_NextRnd);
  }
  else {
    nLevels = RndLib::iSkipLevel(nLvlLevels,iLvlRatio);
  }
  RASSERT(nLevels >= 1 && nLevels <= nLvlLevels);
  return nLevels;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


Table* AM::pTableFind(Stz* szTableNm)
{
  Table* pTable = nullptr;

  DicStack* pDicStack = AM::TASK::pDicStack();
  
  if (pDicStack && pDicStack->pTop()) {
    pTable = Table::pFindTable(pDicStack,szTableNm);
  }
  else {
    pDicStack = new DicStack;
    
    if (pDicStack->bTest("rapidsse"_Stz,"public"_Stz,szTableNm))
      pTable = pDicStack->pTable();
    
    delete pDicStack;
  }
  
  return pTable;
}

StreamDef* AM::pStreamDefFind(Stz* szTableNm) {
  
  Table* pTable = AM::pTableFind(szTableNm);
  StreamDef* pStreamDef = pTable->pStreamDef();

  return pStreamDef;
}


i8 AM::iTaskAcksRmg_assert_only_(Task* pTask) { return pTask->iAcksRmg_assert_only_(); }
void AM::vTaskDump(void) {
  if (AM::pThisTLSTask())
    AM::pThisTLSTask()->vDump();
}





void AM::vTaskDump(Task* pTask)
{
  Job* pJob = pTask->pJob();
  
  if (AM::aReqTuple_tls())
    AM::aReqTuple_tls()->vContentDump();
  
  AM::vJobContentDump(pJob);
  
  if (pJob->pLogicUnit_job()) {
    if (pJob->pLogicUnit_job()->m_xpOp.bIsNotNull())
      pJob->pLogicUnit_job()->m_xpOp->vDump();
  
    pJob->pLogicUnit_job()->vContentDump();
  }

}









LogicUnit* AM::pTaskLogicUnit(Task* pTask) {
  Job* pJob = pTask->pJob();
  return pJob->pLogicUnit_job();
}



void AM::vTaskOnExceptionTrace(Task* pTask)
{
  Job* pJob = pTask->pJob();
  
  vTrace("\n\n"_Stz);
  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  vTrace("Task::vCaught(xException) (Node:%s) for job %s\n"_Stz,
         AM::pGrid()->pThisNode()->szNodeNm()->azChar(),
         aCN[AM::eJobClass(pJob)]
         );
  

  // dump the call stack
  vThreadCallStackDump();
  
  // write activity log
  AM::vWriteLog();
  
  // dump api state
  //AM::vDumpApi();

  pTask->vContentDump();
  
  if (AM::aReqTuple_tls()) AM::aReqTuple_tls()->vContentDump();

  AM::vJobContentDump(pJob);
  
  pJob->pLogicUnit_job()->vContentDump();
  
  AM::pGrid()->vDsv();

  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
}



i8 AM::iTaskQueue(Task* pTask) {
  eQK eQueue = pTask->eQueue();
  return eQueue;
}
i8 AM::iTaskState(Task* pTask) {
  eTS eState = pTask->eState();
  return eState;
}
Job* AM::pThisTLSJob(void) {
  Thread* pThread = Thread::pThisTLSThread();
  if (! pThread || C_UNKNOWN == (i8)pThread)
    return nullptr;
  
  Task* pTask = pThread->pTask();
  if (! pTask)
    return nullptr;
  
  Job* pJob = pTask->pJob();
  return pJob;
}

t8 AM::tThisTLSPespective(void)
{
  Tx* pTx = pThisTLSTx();
  if (! pTx)
    return C_UNKNOWN;

  t8 tPerspective = pTx->tPerspective();
  
  return tPerspective;
}

//STORE::Task* AM::pThisTLSTask(void) {
//  Thread* pThread = Thread::pThisTLSThread();
//  if (! pThread)
//    return nullptr;
//  Task* pTask = pThread->pTask();
//  return pTask;
//}
LogicUnit* AM::pThisTLSLogicUnit(void) {
  Task* pTask = Task::pThisTLSTask();
  Job* pJob = pTask->pJob();
  LogicUnit* pLogicUnit = pJob->pLogicUnit_job();

  return pLogicUnit;
}

Task* AM::pThisTLSTask(void) {
  Task* pTask = Task::pThisTLSTask();
  return pTask;
}

Thread* AM::pThisTLSThread(void) {
  Thread* pThread = Thread::pThisTLSThread();
  return pThread;
}

i8 AM::iThisTLSThreadNo(void) {
  Thread* pThread = Thread::pThisTLSThread();
  i8 iThreadNo = pThread->iThreadNo();
  return iThreadNo;
}

Tx* AM::pThisTLSTx(void) {

  Task* pTask = pThisTLSTask();
  if (! pTask)
    return nullptr;
  
  Job* pJob = pTask->pJob();
  LogicUnit* pLogicUnit = pJob->pLogicUnit_job();

  Tx* pTx = *pLogicUnit->m_xpTx;
  return pTx;
}

PsxThd* AM::pThisTLSPsxThd(void) {
  Thread* pThread = Thread::pThisTLSThread();
  PsxThd* pPsxThd =pThread->pPsxThd();
  return pPsxThd;
}

HeapBuffer* AM::pToHeapBuffer(i8 i_Buffer) {
  
  HeapBuffer* pHeapBuffer =
    reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(i_Buffer));
  
  return pHeapBuffer;

}



Object* AM::pToLargeObect(i8 i_Buffer) {
    
  HeapBuffer* pHeapBuffer = pToHeapBuffer(i_Buffer);
  
  Object* pObject = reinterpret_cast<Object*>(pHeapBuffer->aFirstChunk());

  return pObject;

}

void AM::vWelcomeText(s256& sWelcome) {
  Engine::vGetWelcome(sWelcome);
}
void AM::vCopyrightText(s256& sCopyright) {
  Engine::vGetCopyright(sCopyright);
}

Crv* AM::aCrv(DataBuffer* pDataBuffer, i8 iItem) {
  CrvBuffer* pCrvBuffer = _pCrvBuffer(pDataBuffer);
  Crv* aCrv = pCrvBuffer->aGetCrv(iItem);
  return aCrv;
}

void AM::vWriteLog(void) {
  Log::vWriteCsv(false);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get pSession given i_Session                                               */
/*----------------------------------------------------------------------------*/
void AM::vSend(Task* pTask, Node* pSendNode, XmitOperand* pXmitOperand)
{
  pTask->pJob()->pLogicUnit_job()->m_pSendNode = pSendNode;
  
  // grab socket wite lock
  Socket* pSocket = pSendNode->pSendSocket();
  pSocket->vGrabWrite();
  
  SysCall oSysCall;
  i8 nObjectSize = gxaSizeHeap[pXmitOperand->eGetClassNo()].m_nClassSize;
  bool bResult = pSocket->bSendTo(pXmitOperand, nObjectSize, oSysCall);
  
  Operand* pOperand = pXmitOperand->m_pOperand;
  
  nObjectSize = gxaSizeHeap[pOperand->eGetClassNo()].m_nClassSize;
  bResult = pSocket->bSendTo(pOperand, nObjectSize, oSysCall);
  
  // drop the socket write lock
  pSocket->vDropWrite();
  
}

/*----------------------------------------------------------------------------*/
/* get pSession given i_Session                                               */
/*----------------------------------------------------------------------------*/
Session* AM::pSessionGet(ReqTuple* aReqTuple)
{
  if (aReqTuple->m_i_Origin != AM::iThisNodeNo())
    return nullptr;
  
  if (C_UNKNOWN == aReqTuple->m_i_Session)
    return nullptr;
  
  Session* pSession = AM::pSessions()->pGetSession(aReqTuple->m_i_Session);

  return pSession;
}

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
bool AM::bSpawnJob_connector(Job* pJob)
{
  bool bResult = Api::bSpawnJob_connector(pJob);
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
void AM::vSpawnJob_dedicated(Job* pJob)
{
  Api::vSpawnJob_dedicated(pJob);
}

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
void AM::vSpawnJob_op(Job* pJob)
{
  Api::vSpawnJob_op(pJob);
}
/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
bool AM::bSpawnJob_session(Job* pJob)
{
  bool bResult = Api::bSpawnJob_session(pJob);
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
void AM::vSpawnJob_received(Job* pJob)
{
  Api::vSpawnJob_received(pJob);
}

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
void AM::vSpawnJob_service(Job* pJob)
{
  Api::vSpawnJob_service(pJob);
}

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
void AM::vScheduleNextPhase(Job* pParticipantJob, Task* pActiviating2PcTask)
{
  Api::vScheduleNextPhase(pParticipantJob,pActiviating2PcTask);
}

/*----------------------------------------------------------------------------*/
/* quick config summary                                                       */
/*----------------------------------------------------------------------------*/
Stz* AM::szSummary(s256& sSummary)
{
  Now spNow;
  
  sSummary.vSprint("%lldN%lldP%lldT %s %s"_Stz,
                   AM::nBootNodes(),
                   AM::nPtnsPerTable(),
                   AM::nThreads(),
                   AM::szBuild(),
                   spNow.sz()
                   );
  
  return sSummary.szStz();
}

/*----------------------------------------------------------------------------*/
/* quick config summary usefule for dir name                                  */
/*----------------------------------------------------------------------------*/
Stz* AM::szSummaryDir(s256& sSummary, Stz* szNote)
{
  Now spNow;
  
  sSummary.vSprint("%s_%lldN%lldP%lldT"_Stz,
                   szNote,
                   AM::nBootNodes(),
                   AM::nPtnsPerTable(),
                   AM::nThreads()
                   );
  
  return sSummary.szStz();
}

/*----------------------------------------------------------------------------*/
/* write to trace file etc                                                    */
/*----------------------------------------------------------------------------*/
void AM::vWriteTrace(eYA eSysAct, Stz* aFormat, ...)
{
  va_list xVarArgs;
  
  va_start(xVarArgs,aFormat);
  AM::vWriteTrace_vargs(eSysAct,aFormat,xVarArgs);
  va_end(xVarArgs);

}

static TraceLock oTraceLock;

/*----------------------------------------------------------------------------*/
/* write to trace file etc                                                    */
/*----------------------------------------------------------------------------*/
void AM::vWriteTrace_vargs(eYA eSysAct, Stz* aFormat, va_list xArgs)
{
  static char caFormatted[C_1MB] = {'\0'};
  
  bool bUseLock = (gpAttachMap ? true : false);
  bool bLowMem  = false; //gpBufferPool ? gpBufferPool->bLowMemory() : false;
  
  if (bUseLock)
    oTraceLock.vGrabWrite();
  
  // produce formatted line
  vsnprintf(caFormatted,sizeof(caFormatted) - 3,aFormat->azChar(),xArgs);
  
  // print formatted line
  fprintf(stdout,"%s%s",bLowMem ? "LOW-MEMORY! " : "",caFormatted);
  fflush(stdout);
  
  if (gpTraceFile) {
    gpTraceFile->vPrint("%s%s"_Stz,bLowMem ? "LOW-MEMORY! " : "",caFormatted);
    gpTraceFile->vFlush();
  }
  
  StrLib::vStrip(caFormatted,'\n');
  
  if (gpAttachMap)
    AM::vLogSys(SL_INF,gpAttachMap,eSysAct,
                "%s"_Stz, //!format-LogSys
                caFormatted);
  
  if (bUseLock)
    oTraceLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* write to trace file etc                                                    */
/*----------------------------------------------------------------------------*/
void AM::vWriteTrace_vargs(File* pFile, eYA eSysAct, Stz* aFormat, va_list xArgs)
{
  static char caFormatted[C_1MB] = {'\0'};
  
  bool bUseLock = (gpAttachMap ? true : false);
  bool bLowMem  = gpBufferPool ? gpBufferPool->bLowMemory() : false;
  
  if (bUseLock)
    oTraceLock.vGrabWrite();
  
  // produce formatted line
  vsnprintf(caFormatted,sizeof(caFormatted) - 3,aFormat->azChar(),xArgs);
  
  // print formatted line
  fprintf(stdout,"%s%s",bLowMem ? "LOW-MEMORY! " : "",caFormatted);
  fflush(stdout);
  
  if (gpTraceFile) {
    gpTraceFile->vPrint("%s%s"_Stz,bLowMem ? "LOW-MEMORY! " : "",caFormatted);
    gpTraceFile->vFlush();
  }
  
  if (pFile) {
    pFile->vPrint("%s"_Stz,caFormatted);
    pFile->vFlush();
  }
  
  StrLib::vStrip(caFormatted,'\n');
  
  if (gpAttachMap)
    AM::vLogSys(SL_INF,gpAttachMap,eSysAct,
                "%s"_Stz, //!format-LogSys
                caFormatted);
  
  if (bUseLock)
    oTraceLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a info log entry                                                       */
/*----------------------------------------------------------------------------*/
void AM::vLogAll(void)
{
#ifdef WITH_SHARED_LOG
//  if (! AM::bLogging())
//    return;
//
//  gpLog->vMarkAll(true);
#endif
}

/*----------------------------------------------------------------------------*/
/* show how a request was run                                                 */
/*----------------------------------------------------------------------------*/
void AM::vLogHow(i8 , i8 )
{
#ifdef WITH_SHARED_LOG
//  if (! AM::bLogging())
//    return;
//
//  gpLog->vHow(iRequestId,iDetail);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a info log entry                                                       */
/*----------------------------------------------------------------------------*/
void AM::vLogFyi(Stz* , ...)
{
#ifdef WITH_SHARED_LOG
//  if (! AM::bLogging())
//    return;
//
//  va_list xArgs;
//
//  va_start(xArgs,szFormat);
//
//  i8 i_Entry = gpLog->iEntryFyi(CN_NONE,szFormat,xArgs);
//
//  va_end(xArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a ddl log entry                                                        */
/*----------------------------------------------------------------------------*/
void AM::vLogDdl(eSL , Job* , eDA , Stz* , ...)
{
#ifdef WITH_SHARED_LOG
//  if (! AM::bLogging())
//    return;
//
//  if (! gpLog->bIsLoggable(pJob))
//    return;
//
//  va_list xVarArgs;
//
//  va_start(xVarArgs,szFormat);
//
//  i8 i_Entry = gpLog->iEntryDdl(eSeverity,pJob,eDdlAct,
//                                szFormat,xVarArgs);
//
//  va_end(xVarArgs);
//
//  pJob->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a job log entry                                                        */
/*----------------------------------------------------------------------------*/
void AM::vLogJob(eSL , Job* , eJA , Stz* , ...)
{
#ifdef WITH_SHARED_LOG
//  if (! AM::bLogging())
//    return;
//
//  if (! gpLog->bIsLoggable(pJob))
//    return;
//
//  va_list xVarArgs;
//
//  va_start(xVarArgs,szFormat);
//
//  i8 i_Entry = gpLog->iEntryJob(eSeverity,pJob,eJobAct,
//                                szFormat,xVarArgs);
//
//  va_end(xVarArgs);
//
//  pJob->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a lock log entry                                                       */
/*----------------------------------------------------------------------------*/
void AM::vLogLck(eSL , Lock* , eLA , Stz* , ...)
{
#ifdef WITH_SHARED_LOG
//  if (! AM::bLogging())
//    return;
//
//  if (! gpLog->bIsLoggable(pLock))
//    return;
//
//  va_list xVarArgs;
//
//  va_start(xVarArgs,szFormat);
//
//  i8 i_Entry = gpLog->iEntryLck(eSeverity,pLock,eLckAct,
//                                szFormat,xVarArgs);
//
//  va_end(xVarArgs);
//
//  pLock->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a log log entry                                                        */
/*----------------------------------------------------------------------------*/
void AM::vLogLog(eSL , Log* , eGA , Stz* , ...)
{
#ifdef WITH_SHARED_LOG
//  if (! AM::bLogging())
//    return;
//
//  if (! gpLog->bIsLoggable(pLog))
//    return;
//
//  va_list xVarArgs;
//
//  va_start(xVarArgs,szFormat);
//
//  i8 i_Entry = gpLog->iEntryLog(eSeverity,pLog,eLogAct,
//                                szFormat,xVarArgs);
//
//  va_end(xVarArgs);
//
//  pLog->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a management log entry                                                 */
/*----------------------------------------------------------------------------*/
void AM::vLogMgt(eSL , Job* , eMA , Stz* , ...)
{
#ifdef WITH_SHARED_LOG
//  if (! AM::bLogging())
//    return;
//
//  if (! gpLog->bIsLoggable(pJob))
//    return;
//
//  va_list xVarArgs;
//
//  va_start(xVarArgs,szFormat);
//
//  i8 i_Entry = gpLog->iEntryMgt(eSeverity,pJob,eMgtAct,
//                                szFormat,xVarArgs);
//
//  va_end(xVarArgs);
//
//  pJob->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a operator log entry                                                   */
/*----------------------------------------------------------------------------*/
void AM::vLogOpr(eSL , Op* , eOA , Stz* , ...)
{
#ifdef WITH_SHARED_LOG
//  if (! AM::bLogging())
//    return;
//
//  if (! gpLog->bIsLoggable(pOpr))
//    return;
//
//  va_list xVarArgs;
//
//  va_start(xVarArgs,szFormat);
//
//  i8 i_Entry = gpLog->iEntryOpr(eSeverity,pOpr,eOprAct,
//                                szFormat,xVarArgs);
//
//  va_end(xVarArgs);
//
//  pOpr->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a prompt log entry                                                     */
/*----------------------------------------------------------------------------*/
void AM::vLogPrm(eSL , SePrompt* , ePA , Stz* , ...)
{
//#ifdef WITH_SHARED_LOG
//  if (! AM::bLogging())
//    return;
//  
//  if (! gpLog->bIsLoggable(pPrm))
//    return;
//  
//  va_list xVarArgs;
//  
//  va_start(xVarArgs,szFormat);
//  
//  i8 i_Entry = gpLog->iEntryPrm(eSeverity,pPrm,ePrmAct,
//                                szFormat,xVarArgs);
//  
//  va_end(xVarArgs);
//  
//  pPrm->m_i_Log = i_Entry;
//#endif
}

/*----------------------------------------------------------------------------*/
/* add a operator log entry                                                   */
/*----------------------------------------------------------------------------*/
void AM::vLogQue(eSL , Schedule* , eQA , Stz* , ...)
{
#ifdef WITH_SHARED_LOG
//  if (! AM::bLogging())
//    return;
//
//  if (! gpLog->bIsLoggable(pQue))
//    return;
//
//  va_list xVarArgs;
//
//  va_start(xVarArgs,szFormat);
//
//  i8 i_Entry = gpLog->iEntryQue(eSeverity,pQue,eQueAct,
//                                szFormat,xVarArgs);
//
//  va_end(xVarArgs);
//
//  pQue->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a resource log entry                                                   */
/*----------------------------------------------------------------------------*/
void AM::vLogRes(eSL , Resource* , eRA , Stz* , ...)
{
#ifdef WITH_SHARED_LOG
//  if (! AM::bLogging())
//    return;
//
//  if (! gpLog->bIsLoggable(pRes))
//    return;
//
//  va_list xVarArgs;
//
//  va_start(xVarArgs,szFormat);
//
//  i8 i_Entry = gpLog->iEntryRes(eSeverity,pRes,eResAct,
//                                szFormat,xVarArgs);
//
//  va_end(xVarArgs);
//
//  pRes->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a session log entry                                                    */
/*----------------------------------------------------------------------------*/
void AM::vLogSsn(eSL , Session* , eAA , Stz* , ...)
{
#ifdef WITH_SHARED_LOG
//  if (! AM::bLogging())
//    return;
//
//  if (! gpLog->bIsLoggable(pSsn))
//    return;
//
//  va_list xVarArgs;
//
//  va_start(xVarArgs,szFormat);
//
//  i8 i_Entry = gpLog->iEntrySsn(eSeverity,pSsn,eApiAct,
//                                szFormat,xVarArgs);
//
//  va_end(xVarArgs);
//
//  pSsn->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a service log entry                                                    */
/*----------------------------------------------------------------------------*/
void AM::vLogSvc(eSL , Job* , eVA , Stz* , ...)
{
#ifdef WITH_SHARED_LOG
//  if (! AM::bLogging())
//    return;
//
//  if (! gpLog->bIsLoggable(pJob))
//    return;
//
//  va_list xVarArgs;
//
//  va_start(xVarArgs,szFormat);
//
//  i8 i_Entry = gpLog->iEntrySvc(eSeverity,pJob,eSvcAct,
//                                szFormat,xVarArgs);
//
//  va_end(xVarArgs);
//
//  pJob->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a system log entry                                                     */
/*----------------------------------------------------------------------------*/
void AM::vLogSys(eSL , AttachMap* , eYA , Stz* , ...)
{
#ifdef WITH_SHARED_LOG
//  if (! AM::bLogging())
//    return;
//
//  if (! gpLog->bIsLoggable(gpAttachMap))
//    return;
//
//  va_list xVarArgs;
//
//  va_start(xVarArgs,szFormat);
//
//  i8 i_Entry = gpLog->iEntrySys(eSeverity,gpAttachMap,eSysAct,
//                                szFormat,xVarArgs);
//
//  va_end(xVarArgs);
//
//  gpAttachMap->m_i_Log = i_Entry;
//
#endif
}

/*----------------------------------------------------------------------------*/
/* add a thread log entry                                                     */
/*----------------------------------------------------------------------------*/
void AM::vLogThd(eSL , Thread* , eHA , Stz* , ...)
{
#ifdef WITH_SHARED_LOG
//  if (! AM::bLogging())
//    return;
//
//  if (! gpLog->bIsLoggable(pThread))
//    return;
//
//  va_list xVarArgs;
//
//  va_start(xVarArgs,szFormat);
//
//  i8 i_Entry = gpLog->iEntryThd(eSeverity,pThread,eThdAct,
//                                szFormat,xVarArgs);
//
//  va_end(xVarArgs);
//
//  pThread->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a rask log entry                                                       */
/*----------------------------------------------------------------------------*/
void AM::vLogTsk(eSL , Task* , eTA , Stz* , ...)
{
#ifdef WITH_SHARED_LOG
//  if (! AM::bLogging())
//    return;
//
//  if (! gpLog->bIsLoggable(pTask))
//    return;
//
//  va_list xVarArgs;
//
//  va_start(xVarArgs,szFormat);
//
//  i8 i_Entry = gpLog->iEntryTsk(eSeverity,pTask,eTskAct,
//                                szFormat,xVarArgs);
//
//  va_end(xVarArgs);
//
//  pTask->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a transaction log entry                                                */
/*----------------------------------------------------------------------------*/
void AM::vLogTxn(eSL , Tx* , eXA , Stz* , ...)
{
#ifdef WITH_SHARED_LOG
//  if (! AM::bLogging())
//    return;
//  
//  if (! gpLog->bIsLoggable(pTxn))
//    return;
//
//  va_list xVarArgs;
//  
//  va_start(xVarArgs,szFormat);
//  
//  i8 i_Entry = gpLog->iEntryTxn(eSeverity,pTxn,eTxnAct,
//                                szFormat,xVarArgs);
//
//  va_end(xVarArgs);
//  
//  pTxn->m_i_Log = i_Entry;
#endif
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* nap for given number of milliseconds (no known calling thread)             */
/*----------------------------------------------------------------------------*/
void AM::vNapMsec(i8 nMilliSeconds, eCN eCallingClass)
{
  struct timespec xReq;
  struct timespec xRem;
  
  if (nMilliSeconds >= C_1K) {
    xReq.tv_nsec = (nMilliSeconds % C_1K) * C_1M;
    xReq.tv_sec  = DIV(nMilliSeconds,C_1K);
  }
  else {
    xReq.tv_nsec = nMilliSeconds * C_1M;
    xReq.tv_sec  = 0;
  }
  
#ifdef WITH_STATISTICS_COUNTERS
  Statistics* pStats = &gpAttachMap->m_oStatistics;
  pStats->Thread.m_nSleeps++;
  pStats->Thread.m_iaSleeps[eCallingClass]++;
  pStats->Thread.m_iaDuration[eCallingClass] += nMilliSeconds * C_1K;
#endif
  
  AM::vLogFyi("%s naps for %lldms"_Stz, //!format-LogFyi
              aCN[eCallingClass],nMilliSeconds);
  
  i8 rCode = nanosleep(&xReq,&xRem);
  
  if (rCode && errno != EINTR) {
    vWarn("AM::vNap error; errno=%lld\n"_Stz,errno);
  }
  
  AM::vLogFyi("%s woken from nap"_Stz, //!format-LogFyi
              aCN[eCallingClass]);
}

/*----------------------------------------------------------------------------*/
/* nap for given number of microseconds (no known calling thread)             */
/*----------------------------------------------------------------------------*/
void AM::vNapUsec(i8 nMicroSeconds, eCN eCallingClass)
{
  struct timespec xReq;
  struct timespec xRem;
  
  if (nMicroSeconds >= C_1M) {
    xReq.tv_nsec = (nMicroSeconds % C_1M) * C_1K;
    xReq.tv_sec  = DIV(nMicroSeconds,C_1M);
  }
  else {
    xReq.tv_nsec = nMicroSeconds * C_1K;
    xReq.tv_sec  = 0;
  }
  
#ifdef WITH_STATISTICS_COUNTERS
  Statistics* pStats = &gpAttachMap->m_oStatistics;
  pStats->Thread.m_nSleeps++;
  pStats->Thread.m_iaSleeps[eCallingClass]++;
  pStats->Thread.m_iaDuration[eCallingClass] += nMilliSeconds * C_1K;
#endif
  
  AM::vLogFyi("%s naps for %lldus"_Stz, //!format-LogFyi
              aCN[eCallingClass],nMicroSeconds);
  
  i8 rCode = nanosleep(&xReq,&xRem);
  
  if (rCode && errno != EINTR) {
    vWarn("AM::vNap error; errno=%lld\n"_Stz,errno);
  }
  
  AM::vLogFyi("%s woken from nap"_Stz, //!format-LogFyi
              aCN[eCallingClass]);
}

/*----------------------------------------------------------------------------*/
/* get next object number                                                     */
/*----------------------------------------------------------------------------*/
i8 AM::iNextObjectNo(eCN eClassNo)
{
  i8 iObjectNo ;
    
  if (gpAttachMap && gpAttachMap->m_pObjects)
    iObjectNo = gpAttachMap->m_pObjects->iNextNo(eClassNo);
  else
    iObjectNo = 1;
  
  return iObjectNo;
}

/*----------------------------------------------------------------------------*/
/* delete the xmitOperand                                                     */
/*----------------------------------------------------------------------------*/
void AM::vDeleteXmitOperand(XmitOperand* pXmitOperand)
{
  delete pXmitOperand;
}

/*----------------------------------------------------------------------------*/
/* duplicate the operand ( marker / streamDef )                               */
/*----------------------------------------------------------------------------*/
Operand* AM::pDuplicateOperand(Operand* pOperand)
{
  if (pOperand->eGetClassNo() == CN_MARKER) {
    Marker* pMarker = _pMarker(pOperand);
    Marker* pNewMarker = new Marker(pMarker->eOpMarker());
    return pNewMarker;
  }
  else {
    ASSERT(pOperand->eGetClassNo() == CN_STREAMDEF);
    StreamDef* pStreamDef = _pStreamDef(pOperand);
    StreamDef* pNewStreamDef = new StreamDef(pStreamDef);
    return pNewStreamDef;
  }
  
}

/*----------------------------------------------------------------------------*/
/* new an operand job                                                         */
/*----------------------------------------------------------------------------*/
XmitOperand* AM::pNewXmitOperand(Job* pParentJob, Operand* pOperand,
                                 ePX eCrossTopo, i8 iNextStage, i8 iNextOp,
                                 i8 i_Node, i8 tPtnNo)
{
  XmitOperand* pXmitOperand =
    XmitOperand::pNewXmitOperand(pOperand,pParentJob->m_iObjectId,
                                 eCrossTopo,iNextStage,iNextOp,i_Node,tPtnNo);
  
  return pXmitOperand;
}

/*----------------------------------------------------------------------------*/
/* set operand in operand job                                                 */
/*----------------------------------------------------------------------------*/
void AM::vSetOperand(XmitOperand* pXmitOperand, Operand* pOperand)
{
  pXmitOperand->m_pOperand = pOperand;
}

/*----------------------------------------------------------------------------*/
/* new a universal job for op                                                 */
/*----------------------------------------------------------------------------*/
Job* AM::pNewJobOp(Op* pOp, ReqTuple* pReqTuple)
{
  Job* pNewJob;
  
  // get the CmdTag words tagSet
  TagSet* pCmdTagSet = CmdHelper::pCmdTagSet(pOp->szOpNm());
  
  // get the jobKind from the cmdTagSet
  eJK eJobKind = (eJK)pCmdTagSet->iValue(C_TAG_JOBKIND);
  UNUSED(eJobKind);

  pNewJob = new Job(pReqTuple);

  pNewJob->m_pOp = pOp;
  pNewJob->vSeed(SS_SEED_REQ);

  return pNewJob;
}

/*----------------------------------------------------------------------------*/
/* new a universal job for op                                                 */
/*----------------------------------------------------------------------------*/
Stz* AM::pOperandName(Operand* pOperand)
{
  if (pOperand->bIsMarker())
    return aOM[(_pMarker(pOperand))->eOpMarker()];
  
  if (pOperand->bIsInfoSet())
    return aOI[(_pInfoSet(pOperand))->eOpInfoSet()];
  else
    return aCN[pOperand->eGetClassNo()];
}

/*----------------------------------------------------------------------------*/
/* set the number of operands in the system                                   */
/*----------------------------------------------------------------------------*/
i8 AM::nOperandsArg(void)
{
  return gpAttachMap->m_xSysArgs.m_nOperands;
}

/*----------------------------------------------------------------------------*/
/* set the number of operands in the system                                   */
/*----------------------------------------------------------------------------*/
void AM::vOperandsArg(i8 nOperands)
{
  gpAttachMap->m_xSysArgs.m_nOperands = nOperands;
}

/*----------------------------------------------------------------------------*/
/* decrement the number of operands in the system                             */
/*----------------------------------------------------------------------------*/
void AM::vOperandsDec(void)
{
  -- gpAttachMap->m_nxOperands;
}

/*----------------------------------------------------------------------------*/
/* get the number of operands in the system                                   */
/*----------------------------------------------------------------------------*/
i8 AM::nOperandsGet(void)
{
  i8 nOperands = * gpAttachMap->m_nxOperands;
  
  return nOperands;
}

/*----------------------------------------------------------------------------*/
/* increment the number of operands in the system                             */
/*----------------------------------------------------------------------------*/
void AM::vOperandsInc(void)
{
  ++ gpAttachMap->m_nxOperands;
}

/*----------------------------------------------------------------------------*/
/* what kind of operand                                                       */
/*----------------------------------------------------------------------------*/
i8 AM::iOperandZkKind(Operand* pOperand)
{
  eZK eOperand = ZK_NONE;
  
  eCN eClass_opd = pOperand->eGetClassNo();
  
  switch (eClass_opd) {
    
    case CN_CRWBUFFER: eOperand = ZK_CRWBUFFER; break;
    case CN_CSVBUFFER: eOperand = ZK_CSVBUFFER; break;
    case CN_ENTBUFFER: eOperand = ZK_ENTBUFFER; break;
    case CN_INDEXDEF:  eOperand = ZK_INDEXDEF;  break;
      
    case CN_INFOSET:
    {
      InfoSet* pInfoSet = _pInfoSet(pOperand);
      
      eOI eInfoSet = pInfoSet->eOpInfoSet();
      switch(eInfoSet) {
        case OI_CALLPIPELINE:   eOperand = ZK_CALLPIPELINE;   break;
        case OI_COMMANDERRORS:  eOperand = ZK_COMMANDERRORS;  break;
        case OI_COUNTINFO:      eOperand = ZK_COUNTINFO;      break;
        case OI_EXECERRORS:     eOperand = ZK_EXECERRORS;     break;
        case OI_LOADRESULT:     eOperand = ZK_LOADRESULT;     break;
        case OI_LOCKSTATS:      eOperand = ZK_LOCKSTATS;      break;
        case OI_NODEINFO:       eOperand = ZK_NODEINFO;       break;
        case OI_PATHINFO:       eOperand = ZK_PATHINFO;       break;
        case OI_PROGRESSUNITS:  eOperand = ZK_PROGRESSUNITS;  break;
        case OI_RETURNPIPELINE: eOperand = ZK_RETURNPIPELINE; break;
        case OI_SEEKOFFSETS:    eOperand = ZK_SEEKOFFSETS;    break;
        case OI_TOTALUNITS:     eOperand = ZK_TOTALUNITS;     break;
        default:
          RASSERT(0);
      }
      break;
    }
      
    case CN_MARKER:
    {
      Marker* pMarker = _pMarker(pOperand);

      eOM eMarker = pMarker->eOpMarker();
      switch(eMarker) {
        case OM_CANCELMARKER:  eOperand = ZK_CANCELMARKER;  break;
        case OM_DATAPULSE:     eOperand = ZK_DATAPULSE;     break;
        case OM_DESTROYMARKER: eOperand = ZK_DESTROYMARKER; break;
        case OM_EOSMARKER:     eOperand = ZK_EOSMARKER;     break;
        case OM_ERRORMARKER:   eOperand = ZK_ERRORMARKER;   break;
        case OM_ERROREDMARKER: eOperand = ZK_ERROREDMARKER; break;
        case OM_EXITMARKER:    eOperand = ZK_EXITMARKER;    break;
        case OM_EXITEDMARKER:  eOperand = ZK_EXITEDMARKER;  break;
        case OM_PULSEDMARKER:  eOperand = ZK_PULSEDMARKER;  break;
        case OM_STARTMARKER:   eOperand = ZK_STARTMARKER;   break;
        case OM_STARTEDMARKER: eOperand = ZK_STARTEDMARKER; break;
        default:
          RASSERT(0);
      }
      break;
    }

    case CN_NAMEDBUFFER:  eOperand = ZK_NAMEDBUFFER;    break;
    case CN_OTHEROPERAND: eOperand = ZK_OTHEROPERAND;   break;
    case CN_PLANOPERAND:  eOperand = ZK_PLANOPERAND;    break;
    case CN_PROFILE:      eOperand = ZK_PROFILE;        break;
    case CN_ROWBUFFER:    eOperand = ZK_ROWBUFFER;      break;
    case CN_STREAMDEF:    eOperand = ZK_STREAMDEF;      break;
    case CN_STREAMSET:    eOperand = ZK_STREAMSET;      break;
      
    default:
      RASSERT(0);
      break;
  }
  
  return (i8)eOperand;
}

/*----------------------------------------------------------------------------*/
/* passes the low-memory condition?                                           */
/*----------------------------------------------------------------------------*/
bool AM::bPassesLowMem(Job* pJob)
{
  _static_

  if (! gpBufferPool->bLowMemory())
    return true;
  
  pJob->pJobTags()->vContentDump();
  
  if (pJob->bHasTag(C_TAG_LOWMEMORY))
    return true;
  
  return false;
  
  //! ALKA: while cleaning up JobClass all jobs had m_eFlags
  //! as JF_LOWMEM hence returning true
  //return true;
  
//  if (aJobCLass->m_eFlags != JF_LOWMEM)
//    return true;
//
//  if (! gpBufferPool->bLowMemory())
//    return true;
//
//  if (aJobCLass->m_aCommand2)
//    vTrace("Error: unable to run %s %s under low memory conditions\n"_Stz,
//           aJobCLass->m_aCommand1,aJobCLass->m_aCommand2);
//  else
//    vTrace("Error: unable to run %s under low memory conditions\n"_Stz,
//           aJobCLass->m_aCommand1);

//  return false;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* things to additinally check at boot                                        */
/*----------------------------------------------------------------------------*/
void AM::vBootCheck(void)
{
  // No of entries in eCN , aCN and gpDump should match
  i8 iEntriesECN = CN_ENUMS;
  i8 iEntriesPCN = 0;
  i8 iEntriesGCLASSSIZE = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (AM::bVerboseProgress())
    vInfo("  aCN[] has CN_ENUMS\n"_Stz);
  
  bool bFoundEnum = false;
  // find number of entries in aCN
  for (i8 i_Class = 0; i_Class <= CN_ENUMS; i_Class++) {
    if (aCN[i_Class]->bEqual("ENUMS"_Stz)) {
      bFoundEnum = true;
      break;
    }
    iEntriesPCN++;
  }
  
  if (!bFoundEnum)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Enum for CN_ENUMS not found in aCN\n"_Stz);
  
  if (iEntriesECN != iEntriesPCN) {
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "\n!!!ALERT!!! No of entries in aCN (%lld) "
           "do not match the no in eCN (%lld) -- missing comma??\n\n"_Stz,
           iEntriesPCN, iEntriesECN);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (AM::bVerboseProgress())
    vInfo("  gxaSizeHeap[] matches aCN[]\n"_Stz);
  
  bFoundEnum = false;
  // find number of entries in gxaSizeHeap
  for (i8 i_Class = 0; i_Class <= CN_ENUMS; i_Class++) {
    if (gxaSizeHeap[i_Class].m_nClassSize == -1) {
      bFoundEnum = true;
      break;
    }
    iEntriesGCLASSSIZE++;
  }
  
  if (!bFoundEnum)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Enum for CN_ENUM not found in gxaSizeHeap\n"_Stz);
  
  if (iEntriesECN != iEntriesGCLASSSIZE) {
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "\n!!!ALERT!!! No of entries in gxaSizeHeap (%lld) "
           "do not match the no in eCN (%lld)\n\n"_Stz,
           iEntriesGCLASSSIZE, iEntriesECN);
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (AM::bVerboseProgress())
    vInfo("  Heap assignment matches class size\n"_Stz);
  
  // check sizes are less than pool block size
  for (i8 iClass = 0; iClass <= CN_ENUMS; iClass++) {
    
    eCN eClass = (eCN)iClass;
    i8  iClassSize = gxaSizeHeap[eClass].m_nClassSize;
    UNUSED(iClassSize);

    switch (gxaSizeHeap[eClass].m_eWhichHeap) {
        
      case HT_INVALID:
        continue;
        
      case HT_P_TINY:
      case HT_T_TINY:
        ASSERT(iClassSize < C_SIZE_TINY_OBJECT);
        continue;
        
      case HT_P_SMALL:
      case HT_T_SMALL:
        ASSERT(iClassSize < C_SIZE_SMALL_OBJECT);
        continue;
        
      case HT_P_MEDIUM:
      case HT_T_MEDIUM:
        ASSERT(iClassSize < C_SIZE_MEDIUM_OBJECT);
        continue;
        
      case HT_P_LARGE:
      case HT_T_LARGE:
        ASSERT(iClassSize < C_SIZE_LARGE_OBJECT);
        continue;
                
      default:
        ASSERT(0);
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
}

/*----------------------------------------------------------------------------*/
/* get build string                                                           */
/*----------------------------------------------------------------------------*/
Stz* AM::szBuild(void)
{
#ifdef WITH_DEBUG_VERSION
  return "debug"_Stz;
#endif
#ifdef WITH_RELEASE_VERSION
  return "release"_Stz;
#endif
  NEVER_REACH
}
  
  
void AM::vLogOpenByApi(void)           { Log::vOpenByApi();          }
void AM::vLogOpenBySE(bool bStarting)  { Log::vOpenBySE(bStarting);  }
void AM::vLogWriteCsv(bool bShortForm) { Log::vWriteCsv(bShortForm); }
void AM::vLogText(i8 , Stz* ) {;}
bool AM::bLogEntryExists(i8 i_Entry) {  return Log::bEntryExists(i_Entry); }
i8   AM::iLogEntryPrev(i8 i_Entry)   {  return Log::iEntryPrev(i_Entry); }
void AM::vLogEntrySprint(s4K& sString, i8 i_Entry) {
  Log::vEntrySprint(sString,i_Entry);
}

void AM::vFilePrint(File* pFile, Stz* szFormat,...) {
  
  va_list xArgs;
  va_start(xArgs,szFormat);
  pFile->vPrint(xArgs, szFormat);
  va_end(xArgs);
  
}

void AM::vLog(TokenVec *pTknVec) { gpLog->vLog(pTknVec);                        }


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

i8 AM::iRdTickingDevice(void) { return TickingDevice::iGetTick();                  }
i8 AM::iAMTimePoint(void) { return gpAttachMap->m_iTimePoint;                  }

bool AM::bIsOperandOoO(Operand* pOperand)   {
  
  // is it a marker ?
  if ( ! pOperand->bIsMarker() )
    return false;
  
  Marker* pMarker = _pMarker(pOperand);
  if (pMarker->eOpMarker() == OM_CANCELMARKER)
    return true;
  
  return false;
}

bool AM::bInBufferPool(Object* pObject) {
  return gpBufferPool->bInBufferPool(pObject);
}

Stz* AM::szJobName(Job* pJob)    { return pJob->szJobNm();                      }
i8   AM::iJobUniqueId(Job* pJob) { return pJob->m_iObjectId;                     }

void AM::vPrintGlobals(void)
{
  printf("gxCmdLineArgs     @%p\n",&gxCmdLineArgs);
  printf("\n");

  printf("gpAttachMap     @%p : %p\n",&gpAttachMap,  gpAttachMap);
  printf("gpBufferPool    @%p : %p\n",&gpBufferPool, gpBufferPool);
  printf("gpLog           @%p : %p\n",&gpLog,        gpLog);
  printf("gaSeShmRegion   @%p : %p\n",&gaSeShmRegion,gaSeShmRegion);
  printf("gpTraceFile     @%p : %p\n",&gpTraceFile,  gpTraceFile);
}

Index* AM::pReadApiIndex(void) { return gpReadApiIndex; }
void   AM::vReadApiIndex(Index* pIndex) { gpReadApiIndex = pIndex; }

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

void AM::vJobContentDump(Job* pJob) { pJob->vContentDump(); }

void AM::vJobDelete(Job** ppJob) {
  
  delete * ppJob;
  
  * ppJob = nullptr;
}

void AM::vJobInitExeContext(Job* pJob) {
  
  if (! pJob->pLogicUnit_job()) {
    LogicUnit* pLogicUnit = new LogicUnit(pJob->pTask());
    pJob->vLogicUnit_job( & pLogicUnit );
  }

}




Task* AM::pTaskNew(Job* pJob) {
  Task* pTask = new Task(pJob);
  pJob->vTask(pTask);
  return pTask;
}
bool AM::bTaskIsLocked(Task* pTask) {
  bool bLocked = pTask->bIsLocked();
  return bLocked;
}

Job* AM::pTaskJob(void) {
  return pThisTLSTask()->pJob();
}
Job* AM::pTaskJob(Task* pTask) {
  if (pTask)
    return pTask->pJob();
  else
    return nullptr;
}

void AM::vJobObjectName(Job* pJob, Stz* szObjectName) {
  pJob->pJobTags()->vInsert(C_TAG_TABLENM,szObjectName);
}






////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

ReqTuple* AM::aReqTuple_tls(void) {
  
  Job* pJob_tls = AM::pThisTLSJob();
  
  if (pJob_tls)
    return &pJob_tls->m_xReqTuple;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get RU_Cpu                                                                 */
/*----------------------------------------------------------------------------*/
RU_Cpu* AM::pRU_Cpu(void)
{
  Task* pTask = AM::pThisTLSTask();
  
  if (! pTask)
    return nullptr;
  
  Profile* pProfile = pTask->pProfile();
  if (! pProfile)
    return nullptr;
  
  RU_Cpu* pRU_Cpu = pProfile->pRU_Cpu();
  return pRU_Cpu;
}

/*----------------------------------------------------------------------------*/
/* get RU_Memory                                                              */
/*----------------------------------------------------------------------------*/
RU_Memory* AM::pRU_Memory(void)
{
  Task* pTask = AM::pThisTLSTask();
  
  if (! pTask)
    return nullptr;
  
  Profile* pProfile = pTask->pProfile();
  if (! pProfile)
    return nullptr;
  
  RU_Memory* pRU_Memory = pProfile->pRU_Memory();
  return pRU_Memory;
}

/*----------------------------------------------------------------------------*/
/* allocInc                                                                   */
/*----------------------------------------------------------------------------*/
void AM::vAllocInc(RU_Memory* pRU_Memory, eCN eAllocClass)
{
  pRU_Memory->vAllocInc(eAllocClass);
}

/*----------------------------------------------------------------------------*/
/* FreeInc                                                                    */
/*----------------------------------------------------------------------------*/
void AM::vFreeInc(RU_Memory* pRU_Memory, eCN eAllocClass)
{
  pRU_Memory->vFreeInc(eAllocClass);
}

/*----------------------------------------------------------------------------*/
/* dump routine of word                                                       */
/*----------------------------------------------------------------------------*/
void AM::vWordSee(Word* pWord, DicStack* pDicStack)
{
  s48K sString;
  
  SP_Word spWord;
  vTrace("\n"_Stz);
  vTrace("See: %s\n"_Stz,spWord.szSeeWord(pWord));
  vTrace("-------------------------------------------------------------\n"_Stz);

  CodeCell* aDoes = pWord->aEntryCell();
  
  Word* pDoesWord = CodeBlock::pToWord(aDoes);
  
  vTrace("%s\n"_Stz,spWord.szSeeDoes(pDoesWord));

  AM::vWordSeeDoes(pDoesWord,aDoes, pDicStack);
  
  vTrace("=============================================================\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* dump routine of word                                                       */
/*----------------------------------------------------------------------------*/
void AM::vWordSeeDoes(Word* pDoesWord, CodeCell* aDoes, DicStack* pDicStack)
{
  vTrace("- - - - - - - - - - - - - - - - - - - - -\n"_Stz);

  CodeCell* aLastCell = pDoesWord->aLastCell();
  for (CodeCell* aCell = aDoes; aCell < aLastCell; ) {
    
    fpSTEPFN fpStepFn = *aCell->m_afpStepFn;
    Word* pStepWord = pDicStack->pLocateStepWord(fpStepFn);
    Stz* szStepNm = pStepWord->szWordNm();
    
    // -- -- -- -- -- -- -- -- -- --
    // step is: jump / jump false / jump true
    
    if (fpStepFn == &Step::vSenJump ||
        fpStepFn == &Step::vSenJumpFalse ||
        fpStepFn == &Step::vSenJumpTrue ) {
      
      vTrace("%p: %s"_Stz,aCell,szStepNm);
      aCell++;
      
      vTrace(" %p\n"_Stz,aCell->m_aJumpTo);
      aCell++;
      
      continue;
    }
    
    // -- -- -- -- -- -- -- -- -- --
    // step is: push intval
    
    if (fpStepFn == &Step::vPushIntVal) {
      
      vTrace("%p: %s"_Stz,aCell,szStepNm);
      aCell++;
      
//      IntVal* aIntVal = _aIntVal(aCell);
      TypedCell* aTypedCell = _aTypedCell(aCell);
      vTrace(" %lld\n"_Stz,aTypedCell->m_wxValueCell.iGetColVal());
      
      i8 iBytes = aTypedCell->nStackUsage();
      aCell = aAdd<CodeCell*>(aCell,iBytes);
      
      continue;
    }
    
    // -- -- -- -- -- -- -- -- -- --
    // step is: push ptrval
    
    if (fpStepFn == &Step::vPushPtrVal) {
      
      vTrace("%p: %s"_Stz,aCell,szStepNm);
      aCell++;
      
      //PtrVal* aPtrVal = _aPtrVal(aCell);
      //eCN eClassNo_obj = aPtrVal->pObj()->eGetClassNo();
      TypedCell* aTypedCell = _aTypedCell(aCell);
      eCN eClassNo_obj = aTypedCell->pObj()->eGetClassNo();

      if (CN_WORD == eClassNo_obj) {
        Word* pWord = _pWord(aTypedCell->pObj());
        vTrace(" %p %s\n"_Stz,pWord,pWord->szWordNm());
      }
      else {
        vTrace(" %p %s\n"_Stz,aCN[eClassNo_obj]);
      }
      
      i8 iBytes = aTypedCell->nStackUsage();
      aCell = aAdd<CodeCell*>(aCell,iBytes);
      
      continue;
    }
    
    // -- -- -- -- -- -- -- -- -- --
    // step is: push stzval
    
    if (fpStepFn == &Step::vPushStzVal) {
      
      vTrace("%p: %s"_Stz,aCell,szStepNm);
      aCell++;
      
      TypedCell* aTypedCell = _aTypedCell(aCell);
      vTrace(" \"%s\"\n"_Stz,aTypedCell->szStz());
      
      i8 iBytes = aTypedCell->nStackUsage();
      aCell = aAdd<CodeCell*>(aCell,iBytes);
      
      continue;
    }
    
    // -- -- -- -- -- -- -- -- -- --
    // step is: enter routine
    
    if (fpStepFn == &Step::vSenDoColon) {
      
      CodeCell* aEnterCell = *(CodeCell**)(aCell->m_afpStepFn + 1);
      Word* pEnterWord = CodeBlock::pToWord(aEnterCell);

      vTrace("%p: %s %s\n"_Stz,
             aCell,
             szStepNm,
             pEnterWord->szWordNm());
      aCell++;
      
      continue;
    }
    
    // -- -- -- -- -- -- -- -- -- --
    // step is: call step
  
    vTrace("%p: %s\n"_Stz,aCell,szStepNm);
    aCell++;
  }
  
}

static Stz* aEG[]
{
  "EG_NONE"_Stz,
  
  "EG_AA"_Stz,
  "EG_AC"_Stz,
  "EG_AK"_Stz,
  "EG_AN"_Stz,
  "EG_AT"_Stz,

  "EG_BO"_Stz,

  "EG_CC"_Stz,
  "EG_CK"_Stz,
  "EG_CM"_Stz,
  "EG_CN"_Stz,
  "EG_CP"_Stz,
  "EG_CS"_Stz,
  "EG_CT"_Stz,
  "EG_CX"_Stz,

  "EG_DA"_Stz,
  "EG_DC"_Stz,
  "EG_DL"_Stz,
  "EG_DM"_Stz,
  "EG_DS"_Stz,
  
  "EG_EA"_Stz,
  "EG_EC"_Stz,
  "EG_ED"_Stz,
  "EG_EK"_Stz,
  "EG_EO"_Stz,
  "EG_EP"_Stz,

  "EG_FN"_Stz,

  "EG_GA"_Stz,
  
  "EG_HT"_Stz,
  "EG_HS"_Stz,
  "EG_HT"_Stz,

  "EG_IK"_Stz,
  "EG_IS"_Stz,
  "EG_IT"_Stz,

  "EG_JA"_Stz,
  "EG_JF"_Stz,
  "EG_JK"_Stz,
  "EG_JL"_Stz,
  "EG_JP"_Stz,
  "EG_JS"_Stz,
  
  "EG_KC"_Stz,
  "EG_KE"_Stz,
  "EG_KK"_Stz,

  "EG_LA"_Stz,
  "EG_LN"_Stz,
  
  "EG_MA"_Stz,
  "EG_MC"_Stz,
  "EG_MM"_Stz,
  "EG_MT"_Stz,

  "EG_NE"_Stz,
  "EG_NK"_Stz,
  "EG_NS"_Stz,
  
  "EG_OA"_Stz,
  "EG_OC"_Stz,
  "EG_OI"_Stz,
  "EG_OK"_Stz,
  "EG_OM"_Stz,
  "EG_OP"_Stz,
  "EG_OR"_Stz,

  "EG_PA"_Stz,
  "EG_PF"_Stz,
  "EG_PK"_Stz,
  "EG_PO"_Stz,
  "EG_PP"_Stz,
  "EG_PT"_Stz,
  "EG_PX"_Stz,
  "EG_PY"_Stz,
  
  "EG_QA"_Stz,
  "EG_QK"_Stz,

  "EG_RA"_Stz,
  "EG_RE"_Stz,
  "EG_RG"_Stz,
  "EG_RO"_Stz,
  "EG_RS"_Stz,
  "EG_RU"_Stz,
  
  "EG_SC"_Stz,
  "EG_SI"_Stz,
  "EG_SK"_Stz,
  "EG_SL"_Stz,
  "EG_SS"_Stz,
  "EG_ST"_Stz,

  "EG_TA"_Stz,
  "EG_TN"_Stz,
  "EG_TP"_Stz,
  "EG_TR"_Stz,
  "EG_TS"_Stz,
  "EG_TT"_Stz,
  
  "EG_UF"_Stz,
  "EG_UK"_Stz,

  "EG_VA"_Stz,
  "EG_VL"_Stz,

  "EG_WK"_Stz,

  "EG_XA"_Stz,
  "EG_XK"_Stz,
  "EG_XP"_Stz,

  "EG_YA"_Stz,
  
  "EG_ZK"_Stz,
  
  "EG_ENUMS"_Stz,
};
  

/*----------------------------------------------------------------------------*/
/* get enum text                                                              */
/*----------------------------------------------------------------------------*/
Stz* AM::szEnumText(i8 i_EnumGroup, i8 i_Enum)
{
  switch (i_EnumGroup) {
      
    case EG_AA: return aAA[i_Enum];
    case EG_AC: return aAC[i_Enum];
    case EG_AK: return aAK[i_Enum];
    //case EG_AN: return aAN[i_Enum];
    case EG_AT: return aAT[i_Enum];
             
    case EG_BO: return aBO[i_Enum];
      
    case EG_CC: return aCC[i_Enum];
    case EG_CK: return aCK[i_Enum];
    case EG_CM: return aCM[i_Enum];
    case EG_CN: return aCN[i_Enum];
    case EG_CP: return aCP[i_Enum];
    //case EG_CS: return aCS[i_Enum];
    case EG_CT: return aCT[i_Enum];
    case EG_CX: return aCX[i_Enum];

    case EG_DA: return aDA[i_Enum];
    case EG_DC: return aDC[i_Enum];
    case EG_DL: return aDL[i_Enum];
    case EG_DM: return aDM[i_Enum];
    case EG_DS: return aDS[i_Enum];
      
    case EG_EA: return aEA[i_Enum];
    case EG_EC: return aEC[i_Enum];
    case EG_ED: return aED[i_Enum];
    case EG_EO: return aEO[i_Enum];
    case EG_EP: return aEP[i_Enum];

    case EG_FN: return aFN[i_Enum];

    case EG_GA: return aGA[i_Enum];

    case EG_HA: return aHA[i_Enum];
    case EG_HS: return aHS[i_Enum];
    case EG_HT: return aHT[i_Enum];
      
    case EG_IK: return aIK[i_Enum];
    case EG_IS: return aIS[i_Enum];
    case EG_IT: return aIT[i_Enum];
      
    case EG_JA: return aJA[i_Enum];
    case EG_JF: return aJF[i_Enum];
    case EG_JK: return aJK[i_Enum];
    case EG_JL: return aJL[i_Enum];
    case EG_JP: return aJP[i_Enum];
    case EG_JS: return aJS[i_Enum];
      
    case EG_KC: return aKC[i_Enum];
    case EG_KE: return aKE[i_Enum];
    case EG_KK: return aKK[i_Enum];

    case EG_LA: return aLA[i_Enum];
    case EG_LG: return aLG[i_Enum];
    case EG_LH: return aLH[i_Enum];
      
    case EG_MA: return aMA[i_Enum];
    case EG_MC: return aMC[i_Enum];
    case EG_MM: return aMM[i_Enum];
    case EG_MT: return aMT[i_Enum];

    case EG_NE: return aNE[i_Enum];
    case EG_NK: return aNK[i_Enum];
    case EG_NS: return aNS[i_Enum];
      
    case EG_OA: return aOA[i_Enum];
    case EG_OC: return aOC[i_Enum];
    case EG_OI: return aOI[i_Enum];
    case EG_OK: return aOK[i_Enum];
    case EG_OM: return aOM[i_Enum];
    case EG_OP: return aOP[i_Enum];
    case EG_OR: return aOR[i_Enum];

    case EG_PA: return aPA[i_Enum];
    case EG_PF: return aPF[i_Enum];
    case EG_PK: return aPK[i_Enum];
    case EG_PO: return aPO[i_Enum];
    case EG_PP: return aPP[i_Enum];
    case EG_PT: return aPT[i_Enum];
    case EG_PX: return aPX[i_Enum];
    case EG_PY: return aPY[i_Enum];

    case EG_QA: return aQA[i_Enum];
    case EG_QK: return aQK[i_Enum];

    case EG_RA: return aRA[i_Enum];
    case EG_RE: return aLE[i_Enum];
    case EG_RG: return aRG[i_Enum];
    case EG_RO: return aRO[i_Enum];
    case EG_RS: return aRS[i_Enum];
    case EG_RU: return aRU[i_Enum];
      
    case EG_SC: return aSC[i_Enum];
    case EG_SI: return aSI[i_Enum];
    case EG_SK: return aSK[i_Enum];
    case EG_SL: return aSL[i_Enum];
    case EG_SS: return aSS[i_Enum];
    case EG_ST: return aST[i_Enum];

    case EG_TA: return aTA[i_Enum];
    case EG_TN: return aTN[i_Enum];
    case EG_TP: return aTP[i_Enum];
    case EG_TR: return aTR[i_Enum];
    case EG_TS: return aTS[i_Enum];
    case EG_TT: return aTT[i_Enum];
      
    case EG_UF: return aUF[i_Enum];
    case EG_UK: return aUK[i_Enum];

    case EG_VA: return aVA[i_Enum];
    case EG_VL: return aVL[i_Enum];
 
    case EG_WK: return aWK[i_Enum];

    case EG_XA: return aXA[i_Enum];
    case EG_XK: return aXK[i_Enum];
    case EG_XP: return aXP[i_Enum];

    case EG_YA: return aYA[i_Enum];

    case EG_ZK: return aZK[i_Enum];

    default:
      vTrace("AM::szEnumText: Unknown enum group %lld\n"_Stz, i_EnumGroup);
      return nullptr;
  }
}


/*----------------------------------------------------------------------------*/
/* set format from given tag set                                              */
/*----------------------------------------------------------------------------*/
bool AM::bFormatSetFromTagSet(Format* pFormat, TagSet* pTagSet)
{
  s32 sTagValue;
  i8 iTagValue;
  i8 i_Tag;

  // - - - - - - - - - - - - - - - - - - - - -
  // C_RECORD_TERMINATOR
  i_Tag = pTagSet->i_Find(C_TAG_LD_RECORD_TERMINATOR);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pTagSet->szValue(i_Tag);
    pFormat->m_cRecordTerminator = *sTagValue.azChar();
    if (pFormat->m_cRecordTerminator == '0')
      pFormat->m_cRecordTerminator = '\0';
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // C_RECORD_ENDS
  i_Tag = pTagSet->i_Find(C_TAG_LD_RECORD_ENDS);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pTagSet->szValue(i_Tag);
    char cRecordEnds = *sTagValue.azChar();
    
    // is record_ends 1 character ?
    if (sTagValue.nLength() == 1) {
      if (cRecordEnds == '0') {
        vWarn("Invalid load : record_ends can only be \\n, \\r or \\r\\n\n"_Stz);
        return false;
      }
      else if (cRecordEnds == '\n') {
        pFormat->m_eLineEnd = LE_NL;
      }
      else if (cRecordEnds == '\r') {
        pFormat->m_eLineEnd = LE_CR;
      }
      else {
        vWarn("Invalid load : record_ends can only be \\n, \\r or \\r\\n\n"_Stz);
        return false;
      }
    }
    // record_ends is 2 characters
    else if (sTagValue.nLength() == 2) {
      
      if (sTagValue.bSame("\r\n"_Stz)) {
        pFormat->m_eLineEnd = LE_CRNL;
      }
      else {
        vWarn("Invalid load : record_ends can only be \\n, \\r or \\r\\n\n"_Stz);
        return false;
      }
    }
    else {
      vWarn("Invalid load : record_ends can only be \\n, \\r or \\r\\n\n"_Stz);
      return false;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // C_FIELD_DELIMITER
  i_Tag = pTagSet->i_Find(C_TAG_LD_FIELD_DELIMITER);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pTagSet->szValue(i_Tag);
    if (sTagValue.bSame("\\t"_Stz))
      pFormat->m_cFieldDelimiter = '\t';
    else
      pFormat->m_cFieldDelimiter = *sTagValue.azChar();
    if (pFormat->m_cFieldDelimiter == '0')
      pFormat->m_cFieldDelimiter = '\0';
  }

  // - - - - - - - - - - - - - - - - - - - - -
  // C_ENCLOSED_BY_LEADING
  i_Tag = pTagSet->i_Find(C_TAG_LD_ENCLOSED_BY_LEADING);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pTagSet->szValue(i_Tag);
    pFormat->m_cEnclosedBy_Leading = *sTagValue.azChar();
    if (pFormat->m_cEnclosedBy_Leading == '0')
      pFormat->m_cEnclosedBy_Leading = '\0';
        
    // default set trailing same as leading
    pFormat->m_cEnclosedBy_Trailing =
    pFormat->m_cEnclosedBy_Leading;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // C_ENCLOSED_BY_TRAILING
  i_Tag = pTagSet->i_Find(C_TAG_LD_ENCLOSED_BY_TRAILING);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pTagSet->szValue(i_Tag);
    // if trailing is specified, leading should also have been specified
    if (pFormat->m_cEnclosedBy_Trailing == '\0') {
      if (! AM::bMetadata())
        vWarn("Invalid load : enclosed_by_trailing specified but no enclosed_by_leading\n"_Stz);
      return false;
    }
    pFormat->m_cEnclosedBy_Trailing = *sTagValue.azChar();
    if (pFormat->m_cEnclosedBy_Trailing == '0')
      pFormat->m_cEnclosedBy_Trailing = '\0';
  }
  //
  //    DATE_FORMAT='YYYY.MM.DD’
  //
  //
  // - - - - - - - - - - - - - - - - - - - - -
  // C_DATE_FORMAT
  i_Tag = pTagSet->i_Find(C_TAG_LD_DATE_FORMAT);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pTagSet->szValue(i_Tag);
    // extract date format
    if ( ! pFormat->bExtractDateFormat(sTagValue.szStz()) ) {
      if ( ! AM::bMetadata())
        vWarn("Invalid load : enclosed_by_trailing specified but no enclosed_by_leading\n"_Stz);
      return false;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // C_ESCAPE_CHAR
  i_Tag = pTagSet->i_Find(C_TAG_LD_ESCAPE_CHAR);
  if (i_Tag != C_UNKNOWN) {
    sTagValue = pTagSet->szValue(i_Tag);
    pFormat->m_cEscapeChar = *sTagValue.azChar();
    if (pFormat->m_cEscapeChar == '0')
      pFormat->m_cEscapeChar = '\0';
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // C_MAX_ERRORS
  i_Tag = pTagSet->i_Find(C_TAG_LD_MAX_ERRORS);
  if (i_Tag != C_UNKNOWN) {
    iTagValue = pTagSet->iValue(i_Tag);
    
    pFormat->m_nMaxErrors = iTagValue;
  }
        
  return true;
}









/*----------------------------------------------------------------------------*/
/* add job to demon list                                                      */
/*----------------------------------------------------------------------------*/
void AM::vJob_AddDemon(Job* pJob)
{
  AM::pDemonJobs()->vGrabLock();
  AM::pDemonJobs()->vAddJob(pJob);
  AM::pDemonJobs()->vDropLock();
  
  pJob->vEventNoInc();
  AM::vLogJob(SL_INF,pJob,JA_JOB_ADD_DEMON,
              "Job added to DemonJobs"_Stz); //!format-LogJob
}

/*----------------------------------------------------------------------------*/
/* add job to local list                                                      */
/*----------------------------------------------------------------------------*/
void AM::vJob_AddLocal(Job* pJob)
{
  AM::pLocalJobs()->vGrabLock();
  AM::pLocalJobs()->vAddJob(pJob);
  AM::pLocalJobs()->vDropLock();
  
  pJob->vEventNoInc();
  AM::vLogJob(SL_INF,pJob,JA_JOB_ADD_LOCAL,
              "Job added to LocalJobs"_Stz); //!format-LogJob
}

/*----------------------------------------------------------------------------*/
/* add job to originating list                                                */
/*----------------------------------------------------------------------------*/
void AM::vJob_AddOriginating(Job* pJob)
{
  AM::pOriginatingJobs()->vGrabLock();
  AM::pOriginatingJobs()->vAddJob(pJob);
  AM::pOriginatingJobs()->vDropLock();
  
  pJob->vEventNoInc();
  AM::vLogJob(SL_INF,pJob,JA_JOB_ADD_ORIGIN,
              "Job added to OriginatingJobs"_Stz); //!format-LogJob
}

/*----------------------------------------------------------------------------*/
/* add job to participating list                                              */
/*----------------------------------------------------------------------------*/
void AM::vJob_AddParticipating(Job* pJob)
{
  AM::pParticipatingJobs()->vGrabLock();
  AM::pParticipatingJobs()->vAddJob(pJob);
  AM::pParticipatingJobs()->vDropLock();
  
  pJob->vEventNoInc();
  AM::vLogJob(SL_INF,pJob,JA_JOB_ADD_PARTG,
              "Job added to ParticipatingJobs"_Stz); //!format-LogJob
  
}

/*----------------------------------------------------------------------------*/
/* dump demon job list                                                        */
/*----------------------------------------------------------------------------*/
void AM::vJob_DumpDemon(void)
{
  Schedule* pSchedule = AM::pSchedule();
  
  AM::vLogQue(SL_INF,pSchedule,QA_QUE_ORIGINATORS,
              "DemonJobs list:"_Stz); //!format-LogQue
  
  AM::pDemonJobs()->vGrabLock();
  AM::pDemonJobs()->vContentDump();
  AM::pDemonJobs()->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* dump local job list                                                        */
/*----------------------------------------------------------------------------*/
void AM::vJob_DumpLocal(void)
{
  Schedule* pSchedule = AM::pSchedule();
  
  AM::vLogQue(SL_INF,pSchedule,QA_QUE_ORIGINATORS,
              "LocalJobs list:"_Stz); //!format-LogQue
  
  AM::pLocalJobs()->vGrabLock();
  AM::pLocalJobs()->vContentDump();
  AM::pLocalJobs()->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* dump originating job list                                                  */
/*----------------------------------------------------------------------------*/
void AM::vJob_DumpOriginating(void)
{
  Schedule* pSchedule = AM::pSchedule();
  
  AM::vLogQue(SL_INF,pSchedule,QA_QUE_ORIGINATORS,
              "OriginatingJobs list:"_Stz); //!format-LogQue

  AM::pOriginatingJobs()->vGrabLock();
  AM::pOriginatingJobs()->vContentDump();
  AM::pOriginatingJobs()->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* dump participating job list                                                */
/*----------------------------------------------------------------------------*/
void AM::vJob_DumpParticipating(void)
{
  Schedule* pSchedule = AM::pSchedule();
  
  AM::vLogQue(SL_INF,pSchedule,QA_QUE_PARTICPANTS,
              "ParticipatingJobs list:"_Stz); //!format-LogQue
  
  AM::pParticipatingJobs()->vGrabLock();
  AM::pParticipatingJobs()->vContentDump();
  AM::pParticipatingJobs()->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* find job in originating list                                               */
/*----------------------------------------------------------------------------*/
Job* AM::pJob_FindOriginating(i8 iJobId)
{
  Schedule* pSchedule = AM::pSchedule();
  
  AM::vLogQue(SL_INF,pSchedule,QA_QUE_FIND_ORIG,
              "Find job %lld in OriginatingJobs"_Stz, //!format-LogQue
              iJobId);
  
  Job* pJob = AM::pOriginatingJobs()->pFindJob(iJobId);
  
  if (pJob)
    AM::vLogQue(SL_INF,pSchedule,QA_QUE_FOUND_ORIG,
                "found job %lld in OriginatingJobs"_Stz, //!format-LogQue
                iJobId);
  else
    AM::vLogQue(SL_INF,pSchedule,QA_QUE_NOT_ORIG,
                "did not find job %lld in OriginatingJobs"_Stz, //!format-LogQue
                iJobId);
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* find job in participating list                                             */
/*----------------------------------------------------------------------------*/
Job* AM::pJob_FindParticipating(i8 iJobId)
{
  Schedule* pSchedule = AM::pSchedule();

  AM::vLogQue(SL_INF,pSchedule,QA_QUE_FIND_PARTG,
              "find job %lld in ParticipatingJobs"_Stz, //!format-LogQue
              iJobId);
  
  Job* pJob = AM::pParticipatingJobs()->pFindJob(iJobId);
  
  if (pJob)
    AM::vLogQue(SL_INF,pSchedule,QA_QUE_FOUND_PARTG,
                "found job %lld in ParticipatingJobs"_Stz, //!format-LogQue
                iJobId);
  else
    AM::vLogQue(SL_INF,pSchedule,QA_QUE_NOT_PARTG,
                "did not find job %lld in ParticipatingJobs"_Stz, //!format-LogQue
                iJobId);

  return pJob;
}

/*----------------------------------------------------------------------------*/
/* remove job from local list                                                 */
/*----------------------------------------------------------------------------*/
bool AM::bJob_RemoveLocal(Job* pJob, bool bDump)
{
  AM::pLocalJobs()->vGrabLock();
  
  if (bDump)
    AM::pLocalJobs()->vContentDump();
  
  bool bRemoved = AM::pLocalJobs()->bRemoveJob(pJob);
  
  AM::pLocalJobs()->vDropLock();
  
  pJob->vEventNoInc();
  AM::vLogJob(SL_INF,pJob,JA_JOB_REMOVE_LOCAL,
              "Job removed from LocalJobs"_Stz); //!format-LogJob
  
  return bRemoved;
}

/*----------------------------------------------------------------------------*/
/* remove job from originating list                                           */
/*----------------------------------------------------------------------------*/
bool AM::bJob_RemoveOriginating(Job* pJob, bool bDump)
{
  AM::pOriginatingJobs()->vGrabLock();
  
  if (bDump)
    AM::pOriginatingJobs()->vContentDump();
  
  bool bRemoved = AM::pOriginatingJobs()->bRemoveJob(pJob);
  
  AM::pOriginatingJobs()->vDropLock();
  
  pJob->vEventNoInc();
  AM::vLogJob(SL_INF,pJob,JA_JOB_REMOVE_ORIGIN,
              "Job removed from OriginatingJobs"_Stz); //!format-LogJob
  
  return bRemoved;
}

/*----------------------------------------------------------------------------*/
/* remove job from participating list                                         */
/*----------------------------------------------------------------------------*/
bool AM::bJob_RemoveParticipating(Job* pJob, bool bDump)
{
  AM::pParticipatingJobs()->vGrabLock();
  
  if (bDump)
    AM::pParticipatingJobs()->vContentDump();

  bool bRemoved = AM::pParticipatingJobs()->bRemoveJob(pJob);
  
  AM::pParticipatingJobs()->vDropLock();
  
  pJob->vEventNoInc();
  AM::vLogJob(SL_INF,pJob,JA_JOB_REMOVE_PARTG,
              "Job fremoved from ParticipatingJobs"_Stz); //!format-LogJob
  
  return bRemoved;
}


/*----------------------------------------------------------------------------*/
/* update heartbeat info for a given node                                     */
/*----------------------------------------------------------------------------*/
void AM::vPutHeartbeat(i8 iNode, const Heartbeat& xHeartbeat)
{
  Node *pThisNode = AM::pGrid()->pGetNode(iNode);

  if (pThisNode) {
    pThisNode->vPutHeartbeat(xHeartbeat);
  } else {
    AM::vLogFyi("Cannot find Node %lld in the cluster"_Stz, //!format-LogFyi
                iNode);
  }
}

/*----------------------------------------------------------------------------*/
/* update heartbeat for a given node by name                                  */
/*----------------------------------------------------------------------------*/
void AM::vPutHeartbeat(Stz* pszName, const Heartbeat& xHeartbeat )
{
  Node *pThisNode = AM::pGrid()->pFindNode(pszName);

  if (pThisNode) {
    pThisNode->vPutHeartbeat(xHeartbeat);
  } else {
    AM::vLogFyi("Cannot find Node %s in the cluster"_Stz, //!format-LogFyi
                pszName);
  }
}

/*----------------------------------------------------------------------------*/
/* get heartbeat info for a give node                                         */
/*----------------------------------------------------------------------------*/
Heartbeat AM::xGetHeartbeat(i8 iNode)
{
  Node *pThisNode = AM::pGrid()->pGetNode(iNode);

  if (!pThisNode) {
    vThrow(FK_FATAL, VL_FATAL, __func__, __FILE__, __LINE__,
           "Cannot find Node %lld in the cluster"_Stz,
           iNode);
  }
  return pThisNode->xGetHeartbeat();
}

/*----------------------------------------------------------------------------*/
/* get heartbeat info for a give node                                         */
/*----------------------------------------------------------------------------*/
Heartbeat AM::xGetHeartbeat(Stz* pszName)
{
  Node *pThisNode = AM::pGrid()->pFindNode(pszName);

  if (!pThisNode) {
    vThrow(FK_FATAL, VL_FATAL, __func__, __FILE__, __LINE__,
           "Cannot find Node %s in the cluster"_Stz,
           pszName);
  }

  return pThisNode->xGetHeartbeat();

}


/*----------------------------------------------------------------------------*/
/* get supervisor halt flag                                                   */
/*----------------------------------------------------------------------------*/
bool AM::bSupervisorHalt(void)
{
  return gpAttachMap->m_bSupervisorHalt;
}

/*----------------------------------------------------------------------------*/
/* set supervisor halt flag                                                   */
/*----------------------------------------------------------------------------*/
void AM::vSupervisorHalt(bool bHalt)
{
  gpAttachMap->m_bSupervisorHalt = bHalt;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
