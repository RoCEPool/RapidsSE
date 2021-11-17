/*============================================================================*/
/* SizesHeaps.cpp -- Class Sizes and Heaps                                    */
/*============================================================================*/

#include "BASE.h"

#include "eBO.h"
#include "eCC.h"
#include "eCP.h"
#include "eCX.h"
#include "eDL.h"
#include "eDS.h"
#include "eED.h"
#include "eFN.h"
#include "eIK.h"
#include "eJF.h"
#include "eLG.h"
#include "eMM.h"
#include "eMT.h"
#include "eNE.h"
#include "eOK.h"
#include "eOR.h"
#include "ePF.h"
#include "ePP.h"
#include "eYG.h"
#include "ePY.h"
#include "eYX.h"
#include "eRG.h"
#include "eRO.h"
#include "eRU.h"
#include "eST.h"
#include "eTP.h"
#include "eTT.h"
#include "eUK.h"
#include "eXK.h"

#include "SizesHeaps.hpp"

#include "backend/Backend.hpp"

#include "CmdLineArgs.hpp"

#include "DdlLock.hpp"
#include "DicLock.hpp"

#include "EachRowBuffer.hpp"

#include "IndexDef.hpp"

#include "Index.hpp"

#include "Folder.hpp"
#include "Schema.hpp"
#include "Catalog.hpp"
#include "Tenant.hpp"
#include "Tenants.hpp"
#include "Marker.hpp"
#include "Record.hpp"
#include "RowBuffer.hpp"
#include "HashSet.hpp"
#include "Pot.hpp"

#include "InfoSet.hpp"
#include "NamedBuffer.hpp"
#include "StreamSet.hpp"
#include "RowCoder.hpp"

#include "HeadLine.hpp"
#include "Crw.hpp"

#include "Encoding.hpp"

#include "PtnMap.hpp"


#include "BucketMap_dyn.hpp"
#include "BufferMap.hpp"

#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "RowCompactor.hpp"
#include "OtherOperand.hpp"
#include "PlanOperand.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "EntBufSorter.hpp"
#include "IndexTree.hpp"

#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "CrwTablePtn.hpp"
#include "Bucket.hpp"
#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "PtnStm_cursor.hpp"
#include "PtnStream.hpp"
#include "Table.hpp"

#include "Node.hpp"
#include "Node_I.hpp"
#include "Grid.hpp"
#include "Session.hpp"
#include "Arg.hpp"
#include "RelOp.hpp"
#include "Predicate.hpp"
#include "PredicateVec.hpp"

#include "AccessPath.hpp"
#include "Api.hpp"
#include "AttachMap.hpp"

#include "Exemplar.hpp"

#include "Combined.hpp"
#include "InternalTables.hpp"

#include "Job.hpp"
#include "JobList.hpp"
#include "JobReport.hpp"
#include "Jobs.hpp"

#include "Lit.hpp"
#include "LitRecogniser.hpp"
#include "Locator.hpp"
#include "LogicUnit.hpp"

#include "Metadata.hpp"
#include "MetaGet.hpp"
#include "backend/ExprEval.hpp"
#include "BucketGet.hpp"

#include "Parcel.hpp"
#include "Numbering.hpp"
#include "Output.hpp"
#include "Op.hpp"
#include "OpSpec.hpp"

#include "PathIdentifier.hpp"
#include "PipelineSpec.hpp"
#include "Pipeline.hpp"
#include "PlannerLock.hpp"

#include "Dynamo.hpp"
#include "Optimus.hpp"

#include "Procedure.hpp"

#include "ReapLock.hpp"
#include "Dissect.hpp"
#include "Reassemble.hpp"
#include "Ribbon.hpp"
#include "RibbonLine.hpp"
#include "RowTablePtn.hpp"
#include "Drafter.hpp"

#include "Card.hpp"
#include "Card_Aggreg.hpp"
#include "Card_AggregList.hpp"
#include "Card_AggregPred.hpp"
#include "Card_AndPred.hpp"
#include "Card_As.hpp"
#include "Card_Atom.hpp"
#include "Card_AtomList.hpp"
#include "Card_By.hpp"
#include "Card_ByList.hpp"
#include "Card_ByPred.hpp"
#include "Card_ComparePred.hpp"
#include "Card_CorrSubquery.hpp"
#include "Card_CreateTable.hpp"
#include "Card_DropTable.hpp"
#include "Card_Exists.hpp"
#include "Card_Expr.hpp"
#include "Card_ExprList.hpp"
#include "Card_Filter.hpp"
#include "Card_FilterList.hpp"
#include "Card_HashCast.hpp"
#include "Card_Having.hpp"
#include "Card_HavingList.hpp"
#include "Card_Index.hpp"
#include "Card_IndexJoin.hpp"
#include "Card_IndexJoinList.hpp"
#include "Card_IndexPred.hpp"
#include "Card_IndexScan.hpp"
#include "Card_IndexScanList.hpp"
#include "Card_Limit.hpp"
#include "Card_MainSubquery.hpp"
#include "Card_NodeCast.hpp"
#include "Card_NoncSubquery.hpp"
#include "Card_On.hpp"
#include "Card_OnList.hpp"
#include "Card_OriginCast.hpp"
#include "Card_OrPred.hpp"
#include "Card_Project.hpp"
#include "Card_ProjectList.hpp"
#include "Card_ProjectPred.hpp"
#include "Card_Scan.hpp"
#include "Card_Send.hpp"
#include "Card_Sort.hpp"
#include "Card_SortList.hpp"
#include "Card_Stage.hpp"
#include "Card_StreamDef.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Type.hpp"

#include "Board.hpp"
#include "Bench.hpp"
#include "Eval_And.hpp"
#include "Eval_Compare.hpp"
#include "Eval_Filter.hpp"
#include "Eval_Or.hpp"

#include "Scanner.hpp"

#include "SeApi.hpp"
#include "Sessions.hpp"

#include "ShardDef.hpp"
#include "ShortCut.hpp"
#include "SeDeputy.hpp"
#include "SePrompt.hpp"
#include "Shape.hpp"
#include "Stage.hpp"
#include "StepAccum.hpp"
#include "StageSpec.hpp"
#include "Statement.hpp"
#include "Stencil.hpp"
#include "Steps.hpp"
#include "Stream.hpp"
#include "StreamDefRht.hpp"
#include "StzList.hpp"
#include "SubPipelines.hpp"

#include "SP_Gid.hpp"
#include "SP_Elapsed.hpp"
#include "SP_String.hpp"

#include "SN_Bcd.hpp"
#include "SN_Bol.hpp"
#include "SN_Dte.hpp"
#include "SN_Flt.hpp"
#include "SN_Int.hpp"
#include "SN_Ivl.hpp"
#include "SN_Tme.hpp"
#include "SN_Tsp.hpp"
#include "SN_Vch.hpp"

#include "TccLock.hpp"
#include "ToBktCrw.hpp"
#include "ToBktRow.hpp"
#include "Tools.hpp"
#include "TraceLock.hpp"
#include "TraceMultiLock.hpp"
#include "Tub.hpp"

#include "UnitInt.hpp"
#include "UT_SkipList.hpp"

#include "Parcel.hpp"
#include "Sends.hpp"

#include "CodeSynth.hpp"
#include "Flatten.hpp"

#include "graph/PlanConstants.hpp"
#include "graph/PlanNode.hpp"
#include "graph/PlanNodeBuilder.hpp"
#include "graph/RootNode.hpp"
#include "graph/DefinitionNode.hpp"
#include "graph/PrimitiveNode.hpp"
#include "graph/QueryNode.hpp"
#include "graph/PlanDefinitionParser.hpp"
#include "graph/ConditionalQueryNodeIter.hpp"

#include "graphTools/GraphNav.hpp"
#include "graphTools/GraphEditor.hpp"

#include "parser/stages/parser/SqlPlanner.hpp"
using namespace Parser;

/*----------------------------------------------------------------------------*/
/* sizes & heaps                                                              */
/*----------------------------------------------------------------------------*/
#define FILL ,0,0,0,0,0,0

SizeHeap gxaSizeHeap[]
{
  { 0,                          HT_INVALID   ,0 FILL }, // CN_NONE

  { 'A' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__A__
  { sizeof(AccessPath),         HT_T_MEDIUM  ,0 FILL }, // CN_ACCESSPATH
  { sizeof(AggTimer),           HT_T_TINY    ,0 FILL }, // CN_AGGTIMER
  { 0,                          HT_INVALID   ,0 FILL }, // CN_AM
  { sizeof(Api),                HT_INVALID   ,0 FILL }, // CN_API
  { sizeof(Arg),                HT_T_TINY    ,0 FILL }, // CN_ARG
  { sizeof(Arrange),            HT_T_TINY    ,0 FILL }, // CN_ARRANGE
  { sizeof(Atomic),             HT_INVALID   ,0 FILL }, // CN_ATOMIC
  { sizeof(AttachMap),          HT_INVALID   ,0 FILL }, // CN_ATTACHMAP

  { 'B' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__B__
  { sizeof(Backend),/*yes*/     HT_T_MEDIUM  ,0 FILL }, // CN_BACKEND
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 FILL }, // CN_BCDVAL
  { sizeof(Bench),              HT_T_SMALL   ,0 FILL }, // CN_BENCH
  { sizeof(Birth),              HT_INVALID   ,0 FILL }, // CN_BIRTH
  { sizeof(Board),              HT_T_SMALL   ,0 FILL }, // CN_BOARD
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 FILL }, // CN_BOLVAL
  { sizeof(Bucket),             HT_T_TINY    ,0 FILL }, // CN_BUCKET
  { sizeof(BucketGet),          HT_T_TINY    ,0 FILL }, // CN_BUCKETGET
  { sizeof(BucketMap_dyn),      HT_T_LARGE   ,0 FILL }, // CN_BUCKETMAP_dyn
  { sizeof(BucketVec),          HT_T_SMALL   ,0 FILL }, // CN_BUCKETVEC
  { sizeof(BufferMap),          HT_INVALID   ,0 FILL }, // CN_BUFFERMAP
  { sizeof(BufferPool),         HT_INVALID   ,0 FILL }, // CN_BUFFERPOOL
  { sizeof(ByteRate),           HT_T_TINY    ,0 FILL }, // CN_BYTERATE
  { sizeof(s32),                HT_INVALID   ,0 FILL }, // CN_BYTES_T

  { 'C' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__C__
  { sizeof(Card),               HT_T_SMALL   ,0 FILL }, // CN_CARD
  { sizeof(Card_Aggreg),        HT_T_TINY    ,0 FILL }, // CN_CARD_AGGREG
  { sizeof(Card_AggregList),    HT_T_TINY    ,0 FILL }, // CN_CARD_AGGREGLIST
  { sizeof(Card_AggregPred),    HT_T_TINY    ,0 FILL }, // CN_CARD_AGGREGPRED
  { sizeof(Card_AndPred),       HT_T_TINY    ,0 FILL }, // CN_CARD_ANDPRED
  { sizeof(Card_As),            HT_T_TINY    ,0 FILL }, // CN_CARD_AS
  { sizeof(Card_Atom),          HT_T_TINY    ,0 FILL }, // CN_CARD_ATOM
  { sizeof(Card_AtomList),      HT_T_TINY    ,0 FILL }, // CN_CARD_ATOMLIST
  { sizeof(Card_By),            HT_T_TINY    ,0 FILL }, // CN_CARD_BY
  { sizeof(Card_ByList),        HT_T_TINY    ,0 FILL }, // CN_CARD_BYLIST
  { sizeof(Card_ByPred),        HT_T_TINY    ,0 FILL }, // CN_CARD_BYPRED
  { sizeof(Card_ComparePred),   HT_T_TINY    ,0 FILL }, // CN_CARD_COMPAREPRED
  { sizeof(Card_CorrSubquery),  HT_T_TINY    ,0 FILL }, // CN_CARD_CORRSUBQUERY
  { sizeof(Card_CreateTable),   HT_T_TINY    ,0 FILL }, // CN_CARD_CREATETABLE
  { sizeof(Card_DropTable),     HT_T_TINY    ,0 FILL }, // CN_CARD_DROPTABLE
  { sizeof(Card_Exists),        HT_T_TINY    ,0 FILL }, // CN_CARD_EXISTS
  { sizeof(Card_Expr),          HT_T_TINY    ,0 FILL }, // CN_CARD_EXPR
  { sizeof(Card_ExprList),      HT_T_TINY    ,0 FILL }, // CN_CARD_EXPRLIST
  { sizeof(Card_Filter),        HT_T_TINY    ,0 FILL }, // CN_CARD_FILTER
  { sizeof(Card_FilterList),    HT_T_TINY    ,0 FILL }, // CN_CARD_FILTERLIST
  { sizeof(Card_HashCast),      HT_T_TINY    ,0 FILL }, // CN_CARD_HASHCAST
  { sizeof(Card_Having),        HT_T_TINY    ,0 FILL }, // CN_CARD_HAVING
  { sizeof(Card_HavingList),    HT_T_TINY    ,0 FILL }, // CN_CARD_HAVINGLIST
  { sizeof(Card_Index),         HT_T_TINY    ,0 FILL }, // CN_CARD_INDEX
  { sizeof(Card_IndexJoin),     HT_T_TINY    ,0 FILL }, // CN_CARD_INDEXJOIN
  { sizeof(Card_IndexJoinList), HT_T_TINY    ,0 FILL }, // CN_CARD_INDEXJOINLIST
  { sizeof(Card_IndexPred),     HT_T_TINY    ,0 FILL }, // CN_CARD_INDEXPRED
  { sizeof(Card_IndexScan),     HT_T_TINY    ,0 FILL }, // CN_CARD_INDEXSCAN
  { sizeof(Card_IndexScanList), HT_T_TINY    ,0 FILL }, // CN_CARD_INDEXSCANLIST
  { sizeof(Card_Limit),         HT_T_TINY    ,0 FILL }, // CN_CARD_LIMIT
  { sizeof(Card_MainSubquery),  HT_T_TINY    ,0 FILL }, // CN_CARD_MAINSUBQUERY
  { sizeof(Card_NodeCast),      HT_T_TINY    ,0 FILL }, // CN_CARD_NODECAST
  { sizeof(Card_NoncSubquery),  HT_T_TINY    ,0 FILL }, // CN_CARD_NONCSUBQUERY
  { sizeof(Card_On),            HT_T_TINY    ,0 FILL }, // CN_CARD_ON
  { sizeof(Card_OnList),        HT_T_TINY    ,0 FILL }, // CN_CARD_ONLIST
  { sizeof(Card_OrdAgg),        HT_T_TINY    ,0 FILL }, // CN_CARD_ORDAGG
  { sizeof(Card_OriginCast),    HT_T_TINY    ,0 FILL }, // CN_CARD_ORIGINCAST
  { sizeof(Card_OrPred),        HT_T_TINY    ,0 FILL }, // CN_CARD_ORPRED
  { sizeof(Card_Project),       HT_T_TINY    ,0 FILL }, // CN_CARD_PROJECT
  { sizeof(Card_ProjectList),   HT_T_TINY    ,0 FILL }, // CN_CARD_PROJECTLIST
  { sizeof(Card_ProjectPred),   HT_T_TINY    ,0 FILL }, // CN_CARD_PROJECTPRED
  { sizeof(Card_Scan),          HT_T_TINY    ,0 FILL }, // CN_CARD_SCAN
  { sizeof(Card_Send),          HT_T_TINY    ,0 FILL }, // CN_CARD_SEND
  { sizeof(Card_Sort),          HT_T_TINY    ,0 FILL }, // CN_CARD_SORT
  { sizeof(Card_SortList),      HT_T_TINY    ,0 FILL }, // CN_CARD_SORTLIST
  { sizeof(Card_Stage),         HT_T_TINY    ,0 FILL }, // CN_CARD_STAGE
  { sizeof(Card_StreamDef),     HT_T_TINY    ,0 FILL }, // CN_CARD_STREAMDEF
  { sizeof(Card_Table),         HT_T_TINY    ,0 FILL }, // CN_CARD_TABLE
  { sizeof(Card_TableList),     HT_T_TINY    ,0 FILL }, // CN_CARD_TABLELIST
  { sizeof(Card_Type),          HT_T_TINY    ,0 FILL }, // CN_CARD_TYPE
  { sizeof(Catalog),            HT_INVALID   ,0 FILL }, // CN_CATALOG
  { sizeof(CellSingle),         HT_INVALID   ,0 FILL }, // CN_CELLSIGN:E
  { sizeof(Charser),            HT_T_LARGE   ,0 FILL }, // CN_CHARSER
  { sizeof(ClassPlaceHolder),   HT_T_LARGE   ,0 FILL }, // CN_CLASSPLACEHOLDER
  { sizeof(CmdLineArgs),        HT_INVALID   ,0 FILL }, // CN_CMDLINEARGS
  { sizeof(CodeBlock),          HT_T_SMALL   ,0 FILL }, // CN_CODEBLOCK
  { sizeof(CodeSynth),          HT_T_LARGE   ,0 FILL }, // CN_CODESYNTH
  { sizeof(Column),             HT_T_TINY    ,0 FILL }, // CN_COLUMN
  { sizeof(Combined),           HT_T_LARGE   ,0 FILL }, // CN_COMBINED
  { sizeof(ConditionalQueryNodeIter), HT_T_TINY,0 FILL }, // CN_CONDITIONALQUERYNODEITER
  { sizeof(CrvBuffer),          HT_T_LARGE   ,0 FILL }, // CN_CRVBUFFER
  { sizeof(CrwBuffer),          HT_T_LARGE   ,0 FILL }, // CN_CRWBUFFER
  { sizeof(CrwTablePtn),        HT_T_TINY    ,0 FILL }, // CN_CRWTABLEPTN
  { sizeof(CrwToRow),           HT_T_TINY    ,0 FILL }, // CN_CRWTOROW
  { sizeof(CsvBuffer),          HT_T_LARGE   ,0 FILL }, // CN_CSVBUFFER
  { sizeof(CsvMeta),            HT_T_TINY    ,0 FILL }, // CN_CSVMETA

  { 'D' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__D__
  { sizeof(DdlLock),            HT_T_TINY    ,0 FILL }, // CN_DDLLOCK
  { sizeof(DecaCnt),            HT_T_TINY    ,0 FILL }, // CN_DECACNT
  { sizeof(Decider),            HT_T_TINY    ,0 FILL }, // CN_DECIDER
  { sizeof(DefinitionNode),     HT_T_TINY    ,-8 FILL }, // CN_DEFINITIONNODE
  { sizeof(DevIXT),             HT_T_TINY    ,0 FILL }, // CN_DEVIXT
  { sizeof(DevROW),             HT_T_TINY    ,0 FILL }, // CN_DEVROW
  { sizeof(Dic),                HT_T_TINY    ,0 FILL }, // CN_DIC
  { sizeof(DicLock),            HT_T_TINY    ,0 FILL }, // CN_DICLOCK
  { sizeof(DicStack),           HT_T_TINY    ,0 FILL }, // CN_DICSTACK
  { sizeof(Dir),                HT_T_TINY    ,0 FILL }, // CN_DIR
  { sizeof(Dissect),            HT_T_TINY    ,0 FILL }, // CN_DISSECT
  { sizeof(Drafter),            HT_T_TINY    ,0 FILL }, // CN_DRAFTER
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 FILL }, // CN_DTEVAL
  { sizeof(Dynamo),             HT_T_TINY    ,0 FILL }, // CN_DYNAMO

  { 'E' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__E__
  { sizeof(EachRowBuffer),      HT_T_MEDIUM  ,0 FILL }, // CN_EACHROWBUFFER
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 FILL }, // CN_EDTVAL
  { sizeof(Encoding),           HT_T_MEDIUM  ,0 FILL }, // CN_ENCODING
  { sizeof(Ent),                HT_INVALID   ,0 FILL }, // CN_ENT
  { sizeof(EntBuffer),          HT_T_LARGE   ,0 FILL }, // CN_ENTBUFFER
  { sizeof(EntBufSorter),       HT_T_LARGE   ,0 FILL }, // CN_ENTBUFSORTER
  { sizeof(EntWrk),             HT_INVALID   ,0 FILL }, // CN_ENTWRK
  { sizeof(ClassFlags),         HT_T_TINY    ,0 FILL }, // CN_ENUMFLAGS
  { sizeof(Eval_And),           HT_T_TINY    ,0 FILL }, // CN_EVAL_AND
  { sizeof(Eval_Compare),       HT_T_TINY    ,0 FILL }, // CN_EVAL_COMPARE
  { sizeof(Eval_Filter),        HT_T_TINY    ,0 FILL }, // CN_EVAL_FILTER
  { sizeof(Eval_Or),            HT_T_TINY    ,0 FILL }, // CN_EVAL_OR
  { sizeof(Exemplar),           HT_T_TINY    ,0 FILL }, // CN_EXEMPLAR
  { sizeof(ExprEval),           HT_T_MEDIUM
    
    
    ,0 FILL }, // CN_EXPREVAL

  { 'F' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__F__
  { sizeof(Fifo),               HT_T_TINY    ,0 FILL }, // CN_FIFO
  { sizeof(File),               HT_T_TINY    ,0 FILL }, // CN_FILE
  { sizeof(TypedCell), /*yes*/  HT_INVALID   ,0 FILL }, // CN_FIPVAL
  { sizeof(Flatten), /*yes*/    HT_T_TINY    ,0 FILL }, // CN_FLATTEN
  { sizeof(TypedCell), /*yes*/  HT_INVALID   ,0 FILL }, // CN_FLTVAL
  { sizeof(Format),             HT_T_TINY    ,0 FILL }, // CN_FORMAT
  { sizeof(PoolBuffer),/*yes*/  HT_INVALID   ,0 FILL }, // CN_FREEBUFFER
  { sizeof(FreeChunk),          HT_INVALID   ,0 FILL }, // CN_FREECHUNK
  { sizeof(FreedNot),           HT_INVALID   ,0 FILL }, // CN_FREEDNOT
  { 0,                          HT_INVALID   ,0 FILL }, // CN_FREEITEM
  { sizeof(FreqDist),           HT_T_SMALL   ,0 FILL }, // CN_FREQDIST

  { 'G' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__G__
  { sizeof(Grid),               HT_T_SMALL   ,0 FILL }, // CN_GRID

  { 'H' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__H__
  { sizeof(HashBuffer),         HT_T_LARGE   ,0 FILL }, // CN_HASHBUFFER
  { sizeof(HashSet),            HT_T_LARGE   ,0 FILL }, // CN_HASHSET
  { sizeof(HashMap),            HT_T_TINY    ,0 FILL }, // CN_HASHMAP
  { sizeof(HeapBuffer),         HT_INVALID   ,0 FILL }, // CN_HEAPBUFFER
  { sizeof(Heaps),              HT_INVALID   ,0 FILL }, // CN_HEAPMEM

  { 'I' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__I__
  { sizeof(Index),              HT_T_TINY    ,0 FILL }, // CN_INDEX
  { sizeof(IndexDef),           HT_T_MEDIUM  ,0 FILL }, // CN_INDEXDEF
  { sizeof(IndexStream),        HT_T_TINY    ,0 FILL }, // CN_INDEXSTREAM
  { sizeof(IndexTree),          HT_T_TINY    ,0 FILL }, // CN_INDEXTREE
  { sizeof(IndexTreeVec),       HT_T_MEDIUM  ,0 FILL }, // CN_INDEXTREEVEC
  { sizeof(IdxVec),             HT_T_MEDIUM  ,0 FILL }, // CN_IDXVEC
  { sizeof(InfoSet),            HT_T_LARGE   ,0 FILL }, // CN_INFOSET
  { sizeof(IngestChunk),        HT_T_LARGE   ,0 FILL }, // CN_INGESTCHUNK
  { sizeof(Input),              HT_T_TINY    ,0 FILL }, // CN_INPUT
  { sizeof(InternalTables),     HT_T_SMALL   ,0 FILL }, // CN_INTERNALTABLES
  { sizeof(IntSet),             HT_T_TINY    ,0 FILL }, // CN_INTSET
  { sizeof(IntStack),           HT_T_TINY    ,0 FILL }, // CN_INTSTACK
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 FILL }, // CN_INTVAL
  { sizeof(IoChunk),            HT_T_LARGE   ,0 FILL }, // CN_IOCHUNK
  { sizeof(IoCopy),             HT_T_TINY    ,0 FILL }, // CN_IOCOPY
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 FILL }, // CN_IVLVAL

  { 'J' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__J__
  { sizeof(Job),                HT_T_MEDIUM  ,0 FILL }, // CN_JOB
  { sizeof(JobList),            HT_T_TINY    ,0 FILL }, // CN_JOBLIST
  { sizeof(JobReport),          HT_T_TINY    ,0 FILL }, // CN_JOBREPORT
  { sizeof(Jobs),               HT_T_TINY    ,0 FILL }, // CN_JOBS

  { 'K' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__K__
  { sizeof(Key),                HT_INVALID   ,0 FILL }, // CN_KEY

  { 'L' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__L__
  { sizeof(Heap), /*correct*/   HT_INVALID   ,0 FILL }, // CN_LARGEPERSISTENT
  { sizeof(Heap), /*correct*/   HT_INVALID   ,0 FILL }, // CN_LARGETRANSIENT
  { sizeof(Heap), /*correct*/   HT_INVALID   ,0 FILL }, // CN_LARGEVIRTUAL
  { sizeof(Licence),            HT_T_TINY    ,0 FILL }, // CN_LICENCE
  { sizeof(Limits),             HT_T_TINY    ,0 FILL }, // CN_LIMITS
  { sizeof(ListExoNode_t<Object>), HT_T_TINY ,0 FILL }, // CN_LISTEXONODE_t
  { sizeof(ListExoIter_t<Object>), HT_T_TINY ,0 FILL }, // CN_LISTEXOITER_t
  { sizeof(Lit),                HT_T_TINY    ,0 FILL }, // CN_LIT
  { sizeof(LitRecogniser),      HT_T_TINY    ,0 FILL }, // CN_LITRECOGNISER
  { sizeof(LoadChunk),          HT_T_LARGE   ,0 FILL }, // CN_LOADCHUNK
  { sizeof(Local),              HT_INVALID   ,0 FILL }, // CN_LOCAL
  { sizeof(LocalChain),         HT_T_LARGE   ,0 FILL }, // CN_LOCALCHAIN
  { sizeof(LocalDir),           HT_INVALID   ,0 FILL }, // CN_LOCALDIR
  { sizeof(LocalFile),          HT_INVALID   ,0 FILL }, // CN_LOCALFILE
  { sizeof(BASE::Location),     HT_T_TINY    ,0 FILL }, // CN_LOCATION
  { sizeof(Locator),            HT_T_TINY    ,0 FILL }, // CN_LOCATOR
  { sizeof(Lock),               HT_INVALID   ,0 FILL }, // CN_LOCK
  { 0,                          HT_INVALID   ,0 FILL }, // CN_LOCKSET
  { sizeof(LogicCompiler),      HT_T_LARGE   ,0 FILL }, // CN_LOGICCOMPILER
  { sizeof(LogicTimer),         HT_T_MEDIUM  ,0 FILL }, // CN_LOGICTIMER
  { sizeof(LogicUnit),          HT_T_LARGE   ,0 FILL }, // CN_LOGICUNIT

  { 'M' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__M__
  { sizeof(Marker),             HT_T_TINY    ,0 FILL }, // CN_MARKER
  { sizeof(Heap), /*correct*/   HT_INVALID   ,0 FILL }, // CN_MEDIUMPERSISTENT
  { sizeof(Heap), /*correct*/   HT_INVALID   ,0 FILL }, // CN_MEDIUMTRANSIENT
  { sizeof(Heap), /*correct*/   HT_INVALID   ,0 FILL }, // CN_MEDIUMTVIRTUAL
  { sizeof(MemTest),            HT_T_TINY    ,0 FILL }, // CN_MEMTEST
  { sizeof(Metadata),           HT_T_TINY    ,0 FILL }, // CN_METADATA
  { sizeof(MetaGet),            HT_T_TINY     ,0 FILL }, // CN_METAGET
  { sizeof(Mutex),              HT_T_TINY    ,0 FILL }, // CN_MUTEX

  { 'N' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__N__
  { sizeof(NamedBuffer),        HT_T_LARGE   ,0 FILL }, // CN_NESTEDPIPELINESPEC
  { 0,                          HT_INVALID   ,0 FILL }, // CN_NEXTGID
  { sizeof(NextGid),            HT_INVALID   ,0 FILL }, // CN_NEXTGID
  { sizeof(NmlStr),             HT_T_MEDIUM  ,0 FILL }, // CN_NMLSTR
  { sizeof(Node),               HT_T_MEDIUM  ,0 FILL }, // CN_NODE
  { sizeof(Node_I),             HT_T_TINY    ,0 FILL }, // CN_NODE_I
  { sizeof(Now),                HT_T_TINY    ,0 FILL }, // CN_NOW
  { sizeof(Numbat),             HT_T_LARGE   ,0 FILL }, // CN_NUMBAT
  { sizeof(Numbering),          HT_T_MEDIUM  ,0 FILL }, // CN_NUMBERING

  { 'O' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__O__
  { sizeof(ObjBuffer),          HT_T_LARGE   ,0 FILL }, // CN_OBJBUFFER
  { sizeof(ObjectSnapshot),     HT_T_SMALL   ,0 FILL }, // CN_OBJECTSNAPSHOT
  { sizeof(Output),             HT_T_TINY    ,0 FILL }, // CN_OUTPUT
  { sizeof(Op),                 HT_T_MEDIUM  ,0 FILL }, // CN_OP
  { sizeof(OpenFiles),          HT_T_TINY    ,0 FILL }, // CN_OPENFILES
  { sizeof(OperandQueue),       HT_T_TINY    ,0 FILL }, // CN_OPERANDQUEUE
  { sizeof(OpSpec),             HT_T_MEDIUM  ,0 FILL }, // CN_OPSPEC
  { sizeof(Optimus),            HT_T_TINY    ,0 FILL }, // CN_OPTIMUS
  { sizeof(OtherOperand),       HT_T_TINY    ,0 FILL }, // CN_OTHEROPERAND

  { 'P' - ('A' - 1),              HT_INVALID   ,0 FILL }, // CN__P__
  { sizeof(Parcel),               HT_T_TINY    ,0 FILL }, // CN_PARCEL
  { sizeof(PathIdentifier),       HT_T_MEDIUM  ,0 FILL }, // CN_PATHIDENTIFIER
  { sizeof(PathVec),              HT_T_MEDIUM  ,0 FILL }, // CN_PATHVEC
  { sizeof(Peruse),               HT_T_MEDIUM  ,0 FILL }, // CN_PERUSE
  { sizeof(Pipeline),             HT_T_SMALL   ,0 FILL }, // CN_PIPELINE
  { sizeof(PipelineSpec),         HT_T_MEDIUM  ,0 FILL }, // CN_PIPELINESPEC
  { sizeof(PlanDefinitionParser), HT_T_MEDIUM  , -8 FILL }, // CN_PLANDEFINITIONPARSER
  { sizeof(PlannerLock),          HT_T_TINY    ,0 FILL }, // CN_PLANNERLOCK
  { sizeof(PlanNode),             HT_T_TINY    , -8 FILL }, // CN_PLANNODE
  { sizeof(PlanNodeBuilder),      HT_T_TINY    ,0 FILL }, // CN_PLANNODEBUILDER
  { sizeof(PlanNodeBuilderList),  HT_T_TINY    ,0 FILL }, // CN_PLANNODEBUILDERLIST
  { sizeof(PlanNodeBuilderStack), HT_T_MEDIUM  ,0 FILL }, // CN_PLANNODEBUILDERSTACK
  { sizeof(PlanNodeList),         HT_T_TINY    ,0 FILL }, // CN_PLANNODELIST
  { sizeof(PlanOperand),          HT_T_TINY    ,0 FILL }, // CN_PLANOPERAND
  { sizeof(Pot),                  HT_T_LARGE   ,0 FILL }, // CN_POT
  { sizeof(Predicate),            HT_T_TINY    ,0 FILL }, // CN_PREDICATE
  { sizeof(PredicateVec),         HT_T_MEDIUM  ,0 FILL }, // CN_PREDICATEVEC
  { sizeof(PrimitiveNode),        HT_T_TINY    , -8 FILL }, // CN_PRIMITIVENODE
  { sizeof(Procedure),            HT_T_MEDIUM  ,0 FILL }, // CN_PROCEDURE
  { sizeof(Profile),              HT_T_MEDIUM  ,0 FILL }, // CN_PROFILE
  { sizeof(PsxThd),               HT_T_TINY    ,0 FILL }, // CN_PSXTHD
  { sizeof(PtnMap),               HT_T_LARGE   ,0 FILL }, // CN_PTNMAP
  { sizeof(PtnStm_cursor),        HT_T_TINY    ,0 FILL }, // CN_PTNSTMCURSOR
  { sizeof(PtnStream),            HT_T_TINY    ,0 FILL }, // CN_PTNSTREAM
  { sizeof(TypedCell),/*yes*/     HT_INVALID   ,0 FILL }, // CN_PTRVAL

  { 'Q' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__Q__
  { sizeof(QueryNode),          HT_T_TINY    ,-8 FILL }, // CN_QUERYNODE
  { sizeof(QueryNodeList),      HT_T_TINY    ,0 FILL }, // CN_QUERYNODELIST
  { sizeof(QueryNodeStack),     HT_T_MEDIUM  ,0 FILL }, // CN_QUERYNODESTACK
  { sizeof(Queue),              HT_T_TINY    ,0 FILL }, // CN_QUEUE
  { sizeof(QueueBuffer),        HT_INVALID   ,0 FILL }, // CN_QUEUEBUFFER

  { 'R' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__R__
  { sizeof(Range),              HT_T_LARGE   ,0 FILL }, // CN_RANGE
  { sizeof(Reader),             HT_T_TINY    ,0 FILL }, // CN_READER
  { sizeof(ReapLock),           HT_T_TINY    ,0 FILL }, // CN_REAPLOCK
  { sizeof(Reassemble),         HT_T_TINY    ,0 FILL }, // CN_REASSEMBLE
  { sizeof(Record),             HT_T_MEDIUM  ,0 FILL }, // CN_RECORD
  { sizeof(RelOp),              HT_T_TINY    ,0 FILL }, // CN_RELOP
  { sizeof(Resources),          HT_T_TINY    ,0 FILL }, // CN_RESOURCES
  { sizeof(Ribbon),             HT_T_TINY    ,0 FILL }, // CN_RIBBON
  { sizeof(RibbonLine),         HT_T_SMALL   ,0 FILL }, // CN_RIBBONLINE
  { sizeof(RluDiary),           HT_T_LARGE   ,0 FILL }, // CN_RLUDIARY
  { sizeof(Rand),               HT_INVALID   ,0 FILL }, // CN_RN_RAND
  { sizeof(RootNode),           HT_T_TINY    ,-8 FILL }, // CN_ROOTNODE
  { sizeof(Row),                HT_T_LARGE   ,0 FILL }, // CN_ROW
  { sizeof(Row),/*correct*/     HT_T_MEDIUM  ,0 FILL }, // CN_ROWAGG
  { 0,                          HT_INVALID   ,0 FILL }, // CN_ROWBOX
  { sizeof(RowBuffer),          HT_T_LARGE   ,0 FILL }, // CN_ROWBUFFER
  { sizeof(RowCompactor),       HT_T_SMALL   ,0 FILL }, // CN_ROWCOMPACTOR
  { sizeof(RowOnce),            HT_T_TINY    ,0 FILL }, // CN_ROWONCE
  { sizeof(RowPix),             HT_INVALID   ,0 FILL }, // CN_ROWPIX
  { sizeof(RowSingle),          HT_T_MEDIUM  ,0 FILL }, // CN_ROWSINGLE
  { sizeof(RowSingleList),      HT_T_TINY    ,0 FILL }, // CN_ROWSINGLELIST
  { sizeof(RowTablePtn),        HT_T_TINY    ,0 FILL }, // CN_ROWTABLEPTN
  { sizeof(RowTray),            HT_T_LARGE   ,0 FILL }, // CN_ROWTRAY
  { sizeof(RU_Cpu),             HT_INVALID   ,0 FILL }, // CN_RUCPU
  { 0,/*sizeof(RU_Dir)*/        HT_INVALID   ,0 FILL }, // CN_RUDIR
  { 0,/*sizeof(RU_File)*/       HT_INVALID   ,0 FILL }, // CN_RUFILE
  { sizeof(RU_Memory),          HT_INVALID   ,0 FILL }, // CN_RUMEMORY
  { 0,/*sizeof(RU_Mutex)*/      HT_INVALID   ,0 FILL }, // CN_RUMUTEX
  { 0,/*sizeof(RU_Pass)*/       HT_INVALID   ,0 FILL }, // CN_RUPASS
  { 0,/*sizeof(RU_Socket)*/     HT_INVALID   ,0 FILL }, // CN_RUSOCKET
  { 0,/*sizeof(RU_Time)*/       HT_INVALID   ,0 FILL }, // CN_RUTIME
  { 0,/*sizeof(RU_Word)*/       HT_INVALID   ,0 FILL }, // CN_RUWORK

  { 'S' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__S__
  { sizeof(Scanner),            HT_T_TINY    ,0 FILL }, // CN_SCANNER
  { sizeof(Scheduler),          HT_INVALID   ,0 FILL }, // CN_SCHEDULER
  { sizeof(Schema),             HT_T_TINY    ,0 FILL }, // CN_SCHEMA
  { sizeof(SeApi),              HT_T_LARGE   ,0 FILL }, // CN_SEAPI
  { sizeof(SeDeputy),           HT_T_TINY    ,0 FILL }, // CN_SEDEPUTY
  { sizeof(SeeMe),              HT_INVALID   ,0 FILL }, // CN_SEEME
  { sizeof(SenCompiler),        HT_T_TINY    ,0 FILL }, // CN_SENCOMPILER
  { sizeof(Sends),              HT_T_TINY    ,0 FILL }, // CN_SENDS
  { sizeof(SePrompt),           HT_T_LARGE   ,0 FILL }, // CN_SEPROMPT
  { sizeof(Session),            HT_T_LARGE   ,0 FILL }, // CN_SESSION
  { sizeof(Sessions),           HT_T_TINY    ,0 FILL }, // CN_SESSIONS
  { sizeof(Shape),              HT_T_TINY    ,0 FILL }, // CN_SHAPE
  { sizeof(ShardDef),           HT_T_LARGE   ,0 FILL }, // CN_SHARDDEF
  { sizeof(ShortCut),           HT_T_MEDIUM  ,0 FILL }, // CN_SHORTCUT
  { sizeof(ShortCutVec),        HT_T_MEDIUM  ,0 FILL }, // CN_SHORTCUTVEC
  { sizeof(Birth),              HT_INVALID   ,0 FILL }, // CN_SINCE
  { sizeof(SkipListNL),         HT_T_TINY    ,0 FILL }, // CN_SKIPLIST
  { sizeof(SkipNode),           HT_T_TINY    ,0 FILL }, // CN_SKIPNODE
  { sizeof(Slab),               HT_T_LARGE   ,0 FILL }, // CN_SLAB
  { sizeof(SlabAllocator),      HT_T_TINY    ,0 FILL }, // CN_SLABALLOCATOR
  { sizeof(Heap),/*correct*/    HT_INVALID   ,0 FILL }, // SN_SMALLPERSISTENT
  { sizeof(Heap),/*correct*/    HT_INVALID   ,0 FILL }, // SN_SMALLTRANSIENT
  { sizeof(Heap),/*correct*/    HT_INVALID   ,0 FILL }, // SN_SMALLVIRTUAL
  { sizeof(SN_Bcd),             HT_INVALID   ,0 FILL }, // CN_SNBCD
  { sizeof(SN_Bol),             HT_INVALID   ,0 FILL }, // CN_SNBOL
  { sizeof(SN_Dte),             HT_INVALID   ,0 FILL }, // CN_SNDTE
  { sizeof(SN_Flt),             HT_INVALID   ,0 FILL }, // CN_SNFLT
  { sizeof(SN_Int),             HT_INVALID   ,0 FILL }, // CN_SNINT
  { sizeof(SN_Ivl),             HT_INVALID   ,0 FILL }, // CN_SNIVL
  { sizeof(SN_Tme),             HT_INVALID   ,0 FILL }, // CN_SNTME
  { sizeof(SN_Tsp),             HT_INVALID   ,0 FILL }, // CN_SNTSP
  { sizeof(SN_Vch),             HT_INVALID   ,0 FILL }, // CN_SNVCH
  { sizeof(Socket),             HT_T_TINY    ,0 FILL }, // CN_SOCKET
  { sizeof(SP_Elapsed),         HT_INVALID   ,0 FILL }, // CN_SPELAPSED
  { sizeof(SP_Gid),             HT_INVALID   ,0 FILL }, // CN_SPID
  { sizeof(SP_String),          HT_INVALID   ,0 FILL }, // CN_SPSTRING
  { sizeof(SPmedium),           HT_INVALID   ,0 FILL }, // CN_SPMEDIUM
  { sizeof(SPsmall),            HT_INVALID   ,0 FILL }, // CN_SPSMALL
  { sizeof(SqlPlanner),         HT_T_MEDIUM  ,-8 FILL }, // CN_SQLPLANNER
  { sizeof(TypedStack), /*yes*/ HT_T_MEDIUM  ,0 FILL }, // CN_STACK
  { sizeof(Stage),              HT_T_SMALL   ,0 FILL }, // CN_STAGE
  { sizeof(StageSpec),          HT_T_SMALL   ,0 FILL }, // CN_STAGESPEC
  { sizeof(Statement),          HT_T_MEDIUM  ,0 FILL }, // CN_STATEMENT
  { sizeof(Statistics),         HT_INVALID   ,0 FILL }, // CN_STATISTICS
  { sizeof(Stats),              HT_T_SMALL   ,0 FILL }, // CN_STATS
  { sizeof(Stencil),            HT_T_SMALL   ,0 FILL }, // CN_STENCIL
  { sizeof(StepAccum),          HT_INVALID   ,0 FILL }, // CN_STEPACCUM
  { 0,                          HT_INVALID   ,0 FILL }, // CN_STEP
  { sizeof(Steps),              HT_T_LARGE   ,0 FILL }, // CN_STEPS
  { sizeof(StmtList),           HT_T_TINY    ,0 FILL }, // CN_STMTLIST
  { sizeof(Stream),             HT_T_SMALL   ,0 FILL }, // CN_STREAM
  { sizeof(StreamDef),          HT_T_LARGE   ,0 FILL }, // CN_STREAMDEF
  { sizeof(StreamDefRht),       HT_T_LARGE   ,0 FILL }, // CN_STREAMDEFRHT
  { sizeof(StreamSet),          HT_T_LARGE   ,0 FILL }, // CN_STREAMSET
  { sizeof(s32),                HT_INVALID   ,0 FILL }, // CN_STRING_T
  { sizeof(StzList),            HT_T_TINY    ,0 FILL }, // CN_STZLIST
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 FILL }, // CN_STZVAL
  { sizeof(SubPipelines),       HT_T_TINY    ,0 FILL }, // CN_SUBPIPELINES
  { sizeof(SysCall),            HT_INVALID   ,0 FILL }, // CN_SYSCALL
  { sizeof(Schedule),           HT_T_TINY    ,0 FILL }, // CN_SCHEDULE

  { 'T' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__T__
  { sizeof(Table),              HT_T_TINY    ,0 FILL }, // CN_TABLE
  { sizeof(Table),              HT_T_MEDIUM  ,0 FILL }, // CN_TABLEDEF
  { sizeof(TablePtn),           HT_T_TINY    ,0 FILL }, // CN_TABLEPTN
  { sizeof(TablePtnVec),        HT_T_SMALL   ,0 FILL }, // CN_TABLEPTNVEC
  { sizeof(TagSet),             HT_T_MEDIUM  ,0 FILL }, // CN_TAGSET
  { sizeof(Task),               HT_T_SMALL   ,0 FILL }, // CN_TASK
  { sizeof(TaskList),           HT_T_TINY    ,0 FILL }, // CN_TASKLIST
  { sizeof(TaskQueue),          HT_T_TINY    ,0 FILL }, // CN_TEMPNAME
  { sizeof(TccLock),            HT_T_TINY    ,0 FILL }, // CN_TCCLOCK
  { sizeof(TempName),           HT_T_TINY    ,0 FILL }, // CN_TENANT
  { sizeof(Tenant),             HT_T_TINY    ,0 FILL }, // CN_TENANTS
  { sizeof(Tenants),            HT_T_TINY    ,0 FILL }, // CN_TASKQUEUE
  { sizeof(Text),               HT_T_TINY    ,0 FILL }, // CN_TEXT
  { sizeof(TextLarge),          HT_T_LARGE   ,-8 FILL }, // CN_TEXTLARGE
  { sizeof(TextMedium),         HT_T_MEDIUM  ,-8 FILL }, // CN_TEXTMEDIUM
  { sizeof(TextSmall),          HT_T_SMALL   ,-8 FILL }, // CN_TEXTSMALL
  { sizeof(TextStack),          HT_T_MEDIUM  ,0 FILL }, // CN_TEXTSTACK
  { sizeof(TextTiny),           HT_T_TINY    ,-8 FILL }, // CN_TEXTTINY
  { sizeof(Thing),              HT_T_TINY    ,0 FILL }, // CN_THING
  { sizeof(Thread),             HT_T_TINY    ,0 FILL }, // CN_THREAD
  { sizeof(ThreadQueue),        HT_T_TINY    ,0 FILL }, // CN_THREADQUEUE
  { sizeof(ThreadVec),          HT_T_MEDIUM  ,0 FILL }, // CN_THREADVEC
  { sizeof(Timer),              HT_T_TINY    ,0 FILL }, // CN_TIMER
  { sizeof(TinyC),              HT_T_TINY    ,0 FILL }, // CN_TINYC
  { sizeof(Heap),/*correct*/    HT_INVALID   ,0 FILL }, // CN_TINYPERSISTENT
  { sizeof(Heap),/*correct*/    HT_INVALID   ,0 FILL }, // CN_TINYTRANSIENT
  { sizeof(Heap),/*correct*/    HT_INVALID   ,0 FILL }, // CN_TINYVIRTUAL
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 FILL }, // CN_TMEVAL
  { sizeof(ToBktCrw),           HT_T_LARGE   ,0 FILL }, // CN_TOBKTCRW
  { sizeof(ToBktRow),           HT_T_LARGE   ,0 FILL }, // CN_TOBKTROW
  { sizeof(Token),              HT_T_TINY    ,0 FILL }, // CN_TOKEN
  { sizeof(TokenVec),           HT_T_LARGE   ,0 FILL }, // CN_TOKENVEC
  { sizeof(ToNstRow),           HT_T_MEDIUM  ,0 FILL }, // CN_TONSTROW
  { sizeof(Tools),              HT_T_TINY    ,0 FILL }, // CN_TOOLS
  { sizeof(ToPtnCrw),           HT_T_LARGE   ,0 FILL }, // CN_TOPTNCRW
  { sizeof(ToPtnRow),           HT_T_LARGE   ,0 FILL }, // CN_TOPTNROW
  { sizeof(ToRow),              HT_T_MEDIUM  ,0 FILL }, // CN_TOROW
  { sizeof(TraceLock),          HT_T_TINY    ,0 FILL }, // CN_TRACELOCK
  { sizeof(TraceMultiLock),     HT_T_TINY    ,0 FILL }, // CN_TRACEMULTILOCK
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 FILL }, // CN_TSPVAL
  { sizeof(Tub),                HT_T_TINY    ,0 FILL }, // CN_TUB
  { sizeof(Tx),                 HT_T_TINY    ,0 FILL }, // CN_TX
  { sizeof(TxSet),              HT_T_TINY    ,0 FILL }, // CN_TXSET

  { 'U' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__U__
  { sizeof(UnitInt),            HT_T_TINY    ,0 FILL }, // CN_UNITINT
  { sizeof(UT_SkipList),        HT_T_LARGE   ,0 FILL }, // CN_UT_SKIPLIST

  { 'V' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__V__
  { sizeof(Verbose),            HT_T_TINY    ,0 FILL }, // CN_VERBOSE

  { 'W' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__W__
  { sizeof(Wakeup),             HT_INVALID   ,0 FILL }, // CN_WAKEUP
  { sizeof(Wilderness),         HT_INVALID   ,0 FILL }, // CN_WILDERNESS
  { sizeof(Word),               HT_T_TINY    ,0 FILL }, // CN_WORD
  { 0,                          HT_INVALID   ,0 FILL }, // CN_WSPCONVERGENT_t

  { 'X' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__X__

  { 'Y' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__Y__

  { 'Z' - ('A' - 1),            HT_INVALID   ,0 FILL }, // CN__Z__

  { C_VARGS_END,                HT_INVALID   ,0 FILL }  // CN_ENUMS
};

#undef FILL

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
