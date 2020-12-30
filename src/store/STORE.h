/*============================================================================*/
/* STORE.h -- STORE namespace                                                 */
/*============================================================================*/
#pragma once

//------------------------------------------------------------------------------

namespace STORE
{
  void vCheckAllAssumptions(void);
  void vCheckAssumptions(void);
  void vSetSignalHandlers(void);
}

//------------------------------------------------------------------------------

static const i8 C_DEF_LVL_LEVELS = 3;
static const i8 C_DEF_LVL_RATIO  = 3; // this means 1/2^3 i.e. reducing by 8


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
//#include "Limits.hpp"
//#include "Licence.hpp"

//------------------------------------------------------------------------------


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


#include "Literal.hpp"

//------------------------------------------------------------------------------

#include "BucketMap.hpp"
#include "PtnMap.hpp"

#include "StreamDef.hpp"

#include "IndexDef.hpp"

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

#include "Ref.hpp"

#include "Row.hpp"
#include "NmlStr.hpp"
#include "Record.hpp"
#include "RowBox_t.hpp"

#include "RowSingle.hpp"

#include "CellSegment.hpp"
#include "CellSingle.hpp"

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "OverLine.hpp"
#include "TailLine.hpp"
#include "TailStr.hpp"
#include "Crw.hpp"


//------------------------------------------------------------------------------

#include "Encoding_v2.hpp"
#include "Encoding.hpp"
#include "Packing_v2.hpp"

#include "CrwBuffer.hpp"
#include "RowCompactor.hpp"
#include "RowInflator.hpp"

#include "SP_Encoding_v2.hpp"

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

#include "Table.hpp"


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

//------------------------------------------------------------------------------

#include "graph/PlanConstants.hpp"
#include "graph/PlanNode.hpp"
#include "graph/PlanNodeBuilder.hpp"
#include "graph/RootNode.hpp"
#include "graph/DefinitionNode.hpp"
#include "graph/PrimitiveNode.hpp"
#include "graph/MutablePlanNode.hpp"
#include "graph/QueryNode.hpp"
#include "graph/MetadataNode.hpp"
#include "graph/StatisticsNode.hpp"
#include "graph/PlanDefinitionParser.hpp"

#include "shadowNodes/ShadowNode.hpp"
#include "shadowNodes/TableRef.hpp"
#include "shadowNodes/Statement.hpp"
#include "shadowNodes/expressions/Expr.hpp"
#include "shadowNodes/expressions/LiteralExpr.hpp"
#include "shadowNodes/expressions/ColumnRef.hpp"

#include "graphTools/GraphNav.hpp"
#include "graphTools/GraphEditor.hpp"

#include "parser/stages/parser/SqlPlanner.hpp"

using namespace Parser;

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

//#include "LockMetrics.hpp"
//#include "MetricsHelper.hpp"

//------------------------------------------------------------------------------

#include "Error.hpp"

#include "StepAccum.hpp"
#include "StepTiming.hpp"
#include "Step.hpp"

//------------------------------------------------------------------------------

#include "FocalObjects.hpp"
#include "AttachMap.hpp"
#include "Constants.hpp"
#include "EnvCheck.hpp"
#include "Portrait.hpp"

//#include "Engine.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
