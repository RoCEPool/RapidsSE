/*============================================================================*/
/* SizesHeaps.cpp -- Class Sizes and Heaps                                    */
/*============================================================================*/

#include "Oblique.h"

#include "Dir.hpp"    //?
#include "Fifo.hpp"   //?
#include "File.hpp"   //?
#include "PsxThd.hpp" //?
#include "Socket.hpp" //?

/*----------------------------------------------------------------------------*/
/* sizes & heaps                                                              */
/*----------------------------------------------------------------------------*/
SizeHeap gxaSizeHeap[]
{
  { 0,                          HT_INVALID           ,0 }, // CN_NONE

  { 'A' - ('A' - 1),            HT_INVALID           ,0 }, // CN__A__
  { sizeof(AccessPath),         HT_TRANSIENT_MEDIUM  ,0 }, // CN_ACCESSPATH
  { sizeof(AggTimer),           HT_TRANSIENT_TINY    ,0 }, // CN_AGGTIMER
  { sizeof(Arg),                HT_TRANSIENT_TINY    ,0 }, // CN_ARG
  { sizeof(Args),               HT_INVALID           ,0 }, // CN_ARGUMENTS
  { sizeof(Arrange),            HT_TRANSIENT_TINY    ,0 }, // CN_ARRANGE
  { sizeof(Article),            HT_TRANSIENT_TINY    ,0 }, // CN_ARTICLE
  { sizeof(Atomic),             HT_INVALID           ,0 }, // CN_ATOMIC
  { sizeof(AttachMap),          HT_INVALID           ,0 }, // CN_ATTACHMAP

  { 'B' - ('A' - 1),            HT_INVALID           ,0 }, // CN__B__
  { sizeof(BcdVal),             HT_INVALID           ,0 }, // CN_BCDVAL
  { sizeof(Birth),              HT_INVALID           ,0 }, // CN_BIRTH
  { sizeof(BolVal),             HT_INVALID           ,0 }, // CN_BOLVAL
  { sizeof(BPA),                HT_TRANSIENT_MEDIUM  ,0 }, // CN_BPA
  { sizeof(BucketMap),          HT_TRANSIENT_LARGE   ,0 }, // CN_BUCKETMAP
  { sizeof(BufferCursor),       HT_TRANSIENT_LARGE   ,0 }, // CN_BUFFERCURSOR
  { sizeof(BufferPool),         HT_INVALID           ,0 }, // CN_BUFFERPOOL
  { sizeof(ByteRate),           HT_TRANSIENT_TINY    ,0 }, // CN_BYTERATE
  { sizeof(s32),                HT_INVALID           ,0 }, // CN_BYTES_T

  { 'C' - ('A' - 1),            HT_INVALID           ,0 }, // CN__C__
  { sizeof(CarryOver),          HT_INVALID           ,0 }, // CN_CARRYOVER
  { sizeof(Catalog),            HT_INVALID           ,0 }, // CN_CATALOG
  { sizeof(Charser),            HT_TRANSIENT_MEDIUM  ,0 }, // CN_CHARSER
  { sizeof(Column),             HT_TRANSIENT_TINY    ,0 }, // CN_COLUMN
  { sizeof(Compiler),           HT_TRANSIENT_TINY    ,0 }, // CN_COMPILER
  { sizeof(CrvBuffer),          HT_TRANSIENT_LARGE   ,0 }, // CN_CRVBUFFER
  { sizeof(CrwBuffer),          HT_TRANSIENT_LARGE   ,0 }, // CN_CRWBUFFER
  { sizeof(CrwToRow),           HT_TRANSIENT_LARGE   ,0 }, // CN_CRWTOROW
  { sizeof(CsvBuffer),          HT_TRANSIENT_LARGE   ,0 }, // CN_CSVBUFFER
  { sizeof(CsvMeta),            HT_TRANSIENT_TINY    ,0 }, // CN_CSVMETA

  { 'D' - ('A' - 1),            HT_INVALID           ,0 }, // CN__D__
  { sizeof(Datum_h),            HT_TRANSIENT_MEDIUM  ,0 }, // CN_DATUM
  { sizeof(DecaCnt),            HT_TRANSIENT_TINY    ,0 }, // CN_DECACNT
  { sizeof(Decider),            HT_TRANSIENT_TINY    ,0 }, // CN_DECIDER
  { sizeof(DevIXT),             HT_TRANSIENT_TINY    ,0 }, // CN_DEVIXT
  { sizeof(DevROW),             HT_TRANSIENT_TINY    ,0 }, // CN_DEVROW
  { sizeof(Dic),                HT_TRANSIENT_TINY    ,0 }, // CN_DIC
  { sizeof(DicStack),           HT_TRANSIENT_TINY    ,0 }, // CN_DICSTACK
  { sizeof(Dir),                HT_TRANSIENT_TINY    ,0 }, // CN_DIR
  { sizeof(DteVal),             HT_INVALID           ,0 }, // CN_DTEVAL
  { sizeof(Dynamo),             HT_TRANSIENT_TINY    ,0 }, // CN_DYNAMO

  { 'E' - ('A' - 1),            HT_INVALID           ,0 }, // CN__E__
  { sizeof(EdtVal),             HT_INVALID           ,0 }, // CN_EDTVAL
  { sizeof(Encoding),           HT_TRANSIENT_MEDIUM  ,0 }, // CN_ENCODING
  { sizeof(Encoding_v2),        HT_TRANSIENT_MEDIUM  ,0 }, // CN_ENCODING_V2
  { sizeof(Ent),                HT_INVALID           ,0 }, // CN_ENT
  { sizeof(EntBuffer),          HT_TRANSIENT_LARGE   ,0 }, // CN_ENTBUFFER
  { sizeof(EntBufSorter),       HT_TRANSIENT_LARGE   ,0 }, // CN_ENTBUFSORTER
  { sizeof(EntWrk),             HT_INVALID           ,0 }, // CN_ENTWRK
  { sizeof(EnumFlags),          HT_TRANSIENT_TINY    ,0 }, // CN_ENUMFLAGS
  { sizeof(Exemplar),           HT_TRANSIENT_TINY    ,0 }, // CN_EXEMPLAR
  { sizeof(ExprAtom),           HT_TRANSIENT_TINY    ,0 }, // CN_EXPRATOM
  { sizeof(ExprColumnRef),      HT_TRANSIENT_TINY    ,0 }, // CN_EXPRCOLUMNREF
  { sizeof(ExprList),           HT_TRANSIENT_TINY    ,0 }, // CN_EXPRLIST

  { 'F' - ('A' - 1),            HT_INVALID           ,0 }, // CN__F__
  { sizeof(Fact),               HT_TRANSIENT_SMALL   ,0 }, // CN_FACT
  { sizeof(Fifo),               HT_TRANSIENT_TINY    ,0 }, // CN_FIFO
  { sizeof(File),               HT_TRANSIENT_TINY    ,0 }, // CN_FILE
  { sizeof(FipVal),             HT_INVALID           ,0 }, // CN_FIPVAL
  { sizeof(FltVal),             HT_INVALID           ,0 }, // CN_FLTVAL
  { sizeof(FocalObjects),       HT_TRANSIENT_LARGE   ,0 }, // CN_FOCALOBJECTS
  { sizeof(Forth),              HT_TRANSIENT_TINY    ,0 }, // CN_FORTH
  { sizeof(Format),             HT_TRANSIENT_TINY    ,0 }, // CN_FORMAT
  { sizeof(PoolBuffer),/*yes*/  HT_INVALID           ,0 }, // CN_FREEBUFFER
  { sizeof(FreeChunk),          HT_INVALID           ,0 }, // CN_FREECHUNK
  { 0,                          HT_INVALID           ,0 }, // CN_FREEDNOT
  { 0,                          HT_INVALID           ,0 }, // CN_FREEITEM
  { sizeof(FreqDist),           HT_TRANSIENT_SMALL   ,0 }, // CN_FREQDIST

  { 'G' - ('A' - 1),            HT_INVALID           ,0 }, // CN__G__
  { sizeof(GGadaptor),          HT_TRANSIENT_TINY    ,0 }, // CN_GGADAPTOR
  { sizeof(Grid),               HT_TRANSIENT_SMALL   ,0 }, // CN_GRID

  { 'H' - ('A' - 1),            HT_INVALID           ,0 }, // CN__H__
  { sizeof(HashBuffer),         HT_TRANSIENT_LARGE   ,0 }, // CN_HASHBUFFER
  { sizeof(HashSet),            HT_TRANSIENT_LARGE   ,0 }, // CN_HASHSET
  { sizeof(HashMap),            HT_TRANSIENT_TINY    ,0 }, // CN_HASHMAP
  { sizeof(HeapBuffer),         HT_INVALID           ,0 }, // CN_HEAPBUFFER

  { 'I' - ('A' - 1),            HT_INVALID           ,0 }, // CN__I__
  { sizeof(Index),              HT_TRANSIENT_TINY    ,0 }, // CN_INDEX
  { sizeof(IndexDef),           HT_TRANSIENT_MEDIUM  ,0 }, // CN_INDEXDEF
  { sizeof(IndexStream),        HT_TRANSIENT_TINY    ,0 }, // CN_INDEXSTREAM
  { sizeof(IndexTree),          HT_TRANSIENT_TINY    ,0 }, // CN_INDEXTREE
  { sizeof(IndexTreeVec),       HT_TRANSIENT_MEDIUM  ,0 }, // CN_INDEXTREEVEC
  { sizeof(IdxVec),             HT_TRANSIENT_MEDIUM  ,0 }, // CN_IDXVEC
  { sizeof(InfoSet),            HT_TRANSIENT_LARGE   ,0 }, // CN_INFOSET
  { sizeof(IngestChunk),        HT_TRANSIENT_LARGE   ,0 }, // CN_INGESTCHUNK
  { sizeof(Input),              HT_TRANSIENT_TINY    ,0 }, // CN_INPUT
  { sizeof(IntBuffer),          HT_TRANSIENT_LARGE   ,0 }, // CN_INTBUFFER
  { sizeof(IntSet),             HT_TRANSIENT_TINY    ,0 }, // CN_INTSET
  { sizeof(IntVal),             HT_INVALID           ,0 }, // CN_INTVAL
  { sizeof(IoChunk),            HT_TRANSIENT_LARGE   ,0 }, // CN_IOCHUNK
  { sizeof(IoCopy),             HT_TRANSIENT_TINY    ,0 }, // CN_IOCOPY
  { sizeof(IvlVal),             HT_INVALID           ,0 }, // CN_IVLVAL

  { 'J' - ('A' - 1),            HT_INVALID           ,0 }, // CN__J__
  { sizeof(Job),                HT_TRANSIENT_MEDIUM  ,0 }, // CN_JOB
  { sizeof(Job__Find),          HT_TRANSIENT_MEDIUM  ,0 }, // CN_JOB__FIND
  { sizeof(JobList),            HT_TRANSIENT_TINY    ,0 }, // CN_JOBLIST

  { 'K' - ('A' - 1),            HT_INVALID           ,0 }, // CN__K__
  { sizeof(Kbase),              HT_TRANSIENT_TINY    ,0 }, // CN_KBASE
  { sizeof(Key),                HT_INVALID           ,0 }, // CN_KEY

  { 'L' - ('A' - 1),            HT_INVALID           ,0 }, // CN__L__
  { sizeof(Heap), /*correct*/   HT_INVALID           ,0 }, // CN_LARGEPERSISTENT
  { sizeof(Heap), /*correct*/   HT_INVALID           ,0 }, // CN_LARGETRANSIENT
  { sizeof(Lexicon),            HT_TRANSIENT_TINY    ,0 }, // CN_LEXICON
  { sizeof(Licence),            HT_TRANSIENT_TINY    ,0 }, // CN_LICENCE
  { sizeof(Limits),             HT_TRANSIENT_TINY    ,0 }, // CN_LIMITS
  { sizeof(LoadChunk),          HT_TRANSIENT_LARGE   ,0 }, // CN_LOADCHUNK
  { sizeof(Local),              HT_INVALID           ,0 }, // CN_LOCAL
  { sizeof(LocalChain),         HT_TRANSIENT_LARGE   ,0 }, // CN_LOCALSCHAIN
  { sizeof(Lock),               HT_INVALID           ,0 }, // CN_LOCK
  { sizeof(Log),                HT_INVALID           ,0 }, // CN_LOG
  { sizeof(LogEntry),           HT_INVALID           ,0 }, // CN_LOGENTRY
  { sizeof(LogicCompiler),      HT_TRANSIENT_LARGE   ,0 }, // CN_LOGICCOMPILER
  { sizeof(LogicTimer),         HT_TRANSIENT_LARGE   ,0 }, // CN_LOGICTIMER
  { sizeof(LogicUnit),          HT_TRANSIENT_MEDIUM  ,0 }, // CN_LOGICUNIT
  { sizeof(Lrw),                HT_TRANSIENT_SMALL   ,0 }, // CN_LRW
  { sizeof(LrwBuffer),          HT_TRANSIENT_LARGE   ,0 }, // CN_LRWBUFFER

  { 'M' - ('A' - 1),            HT_INVALID           ,0 }, // CN__M__
  { sizeof(Marker),             HT_TRANSIENT_TINY    ,0 }, // CN_MARKER
  { sizeof(Heap), /*correct*/   HT_INVALID           ,0 }, // CN_MEDIUMPERSISTENT
  { sizeof(Heap), /*correct*/   HT_INVALID           ,0 }, // CN_MEDIUMTRANSIENT
  { sizeof(MemTest),            HT_TRANSIENT_TINY    ,0 }, // CN_MEMTEST
  { sizeof(Method),             HT_TRANSIENT_MEDIUM  ,0 }, // CN_METHOD
  { sizeof(Mutex),              HT_TRANSIENT_TINY    ,0 }, // CN_MUTEX

  { 'N' - ('A' - 1),            HT_INVALID           ,0 }, // CN__N__
  { sizeof(NextGid),            HT_INVALID           ,0 }, // CN_NEXTGID
  { sizeof(NmlStr),             HT_TRANSIENT_MEDIUM  ,0 }, // CN_NMLSTR
  { sizeof(Node),               HT_TRANSIENT_TINY    ,0 }, // CN_NODE
  { sizeof(Node_I),             HT_TRANSIENT_TINY    ,0 }, // CN_NODE_I
  { sizeof(Now),                HT_TRANSIENT_TINY    ,0 }, // CN_NOW
  { sizeof(Numbat),             HT_TRANSIENT_LARGE   ,0 }, // CN_NUMBAT

  { 'O' - ('A' - 1),            HT_INVALID           ,0 }, // CN__O__
  { sizeof(ObjectsInPool),      HT_TRANSIENT_SMALL   ,0 }, // CN_OBJECTSINPOOL
  { sizeof(Op),                 HT_TRANSIENT_LARGE   ,0 }, // CN_OP
  { sizeof(OperandQueue),       HT_TRANSIENT_TINY    ,0 }, // CN_OPERANDQUEUE
  { sizeof(OpSpec),             HT_TRANSIENT_LARGE   ,0 }, // CN_OPSPEC

  { 'P' - ('A' - 1),            HT_INVALID           ,0 }, // CN__P__
  { sizeof(PathVec),            HT_TRANSIENT_MEDIUM  ,0 }, // CN_PATHVEC
  { sizeof(Peruse),             HT_TRANSIENT_MEDIUM  ,0 }, // CN_PERUSE
  { sizeof(Pipeline),           HT_TRANSIENT_SMALL   ,0 }, // CN_PIPELINE
  { sizeof(PipelineSpec),       HT_TRANSIENT_SMALL   ,0 }, // CN_PIPELINESPEC
  { sizeof(Plan),               HT_TRANSIENT_MEDIUM  ,0 }, // CN_PLAN
  { sizeof(PlanDefinitionParser),HT_TRANSIENT_MEDIUM ,-8 }, // CN_PLANDEFINITIONPARSER
  { sizeof(Planner),            HT_TRANSIENT_MEDIUM  ,0 }, // CN_PLANNER
  { sizeof(PlanNode),           HT_TRANSIENT_SMALL   ,0 }, // CN_PLANNODE
  { sizeof(Pot),                HT_TRANSIENT_LARGE   ,0 }, // CN_POT
  { sizeof(Predicate),          HT_TRANSIENT_TINY    ,0 }, // CN_PREDICATE
  { sizeof(PredicateVec),       HT_TRANSIENT_MEDIUM  ,0 }, // CN_PREDICATEVEC
  { sizeof(Profile),            HT_TRANSIENT_MEDIUM  ,0 }, // CN_PROFILE
  { sizeof(PsxThd),             HT_TRANSIENT_TINY    ,0 }, // CN_PSXTHD
  { sizeof(PtnColVec),          HT_TRANSIENT_MEDIUM  ,0 }, // CN_PTNCOLVEC
  { sizeof(PtnMap),             HT_TRANSIENT_LARGE   ,0 }, // CN_PTNMAP
  { sizeof(PtnStream),          HT_TRANSIENT_TINY    ,0 }, // CN_PTNSTREAM
  { sizeof(PtrVal),             HT_INVALID           ,0 }, // CN_PTRVAL

  { 'Q' - ('A' - 1),            HT_INVALID           ,0 }, // CN__Q__
  { sizeof(Question),           HT_TRANSIENT_TINY    ,0 }, // CN_QUESTION
  { sizeof(Queue),              HT_TRANSIENT_TINY    ,0 }, // CN_QUEUE
  { sizeof(QueueBuffer),        HT_INVALID           ,0 }, // CN_QUEUEBUFFER
  { sizeof(Queues),             HT_TRANSIENT_TINY    ,0 }, // CN_QUEUES

  { 'R' - ('A' - 1),            HT_INVALID           ,0 }, // CN__R__
  { sizeof(ReadApi),            HT_TRANSIENT_TINY    ,0 }, // CN_READAPI
  { sizeof(Reader),             HT_TRANSIENT_TINY    ,0 }, // CN_READER
  { sizeof(Record),             HT_TRANSIENT_MEDIUM  ,0 }, // CN_RECORD
  { sizeof(RelOp),              HT_TRANSIENT_TINY    ,0 }, // CN_RELOP
  { sizeof(ResourceTracker),    HT_TRANSIENT_TINY    ,0 }, // CN_RESOURCETRACKER
  { sizeof(Response),           HT_TRANSIENT_MEDIUM  ,0 }, // CN_RESPONSE
  { sizeof(ResponseVec),        HT_TRANSIENT_SMALL   ,0 }, // CN_RESPONSEVEC
  { sizeof(Result),             HT_TRANSIENT_MEDIUM  ,0 }, // CN_RESULT
  { sizeof(RluDiary),           HT_TRANSIENT_LARGE   ,0 }, // CN_RLUDIARY
  { sizeof(Rand),               HT_INVALID           ,0 }, // CN_RN_RAND
  { sizeof(Routine),            HT_TRANSIENT_SMALL   ,0 }, // CN_ROUTINE
  { sizeof(Row),                HT_TRANSIENT_LARGE   ,0 }, // CN_ROW
  { sizeof(Row),/*correct*/     HT_TRANSIENT_MEDIUM  ,0 }, // CN_ROWAGG
  { 0,                          HT_INVALID           ,0 }, // CN_ROWBOX
  { sizeof(RowBuffer),          HT_TRANSIENT_LARGE   ,0 }, // CN_ROWBUFFER
  { sizeof(RowCompactor),       HT_TRANSIENT_SMALL   ,0 }, // CN_ROWCOMPACTOR
  { sizeof(RowOnce),            HT_TRANSIENT_TINY    ,0 }, // CN_ROWONCE
  { sizeof(RowSingle),          HT_TRANSIENT_MEDIUM  ,0 }, // CN_ROWSINGLE
  { sizeof(RowTray),            HT_TRANSIENT_LARGE   ,0 }, // CN_ROWTRAY
  { sizeof(RU_Cpu),             HT_INVALID           ,0 }, // CN_RUCPU
  { 0,/*sizeof(RU_Dir)*/        HT_INVALID           ,0 }, // CN_RUDIR
  { 0,/*sizeof(RU_File)*/       HT_INVALID           ,0 }, // CN_RUFILE
  { sizeof(RU_Memory),          HT_INVALID           ,0 }, // CN_RUMEMORY
  { 0,/*sizeof(RU_Mutex)*/      HT_INVALID           ,0 }, // CN_RUMUTEX
  { 0,/*sizeof(RU_Pass)*/       HT_INVALID           ,0 }, // CN_RUPASS
  { 0,/*sizeof(RU_Socket)*/     HT_INVALID           ,0 }, // CN_RUSOCKET
  { 0,/*sizeof(RU_Time)*/       HT_INVALID           ,0 }, // CN_RUTIME
  { 0,/*sizeof(RU_Word)*/       HT_INVALID           ,0 }, // CN_RUWORK

  { 'S' - ('A' - 1),            HT_INVALID           ,0 }, // CN__S__
  { sizeof(Schema),             HT_TRANSIENT_TINY    ,0 }, // CN_SCHEMA
  { sizeof(SchemaTest),         HT_TRANSIENT_LARGE   ,0 }, // CN_SCHEMATEST
  { sizeof(SeDeputy),           HT_TRANSIENT_TINY    ,0 }, // CN_SEDEPUTY
  { sizeof(SeeMe),              HT_INVALID           ,0 }, // CN_SEEME
  { sizeof(SePrompt),           HT_TRANSIENT_LARGE   ,0 }, // CN_SEPROMPT
  { sizeof(Session),            HT_TRANSIENT_MEDIUM  ,0 }, // CN_SESSION
  { sizeof(SessionArray),       HT_TRANSIENT_TINY    ,0 }, // CN_SESSIONARRAY
  { sizeof(ShortCut),           HT_TRANSIENT_MEDIUM  ,0 }, // CN_SHORTCUT
  { sizeof(ShortCutVec),        HT_TRANSIENT_MEDIUM  ,0 }, // CN_SHORTCUTVEC
  { sizeof(Birth),              HT_INVALID           ,0 }, // CN_SINCE
  { sizeof(SkipListNL),         HT_TRANSIENT_TINY    ,0 }, // CN_SKIPLIST
  { sizeof(SkipNode),           HT_TRANSIENT_TINY    ,0 }, // CN_SKIPNODE
  { sizeof(Slab),               HT_TRANSIENT_LARGE   ,0 }, // CN_SLAB
  { sizeof(SlabAllocator),      HT_TRANSIENT_TINY    ,0 }, // CN_SLABALLOCATOR
  { sizeof(Sleep),              HT_INVALID           ,0 }, // CN_SLEEP
  { sizeof(Heap),/*correct*/    HT_INVALID           ,0 }, // SN_SMALLPERSISTENT
  { sizeof(Heap),/*correct*/    HT_INVALID           ,0 }, // SN_SMALLTRANSIENT
  { sizeof(SN_Bcd),             HT_INVALID           ,0 }, // CN_SNBCD
  { sizeof(SN_Bol),             HT_INVALID           ,0 }, // CN_SNBOL
  { sizeof(SN_Dte),             HT_INVALID           ,0 }, // CN_SNDTE
  { sizeof(SN_Flt),             HT_INVALID           ,0 }, // CN_SNFLT
  { sizeof(SN_Int),             HT_INVALID           ,0 }, // CN_SNINT
  { sizeof(SN_Ivl),             HT_INVALID           ,0 }, // CN_SNIVL
  { sizeof(SN_Tme),             HT_INVALID           ,0 }, // CN_SNTME
  { sizeof(SN_Tsp),             HT_INVALID           ,0 }, // CN_SNTSP
  { sizeof(SN_Vch),             HT_INVALID           ,0 }, // CN_SNVCH
  { sizeof(Socket),             HT_TRANSIENT_TINY    ,0 }, // CN_SOCKET
  { sizeof(SP_Elapsed),         HT_INVALID           ,0 }, // CN_SPELAPSED
  { sizeof(SP_Gid),             HT_INVALID           ,0 }, // CN_SPID
  { sizeof(SP_String),          HT_INVALID           ,0 }, // CN_SPSTRING
  { sizeof(SPmedium),           HT_INVALID           ,0 }, // CN_SPMEDIUM
  { sizeof(SPsmall),            HT_INVALID           ,0 }, // CN_SPSMALL
  { sizeof(SqlPlanner),         HT_TRANSIENT_TINY    ,0 }, // CN_SQLPLANNER
  { sizeof(SrtBuffer),          HT_TRANSIENT_LARGE   ,0 }, // CN_SRTBUFFER
  { sizeof(SrtSet),             HT_INVALID           ,0 }, // CN_SRTSET
  { sizeof(Stack),              HT_TRANSIENT_MEDIUM  ,0 }, // CN_STACK
  { sizeof(Stage),              HT_TRANSIENT_SMALL   ,0 }, // CN_STAGE
  { sizeof(StageSpec),          HT_TRANSIENT_SMALL   ,0 }, // CN_STAGESPEC
  { sizeof(Statistics),         HT_INVALID           ,0 }, // CN_STEPACCUM
  { sizeof(StepAccum),          HT_INVALID           ,0 }, // CN_STATISTICS
  { sizeof(StepTiming),         HT_TRANSIENT_LARGE   ,0 }, // CN_STEPTIMING
  { sizeof(Strand),             HT_TRANSIENT_SMALL   ,0 }, // CN_STRAND
  { sizeof(StreamDef),          HT_TRANSIENT_LARGE   ,0 }, // CN_STREAMDEF
  { sizeof(StreamSet),          HT_TRANSIENT_LARGE   ,0 }, // CN_STREAMSET
  { sizeof(s32),                HT_INVALID           ,0 }, // CN_STRING_T
  { sizeof(StzVal),             HT_INVALID           ,0 }, // CN_STZVAL
  { sizeof(SysCall),            HT_INVALID           ,0 }, // CN_SYSCALL

  { 'T' - ('A' - 1),            HT_INVALID           ,0 }, // CN__T__
  { sizeof(Table),              HT_TRANSIENT_TINY    ,0 }, // CN_TABLE
  { sizeof(Table),              HT_TRANSIENT_MEDIUM  ,0 }, // CN_TABLEDEF
  { sizeof(TablePtn),           HT_TRANSIENT_TINY    ,0 }, // CN_TABLEPTN
  { sizeof(TablePtnVec),        HT_TRANSIENT_SMALL   ,0 }, // CN_TABLEPTNVEC
  { sizeof(TableRef),           HT_TRANSIENT_SMALL   ,0 }, // CN_TABLEREF
  { sizeof(TagSet),             HT_TRANSIENT_MEDIUM  ,0 }, // CN_TAGSET
  { sizeof(Task),               HT_TRANSIENT_TINY    ,0 }, // CN_TASK
  { sizeof(TaskList),           HT_TRANSIENT_TINY    ,0 }, // CN_TASKLIST
  { sizeof(TaskQueue),          HT_TRANSIENT_TINY    ,0 }, // CN_TEMPNAME
  { sizeof(TempName),           HT_TRANSIENT_TINY    ,0 }, // CN_TENANT
  { sizeof(Tenant),             HT_TRANSIENT_TINY    ,0 }, // CN_TENANTS
  { sizeof(Tenants),            HT_TRANSIENT_TINY    ,0 }, // CN_TASKQUEUE
  { sizeof(Text),               HT_TRANSIENT_TINY    ,0 }, // CN_TEXT
  { sizeof(TextLarge),          HT_TRANSIENT_LARGE   ,0 }, // CN_TEXTLARGE
  { sizeof(TextMedium),         HT_TRANSIENT_MEDIUM  ,0 }, // CN_TEXTMEDIUM
  { sizeof(TextSmall),          HT_TRANSIENT_SMALL   ,0 }, // CN_TEXTSMALL
  { sizeof(TextTiny),           HT_TRANSIENT_TINY    ,0 }, // CN_TEXTTINY
  { sizeof(Thing),              HT_TRANSIENT_TINY    ,0 }, // CN_THING
  { sizeof(Thread),             HT_TRANSIENT_TINY    ,0 }, // CN_THREAD
  { sizeof(ThreadQueue),        HT_TRANSIENT_TINY    ,0 }, // CN_THREADQUEUE
  { sizeof(ThreadVec),          HT_TRANSIENT_MEDIUM  ,0 }, // CN_THREADVEC
  { sizeof(Timer),              HT_TRANSIENT_TINY    ,0 }, // CN_TIMER
  { sizeof(Heap),/*correct*/    HT_INVALID           ,0 }, // CN_TINYPERSISTENT
  { sizeof(Heap),/*correct*/    HT_INVALID           ,0 }, // CN_TINYTRANSIENT
  { sizeof(TmeVal),             HT_INVALID           ,0 }, // CN_TMEVAL
  { sizeof(ToHshCrw),           HT_TRANSIENT_LARGE   ,0 }, // CN_TOHSHCRW
  { sizeof(Token),              HT_TRANSIENT_TINY    ,0 }, // CN_TOKEN
  { sizeof(TokenVec),           HT_TRANSIENT_LARGE   ,0 }, // CN_TOKENVEC
  { sizeof(ToNstRow),           HT_TRANSIENT_MEDIUM  ,0 }, // CN_TONSTROW
  { sizeof(ToPtnRow),           HT_TRANSIENT_LARGE   ,0 }, // CN_TOPTNROW
  { sizeof(ToRow),              HT_TRANSIENT_MEDIUM  ,0 }, // CN_TOROW
  { sizeof(TspVal),             HT_INVALID           ,0 }, // CN_TSPVAL
  { sizeof(Tx),                 HT_TRANSIENT_TINY    ,0 }, // CN_TX
  { sizeof(TxSet),              HT_TRANSIENT_TINY    ,0 }, // CN_TXSET

  { 'U' - ('A' - 1),            HT_INVALID           ,0 }, // CN__U__

  { 'V' - ('A' - 1),            HT_INVALID           ,0 }, // CN__V__

  { 'W' - ('A' - 1),            HT_INVALID           ,0 }, // CN__W__
  { sizeof(Wilderness),         HT_INVALID           ,0 }, // CN_WILDERNESS
  { sizeof(Word),               HT_TRANSIENT_TINY    ,0 }, // CN_WORD
  { 0,                          HT_INVALID           ,0 }, // CN_WSPCONVERGENT_t

  { 'X' - ('A' - 1),            HT_INVALID           ,0 }, // CN__X__
  { sizeof(XmitOperand),        HT_TRANSIENT_TINY    ,0 }, // CN_XMITIOPERAND
  { sizeof(Xmits),              HT_TRANSIENT_TINY    ,0 }, // CN_XMITS

  { 'Y' - ('A' - 1),            HT_INVALID           ,0 }, // CN__Y__

  { 'Z' - ('A' - 1),            HT_INVALID           ,0 }, // CN__Z__

  { C_VARGS_END,                HT_INVALID           ,0 }, // CN_ENUMS
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
