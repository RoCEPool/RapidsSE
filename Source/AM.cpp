/*============================================================================*/
/* AM.cpp -- AttachMap helper                                                 */
/*============================================================================*/

#include "Oblique.h"

extern Arguments     gxArguments;
extern OpCatalog     gxOpCatalog;
extern PromptCatalog gxPromptCatalog;

extern AttachMap*    gpAttachMap;
extern BufferPool*   gpBufferPool;
extern Log*          gpLog;
extern Header*       gaSeShmRegion;
extern File*         gpTraceFile;

extern eHT           geWhichHeap[];
extern i8            giClassSize[];
extern pFINALIZE*    gpFinalize[];

// Arg
Stz* AM::szCommand(void)      { return gxArguments.m_sCommand.szStz();        }
bool AM::bDeveloper(void)     { return gpAttachMap->Arg.m_bDeveloper;         }
i8   AM::nDummyLoop(void)     { return gpAttachMap->Arg.m_nDummyLoop;         }
i8   AM::nDummySleep(void)    { return gpAttachMap->Arg.m_nDummySleep;        }
i8   AM::nDummyThreads(void)  { return gpAttachMap->Arg.m_nDummyThreads;      }
i8   AM::iJumboSize(void)     { return gpAttachMap->Arg.m_iJumboSize;         }
bool AM::bLog(void)           { return gpAttachMap->Arg.m_bLog;               }
i8   AM::nMaxLoadErrors(void) { return gpAttachMap->Arg.m_nMaxLoadErrors;     }
i8   AM::nMaxReadyLP(void)    { return gpAttachMap->Arg.m_nMaxReadyLP;        }
bool AM::bMunged(void)        { return gpAttachMap->Arg.m_bMunged;            }
Stz* AM::szRegionName(void)   { return gpAttachMap->Arg.m_sRegionName.szStz();}
i8   AM::iRegionSize(void)    { return gpAttachMap->Arg.m_iRegionSize;        }
i8   AM::nSlotsPerNode(void)  { return gpAttachMap->Arg.m_nSlotsPerNode;      }
i8   AM::nSubIngesters(void)  { return gpAttachMap->Arg.m_nSubIngesters;      }
i8   AM::nSubInserters(void)  { return gpAttachMap->Arg.m_nSubInserters;      }
i8   AM::nSubLoaders(void)    { return gpAttachMap->Arg.m_nSubLoaders;        }
i8   AM::nThreads(void)       { return gpAttachMap->Arg.m_nThreads;           }

//
SessionArray* AM::pSessionArray(void) { return gpAttachMap->m_pSessionArray;  }
bool AM::bBootError(void)     { return gpAttachMap->m_bBootError;             }
BufferPool* AM::pBufferPool(void) { return gpAttachMap->m_pBufferPool;        }
CarryOver* AM::pCarryOver(void)   { return gpAttachMap->m_pCarryOver;         }
EnumFlags* AM::pClassFlags(void)  { return & gpAttachMap->m_oClassFlags;      }
Lock* AM::pDdlLock(void)      { return & gpAttachMap->m_okDdlLock;            }
Lock* AM::pDicLock(void)      { return & gpAttachMap->m_okDicLock;            }
i8   AM::iElapsedSsec(void)   { return gpAttachMap->m_oBirth.iElapsedSsec();  }
i8   AM::iEpoch(void)         { return gpAttachMap->m_iEpoch;                 }
Formatters* AM::pFormatters(void) { return & gpAttachMap->m_oFormatters;      }
Grid* AM::pGrid(void)         { return gpAttachMap->m_pGrid;                  }
bool AM::bHasStarted(void)    { return gpAttachMap->m_pGrid->m_bHasStarted;   }
HeapMem* AM::pHeapMem(void)   { return & gpAttachMap->m_xHeapMem;             }
i8   AM::nIndexEntries(void)  { return gpAttachMap->m_nIndexEntries;          }
LockTracker* AM::pLockTracker(void) { return & gpAttachMap->m_oLockTracker;   }
bool AM::bLogging(void)       { return gpLog ? true : false;                  }
i8   AM::iNextEpoch(void)     { return ++ gpAttachMap->m_iEpoch;              }
Stz* AM::szNodeNm(void)       {
  return gpAttachMap->m_pGrid->m_pThisNode->szNodeNm();  }
JobList* AM::pOriginatingJobs(void) {
  return gpAttachMap->m_pQueues->m_pOriginatingJobs; }
JobList* AM::pParticipatingJobs(void) {
  return gpAttachMap->m_pQueues->m_pParticipatingJobs; }
i8   AM::iPid(void)           { return gpAttachMap->m_iPid;                   }
i8   AM::nConnectedNodes(void){ return gpAttachMap->m_pGrid->nConnectedNodes(); }
i8   AM::nDefinedNodes(void)  { return gpAttachMap->m_pGrid->nDefinedNodes(); }
Article* AM::pLexiconArticle(void) { return gpAttachMap->m_pLexiconArticle;    }
void AM::vProcessExit(void)   { Process::vExitFailure();                      }
bool AM::bProgress(void)      { return gpAttachMap->m_bProgress;              }
bool AM::bMetadata(void)      { return gpAttachMap->m_bMetadata;              }

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

i8*  AM::iaObjectsInPool(void){ return gpAttachMap->m_nObjectsInPool;         }
i8   AM::nPartitions(void)    { return gpAttachMap->m_pGrid->nPartitions();   }
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
Word* AM::pTenants(void)      { return gpAttachMap->m_pTenants;               }
i8   AM::iThisNodeNo(void)    { return gpAttachMap->m_pGrid->iThisNode();     }
void AM::vThreadBegins(void)  { ++ gpAttachMap->m_ixThreads;                  }
i8   AM::nThreadsBegun(void)  { return * gpAttachMap->m_ixThreads;            }
bool AM::bTracepoint(void)    { return gpAttachMap->m_bTracepoint;            }
TxSet* AM::pTxSet(void)       { return gpAttachMap->m_pTxSet;                 }

void AM::vXmitNode(Op* pOp, i8 i_XmitNde) {
  XmitOp* pXmitOp = _pXmitOp(pOp);
  pXmitOp->vXmitNode(i_XmitNde);
}
bool AM::bVerbose(void)       { return gpAttachMap->m_bVerbose;               }

//
void AM::vAckWaitingTask(Task* pTaskWaiting, Task* pTaskAcking) {
  IGNORE_RETURN(Scheduler::bAckWaitingTask(pTaskWaiting,pTaskAcking));
}
void AM::vAddConstant(Stz* szName,Stz* szValue) {
  Constants::vAddConstant(szName,szValue);
}
void AM::vAttachMap(Header* aHeader) {
  gpAttachMap = static_cast<AttachMap*>(aHeader);
}
Queue* AM::pBufferQueue(Job* pJob) {
  Job* pJob_Originating = pJob->m_pOriginatingTask->pJob();
  return pJob_Originating->m_pBufferQueue;
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
      
    case CN_CANCELMARKER: {
      CancelMarker* pCancelMarker = _pCancelMarker(pOperand);
      delete pCancelMarker;
      break;
    }
      
    case CN_CRWBUFFER: {
      CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
      delete pCrwBuffer;
      break;
    }
      
    case CN_ENDMARKER: {
      EndMarker* pEndMarker = _pEndMarker(pOperand);
      delete pEndMarker;
      break;
    }
 
    case CN_STARTMARKER: {
      StartMarker* pStartMarker = _pStartMarker(pOperand);
      delete pStartMarker;
      break;
    }
      
    case CN_STREAMDEF: {
      StreamDef* pStreamDef = _pStreamDef(pOperand);
      delete pStreamDef;
      break;
    }
      
    default: {
      RASSERT(0);
    }
  }
}
void AM::vDropTaskLock(Task* pTask) {
  Scheduler::vDropTaskLock(pTask);
}
void AM::vRootConstant(Word* pRootConstant) {
  gpAttachMap->m_pRootConstant = pRootConstant;
}
void AM::vRootWord(Word* pRootWord) {
  gpAttachMap->m_pRootWord = pRootWord;
}
void AM::vGrabTaskLock(Task* pTask) {
  Scheduler::vGrabTaskLock(pTask);
}
void AM::vIndexEntries(i8 nIndexEntries) {
  gpAttachMap->m_nIndexEntries = nIndexEntries;
}
void AM::vMonitorPortrait(s48K& sPortrait) {
  Engine::vMonitorPortrait(sPortrait);
}
void AM::vNextTempName(s32& sName) {
  gpAttachMap->m_oTempName.vNextName(sName);
}
void AM::vPulseTask(Task* pTask) {
  Scheduler::vGrabTaskLock(pTask);
  Scheduler::vPulsePausedTask(pTask);
}
void AM::vSetAcksRmg(Task* pTask, i8 nAcks) {
  pTask->vAcksRmg(nAcks);
}
void AM::vSendJobOperand(Task* pTask, Operand* pOperand,
                         Job* pParentJob, Op* pOp, i8 iNode) {
  
  // new a xmit job
  Job_Operand* pJob_Operand = Job_Operand::pNewJobOperand(
                                  nullptr, pTask,pOperand, pParentJob->m_iFocalId,pOp,iNode);
  
  // set the send node in the task
  Node* pSendNode = pGrid()->pGetNode(iNode);
  pTask->vSendNode(pSendNode);
  
  // grab socket wite lock
  Socket* pSocket = pSendNode->pSendSocket();
  pSocket->vGrabWrite();
  
  SysCall oSysCall;
  i8 nObjectSize = giClassSize[pJob_Operand->eClass()];
  bool bResult = pSocket->bSendTo(pJob_Operand, nObjectSize, oSysCall);
  
  nObjectSize = giClassSize[pOperand->eClass()];
  bResult = pSocket->bSendTo(pOperand, nObjectSize, oSysCall);
  
  // now delet ethe job_operand
  delete pJob_Operand;
  
  // drop the socket write lock
  pSocket->vDropWrite();
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
void AM::vTriggerBidingTask(Task* pTask) {
  Scheduler::vTriggerBidingTask(pTask);
}

//
bool AM::bAttached(void) {
  if (gpAttachMap)
    return true;
  else
    return false;
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
  gxArguments.vLog();
}
void AM::vPrintArguments(void) {
  gxArguments.vPrint();
}
void AM::vPrintAttachMap(void) {
  gpAttachMap->vPrintAttachMap();
}
void AM::vPrintStartupInfo(void) {
  gxArguments.vPrintStartupInfo();
}
i8 AM::nRandomLevels(void) {
  Task* pTask = Task::pThisTLSTask();
  i8 nLevels = Random::iLogLevel(3,C_FLAT_FREQ + 1,pTask->i_NextRnd()) + 1;
  return nLevels;
}

i8 AM::iTaskAcksRmg_assert_only_(Task* pTask) { return pTask->iAcksRmg_assert_only_(); }
void AM::vTaskBeginOp(Task* pTask)     { pTask->vBeginOp();     }
void AM::vTaskDump(void)               { if (AM::pThisTLSTask()) AM::pThisTLSTask()->vDump(); }
void AM::vTaskMutexLock(Task* pTask)   { pTask->vMutexLock();   }
void AM::vTaskMutexSignal(Task* pTask) { pTask->vMutexSignal(); }
void AM::vTaskMutexUnlock(Task* pTask) { pTask->vMutexUnlock(); }
void AM::vTaskMutexWait(Task* pTask)   { pTask->vMutexWait();   }
RegSet* AM::pTaskRegSet(Task* pTask)   { return pTask->pRegSet(); }

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
Op* AM::pThisTLSOp(void) {
  Thread* pThread = Thread::pThisTLSThread();
  Task* pTask = pThread->pTask();
  Op* pOp = pTask->pOp();
  return pOp;
}
Tasks::Task* AM::pThisTLSTask(void) {
  Thread* pThread = Thread::pThisTLSThread();
  if (! pThread)
    return nullptr;
  Task* pTask = pThread->pTask();
  return pTask;
}
Thread* AM::pThisTLSThread(void) {
  Thread* pThread = Thread::pThisTLSThread();
  return pThread;
}
PsxThd* AM::pThisTLSPsxThd(void) {
  Thread* pThread = Thread::pThisTLSThread();
  PsxThd* pPsxThd =pThread->m_pPsxThd;
  return pPsxThd;
}
i8 AM::iThreadId(Thread* pThread) {
  return pThread->m_iFocalId;
}
void AM::vWelcomeText(s256& sWelcome) {
  Engine::vGetWelcome(sWelcome);
}
void AM::vCopyrightText(s256& sCopyright) {
  Engine::vGetCopyright(sCopyright);
}
void AM::vWriteLog(void) {
  Log::vWriteCsv(false);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get pSession given i_Session                                               */
/*----------------------------------------------------------------------------*/
Session* AM::pSessionGet(ReqTuple* aReqTuple)
{
  if (aReqTuple->m_i_Origin != AM::iThisNodeNo())
    return nullptr;
  
  if (C_UNKNOWN == aReqTuple->m_i_Session)
    return nullptr;
  
  Session* pSession = AM::pSessionArray()->pGetSession(aReqTuple->m_i_Origin);

  return pSession;
}

/*----------------------------------------------------------------------------*/
/* state diagram                                                              */
/*----------------------------------------------------------------------------*/
bool AM::bSpawnJob(Job* pJob)
{
  bool bResult = Api::bSpawnJob(pJob);
  
  return bResult;
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
/* write to trace file etc                                                    */
/*----------------------------------------------------------------------------*/
void AM::vWriteTrace(eYA eSysAct, Stz* aFormat, va_list xArgs)
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
/* add a request log entry                                                    */
/*----------------------------------------------------------------------------*/
void AM::vLogReq(eSL eSeverity, Request* pReq, eCA eReqAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (! gpLog->bIsLoggable(pReq))
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 i_Entry = gpLog->iEntryReq(eSeverity,pReq,eReqAct,
                                szFormat,xVaArgs);
  
  va_end(xVaArgs);
  
  pReq->m_i_Log = i_Entry;
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

void AM::vDdlDropRead (Task* pTask)  { Scheduler::vDdlDropRead (pTask);        }
void AM::vDdlDropWrite(Task* pTask)  { Scheduler::vDdlDropWrite(pTask);        }
void AM::vDdlGrabRead (Task* pTask)  { Scheduler::vDdlGrabRead (pTask);        }
void AM::vDdlGrabWrite(Task* pTask)  { Scheduler::vDdlGrabWrite(pTask);        }

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* nap for given number of milliseconds (no known calling thread)             */
/*----------------------------------------------------------------------------*/
void AM::vNap(i8 nMilliSeconds, eCN eCallingClass)
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
/* new a universal job                                                        */
/*----------------------------------------------------------------------------*/
Job* AM::pNewJobUniversal( Op* pOp, ReqTuple* pReqTuple, i8 iUniqueId)
{
  JobClass* pJobClass_Universal = JobClass::pFind(CN_JOB_UNIVERSAL);

  Job_Universal* pJobUniversal = new Job_Universal(pOp->m_pJobPipeline, pJobClass_Universal,pReqTuple,iUniqueId);
  
  pJobUniversal->m_pOp = pOp;
  pJobUniversal->pEnameSet()->vAdd("<path>"_Stz,
                       pOp->m_pJobPipeline->pEnameSet()->szValue("<path>"_Stz));
  pJobUniversal->pEnameSet()->vAdd("<jobNm>"_Stz,pOp->m_sName);
  pJobUniversal->vSeed(SS_SEED_REQ);

  pJobUniversal->pEnameSet()->vPrint();
  
  return pJobUniversal;
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
  i8 iEntriesGPDUMP = 0;
  i8 iEntriesGCLASSSIZE = 0;
  i8 iEntriesGPFINALIZE = 0;
  
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
           "Enum for CN_ENUM not found in aCN\n"_Stz);
  
  if (iEntriesECN != iEntriesPCN) {
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "\n!!!ALERT!!! No of entries in aCN (%lld) "
           "do not match the no in eCN (%lld)\n\n"_Stz,
           iEntriesPCN, iEntriesECN);
  }
  
  bFoundEnum = false;
  // find number of entries in gpDump
  for (i8 i_Class = 0; i_Class <= CN_ENUMS; i_Class++) {
    if (gpDump[i_Class] == &Dump::vEnums) {
      bFoundEnum = true;
      break;
    }
    iEntriesGPDUMP++;
  }
  
  if (!bFoundEnum)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Enum for CN_ENUM not found in gpDump\n"_Stz);
  
  if (iEntriesECN != iEntriesGPDUMP) {
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "\n!!!ALERT!!! No of entries in gpDump (%lld) "
           "do not match the no in eCN (%lld)\n\n"_Stz,
           iEntriesECN, iEntriesGPDUMP);
  }
  
  bFoundEnum = false;
  // find number of entries in giClassSize
  for (i8 i_Class = 0; i_Class <= CN_ENUMS; i_Class++) {
    if (giClassSize[i_Class] == -1) {
      bFoundEnum = true;
      break;
    }
    iEntriesGCLASSSIZE++;
  }
  
  if (!bFoundEnum)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Enum for CN_ENUM not found in giClassSize\n"_Stz);
  
  if (iEntriesECN != iEntriesGCLASSSIZE) {
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "\n!!!ALERT!!! No of entries in giClassSize (%lld) "
           "do not match the no in eCN (%lld)\n\n"_Stz,
           iEntriesGCLASSSIZE, iEntriesECN);
  }
  
  bFoundEnum = false;
  // find number of entries in gpDump
  for (i8 i_Class = 0; i_Class <= CN_ENUMS; i_Class++) {
    if (gpFinalize[i_Class] == &Finalize::vEnums) {
      bFoundEnum = true;
      break;
    }
    iEntriesGPFINALIZE++;
  }
  
  if (!bFoundEnum)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Enum for CN_ENUM not found in gpDump\n"_Stz);
  
  if (iEntriesECN != iEntriesGPDUMP) {
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "\n!!!ALERT!!! No of entries in gpDump (%lld) "
           "do not match the no in eCN (%lld)\n\n"_Stz,
           iEntriesECN, iEntriesGPDUMP);
  }
  
  
  // check sizes are less than pool block size
  for (i8 iClass = 0; iClass <= CN_ENUMS; iClass++) {
    
    eCN eClass = (eCN)iClass;
    i8  iClassSize = giClassSize[eClass];
    
    switch (geWhichHeap[eClass]) {
        
      case HT_INVALID:
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
        
      case HT_PERSISTENT_JUMBO:
      case HT_TRANSIENT_JUMBO:
        assert(iClassSize < C_SIZE_JUMBO_OBJECT);
        continue;
        
      default:
        assert(0);
    }
  }
  
  Logging::vCheckAssumptions();
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
  
  else if (Operators::_bIsValidOp(eClassNo))
    _pOp(pFocal)->vFocalOutline(sOutline);
  
  else
    sOutline.vSprint("<%s focal>"_Stz,aCN[eClassNo]);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

i8 AM::iRdTimePoint(void) { return Tick::iRdClock();                           }
i8 AM::iAMTimePoint(void) { return gpAttachMap->m_iTimePoint;                  }

bool AM::bInBufferPool(Object* pObject) {
  return gpBufferPool->bInBufferPool(pObject);
}

i8 AM::iJobUniqueId(Job* pJob) { return pJob->m_iFocalId;                     }

void AM::vPrintGlobals(void)
{
  printf("gxArguments     @%p\n",&gxArguments);
  printf("gxOpCatalog     @%p\n",&gxOpCatalog);
  printf("gxPromptCatalog @%p\n",&gxPromptCatalog);
  printf("\n");

  printf("gpAttachMap     @%p : %p\n",&gpAttachMap,  gpAttachMap);
  printf("gpBufferPool    @%p : %p\n",&gpBufferPool, gpBufferPool);
  printf("gpLog           @%p : %p\n",&gpLog,        gpLog);
  printf("gaSeShmRegion   @%p : %p\n",&gaSeShmRegion,gaSeShmRegion);
  printf("gpTraceFile     @%p : %p\n",&gpTraceFile,  gpTraceFile);
}



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

void AM::vJobContentDump(Job* pJob) { pJob->vContentDump(); }

Task* AM::pTaskNew(Job* pJob) {
  return new Task(pJob);
}
void AM::vTaskSchedulerAct(Tasks::Task* pTask, i8 iTransition) {
  pTask->vSchedulerAct((eSA)iTransition);
}
void AM::vTaskDelete(Task* pTask) {
  delete pTask;
}
bool AM::bTaskIsLocked(Task* pTask) {
  bool bLocked = pTask->bIsLocked();
  return bLocked;
}

bool AM::bJobHasError(Job* pJob) {
  return pJob->m_bHasError;
}

void AM::vSetJobLocalError(Job* pFromJob, Job* pToJob) {
  pToJob->m_bHasError = pFromJob->m_bHasError;
  pToJob->m_xResponse.m_eErrorCode = pFromJob->m_xResponse.m_eErrorCode;
}

void AM::vJobObjectName(Job* pJob, Stz* szObjectName) {
  pJob->pEnameSet()->vAdd("<tableNm>"_Stz,szObjectName);
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
/* dump routine of word                                                       */
/*----------------------------------------------------------------------------*/
void AM::vWordSee(Word* pWord, DicStack* pDicStack)
{
  s48K sString;
  
  SP_Word spWord;
  vTrace("Word: %s\n"_Stz,spWord.sz(pWord));

  Cell* aDoesCell = pWord->aEntryCell();
  
  Word* pDoesWord = (Word*)( ((u8)aDoesCell & (u8)~0xFFFF)
                            + sizeof(HeapBuffer));
  
  while (aDoesCell < pDoesWord->aLastCell()) {
    
    StepFp fpHasStep = *aDoesCell->m_afpIntrinsic;
    
    Word* pCallsWord = pDicStack->pLocateWord(fpHasStep);
    
    // jumps
    if (*aDoesCell->m_afpIntrinsic == &Step::vFlowJump ||
        *aDoesCell->m_afpIntrinsic == &Step::vFlowJumpFalse ||
        *aDoesCell->m_afpIntrinsic == &Step::vFlowJumpTrue ) {
      
      vTrace("%p: %s"_Stz,aDoesCell,pCallsWord->szWordNm());
      
      aDoesCell++;
      
      vTrace(" %p\n"_Stz,aDoesCell->m_aJumpTo);
      
      aDoesCell++;
      
      continue;
    }
    
    // push
    if ( *aDoesCell->m_afpIntrinsic == &Step::vPushValue ) {
      
      vTrace("%p: %s"_Stz,aDoesCell,pCallsWord->szWordNm());
      
      aDoesCell++;
      
      StkVal* aValue = reinterpret_cast<StkVal*>(aDoesCell);
      
      if (CN_STRVAL == aValue->eClass()) {
        vTrace(" \"%s\"\n"_Stz,_aStrVal(aValue)->azChar());
      }
      else {
        aValue->vToString(sString);
        vTrace(" %s\n"_Stz,sString.azChar());
      }
      
      i8 iBytes = aValue->nSize();
      aDoesCell = aAdd<Cell*>(aDoesCell,iBytes);
      
      continue;
    }
    
    // enterRoutine
    if ( *aDoesCell->m_afpIntrinsic == &Step::vEnterRoutine ) {
      
      Word* pCallsWord = (Word*)( ((u8)aDoesCell->m_afpIntrinsic & (u8)~0xFFFF)
                                 + sizeof(HeapBuffer));
      
      vTrace("%p: %s\n"_Stz,aDoesCell,pCallsWord->szWordNm());
      aDoesCell++;
      continue;
    }
    
    // other
    vTrace("%p: %s\n"_Stz,aDoesCell,pCallsWord->szWordNm());
    
    aDoesCell++;
  }
  
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
