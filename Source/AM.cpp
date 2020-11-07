/*============================================================================*/
/* AM.cpp -- AttachMap helper                                                 */
/*============================================================================*/

#include "Oblique.h"

extern Args          gxArgs;
extern PromptCatalog gxPromptCatalog;

extern AttachMap*    gpAttachMap;
extern BufferPool*   gpBufferPool;
extern File*         gpTraceFile;
extern Header*       gaSeShmRegion;
extern Index*        gpReadApiIndex;
extern Log*          gpLog;

extern SizeHeap      gxaSizeHeap[];

// Arg
bool AM::bDebugIndex(void)    { return gpAttachMap->Arg.m_bDebugIndex;        }
bool AM::bDebugMode(void)     { return gpAttachMap->Arg.m_bDebugMode;         }
bool AM::bDeveloper(void)     { return gpAttachMap->Arg.m_bDeveloper;         }
i8   AM::nDummyLoop(void)     { return gpAttachMap->Arg.m_nDummyLoop;         }
i8   AM::nDummySleep(void)    { return gpAttachMap->Arg.m_nDummySleep;        }
i8   AM::nDummyThreads(void)  { return gpAttachMap->Arg.m_nDummyThreads;      }
bool AM::bLog(void)           { return gpAttachMap->Arg.m_bLog;               }
i8   AM::nMaxLoadErrors(void) { return gpAttachMap->Arg.m_nMaxLoadErrors;     }
i8   AM::nMaxReadyLP(void)    { return gpAttachMap->Arg.m_nMaxReadyLP;        }
bool AM::bMunged(void)        { return gpAttachMap->Arg.m_bMunged;            }
Stz* AM::szRegionName(void)   { return gpAttachMap->Arg.m_sRegionName.szStz();}
bool AM::bRLUDiary(void)      { return gpAttachMap->Arg.m_bRLUDiary;          }
bool AM::bRLUTimer(void)      { return gpAttachMap->Arg.m_bRLUTimer;          }
i8   AM::iRegionSize(void)    { return gpAttachMap->Arg.m_iRegionSize;        }
i8   AM::nSubIngesters(void)  { return gpAttachMap->Arg.m_nSubIngesters;      }
i8   AM::nSubInserters(void)  { return gpAttachMap->Arg.m_nSubInserters;      }
i8   AM::nSubLoaders(void)    { return gpAttachMap->Arg.m_nSubLoaders;        }
i8   AM::nTablePtns(void)     { return gpAttachMap->Arg.m_nTablePtns;         }
i8   AM::nThreads(void)       { return gpAttachMap->Arg.m_nThreads;           }

TagSet* AM::pDbgTagSet(void)  { return & gpAttachMap->Arg.m_oDbgTagSet;       }

bool AM::bDbgTag(Stz* szTagNm) {
  
  if (! AM::bDebugMode())
    return false;
  
  bool bHasDbgTag = gpAttachMap->Arg.m_oDbgTagSet.bHasName(szTagNm);
  
  return bHasDbgTag;
}

Stz* AM::szCommand(void)      { return gxArgs.m_sCommand.szStz();             }
Stz* AM::szInitial(void)      { return gxArgs.m_sInitial.szStz();             }

// file names
Stz* AM::szLogRegion(void)    { return gpAttachMap->m_sLogRegion;             }
Stz* AM::szLogCsvFile(void)   { return gpAttachMap->m_sLogCsvFile;            }
Stz* AM::szLogTxtFile(void)   { return gpAttachMap->m_sLogTxtFile;            }

//
SessionArray* AM::pSessionArray(void) { return gpAttachMap->m_pSessionArray;  }


bool AM::bBootError(void)     { return gpAttachMap->m_bBootError;             }
i8   AM::nBuckets(void)       { return C_MAX_BUCKETS;                         }
BufferPool* AM::pBufferPool(void) { return gpAttachMap->m_pBufferPool;        }
CarryOver* AM::pCarryOver(void)   { return gpAttachMap->m_pCarryOver;         }
EnumFlags* AM::pClassFlags(void)  { return & gpAttachMap->m_oClassFlags;      }
Lock* AM::pDdlLock(void)      { return & gpAttachMap->m_okDdlLock;            }
JobList* AM::pDemonJobs(void) {
  return gpAttachMap->m_pQueues->m_pDemonJobs;                                }
Lock* AM::pDicLock(void)      { return & gpAttachMap->m_okDicLock;            }
i8   AM::iElapsedSsec(void)   { return gpAttachMap->m_oBirth.iElapsedSsec();  }
i8   AM::iEpoch(void)         { return gpAttachMap->m_iEpoch;                 }
Format* AM::pFmt(void)        { return & gpAttachMap->m_oFormatters;          }
Grid* AM::pGrid(void)         { return gpAttachMap->m_pGrid;                  }
bool AM::bHasStarted(void)    { return gpAttachMap->m_pGrid->bHasStarted();   }
HeapMem* AM::pHeapMem(void)   { return & gpAttachMap->m_xHeapMem;             }
i8   AM::nIndexEntries(void)  { return gpAttachMap->Arg.m_nIndexEntries;      }
JobList* AM::pLocalJobs(void) {
  return gpAttachMap->m_pQueues->m_pLocalJobs;                                }
LockTracker* AM::pLockTracker(void) { return & gpAttachMap->m_oLockTracker;   }
bool AM::bLogging(void)       { return gpLog ? true : false;                  }
i8   AM::iNextEpoch(void)     { return ++ gpAttachMap->m_iEpoch;              }
Stz* AM::szNodeNm(void)       {
  return gpAttachMap->m_pGrid->pThisNode()->szNodeNm();  }
JobList* AM::pOriginatingJobs(void) {
  return gpAttachMap->m_pQueues->m_pOriginatingJobs; }
JobList* AM::pParticipatingJobs(void) {
  return gpAttachMap->m_pQueues->m_pParticipatingJobs; }
i8   AM::iPid(void)           { return gpAttachMap->m_iPid;                   }
i8   AM::nConnectedNodes(void){ return gpAttachMap->m_pGrid->nConnectedNodes(); }
i8   AM::nDefinedNodes(void)  { return gpAttachMap->m_pGrid->nDefinedNodes(); }
Article* AM::pLexiconArticle(void) { return gpAttachMap->m_pLexiconArticle;    }
void AM::vProcessExit(void)   { Process::vExitFailure();                      }
bool AM::bProgress(void)      { return gpAttachMap->Arg.m_bProgress;              }
bool AM::bMetadata(void)      { return gpAttachMap->Arg.m_bMetadata;          }

i8   AM::iNextDiagram(void)   { return ++ gpAttachMap->m_inNextDiagram;       }
i8   AM::iNextJobId(void)     { return ++ gpAttachMap->m_inNextJobId;         }
i8   AM::iNextLockId(void)    {
  if (gpAttachMap)
    return ++ gpAttachMap->m_inNextLockId;
  else
    return C_UNKNOWN;
}
i8   AM::iNextOpId(void)      { return ++ gpAttachMap->m_inNextOpId;          }
i8   AM::iNextPlanId(void)    { return ++ gpAttachMap->m_inNextPlanId;        }
i8   AM::iNextPromptId(void)  { return ++ gpAttachMap->m_inNextPromptId;      }
i8   AM::iNextRequest(void)   { return ++ gpAttachMap->m_inNextRequest;       }
i8   AM::iNextResourceId(void){
  if (gpAttachMap)
    return ++ gpAttachMap->m_inNextResourceId;
  else
    return C_UNKNOWN;
}
i8   AM::iNextSessionId(void) { return ++ gpAttachMap->m_inNextSessionId;     }
i8   AM::iNextStreamId(void)  { return ++ gpAttachMap->m_inNextStreamId;      }
i8   AM::iNextTaskId(void)    { return ++ gpAttachMap->m_inNextTaskId;        }
i8   AM::iNextThreadId(void)  { return ++ gpAttachMap->m_inNextThreadId;      }
i8   AM::iNextTxId(void)      { return ++ gpAttachMap->m_inNextTxId;          }

i8   AM::nPartitionsInGrid(void)    { return gpAttachMap->m_pGrid->nPartitionsInGrid();   }
Queues* AM::pQueues(void)     { return gpAttachMap->m_pQueues;                }
ResourceTracker* AM::pResourceTracker(void) {
  return & gpAttachMap->m_oResourceTracker; }
Article* AM::pRootArticle(void)     { return gpAttachMap->m_pRootArcticle;    }
Word* AM::pRootConstant(void) { return gpAttachMap->m_pRootConstant;          }
Dic* AM::pRootDic(void)       { return gpAttachMap->m_pRootDic;               }
Word* AM::pRootWord(void)     { return gpAttachMap->m_pRootWord;              }
Statistics* AM::pStatistics(void)   { return & gpAttachMap->m_oStatistics;    }
StepTiming* AM::pStepTiming(void)   { return gpAttachMap->m_pStepTiming;      }
PoolBuffer* AM::pTakenBuffers(void) { return gpAttachMap->m_pTakenBuffers;    }
i8   AM::nTakenBytes(void)    { return gpAttachMap->m_nTakenBytes;            }
Lock* AM::pTccLock(void)      { return & gpAttachMap->m_okTccLock;            }
Word* AM::pTenants(void)      { return gpAttachMap->m_pTenants;               }
i8   AM::iThisNodeNo(void)    { return gpAttachMap->m_pGrid->iThisNode();     }
void AM::vThreadBegins(void)  { ++ gpAttachMap->m_ixThreads;                  }
i8   AM::nThreadsBegun(void)  { return * gpAttachMap->m_ixThreads;            }
bool AM::bTracepoint(void)    { return gpAttachMap->Arg.m_bTracepoint;        }
TxSet* AM::pTxSet(void)       { return gpAttachMap->m_pTxSet;                 }

void AM::vXmitNode(Op* pOp, i8 i_XmitNde) {
  pOp->vXmitNode(i_XmitNde);
}
bool AM::bVerbose(void)       { return gpAttachMap->Arg.m_bVerbose;           }
bool AM::bVerboseProgress(void)  {
  if (gxArgs.m_bVerbose || gxArgs.m_bProgress)
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
Queue* AM::pBufferQueue(Job* pJob) {
  
  Job* pJob_Originating;
  
  if (pJob->eJobKind() == JK_LOCAL)
    pJob_Originating = pJob;
  else
    pJob_Originating = pJob->m_pOriginatingTask->pJob();
  
  return pJob_Originating->m_xResult.m_pOriginBufferQueue;
}
void AM::vBootError(bool bBootError) {
  gpAttachMap->m_bBootError = bBootError;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
void AM::vDeleteOperand(Operand* pOperand)
{
  
  eCN eClass = pOperand->eClass();
  
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
      
    case CN_RSBBUFFER: {
      RsbBuffer* pRsbBuffer = _pRsbBuffer(pOperand);
      delete pRsbBuffer;
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
void AM::vRootConstant(Word* pRootConstant) {
  gpAttachMap->m_pRootConstant = pRootConstant;
}
void AM::vRootWord(Word* pRootWord) {
  gpAttachMap->m_pRootWord = pRootWord;
}
void AM::vGrabTaskLock(Task* pTask) {
  pTask->vGrabTaskLock();
}
void AM::vIndexEntries(i8 nIndexEntries) {
  gpAttachMap->Arg.m_nIndexEntries = nIndexEntries;
}
void AM::vMonitorPortrait(s48K& sPortrait) {
  Engine::vMonitorPortrait(sPortrait);
}
void AM::vNextTempName(s32& sName) {
  gpAttachMap->m_oTempName.vNextName(sName);
}
void AM::vSetAcksRmg(Task* pTask, i8 nAcks) {
  pTask->vAcksRmg(nAcks);
}
void AM::vStepTiming(StepTiming* pStepTiming) {
  gpAttachMap->m_pStepTiming = pStepTiming;
}
void AM::vSubInserters(i8 nSubInserters) {
  gpAttachMap->Arg.m_nSubInserters = nSubInserters;
}
void AM::vTakenBuffers(PoolBuffer* pTakenBuffers) {
  gpAttachMap->m_pTakenBuffers = pTakenBuffers;
}
void AM::vTakeBytes(i8 nTakeBytes) {
  gpAttachMap->m_nTakenBytes = nTakeBytes;
}
void AM::vTakenBytes(i8 nTakenBytes) {
  gpAttachMap->m_nTakenBytes += nTakenBytes;
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

TagSet* AM::pJobTags(Job* pJob)
{
  return pJob->pJobTags();
}


void AM::vDumpApi(void) {
  pSessionArray()->vDump();
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
  gxArgs.vLog();
}

i8 AM::nLvlLevels(void) {
  return gpAttachMap->Arg.m_nLvlLevels;
}
i8 AM::iLvlRatio(void) {
  return gpAttachMap->Arg.m_iLvlRatio;
}

bool AM::bParseCmd(TokenVec* pTknVec, TagSet* pExtraTagSet)
{
  return CmdHelper::bParseCmd(pTknVec,pExtraTagSet);
}


Job* AM::pPipelineJob(void)
{
  
  Job* pOpJob = AM::pTaskJob();
  
  Job* pPipelineJob = pOpJob->m_pOp->pPipeline()->pJob();
  return pPipelineJob;
}



void AM::vPoolAnalyze(void)   { return PoolHelper::vPoolAnalyze(); }

eEC AM::ePopulateJob(Job* pJob, TagSet* pTagSet)
{
  eEC eError = pJob->ePopulateJob(pTagSet);

  return eError;
}

void AM::vPrintArguments(void) {
  gxArgs.vPrint();
}
void AM::vPrintAttachMap(void) {
  gpAttachMap->vPrintAttachMap();
}
void AM::vPrintStartupInfo(void) {
  gxArgs.vPrintStartupInfo();
}
i8 AM::nRndSkipLevel(void) {
  
  i8 nLvlLevels = AM::nLvlLevels();
  i8 iLvlRatio  = AM::iLvlRatio();

  i8 nLevels;
  if (AM::bDeveloper()) {
    Task* pTask = Task::pThisTLSTask();
    nLevels = RndLib::iSkipLevel(nLvlLevels,iLvlRatio,pTask->i_NextRnd());
  }
  else {
    nLevels = RndLib::iSkipLevel(nLvlLevels,iLvlRatio);
  }
  RASSERT(nLevels >= 1 && nLevels <= nLvlLevels);
  return nLevels;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


Table* AM::pTableFind(Stz* szTableNm) {
  
  DicStack* pDicStack = pTaskDicStack();
  
  Table* pTable = Table::pFindTable(pDicStack,szTableNm);
  
  return pTable;
}

i8 AM::iTaskAcksRmg_assert_only_(Task* pTask) { return pTask->iAcksRmg_assert_only_(); }
void AM::vTaskBeginOp(Task* pTask)     { pTask->vBeginOp();     }
void AM::vTaskDump(void) {
  if (AM::pThisTLSTask())
    AM::pThisTLSTask()->vDump();
}

LogicUnit* AM::pTaskLogicUnit(Task* pTask)   { return pTask->pLogicUnit(); }

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

//Tasks::Task* AM::pThisTLSTask(void) {
//  Thread* pThread = Thread::pThisTLSThread();
//  if (! pThread)
//    return nullptr;
//  Task* pTask = pThread->pTask();
//  return pTask;
//}
LogicUnit* AM::pThisTLSLogicUnit(void) {
  Task* pTask = Task::pThisTLSTask();
  LogicUnit* pLogicUnit = pTask->pLogicUnit();

  return pLogicUnit;
}

Tasks::Task* AM::pThisTLSTask(void) {
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
  
  Tx* pTx = *pTask->pLogicUnit()->m_xpTx;
  return pTx;
}

PsxThd* AM::pThisTLSPsxThd(void) {
  Thread* pThread = Thread::pThisTLSThread();
  PsxThd* pPsxThd =pThread->m_pPsxThd;
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
  pTask->vSendNode(pSendNode);
  
  // grab socket wite lock
  Socket* pSocket = pSendNode->pSendSocket();
  pSocket->vGrabWrite();
  
  SysCall oSysCall;
  i8 nObjectSize = gxaSizeHeap[pXmitOperand->eClass()].m_nClassSize;
  bool bResult = pSocket->bSendTo(pXmitOperand, nObjectSize, oSysCall);
  
  Operand* pOperand = pXmitOperand->m_pOperand;
  
  nObjectSize = gxaSizeHeap[pOperand->eClass()].m_nClassSize;
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
  
  Session* pSession = AM::pSessionArray()->pGetSession(aReqTuple->m_i_Session);

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
/* state diagram                                                              */
/*----------------------------------------------------------------------------*/
void AM::vStateDiagram(s256& sDiagram, eCN eJobClass)
{
  TaskList* pExecuting     = AM::pQueues()->m_pExecutingSet;
  JobList*  pOriginating   = AM::pOriginatingJobs();
  JobList*  pParticipating = AM::pParticipatingJobs();
  
  sDiagram = ""_Stz;
  
  switch (eJobClass) {
/*
      // generate state diagram for export job
    case CN_JOB_EXPORT: {
      
      i8 iDiagram = AM::iNextDiagram();
      
      // whats in the system relevant to export?
      bool bKE_received   = pExecuting->bHasJobClass(CN_JOB_KIDEXPORT,false);
      bool bE_neutral     = pExecuting->bHasJobClass(CN_JOB_EXPORT,false);
      bool bE_originator  = pOriginating->bHasJobClass(CN_JOB_EXPORT);
      bool bE_participant = pParticipating->bHasJobClass(CN_JOB_EXPORT);
      bool bE_received    = pExecuting->bHasJobClass(CN_JOB_EXPORT,true);
      bool bSE_neutral    = pExecuting->bHasJobClass(CN_JOB_SUBEXPORTER,false);
      
      // form the state diagram
      sDiagram.vSprint("%03lld# "_Stz,iDiagram);
      
      if (bKE_received)         { sDiagram.vConcat("KE`"_Stz);  }
      if (bE_originator)        { sDiagram.vConcat("[E]"_Stz);  }
      if (! bE_originator &&
          bE_neutral)           { sDiagram.vConcat("E"_Stz);    }
      if (bE_participant)       { sDiagram.vConcat("(E`)"_Stz); }
      if (! bE_participant &&
          bE_received)          { sDiagram.vConcat("E`"_Stz);   }
      
      if (bSE_neutral)          { sDiagram.vConcat("SE"_Stz);   }
      
      sDiagram.vConcat(":"_Stz);
      
      if (bE_participant)       { sDiagram.vConcat("(E`)"_Stz); }
      if (! bE_participant &&
          bE_received)          { sDiagram.vConcat("E`"_Stz);   }
      if (bSE_neutral)          { sDiagram.vConcat("SE"_Stz);   }
      
      return;
    }
*/
    default:
      sDiagram.vSprint("Warning: No diagram for class %lld %s"_Stz,
                       eJobClass,aCN[eJobClass]);
  }
}

/*----------------------------------------------------------------------------*/
/* quick config summary                                                       */
/*----------------------------------------------------------------------------*/
Stz* AM::szSummary(s256& sSummary)
{
  Now spNow;
  
  sSummary.vSprint("%lldN%lldP%lldT %s %s"_Stz,
                   AM::nDefinedNodes(),
                   AM::nTablePtns(),
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
                   AM::nDefinedNodes(),
                   AM::nTablePtns(),
                   AM::nThreads()
                   );
  
  return sSummary.szStz();
}

/*----------------------------------------------------------------------------*/
/* write to trace file etc                                                    */
/*----------------------------------------------------------------------------*/
void AM::vWriteTrace(eYA eSysAct, Stz* aFormat, ...)
{
  va_list xVaArgs;
  
  va_start(xVaArgs,aFormat);
  AM::vWriteTrace_vargs(YA_SYS_TRACE,aFormat,xVaArgs);
  va_end(xVaArgs);

}

/*----------------------------------------------------------------------------*/
/* write to trace file etc                                                    */
/*----------------------------------------------------------------------------*/
void AM::vWriteTrace_vargs(eYA eSysAct, Stz* aFormat, va_list xArgs)
{
  static Lock oLock(LK_TRACE,nullptr);
  static char caFormatted[C_1MB] = {'\0'};
  
  bool bUseLock = (gpAttachMap ? true : false);
  bool bLowMem  = gpBufferPool ? gpBufferPool->bLowMemory() : false;
  
  if (bUseLock)
    oLock.vGrabWrite();
  
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
    oLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* write to trace file etc                                                    */
/*----------------------------------------------------------------------------*/
void AM::vWriteTrace_vargs(File* pFile, eYA eSysAct, Stz* aFormat, va_list xArgs)
{
  static Lock oLock(LK_TRACE,nullptr);
  static char caFormatted[C_1MB] = {'\0'};
  
  bool bUseLock = (gpAttachMap ? true : false);
  bool bLowMem  = gpBufferPool ? gpBufferPool->bLowMemory() : false;
  
  if (bUseLock)
    oLock.vGrabWrite();
  
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
    oLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a info log entry                                                       */
/*----------------------------------------------------------------------------*/
void AM::vLogAll(void)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  gpLog->vMarkAll(true);
#endif
}

/*----------------------------------------------------------------------------*/
/* show how a request was run                                                 */
/*----------------------------------------------------------------------------*/
void AM::vLogHow(i8 iRequestId, i8 iDetail)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  gpLog->vHow(iRequestId,iDetail);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a info log entry                                                       */
/*----------------------------------------------------------------------------*/
void AM::vLogFyi(Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  va_list xArgs;
  
  va_start(xArgs,szFormat);
  
  i8 i_Entry = gpLog->iEntryFyi(CN_NONE,szFormat,xArgs);
  
  va_end(xArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a ddl log entry                                                        */
/*----------------------------------------------------------------------------*/
void AM::vLogDdl(eSL eSeverity, Job* pJob, eDA eDdlAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (! gpLog->bIsLoggable(pJob))
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 i_Entry = gpLog->iEntryDdl(eSeverity,pJob,eDdlAct,
                                szFormat,xVaArgs);
  
  va_end(xVaArgs);
  
  pJob->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a job log entry                                                        */
/*----------------------------------------------------------------------------*/
void AM::vLogJob(eSL eSeverity, Job* pJob, eJA eJobAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (! gpLog->bIsLoggable(pJob))
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 i_Entry = gpLog->iEntryJob(eSeverity,pJob,eJobAct,
                                szFormat,xVaArgs);
  
  va_end(xVaArgs);
  
  pJob->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a lock log entry                                                       */
/*----------------------------------------------------------------------------*/
void AM::vLogLck(eSL eSeverity, Lock* pLock, eLA eLckAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (! gpLog->bIsLoggable(pLock))
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 i_Entry = gpLog->iEntryLck(eSeverity,pLock,eLckAct,
                                szFormat,xVaArgs);
  
  va_end(xVaArgs);
  
  pLock->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a log log entry                                                        */
/*----------------------------------------------------------------------------*/
void AM::vLogLog(eSL eSeverity, Log* pLog, eGA eLogAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (! gpLog->bIsLoggable(pLog))
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 i_Entry = gpLog->iEntryLog(eSeverity,pLog,eLogAct,
                                szFormat,xVaArgs);
  
  va_end(xVaArgs);
  
  pLog->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a management log entry                                                 */
/*----------------------------------------------------------------------------*/
void AM::vLogMgt(eSL eSeverity, Job* pJob, eMA eMgtAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (! gpLog->bIsLoggable(pJob))
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 i_Entry = gpLog->iEntryMgt(eSeverity,pJob,eMgtAct,
                                szFormat,xVaArgs);
  
  va_end(xVaArgs);
  
  pJob->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a operator log entry                                                   */
/*----------------------------------------------------------------------------*/
void AM::vLogOpr(eSL eSeverity, Op* pOpr, eOA eOprAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (! gpLog->bIsLoggable(pOpr))
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 i_Entry = gpLog->iEntryOpr(eSeverity,pOpr,eOprAct,
                                szFormat,xVaArgs);
  
  va_end(xVaArgs);
  
  pOpr->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a prompt log entry                                                     */
/*----------------------------------------------------------------------------*/
void AM::vLogPrm(eSL eSeverity, SePrompt* pPrm, ePA ePrmAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (! gpLog->bIsLoggable(pPrm))
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 i_Entry = gpLog->iEntryPrm(eSeverity,pPrm,ePrmAct,
                                szFormat,xVaArgs);
  
  va_end(xVaArgs);
  
  pPrm->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a operator log entry                                                   */
/*----------------------------------------------------------------------------*/
void AM::vLogQue(eSL eSeverity, Queues* pQue, eQA eQueAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;

  if (! gpLog->bIsLoggable(pQue))
    return;
  
  va_list xVaArgs;

  va_start(xVaArgs,szFormat);

  i8 i_Entry = gpLog->iEntryQue(eSeverity,pQue,eQueAct,
                                szFormat,xVaArgs);
  
  va_end(xVaArgs);

  pQue->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a resource log entry                                                   */
/*----------------------------------------------------------------------------*/
void AM::vLogRes(eSL eSeverity, Resource* pRes, eRA eResAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
    
  if (! gpLog->bIsLoggable(pRes))
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 i_Entry = gpLog->iEntryRes(eSeverity,pRes,eResAct,
                                szFormat,xVaArgs);
  
  va_end(xVaArgs);
  
  pRes->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a session log entry                                                    */
/*----------------------------------------------------------------------------*/
void AM::vLogSsn(eSL eSeverity, Session* pSsn, eAA eApiAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (! gpLog->bIsLoggable(pSsn))
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 i_Entry = gpLog->iEntrySsn(eSeverity,pSsn,eApiAct,
                                szFormat,xVaArgs);
  
  va_end(xVaArgs);
  
  pSsn->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a service log entry                                                    */
/*----------------------------------------------------------------------------*/
void AM::vLogSvc(eSL eSeverity, Job* pJob, eVA eSvcAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (! gpLog->bIsLoggable(pJob))
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 i_Entry = gpLog->iEntrySvc(eSeverity,pJob,eSvcAct,
                                szFormat,xVaArgs);
  
  va_end(xVaArgs);
  
  pJob->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a system log entry                                                     */
/*----------------------------------------------------------------------------*/
void AM::vLogSys(eSL eSeverity, AttachMap* , eYA eSysAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (! gpLog->bIsLoggable(gpAttachMap))
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 i_Entry = gpLog->iEntrySys(eSeverity,gpAttachMap,eSysAct,
                                szFormat,xVaArgs);
  
  va_end(xVaArgs);
  
  gpAttachMap->m_i_Log = i_Entry;

#endif
}

/*----------------------------------------------------------------------------*/
/* add a thread log entry                                                     */
/*----------------------------------------------------------------------------*/
void AM::vLogThd(eSL eSeverity, Thread* pThread, eHA eThdAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (! gpLog->bIsLoggable(pThread))
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 i_Entry = gpLog->iEntryThd(eSeverity,pThread,eThdAct,
                                szFormat,xVaArgs);

  va_end(xVaArgs);
  
  pThread->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a rask log entry                                                       */
/*----------------------------------------------------------------------------*/
void AM::vLogTsk(eSL eSeverity, Task* pTask, eTA eTskAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (! gpLog->bIsLoggable(pTask))
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
    
  i8 i_Entry = gpLog->iEntryTsk(eSeverity,pTask,eTskAct,
                                szFormat,xVaArgs);
  
  va_end(xVaArgs);
  
  pTask->m_i_Log = i_Entry;
#endif
}

/*----------------------------------------------------------------------------*/
/* add a transaction log entry                                                */
/*----------------------------------------------------------------------------*/
void AM::vLogTxn(eSL eSeverity, Tx* pTxn, eXA eTxnAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (! gpLog->bIsLoggable(pTxn))
    return;

  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 i_Entry = gpLog->iEntryTxn(eSeverity,pTxn,eTxnAct,
                                szFormat,xVaArgs);

  va_end(xVaArgs);
  
  pTxn->m_i_Log = i_Entry;
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
  if (pOperand->eClass() == CN_MARKER) {
    Marker* pMarker = _pMarker(pOperand);
    Marker* pNewMarker = new Marker(pMarker->eOpMarker());
    return pNewMarker;
  }
  else {
    ASSERT(pOperand->eClass() == CN_STREAMDEF);
    StreamDef* pStreamDef = _pStreamDef(pOperand);
    StreamDef* pNewStreamDef = new StreamDef(pStreamDef);
    return pNewStreamDef;
  }
  
}

/*----------------------------------------------------------------------------*/
/* new an operand job                                                         */
/*----------------------------------------------------------------------------*/
XmitOperand* AM::pNewXmitOperand(Job* pParentJob, Operand* pOperand, Op* pOp,
                                 i8 i_Node, i8 tPtnNo)
{
  XmitOperand* pXmitOperand =
    XmitOperand::pNewXmitOperand(pOperand,pParentJob->m_iFocalId,pOp,i_Node,tPtnNo);
  
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
    return aCN[pOperand->eClass()];
}

/*----------------------------------------------------------------------------*/
/* set the number of operands in the system                                   */
/*----------------------------------------------------------------------------*/
i8 AM::nOperandsArg(void)
{
  return gpAttachMap->Arg.m_nOperands;
}

/*----------------------------------------------------------------------------*/
/* set the number of operands in the system                                   */
/*----------------------------------------------------------------------------*/
void AM::vOperandsArg(i8 nOperands)
{
  gpAttachMap->Arg.m_nOperands = nOperands;
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
/* passes the low-memory condition?                                           */
/*----------------------------------------------------------------------------*/
bool AM::bPassesLowMem(void)
{
  _static_

  //! ALKA: while cleaning up JobClass all jobs had m_eFlags
  //! as JF_LOWMEM hence returning true
  return true;
  
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
    
    switch (gxaSizeHeap[eClass].m_eWhichHeap) {
        
      case HT_INVALID:
        continue;
        
      case HT_PERSISTENT_TINY:
      case HT_TRANSIENT_TINY:
        assert(iClassSize < C_SIZE_TINY_OBJECT);
        continue;
        
      case HT_PERSISTENT_SMALL:
      case HT_TRANSIENT_SMALL:
        assert(iClassSize < C_SIZE_SMALL_OBJECT);
        continue;
        
      case HT_PERSISTENT_MEDIUM:
      case HT_TRANSIENT_MEDIUM:
        assert(iClassSize < C_SIZE_MEDIUM_OBJECT);
        continue;
        
      case HT_PERSISTENT_LARGE:
      case HT_TRANSIENT_LARGE:
        assert(iClassSize < C_SIZE_LARGE_OBJECT);
        continue;
                
      default:
        assert(0);
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Logging::vCheckAssumptions();
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
void AM::vLogText(i8 iId, Stz* szText) {;}
bool AM::bLogEntryExists(i8 i_Entry) {  return Log::bEntryExists(i_Entry); }
i8   AM::iLogEntryPrev(i8 i_Entry)   {  return Log::iEntryPrev(i_Entry); }
void AM::vLogEntrySprint(s4K& sString, i8 i_Entry) {
  Log::vEntrySprint(sString,i_Entry);
}

void AM::vFocalAdd(Focal* pFocal)      {
  if (! gpAttachMap || !gpAttachMap->m_pFocalObjects)
    return;
  gpAttachMap->m_pFocalObjects->vAdd(pFocal);
}
void AM::vFocalRemove(Focal* pFocal)   {
  if (C_UNKNOWN == pFocal->m_i_Focal)
    return;
  gpAttachMap->m_pFocalObjects->vRemove(pFocal);
}
void AM::vFilePrint(File* pFile, Stz* szFormat,...) {
  
  va_list xArgs;
  va_start(xArgs,szFormat);
  pFile->vPrint(xArgs, szFormat);
  va_end(xArgs);
  
}

void AM::vMri(TokenVec *pTknVec) { gpAttachMap->m_pFocalObjects->vMri(pTknVec); }
void AM::vLog(TokenVec *pTknVec) { gpLog->vLog(pTknVec);                        }


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* focal outline                                                              */
/*----------------------------------------------------------------------------*/
void AM::vFocalOutline(s4K& sOutline, Focal* pFocal) {

  eCN eClassNo = pFocal->eClass();
  
  if (CN_ATTACHMAP == eClassNo)
    _pAttachMap(pFocal)->vFocalOutline(sOutline);
  
  else if (CN_LOG == eClassNo)
    _pLog(pFocal)->vFocalOutline(sOutline);
  
  else if (CN_SEPROMPT == eClassNo)
    _pSePrompt(pFocal)->vFocalOutline(sOutline);
  
  else if (CN_QUEUES == eClassNo)
    _pQueues(pFocal)->vFocalOutline(sOutline);
  
  else if (CN_TASK == eClassNo)
    _pTask(pFocal)->vFocalOutline(sOutline);
  
  else if (CN_THREAD == eClassNo)
    _pThread(pFocal)->vFocalOutline(sOutline);
  
  else if (CN_TX == eClassNo)
    _pTx(pFocal)->vFocalOutline(sOutline);
  
  else if (CN_SESSION == eClassNo)
    _pSession(pFocal)->vFocalOutline(sOutline);
  
  else if (CN_DIR == eClassNo)
    _pDir(pFocal)->vFocalOutline(sOutline);
  
  else if (CN_FILE == eClassNo)
    _pFile(pFocal)->vFocalOutline(sOutline);
  
  else if (CN_FIFO == eClassNo)
    _pFifo(pFocal)->vFocalOutline(sOutline);
  
  else if (CN_MUTEX == eClassNo)
    _pMutex(pFocal)->vFocalOutline(sOutline);
  
  else if (CN_PSXTHD == eClassNo)
    _pPsxThd(pFocal)->vFocalOutline(sOutline);
  
  else if (CN_SOCKET == eClassNo)
    _pSocket(pFocal)->vFocalOutline(sOutline);
  
  else if (CN_LOCK == eClassNo)
    _pLock(pFocal)->vFocalOutline(sOutline);
    
  else if (Jobs::_bIsValidJob(eClassNo))
    _pJob(pFocal)->vFocalOutline(sOutline);
  
  else
    sOutline.vSprint("<%s focal>"_Stz,aCN[eClassNo]);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

i8 AM::iRdTimePoint(void) { return TickingDevice::iGetTick();                  }
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

void AM::vIncRowsInvalid(Job* pJob)   { pJob->vIncRowsInvalid();               }
void AM::vIncRowsProcessed(Job* pJob) { pJob->vIncRowsProcessed();             }
bool AM::bInBufferPool(Object* pObject) {
  return gpBufferPool->bInBufferPool(pObject);
}

Stz* AM::szJobName(Job* pJob)    { return pJob->szJobNm();                      }
i8   AM::iJobUniqueId(Job* pJob) { return pJob->m_iFocalId;                     }

void AM::vPrintGlobals(void)
{
  printf("gxArgs     @%p\n",&gxArgs);
  printf("gxPromptCatalog @%p\n",&gxPromptCatalog);
  printf("\n");

  printf("gpAttachMap     @%p : %p\n",&gpAttachMap,  gpAttachMap);
  printf("gpBufferPool    @%p : %p\n",&gpBufferPool, gpBufferPool);
  printf("gpLog           @%p : %p\n",&gpLog,        gpLog);
  printf("gaSeShmRegion   @%p : %p\n",&gaSeShmRegion,gaSeShmRegion);
  printf("gpTraceFile     @%p : %p\n",&gpTraceFile,  gpTraceFile);
}
void AM::vPromptCall(void)
{
  SePrompt* pSePrompt = new SePrompt(gxArgs.m_bCommand, gxArgs.m_bSethScript);
  
  pSePrompt->vPromptLoop();
  
  delete pSePrompt;
}

Index* AM::pReadApiIndex(void) { return gpReadApiIndex; }
void   AM::vReadApiIndex(Index* pIndex) { gpReadApiIndex = pIndex; }

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

void AM::vJobContentDump(Job* pJob) { pJob->vContentDump(); }

Task* AM::pTaskNew(Job* pJob) {
  return new Task(pJob);
}
bool AM::bTaskIsLocked(Task* pTask) {
  bool bLocked = pTask->bIsLocked();
  return bLocked;
}

DicStack* AM::pTaskDicStack(void) {
  return pThisTLSTask()->pForth()->pDicStack();
}
Job* AM::pTaskJob(void) {
  return pThisTLSTask()->pJob();
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

  Cell* aDoes = pWord->aEntryCell();
  
  Word* pDoesWord = Routine::pToWord(aDoes);
  
  vTrace("%s\n"_Stz,spWord.szSeeDoes(pDoesWord));

  AM::vWordSeeDoes(pWord,pDoesWord,aDoes, pDicStack);
  
  vTrace("=============================================================\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* dump routine of word                                                       */
/*----------------------------------------------------------------------------*/
void AM::vWordSeeDoes(Word* pWord, Word* pDoesWord, Cell* aDoes,
                      DicStack* pDicStack)
{
  vTrace("- - - - - - - - - - - - - - - - - - - - -\n"_Stz);

  Cell* aLastCell = pDoesWord->aLastCell();
  for (Cell* aCell = aDoes; aCell < aLastCell; ) {
    
    StepFp fpStep = *aCell->m_afpStep;
    Word* pStepWord = pDicStack->pLocateStepWord(fpStep);
    Stz* szStepNm = pStepWord->szWordNm();
    
    // -- -- -- -- -- -- -- -- -- --
    // step is: jump / jump false / jump true
    
    if (fpStep == &Step::vFlowJump ||
        fpStep == &Step::vFlowJumpFalse ||
        fpStep == &Step::vFlowJumpTrue ) {
      
      vTrace("%p: %s"_Stz,aCell,szStepNm);
      aCell++;
      
      vTrace(" %p\n"_Stz,aCell->m_aJumpTo);
      aCell++;
      
      continue;
    }
    
    // -- -- -- -- -- -- -- -- -- --
    // step is: push intval
    
    if (fpStep == &Step::vPushIntVal) {
      
      vTrace("%p: %s"_Stz,aCell,szStepNm);
      aCell++;
      
      IntVal* aIntVal = _aIntVal(aCell);
      vTrace(" %lld\n"_Stz,aIntVal->iInt());
      
      i8 iBytes = aIntVal->nStackUsage();
      aCell = aAdd<Cell*>(aCell,iBytes);
      
      continue;
    }
    
    // -- -- -- -- -- -- -- -- -- --
    // step is: push ptrval
    
    if (fpStep == &Step::vPushPtrVal) {
      
      vTrace("%p: %s"_Stz,aCell,szStepNm);
      aCell++;
      
      PtrVal* aPtrVal = _aPtrVal(aCell);
      eCN eClassNo_obj = aPtrVal->pObj()->eClass();
      
      if (CN_WORD == eClassNo_obj) {
        Word* pWord = _pWord(aPtrVal->pObj());
        vTrace(" %p %s\n"_Stz,pWord,pWord->szWordNm());
      }
      else {
        vTrace(" %p %s\n"_Stz,aCN[eClassNo_obj]);
      }
      
      i8 iBytes = aPtrVal->nStackUsage();
      aCell = aAdd<Cell*>(aCell,iBytes);
      
      continue;
    }
    
    // -- -- -- -- -- -- -- -- -- --
    // step is: push stzval
    
    if (fpStep == &Step::vPushStzVal) {
      
      vTrace("%p: %s"_Stz,aCell,szStepNm);
      aCell++;
      
      StzVal* aStzVal = _aStzVal(aCell);
      vTrace(" \"%s\"\n"_Stz,aStzVal->szStz());
      
      i8 iBytes = aStzVal->nStackUsage();
      aCell = aAdd<Cell*>(aCell,iBytes);
      
      continue;
    }
    
    // -- -- -- -- -- -- -- -- -- --
    // step is: enter routine
    
    if (fpStep == &Step::vEnterRoutine) {
      
      Cell* aEnterCell = *(Cell**)(aCell->m_afpStep + 1);
      Word* pEnterWord = Routine::pToWord(aEnterCell);

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
