/*============================================================================*/
/* StorageEngine.h -- StorageEngine namespace                                 */
/*============================================================================*/
#pragma once

#include "Base.h"

//------------------------------------------------------------------------------

namespace StorageEngine
{
  void vCheckAllAssumptions(void);
  void vCheckAssumptions(void);
  void vSetSignalHandlers(void);
}



//------------------------------------------------------------------------------

static const i8 C_DEF_LVL_LEVELS = 3;
static const i8 C_DEF_LVL_RATIO  = 3; // this means 1/2^3 i.e. reducing by 8




// tags
static Stz* C_TAGSET_NAME    = "<aTagSetNm>"_Stz;
 
static Stz* C_TAG_BG         = "<bg>"_Stz;
static Stz* C_TAG_BYPROMPT   = "<byPrompt>"_Stz;
static Stz* C_TAG_CMD        = "<cmd>"_Stz;
static Stz* C_TAG_CMDFILE    = "<cmdFile>"_Stz;
static Stz* C_TAG_CMDLINE    = "<cmdLine>"_Stz;
static Stz* C_TAG_DIARY      = "<diary>"_Stz;
static Stz* C_TAG_DIARYDIR   = "<diaryDir>"_Stz;
static Stz* C_TAG_DIVISION   = "<division>"_Stz;
static Stz* C_TAG_FLOW       = "<flow>"_Stz;
static Stz* C_TAG_FORCELOG   = "<forceLog>"_Stz;
static Stz* C_TAG_FREEPCT    = "<freePct>"_Stz;
static Stz* C_TAG_DISTID     = "<distId>"_Stz;
static Stz* C_TAG_GRID       = "<grid>"_Stz;
static Stz* C_TAG_HOST       = "<host>"_Stz;
static Stz* C_TAG_JOBKIND    = "<jobKind>"_Stz;
static Stz* C_TAG_JOBNM      = "<jobNm>"_Stz;
static Stz* C_TAG_LIFE       = "<life>"_Stz;
static Stz* C_TAG_LOWMEMORY  = "<lowMemory>"_Stz;
static Stz* C_TAG_NEXTOP     = "<nextOp>"_Stz;
static Stz* C_TAG_NEXTSTAGE  = "<nextStage>"_Stz;
static Stz* C_TAG_NODE       = "<node>"_Stz;
static Stz* C_TAG_NOTE       = "<note>"_Stz;
static Stz* C_TAG_ORIGIN     = "<origin>"_Stz;
static Stz* C_TAG_PATH       = "<path>"_Stz;
static Stz* C_TAG_PARAM0     = "<param0>"_Stz;
static Stz* C_TAG_PARAM1     = "<param1>"_Stz;
static Stz* C_TAG_PARAM2     = "<param2>"_Stz;
static Stz* C_TAG_PARAM3     = "<param3>"_Stz;
static Stz* C_TAG_PARAM4     = "<param4>"_Stz;
static Stz* C_TAG_REQUEST    = "<request>"_Stz;
static Stz* C_TAG_REQNO      = "<reqNo>"_Stz;
static Stz* C_TAG_REQOF      = "<reqOf>"_Stz;
static Stz* C_TAG_NOWAIT     = "<noWait>"_Stz;
static Stz* C_TAG_PLPROTOCOL = "<plProtocol>"_Stz;
static Stz* C_TAG_SECTION    = "<section>"_Stz;
static Stz* C_TAG_SESSION    = "<session>"_Stz;
static Stz* C_TAG_SHM        = "<shm>"_Stz;
static Stz* C_TAG_STMT       = "<stmt>"_Stz;
static Stz* C_TAG_SUMMARY    = "<summary>"_Stz;
static Stz* C_TAG_TABLENM    = "<tableNm>"_Stz;
static Stz* C_TAG_TAGS       = "<tags>"_Stz;
static Stz* C_TAG_TIME       = "<time>"_Stz;
static Stz* C_TAG_TOPOLOGY   = "<topology>"_Stz;
static Stz* C_TAG_TRACEOP    = "<traceOp>"_Stz;
static Stz* C_TAG_WHEN       = "<when>"_Stz;
static Stz* C_TAG_XFER       = "<xfer>"_Stz;
static Stz* C_TAG_XMITKIND   = "<xmitKind>"_Stz;
static Stz* C_TAG_XMITS      = "<xmits>"_Stz;
 
static Stz* C_TAG_DBG_ACKS   = "<dbg_acks>"_Stz;

static Stz* C_TAG_LD_DATE_FORMAT          = "<date_format>"_Stz;
static Stz* C_TAG_LD_ENCLOSED_BY_LEADING  = "<enclosed_by_leading>"_Stz;
static Stz* C_TAG_LD_ENCLOSED_BY_TRAILING = "<enclosed_by_trailing>"_Stz;
static Stz* C_TAG_LD_ESCAPE_CHAR          = "<escape_char>"_Stz;
static Stz* C_TAG_LD_FIELD_DELIMITER      = "<field_delimiter>"_Stz;
static Stz* C_TAG_LD_MAX_ERRORS           = "<max_errors>"_Stz;
static Stz* C_TAG_LD_RECORD_TERMINATOR    = "<record_terminator>"_Stz;
static Stz* C_TAG_LD_RECORD_ENDS          = "<record_ends>"_Stz;

// job info tags
static Stz* C_TAG_SEEDCAN  = "<seedCan>"_Stz;
static Stz* C_TAG_SEEDEND  = "<seedEnd>"_Stz;
static Stz* C_TAG_SEEDEXE  = "<seedExe>"_Stz;
static Stz* C_TAG_SEEDRCV  = "<seedRcv>"_Stz;
static Stz* C_TAG_SEEDREQ  = "<seedReq>"_Stz;
static Stz* C_TAG_SEEDRUN  = "<seedRun>"_Stz;

// ASM Tags
static Stz* C_TAG_QUERY_NO = "<queryNo>"_Stz;
static Stz* C_TAG_SUBQUERY_NO = "<subQueryNo>"_Stz;

static Stz* C_TAG_SRC_TABLENM  = "<srcTableNm>"_Stz;
static Stz* C_TAG_SRC_INDEXNM  = "<srcIndexNm>"_Stz;
static Stz* C_TAG_ALT_TABLENM  = "<altTableNm>"_Stz;
static Stz* C_TAG_ALT_INDEXNM  = "<altIndexNm>"_Stz;

static Stz* C_TAG_COLUMN_SCALE  = "<columnScale>"_Stz;
static Stz* C_TAG_COLUMN_LENGTH = "<columnLength>"_Stz;
static Stz* C_TAG_COLUMN_TYPE   = "<columnType>"_Stz;
static Stz* C_TAG_COLUMN_NAME   = "<columnName>"_Stz;

static Stz* C_TAG_LOGICUNIT_HANDLER     = "<logicUnitHandler>"_Stz;
static Stz* C_TAG_CRW_PROJECT_ROUTINE   = "<crwProjectRoutine>"_Stz;
static Stz* C_TAG_CRW_FILTER_ROUTINE    = "<crwFilterRoutine>"_Stz;
static Stz* C_TAG_DATAPULSE_PROCESS_ROUTINE = "<dataPulseProcessRoutine>"_Stz;

static Stz* C_TAG_INDEX_RANGE_ROUTINE = "<indexRangeRoutine>"_Stz;

static Stz* C_TAG_SRC_EXISTS_ALTIDX     = "<srcExistsAltIdx>"_Stz;

static Stz* C_TAG_ADD_PTN_HASH          = "<addPtnHash>"_Stz;

static Stz* C_TAG_PROJECT_ITEMS         = "<projectItems>"_Stz;
static Stz* C_TAG_PROJECT_SRC_COLUMN    = "<projectSrcColumn>"_Stz;
static Stz* C_TAG_PROJECT_SRC_COLUMNS   = "<projectSrcColumns>"_Stz;

static Stz* C_TAG_LOCALS_COLUMN = "<localsColumn>"_Stz;
static Stz* C_TAG_SUM_COLUMN    = "<sumColumn>"_Stz;

static Stz* C_TAG_DST_COLUMN       = "<dstColumn>"_Stz;

static Stz* C_TAG_PREDICATE_COLUMNS     = "<predicateColumns>"_Stz;
static Stz* C_TAG_PREDICATE_SRC_COLUMN  = "<predicateSrcColumn>"_Stz;
static Stz* C_TAG_FILTERNO  = "<filterNo>"_Stz;

static Stz* C_TAG_LITERAL_TYPE  = "<literalType>"_Stz;
static Stz* C_TAG_LITERAL_VALUE = "<literalValue>"_Stz;
static Stz* C_TAG_RELATIONAL_OP = "<relationalOp>"_Stz;
static Stz* C_TAG_LBOUND_VALUE = "<lboundValue>"_Stz;
static Stz* C_TAG_UBOUND_VALUE = "<uboundValue>"_Stz;

static Stz* C_TAG_LOWER_BOUND = "<lowerBound>"_Stz;
static Stz* C_TAG_UPPER_BOUND = "<upperBound>"_Stz;


static const i8 C_TPCH_CUSTOMER =  150 * C_1K;
static const i8 C_TPCH_LINEITEM = 6000 * C_1K;
static const i8 C_TPCH_ORDERS   = 1500 * C_1K;
static const i8 C_TPCH_PART     =  200 * C_1K;
static const i8 C_TPCH_PARTSUPP =  800 * C_1K;
static const i8 C_TPCH_SUPPLIER =   10 * C_1K;

static const i8 C_TPCH_NATION   = 25;
static const i8 C_TPCH_REGION   = 5;


//------------------------------------------------------------------------------


#include "Args.hpp"
#include "Limits.hpp"
#include "Licence.hpp"


//------------------------------------------------------------------------------

#include "Format.hpp"

#include "DataChunk.hpp"
#include "RecordChunk.hpp"
#include "FileChunk.hpp"

#include "IngestChunk.hpp"
#include "IoChunk.hpp"
#include "IoCopy.hpp"
#include "LoadChunk.hpp"



#include "Operand.hpp"
#include "LinkedBuffer.hpp"
#include "Definition.hpp"
#include "OperandQueue.hpp"


//------------------------------------------------------------------------------

#include "Dic.hpp"
#include "DicStack.hpp"
#include "Local.hpp"
#include "LocalChain.hpp"
#include "Routine.hpp"
#include "TypedStack.hpp"

#include "Compiler.hpp"
#include "Forth.hpp"
#include "Word.hpp"

// -- -- -- -- -- -- -- --

#include "SP_Word.hpp"

//------------------------------------------------------------------------------

#include "ColPix.h"
#include "RowPix.h"
#include "Column.hpp"

#include "SN_Bcd.hpp"
#include "SN_Bol.hpp"
#include "SN_Dte.hpp"
#include "SN_Flt.hpp"
#include "SN_Int.hpp"
#include "SN_Ivl.hpp"
#include "SN_Tme.hpp"
#include "SN_Tsp.hpp"
#include "SN_Vch.hpp"

#include "SP_Bcd.hpp"
#include "SP_Bol.hpp"
#include "SP_Dte.hpp"
#include "SP_Flt.hpp"
#include "SP_Int.hpp"
#include "SP_Ivl.hpp"
#include "SP_Tme.hpp"
#include "SP_Tsp.hpp"
#include "SP_Vch.hpp"

#include "Literal.hpp"



//------------------------------------------------------------------------------

#include "BucketMap.hpp"
#include "PtnMap.hpp"

#include "StreamDef.hpp"

#include "IndexDef.hpp"

typedef VectorNL_t<CN_IDXVEC,Index,C_MAX_INDEXES> IdxVec;
typedef VectorNL_t<CN_TABLEPTNVEC,Arguments::TablePtn,C_MAX_TABLEPTNS>TablePtnVec;

#include "Table.hpp"

#include "Index.hpp"



#include "Folder.hpp"
#include "Schema.hpp"
#include "Catalog.hpp"
#include "Tenant.hpp"
#include "Tenants.hpp"



#include "Marker.hpp"




#include "Tx.hpp"
#include "TxSet.hpp"



//------------------------------------------------------------------------------

static const i8 C_WSP_ROWBUFFER = C_SIZE_LARGE_OBJECT - C_16KB; // ref NmlStr

#include "Ref.hpp"

#include "Row.hpp"
#include "NmlStr.hpp"
#include "Record.hpp"
typedef HV<Record,RowPix>       HV_Record;
#include "RowBox_t.hpp"


#include "RowSingle.hpp"

#include "CellSegment.hpp"
#include "CellSingle.hpp"

//------------------------------------------------------------------------------

typedef HV<RowSingle,StreamDef> HV_RowSingle;






//------------------------------------------------------------------------------

#include "DataBuffer.hpp"

#include "RowUte.hpp"

#include "RowBuffer.hpp"
#include "CsvMeta.hpp"
#include "CsvBuffer.hpp"

#include "HashSet.hpp"
#include "Pot.hpp"

#include "Reader.hpp"

#include "SP_DataBuffer.hpp"




#include "InfoSetBase.hpp"
#include "InfoSet.hpp"
#include "StreamSet.hpp"





#include "Pay.hpp"
#include "Key.hpp"
#include "NEO.h"
#include "Ent.hpp"
#include "Ent_flat.hpp"
#include "EntWrk.hpp"

#include "SP_Ent.hpp"
#include "SP_Key.hpp"
#include "SP_Ref.hpp"





#define STRx1(__uLow) (__uLow)            //   0B .. |1 * 16 =  16| ..  15B
#define STRx2(__uLow) ( 16 + 2 * __uLow)  //  16B .. |2 * 16 =  32| ..  47B
#define STRx4(__uLow) ( 48 + 4 * __uLow)  //  48B .. |4 * 16 =  64| .. 111B
#define STRx8(__uLow) (112 + 8 * __uLow)  // 112B .. |8 * 16 = 128| .. 239B

#define METACODE_FORM(__high,__low) ( (u1) ( (u1)__high << 4 | (u1)__low )  )

#define METACODE_HIGH(__uCode) ( __uCode >> 4 )
#define METACODE_LOW(__uCode) ( __uCode & 0x0F )

#define METACODE_SPLIT(__uCode,__iHigh,__iLow) \
  __iHigh = (__uCode >> 4); \
  __iLow  = (__uCode &  0x0F);

//------------------------------------------------------------------------------

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "OverLine.hpp"
#include "TailLine.hpp"
#include "TailStr.hpp"
#include "Crw.hpp"

//------------------------------------------------------------------------------

static const i8 C_CRW_HEADBYTES = 32;

static const i8 C_CRW_FIRSTHEAD  = 0x210;

static const i8 C_TRAY_ROWS = (C_SIZE_LARGE_OBJECT - C_1KB) / sizeof(HeadLine);

typedef    Nub_t <CN_ROWTRAY,HeadLine,OverLine,TailLine,StreamDef> RowNub;

typedef TrayNL_t <CN_ROWTRAY,HeadLine,OverLine,TailLine,StreamDef,
                  (C_TRAY_ROWS * sizeof(HeadLine))>               RowTray;

//------------------------------------------------------------------------------

#include "Encoding_v2.hpp"
#include "Encoding.hpp"
#include "Packing_v2.hpp"

#include "CrwBuffer.hpp"
#include "RowCompactor.hpp"
#include "RowInflator.hpp"

#include "SP_Encoding_v2.hpp"





#include "ToRow.hpp"
#include "ToNstRow.hpp"
#include "ToPtnRow.hpp"
#include "ToHshCrw.hpp"

#include "CrwToRow.hpp"




#include "SrtSet.hpp"
#include "SrtBuffer.hpp"
#include "QuickSort.hpp"
#include "BufferCursor.hpp"
#include "BufferSort.hpp"

#include "IntBuffer.hpp"





//#define TRACE_COMPACT(x) x EndDefine
#define TRACE_COMPACT(x) EndDefine

// - - - - - - - - - - - -

#include "KeyBuilder.hpp"

#include "SkipRoots.hpp"

//#define EB_TRACE_INSERT
//#define EB_LOCAL_VERIFY
//#define EB_SPLIT_RIGHT
#include "EntBuffer.hpp"


#include "EntBufSorter.hpp"

#include "IndexTree.hpp"
typedef VectorNL_t<CN_INDEXTREEVEC, IndexTree, C_MAX_INDEXES> IndexTreeVec;







#include "Crv.hpp"
#include "CrvBuffer.hpp"
#include "ReadApi.hpp"



//#define TRACE_UPSERT(smt) smt EndDefine
#define TRACE_UPSERT(smt) EndDefine

//------------------------------------------------------------------------------

#include "TablePtn.hpp"
#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "PtnStream.hpp"




#include "Node.hpp"
#include "Node_I.hpp"
#include "Grid.hpp"

#include "SP_Grid.hpp"
#include "SP_Node.hpp"




#include "Session.hpp"

#include "SP_Session.hpp"




//------------------------------------------------------------------------------

#include "Arg.hpp"
#include "RelOp.hpp"
#include "Predicate.hpp"
#include "PredicateVec.hpp"


#include "AccessPath.hpp"
typedef VectorNL_t<CN_PATHVEC,AccessPath,128> PathVec;

#include "Plan.hpp"
#include "Planner.hpp"


#include "OpSpec.hpp"
#include "StageSpec.hpp"
#include "PipelineSpec.hpp"



#include "Xmits.hpp"
#include "Op.hpp"
#include "Stream.hpp"
#include "Stage.hpp"
#include "Pipeline.hpp"



#include "Response.hpp"
typedef Vector_t<CN_RESPONSEVEC,Response,C_MAX_NODES> ResponseVec;

#include "Result.hpp"
#include "JobFilePath.hpp"
#include "Job.hpp"
#include "JobLog.hpp"
#include "CmdHelper.hpp"
#include "Job__Find.hpp"
#include "XmitOperand.hpp"
#include "JobList.hpp"
#include "SP_Job.hpp"

#include "Procedure.hpp"

#include "Dynamo.hpp"



//------------------------------------------------------------------------------


#include "graph/PlanConstants.hpp"
#include "graph/PlanNode.hpp"
#include "graph/PlanNodeList.hpp"
#include "graph/PlanNodeBuilderList.hpp"
#include "graph/PlanNodeBuilder.hpp"
#include "graph/RootNode.hpp"
#include "graph/DefinitionNode.hpp"
#include "graph/PrimitiveNode.hpp"
#include "graph/MutablePlanNode.hpp"
#include "graph/QueryNode.hpp"
#include "graph/MetadataNode.hpp"
#include "graph/StatisticsNode.hpp"
#include "graph/PlanDefinitionParser.hpp"

#include "graphTools/GraphNav.hpp"
#include "graphTools/GraphEditor.hpp"

#include "parser/stages/parser/ExprAtom.hpp"
#include "parser/stages/parser/ExprColumnRef.hpp"
#include "parser/stages/parser/ExprList.hpp"
#include "parser/stages/parser/TableRef.hpp"
#include "parser/stages/parser/SqlPlanner.hpp"

//------------------------------------------------------------------------------

#include "RluCommon.h"

#include "RluDiary.hpp"

#include "Device.hpp"
#include "DevIXT.hpp"
#include "DevROW.hpp"

#include "LogicTimer.hpp"

#include "LogicCompiler.hpp"
#include "Project.hpp"

#include "LogicUnit.hpp"




#include "Profile.hpp"
#include "Task.hpp"
#include "TaskList.hpp"
#include "TaskQueue.hpp"

#include "EnterExit.h"

#include "SP_Task.hpp"


//------------------------------------------------------------------------------


#include "Scheduler.hpp"

#include "Thread.hpp"
#include "ThreadQueue.hpp"
#include "ThreadVec.hpp"

#include "Queues.hpp"

#include "SP_Thread.hpp"

//------------------------------------------------------------------------------

#include "ApiMacros.h"

#include "Api.hpp"
#include "SessionArray.hpp"

#include "RapidsSE.hpp"


//------------------------------------------------------------------------------

#include "LockMetrics.hpp"
#include "MetricsHelper.hpp"

//------------------------------------------------------------------------------

#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "Error.hpp"
#include "Step.hpp"

//------------------------------------------------------------------------------

#include "FocalObjects.hpp"
#include "AttachMap.hpp"
#include "Constants.hpp"
#include "EnvCheck.hpp"
#include "Globals.hpp"
#include "Portrait.hpp"

#include "Engine.hpp"

#include "ShortCut.hpp"
typedef VectorNL_t<CN_SHORTCUTVEC, ShortCut, C_MAX_SHORTCUTS> ShortCutVec;

#include "SeDeputy.hpp"
#include "SePrompt.hpp"

#include "PromptFun.hpp"
#include "PromptCatalog.hpp"
#include "PromptHelper.hpp"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#include "Prompt_antlr.hpp"
#include "Prompt_apiDemo.hpp"
#include "Prompt_args.hpp"
#include "Prompt_ascii.hpp"
#include "Prompt_attachMap.hpp"

#include "Prompt_bp.hpp"
#include "Prompt_bpa.hpp"
#include "Prompt_bye.hpp"

#include "Prompt_cache.hpp"
#include "Prompt_calc.hpp"
#include "Prompt_cancel.hpp"
#include "Prompt_cd.hpp"
#include "Prompt_classes.hpp"
#include "Prompt_cmds.hpp"
#include "Prompt_config.hpp"
#include "Prompt_constants.hpp"

#include "Prompt_ddlLock.hpp"

#include "Prompt_describe.hpp"
#include "Prompt_dlist.hpp"
#include "Prompt_dsv.hpp"
#include "Prompt_dumpTokens.hpp"

#include "Prompt_enums.hpp"
#include "Prompt_errno.hpp"
#include "Prompt_executing.hpp"

#include "Prompt_fact.hpp"
#include "Prompt_file.hpp"
#include "Prompt_fileCat.hpp"
#include "Prompt_fileProc.hpp"
#include "Prompt_fileRand.hpp"
#include "Prompt_floats.hpp"
#include "Prompt_funs.hpp"

#include "Prompt_giveMem.hpp"
#include "Prompt_globals.hpp"

#include "Prompt_hashDefs.hpp"
#include "Prompt_heapSummary.hpp"
#include "Prompt_help.hpp"
#include "Prompt_hexdump.hpp"
#include "Prompt_how.hpp"
#include "Prompt_hw.hpp"

#include "Prompt_indexEntries.hpp"

#include "Prompt_job.hpp"

#include "Prompt_limits.hpp"
#include "Prompt_llist.hpp"
#include "Prompt_locks.hpp"
#include "Prompt_log.hpp"
#include "Prompt_logNote.hpp"
#include "Prompt_logSection.hpp"
#include "Prompt_logWrite.hpp"
#include "Prompt_ls.hpp"
#include "Prompt_lvl.hpp"

#include "Prompt_masks.hpp"
#include "Prompt_memory.hpp"
#include "Prompt_memoryLeaks.hpp"
#include "Prompt_memTest.hpp"
#include "Prompt_mkfifo.hpp"
#include "Prompt_mon.hpp"
#include "Prompt_mri.hpp"

#include "Prompt_now.hpp"
#include "Prompt_numbers.hpp"

#include "Prompt_objectList.hpp"
#include "Prompt_objectValidate.hpp"
#include "Prompt_olist.hpp"
#include "Prompt_opdq.hpp"
#include "Prompt_operands.hpp"
#include "Prompt_opsys.hpp"

#include "Prompt_plist.hpp"
#include "Prompt_pmc.hpp"
#include "Prompt_pow2.hpp"
#include "Prompt_prefixes.hpp"
#include "Prompt_print.hpp"
#include "Prompt_procedure.hpp"
#include "Prompt_prompt.hpp"
#include "Prompt_promptDebug.hpp"
#include "Prompt_pwd.hpp"

#include "Prompt_question.hpp"

#include "Prompt_rassert.hpp"
#include "Prompt_ready.hpp"
#include "Prompt_resources.hpp"
#include "Prompt_rmfifo.hpp"
#include "Prompt_rnd.hpp"

#include "Prompt_selfies.hpp"
#include "Prompt_setDbg.hpp"
#include "Prompt_shell.hpp"
#include "Prompt_shortcut.hpp"
#include "Prompt_show.hpp"
#include "Prompt_sizes.hpp"
#include "Prompt_sleep.hpp"
#include "Prompt_sleepers.hpp"
#include "Prompt_sleeping.hpp"
#include "Prompt_snapshotDelta.hpp"
#include "Prompt_snapshotInspect.hpp"
#include "Prompt_snapshotList.hpp"
#include "Prompt_snapshotNewer.hpp"
#include "Prompt_snapshotObjects.hpp"
#include "Prompt_so.hpp"
#include "Prompt_socket.hpp"
#include "Prompt_ssn.hpp"
#include "Prompt_ssnState.hpp"
#include "Prompt_stepTiming.hpp"
#include "Prompt_store.hpp"

#include "Prompt_tag.hpp"
#include "Prompt_tags.hpp"
#include "Prompt_takeMem.hpp"
#include "Prompt_taskQueues.hpp"
#include "Prompt_tempName.hpp"
#include "Prompt_threads.hpp"
#include "Prompt_timerEnd.hpp"
#include "Prompt_timerStart.hpp"
#include "Prompt_tools.hpp"
#include "Prompt_tpch.hpp"
#include "Prompt_trackMemory.hpp"

#include "Prompt_uptime.hpp"

#include "Prompt_version.hpp"

#include "Prompt_wait_bg.hpp"
#include "Prompt_waiting.hpp"
#include "Prompt_with.hpp"
#include "Prompt_without.hpp"




#include "ShmLog.hpp"

#include "LogEntryDdl.hpp"
#include "LogEntryFyi.hpp"
#include "LogEntryJob.hpp"
#include "LogEntryLck.hpp"
#include "LogEntryLog.hpp"
#include "LogEntryMgt.hpp"
#include "LogEntryOp.hpp"
#include "LogEntryPrm.hpp"
#include "LogEntryQue.hpp"
#include "LogEntryRes.hpp"
#include "LogEntrySsn.hpp"
#include "LogEntrySvc.hpp"
#include "LogEntrySys.hpp"
#include "LogEntryThd.hpp"
#include "LogEntryTsk.hpp"
#include "LogEntryTxn.hpp"
#include "LogEntry.hpp"
#include "Log.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
