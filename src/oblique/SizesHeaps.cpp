/*============================================================================*/
/* SizesHeaps.cpp -- Class Sizes and Heaps                                    */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "OBLIQUE.h"
#include "PROMPT.h"

#include "LogEntry.hpp"
#include "Log.hpp"


#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "SP_Gid.hpp"


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

/*----------------------------------------------------------------------------*/
/* sizes & heaps                                                              */
/*----------------------------------------------------------------------------*/
SizeHeap gxaSizeHeap[]
{
  { 0,                          HT_INVALID   ,0 }, // CN_NONE

  { 'A' - ('A' - 1),            HT_INVALID   ,0 }, // CN__A__
  { sizeof(AccessPath),         HT_T_MEDIUM  ,0 }, // CN_ACCESSPATH
  { sizeof(AggTimer),           HT_T_TINY    ,0 }, // CN_AGGTIMER
  { sizeof(Arg),                HT_T_TINY    ,0 }, // CN_ARG
  { sizeof(Args),               HT_INVALID   ,0 }, // CN_ARGUMENTS
  { sizeof(Arrange),            HT_T_TINY    ,0 }, // CN_ARRANGE
  { sizeof(Article),            HT_T_TINY    ,0 }, // CN_ARTICLE
  { sizeof(Atomic),             HT_INVALID   ,0 }, // CN_ATOMIC
  { sizeof(AttachMap),          HT_INVALID   ,0 }, // CN_ATTACHMAP

  { 'B' - ('A' - 1),            HT_INVALID   ,0 }, // CN__B__
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 }, // CN_BCDVAL
  { sizeof(Birth),              HT_INVALID   ,0 }, // CN_BIRTH
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 }, // CN_BOLVAL
  { sizeof(BucketMap),          HT_T_LARGE   ,0 }, // CN_BUCKETMAP
   { sizeof(BufferPool),         HT_INVALID   ,0 }, // CN_BUFFERPOOL
  { sizeof(ByteRate),           HT_T_TINY    ,0 }, // CN_BYTERATE
  { sizeof(s32),                HT_INVALID   ,0 }, // CN_BYTES_T

  { 'C' - ('A' - 1),            HT_INVALID   ,0 }, // CN__C__
  { sizeof(Catalog),            HT_INVALID   ,0 }, // CN_CATALOG
  { sizeof(CellSingle),         HT_INVALID   ,0 }, // CN_CELLSIGN:E
  { sizeof(Charser),            HT_T_MEDIUM  ,0 }, // CN_CHARSER
  { sizeof(Column),             HT_T_TINY    ,0 }, // CN_COLUMN
  { sizeof(Compiler),           HT_T_TINY    ,0 }, // CN_COMPILER
  { sizeof(CrvBuffer),          HT_T_LARGE   ,0 }, // CN_CRVBUFFER
  { sizeof(CrwBuffer),          HT_T_LARGE   ,0 }, // CN_CRWBUFFER
  { sizeof(CrwToRow),           HT_T_LARGE   ,0 }, // CN_CRWTOROW
  { sizeof(CsvBuffer),          HT_T_LARGE   ,0 }, // CN_CSVBUFFER
  { sizeof(CsvMeta),            HT_T_TINY    ,0 }, // CN_CSVMETA

  { 'D' - ('A' - 1),            HT_INVALID   ,0 }, // CN__D__
  { sizeof(DecaCnt),            HT_T_TINY    ,0 }, // CN_DECACNT
  { sizeof(Decider),            HT_T_TINY    ,0 }, // CN_DECIDER
  { sizeof(DefinitionNode),     HT_T_TINY    ,-8 }, // CN_DEFINITIONNODE
  { sizeof(DevIXT),             HT_T_TINY    ,0 }, // CN_DEVIXT
  { sizeof(DevROW),             HT_T_TINY    ,0 }, // CN_DEVROW
  { sizeof(Dic),                HT_T_TINY    ,0 }, // CN_DIC
  { sizeof(DicStack),           HT_T_TINY    ,0 }, // CN_DICSTACK
  { sizeof(Dir),                HT_T_TINY    ,0 }, // CN_DIR
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 }, // CN_DTEVAL
  { sizeof(Dynamo),             HT_T_TINY    ,0 }, // CN_DYNAMO

  { 'E' - ('A' - 1),            HT_INVALID   ,0 }, // CN__E__
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 }, // CN_EDTVAL
  { sizeof(Encoding),           HT_T_MEDIUM  ,0 }, // CN_ENCODING
  { sizeof(Encoding_v2),        HT_T_MEDIUM  ,0 }, // CN_ENCODING_V2
  { sizeof(Ent),                HT_INVALID   ,0 }, // CN_ENT
  { sizeof(EntBuffer),          HT_T_LARGE   ,0 }, // CN_ENTBUFFER
  { sizeof(EntBufSorter),       HT_T_LARGE   ,0 }, // CN_ENTBUFSORTER
  { sizeof(EntWrk),             HT_INVALID   ,0 }, // CN_ENTWRK
  { sizeof(EnumFlags),          HT_T_TINY    ,0 }, // CN_ENUMFLAGS
  { sizeof(Exemplar),           HT_T_TINY    ,0 }, // CN_EXEMPLAR

  { 'F' - ('A' - 1),            HT_INVALID   ,0 }, // CN__F__
  { sizeof(Fifo),               HT_T_TINY    ,0 }, // CN_FIFO
  { sizeof(File),               HT_T_TINY    ,0 }, // CN_FILE
  { sizeof(TypedCell), /*yes*/  HT_INVALID   ,0 }, // CN_FIPVAL
  { sizeof(TypedCell), /*yes*/  HT_INVALID   ,0 }, // CN_FLTVAL
  { sizeof(Format),             HT_T_TINY    ,0 }, // CN_FORMAT
  { sizeof(PoolBuffer),/*yes*/  HT_INVALID   ,0 }, // CN_FREEBUFFER
  { sizeof(FreeChunk),          HT_INVALID   ,0 }, // CN_FREECHUNK
  { 0,                          HT_INVALID   ,0 }, // CN_FREEDNOT
  { 0,                          HT_INVALID   ,0 }, // CN_FREEITEM
  { sizeof(FreqDist),           HT_T_SMALL   ,0 }, // CN_FREQDIST

  { 'G' - ('A' - 1),            HT_INVALID   ,0 }, // CN__G__
  { sizeof(GraphEditor),        HT_T_TINY    ,0 }, // CN_GRAPHEDITOR
  { sizeof(GraphNav),           HT_T_TINY    ,0 }, // CN_GRAPHNAV
  { sizeof(Grid),               HT_T_SMALL   ,0 }, // CN_GRID

  { 'H' - ('A' - 1),            HT_INVALID   ,0 }, // CN__H__
  { sizeof(HashBuffer),         HT_T_LARGE   ,0 }, // CN_HASHBUFFER
  { sizeof(HashSet),            HT_T_LARGE   ,0 }, // CN_HASHSET
  { sizeof(HashMap),            HT_T_TINY    ,0 }, // CN_HASHMAP
  { sizeof(HeapBuffer),         HT_INVALID   ,0 }, // CN_HEAPBUFFER
  { sizeof(HeapMem),            HT_INVALID   ,0 }, // CN_HEAPMEM

  { 'I' - ('A' - 1),            HT_INVALID   ,0 }, // CN__I__
  { sizeof(Index),              HT_T_TINY    ,0 }, // CN_INDEX
  { sizeof(IndexDef),           HT_T_MEDIUM  ,0 }, // CN_INDEXDEF
  { sizeof(IndexStream),        HT_T_TINY    ,0 }, // CN_INDEXSTREAM
  { sizeof(IndexTree),          HT_T_TINY    ,0 }, // CN_INDEXTREE
  { sizeof(IndexTreeVec),       HT_T_MEDIUM  ,0 }, // CN_INDEXTREEVEC
  { sizeof(IdxVec),             HT_T_MEDIUM  ,0 }, // CN_IDXVEC
  { sizeof(InfoSet),            HT_T_LARGE   ,0 }, // CN_INFOSET
  { sizeof(IngestChunk),        HT_T_LARGE   ,0 }, // CN_INGESTCHUNK
  { sizeof(Input),              HT_T_TINY    ,0 }, // CN_INPUT
  { sizeof(IntSet),             HT_T_TINY    ,0 }, // CN_INTSET
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 }, // CN_INTVAL
  { sizeof(IoChunk),            HT_T_LARGE   ,0 }, // CN_IOCHUNK
  { sizeof(IoCopy),             HT_T_TINY    ,0 }, // CN_IOCOPY
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 }, // CN_IVLVAL

  { 'J' - ('A' - 1),            HT_INVALID   ,0 }, // CN__J__
  { sizeof(Job),                HT_T_MEDIUM  ,0 }, // CN_JOB
  { sizeof(Job__Find),          HT_T_MEDIUM  ,0 }, // CN_JOB__FIND
  { sizeof(JobList),            HT_T_TINY    ,0 }, // CN_JOBLIST

  { 'K' - ('A' - 1),            HT_INVALID   ,0 }, // CN__K__
  { sizeof(Key),                HT_INVALID   ,0 }, // CN_KEY

  { 'L' - ('A' - 1),            HT_INVALID   ,0 }, // CN__L__
  { sizeof(Heap), /*correct*/   HT_INVALID   ,0 }, // CN_LARGEPERSISTENT
  { sizeof(Heap), /*correct*/   HT_INVALID   ,0 }, // CN_LARGETRANSIENT
  { sizeof(Heap), /*correct*/   HT_INVALID   ,0 }, // CN_LARGEVIRTUAL
  { sizeof(Lexicon),            HT_T_TINY    ,0 }, // CN_LEXICON
  { sizeof(Licence),            HT_T_TINY    ,0 }, // CN_LICENCE
  { sizeof(Limits),             HT_T_TINY    ,0 }, // CN_LIMITS
  { sizeof(ListExoNode_t<Object>), HT_T_TINY ,0 }, // CN_LISTEXONODE_t
  { sizeof(LoadChunk),          HT_T_LARGE   ,0 }, // CN_LOADCHUNK
  { sizeof(Local),              HT_INVALID   ,0 }, // CN_LOCAL
  { sizeof(LocalChain),         HT_T_LARGE   ,0 }, // CN_LOCALSCHAIN
  { sizeof(Lock),               HT_INVALID   ,0 }, // CN_LOCK
  { sizeof(Log),                HT_INVALID   ,0 }, // CN_LOG
  { sizeof(LogEntry),           HT_INVALID   ,0 }, // CN_LOGENTRY
  { sizeof(LogicCompiler),      HT_T_LARGE   ,0 }, // CN_LOGICCOMPILER
  { sizeof(LogicTimer),         HT_T_LARGE   ,0 }, // CN_LOGICTIMER
  { sizeof(LogicUnit),          HT_T_MEDIUM  ,0 }, // CN_LOGICUNIT

  { 'M' - ('A' - 1),            HT_INVALID   ,0 }, // CN__M__
  { sizeof(Marker),             HT_T_TINY    ,0 }, // CN_MARKER
  { sizeof(Heap), /*correct*/   HT_INVALID   ,0 }, // CN_MEDIUMPERSISTENT
  { sizeof(Heap), /*correct*/   HT_INVALID   ,0 }, // CN_MEDIUMTRANSIENT
  { sizeof(Heap), /*correct*/   HT_INVALID   ,0 }, // CN_MEDIUMTVIRTUAL
  { sizeof(MemTest),            HT_T_TINY    ,0 }, // CN_MEMTEST
  { sizeof(MetadataNode),       HT_T_TINY    ,-8 }, // CN_METADATANODE
  { sizeof(MutablePlanNode),    HT_T_TINY    ,-8 }, // CN_MUTABLEPLANNODE
  { sizeof(Mutex),              HT_T_TINY    ,0 }, // CN_MUTEX

  { 'N' - ('A' - 1),            HT_INVALID   ,0 }, // CN__N__
  { sizeof(NextGid),            HT_INVALID   ,0 }, // CN_NEXTGID
  { sizeof(NmlStr),             HT_T_MEDIUM  ,0 }, // CN_NMLSTR
  { sizeof(Node),               HT_T_TINY    ,0 }, // CN_NODE
  { sizeof(Node_I),             HT_T_TINY    ,0 }, // CN_NODE_I
  { sizeof(Now),                HT_T_TINY    ,0 }, // CN_NOW
  { sizeof(Numbat),             HT_T_LARGE   ,0 }, // CN_NUMBAT

  { 'O' - ('A' - 1),            HT_INVALID   ,0 }, // CN__O__
  { sizeof(Op),                 HT_T_LARGE   ,0 }, // CN_OP
  { sizeof(OpenFiles),          HT_T_TINY    ,0 }, // CN_OPENFILES
  { sizeof(OperandQueue),       HT_T_TINY    ,0 }, // CN_OPERANDQUEUE
  { sizeof(OpSpec),             HT_T_LARGE   ,0 }, // CN_OPSPEC

  { 'P' - ('A' - 1),              HT_INVALID   ,0 }, // CN__P__
  { sizeof(PathVec),              HT_T_MEDIUM  ,0 }, // CN_PATHVEC
  { sizeof(Peruse),               HT_T_MEDIUM  ,0 }, // CN_PERUSE
  { sizeof(Pipeline),             HT_T_SMALL   ,0 }, // CN_PIPELINE
  { sizeof(PipelineSpec),         HT_T_SMALL   ,0 }, // CN_PIPELINESPEC
  { sizeof(Plan),                 HT_T_MEDIUM  ,0 }, // CN_PLAN
  { sizeof(PlanDefinitionParser), HT_T_MEDIUM  ,  -8 }, // CN_PLANDEFINITIONPARSER
  { sizeof(Planner),              HT_T_MEDIUM  ,0 }, // CN_PLANNER
  { sizeof(PlanNode),             HT_T_TINY    , -8 }, // CN_PLANNODE
  { sizeof(PlanNodeBuilder),      HT_T_TINY    ,0 }, // CN_PLANNODEBUILDER
  { sizeof(PlanNodeBuilderList),  HT_T_TINY    ,0 }, // CN_PLANNODEBUILDERLIST
  { sizeof(PlanNodeBuilderStack), HT_T_MEDIUM  ,0 }, // CN_PLANNODEBUILDERSTACK
  { sizeof(PlanNodeList),         HT_T_TINY    ,0 }, // CN_PLANNODELIST
  { sizeof(ObjectSnapshot),       HT_T_SMALL   ,0 }, // CN_OBJECTSNAPSHOT
  { sizeof(Pot),                  HT_T_LARGE   ,0 }, // CN_POT
  { sizeof(Predicate),            HT_T_TINY    ,0 }, // CN_PREDICATE
  { sizeof(PredicateVec),         HT_T_MEDIUM  ,0 }, // CN_PREDICATEVEC
  { sizeof(PrimitiveNode),        HT_T_TINY    , -8 }, // CN_PRIMITIVENODE
  { sizeof(Profile),              HT_T_MEDIUM  ,0 }, // CN_PROFILE
  { sizeof(Project),              HT_T_LARGE   ,0 }, // CN_PROJECT
  { sizeof(PsxThd),               HT_T_TINY    ,0 }, // CN_PSXTHD
  { sizeof(PtnMap),               HT_T_LARGE   ,0 }, // CN_PTNMAP
  { sizeof(PtnStream),            HT_T_TINY    ,0 }, // CN_PTNSTREAM
  { sizeof(TypedCell),/*yes*/     HT_INVALID   ,0 }, // CN_PTRVAL

  { 'Q' - ('A' - 1),            HT_INVALID   ,0 }, // CN__Q__
  { sizeof(QueryNode),          HT_T_TINY    ,-8 }, // CN_QUERYNODE
  { sizeof(QueryNodeList),      HT_T_TINY    ,0 }, // CN_QUERYNODELIST
  { sizeof(QueryNodeStack),     HT_T_MEDIUM  ,0 }, // CN_QUERYNODESTACK
  { sizeof(Queue),              HT_T_TINY    ,0 }, // CN_QUEUE
  { sizeof(QueueBuffer),        HT_INVALID   ,0 }, // CN_QUEUEBUFFER
  { sizeof(Queues),             HT_T_TINY    ,0 }, // CN_QUEUES

  { 'R' - ('A' - 1),            HT_INVALID   ,0 }, // CN__R__
  { sizeof(Range),              HT_T_LARGE   ,0 }, // CN_RANGE
  { sizeof(ReadApi),            HT_T_TINY    ,0 }, // CN_READAPI
  { sizeof(Reader),             HT_T_TINY    ,0 }, // CN_READER
  { sizeof(Record),             HT_T_MEDIUM  ,0 }, // CN_RECORD
  { sizeof(RelOp),              HT_T_TINY    ,0 }, // CN_RELOP
  { sizeof(ResourceTracker),    HT_T_TINY    ,0 }, // CN_RESOURCETRACKER
  { sizeof(Response),           HT_T_MEDIUM  ,0 }, // CN_RESPONSE
  { sizeof(ResponseVec),        HT_T_SMALL   ,0 }, // CN_RESPONSEVEC
  { sizeof(Result),             HT_T_MEDIUM  ,0 }, // CN_RESULT
  { sizeof(RluDiary),           HT_T_LARGE   ,0 }, // CN_RLUDIARY
  { sizeof(Rand),               HT_INVALID   ,0 }, // CN_RN_RAND
  { sizeof(RootNode),           HT_T_TINY    ,-8 }, // CN_ROOTNODE
  { sizeof(Routine),            HT_T_SMALL   ,0 }, // CN_ROUTINE
  { sizeof(Row),                HT_T_LARGE   ,0 }, // CN_ROW
  { sizeof(Row),/*correct*/     HT_T_MEDIUM  ,0 }, // CN_ROWAGG
  { 0,                          HT_INVALID   ,0 }, // CN_ROWBOX
  { sizeof(RowBuffer),          HT_T_LARGE   ,0 }, // CN_ROWBUFFER
  { sizeof(RowCompactor),       HT_T_SMALL   ,0 }, // CN_ROWCOMPACTOR
  { sizeof(RowOnce),            HT_T_TINY    ,0 }, // CN_ROWONCE
  { sizeof(RowSingle),          HT_T_MEDIUM  ,0 }, // CN_ROWSINGLE
  { sizeof(RowTray),            HT_T_LARGE   ,0 }, // CN_ROWTRAY
  { sizeof(RU_Cpu),             HT_INVALID   ,0 }, // CN_RUCPU
  { 0,/*sizeof(RU_Dir)*/        HT_INVALID   ,0 }, // CN_RUDIR
  { 0,/*sizeof(RU_File)*/       HT_INVALID   ,0 }, // CN_RUFILE
  { sizeof(RU_Memory),          HT_INVALID   ,0 }, // CN_RUMEMORY
  { 0,/*sizeof(RU_Mutex)*/      HT_INVALID   ,0 }, // CN_RUMUTEX
  { 0,/*sizeof(RU_Pass)*/       HT_INVALID   ,0 }, // CN_RUPASS
  { 0,/*sizeof(RU_Socket)*/     HT_INVALID   ,0 }, // CN_RUSOCKET
  { 0,/*sizeof(RU_Time)*/       HT_INVALID   ,0 }, // CN_RUTIME
  { 0,/*sizeof(RU_Word)*/       HT_INVALID   ,0 }, // CN_RUWORK

  { 'S' - ('A' - 1),            HT_INVALID   ,0 }, // CN__S__
  { sizeof(Schema),             HT_T_TINY    ,0 }, // CN_SCHEMA
  { sizeof(SeDeputy),           HT_T_TINY    ,0 }, // CN_SEDEPUTY
  { sizeof(SeeMe),              HT_INVALID   ,0 }, // CN_SEEME
  { sizeof(SePrompt),           HT_T_LARGE   ,0 }, // CN_SEPROMPT
  { sizeof(Session),            HT_T_MEDIUM  ,0 }, // CN_SESSION
  { sizeof(SessionArray),       HT_T_TINY    ,0 }, // CN_SESSIONARRAY
  { sizeof(ShortCut),           HT_T_MEDIUM  ,0 }, // CN_SHORTCUT
  { sizeof(ShortCutVec),        HT_T_MEDIUM  ,0 }, // CN_SHORTCUTVEC
  { sizeof(Birth),              HT_INVALID   ,0 }, // CN_SINCE
  { sizeof(SkipListNL),         HT_T_TINY    ,0 }, // CN_SKIPLIST
  { sizeof(SkipNode),           HT_T_TINY    ,0 }, // CN_SKIPNODE
  { sizeof(Slab),               HT_T_LARGE   ,0 }, // CN_SLAB
  { sizeof(SlabAllocator),      HT_T_TINY    ,0 }, // CN_SLABALLOCATOR
  { sizeof(Sleep),              HT_INVALID   ,0 }, // CN_SLEEP
  { sizeof(Heap),/*correct*/    HT_INVALID   ,0 }, // SN_SMALLPERSISTENT
  { sizeof(Heap),/*correct*/    HT_INVALID   ,0 }, // SN_SMALLTRANSIENT
  { sizeof(Heap),/*correct*/    HT_INVALID   ,0 }, // SN_SMALLVIRTUAL
  { sizeof(SN_Bcd),             HT_INVALID   ,0 }, // CN_SNBCD
  { sizeof(SN_Bol),             HT_INVALID   ,0 }, // CN_SNBOL
  { sizeof(SN_Dte),             HT_INVALID   ,0 }, // CN_SNDTE
  { sizeof(SN_Flt),             HT_INVALID   ,0 }, // CN_SNFLT
  { sizeof(SN_Int),             HT_INVALID   ,0 }, // CN_SNINT
  { sizeof(SN_Ivl),             HT_INVALID   ,0 }, // CN_SNIVL
  { sizeof(SN_Tme),             HT_INVALID   ,0 }, // CN_SNTME
  { sizeof(SN_Tsp),             HT_INVALID   ,0 }, // CN_SNTSP
  { sizeof(SN_Vch),             HT_INVALID   ,0 }, // CN_SNVCH
  { sizeof(Socket),             HT_T_TINY    ,0 }, // CN_SOCKET
  { sizeof(SP_Elapsed),         HT_INVALID   ,0 }, // CN_SPELAPSED
  { sizeof(SP_Gid),             HT_INVALID   ,0 }, // CN_SPID
  { sizeof(SP_String),          HT_INVALID   ,0 }, // CN_SPSTRING
  { sizeof(SPmedium),           HT_INVALID   ,0 }, // CN_SPMEDIUM
  { sizeof(SPsmall),            HT_INVALID   ,0 }, // CN_SPSMALL
  { sizeof(SqlPlanner),         HT_T_MEDIUM  ,-8 }, // CN_SQLPLANNER
  { sizeof(TypedStack),         HT_T_MEDIUM  ,0 }, // CN_STACK
  { sizeof(Stage),              HT_T_SMALL   ,0 }, // CN_STAGE
  { sizeof(StageSpec),          HT_T_SMALL   ,0 }, // CN_STAGESPEC
  { sizeof(Statistics),         HT_INVALID   ,0 }, // CN_STATISTICS
  { sizeof(StatisticsNode),     HT_T_TINY    ,-8 }, // CN_STATISTICSNODE
  { sizeof(StepAccum),          HT_INVALID   ,0 }, // CN_STEPACCUM
  { sizeof(StepTiming),         HT_T_LARGE   ,0 }, // CN_STEPTIMING
  { sizeof(Stream),             HT_T_SMALL   ,0 }, // CN_STREAM
  { sizeof(StreamDef),          HT_T_LARGE   ,0 }, // CN_STREAMDEF
  { sizeof(StreamSet),          HT_T_LARGE   ,0 }, // CN_STREAMSET
  { sizeof(s32),                HT_INVALID   ,0 }, // CN_STRING_T
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 }, // CN_STZVAL
  { sizeof(SysCall),            HT_INVALID   ,0 }, // CN_SYSCALL

  { 'T' - ('A' - 1),            HT_INVALID   ,0 }, // CN__T__
  { sizeof(Table),              HT_T_TINY    ,0 }, // CN_TABLE
  { sizeof(Table),              HT_T_MEDIUM  ,0 }, // CN_TABLEDEF
  { sizeof(TablePtn),           HT_T_TINY    ,0 }, // CN_TABLEPTN
  { sizeof(TablePtnVec),        HT_T_SMALL   ,0 }, // CN_TABLEPTNVEC
  { sizeof(TagSet),             HT_T_MEDIUM  ,0 }, // CN_TAGSET
  { sizeof(Task),               HT_T_TINY    ,0 }, // CN_TASK
  { sizeof(TaskList),           HT_T_TINY    ,0 }, // CN_TASKLIST
  { sizeof(TaskQueue),          HT_T_TINY    ,0 }, // CN_TEMPNAME
  { sizeof(TempName),           HT_T_TINY    ,0 }, // CN_TENANT
  { sizeof(Tenant),             HT_T_TINY    ,0 }, // CN_TENANTS
  { sizeof(Tenants),            HT_T_TINY    ,0 }, // CN_TASKQUEUE
  { sizeof(Text),               HT_T_TINY    ,0 }, // CN_TEXT
  { sizeof(TextLarge),          HT_T_LARGE   ,-8 }, // CN_TEXTLARGE
  { sizeof(TextMedium),         HT_T_MEDIUM  ,-8 }, // CN_TEXTMEDIUM
  { sizeof(TextSmall),          HT_T_SMALL   ,-8 }, // CN_TEXTSMALL
  { sizeof(TextStack),          HT_T_MEDIUM  ,0 }, // CN_TEXTSTACK
  { sizeof(TextTiny),           HT_T_TINY    ,-8 }, // CN_TEXTTINY
  { sizeof(Thing),              HT_T_TINY    ,0 }, // CN_THING
  { sizeof(Thread),             HT_T_TINY    ,0 }, // CN_THREAD
  { sizeof(ThreadQueue),        HT_T_TINY    ,0 }, // CN_THREADQUEUE
  { sizeof(ThreadVec),          HT_T_MEDIUM  ,0 }, // CN_THREADVEC
  { sizeof(Timer),              HT_T_TINY    ,0 }, // CN_TIMER
  { sizeof(Heap),/*correct*/    HT_INVALID   ,0 }, // CN_TINYPERSISTENT
  { sizeof(Heap),/*correct*/    HT_INVALID   ,0 }, // CN_TINYTRANSIENT
  { sizeof(Heap),/*correct*/    HT_INVALID   ,0 }, // CN_TINYVIRTUAL
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 }, // CN_TMEVAL
  { sizeof(ToHshCrw),           HT_T_LARGE   ,0 }, // CN_TOHSHCRW
  { sizeof(Token),              HT_T_TINY    ,0 }, // CN_TOKEN
  { sizeof(TokenVec),           HT_T_LARGE   ,0 }, // CN_TOKENVEC
  { sizeof(ToNstRow),           HT_T_MEDIUM  ,0 }, // CN_TONSTROW
  { sizeof(ToPtnRow),           HT_T_LARGE   ,0 }, // CN_TOPTNROW
  { sizeof(ToRow),              HT_T_MEDIUM  ,0 }, // CN_TOROW
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 }, // CN_TSPVAL
  { sizeof(Tx),                 HT_T_TINY    ,0 }, // CN_TX
  { sizeof(TxSet),              HT_T_TINY    ,0 }, // CN_TXSET

  { 'U' - ('A' - 1),            HT_INVALID   ,0 }, // CN__U__

  { 'V' - ('A' - 1),            HT_INVALID   ,0 }, // CN__V__

  { 'W' - ('A' - 1),            HT_INVALID   ,0 }, // CN__W__
  { sizeof(Wilderness),         HT_INVALID   ,0 }, // CN_WILDERNESS
  { sizeof(Word),               HT_T_TINY    ,0 }, // CN_WORD
  { 0,                          HT_INVALID   ,0 }, // CN_WSPCONVERGENT_t

  { 'X' - ('A' - 1),            HT_INVALID   ,0 }, // CN__X__
  { sizeof(XmitOperand),        HT_T_TINY    ,0 }, // CN_XMITIOPERAND
  { sizeof(Xmits),              HT_T_TINY    ,0 }, // CN_XMITS

  { 'Y' - ('A' - 1),            HT_INVALID   ,0 }, // CN__Y__

  { 'Z' - ('A' - 1),            HT_INVALID   ,0 }, // CN__Z__

  { C_VARGS_END,                HT_INVALID   ,0 }, // CN_ENUMS
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
