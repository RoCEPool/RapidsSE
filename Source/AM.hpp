/*============================================================================*/
/* AM.hpp -- AttachMap helper                                                 */
/*============================================================================*/
#pragma once

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
    static i8   nSlotsPerNode(void);
    static i8   nSubIngesters(void);
    static i8   nSubInserters(void);
    static i8   nSubLoaders(void);
    static i8   nThreads(void);

    //
    static Interface::ApiState* pApiState(void);
    static bool bBootError(void);
    static Pool::BufferPool* pBufferPool(void);
    static Persistence::CarryOver* pCarryOver(void);
    static System::Lock* pDdlLock(void);
    static System::Lock* pDicLock(void);
    static i8   iElapsedSsec(void);
    static i8   iEpoch(void);
    static Chunks::Formatters* pFormatters(void);
    static Forth::ForthWord* pForthWord(void);
    static Grids::Grid* pGrid(void);
    static Pool::HeapMem* pHeapMem(void);
    static i8   nIndexEntries(void);
    static Measurement::LockTracker* pLockTracker(void);
    static bool bLogging(void);
    static Correctness::EnumFlags* pMarkAction(void);
    static Correctness::EnumFlags* pMarkApi(void);
    static Correctness::EnumFlags* pMarkJob(void);
    static Correctness::EnumFlags* pMarkResource(void);
    static Correctness::EnumFlags* pMarkTrace(void);
    static i8*   iaObjectsInPool(void);
    static Jobs::JobList* pOriginatingJobs(void);
    static Jobs::JobList* pParticipatingJobs(void);
    static i8   iPid(void);
    static bool bProgress(void);
    static bool bMetadata(void);
    static i8   iNextDiagram(void);
    static i8   iNextEpoch(void);
    static i8   iNextJobId(void);
    static i8   iNextPlanId(void);
    static Scheduling::Queues* pQueues(void);
    static Correctness::ResourceTracker* pResourceTracker(void);
    static Forth::Dic* pRootDic(void);
    static Correctness::Statistics* pStatistics(void);
    static Measurement::StepTiming* pStepTiming(void);
    static Pool::PoolBuffer* pTakenBuffers(void);
    static i8   nTakenBytes(void);
    static Forth::ForthWord* pTenants(void);
    static i8   iThisNodeNo(void);
    static void vThreadBegins(void);
    static i8   nThreadsBegun(void);
    static bool bTracepoint(void);
    static Transactions::TxSet* pTxSet(void);
    static bool bVerbose(void);
  
    //
    static void vAttachMap(Header* aHeader);
    static void vBootError(bool bBootError);
    static void vForthWord(Forth::ForthWord* pForthWord);
    static void vStepTiming(Measurement::StepTiming* pStepTiming);
    static void vSubInserters(i8 nSubInserters);
    static void vTakenBuffers(Pool::PoolBuffer* pTakenBuffers);
    static void vTakeBytes(i8 nTakeBytes);
    static void vTakenBytes(i8 nTakenBytes);
    static void vTenants(Forth::ForthWord* pTenants);
  
    //
    static bool bAttached(void);
    static void vDumpApi(void);
    static void vWriteLog(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    //
    static void vNote(eSR eSysRes,Stz* szFormat, ...);
    static void vWriteTrace(Stz* aFormat, va_list xArgs);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
