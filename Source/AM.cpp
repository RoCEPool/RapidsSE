/*============================================================================*/
/* AM.cpp -- AttachMap helper                                                 */
/*============================================================================*/

#include "Oblique.h"

extern AttachMap*  gpAttachMap;

extern Arguments   gxArguments;
extern BufferPool* gpBufferPool;
extern File*       gpTraceFile;
extern Log*        gpLog;

extern eHT        geWhichHeap[];
extern i8         giClassSize[];
extern pFINALIZE *gpFinalize[];

// Arg
bool AM::bDeveloper(void)     { return gpAttachMap->Arg.m_bDeveloper;         }
i8   AM::nDummyLoop(void)     { return gpAttachMap->Arg.m_nDummyLoop;         }
i8   AM::nDummySleep(void)    { return gpAttachMap->Arg.m_nDummySleep;        }
i8   AM::nDummyThreads(void)  { return gpAttachMap->Arg.m_nDummyThreads;      }
i8   AM::iJumboSize(void)     { return gpAttachMap->Arg.m_iJumboSize;         }
bool AM::bLog(void)           { return gpAttachMap->Arg.m_bLog;               }
i8   AM::nMaxLoadErrors(void) { return gpAttachMap->Arg.m_nMaxLoadErrors;     }
i8   AM::nMaxReadyLP(void)    { return gpAttachMap->Arg.m_nMaxReadyLP;        }
i8   AM::nNodes(void)         { return gpAttachMap->Arg.m_nNodes;             }
Stz* AM::szRegionName(void)   { return gpAttachMap->Arg.m_sRegionName.aStz(); }
i8   AM::iRegionSize(void)    { return gpAttachMap->Arg.m_iRegionSize;        }
i8   AM::nSlotsPerNode(void)  { return gpAttachMap->Arg.m_nSlotsPerNode;      }
i8   AM::nSubIngesters(void)  { return gpAttachMap->Arg.m_nSubIngesters;      }
i8   AM::nSubInserters(void)  { return gpAttachMap->Arg.m_nSubInserters;      }
i8   AM::nSubLoaders(void)    { return gpAttachMap->Arg.m_nSubLoaders;        }
i8   AM::nThreads(void)       { return gpAttachMap->Arg.m_nThreads;           }

//
ApiState* AM::pApiState(void) { return gpAttachMap->m_pApiState;              }
bool AM::bBootError(void)     { return gpAttachMap->m_bBootError;             }
BufferPool* AM::pBufferPool(void) { return gpAttachMap->m_pBufferPool;        }
CarryOver* AM::pCarryOver(void)   { return gpAttachMap->m_pCarryOver;         }
EnumFlags* AM::pClassFlags(void)  { return & gpAttachMap->m_oClassFlags;      }
Lock* AM::pDdlLock(void)      { return & gpAttachMap->m_okDdlLock;            }
Lock* AM::pDicLock(void)      { return & gpAttachMap->m_okDicLock;            }
i8   AM::iElapsedSsec(void)   { return gpAttachMap->m_oBirth.iElapsedSsec();  }
i8   AM::iEpoch(void)         { return gpAttachMap->m_iEpoch;                 }
Formatters* AM::pFormatters(void) { return & gpAttachMap->m_oFormatters;      }
ForthWord* AM::pForthWord(void)   { return gpAttachMap->m_pForthWord;         }
Grid* AM::pGrid(void)         { return gpAttachMap->m_pGrid;                  }
bool AM::bHasStarted(void)    { return gpAttachMap->m_pGrid->m_bHasStarted;   }
HeapMem* AM::pHeapMem(void)   { return & gpAttachMap->m_xHeapMem;             }
i8   AM::nIndexEntries(void)  { return gpAttachMap->m_nIndexEntries;          }
LockTracker* AM::pLockTracker(void) { return & gpAttachMap->m_oLockTracker;   }
bool AM::bLogging(void)       { return gpLog ? true : false;                  }
i8   AM::iNextEpoch(void)     { return ++ gpAttachMap->m_iEpoch;              }
Stz* AM::szNodeName(void)     {
  return gpAttachMap->m_pGrid->m_pThisNode->aNodeName();  }
JobList* AM::pOriginatingJobs(void) {
  return gpAttachMap->m_pQueues->m_pOriginatingJobs; }
JobList* AM::pParticipatingJobs(void) {
  return gpAttachMap->m_pQueues->m_pParticipatingJobs; }
i8   AM::iPid(void)           { return gpAttachMap->m_iPid;                   }
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
i8   AM::nPartitions(void)    { return gpAttachMap->m_pGrid->m_nPartitions;   }
Queues* AM::pQueues(void)     { return gpAttachMap->m_pQueues;                }
ResourceTracker* AM::pResourceTracker(void) {
  return & gpAttachMap->m_oResourceTracker; }
Dic* AM::pRootDic(void)       { return & gpAttachMap->m_oRootDic;             }
Statistics* AM::pStatistics(void) { return & gpAttachMap->m_oStatistics;      }
StepTiming* AM::pStepTiming(void)  { return gpAttachMap->m_pStepTiming;       }
PoolBuffer* AM::pTakenBuffers(void) { return gpAttachMap->m_pTakenBuffers;    }
i8   AM::nTakenBytes(void)    { return gpAttachMap->m_nTakenBytes;            }
ForthWord* AM::pTenants(void) { return gpAttachMap->m_pTenants;               }
i8   AM::iThisNodeNo(void)    { return gpAttachMap->m_pGrid->iThisNode();     }
void AM::vThreadBegins(void)  { ++ gpAttachMap->m_ixThreads;                  }
i8   AM::nThreadsBegun(void)  { return * gpAttachMap->m_ixThreads;            }
bool AM::bTracepoint(void)    { return gpAttachMap->m_bTracepoint;            }
TxSet* AM::pTxSet(void)       { return gpAttachMap->m_pTxSet;                 }
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
void AM::vBootError(bool bBootError) {
  gpAttachMap->m_bBootError = bBootError;
}
void AM::vDropTaskLock(Task* pTask) {
  Scheduler::vDropTaskLock(pTask);
}
void AM::vForthWord(ForthWord *pWord) {
  gpAttachMap->m_pForthWord = pWord;
}
void AM::vGrabTaskLock(Task* pTask) {
  Scheduler::vGrabTaskLock(pTask);
}
void AM::vIndexEntries(i8 nIndexEntries) {
  gpAttachMap->m_nIndexEntries = nIndexEntries;
}
void AM::vIndexName(Job_Dynamic* pJob, Stz* szIndexNm) {
  pJob->m_sIndexName = szIndexNm;
}
Index* AM::pIndex(Job_Dynamic* pJob) {
  return pJob->m_pIndex;
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
void AM::vStepTiming(StepTiming* pStepTiming) {
  gpAttachMap->m_pStepTiming = pStepTiming;
}
void AM::vSubInserters(i8 nSubInserters) {
  gpAttachMap->Arg.m_nSubInserters = nSubInserters;
}
void AM::vTableName(Job_Dynamic* pJob, Stz* szTableNm) {
  pJob->m_sObjectName = szTableNm;
}
Table* AM::pTable(Job_Dynamic* pJob) {
  return pJob->m_pTable;
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
void AM::vTaskTransition(Tasks::Task* pTask, i8 iTransition) {
  pTask->vTransition((eTR)iTransition);
}
bool AM::bTaskIsLocked(Task* pTask) {
  bool bLocked = pTask->bIsLocked();
  return bLocked;
}
void AM::vTenants(ForthWord *pTenants) {
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
  pApiState()->vDump();
}
Task* AM::pExecutingTask(Job* pJob) {
  Task* pTask = pJob->m_pExecutingTask;
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
  Task* pTask = pThread->pTask();
  Job* pJob = pTask->pJob();
  return pJob;
}
Op* AM::pThisTLSOp(void) {
  Thread* pThread = Thread::pThisTLSThread();
  Task* pTask = pThread->pTask();
  Op* pOp = pTask->pOp();
  return pOp;
}
Session* AM::pThisTLSSession(void) {
  Thread* pThread = Thread::pThisTLSThread();
  Task* pTask = pThread->pTask();
  Session* pSession = pTask->pSession();
  return pSession;
}
Tasks::Task* AM::pThisTLSTask(void) {
  Thread* pThread = Thread::pThisTLSThread();
  Task* pTask = pThread->pTask();
  return pTask;
}
Thread* AM::pThisTLSThread(void) {
  Thread* pThread = Thread::pThisTLSThread();
  return pThread;
}
Pthread* AM::pThisTLSPthread(void) {
  Thread* pThread = Thread::pThisTLSThread();
  Pthread* pPthread =pThread->m_pPthread;
  return pPthread;
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
/* state diagram                                                              */
/*----------------------------------------------------------------------------*/
bool AM::bSpawnJob(Session* pSession, Job* pJob, i8 iSeed)
{
  bool bResult = Api::bSpawnJob(pSession,pJob,(eSS)iSeed);
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* state diagram                                                              */
/*----------------------------------------------------------------------------*/
bool AM::bSpawnOp(Session* pSession, Op* pOp)
{
  bool bResult = Api::bSpawnOp(pSession,pOp);
  
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
  static Lock oLock(LK_TRACE);
  static char caFormatted[C_1MB] = {'\0'};
  
  bool bUseLock = (gpAttachMap ? true : false);
  bool bLowMem  = gpBufferPool ? gpBufferPool->bLowMemory() : false;
  
  if (bUseLock)
    oLock.vGrabWrite();
  
  // produce formatted line
  vsnprintf(caFormatted,sizeof(caFormatted) - 3,aFormat->aChar(),xArgs);
  
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
void AM::vLogReq(eSL eSeverity, Request* pReq, eMA eReqAct, Stz* szFormat, ...)
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
    vFatal("Enum for CN_ENUM not found in aCN\n"_Stz);
  
  if (iEntriesECN != iEntriesPCN) {
    vFatal("\n!!!ALERT!!! No of entries in aCN (%lld) do not match the no in eCN (%lld)\n\n"_Stz,
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
    vFatal("Enum for CN_ENUM not found in gpDump\n"_Stz);
  
  if (iEntriesECN != iEntriesGPDUMP) {
    vFatal("\n!!!ALERT!!! No of entries in gpDump (%lld) do not match the no in eCN (%lld)\n\n"_Stz,
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
    vFatal("Enum for CN_ENUM not found in giClassSize\n"_Stz);
  
  if (iEntriesECN != iEntriesGCLASSSIZE) {
    vFatal("\n!!!ALERT!!! No of entries in giClassSize (%lld) do not match the no in eCN (%lld)\n\n"_Stz,
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
    vFatal("Enum for CN_ENUM not found in gpDump\n"_Stz);
  
  if (iEntriesECN != iEntriesGPDUMP) {
    vFatal("\n!!!ALERT!!! No of entries in gpDump (%lld) do not match the no in eCN (%lld)\n\n"_Stz,
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
  
  else if (CN_PTHREAD == eClassNo)
    _pPthread(pFocal)->vFocalOutline(sOutline);
  
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
