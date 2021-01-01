/*============================================================================*/
/* AM.hpp -- AttachMap helper                                                 */
/*============================================================================*/
#pragma once

//!                    ** Do not include BASE.h **

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
#include "eQA.h"
#include "eRA.h"
#include "eSL.h"
#include "eTA.h"
#include "eVA.h"
#include "eXA.h"
#include "eYA.h"

namespace BASE { class File;  class PsxThd; class Resource; class ResourceTracker;}
namespace BASE { class Operand;                                  }
namespace BASE { class Dic;             class DicStack;
                 class Word;              }
namespace BASE             { class Task;                                     }


namespace STORE          { class Session;         class SessionState;      }


namespace STORE    { class Crw;             class CrwBuffer;
                              class RowCompactor;    class RowInflator;
                              class HeadLine;        class Encoding_v2;
                     class SP_Encoding_v2;
                      class Packing_v2;
                              class OverLine;        class TailLine;          }
namespace STORE       { class DataBuffer;      class CsvBuffer;
                              class RowBuffer;       class RowBuffer;         }


namespace STORE    { class Dynamo;                                   }
namespace STORE       { class LogicUnit;                                }

namespace STORE             { class Grid;            class Node;              }

namespace STORE          { class EntBuffer;       class IndexTree;         }
namespace STORE          { class InfoSetBase;                              }
namespace STORE         { class SessionArray;                             }
namespace STORE              { class Job;
                              class JobList;         class Job_CreateTable;
                              class Job_CreateIndex; class XmitOperand;
                              class Job_Distributed;                          }
namespace STORE           { class Log;             class LogEntry;          }
namespace STORE          { class Table;           class IdxColVec;
                              class Index;
                              class BucketMap;       class PtnMap;            }
namespace STORE      {
                              class PtnStream;       class TablePtn;          }
namespace STORE         { class Xmits;           class Op;
                              class Pipeline;        class Stage;
                              class StageVec;        class Stream;            }
namespace STORE     { class OpSpec;          class PipelineSpec;
                              class StageSpec;                                }
namespace STORE             { class Planner;                                  }
namespace STORE   { class Crv;                                      }
namespace STORE              { class Row;                                      }
namespace STORE             { class Step;            class StepTiming;        }
namespace STORE     { class AttachMap;       class Engine;            }


namespace STORE           { class Scheduler;       class Thread;
                              class Queues;                                   }
namespace STORE      { class Tx;              class TxSet;             }


using namespace STORE;



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace OBLIQUE
{
  class AM
  {
  public:
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
    static SessionArray* pSessionArray(void);
    static bool bBootError(void);
    static i8   nBuckets(void);
    static BufferPool* pBufferPool(void);
    static EnumFlags* pClassFlags(void);
    static Lock* pDdlLock(void);
    static void vDeleteOperand(Operand* pOperand);
    static Lock* pDicLock(void);
    static i8   iElapsedSsec(void);
    static i8   iEpoch(void);
    static Format* pFmt(void);
    static Grid* pGrid(void);
    static i8   nBootNodes(void);
    static i8   nConnectedNodes(void);
    static i8   nDefinedNodes(void);
    static i8   nNodes(void);
    static JobList* pDemonJobs(void);
    static bool bHasStarted(void);
    static HeapMem* pHeapMem(void);
    static i8   nIndexEntries(void);
    static Article* pLexiconArticle(void);
    static JobList* pLocalJobs(void);
    static LockTracker* pLockTracker(void);
    static bool bLogging(void);
    static i8   nLvlLevels(void);
    static i8   iLvlRatio(void);
    static bool bMetadata(void);
    static i8   iNextDiagram(void);
    static i8   iNextEpoch(void);
    static i8   iNextJobId(void);
    static i8   iNextLockId(void);
    static i8   iNextOpId(void);
    static i8   iNextPlanId(void);
    static i8   iNextPromptId(void);
    static i8   iNextRequest(void);
    static i8   iNextResourceId(void);
    static i8   iNextSessionId(void);
    static i8   iNextStreamId(void);
    static i8   iNextTaskId(void);
    static i8   iNextThreadId(void);
    static i8   iNextTxId(void);
    static Stz* szNodeNm(void);
    static JobList* pOriginatingJobs(void);
    static JobList* pParticipatingJobs(void);
    static i8   iPid(void);
    static void vProcessExit(void);
    static bool bProgress(void);
    static i8   nPtnsInGrid(void);
    static Queues* pQueues(void);
    static ResourceTracker* pResourceTracker(void);
    static Article* pRootArticle(void);
    static Word* pRootConstant(void);
    static void vRootConstant(Word* pRootConstant);
    static Dic* pRootDic(void);
    static Word* pRootWord(void);
    static void vRootWord(Word* pRootWord);
    static Statistics* pStatistics(void);
    static StepTiming* pStepTiming(void);
    static PoolBuffer* pTakenBuffers(void);
    static i8   nTakenBytes(void);
    static Lock* pTccLock(void);
    static Word* pTenants(void);
    static i8   iThisNodeNo(void);
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
    static Queue* pBufferQueue(Job* pJob);
    static void vAttachMap(Header* aHeader);
    static void vBootError(bool bBootError);
    static void vDropTaskLock(Task* pTask);
    static void vGrabTaskLock(Task* pTask);
    static void vIndexEntries(i8 nIndexEntries);
    static void vNextTempName(s32& sName);
    static void vSetAcksRmg(Task* pTask, i8 nAcks);
    static void vStepTiming(StepTiming* pStepTiming);
    static void vTakenBuffers(PoolBuffer* pTakenBuffers);
    static void vTakeBytes(i8 nTakeBytes);
    static void vTakenBytes(i8 nTakenBytes);
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
    static void vIncRowsInvalid(Job* pJob);
    static void vIncRowsProcessed(Job* pJob);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static bool bInBufferPool(Object* pObject);
  
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
    static void vLogQue(eSL, Queues*    pQue, eQA eQueAct, Stz* szFormat, ...);
    static void vLogRes(eSL, Resource*  pRes, eRA eResAct, Stz* szFormat, ...);
    static void vLogSvc(eSL, Job*       pJob, eVA eSvcAct, Stz* szFormat, ...);
    static void vLogSsn(eSL, Session*   pSsn, eAA eApiAct, Stz* szFormat, ...);
    static void vLogSys(eSL, AttachMap* pSys, eYA eSysAct, Stz* szFormat, ...);
    static void vLogThd(eSL, Thread*    pThd, eHA eThdAct, Stz* szFormat, ...);
    static void vLogTsk(eSL, Task*      pTsk, eTA eTskAct, Stz* szFormat, ...);
    static void vLogTxn(eSL, Tx*        pTxn, eXA eTxnAct, Stz* szFormat, ...);
  
    static void vLogOpenByApi(void);
    static void vLogOpenBySE(bool bStarting);
    static void vLogWriteCsv(bool bShortForm);
    static bool bLogEntryExists(i8 i_Entry);
    static i8   iLogEntryPrev(i8 i_Entry);
    static void vLogEntrySprint(s4K& sString, i8 i_Entry);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    static void vNapMsec(i8 nMilliSeconds, eCN eCallingClass);
    static void vNapUsec(i8 nMicroSeconds, eCN eCallingClass);
    static void vDeleteXmitOperand(XmitOperand* pXmitOperand);
    static Operand* pDuplicateOperand(Operand* pOperand);
    static XmitOperand* pNewXmitOperand(Job* pParentJob, Operand* pOperand,
                                        Op* pOp, i8 iNde, tPN tPtnNo);
    
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
    static i8   iRdTimePoint(void);
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
    static void vStateDiagram(s256& sDiagram, eCN eJobClass);
    static Stz* szSummary(s256& sSummary);
    static Stz* szSummaryDir(s256& sSummary, Stz* szNote);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    static Table* pTableFind(Stz* szTableNm);

    static i8   iTaskAcksRmg_assert_only_(Task* pTask);
    static DicStack* pTaskDicStack(void);
    static void vTaskDump(void);
    static void vTaskDump(Task* pTask);
    static Job* pTaskJob(void);
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
    static void vWordSeeDoes(Word* pWord, Word* pDoesWord, ForthCell* aFip, DicStack* pDicStack);
    static void vWriteLog(void);
    static void vWriteTrace_vargs(eYA eSysAct, Stz* szFormat, va_list xArgs);
    static void vWriteTrace_vargs(File* pFile, eYA eSysAct, Stz* aFormat, va_list xArgs);
    static void vWriteTrace(eYA eSysAct, Stz* szFormat, ...);

  
  
    static Stz* szEnumText(i8 i_EnumGroup, i8 i_Enum);
  
  
    static bool bFormatSetFromTagSet(Format* pFormat, TagSet* pTagSet);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
