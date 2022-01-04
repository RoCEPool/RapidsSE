/*============================================================================*/
/* AM.hpp -- AttachMap helper                                                 */
/*============================================================================*/
#pragma once

//!                    ** Do not include BASE.h **
#include <list>
#include <iterator>

#include "Heartbeat.hpp"
#include "eAA.h"
#include "eDA.h"
#include "eEC.h"
#include "eEV.h"
#include "eGA.h"
#include "eHA.h"
#include "eLA.h"
#include "eMA.h"
#include "eOA.h"
#include "ePA.h"
#include "eYX.h"
#include "eRA.h"
#include "eTA.h"
#include "eVA.h"
#include "eXA.h"


class PtnId;

namespace OBLIQUE {
  class AM;
}

using namespace OBLIQUE;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace BASE {
         
  class Birth;
  class BufferPool;

  class Charser;
  class ClassFlags;
  class CodeCell;
  class CsvBuffer;

  class DataBuffer;
  class Datum_h;
  class Dic;
  class DicStack;

  class File;
  class Format;

  class Header;
  class Heap;
  class Heaps;
  class HeapBuffer;

  class Input;
  class IvlLib;

  class JobId;

  class Lock;
  class Locks;

  class MasterLock;
  class Mutex;

  class Now;

  class Object;
  class ObjectSnapshot;
  class Operand;

  class PoolBuffer;
  class PoolHelper;
  class PsxThd;

  class Queue;

  class Resource;
  class Resources;

  class Schedule;
  class Scheduler;
  class Socket;
  class Starter;
  class Statistics;
  class String;

  class TagSet;
  class Task;
  class Thread;
  class ThreadVec;
  class Timer;
  class Token;
  class TokenVec;
  class Tx;
  class TxSet;

  class Word;

  class RU_Cpu;
  class RU_Memory;

  class SN_Bcd;
  class SN_Bol;
  class SN_Dte;
  class SN_Flt;
  class SN_Int;
  class SN_Ivl;
  class SN_Tme;
  class SN_Tsp;

  class SP_Bcd;
  class SP_Bol;
  class SP_Dte;
  class SP_Flt;
  class SP_Int;
  class SP_Ivl;
  class SP_Tme;
  class SP_Tsp;

  template <i8 SPACE> class Datum_t;
}

using namespace BASE;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace STORE {
  class AttachMap;

  class Bucket;
  //class BucketMap;
  class BucketMap_dyn;

  class Crv;
  class Crw;
  class CrwBuffer;
  class CrwPacker;

  class Dynamo;

  class EachRowBuffer;
  class Encoding;
  class Engine;
  class EntBuffer;
  class EventHandlers;

  class Grid;

  class HeadLine;

  class IdxColVec;
  class Index;
  class IndexTree;
  class InfoSet;
  class InfoSetBase;
  class InternalTables;

  class Job;
  class JobList;

  class LogicUnit;

  class Node;

  class Op;
  class OverLine;

  class Pipeline;
  class PtnMap;
  class PtnStm_cursor;
  class PtnStream;

  class Row;
  class RowBuffer;
  class RowCompactor;
  class RowInflator;

  class Session;
  class Sessions;
  class SessionState;
  class Stage;
  class StageVec;
  class Step;
  class Steps;
  class Stream;
  class StreamDef;

  class Table;
  class TablePtn;
  class TailLine;
  
  class Parcel;
  
  class Sends;

  class SP_Encoding;
}

using namespace STORE;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace PROMPT {
  class SePrompt;
}

using namespace PROMPT;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace OBLIQUE
{
  class AM
  {
  public:

    class BKTMAP
    {
    public:
      static PtnId tBktIdToLglPtnId(i8 iBktId);

      static BucketMap_dyn* pBucketMap(void);
      static i8    iHashToBktId(i8 iHash);
    };

    class DIC
    {
    public:
      static void vListContents(Dic* pDic);
      static void vListContents(Dic* pDic, i8 i_Depth);
      static Dic* pRoot(void);
    };
  
    class DEMON
    {
    public:
      static void vPipeTo(Stz* szDemonNm, Operand* pOperand);
    };
  
    class EE
    {
    public:
      static void vAssertEEDelete(eCN eInstance,
                                  Object* pInstance,
                                  
                                  eCN eExpecting,
                                  eCN eDeleter,
                                  Object* pDeleter,
                                  Stz* szDelFunNm,
                                  bool bTrace);
    };
  
    class ENGINE
    {
    public:
      static void vSignalHandler(i8 iSignal);
    };

    class GRID
    {
    public:
      static Node* pGetNode(i8 iNodeNo);
      static Node* pGetThisNode(void);
      static i8   iGetNthNodeNo(i8 i_Node);
      static i8   nKnownNodes(void);
      static void vNodeDetected(i8 iNodeNo, Stz* szNodeNm,
                                Stz* szIPAddr, i8 iPortNo,
                                Stz* pszRegionNm);
      static i8   iThisNodeNo(void);
    };
  
    class HANDLERS
    {
    public:
      static EventHandlers* pEventHandlers(void);
    };

    class JOB
    {
    public:
      static bool bHasTag(Job* pJob, Stz* szTagNm);
      static Stz* szJobNm(Job* pJob);
      static eEC  ePreExecuteTest(Job* pJob);
    };
  
    class LOCK
    {
    public:
      static void vIntendReadUnlocked(Lock* pLock);
      static void vJustReadLocked(Lock* pLock);
      static void vIntendWriteUnlocked(Lock* pLock);
      static void vJustWriteLocked(Lock* pLock);
    };
    
    class LOGICUNIT
    {
    public:
      static void vAddReg(LogicUnit* pLogicUnit, void* aReg);
      static void vDelete(LogicUnit** ppLogicUnit);
    };

    class MASTERLOCK
    {
    public:
      static void vDropDdlReadLock(LogicUnit* pLogicUnit);
      static void vGrabDdlReadLock(LogicUnit* pLogicUnit);
    
      static void vDropReap(LogicUnit* pLogicUnit);
      static void vGrabReap(LogicUnit* pLogicUnit);
    
      static void vDropTraceLine(void);
      static void vGrabTraceLine(void);
    
      static void vDropTraceMulti(void);
      static void vGrabTraceMulti(void);
    
      static void vDropDdlWriteLock(LogicUnit* pLogicUnit);
      static void vGrabDdlWriteLock(LogicUnit* pLogicUnit);
    
      static void vDdlLockTestDrop(LogicUnit* pLogicUnit);

      static u8   uDdlLockWord(void);
    };
  
    class NO
    {
    public:
      static bool bFree(void);
    };

    class OBJECT
    {
    public:
      static i8 iNextNo(eCN eClassNo);
      static eEC eValidObject(Object* pObject);
    };

    class OP
    {
    public:
      static void vPipeToNextOp(Op* pCurrOp, Operand* pOperand,
                                Task* pTriggeringTask);
      static void vPipeToOp(Op* pOp, Operand* pOperand,
                            Task* pTriggeringTask);
    };

    class OPERAND
    {
    public:
      static bool bIsOoO(Operand* pOperand);
    };

    class PARCEL
    {
    public:
      static void vRecvContents(Parcel* pParcel, Socket* pSocket);
    };
  
    class POOL
    {
    public:
      static i8 iFreeSpace(void);
      static i8 iPctFree(void);
      static i8 iPctUsed(void);
      static i8 iTotalSpace(void);
      static i8 iUsedSpace(void);
    };

    class PROMPT
    {
    public:
      static bool bToPipelineStmt(String* pAsPipelineStmt,
                                  TokenVec* pFromTokenVec);
      static InfoSet* pToPromptOut(Stz* szStz);
    };


    class PTNMAP
    {
    public:
      static PtnId tHashToLglPtnId(i8 iHash);
      static i8 nKnownNodes(void);
      static i8 nPtnsPerNode(void);
      static i8 nPtnsInGrid(void);
      static PtnId tRobinToDtdPtnStream(i8 iRobin);
      static PtnId tRobinToLclPtnStream(i8 iRobin);
      static PtnId tRobinToNdePtnNo(i8 iRobin);
    };

    class SCHEDULER
    {
    public:
      static i8 nIdleThreads(void);
    };

    class SE
    {
    public:
      static void vWhereAmI(void);
    };
  
    class SESSION
    {
    public:
      static i8 iInternalSession(void);
      static Session* pInternalSession(void);
    };

    class SHOW
    {
    public:
      static bool bBoot(void);
      static bool bDir(void);
      static bool bDir_Failure(void);
      static bool bConnection(void);
      static bool bException(void);
      static bool bFile(void);
      static bool bFile_Failure(void);
      static bool bLowMem(void);
      static bool bLowMem_Recv(void);
      static bool bRecurring(void);
      static bool bSocket(void);
      static bool bSocket_Failure(void);
    };

    class STAGE
    {
    public:
      static Op* pGetOp(Stage* pStage, PtnId tPtnId, i8 i_Op);
    };
  
    class STARTER
    {
    public:
      static Starter* pStarter(void);
    };

    class STR
    {
    public:
      static void vReplaceTags(Stz* szDst, Stz* szSrc, TagSet* pTags);
    };

    class SYSARGS
    {
    public:
      static i8   nDefinedNodes(void);
      static bool bJobMetrics(void);
      static void vJobMetrics(bool bJobMetrics);
      static bool bLockTracking(void);
      static void vLockTracking(bool bLockTracking);
      static i8   nMetricsSleep(void);
      static void vMetricsSleep(i8 nMetricsSleep);
      static i8   iNetOpt(void);
      static void vNetOpt(i8 iNetOpt);
      static i8   nOperandYield(void);
      static void vOperandYield(i8 nOperandYield);
      static i8   nPtnsPerNode(void);
      static TagSet* pTagSet(void);
      static i8   nThreads(void);
      static void vThreadsInc(i8 iBy);
      static void vVerbose(bool bVerbose);
    };

    class SYSTEM
    {
    public:
      static i8   nActiveJobs(void);
      static i8   nNanoseconds(void);
      static bool bPassesLowMem(Job* pJob);
      static bool bPassesOpenSystem(Job* pJob);
      static bool bReapEnabled(void);
      static void vReapEnabled(bool bReapEnabled);
      static bool bStarted(void);
      static void vStarted(bool bStarted);
      static bool bSystemOpen(void);
      static void vSystemOpen(bool bSystemOpen);
      static bool bReady(void);
      static void vSystemReady(void);
      static i8   nUptimeInSeconds(void);
    };

    class TASK
    {
    public:
      static DicStack* pDicStack(void);
      static void vRetire(Task* pTask);
      static void vTriggerBiding(Task* pTaskBiding, Task* pTriggeringTask);
    };
  
    class THREAD
    {
    public:
      static ThreadVec* pDedicatedThreadVec(void);
      static void vDeleteThread(Thread* pThread);
      static ThreadVec* pGeneralThreadVec(void);
      static Thread* pNewThread(void);    
    };
  
    class TIMER
    {
    public:
      static Timer* pReferenceTimer(void);
    };

    class TRACE
    {
    public:
      static bool bVBlong(void);
      static bool bVerbose(Stz* szVBtag);
      static bool bVerbose(Stz* szVBtag1, Stz* szVBtag2);
    };

    class WORD
    {
    public:
      static void vDisassemble(Word* pWord, DicStack* pDicStack,
                               CodeCell* aInxPtr=nullptr);
      static Word* pFind(Stz* szWordNm);
      static void vListContents(Word* pWord, i8 i_Depth);
    };


  public:
    static Stz*  szVersionString(void);
    static i8    iVersionNumber(void);

    static Stz*  szFormatString(void);
    static i8    iFormatNumber(void);

    // args
    static bool bDebugIndex(void);
    static bool bDebugMode(void);
    static bool bDeveloper(void);
    static Stz* szCommand(void);
    static i8   nDummyLoop(void);
    static i8   nDummySleep(void);
    static i8   nDummyThreads(void);
    static Stz* szInitial(void);
    static i8   nMaxLoadErrors(void);
    static i8   nMaxReadyLP(void);
    static bool bMunged(void);
    static Stz* szRegionName(void);
    static i8   iRegionSize(void);
    static bool bRLUDiary(void);
    static bool bRLUTimer(void);
    static bool bSethScript(void);
    static i8   nThreads(void);

    static TagSet* pDbgTagSet(void);
    static bool    bDbgTag(Stz* szTagNm);

    static Stz* szLogRegion(void);
    static Stz* szLogCsvFile(void);
    static Stz* szLogTxtFile(void);
    
    //
    static Sessions* pSessions(void);
    static bool bBootError(void);
    static i8   nBuckets(void);
    static BufferPool* pBufferPool(void);
    static ClassFlags* pClassFlags(void);
    static void vDeleteOperand(Operand* pOperand);
    static MasterLock* pDicLock(void);
    static i8   iElapsedSsec(void);
    static i8   iEpoch(void);
    static Format* pFmt(void);
    static Grid* pGrid(void);
    static i8   nDefinedNodes(void);

    // newcode
    static std::list<Node*>::const_iterator itBeginNodes(void);
    static std::list<Node*>::const_iterator itEndNodes(void);

    static JobList* pDemonJobs(void);
    static Heaps* pHeaps(void);
    static i8   nIndexEntries(void);
    static JobList* pLocalJobs(void);
    static Locks* pLocks(void);
    static bool bLogging(void);
    static i8   nLvlLevels(void);
    static i8   iLvlRatio(void);
    static bool bMetadata(void);
  
    static i8   iNextEpoch(void);
    static i8   iNextTxId(void);
    static Stz* szNodeNm(void);
    static JobList* pOriginatingJobs(void);
    static JobList* pParticipatingJobs(void);
    static JobList* pRetiredJobs(void);
    static i8   iPid(void);
    static void vProcessExit(void);
    static i8   iSystemUsec(void);
    static i8   iUserUsec(void);
    static i8   iResidentSize(void);
    static bool bProgress(void);
    static Schedule* pSchedule(void);
    static Resources* pResources(void);
    static Word* pConstantRoot(void);
    static void vRootConstant(Word* pConstantRoot);
    static Word* pWordRoot(void);
    static void vRootWord(Word* pWordRoot);
    static Statistics* pStatistics(void);
    static Steps* pSteps(void);
    static PoolBuffer* pTakenBuffers(void);
    static i8   nTakenBytes(void);
    static MasterLock* pTccLock(void);
    static Word* pTenants(void);
    static i8   iThisNodeNo(void);
    static void vPutHeartbeat(i8 iNode, const Heartbeat& xHeartbeat);
    static bool bPutHeartbeat(Stz* pszNodeName, const Heartbeat& xHeartbeat);
    static Heartbeat xGetHeartbeat(i8 iNode);
    static Heartbeat xGetHeartbeat(Stz* pszNodeName);
    static void vRemoveNode(Stz* pszName);
    static void vThreadBegins(void);
    static i8   nThreadsBegun(void);
    static bool bTracepoint(void);
    static TxSet* pTxSet(void);
    static void vSendNode(Op* pOp, i8 i_SendNode);

    //
    static void vAckWaitingTask(Task* pTaskWaiting,
                                Task* pTaskAcking,
                                i8& nAcksWereRemaining);
    static void vAddConstant(Stz* szName,Stz* szValue);
    static Queue* pOutputQueue(Session* pSession);
    static void vAttachMap(Header* aHeader);
    static void vBootError(bool bBootError);
    static void vDropTaskLock(Task* pTask, Stz* szFunc);
    static void vGrabTaskLock(Task* pTask, Stz* szFunc);
    static void vIndexEntries(i8 nIndexEntries);
    static void vNextTempName(s32& sName);
    static void vSetAcksRmg(Task* pTask, i8 nAcks);
    static void vSteps(Steps* pSteps);
    static bool bTaskIsLocked(Task* pTask);
    static void vTenants(Word* pTenants);

    //
    static Task* pExecutingTask(Job* pJob);
    static void vPrintArguments(void);
    static void vPrintAttachMap(void);
    static void vPrintStartupInfo(void);
    static i8   nRndSkipLevel(void);
    static t8   tThisTLSPespective(void);
    static LogicUnit* pThisTLSLogicUnit(void);
    static Task* pThisTLSTask(void);
    static Thread* pThisTLSThread(void);
    static i8    iThisTLSThreadNo(void);
    static Tx* pThisTLSTx(void);
    static PsxThd* pThisTLSPsxThd(void);

  
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static void vAppendJobTags(Job* pJob, TagSet* pTagSet);
    static bool bAttached(void);
    static i8   iAMTimePoint(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static void vBootCheck(void);
    static Stz* szBuild(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static void vCopyrightText(s256& sCopyRight);
    static Crv* aCrv(DataBuffer* pDataBuffer, i8 iItem);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static void vDumpApi(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static eCN eJobClass(Job* pJob);
    static Stz* szJobNm(Job* pJob);
    static Session* pJobSession(Job* pJob);
    static TagSet* pJobTags(Job* pJob);
    static bool bJobWasReceived(Job* pJob);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    static void vFilePrint(File* pFile, Stz* szFormat,...);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static void vJobContentDump(Job* pJob);
    static void vJobDelete(Job** ppJob, Object* pDeleter, Stz* szFunc);
    static void vJobObjectName(Job* pJob, Stz* pObjectName);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static bool bInBufferPool(Object* pObject);
    static InternalTables* pInternalTables(void);
        
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static void vNapMsec(i8 nMilliSeconds, eCN eCallingClass);
    static void vNapUsec(i8 nMicroSeconds, eCN eCallingClass);
    static Operand* pDuplicateOperand(Operand* pOperand);
        
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static Stz* pOperandName(Operand* pOperand);
    static i8   nOperandsArg(void);
    static void vOperandsArg(i8 nOperands);
    static void vOperandsDec(void);
    static i8   nOperandsGet(void);
    static void vOperandsInc(void);
    static i8   iOperandZkKind(Operand* pOperand);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static bool bParseCmd(TokenVec* pTknVec, TagSet* pExtraTagSet);
    static Job* pPipelineJob(void);
    static void vPoolAnalyze(void);
    static void vPrintGlobals(void);
    static PtnMap* pPtnMap(void);
    static BucketMap_dyn* pBucketMap(void);

    static i8 iQueryNoInc(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static i8   iRdTickingDevice(void);
    static void vReadApiIndex(Index* pIndex);
    static Index* pReadApiIndex(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    static RU_Cpu* pRU_Cpu(void);
    static RU_Memory* pRU_Memory(void);
    static void vAllocInc(RU_Memory* pRU_Memory, eCN eAllocClass);
    static void vFreeInc(RU_Memory* pRU_Memory, eCN eAllocClass);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static Session* pSessionGet(i8 i_Origin,  i8 i_Session);
    
    static bool bSpawnTask_session(Job* pJob);
    
    static void vScheduleNextPhase(Job* pJob, Task* pActiviating2PcTask);
    static void vSpawnTaskDedicated_job(Job* pJob);
    static void vSpawnTaskDedicated_op(Op* pOp);
    static void vSpawnTask_job(Job* pJob);
    static void vSpawnTask_op(Op* pOp);
    static void vSpawnTask_receivedJob(Job* pJob);
    static void vSpawnTask_demonJob(Job* pJob);
    static Stz* szSummary(s256& sSummary);
    static Stz* szSummaryDir(s256& sSummary, Stz* szNote);
    static bool bSupervisorHalt(void);
    static void vSupervisorHalt(bool bHalt);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    static Table* pTableFind(Stz* szTableNm);
    static StreamDef* pStreamDefFind(Stz* szTableNm);

    static i8   iTaskAcksRmg_assert_only_(Task* pTask);
    static void vTaskDump(void);
    static void vTaskDump(Task* pTask);
    static Job* pTaskJob(void);
    static Job* pTaskJob(Task* pTask);
    static void vTaskMutexLock(Task* pTask);
    static void vTaskMutexSignal(Task* pTask);
    static void vTaskMutexUnlock(Task* pTask);
    static void vTaskMutexWait(Task* pTask);
    static void vTaskFullTrace(Task* pTask);
    static i8   iTaskQueue(Task* pTask);
    static LogicUnit* pTaskOpLogicUnit(Task* pTask);
    static LogicUnit* pTaskLogicUnit(Task* pTask);
    static i8   iTaskState(Task* pTask);

     // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    static HeapBuffer* pToHeapBuffer(i8 iBufferNo);
    static Object* pToLargeObect(i8 iBufferNo);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static void vWelcomeText(s256& sWelcome);
    static void vWordSeeDoes(Word* pDoesWord, CodeCell* aFip,
                             DicStack* pDicStack, CodeCell* aInxPtr=nullptr);
    static void vWriteLog(void);
    static void vWriteTrace_vargs(Stz* szFormat, va_list xArgs);
    static void vWriteTrace_vargs(File* pFile, Stz* aFormat, va_list xArgs);
    static void vWriteTrace(Stz* szFormat, ...);

  
  
    static Stz* szEnumText(i8 i_EnumGroup, i8 i_Enum);
  
  
    static bool bFormatSetFromTagSet(Format* pFormat, TagSet* pTagSet);
  
  
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // job interface

  public:
    static void vJob_AddDemon(Job* pJob);
    static void vJob_AddLocal(Job* pJob);
    static void vJob_AddOriginating(Job* pJob);
    static void vJob_AddParticipating(Job* pJob);
    static void vJob_AddRetired(Job* pJob);
    static void vJob_DumpDemon(void);
    static void vJob_DumpLocal(void);
    static void vJob_DumpOriginating(void);
    static void vJob_DumpParticipating(void);
    static Job* pJob_FindOriginating(JobId tJobId);
    static Job* pJob_FindParticipating(JobId tJobId);
    static void vJob_RemoveLocal(Job* pJob);
    static void vJob_RemoveOriginating(Job* pJob);
    static void vJob_RemoveParticipating(Job* pJob);
    static void vJob_RemoveRetired(Job* pJob);

  };

}

void where(void);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
