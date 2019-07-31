/*============================================================================*/
/* AM.cpp -- AttachMap helper                                                 */
/*============================================================================*/

#include "StorageEngine.h"

AttachMap *   gpAttachMap    = nullptr;

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
bool AM::bSethScript(void)    { return gpAttachMap->Arg.m_bSethScript;        }
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
Stz* AM::szNodeName(void)     { return gpAttachMap->m_pGrid->m_pThisNode->aNodeName();  }
JobList* AM::pOriginatingJobs(void) { return gpAttachMap->m_pOriginatingJobs; }
JobList* AM::pParticipatingJobs(void) { return gpAttachMap->m_pParticipatingJobs; }
i8   AM::iPid(void)           { return gpAttachMap->m_iPid;                   }
bool AM::bProgress(void)      { return gpAttachMap->m_bProgress;              }
bool AM::bMetadata(void)      { return gpAttachMap->m_bMetadata;              }
i8   AM::iNextDiagram(void)   { return ++ gpAttachMap->m_ixNextDiagram;       }
i8   AM::iNextJobId(void)     { return ++ gpAttachMap->m_ixNextJobId;         }
i8   AM::iNextPlanId(void)    { return ++ gpAttachMap->m_ixNextPlanId;        }
i8*  AM::iaObjectsInPool(void){ return gpAttachMap->m_nObjectsInPool;         }
i8   AM::nPartitions(void)    { return gpAttachMap->m_pGrid->m_nPartitions;   }
Queues* AM::pQueues(void)     { return gpAttachMap->m_pQueues;                }
ResourceTracker* AM::pResourceTracker(void) { return & gpAttachMap->m_oResourceTracker; }
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
  pTask->vDropLock();
}
void AM::vForthWord(ForthWord *pWord) {
  gpAttachMap->m_pForthWord = pWord;
}
void AM::vGrabTaskLock(Task* pTask) {
  pTask->vGrabLock();
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
void AM::vNoteArguments(void) {
  gxArguments.vMark();
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
void AM::vWriteLog(void) {
  Log::vWriteCsv(false);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump enums                                                                 */
/*----------------------------------------------------------------------------*/
Stz* AM::szEnumText(i8 iEnumGroup, i8 i_Enum)
{
  switch (iEnumGroup) {
      
    case _EG_AC: return aAC[i_Enum];
    case _EG_AK: return aAK[i_Enum];
    case _EG_AN: return aAN[i_Enum];
    case _EG_AT: return aAT[i_Enum];
    case _EG_CN: return aCN[i_Enum];
    case _EG_CT: return aCT[i_Enum];
    case _EG_DC: return aDC[i_Enum];
    case _EG_DL: return aDL[i_Enum];
    case _EG_DM: return aDM[i_Enum];
    case _EG_DS: return aDS[i_Enum];
    case _EG_EC: return aEC[i_Enum];
    case _EG_HT: return aHT[i_Enum];
      //case _EG_IF: return aIF[i_Enum];
    case _EG_IS: return aIS[i_Enum];
    case _EG_IT: return aIT[i_Enum];
    case _EG_JF: return aJF[i_Enum];
    case _EG_JK: return aJK[i_Enum];
    case _EG_JL: return aJL[i_Enum];
    case _EG_JP: return aJP[i_Enum];
    case _EG_JS: return aJS[i_Enum];
    case _EG_JT: return aJT[i_Enum];
    case _EG_KC: return aKC[i_Enum];
    case _EG_LK: return aLK[i_Enum];
    case _EG_PF: return aPF[i_Enum];
    case _EG_PK: return aPK[i_Enum];
    case _EG_PY: return aPY[i_Enum];
    case _EG_QK: return aQK[i_Enum];
      //case _EG_QR: return aQR[i_Enum];
    case _EG_RE: return aRE[i_Enum];
    case _EG_RO: return aRO[i_Enum];
    case _EG_SC: return aSC[i_Enum];
    case _EG_SR: return aSR_Short[i_Enum];
    case _EG_SS: return aSS[i_Enum];
    case _EG_TD: return aTD[i_Enum];
    case _EG_TK: return aTK[i_Enum];
    case _EG_TN: return aTN[i_Enum];
    case _EG_TP: return aTP[i_Enum];
    case _EG_TR: return aTR[i_Enum];
    case _EG_TS: return aTS[i_Enum];
    case _EG_TT: return aTT[i_Enum];
    case _EG_WT: return aWT[i_Enum];
      
    default:
      vTrace("AM::szEnumText: Unknown enum group %lld\n"_Stz, iEnumGroup);
      return nullptr;
  }
}

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
    case Correctness::CN_JOB_EXPORT: {
      
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
/* add a log entry                                                            */
/*----------------------------------------------------------------------------*/
void AM::vNote(Stz* aFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  
  gpLog->iAddNoteArg(C_INF_GROUP,0,false,aFormat,xArgs);
  
  va_end(xArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a log entry for resource                                               */
/*----------------------------------------------------------------------------*/
void AM::vNote(eSR eSysRes,Stz* szFormat, ...)
{
#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  if (AM::pMarkResource()->bEnum(eSysRes) ||
      AM::pMarkResource()->bAll())
    return;
  
  va_list xVaArgs;
  
  va_start(xVaArgs,szFormat);
  
  gpLog->iAddNoteArg(_EG_SR,eSysRes,false,szFormat,xVaArgs);
  
  va_end(xVaArgs);
#endif
}

/*----------------------------------------------------------------------------*/
/* write to trace file etc                                                    */
/*----------------------------------------------------------------------------*/
void AM::vWriteTrace(Stz* aFormat, va_list xArgs)
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
  
  if (bUseLock)
    oLock.vDropWrite();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
