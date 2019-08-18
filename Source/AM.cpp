/*============================================================================*/
/* AM.cpp -- AttachMap helper                                                 */
/*============================================================================*/

#include "Oblique.h"

extern AttachMap*  gpAttachMap;

extern Arguments   gxArguments;
extern BufferPool* gpBufferPool;
extern File*       gpTraceFile;
extern Log*        gpLog;


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
CarryOver* AM::pCarryOver(void) { return gpAttachMap->m_pCarryOver;           }
Lock* AM::pDdlLock(void)      { return & gpAttachMap->m_okDdlLock;            }
Lock* AM::pDicLock(void)      { return & gpAttachMap->m_okDicLock;            }
i8   AM::iElapsedSsec(void)   { return gpAttachMap->m_oBirth.iElapsedSsec();  }
i8   AM::iEpoch(void)         { return gpAttachMap->m_iEpoch;                 }
Formatters* AM::pFormatters(void) { return & gpAttachMap->m_oFormatters;      }
ForthWord* AM::pForthWord(void) { return gpAttachMap->m_pForthWord;           }
Grid* AM::pGrid(void)         { return gpAttachMap->m_pGrid;                  }
bool AM::bHasStarted(void)    { return gpAttachMap->m_pGrid->m_bHasStarted;   }
HeapMem* AM::pHeapMem(void)   { return & gpAttachMap->m_xHeapMem;             }
i8   AM::nIndexEntries(void)  { return gpAttachMap->m_nIndexEntries;          }
LockTracker* AM::pLockTracker(void) { return & gpAttachMap->m_oLockTracker;   }
bool AM::bLogging(void)       { return gpLog ? true : false;                  }
EnumFlags* AM::pMarkAction(void) { return & gpAttachMap->m_oMarkAction;       }
EnumFlags* AM::pMarkApi(void) { return & gpAttachMap->m_oMarkApi;             }
EnumFlags* AM::pMarkJob(void) { return & gpAttachMap->m_oMarkJob;             }
EnumFlags* AM::pMarkResource(void) { return & gpAttachMap->m_oMarkResource;   }
EnumFlags* AM::pMarkTrace(void) { return & gpAttachMap->m_oMarkTrace;         }
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
i8   AM::iNextResourceId(void){
  if (gpAttachMap)
    return ++ gpAttachMap->m_inNextResourceId;
  else
    return C_UNKNOWN;
}
i8   AM::iNextStreamId(void)  { return ++ gpAttachMap->m_inNextStreamId;      }
i8   AM::iNextTaskId(void)    { return ++ gpAttachMap->m_inNextTaskId;        }
i8   AM::iNextThreadId(void)  { return ++ gpAttachMap->m_inNextThreadId;      }
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
  
  AM::vLogSys(nullptr,eSysAct,"%s"_Stz,caFormatted);
  
  if (bUseLock)
    oLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
  
  gpLog->iEntryFyi(CN_NONE,szFormat,xArgs);
  
  va_end(xArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a interface log entry                                                  */
/*----------------------------------------------------------------------------*/
void AM::vLogApi(Session* pSsn, eAA eApiAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (! (AM::pMarkAction()->bEnum(eApiAct) ||
         AM::pMarkAction()->bAll()) )
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 iSsnId = C_UNKNOWN; //pSsn->iSsnId();
  
  gpLog->iEntryApi(pSsn->eClass(),iSsnId,eApiAct,szFormat,xVaArgs);
  
  va_end(xVaArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a job log entry                                                        */
/*----------------------------------------------------------------------------*/
void AM::vLogJob(Job* pJob, eJA eJobAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (! (AM::pMarkAction()->bEnum(eJobAct) ||
         AM::pMarkAction()->bAll()) )
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 iJobId = pJob ? pJob->iJobId() : C_UNKNOWN;
  gpLog->iEntryJob(iJobId,eJobAct,szFormat,xVaArgs);
  
  va_end(xVaArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a lock log entry                                                       */
/*----------------------------------------------------------------------------*/
void AM::vLogLck(Lock* pLock, eLA eLckAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  
  if (LK_ACTION_LOG == pLock->eLockKind())
    return;
  
  if (! AM::bLogging())
    return;
  
  if (! (AM::pMarkAction()->bEnum(eLckAct) ||
         AM::pMarkAction()->bAll()) )
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 iLckId =  pLock ? pLock->iLockId() : C_UNKNOWN;
  gpLog->iEntryLck(pLock->eClass(),iLckId,eLckAct,szFormat,xVaArgs);
  
  va_end(xVaArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a log log entry                                                        */
/*----------------------------------------------------------------------------*/
void AM::vLogLog(Log* pLog, eGA eLogAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  
  if (! AM::bLogging())
    return;
  
//  if (! (AM::pMarkAction()->bEnum(eLogAct) ||
//         AM::pMarkAction()->bAll()) )
//  return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 iLogId = 0;
  gpLog->iEntryLog(pLog->eClass(),iLogId,eLogAct,szFormat,xVaArgs);
  
  va_end(xVaArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a object log entry                                                     */
/*----------------------------------------------------------------------------*/
void AM::vLogObj(Object* pObject, eCA eObjAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  
  if (! AM::bLogging())
    return;
  
  if (! (AM::pMarkAction()->bEnum(eObjAct) ||
         AM::pMarkAction()->bAll()) )
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  gpLog->iEntryObj(pObject,eObjAct,szFormat,xVaArgs);
  
  va_end(xVaArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a operator log entry                                                   */
/*----------------------------------------------------------------------------*/
void AM::vLogOpr(Op* pOpr, eOA eOprAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (! (AM::pMarkAction()->bEnum(eOprAct) ||
         AM::pMarkAction()->bAll()) )
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 iOprId = pOpr->iOpId();
  
  gpLog->iEntryOpr(pOpr->eClass(),iOprId,eOprAct,szFormat,xVaArgs);
  
  va_end(xVaArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a prompt log entry                                                     */
/*----------------------------------------------------------------------------*/
void AM::vLogPrm(SePrompt* pPrm, ePA ePrmAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
//  if (! (AM::pMarkAction()->bEnum(ePrmAct) ||
//         AM::pMarkAction()->bAll()) )
//    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 iPrmId = pPrm->iPromptId();
  gpLog->iEntryPrm(pPrm->eClass(),iPrmId,ePrmAct,szFormat,xVaArgs);
  
  va_end(xVaArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a resource log entry                                                   */
/*----------------------------------------------------------------------------*/
void AM::vLogRes(Resource* pRes, eRA eResAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  eRK eKind  = pRes ? pRes->eKind()  : RK_NONE;
  
  if (! (AM::pMarkResource()->bEnum(eKind) ||
         AM::pMarkResource()->bAll()) )
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8  iResId   = pRes ? pRes->iResId() : C_UNKNOWN;
  eCN eClassNo = pRes ? pRes->eClass() : CN_NONE;
  gpLog->iEntryRes(eClassNo,iResId,eResAct,eKind,szFormat,xVaArgs);
  
  va_end(xVaArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a service log entry                                                    */
/*----------------------------------------------------------------------------*/
void AM::vLogSvc(Job* pJob, eVA eSvcAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (! (AM::pMarkAction()->bEnum(eSvcAct) ||
         AM::pMarkAction()->bAll()) )
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 iSvcId = pJob->m_iJobId;
  
  gpLog->iEntrySvc(pJob->eClass(),iSvcId,eSvcAct,szFormat,xVaArgs);
  
  va_end(xVaArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a system log entry                                                     */
/*----------------------------------------------------------------------------*/
void AM::vLogSys(AttachMap* pSys, eYA eSysAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
//  if (! (AM::pMarkAction()->bEnum(eSysAct) ||
//         AM::pMarkAction()->bAll()) )
//  return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 iSysId = 0;
  
  gpLog->iEntrySys(CN_ATTACHMAP,iSysId,eSysAct,szFormat,xVaArgs);
  
  va_end(xVaArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a thread log entry                                                     */
/*----------------------------------------------------------------------------*/
void AM::vLogThd(Thread* pThread, eHA eThdAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (! (AM::pMarkAction()->bEnum(eThdAct) ||
         AM::pMarkAction()->bAll()) )
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 iThdId    = pThread ? pThread->m_iThreadId : C_UNKNOWN;
  eCN eClassNo = pThread ? pThread->eClass()    : CN_NONE;
  gpLog->iEntryThd(eClassNo,iThdId,eThdAct,szFormat,xVaArgs);

  va_end(xVaArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a rask log entry                                                       */
/*----------------------------------------------------------------------------*/
void AM::vLogTsk(Task* pTask, eTA eTskAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
  return;
  
  if (! (AM::pMarkAction()->bEnum(eTskAct) ||
         AM::pMarkAction()->bAll()) )
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 iTskId    = pTask ? pTask->iTaskId() : C_UNKNOWN;
  eCN eClassNo = pTask ? pTask->eClass()  : CN_NONE;
  gpLog->iEntryTsk(eClassNo,iTskId,eTskAct,szFormat,xVaArgs);
  
  va_end(xVaArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a transaction log entry                                                */
/*----------------------------------------------------------------------------*/
void AM::vLogTxn(Tx* pTx, eXA eTxnAct, Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (! (AM::pMarkAction()->bEnum(eTxnAct) ||
         AM::pMarkAction()->bAll()) )
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  i8 iTxnId = pTx->iDistTxId();
  gpLog->iEntryTxn(pTx->eClass(),iTxnId,eTxnAct,szFormat,xVaArgs);

  va_end(xVaArgs);
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
  
  AM::vLogFyi("%s naps for %lldms"_Stz,aCN[eCallingClass],nMilliSeconds);
  
  i8 rCode = nanosleep(&xReq,&xRem);
  
  if (rCode && errno != EINTR) {
    vWarn("AM::vNap error; errno=%lld\n"_Stz,errno);
  }
  
  AM::vLogFyi("%s woken from nap"_Stz,aCN[eCallingClass]);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
