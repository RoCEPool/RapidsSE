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
#include "eOC.h"
#include "eOK.h"
#include "eOR.h"
#include "ePF.h"
#include "ePP.h"
#include "ePO.h"
#include "ePY.h"
#include "ePX.h"
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

#include "BucketMap.hpp"
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

#include "InternalTables.hpp"

#include "Job.hpp"
#include "Job__Find.hpp"
#include "JobList.hpp"
#include "Jobs.hpp"

#include "Locator.hpp"
#include "LogicUnit.hpp"

#include "Metadata.hpp"
#include <MetaGet.hpp>
#include <backend/ExprEval.hpp>
#include <BucketGet.hpp>

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

#include "RowTablePtn.hpp"

#include "Scanner.hpp"

#include "SeApi.hpp"
#include "Sessions.hpp"

#include "ShardDef.hpp"
#include "ShortCut.hpp"
#include "SeDeputy.hpp"
#include "SePrompt.hpp"
#include "Stage.hpp"
#include "StepAccum.hpp"
#include "StageSpec.hpp"
#include "Steps.hpp"
#include "Stream.hpp"
#include "StreamDefRht.hpp"
#include "StzList.hpp"

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

#include "ToBktCrw.hpp"
#include "ToBktRow.hpp"

#include "TccLock.hpp"
#include "TraceLock.hpp"

#include "Parcel.hpp"
#include "Sends.hpp"

#include "graph/PlanConstants.hpp"
#include "graph/PlanNode.hpp"
#include "graph/PlanNodeBuilder.hpp"
#include "graph/RootNode.hpp"
#include "graph/DefinitionNode.hpp"
#include "graph/PrimitiveNode.hpp"
#include "graph/QueryNode.hpp"
#include "graph/PlanDefinitionParser.hpp"

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
  { sizeof(Api),                HT_INVALID   ,0 }, // CN_API
  { sizeof(Arg),                HT_T_TINY    ,0 }, // CN_ARG
  { sizeof(Arrange),            HT_T_TINY    ,0 }, // CN_ARRANGE
  { sizeof(Atomic),             HT_INVALID   ,0 }, // CN_ATOMIC
  { sizeof(AttachMap),          HT_INVALID   ,0 }, // CN_ATTACHMAP

  { 'B' - ('A' - 1),            HT_INVALID   ,0 }, // CN__B__
  { sizeof(Backend),/*yes*/     HT_T_MEDIUM  ,0 }, // CN_BACKEND
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 }, // CN_BCDVAL
  { sizeof(Birth),              HT_INVALID   ,0 }, // CN_BIRTH
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 }, // CN_BOLVAL
  { sizeof(Bucket),             HT_T_TINY    ,0 }, // CN_BUCKET
  { sizeof(BucketGet),          HT_T_TINY    ,0 }, // CN_BUCKETGET
  { sizeof(BucketMap),          HT_T_LARGE   ,0 }, // CN_BUCKETMAP
  { sizeof(BucketVec),          HT_T_SMALL   ,0 }, // CN_BUCKETVEC
  { sizeof(BufferMap),          HT_INVALID   ,0 }, // CN_BUFFERMAP
  { sizeof(BufferPool),         HT_INVALID   ,0 }, // CN_BUFFERPOOL
  { sizeof(ByteRate),           HT_T_TINY    ,0 }, // CN_BYTERATE
  { sizeof(s32),                HT_INVALID   ,0 }, // CN_BYTES_T

  { 'C' - ('A' - 1),            HT_INVALID   ,0 }, // CN__C__
  { sizeof(Catalog),            HT_INVALID   ,0 }, // CN_CATALOG
  { sizeof(CellSingle),         HT_INVALID   ,0 }, // CN_CELLSIGN:E
  { sizeof(Charser),            HT_T_LARGE   ,0 }, // CN_CHARSER
  { sizeof(ClassPlaceHolder),   HT_T_LARGE   ,0 }, // CN_CLASSPLACEHOLDER
  { sizeof(CmdLineArgs),        HT_INVALID   ,0 }, // CN_CMDLINEARGS
  { sizeof(CodeBlock),          HT_T_SMALL   ,0 }, // CN_CODEBLOCK
  { sizeof(Column),             HT_T_TINY    ,0 }, // CN_COLUMN
  { sizeof(CrvBuffer),          HT_T_LARGE   ,0 }, // CN_CRVBUFFER
  { sizeof(CrwBuffer),          HT_T_LARGE   ,0 }, // CN_CRWBUFFER
  { sizeof(CrwTablePtn),        HT_T_LARGE   ,0 }, // CN_CRWTABLEPTN
  { sizeof(CrwToRow),           HT_T_TINY    ,0 }, // CN_CRWTOROW
  { sizeof(CsvBuffer),          HT_T_LARGE   ,0 }, // CN_CSVBUFFER
  { sizeof(CsvMeta),            HT_T_TINY    ,0 }, // CN_CSVMETA

  { 'D' - ('A' - 1),            HT_INVALID   ,0 }, // CN__D__
  { sizeof(DdlLock),            HT_T_TINY    ,0 }, // CN_DDLLOCK
  { sizeof(DecaCnt),            HT_T_TINY    ,0 }, // CN_DECACNT
  { sizeof(Decider),            HT_T_TINY    ,0 }, // CN_DECIDER
  { sizeof(DefinitionNode),     HT_T_SMALL   ,-8 }, // CN_DEFINITIONNODE
  { sizeof(DevIXT),             HT_T_TINY    ,0 }, // CN_DEVIXT
  { sizeof(DevROW),             HT_T_TINY    ,0 }, // CN_DEVROW
  { sizeof(Dic),                HT_T_TINY    ,0 }, // CN_DIC
  { sizeof(DicLock),            HT_T_TINY    ,0 }, // CN_DICLOCK
  { sizeof(DicStack),           HT_T_TINY    ,0 }, // CN_DICSTACK
  { sizeof(Dir),                HT_T_TINY    ,0 }, // CN_DIR
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 }, // CN_DTEVAL
  { sizeof(Dynamo),             HT_T_TINY    ,0 }, // CN_DYNAMO

  { 'E' - ('A' - 1),            HT_INVALID   ,0 }, // CN__E__
  { sizeof(EachRowBuffer),      HT_T_MEDIUM  ,0 }, // CN_EACHROWBUFFER
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 }, // CN_EDTVAL
  { sizeof(Encoding),           HT_T_MEDIUM  ,0 }, // CN_ENCODING
  { sizeof(Ent),                HT_INVALID   ,0 }, // CN_ENT
  { sizeof(EntBuffer),          HT_T_LARGE   ,0 }, // CN_ENTBUFFER
  { sizeof(EntBufSorter),       HT_T_LARGE   ,0 }, // CN_ENTBUFSORTER
  { sizeof(EntWrk),             HT_INVALID   ,0 }, // CN_ENTWRK
  { sizeof(ClassFlags),         HT_T_TINY    ,0 }, // CN_ENUMFLAGS
  { sizeof(Exemplar),           HT_T_TINY    ,0 }, // CN_EXEMPLAR
  { sizeof(ExprEval),           HT_T_LARGE   ,0 }, // CN_EXPREVAL

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
  { sizeof(Grid),               HT_T_SMALL   ,0 }, // CN_GRID

  { 'H' - ('A' - 1),            HT_INVALID   ,0 }, // CN__H__
  { sizeof(HashBuffer),         HT_T_LARGE   ,0 }, // CN_HASHBUFFER
  { sizeof(HashSet),            HT_T_LARGE   ,0 }, // CN_HASHSET
  { sizeof(HashMap),            HT_T_TINY    ,0 }, // CN_HASHMAP
  { sizeof(HeapBuffer),         HT_INVALID   ,0 }, // CN_HEAPBUFFER
  { sizeof(Heaps),              HT_INVALID   ,0 }, // CN_HEAPMEM

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
  { sizeof(InternalTables),     HT_T_SMALL   ,0 }, // CN_INTERNALTABLES
  { sizeof(IntSet),             HT_T_TINY    ,0 }, // CN_INTSET
  { sizeof(IntStack),           HT_T_TINY    ,0 }, // CN_INTSTACK
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 }, // CN_INTVAL
  { sizeof(IoChunk),            HT_T_LARGE   ,0 }, // CN_IOCHUNK
  { sizeof(IoCopy),             HT_T_TINY    ,0 }, // CN_IOCOPY
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 }, // CN_IVLVAL

  { 'J' - ('A' - 1),            HT_INVALID   ,0 }, // CN__J__
  { sizeof(Job),                HT_T_MEDIUM  ,0 }, // CN_JOB
  { sizeof(Job__Find),          HT_T_MEDIUM  ,0 }, // CN_JOB__FIND
  { sizeof(JobList),            HT_T_TINY    ,0 }, // CN_JOBLIST
  { sizeof(Jobs),               HT_T_TINY    ,0 }, // CN_JOBS

  { 'K' - ('A' - 1),            HT_INVALID   ,0 }, // CN__K__
  { sizeof(Key),                HT_INVALID   ,0 }, // CN_KEY

  { 'L' - ('A' - 1),            HT_INVALID   ,0 }, // CN__L__
  { sizeof(Heap), /*correct*/   HT_INVALID   ,0 }, // CN_LARGEPERSISTENT
  { sizeof(Heap), /*correct*/   HT_INVALID   ,0 }, // CN_LARGETRANSIENT
  { sizeof(Heap), /*correct*/   HT_INVALID   ,0 }, // CN_LARGEVIRTUAL
  { sizeof(Licence),            HT_T_TINY    ,0 }, // CN_LICENCE
  { sizeof(Limits),             HT_T_TINY    ,0 }, // CN_LIMITS
  { sizeof(ListExoNode_t<Object>), HT_T_TINY ,0 }, // CN_LISTEXONODE_t
  { sizeof(ListExoIter_t<Object>), HT_T_TINY ,0 }, // CN_LISTEXOITER_t
  { sizeof(LoadChunk),          HT_T_LARGE   ,0 }, // CN_LOADCHUNK
  { sizeof(Local),              HT_INVALID   ,0 }, // CN_LOCAL
  { sizeof(LocalChain),         HT_T_LARGE   ,0 }, // CN_LOCALCHAIN
  { sizeof(LocalDir),           HT_INVALID   ,0 }, // CN_LOCALDIR
  { sizeof(LocalFile),          HT_INVALID   ,0 }, // CN_LOCALFILE
  { sizeof(BASE::Location),     HT_T_TINY    ,0 }, // CN_LOCATION
  { sizeof(Locator),            HT_T_TINY    ,0 }, // CN_LOCATOR
  { sizeof(Lock),               HT_INVALID   ,0 }, // CN_LOCK
  { 0,                          HT_INVALID   ,0 }, // CN_LOCKSET
  { sizeof(LogicCompiler),      HT_T_LARGE   ,0 }, // CN_LOGICCOMPILER
  { sizeof(LogicTimer),         HT_T_LARGE   ,0 }, // CN_LOGICTIMER
  { sizeof(LogicUnit),          HT_T_LARGE   ,0 }, // CN_LOGICUNIT

  { 'M' - ('A' - 1),            HT_INVALID   ,0 }, // CN__M__
  { sizeof(Marker),             HT_T_TINY    ,0 }, // CN_MARKER
  { sizeof(Heap), /*correct*/   HT_INVALID   ,0 }, // CN_MEDIUMPERSISTENT
  { sizeof(Heap), /*correct*/   HT_INVALID   ,0 }, // CN_MEDIUMTRANSIENT
  { sizeof(Heap), /*correct*/   HT_INVALID   ,0 }, // CN_MEDIUMTVIRTUAL
  { sizeof(MemTest),            HT_T_TINY    ,0 }, // CN_MEMTEST
  { sizeof(Metadata),           HT_T_TINY    ,0 }, // CN_METADATA
  { sizeof(MetaGet),           HT_T_TINY     ,0 }, // CN_METAGET
  { sizeof(Mutex),              HT_T_TINY    ,0 }, // CN_MUTEX

  { 'N' - ('A' - 1),            HT_INVALID   ,0 }, // CN__N__
  { sizeof(NamedBuffer),        HT_T_LARGE   ,0 }, // CN_NESTEDPIPELINESPEC
  { 0,                          HT_INVALID   ,0 }, // CN_NEXTGID
  { sizeof(NextGid),            HT_INVALID   ,0 }, // CN_NEXTGID
  { sizeof(NmlStr),             HT_T_MEDIUM  ,0 }, // CN_NMLSTR
  { sizeof(Node),               HT_T_MEDIUM  ,0 }, // CN_NODE
  { sizeof(Node_I),             HT_T_TINY    ,0 }, // CN_NODE_I
  { sizeof(Now),                HT_T_TINY    ,0 }, // CN_NOW
  { sizeof(Numbat),             HT_T_LARGE   ,0 }, // CN_NUMBAT
  { sizeof(Numbering),          HT_T_MEDIUM  ,0 }, // CN_NUMBERING

  { 'O' - ('A' - 1),            HT_INVALID   ,0 }, // CN__O__
  { sizeof(ObjBuffer),          HT_T_LARGE   ,0 }, // CN_OBJBUFFER
  { sizeof(ObjectSnapshot),     HT_T_SMALL   ,0 }, // CN_OBJECTSNAPSHOT
  { sizeof(Output),             HT_T_TINY    ,0 }, // CN_OUTPUT
  { sizeof(Op),                 HT_T_LARGE   ,0 }, // CN_OP
  { sizeof(OpenFiles),          HT_T_TINY    ,0 }, // CN_OPENFILES
  { sizeof(OperandQueue),       HT_T_TINY    ,0 }, // CN_OPERANDQUEUE
  { sizeof(OpSpec),             HT_T_LARGE   ,0 }, // CN_OPSPEC
  { sizeof(OtherOperand),       HT_T_TINY    ,0 }, // CN_OTHEROPERAND

  { 'P' - ('A' - 1),              HT_INVALID   ,0 }, // CN__P__
  { sizeof(Parcel),               HT_T_TINY    ,0 }, // CN_PARCEL
  { sizeof(PathIdentifier),       HT_T_MEDIUM  ,0 }, // CN_PATHIDENTIFIER
  { sizeof(PathVec),              HT_T_MEDIUM  ,0 }, // CN_PATHVEC
  { sizeof(Peruse),               HT_T_MEDIUM  ,0 }, // CN_PERUSE
  { sizeof(Pipeline),             HT_T_SMALL   ,0 }, // CN_PIPELINE
  { sizeof(PipelineSpec),         HT_T_MEDIUM  ,0 }, // CN_PIPELINESPEC
  { sizeof(PlanDefinitionParser), HT_T_MEDIUM  , -8 }, // CN_PLANDEFINITIONPARSER
  { sizeof(PlannerLock),          HT_T_TINY    ,0 }, // CN_PLANNERLOCK
  { sizeof(PlanNode),             HT_T_TINY    , -8 }, // CN_PLANNODE
  { sizeof(PlanNodeBuilder),      HT_T_SMALL   ,0 }, // CN_PLANNODEBUILDER
  { sizeof(PlanNodeBuilderList),  HT_T_TINY    ,0 }, // CN_PLANNODEBUILDERLIST
  { sizeof(PlanNodeBuilderStack), HT_T_MEDIUM  ,0 }, // CN_PLANNODEBUILDERSTACK
  { sizeof(PlanNodeList),         HT_T_TINY    ,0 }, // CN_PLANNODELIST
  { sizeof(PlanOperand),          HT_T_TINY    ,0 }, // CN_PLANOPERAND
  { sizeof(Pot),                  HT_T_LARGE   ,0 }, // CN_POT
  { sizeof(Predicate),            HT_T_TINY    ,0 }, // CN_PREDICATE
  { sizeof(PredicateVec),         HT_T_MEDIUM  ,0 }, // CN_PREDICATEVEC
  { sizeof(PrimitiveNode),        HT_T_SMALL   , -8 }, // CN_PRIMITIVENODE
  { sizeof(Profile),              HT_T_MEDIUM  ,0 }, // CN_PROFILE
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

  { 'R' - ('A' - 1),            HT_INVALID   ,0 }, // CN__R__
  { sizeof(Range),              HT_T_LARGE   ,0 }, // CN_RANGE
  { sizeof(Reader),             HT_T_TINY    ,0 }, // CN_READER
  { sizeof(Record),             HT_T_MEDIUM  ,0 }, // CN_RECORD
  { sizeof(RelOp),              HT_T_TINY    ,0 }, // CN_RELOP
  { sizeof(Resources),          HT_T_TINY    ,0 }, // CN_RESOURCES
  { sizeof(RluDiary),           HT_T_LARGE   ,0 }, // CN_RLUDIARY
  { sizeof(Rand),               HT_INVALID   ,0 }, // CN_RN_RAND
  { sizeof(RootNode),           HT_T_SMALL   ,-8 }, // CN_ROOTNODE
  { sizeof(Row),                HT_T_LARGE   ,0 }, // CN_ROW
  { sizeof(Row),/*correct*/     HT_T_MEDIUM  ,0 }, // CN_ROWAGG
  { 0,                          HT_INVALID   ,0 }, // CN_ROWBOX
  { sizeof(RowBuffer),          HT_T_LARGE   ,0 }, // CN_ROWBUFFER
  { sizeof(RowCompactor),       HT_T_SMALL   ,0 }, // CN_ROWCOMPACTOR
  { sizeof(RowOnce),            HT_T_TINY    ,0 }, // CN_ROWONCE
  { sizeof(RowPix),             HT_INVALID   ,0 }, // CN_ROWPIX
  { sizeof(RowSingle),          HT_T_MEDIUM  ,0 }, // CN_ROWSINGLE
  { sizeof(RowSingleList),      HT_T_TINY    ,0 }, // CN_ROWSINGLELIST
  { sizeof(RowTablePtn),        HT_T_LARGE   ,0 }, // CN_ROWTABLEPTN
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
  { sizeof(Scanner),            HT_T_TINY    ,0 }, // CN_SCANNER
  { sizeof(Schema),             HT_T_TINY    ,0 }, // CN_SCHEMA
  { sizeof(SeApi),              HT_T_LARGE   ,0 }, // CN_SEAPI
  { sizeof(SeDeputy),           HT_T_TINY    ,0 }, // CN_SEDEPUTY
  { sizeof(SeeMe),              HT_INVALID   ,0 }, // CN_SEEME
  { sizeof(SenCompiler),        HT_T_TINY    ,0 }, // CN_SENCOMPILER
  { sizeof(Sends),              HT_T_TINY    ,0 }, // CN_SENDS
  { sizeof(SePrompt),           HT_T_LARGE   ,0 }, // CN_SEPROMPT
  { sizeof(Session),            HT_T_LARGE   ,0 }, // CN_SESSION
  { sizeof(Sessions),           HT_T_TINY    ,0 }, // CN_SESSIONS
  { sizeof(ShardDef),           HT_T_LARGE   ,0 }, // CN_SHARDDEF
  { sizeof(ShortCut),           HT_T_MEDIUM  ,0 }, // CN_SHORTCUT
  { sizeof(ShortCutVec),        HT_T_MEDIUM  ,0 }, // CN_SHORTCUTVEC
  { sizeof(Birth),              HT_INVALID   ,0 }, // CN_SINCE
  { sizeof(SkipListNL),         HT_T_TINY    ,0 }, // CN_SKIPLIST
  { sizeof(SkipNode),           HT_T_TINY    ,0 }, // CN_SKIPNODE
  { sizeof(Slab),               HT_T_LARGE   ,0 }, // CN_SLAB
  { sizeof(SlabAllocator),      HT_T_TINY    ,0 }, // CN_SLABALLOCATOR
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
  { sizeof(StepAccum),          HT_INVALID   ,0 }, // CN_STEPACCUM
  { sizeof(Steps),              HT_T_LARGE   ,0 }, // CN_STEPS
  { sizeof(Stream),             HT_T_SMALL   ,0 }, // CN_STREAM
  { sizeof(StreamDef),          HT_T_LARGE   ,0 }, // CN_STREAMDEF
  { sizeof(StreamDefRht),       HT_T_LARGE   ,0 }, // CN_STREAMDEFRHT
  { sizeof(StreamSet),          HT_T_LARGE   ,0 }, // CN_STREAMSET
  { sizeof(s32),                HT_INVALID   ,0 }, // CN_STRING_T
  { sizeof(StzList),            HT_T_TINY    ,0 }, // CN_STZLIST
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 }, // CN_STZVAL
  { sizeof(SysCall),            HT_INVALID   ,0 }, // CN_SYSCALL
  { sizeof(Schedule),           HT_T_TINY    ,0 }, // CN_SCHEDULE

  { 'T' - ('A' - 1),            HT_INVALID   ,0 }, // CN__T__
  { sizeof(Table),              HT_T_TINY    ,0 }, // CN_TABLE
  { sizeof(Table),              HT_T_MEDIUM  ,0 }, // CN_TABLEDEF
  { sizeof(TablePtn),           HT_T_TINY    ,0 }, // CN_TABLEPTN
  { sizeof(TablePtnVec),        HT_T_SMALL   ,0 }, // CN_TABLEPTNVEC
  { sizeof(TagSet),             HT_T_MEDIUM  ,0 }, // CN_TAGSET
  { sizeof(Task),               HT_T_SMALL   ,0 }, // CN_TASK
  { sizeof(TaskList),           HT_T_TINY    ,0 }, // CN_TASKLIST
  { sizeof(TaskQueue),          HT_T_TINY    ,0 }, // CN_TEMPNAME
  { sizeof(TccLock),            HT_T_TINY    ,0 }, // CN_TCCLOCK
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
  { sizeof(ToBktCrw),           HT_T_LARGE   ,0 }, // CN_TOBKTCRW
  { sizeof(ToBktRow),           HT_T_LARGE   ,0 }, // CN_TOBKTROW
  { sizeof(Token),              HT_T_TINY    ,0 }, // CN_TOKEN
  { sizeof(TokenVec),           HT_T_LARGE   ,0 }, // CN_TOKENVEC
  { sizeof(ToNstRow),           HT_T_MEDIUM  ,0 }, // CN_TONSTROW
  { sizeof(ToPtnCrw),           HT_T_LARGE   ,0 }, // CN_TOPTNCRW
  { sizeof(ToPtnRow),           HT_T_LARGE   ,0 }, // CN_TOPTNROW
  { sizeof(ToRow),              HT_T_MEDIUM  ,0 }, // CN_TOROW
  { sizeof(TraceLock),          HT_T_TINY    ,0 }, // CN_TRACELOCK
  { sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 }, // CN_TSPVAL
  { sizeof(Tx),                 HT_T_TINY    ,0 }, // CN_TX
  { sizeof(TxSet),              HT_T_TINY    ,0 }, // CN_TXSET

  { 'U' - ('A' - 1),            HT_INVALID   ,0 }, // CN__U__

  { 'V' - ('A' - 1),            HT_INVALID   ,0 }, // CN__V__
  { sizeof(Verbose),            HT_T_TINY    ,0 }, // CN_VERBOSE

  { 'W' - ('A' - 1),            HT_INVALID   ,0 }, // CN__W__
  { sizeof(Wakeup),             HT_INVALID   ,0 }, // CN_WAKEUP
  { sizeof(Wilderness),         HT_INVALID   ,0 }, // CN_WILDERNESS
  { sizeof(Word),               HT_T_TINY    ,0 }, // CN_WORD
  { 0,                          HT_INVALID   ,0 }, // CN_WSPCONVERGENT_t

  { 'X' - ('A' - 1),            HT_INVALID   ,0 }, // CN__X__

  { 'Y' - ('A' - 1),            HT_INVALID   ,0 }, // CN__Y__

  { 'Z' - ('A' - 1),            HT_INVALID   ,0 }, // CN__Z__

  { C_VARGS_END,                HT_INVALID   ,0 }  // CN_ENUMS
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
