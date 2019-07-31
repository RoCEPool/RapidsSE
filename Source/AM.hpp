/*============================================================================*/
/* AM.hpp -- AttachMap helper                                                 */
/*============================================================================*/
#pragma once

//! Do not include StorageEngine.hpp

namespace StorageEngine
{
  class AM
  {
  public:
    // args
    static bool bDeveloper(void);
    static i8   nDummyLoop(void);
    static i8   nDummySleep(void);
    static i8   nDummyThreads(void);
    static i8   iJumboSize(void);
    static bool bLog(void);
    static i8   nMaxLoadErrors(void);
    static i8   nMaxReadyLP(void);
    static i8   nNodes(void);
    static Stz* szRegionName(void);
    static i8   iRegionSize(void);
    static bool bSethScript(void);
    static i8   nSlotsPerNode(void);
    static i8   nSubIngesters(void);
    static i8   nSubInserters(void);
    static i8   nSubLoaders(void);
    static i8   nThreads(void);

    //
    static ApiState* pApiState(void);
    static bool bBootError(void);
    static BufferPool* pBufferPool(void);
    static CarryOver* pCarryOver(void);
    static Lock* pDdlLock(void);
    static Lock* pDicLock(void);
    static i8   iElapsedSsec(void);
    static i8   iEpoch(void);
    static Formatters* pFormatters(void);
    static ForthWord* pForthWord(void);
    static Grid* pGrid(void);
    static bool bHasStarted(void);
    static HeapMem* pHeapMem(void);
    static i8   nIndexEntries(void);
    static LockTracker* pLockTracker(void);
    static bool bLogging(void);
    static EnumFlags* pMarkAction(void);
    static EnumFlags* pMarkApi(void);
    static EnumFlags* pMarkJob(void);
    static EnumFlags* pMarkResource(void);
    static EnumFlags* pMarkTrace(void);
    static bool bMetadata(void);
    static i8   iNextDiagram(void);
    static i8   iNextEpoch(void);
    static i8   iNextJobId(void);
    static i8   iNextPlanId(void);
    static Stz* szNodeName(void);
    static i8*   iaObjectsInPool(void);
    static JobList* pOriginatingJobs(void);
    static JobList* pParticipatingJobs(void);
    static i8   iPid(void);
    static bool bProgress(void);
    static i8   nPartitions(void);
    static Queues* pQueues(void);
    static ResourceTracker* pResourceTracker(void);
    static Dic* pRootDic(void);
    static Statistics* pStatistics(void);
    static StepTiming* pStepTiming(void);
    static PoolBuffer* pTakenBuffers(void);
    static i8   nTakenBytes(void);
    static ForthWord* pTenants(void);
    static i8   iThisNodeNo(void);
    static void vThreadBegins(void);
    static i8   nThreadsBegun(void);
    static bool bTracepoint(void);
    static TxSet* pTxSet(void);
    static bool bVerbose(void);
  
    //
    static void vAckWaitingTask(Task* pTaskWaiting, Task* pTaskAcking);
    static void vAddConstant(Stz* szName,Stz* szValue);
    static void vAttachMap(Header* aHeader);
    static void vBootError(bool bBootError);
    static void vDropTaskLock(Task* pTask);
    static void vForthWord(Forth::ForthWord* pForthWord);
    static void vGrabTaskLock(Task* pTask);
    static void vIndexEntries(i8 nIndexEntries);
    static void vMonitorPortrait(s48K& sRapidsPortrait);
    static void vNextTempName(s32& sName);
    static void vSetAcksRmg(Task* pTask, i8 nAcks);
    static void vStepTiming(StepTiming* pStepTiming);
    static void vSubInserters(i8 nSubInserters);
    static void vTakenBuffers(Pool::PoolBuffer* pTakenBuffers);
    static void vTakeBytes(i8 nTakeBytes);
    static void vTakenBytes(i8 nTakenBytes);
    static bool bTaskIsLocked(Task* pTask);
    static void vTaskTransition(Task* pTask, i8 iTransition);
    static void vTenants(ForthWord* pTenants);
    static void vTriggerBidingTask(Task* pTask);

    //
    static bool bAttached(void);
    static void vDumpApi(void);
    static Task* pExecutingTask(Job* pJob);
    static void vPrintArguments(void);
    static void vPrintAttachMap(void);
    static i8   nRandomLevels(void);
    static i8   iTaskQueue(Task* pTask);
    static i8   iTaskState(Task* pTask);
    static Job* pThisTLSJob(void);
    static Op*  pThisTLSOp(void);
    static Session* pThisTLSSession(void);
    static Task* pThisTLSTask(void);
    static void vWriteLog(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    //
    static Stz* szEnumText(i8 iEnumGroup, i8 i_Enum);
    static bool bSpawnJob(Session* pSession, Job* pJob, i8 iSeed);
    static bool bSpawnOp(Session* pSession, Op* pOp);
    static void vStateDiagram(s256& sDiagram, eCN eJobClass);
    static void vNote(Stz* szFormat, ...);
    static void vNote(eSR eSysRes,Stz* szFormat, ...);
    static void vWriteTrace(Stz* aFormat, va_list xArgs);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
