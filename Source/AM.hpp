/*============================================================================*/
/* AM.hpp -- AttachMap helper                                                 */
/*============================================================================*/
#pragma once

//!                    ** Do not include StorageEngine.hpp **

#include "eHA.h"
#include "eIA.h"
#include "eJA.h"
#include "eOA.h"
#include "ePA.h"
#include "eRA.h"
#include "eRK.h"
#include "eTA.h"
#include "eVA.h"
#include "eXA.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
    static i8   iNextOpId(void);
    static i8   iNextPlanId(void);
    static i8   iNextResourceId(void);
    static i8   iNextStreamId(void);
    static i8   iNextTaskId(void);
    static i8   iNextThreadId(void);
    static Stz* szNodeName(void);
    static i8*   iaObjectsInPool(void);
    static JobList* pOriginatingJobs(void);
    static JobList* pParticipatingJobs(void);
    static i8   iPid(void);
    static void vProcessExit(void);
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
    static void vForthWord(ForthWord* pForthWord);
    static void vGrabTaskLock(Task* pTask);
    static void vIndexEntries(i8 nIndexEntries);
    static void vIndexName(Job_Dynamic* pJob, Stz* szIndexNm);
    static Index* pIndex(Job_Dynamic* pJob);
    static void vMonitorPortrait(s48K& sRapidsPortrait);
    static void vNextTempName(s32& sName);
    static void vPulseTask(Task* pTask);
    static void vSetAcksRmg(Task* pTask, i8 nAcks);
    static void vStepTiming(StepTiming* pStepTiming);
    static void vSubInserters(i8 nSubInserters);
    static void vTableName(Job_Dynamic* pJob, Stz* szTableNm);
    static Table* pTable(Job_Dynamic* pJob);
    static void vTakenBuffers(PoolBuffer* pTakenBuffers);
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
    static bool bJobIsForeign(i8 iJobId);
    static void vLogArguments(void);
    static void vPrintArguments(void);
    static void vPrintAttachMap(void);
    static void vPrintStartupInfo(void);
    static i8   nRandomLevels(void);
    static i8   iTaskQueue(Task* pTask);
    static i8   iTaskState(Task* pTask);
    static Job* pThisTLSJob(void);
    static Op*  pThisTLSOp(void);
    static Session* pThisTLSSession(void);
    static Task* pThisTLSTask(void);
    static void vWelcomeText(s256& sWelcome);
    static void vWriteLog(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    //
    static void vDdlDropRead(Task* pTask);
    static void vDdlDropWrite(Task* pTask);
    static void vDdlGrabRead(Task* pTask);
    static void vDdlGrabWrite(Task* pTask);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    //
    static bool bSpawnJob(Session* pSession, Job* pJob, i8 iSeed);
    static bool bSpawnOp(Session* pSession, Op* pOp);
    static void vStateDiagram(s256& sDiagram, eCN eJobClass);
    static void vWriteTrace(Stz* aFormat, va_list xArgs);

    static void vLogInf(Stz* szFormat, ...);
    static void vLogItf(i8 iItfId, eIA eItfAct, Stz* szFormat, ...);
    static void vLogJob(i8 iJobId, eJA eJobAct, Stz* szFormat, ...);
    static void vLogOp (i8 iOpId,  eOA eOpAct,  Stz* szFormat, ...);
    static void vLogPrm(i8 iPrmId, ePA ePrmAct, Stz* szFormat, ...);
    static void vLogRes(i8 iResId, eRA eResAct, eRK eSysRes, Stz* szFormat, ...);
    static void vLogSvc(i8 iSvcId, eVA eSvcAct, Stz* szFormat, ...);
    static void vLogThd(i8 iThdId, eHA eThdAct, Stz* szFormat, ...);
    static void vLogTsk(i8 iTskId, eTA eTskAct, Stz* szFormat, ...);
    static void vLogTxn(i8 iTxnId, eXA eTxnAct, Stz* szFormat, ...);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
