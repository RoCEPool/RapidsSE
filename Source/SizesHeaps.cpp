/*============================================================================*/
/* SizesHeaps.cpp -- Class Sizes and Heaps                                    */
/*============================================================================*/

#include "Oblique.h"

/*----------------------------------------------------------------------------*/
/* sizes & heaps                                                              */
/*----------------------------------------------------------------------------*/
SizeHeap gxaSizeHeap[]
{
  { 0,                          HT_INVALID           }, // CN_NONE

  { 'A' - ('A' - 1),            HT_INVALID           }, // CN__A__
  { sizeof(AccessPath),         HT_TRANSIENT_MEDIUM  }, // CN_ACCESSPATH
  { sizeof(AggTimer),           HT_TRANSIENT_TINY    }, // CN_AGGTIMER
  { sizeof(Arg),                HT_TRANSIENT_TINY    }, // CN_ARG
  { sizeof(Args),               HT_INVALID           }, // CN_ARGUMENTS
  { sizeof(Arrange),            HT_TRANSIENT_TINY    }, // CN_ARRANGE
  { sizeof(Article),            HT_TRANSIENT_TINY    }, // CN_ARTICLE
  { sizeof(Atomic),             HT_INVALID           }, // CN_ATOMIC
  { sizeof(AttachMap),          HT_INVALID           }, // CN_ATTACHMAP

  { 'B' - ('A' - 1),            HT_INVALID           }, // CN__B__
  { sizeof(BcdVal),             HT_INVALID           }, // CN_BCDVAL
  { sizeof(Birth),              HT_INVALID           }, // CN_BIRTH
  { sizeof(BolVal),             HT_INVALID           }, // CN_BOLVAL
  { sizeof(BPA),                HT_TRANSIENT_MEDIUM  }, // CN_BPA
  { sizeof(BucketMap),          HT_TRANSIENT_LARGE   }, // CN_BUCKETMAP
  { sizeof(BufferCursor),       HT_TRANSIENT_LARGE   }, // CN_BUFFERCURSOR
  { sizeof(BufferPool),         HT_INVALID           }, // CN_BUFFERPOOL
  { sizeof(ByteRate),           HT_TRANSIENT_TINY    }, // CN_BYTERATE
  { sizeof(s32),                HT_INVALID           }, // CN_BYTES_T

  { 'C' - ('A' - 1),            HT_INVALID           }, // CN__C__
  { sizeof(CarryOver),          HT_INVALID           }, // CN_CARRYOVER
  { sizeof(Catalog),            HT_INVALID           }, // CN_CATALOG
  { sizeof(Charser),            HT_TRANSIENT_MEDIUM  }, // CN_CHARSER
  { sizeof(Clause_From),        HT_TRANSIENT_MEDIUM  }, // CN_CLAUSE_FROM
  { sizeof(Clause_GroupBy),     HT_TRANSIENT_MEDIUM  }, // CN_CLAUSE_GROUPBY
  { sizeof(Clause_Join),        HT_TRANSIENT_MEDIUM  }, // CN_CLAUSE_JOIN
  { sizeof(Clause_Limit),       HT_TRANSIENT_MEDIUM  }, // CN_CLAUSE_LIMIT
  { sizeof(Clause_OrderBy),     HT_TRANSIENT_MEDIUM  }, // CN_CLAUSE_ORDERBY
  { sizeof(Clause_Project),     HT_TRANSIENT_MEDIUM  }, // CN_CLAUSE_PROJECT
  { sizeof(Clause_Where),       HT_TRANSIENT_MEDIUM  }, // CN_CLAUSE_WHERE
  { sizeof(Column),             HT_TRANSIENT_TINY    }, // CN_COLUMN
  { sizeof(Compiler),           HT_TRANSIENT_TINY    }, // CN_COMPILER
  { sizeof(ConstDef),           HT_TRANSIENT_MEDIUM  }, // CN_CONSTDEF
  { sizeof(ConstSet),           HT_TRANSIENT_MEDIUM  }, // CN_CONSTSET
  { sizeof(CrvBuffer),          HT_TRANSIENT_LARGE   }, // CN_CRVBUFFER
  { sizeof(CrwBuffer),          HT_TRANSIENT_LARGE   }, // CN_CRWBUFFER
  { sizeof(CrwHash),            HT_TRANSIENT_LARGE   }, // CN_CRWHASH
  { sizeof(CsvBuffer),          HT_TRANSIENT_LARGE   }, // CN_CSVBUFFER

  { 'D' - ('A' - 1),            HT_INVALID           }, // CN__D__
  { sizeof(Datum_h),            HT_TRANSIENT_MEDIUM  }, // CN_DATUM
  { sizeof(DecaCnt),            HT_TRANSIENT_TINY    }, // CN_DECACNT
  { sizeof(Decider),            HT_TRANSIENT_TINY    }, // CN_DECIDER
  { sizeof(Descend),            HT_TRANSIENT_MEDIUM  }, // CN_DESCEND
  { sizeof(DevIXT),             HT_TRANSIENT_TINY    }, // CN_DEVIXT
  { sizeof(Dic),                HT_TRANSIENT_TINY    }, // CN_DIC
  { sizeof(DicStack),           HT_TRANSIENT_TINY    }, // CN_DICSTACK
  { sizeof(Dir),                HT_TRANSIENT_TINY    }, // CN_DIR
  { sizeof(DteVal),             HT_INVALID           }, // CN_DTEVAL
  { sizeof(Dynamo),             HT_TRANSIENT_TINY    }, // CN_DYNAMO

  { 'E' - ('A' - 1),            HT_INVALID           }, // CN__E__
  { sizeof(EdtVal),             HT_INVALID           }, // CN_EDTVAL
  { sizeof(Encoding),           HT_TRANSIENT_LARGE   }, // CN_ENCODING
  { sizeof(Ent),                HT_INVALID           }, // CN_ENT
  { sizeof(EntBuffer),          HT_TRANSIENT_LARGE   }, // CN_ENTBUFFER
  { sizeof(EntBufSorter),       HT_TRANSIENT_LARGE   }, // CN_ENTBUFSORTER
  { sizeof(EntWrk),             HT_INVALID           }, // CN_ENTWRK
  { sizeof(EnumFlags),          HT_TRANSIENT_TINY    }, // CN_ENUMFLAGS
  { sizeof(Exemplar),           HT_TRANSIENT_TINY    }, // CN_EXEMPLAR

  { 'F' - ('A' - 1),            HT_INVALID           }, // CN__F__
  { sizeof(FetchBuffer),        HT_TRANSIENT_LARGE   }, // CN_FETCHBUFFER
  { sizeof(Fifo),               HT_TRANSIENT_TINY    }, // CN_FIFO
  { sizeof(File),               HT_TRANSIENT_TINY    }, // CN_FILE
  { sizeof(Filter_Fetch),       HT_TRANSIENT_MEDIUM  }, // CN_FILTER_FETCH
  { sizeof(Filter_From),        HT_TRANSIENT_MEDIUM  }, // CN_FILTER_FROM
  { sizeof(Filter_GroupBy),     HT_TRANSIENT_MEDIUM  }, // CN_FILTER_GROUPBY
  { sizeof(Filter_Inflate),     HT_TRANSIENT_MEDIUM  }, // CN_FILTER_INFLATE
  { sizeof(Filter_Join),        HT_TRANSIENT_MEDIUM  }, // CN_FILTER_JOIN
  { sizeof(Filter_Limit),       HT_TRANSIENT_MEDIUM  }, // CN_FILTER_LIMIT
  { sizeof(Filter_OrderBy),     HT_TRANSIENT_MEDIUM  }, // CN_FILTER_ORDERBY
  { sizeof(Filter_Output),      HT_TRANSIENT_MEDIUM  }, // CN_FILTER_OUTPUT
  { sizeof(Filter_Project),     HT_TRANSIENT_MEDIUM  }, // CN_FILTER_PROJECT
  { sizeof(Filter_Scan),        HT_TRANSIENT_MEDIUM  }, // CN_FILTER_SCAN
  { sizeof(Filter_Select),      HT_TRANSIENT_MEDIUM  }, // CN_FILTER_SELECT
  { sizeof(Filter_SubQry),      HT_TRANSIENT_MEDIUM  }, // CN_FILTER_SUBQRY
  { sizeof(Filter_Where),       HT_TRANSIENT_MEDIUM  }, // CN_FILTER_WHERE
  { sizeof(Filter_Zed),         HT_TRANSIENT_MEDIUM  }, // CN_FILTER_ZED
  { sizeof(FipVal),             HT_INVALID           }, // CN_FIPVAL
  { sizeof(FltVal),             HT_INVALID           }, // CN_FLTVAL
  { sizeof(FocalObjects),       HT_TRANSIENT_LARGE   }, // CN_FOCALOBJECTS
  { sizeof(Forth),              HT_TRANSIENT_TINY    }, // CN_FORTH
  { sizeof(Format),             HT_TRANSIENT_TINY    }, // CN_FORMAT
  { sizeof(PoolBuffer),/*yes*/  HT_INVALID           }, // CN_FREEBUFFER
  { sizeof(FreeChunk),          HT_INVALID           }, // CN_FREECHUNK
  { 0,                          HT_INVALID           }, // CN_FREEDNOT
  { 0,                          HT_INVALID           }, // CN_FREEITEM
  { sizeof(FreqDist),           HT_TRANSIENT_SMALL   }, // CN_FREQDIST

  { 'G' - ('A' - 1),            HT_INVALID           }, // CN__G__
  { sizeof(GGadaptor),          HT_TRANSIENT_TINY    }, // CN_GGADAPTOR
  { sizeof(Grid),               HT_TRANSIENT_SMALL   }, // CN_GRID

  { 'H' - ('A' - 1),            HT_INVALID           }, // CN__H__
  { sizeof(HashBuffer),         HT_TRANSIENT_LARGE   }, // CN_HASHBUFFER
  { sizeof(HashSet),            HT_TRANSIENT_LARGE   }, // CN_HASHSET
  { sizeof(HashMap),            HT_TRANSIENT_TINY    }, // CN_HASHMAP
  { sizeof(HeapBuffer),         HT_INVALID           }, // CN_HEAPBUFFER
  { sizeof(SP_Row),             HT_INVALID           }, // CN_HPROW

  { 'I' - ('A' - 1),            HT_INVALID           }, // CN__I__
  { sizeof(Index),              HT_TRANSIENT_TINY    }, // CN_INDEX
  { sizeof(IndexDef),           HT_TRANSIENT_MEDIUM  }, // CN_INDEXDEF
  { sizeof(IndexStream),        HT_TRANSIENT_TINY    }, // CN_INDEXSTREAM
  { sizeof(IndexTree),          HT_TRANSIENT_TINY    }, // CN_INDEXTREE
  { sizeof(IndexTreeVec),       HT_TRANSIENT_MEDIUM  }, // CN_INDEXTREEVEC
  { sizeof(IdxVec),             HT_TRANSIENT_MEDIUM  }, // CN_IDXVEC
  { sizeof(InfoSet),            HT_TRANSIENT_LARGE   }, // CN_INFOSET
  { sizeof(IngestChunk),        HT_TRANSIENT_LARGE   }, // CN_INGESTCHUNK
  { sizeof(Input),              HT_TRANSIENT_TINY    }, // CN_INPUT
  { sizeof(IntBuffer),          HT_TRANSIENT_LARGE   }, // CN_INTBUFFER
  { sizeof(IntSet),             HT_TRANSIENT_TINY    }, // CN_INTSET
  { sizeof(IntVal),             HT_INVALID           }, // CN_INTVAL
  { sizeof(IoChunk),            HT_TRANSIENT_LARGE   }, // CN_IOCHUNK
  { sizeof(IoCopy),             HT_TRANSIENT_TINY    }, // CN_IOCOPY
  { sizeof(IrwBuffer),          HT_TRANSIENT_LARGE   }, // CN_IRWBUFFER
  { sizeof(IvlVal),             HT_INVALID           }, // CN_IVLVAL

  { 'J' - ('A' - 1),            HT_INVALID           }, // CN__J__
  { sizeof(Job),                HT_TRANSIENT_MEDIUM  }, // CN_JOB
  { sizeof(Job__Find),          HT_TRANSIENT_LARGE   }, // CN_JOB__FIND
  { sizeof(JobList),            HT_TRANSIENT_TINY    }, // CN_JOBLIST

  { 'K' - ('A' - 1),            HT_INVALID           }, // CN__K__
  { sizeof(Key),                HT_INVALID           }, // CN_KEY

  { 'L' - ('A' - 1),            HT_INVALID           }, // CN__L__
  { sizeof(Heap), /*correct*/   HT_INVALID           }, // CN_LARGEPERSISTENT
  { sizeof(Heap), /*correct*/   HT_INVALID           }, // CN_LARGETRANSIENT
  { sizeof(Lexicon),            HT_TRANSIENT_TINY    }, // CN_LEXICON
  { sizeof(Licence),            HT_TRANSIENT_TINY    }, // CN_LICENCE
  { sizeof(Limits),             HT_TRANSIENT_TINY    }, // CN_LIMITS
  { sizeof(LoadCursor),         HT_TRANSIENT_LARGE   }, // CN_LOADCURSOR
  { sizeof(LoadChunk),          HT_TRANSIENT_LARGE   }, // CN_LOADCHUNK
  { sizeof(Loader),             HT_TRANSIENT_LARGE   }, // CN_LOADER
  { sizeof(Local),              HT_INVALID           }, // CN_LOCAL
  { sizeof(LocalChain),         HT_TRANSIENT_LARGE   }, // CN_LOCALSCHAIN
  { sizeof(Lock),               HT_INVALID           }, // CN_LOCK
  { sizeof(Log),                HT_INVALID           }, // CN_LOG
  { sizeof(LogEntry),           HT_INVALID           }, // CN_LOGENTRY
  { sizeof(LogicCompiler),      HT_TRANSIENT_LARGE   }, // CN_LOGICCOMPILER
  { sizeof(LogicUnit),          HT_TRANSIENT_MEDIUM  }, // CN_LOGICUNIT
  { sizeof(Lrw),                HT_TRANSIENT_SMALL   }, // CN_LRW
  { sizeof(LrwBuffer),          HT_TRANSIENT_LARGE   }, // CN_LRWBUFFER

  { 'M' - ('A' - 1),            HT_INVALID           }, // CN__M__
  { sizeof(Marker),             HT_TRANSIENT_TINY    }, // CN_MARKER
  { sizeof(Heap), /*correct*/   HT_INVALID           }, // CN_MEDIUMPERSISTENT
  { sizeof(Heap), /*correct*/   HT_INVALID           }, // CN_MEDIUMTRANSIENT
  { sizeof(MemTest),            HT_TRANSIENT_TINY    }, // CN_MEMTEST
  { sizeof(MetaBlock),          HT_TRANSIENT_MEDIUM  }, // CN_METABLOCK
  { sizeof(MetaCell),           HT_TRANSIENT_TINY    }, // CN_METACELL
  { sizeof(MetaGraph),          HT_TRANSIENT_MEDIUM  }, // CN_METAGRAPH
  { sizeof(MetaInherit),        HT_TRANSIENT_MEDIUM  }, // CN_METAINHERIT
  { sizeof(MetaInherits),       HT_TRANSIENT_TINY    }, // CN_METAINHERITS
  { sizeof(MetaMember),         HT_TRANSIENT_TINY    }, // CN_METAMEMBER
  { sizeof(MetaType),           HT_TRANSIENT_MEDIUM  }, // CN_METATYPE
  { sizeof(MidEnd_FromVec),     HT_TRANSIENT_SMALL   }, // CN_MIDEND_FROMVEC
  { sizeof(Mutex),              HT_TRANSIENT_TINY    }, // CN_MUTEX

  { 'N' - ('A' - 1),            HT_INVALID           }, // CN__N__
  { sizeof(NextGid),            HT_INVALID           }, // CN_NEXTGID
  { sizeof(NmlStr),             HT_TRANSIENT_MEDIUM  }, // CN_NMLSTR
  { sizeof(Node),               HT_TRANSIENT_TINY    }, // CN_NODE
  { sizeof(Now),                HT_TRANSIENT_TINY    }, // CN_NOW
  { sizeof(Numbat),             HT_TRANSIENT_LARGE   }, // CN_NUMBAT

  { 'O' - ('A' - 1),            HT_INVALID           }, // CN__O__
  { sizeof(ObjectsInPool),      HT_TRANSIENT_SMALL   }, // CN_OBJECTSINPOOL
  { sizeof(Op),                 HT_TRANSIENT_LARGE   }, // CN_OP
  { sizeof(OperandQueue),       HT_TRANSIENT_TINY    }, // CN_OPERANDQUEUE
  { sizeof(OpSpec),             HT_TRANSIENT_LARGE   }, // CN_OPSPEC

  { 'P' - ('A' - 1),            HT_INVALID           }, // CN__P__
  { sizeof(PathVec),            HT_TRANSIENT_MEDIUM  }, // CN_PATHVEC
  { sizeof(Peruse),             HT_TRANSIENT_MEDIUM  }, // CN_PERUSE
  { sizeof(Pipeline),           HT_TRANSIENT_SMALL   }, // CN_PIPELINE
  { sizeof(PipelineSpec),       HT_TRANSIENT_SMALL   }, // CN_PIPELINESPEC
  { sizeof(Plan),               HT_TRANSIENT_MEDIUM  }, // CN_PLAN
  { sizeof(Planner),            HT_TRANSIENT_MEDIUM  }, // CN_PLANNER
  { sizeof(Pot),                HT_TRANSIENT_LARGE   }, // CN_POT
  { sizeof(Predicate),          HT_TRANSIENT_TINY    }, // CN_PREDICATE
  { sizeof(PredicateVec),       HT_TRANSIENT_MEDIUM  }, // CN_PREDICATEVEC
  { sizeof(Profile),            HT_TRANSIENT_MEDIUM  }, // CN_PROFILE
  { sizeof(PsxThd),             HT_TRANSIENT_TINY    }, // CN_PSXTHD
  { sizeof(PtnColVec),          HT_TRANSIENT_MEDIUM  }, // CN_PTNCOLVEC
  { sizeof(PtnMap),             HT_TRANSIENT_LARGE   }, // CN_PTNMAP
  { sizeof(PtnStream),          HT_TRANSIENT_TINY    }, // CN_PTNSTREAM
  { sizeof(PtrVal),             HT_INVALID           }, // CN_PTRVAL

  { 'Q' - ('A' - 1),            HT_INVALID           }, // CN__Q__
  { sizeof(Queue),              HT_TRANSIENT_TINY    }, // CN_QUEUE
  { sizeof(QueueBuffer),        HT_INVALID           }, // CN_QUEUEBUFFER
  { sizeof(Queues),             HT_TRANSIENT_TINY    }, // CN_QUEUES

  { 'R' - ('A' - 1),            HT_INVALID           }, // CN__R__
  { sizeof(ReadApi),            HT_TRANSIENT_TINY    }, // CN_READAPI
  { sizeof(Record),             HT_TRANSIENT_MEDIUM  }, // CN_RECORD
  { sizeof(RecordBuffer),       HT_TRANSIENT_LARGE   }, // CN_RECORDBUFFER
  { sizeof(RelOp),              HT_TRANSIENT_TINY    }, // CN_RELOP
  { sizeof(ResourceTracker),    HT_TRANSIENT_TINY    }, // CN_RESOURCETRACKER
  { sizeof(Response),           HT_TRANSIENT_MEDIUM  }, // CN_RESPONSE
  { sizeof(ResponseVec),        HT_TRANSIENT_SMALL   }, // CN_RESPONSEVEC
  { sizeof(Result),             HT_TRANSIENT_MEDIUM  }, // CN_RESULT
  { sizeof(Rand),               HT_INVALID           }, // CN_RN_RAND
  { sizeof(Routine),            HT_TRANSIENT_SMALL   }, // CN_ROUTINE
  { sizeof(Row),                HT_TRANSIENT_LARGE   }, // CN_ROW
  { sizeof(Row),/*correct*/     HT_TRANSIENT_MEDIUM  }, // CN_ROWAGG
  { sizeof(RowCompactor),       HT_TRANSIENT_LARGE   }, // CN_ROWCOMPACTOR
  { sizeof(RowOnce),            HT_TRANSIENT_TINY    }, // CN_ROWONCE
  { sizeof(RowSingle),          HT_TRANSIENT_MEDIUM  }, // CN_ROWSINGLE
  { sizeof(RowSlab),            HT_TRANSIENT_LARGE   }, // CN_ROWSLAB
  { sizeof(RowTray),            HT_TRANSIENT_LARGE   }, // CN_ROWTRAY
  { sizeof(RsbBuffer),          HT_TRANSIENT_LARGE   }, // CN_RSBBUFFER
  { sizeof(RU_Cpu),             HT_INVALID           }, // CN_RUCPU
  { 0,/*sizeof(RU_Dir)*/        HT_INVALID           }, // CN_RUDIR
  { 0,/*sizeof(RU_File)*/       HT_INVALID           }, // CN_RUFILE
  { sizeof(RU_Memory),          HT_INVALID           }, // CN_RUMEMORY
  { 0,/*sizeof(RU_Mutex)*/      HT_INVALID           }, // CN_RUMUTEX
  { 0,/*sizeof(RU_Pass)*/       HT_INVALID           }, // CN_RUPASS
  { 0,/*sizeof(RU_Socket)*/     HT_INVALID           }, // CN_RUSOCKET
  { 0,/*sizeof(RU_Time)*/       HT_INVALID           }, // CN_RUTIME
  { 0,/*sizeof(RU_Word)*/       HT_INVALID           }, // CN_RUWORK

  { 'S' - ('A' - 1),            HT_INVALID           }, // CN__S__
  { sizeof(Schema),             HT_TRANSIENT_TINY    }, // CN_SCHEMA
  { sizeof(SchemaTest),         HT_TRANSIENT_LARGE   }, // CN_SCHEMATEST
  { sizeof(SeeMe),              HT_INVALID           }, // CN_SEEME
  { sizeof(SePrompt),           HT_TRANSIENT_LARGE   }, // CN_SEPROMPT
  { sizeof(Session),            HT_TRANSIENT_MEDIUM  }, // CN_SESSION
  { sizeof(SessionArray),       HT_TRANSIENT_TINY    }, // CN_SESSIONARRAY
  { sizeof(ShortCut),           HT_TRANSIENT_MEDIUM  }, // CN_SHORTCUT
  { sizeof(ShortCutVec),        HT_TRANSIENT_MEDIUM  }, // CN_SHORTCUTVEC
  { sizeof(Birth),              HT_INVALID           }, // CN_SINCE
  { sizeof(SkipListNL),         HT_TRANSIENT_TINY    }, // CN_SKIPLIST
  { sizeof(SkipNode),           HT_TRANSIENT_TINY    }, // CN_SKIPNODE
  { sizeof(Slab),               HT_TRANSIENT_LARGE   }, // CN_SLAB
  { sizeof(SlabAllocator),      HT_TRANSIENT_TINY    }, // CN_SLABALLOCATOR
  { sizeof(Sleep),              HT_INVALID           }, // CN_SLEEP
  { sizeof(Heap),/*correct*/    HT_INVALID           }, // SN_SMALLPERSISTENT
  { sizeof(Heap),/*correct*/    HT_INVALID           }, // SN_SMALLTRANSIENT
  { sizeof(SN_Bcd),             HT_INVALID           }, // CN_SNBCD
  { sizeof(SN_Bol),             HT_INVALID           }, // CN_SNBOL
  { sizeof(SN_Dte),             HT_INVALID           }, // CN_SNDTE
  { sizeof(SN_Flt),             HT_INVALID           }, // CN_SNFLT
  { sizeof(SN_Int),             HT_INVALID           }, // CN_SNINT
  { sizeof(SN_Ivl),             HT_INVALID           }, // CN_SNIVL
  { sizeof(SN_Tme),             HT_INVALID           }, // CN_SNTME
  { sizeof(SN_Tsp),             HT_INVALID           }, // CN_SNTSP
  { sizeof(SN_Vch),             HT_INVALID           }, // CN_SNVCH
  { sizeof(Socket),             HT_TRANSIENT_TINY    }, // CN_SOCKET
  { sizeof(SP_Elapsed),         HT_INVALID           }, // CN_SPELAPSED
  { sizeof(SP_Gid),             HT_INVALID           }, // CN_SPID
  { sizeof(SP_String),          HT_INVALID           }, // CN_SPSTRING
  
  { sizeof(SPmedium),           HT_INVALID           }, // CN_SPMEDIUM
  { sizeof(SPsmall),            HT_INVALID           }, // CN_SPSMALL

  { sizeof(SrtBuffer),          HT_TRANSIENT_LARGE   }, // CN_SRTBUFFER
  { sizeof(SrtSet),             HT_INVALID           }, // CN_SRTSET
  { sizeof(Stack),              HT_TRANSIENT_MEDIUM  }, // CN_STACK
  { sizeof(Stage),              HT_TRANSIENT_SMALL   }, // CN_STAGE
  { sizeof(StageSpec),          HT_TRANSIENT_SMALL   }, // CN_STAGESPEC
  { sizeof(Statistics),         HT_INVALID           }, // CN_STEPACCUM
  { sizeof(StepAccum),          HT_INVALID           }, // CN_STATISTICS
  { sizeof(StepTiming),         HT_TRANSIENT_LARGE   }, // CN_STEPTIMING
  { sizeof(Stmt_Select),        HT_TRANSIENT_LARGE   }, // CN_STMT_SELECT
  { sizeof(Stmt_SubQry),        HT_TRANSIENT_MEDIUM  }, // CN_STMT_SUBQRY
  { sizeof(Strand),             HT_TRANSIENT_SMALL   }, // CN_STRAND
  { sizeof(StreamDef),          HT_TRANSIENT_LARGE   }, // CN_STREAMDEF
  { sizeof(StreamSet),          HT_TRANSIENT_LARGE   }, // CN_STREAMSET
  { sizeof(s32),                HT_INVALID           }, // CN_STRING_T
  { sizeof(StzVal),             HT_INVALID           }, // CN_STZVAL
  { sizeof(SysCall),            HT_INVALID           }, // CN_SYSCALL

  { 'T' - ('A' - 1),            HT_INVALID           }, // CN__T__
  { sizeof(Table),              HT_TRANSIENT_TINY    }, // CN_TABLE
  { sizeof(Table),              HT_TRANSIENT_MEDIUM  }, // CN_TABLEDEF
  { sizeof(TablePtn),           HT_TRANSIENT_TINY    }, // CN_TABLEPTN
  { sizeof(TablePtnVec),        HT_TRANSIENT_SMALL   }, // CN_TABLEPTNVEC
  { sizeof(TagSet),             HT_TRANSIENT_MEDIUM  }, // CN_TAGSET
  { sizeof(Task),               HT_TRANSIENT_TINY    }, // CN_TASK
  { sizeof(TaskList),           HT_TRANSIENT_TINY    }, // CN_TASKLIST
  { sizeof(TaskQueue),          HT_TRANSIENT_TINY    }, // CN_TEMPNAME
  { sizeof(TempName),           HT_TRANSIENT_TINY    }, // CN_TENANT
  { sizeof(Tenant),             HT_TRANSIENT_TINY    }, // CN_TENANTS
  { sizeof(Tenants),            HT_TRANSIENT_TINY    }, // CN_TASKQUEUE
  { sizeof(Thing),              HT_TRANSIENT_TINY    }, // CN_THING
  { sizeof(Thread),             HT_TRANSIENT_TINY    }, // CN_THREAD
  { sizeof(ThreadQueue),        HT_TRANSIENT_TINY    }, // CN_THREADQUEUE
  { sizeof(ThreadVec),          HT_TRANSIENT_SMALL   }, // CN_THREADVEC
  { sizeof(Timer),              HT_TRANSIENT_TINY    }, // CN_TIMER
  { sizeof(Heap),/*correct*/    HT_INVALID           }, // CN_TINYPERSISTENT
  { sizeof(Heap),/*correct*/    HT_INVALID           }, // CN_TINYTRANSIENT
  { sizeof(TmeVal),             HT_INVALID           }, // CN_TMEVAL
  { sizeof(ToHshCrw),           HT_TRANSIENT_LARGE   }, // CN_TOHSHCRW
  { sizeof(Token),              HT_TRANSIENT_TINY    }, // CN_TOKEN
  { sizeof(TokenVec),           HT_TRANSIENT_LARGE   }, // CN_TOKENVEC
  { sizeof(ToNstRsb),           HT_TRANSIENT_MEDIUM  }, // CN_TONSTRSB
  { sizeof(ToPtnRsb),           HT_TRANSIENT_LARGE   }, // CN_TOPTNRSB
  { sizeof(ToRsb),              HT_TRANSIENT_MEDIUM  }, // CN_TORSB
  { sizeof(TspVal),             HT_INVALID           }, // CN_TSPVAL
  { sizeof(Tx),                 HT_TRANSIENT_TINY    }, // CN_TX
  { sizeof(TxSet),              HT_TRANSIENT_TINY    }, // CN_TXSET

  { 'U' - ('A' - 1),            HT_INVALID           }, // CN__U__

  { 'V' - ('A' - 1),            HT_INVALID           }, // CN__V__
  { sizeof(Vortex),             HT_TRANSIENT_LARGE   }, // CN_VORTEX

  { 'W' - ('A' - 1),            HT_INVALID           }, // CN__W__
  { sizeof(Wilderness),         HT_INVALID           }, // CN_WILDERNESS
  { sizeof(Word),               HT_TRANSIENT_TINY    }, // CN_WORD

  { 'X' - ('A' - 1),            HT_INVALID           }, // CN__X__
  { sizeof(XmitOperand),        HT_TRANSIENT_TINY    }, // CN_XMITIOPERAND
  { sizeof(Xmits),              HT_TRANSIENT_TINY    }, // CN_XMITS

  { 'Y' - ('A' - 1),            HT_INVALID           }, // CN__Y__

  { 'Z' - ('A' - 1),            HT_INVALID           }, // CN__Z__

  { -1,                         HT_INVALID           }, // CN_ENUMS
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
