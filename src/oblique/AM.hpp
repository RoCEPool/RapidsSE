/*============================================================================*/
/* AM.hpp -- AttachMap helper                                                 */
/*============================================================================*/
#pragma once

//!                    ** Do not include BASE.h **
#include <list>
#include <iterator>

#include <Heartbeat.hpp>
#include "eAA.h"
#include "eDA.h"
#include "eEC.h"
#include "eGA.h"
#include "eHA.h"
#include "eJA.h"
#include "eLA.h"
#include "eMA.h"
#include "eOA.h"
#include "ePA.h"
#include "ePX.h"
#include "eQA.h"
#include "eRA.h"
#include "eSL.h"
#include "eTA.h"
#include "eVA.h"
#include "eXA.h"
#include "eYA.h"

namespace OBLIQUE {
  class AM;
}

using namespace OBLIQUE;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace BASE {

  class Article;
         
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

  class Lock;
  class Locks;
  class Log;
  class LogEntry;
  class Loggable;

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

  class ReqTuple;
  class Resource;
  class Resources;

  class Schedule;
  class Scheduler;
  class Statistics;

  class TagSet;
  class Task;
  class Thread;
  class ThreadVec;
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

  class BucketMap;

  class Crv;
  class Crw;
  class CrwBuffer;
  class CrwPacker;

  class Dynamo;

  class Encoding;
  class Engine;
  class EntBuffer;

  class Grid;

  class HeadLine;

  class IdxColVec;
  class Index;
  class IndexTree;
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
  
  class WriteBehind;
  class XmitOperand;
  
  class Xmits;

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
  
    class DEMON
    {
    public:
      static void vPipeTo(Stz* szDemonNm, RowBuffer* pRowBuffer);
    };
  
    class JOB
    {
    public:
      static bool bHasTag(Job* pJob, Stz* szTagNm);
    };
  
    class LOG
    {
    public:
      static void vMemoryAllocation(Object* pObject);
      static void vMemoryFree(Object* pObject);
      static void vResourceUse(Object* pObject, i8 iAction, i8 nBytes);
    };

    class SYSTEM
    {
    public:
      static i8 iElapsedTns(void);
    };

    class TASK
    {
    public:
      static DicStack* pDicStack(void);
      static bool bLogMemory(void);
    };
  
    class THREAD
    {
    public:
      static ThreadVec* pDedicatedThreadVec(void);
      static void vDeleteThread(Thread* pThread);
      static ThreadVec* pGeneralThreadVec(void);
      static Thread* pNewThread(void);
      static WriteBehind* pWriteBehind(void);
      static WriteBehind* pWriteBehind(Thread* pThread);
    
    };
  
    class WORD
    {
    public:
      static Word* pFind(Stz* szWordNm);
    };


  public:
    static Stz*  szVersionString(void);
    static i8    iVersionNumber(void);

    static Stz*  szFormatString(void);
    static i8    iFormatNumber(void);

    // args
    static bool bDebugIndex(void);
    static bool bDebugMode(void);
    static i8   nDegOfPar(void);
    static bool bDeveloper(void);
    static Stz* szCommand(void);
    static i8   nDummyLoop(void);
    static i8   nDummySleep(void);
    static i8   nDummyThreads(void);
    static Stz* szInitial(void);
    static bool bLog(void);
    static i8   nMaxLoadErrors(void);
    static i8   nMaxReadyLP(void);
    static bool bMunged(void);
    static Stz* szRegionName(void);
    static i8   iRegionSize(void);
    static bool bRLUDiary(void);
    static bool bRLUTimer(void);
    static bool bSethScript(void);
    static i8   nPtnsPerTable(void);
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
    static MasterLock* pDdlLock(void);
    static void vDeleteOperand(Operand* pOperand);
    static MasterLock* pDicLock(void);
    static i8   iElapsedSsec(void);
    static i8   iEpoch(void);
    static Format* pFmt(void);
    static Grid* pGrid(void);
    static i8   nBootNodes(void);
    static i8   nConnectedNodes(void);
    static i8   nDefinedNodes(void);
    static i8   nNodes(void);

    // newcode
    static std::list<Node*>::const_iterator itBeginNodes(void);
    static std::list<Node*>::const_iterator itEndNodes(void);

    static JobList* pDemonJobs(void);
    static bool bHasStarted(void);
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
    static i8   iPid(void);
    static void vProcessExit(void);
    static bool bProgress(void);
    static i8   nPtnsInGrid(void);
    static Schedule* pSchedule(void);
    static Resources* pResources(void);
    static Article* pArcticleRoot(void);
    static Word* pConstantRoot(void);
    static void vRootConstant(Word* pConstantRoot);
    static Dic* pDicRoot(void);
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
    static void vPutHeartbeat(Stz* pszNodeName, const Heartbeat& xHeartbeat);
    static Heartbeat xGetHeartbeat(i8 iNode);
    static Heartbeat xGetHeartbeat(Stz* pszNodeName);
    static void vThreadBegins(void);
    static i8   nThreadsBegun(void);
    static bool bTracepoint(void);
    static TxSet* pTxSet(void);
    static void vXmitNode(Op* pOp, i8 i_XmitNde);
    static bool bVerbose(void);
    static bool bVerboseProgress(void);

    //
    static void vAckWaitingTask(Task* pTaskWaiting, Task* pTaskAcking);
    static void vAddConstant(Stz* szName,Stz* szValue);
    static Queue* pOutputQueue(Session* pSession);
    static void vAttachMap(Header* aHeader);
    static void vBootError(bool bBootError);
    static void vDropTaskLock(Task* pTask);
    static void vGrabTaskLock(Task* pTask);
    static void vIndexEntries(i8 nIndexEntries);
    static void vNextTempName(s32& sName);
    static void vSetAcksRmg(Task* pTask, i8 nAcks);
    static void vSteps(Steps* pSteps);
    static bool bTaskIsLocked(Task* pTask);
    static Task* pTaskNew(Job* pJob);
    static void vTenants(Word* pTenants);
    static void vTriggerBidingTask(Task* pTaskBiding, Task* pTriggeringTask);

    //
    static Task* pExecutingTask(Job* pJob);
    static bool bJobIsForeign(i8 iJobId);
    static void vPrintArguments(void);
    static void vPrintAttachMap(void);
    static void vPrintStartupInfo(void);
    static i8   nRndSkipLevel(void);
    static Job* pThisTLSJob(void);
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
    static Stz* szJobOpNm(Job* pJob);
    static Stz* szJobSeed(Job* pJob);
    static Session* pJobSession(Job* pJob);
    static TagSet* pJobTags(Job* pJob);
    static bool bJobWasReceived(Job* pJob);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    static void vFilePrint(File* pFile, Stz* szFormat,...);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static i8   iJobUniqueId(Job* pJob);
    static void vJobContentDump(Job* pJob);
    static void vJobDelete(Job** ppJob);
    static void vJobInitExeContext(Job* pJob);
    static void vJobObjectName(Job* pJob, Stz* pObjectName);

    static bool bIsOperandOoO(Operand* pOperand);

    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static bool bInBufferPool(Object* pObject);
    static InternalTables* pInternalTables(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static void vLog(TokenVec* pTknVec);
    static void vLogAll(void);
    static void vLogArguments(void);
    static void vLogHow(i8 iRequestId, i8 iDetail);
  
    static void vLogFyi(Stz* szFormat, ...);
    static void vLogText(i8 iId, Stz* szText);
  
    static void vLogDdl(eSL, Job*       pJob, eDA eDdlAct, Stz* szFormat, ...);
    static void vLogJob(eSL, Job*       pJob, eJA eJobAct, Stz* szFormat, ...);
    static void vLogLck(eSL, Lock*      pLck, eLA eLckAct, Stz* szFormat, ...);
    static void vLogLog(eSL, Log*       pLog, eGA eLogAct, Stz* szFormat, ...);
    static void vLogOpr(eSL, Op*        pOpr, eOA eOpAct,  Stz* szFormat, ...);
    static void vLogMgt(eSL, Job*       pJob, eMA eMgtAct, Stz* szFormat, ...);
    static void vLogPrm(eSL, SePrompt*  pPrm, ePA ePrmAct, Stz* szFormat, ...);
    static void vLogQue(eSL, Schedule*    pQue, eQA eQueAct, Stz* szFormat, ...);
    static void vLogRes(eSL, Resource*  pRes, eRA eResAct, Stz* szFormat, ...);
    static void vLogSvc(eSL, Job*       pJob, eVA eSvcAct, Stz* szFormat, ...);
    static void vLogSsn(eSL, Session*   pSsn, eAA eApiAct, Stz* szFormat, ...);
    static void vLogSys(eSL, AttachMap* pSys, eYA eSysAct, Stz* szFormat, ...);
    static void vLogThd(eSL, Thread*    pThd, eHA eThdAct, Stz* szFormat, ...);
    static void vLogTsk(eSL, Task*      pTsk, eTA eTskAct, Stz* szFormat, ...);
    static void vLogTxn(eSL, Tx*        pTxn, eXA eTxnAct, Stz* szFormat, ...);
  
    static void vLogTI(i8 iTable, ...);

    static void vLogOpenByApi(void);
    static void vLogOpenBySE(bool bStarting);
    static void vLogWriteCsv(bool bShortForm);
    static bool bLogEntryExists(i8 i_Entry);
    static i8   iLogEntryPrev(i8 i_Entry);
    static void vLogEntrySprint(s4K& sString, i8 i_Entry);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static void vNapMsec(i8 nMilliSeconds, eCN eCallingClass);
    static void vNapUsec(i8 nMicroSeconds, eCN eCallingClass);
    static i8   iNextObjectNo(eCN eClassNo);
    static void vDeleteXmitOperand(XmitOperand* pXmitOperand);
    static Operand* pDuplicateOperand(Operand* pOperand);
    static XmitOperand* pNewXmitOperand(Job* pParentJob, Operand* pOperand,
                                        ePX eCrossTopo, i8 iNextStage, i8 iNextOp,
                                        i8 iNde, tPN tPtnNo);
    
    static Job* pNewJobOp(Op* pOp, ReqTuple* pReqTuple);
    
    static Stz* szJobName(Job* pJob);
    
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
    static bool bPassesLowMem(Job* pJob);
    static Job* pPipelineJob(void);
    static void vPoolAnalyze(void);
    static eEC ePopulateJob(Job* pJob, TagSet* pTagSet);
    static void vPrintGlobals(void);
    static PtnMap* pPtnMap(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static i8   iRdTickingDevice(void);
    static void vReadApiIndex(Index* pIndex);
    static Index* pReadApiIndex(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static ReqTuple* aReqTuple_tls(void);
  
    static RU_Cpu* pRU_Cpu(void);
    static RU_Memory* pRU_Memory(void);
    static void vAllocInc(RU_Memory* pRU_Memory, eCN eAllocClass);
    static void vFreeInc(RU_Memory* pRU_Memory, eCN eAllocClass);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static void vSend(Task* pTask, Node* pSendNode, XmitOperand* pXmitOperand);
    static Session* pSessionGet(ReqTuple* aReqTuple);
    static void vSetOperand(XmitOperand* pXmitOperand, Operand* pOperand);
    
    static bool bSpawnJob_connector(Job* pJob);
    static bool bSpawnJob_session(Job* pJob);
    
    static void vScheduleNextPhase(Job* pJob, Task* pActiviating2PcTask);
    static void vSpawnJob_dedicated(Job* pJob);
    static void vSpawnJob_op(Job* pJob);    
    static void vSpawnJob_received(Job* pJob);
    static void vSpawnJob_service(Job* pJob);
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
    static void vTaskOnExceptionTrace(Task* pTask);
    static i8   iTaskQueue(Task* pTask);
    static LogicUnit* pTaskLogicUnit(Task* pTask);
    static i8   iTaskState(Task* pTask);
 
     // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    static HeapBuffer* pToHeapBuffer(i8 iBufferNo);
    static Object* pToLargeObect(i8 iBufferNo);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static void vWelcomeText(s256& sWelcome);
    static void vWordSee(Word* pWord, DicStack* pDicStack);
    static void vWordSeeDoes(Word* pDoesWord, CodeCell* aFip, DicStack* pDicStack);
    static void vWriteLog(void);
    static void vWriteTrace_vargs(eYA eSysAct, Stz* szFormat, va_list xArgs);
    static void vWriteTrace_vargs(File* pFile, eYA eSysAct, Stz* aFormat, va_list xArgs);
    static void vWriteTrace(eYA eSysAct, Stz* szFormat, ...);

  
  
    static Stz* szEnumText(i8 i_EnumGroup, i8 i_Enum);
  
  
    static bool bFormatSetFromTagSet(Format* pFormat, TagSet* pTagSet);
  
  
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // job interface

  public:
    static void vJob_AddDemon(Job* pJob);
    static void vJob_AddLocal(Job* pJob);
    static void vJob_AddOriginating(Job* pJob);
    static void vJob_AddParticipating(Job* pJob);
    static void vJob_DumpDemon(void);
    static void vJob_DumpLocal(void);
    static void vJob_DumpOriginating(void);
    static void vJob_DumpParticipating(void);
    static Job* pJob_FindOriginating(i8 iJobId);
    static Job* pJob_FindParticipating(i8 iJobId);
    static bool bJob_RemoveLocal(Job* pJob, bool bDump=false);
    static bool bJob_RemoveOriginating(Job* pJob, bool bDump=false);
    static bool bJob_RemoveParticipating(Job* pJob, bool bDump=false);
  
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
