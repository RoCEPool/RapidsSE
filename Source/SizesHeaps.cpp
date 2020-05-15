/*============================================================================*/
/* SizesHeaps.cpp -- Class Sizes and Heaps                                    */
/*============================================================================*/

#include "Oblique.h"

/*----------------------------------------------------------------------------*/
/* class sizes                                                                */
/*----------------------------------------------------------------------------*/
i8 gnClassSize[]
{
  /* */ 0,                          // CN_NONE
  
  /* */ 'A' - ('A' - 1),            // CN__A__
  /* */ sizeof(AccessPath),         // CN_ACCESSPATH
  /* */ sizeof(AggTimer),           // CN_AGGTIMER
  /* */ sizeof(Arg),                // CN_ARG
  /* */ sizeof(Args),               // CN_ARGS
  /* */ sizeof(Article),            // CN_ARTICLE
  /* */ sizeof(Atomic),             // CN_ATOMIC
  /* */ sizeof(AttachMap),          // CN_ATTACHMAP

  /* */ 'B' - ('A' - 1),            // CN__B__
  /* */ sizeof(BcdVal),             // CN_BCDVAL
  /* */ sizeof(Birth),              // CN_BIRTH
  /* */ sizeof(BolVal),             // CN_BOLVAL
  /* */ sizeof(BPA),                // CN_BPA
  /* */ sizeof(BucketMap),          // CN_BUCKETMAP
  /* */ sizeof(BufferCursor),       // CN_BUFFERCURSOR
  /* */ sizeof(BufferPool),         // CN_BUFFERPOOL
  /* */ sizeof(ByteRate),           // CN_BYTERATE
  /* */ sizeof(s32),                // CN_BYTES_T

  /* */ 'C' - ('A' - 1),            // CN__C__
  /* */ sizeof(Calculator),         // CN_CALCULATOR
  /* */ sizeof(CarryOver),          // CN_CARRYOVER
  /* */ sizeof(Catalog),            // CN_CATALOG
  /* */ sizeof(Charser),             // CN_CHARSER
  /* */ sizeof(Clause_From),        // CN_CLAUSE_FROM
  /* */ sizeof(Clause_GroupBy),     // CN_CLAUSE_GROUPBY
  /* */ sizeof(Clause_Join),        // CN_CLAUSE_JOIN
  /* */ sizeof(Clause_Limit),       // CN_CLAUSE_LIMIT
  /* */ sizeof(Clause_OrderBy),     // CN_CLAUSE_ORDERBY
  /* */ sizeof(Clause_Select),      // CN_CLAUSE_SELECT
  /* */ sizeof(Clause_SubQry),      // CN_CLAUSE_SUBQRY
  /* */ sizeof(Clause_Where),       // CN_CLAUSE_WHERE
  /* */ sizeof(Cmd_Select),         // CN_CMD_SELECT
  /* */ sizeof(Column),             // CN_COLUMN
  /* */ sizeof(Request),            // CN_COMMAND
  /* */ sizeof(Compiler),           // CN_COMPILER
  /* */ sizeof(ConstDef),           // CN_CONSTDEF
  /* */ sizeof(ConstSet),           // CN_CONSTSET
  /* */ sizeof(CrvBuffer),          // CN_CRVBUFFER
  /* */ sizeof(CrwBuffer),          // CN_CRWBUFFER
  /* */ sizeof(CsvBuffer),          // CN_CSVBUFFER

  /* */ 'D' - ('A' - 1),            // CN__D__
  /* */ sizeof(Datum_h),            // CN_DATUM
  /* */ sizeof(DecaCnt),            // CN_DECACNT
  /* */ sizeof(Decider),            // CN_DECIDER
  /* */ sizeof(Dic),                // CN_DIC
  /* */ sizeof(DicStack),           // CN_DICSTACK
  /* */ sizeof(Dir),                // CN_DIR
  /* */ sizeof(DteVal),             // CN_DTEVAL
  /* */ sizeof(Dynamo),             // CN_DYNAMO

  /* */ 'E' - ('A' - 1),            // CN__E__
  /* */ sizeof(EdtVal),             // CN_EDTVAL
  /* */ sizeof(Encoding),           // CN_ENCODING
  /* */ sizeof(Ent),                // CN_ENT
  /* */ sizeof(EntBuffer),          // CN_ENTBUFFER
  /* */ sizeof(EntWrk),             // CN_ENTWRK
  /* */ sizeof(EnumFlags),          // CN_ENUMFLAGS
  /* */ sizeof(Exemplar),           // CN_EXEMPLAR
  /* */ sizeof(ExtractRow),         // CN_EXTRACTROW

  /* */ 'F' - ('A' - 1),            // CN__F__
  /* */ sizeof(FetchBuffer),        // CN_FETCHBUFFER
  /* */ sizeof(Fifo),               // CN_FIFO
  /* */ sizeof(File),               // CN_FILE
  /* */ sizeof(Filter_Fetch),       // CN_FILTER_FETCH
  /* */ sizeof(Filter_From),        // CN_FILTER_FROM
  /* */ sizeof(Filter_GroupBy),     // CN_FILTER_GROUPBY
  /* */ sizeof(Filter_Inflate),     // CN_FILTER_INFLATE
  /* */ sizeof(Filter_Join),        // CN_FILTER_JOIN
  /* */ sizeof(Filter_Limit),       // CN_FILTER_LIMIT
  /* */ sizeof(Filter_OrderBy),     // CN_FILTER_ORDERBY
  /* */ sizeof(Filter_Output),      // CN_FILTER_OUTPUT
  /* */ sizeof(Filter_Project),     // CN_FILTER_PROJECT
  /* */ sizeof(Filter_Scan),        // CN_FILTER_SCAN
  /* */ sizeof(Filter_Select),      // CN_FILTER_SELECT
  /* */ sizeof(Filter_SubQry),      // CN_FILTER_SUBQRY
  /* */ sizeof(Filter_Where),       // CN_FILTER_WHERE
  /* */ sizeof(Filter_Zed),         // CN_FILTER_ZED
  /* */ sizeof(FipVal),             // CN_FIPVAL
  /* */ sizeof(FltVal),             // CN_FLTVAL
  /* */ sizeof(FocalObjects),       // CN_FOCALOBJECTS
  /* */ sizeof(Forth),              // CN_FORTH
  /* */ sizeof(Format),             // CN_FORMAT
  /* */ sizeof(PoolBuffer),         // CN_FREEBUFFER
  /* */ sizeof(FreeChunk),          // CN_FREECHUNK
  /* */ 0,                          // CN_FREEDNOT
  /* */ 0,                          // CN_FREEITEM
  /* */ sizeof(FreqDist),           // CN_FREQDIST

  /* */ 'G' - ('A' - 1),            // CN__G__
  /* */ sizeof(GGadaptor),          // CN_GGADAPTOR
  /* */ sizeof(Grid),               // CN_GRID
  
  /* */ 'H' - ('A' - 1),            // CN__H__
  /* */ sizeof(HashBuffer),         // CN_HASHBUFFER
  /* */ sizeof(HashMap),            // CN_HASHMAP
  /* */ sizeof(HeapBuffer),         // CN_HEAPBUFFER
  /* */ sizeof(SP_Row),             // CN_HPROW

  /* */ 'I' - ('A' - 1),            // CN__I__
  /* */ sizeof(Index),              // CN_INDEX
  /* */ sizeof(IndexDef),           // CN_INDEXDEF
  /* */ sizeof(IndexTree),          // CN_INDEXTREE
  /* */ sizeof(IndexTreeVec),       // CN_INDEXTREEVEC
  /* */ sizeof(IdxColVec),          // CN_IDXCOLVEC
  /* */ sizeof(IdxVec),             // CN_IDXVEC
  /* */ sizeof(InfoSet),            // CN_INFOSET
  /* */ sizeof(IngestChunk),        // CN_INGESTCHUNK
  /* */ sizeof(Input),              // CN_INPUT
  /* */ sizeof(IntBuffer),          // CN_INTBUFFER
  /* */ sizeof(IntVal),             // CN_INTVAL
  /* */ sizeof(IoChunk),            // CN_IOCHUNK
  /* */ sizeof(IoCopy),             // CN_IOCOPY
  /* */ sizeof(IrwBuffer),          // CN_IRWBUFFER
  /* */ sizeof(IvlVal),             // CN_IVLVAL

  /* */ 'J' - ('A' - 1),            // CN__J__
  /* */ sizeof(Job__Find),          // CN_JOB__FIND
  /* */ sizeof(Job_Boot),           // CN_JOB_BOOT
  /* */ sizeof(Job_Distributed),    // CN_JOB_DISTRIBUTED
  /* */ sizeof(Job_Local),          // CN_JOB_LOCAL
  /* */ sizeof(Job_NodeInfo),       // CN_JOB_NODEINFO
  /* */ sizeof(Job_Operand),        // CN_JOB_OPERAND
  /* */ sizeof(Job_Receiver),       // CN_JOB_RECEIVER
  /* */ sizeof(Job_Start),          // CN_JOB_START
  /* */ sizeof(Job_WriteBuffer),    // CN_JOB_WRITEBUFFER
  /* */ sizeof(JobList),            // CN_JOBLIST
  /* */ sizeof(Heap), // correct    // CN_JUMBOPERSISTENT
  /* */ sizeof(Heap), // correct    // CN_JUMBOTRANSIENT
  
  /* */ 'K' - ('A' - 1),            // CN__K__
  /* */ sizeof(Key),                // CN_KEY
  
  /* */ 'L' - ('A' - 1),            // CN__L__
  /* */ sizeof(Heap), // correct    // CN_LARGEPERSISTENT
  /* */ sizeof(Heap), // correct    // CN_LARGETRANSIENT
  /* */ sizeof(Lexicon),            // CN_LEXICON
  /* */ sizeof(LoadChunk),          // CN_LOADCHUNK
  /* */ sizeof(Loader),             // CN_LOADER
  /* */ sizeof(LoadMap),            // CN_LOADMAP
  /* */ sizeof(Local),              // CN_LOCAL
  /* */ sizeof(LocalChain),         // CN_LOCALSCHAIN
  /* */ sizeof(Lock),               // CN_LOCK
  /* */ sizeof(Log),                // CN_LOG
  /* */ sizeof(LogEntry),           // CN_LOGENTRY

  /* */ 'M' - ('A' - 1),            // CN__M__
  /* */ sizeof(Marker),             // CN_MARKER
  /* */ sizeof(Heap), // correct    // CN_MEDIUMPERSISTENT
  /* */ sizeof(Heap), // correct    // CN_MEDIUMTRANSIENT
  /* */ sizeof(MemTest),            // CN_MEMTEST
  /* */ sizeof(MetaBlock),          // CN_METABLOCK
  /* */ sizeof(MetaCell),           // CN_METACELL
  /* */ sizeof(MetaGraph),          // CN_METAGRAPH
  /* */ sizeof(MetaInherit),        // CN_METAINHERIT
  /* */ sizeof(MetaInherits),       // CN_METAINHERITS
  /* */ sizeof(MetaMember),         // CN_METAMEMBER
  /* */ sizeof(MetaType),           // CN_METATYPE
  /* */ sizeof(Mutex),              // CN_MUTEX

  /* */ 'N' - ('A' - 1),            // CN__N__
  /* */ sizeof(NextGid),            // CN_NEXTGID
  /* */ sizeof(NmStr),              // CN_NMSTR
  /* */ sizeof(NmStrList),          // CN_NMSTRLIST
  /* */ sizeof(NmVal),              // CN_NMVAL
  /* */ sizeof(Node),               // CN_NODE
  /* */ sizeof(Now),                // CN_NOW
  /* */ sizeof(Numbat),             // CN_NUMBAT

  /* */ 'O' - ('A' - 1),            // CN__O__
  /* */ sizeof(ObjectsInPool),      // CN_OBJECTSINPOOL
  /* */ sizeof(Op),                 // CN_OP
  /* */ sizeof(OperandQueue),       // CN_OPERANDQUEUE
  /* */ sizeof(OpSpec),             // CN_OPSPEC

  /* */ 'P' - ('A' - 1),            // CN__P__
  /* */ sizeof(PathVec),            // CN_PATHVEC
  /* */ sizeof(Peruse),             // CN_PERUSE
  /* */ sizeof(Pipeline),           // CN_PIPELINE
  /* */ sizeof(PipelineSpec),       // CN_PIPELINESPEC
  /* */ sizeof(Plan),               // CN_PLAN
  /* */ sizeof(Planner),            // CN_PLANNER
  /* */ sizeof(Predicate),          // CN_PREDICATE
  /* */ sizeof(PredicateVec),       // CN_PREDICATEVEC
  /* */ sizeof(Profile),            // CN_PROFILE
  /* */ sizeof(PsxThd),             // CN_PSXTHD
  /* */ sizeof(PtnColVec),          // CN_PTNCOLVEC
  /* */ sizeof(PtrVal),             // CN_PTRVAL

  /* */ 'Q' - ('A' - 1),            // CN__Q__
  /* */ sizeof(Queue),              // CN_QUEUE
  /* */ sizeof(QueueBuffer),        // CN_QUEUEBUFFER
  /* */ sizeof(Queues),             // CN_QUEUES
  
  /* */ 'R' - ('A' - 1),            // CN__R__
  /* */ sizeof(ReadApi),            // CN_READAPI
  /* */ sizeof(Record),             // CN_RECORD
  /* */ sizeof(RecordBuffer),       // CN_RECORDBUFFER
  /* */ sizeof(RecordData),         // CN_RECORDDATA
  /* */ sizeof(RegSet),             // CN_REGSET
  /* */ sizeof(RelOp),              // CN_RELOP
  /* */ sizeof(Request),            // CN_REQUEST
  /* */ sizeof(ResourceTracker),    // CN_RESOURCETRACKER
  /* */ sizeof(Response),           // CN_RESPONSE
  /* */ sizeof(ResponseVec),        // CN_RESPONSEVEC
  /* */ sizeof(Result),             // CN_RESULT
  /* */ sizeof(Rand),               // CN_RN_RAND
  /* */ sizeof(Routine),            // CN_ROUTINE
  /* */ sizeof(Row),                // CN_ROW
  /* */ sizeof(Row),  //correct     // CN_ROWAGG
  /* */ sizeof(RowCompactor),       // CN_ROWCOMPACTOR
  /* */ sizeof(RowOnce),            // CN_ROWONCE
  /* */ sizeof(RowSingle),          // CN_ROWSINGLE
  /* */ sizeof(RowTray),            // CN_ROWTRAY
  /* */ 0,//sizeof(RU_Cpu),            // CN_RUCPU
  /* */ 0,//sizeof(RU_Dir),            // CN_RUDIR
  /* */ 0,//sizeof(RU_File),            // CN_RUFILE
  /* */ sizeof(RU_Memory),          // CN_RUMEMORY
  /* */ 0,//sizeof(RU_Mutex),            // CN_RUMUTEX
  /* */ 0,//sizeof(RU_Pass),            // CN_RUPASS
  /* */ 0,//sizeof(RU_Socket),            // CN_RUSOCKET
  /* */ 0,//sizeof(RU_Time),            // CN_RUTIME
  /* */ 0,//sizeof(RU_Word),            // CN_RUWORD

  /* */ 'S' - ('A' - 1),            // CN__S__
  /* */ sizeof(Schema),             // CN_SCHEMA
  /* */ sizeof(SchemaTest),         // CN_SCHEMATEST
  /* */ sizeof(SeeMe),              // CN_SEEME
  /* */ sizeof(SePrompt),           // CN_SEPROMPT
  /* */ sizeof(Session),            // CN_SESSION
  /* */ sizeof(SessionArray),       // CN_SESSIONARRAY
  /* */ sizeof(ShortCut),           // CN_SHORTCUT
  /* */ sizeof(ShortCutVec),        // CN_SHORTCUTVEC
  /* */ sizeof(Birth),              // CN_SINCE
  /* */ sizeof(SkipListNL),         // CN_SKIPLIST
  /* */ sizeof(SkipNode),           // CN_SKIPNODE
  /* */ sizeof(Slab),               // CN_SLAB
  /* */ sizeof(SlabAllocator),      // CN_SLABALLOCATOR
  /* */ sizeof(Sleep),              // CN_SLEEP
  /* */ sizeof(Heap), // correct    // CN_SMALLPERSISTENT
  /* */ sizeof(Heap), // correct    // CN_SMALLTRANSIENT
  /* */ sizeof(SN_Bcd),             // CN_SNBCD
  /* */ sizeof(SN_Bol),             // CN_SNBOL
  /* */ sizeof(SN_Dte),             // CN_SNDTE
  /* */ sizeof(SN_Flt),             // CN_SNFLT
  /* */ sizeof(SN_Int),             // CN_SNINT
  /* */ sizeof(SN_Ivl),             // CN_SNIVL
  /* */ sizeof(SN_Tme),             // CN_SNTME
  /* */ sizeof(SN_Tsp),             // CN_SNTSP
  /* */ sizeof(SN_Vch),             // CN_SNVCH
  /* */ sizeof(Socket),             // CN_SOCKET
  /* */ sizeof(SP_Bcd),             // CN_SPBCD
  /* */ sizeof(SP_Bol),             // CN_SPBOL
  /* */ sizeof(SP_Dte),             // CN_SPDTE
  /* */ sizeof(SP_Elapsed),         // CN_SPELAPSED
  /* */ sizeof(SP_Flt),             // CN_SPFLT
  /* */ sizeof(SP_Gid),             // CN_SPID
  /* */ sizeof(SP_Int),             // CN_SPINT
  /* */ sizeof(SP_Ivl),             // CN_SPIVL
  /* */ sizeof(SP_String),          // CN_SPSTRING
  /* */ sizeof(SP_TknVecStr),       // CN_SPTKNVECSTR
  /* */ sizeof(SP_Tme),             // CN_SPTME
  /* */ sizeof(SP_Tsp),             // CN_SPTSP
  /* */ sizeof(SP_Vch),             // CN_SPVCH
  /* */ sizeof(SrtBuffer),          // CN_SRTBUFFER
  /* */ sizeof(SrtSet),             // CN_SRTSET
  /* */ sizeof(Stack),              // CN_STACK
  /* */ sizeof(Stage),              // CN_STAGE
  /* */ sizeof(StageSpec),          // CN_STAGESPEC
  /* */ sizeof(Statistics),         // CN_STATISTICS
  /* */ sizeof(StepAccum),          // CN_STEPACCUM
  /* */ sizeof(StepTiming),         // CN_STEPTIMING
  /* */ sizeof(Stmt_Select),        // CN_STMT_SELECT
  /* */ sizeof(Strand),             // CN_STRAND
  /* */ sizeof(StreamDef),          // CN_STREAMDEF
  /* */ sizeof(StreamSet),          // CN_STREAMSET
  /* */ sizeof(s32),                // CN_STRING_T
  /* */ sizeof(StzVal),             // CN_STZVAL
  /* */ sizeof(SysCall),            // CN_SYSCALL
  
  /* */ 'T' - ('A' - 1),            // CN__T__
  /* */ sizeof(Table),              // CN_TABLE
  /* */ sizeof(TableDef),           // CN_TABLEDEF
  /* */ sizeof(TablePtn),           // CN_TABLEPTN
  /* */ sizeof(TablePtnVec),        // CN_TABLEPTNVEC
  /* */ sizeof(TagSet),             // CN_TAGSET
  /* */ sizeof(Task),               // CN_TASK
  /* */ sizeof(TaskList),           // CN_TASKLIST
  /* */ sizeof(TaskQueue),          // CN_TASKQUEUE
  /* */ sizeof(TempName),           // CN_TEMPNAME
  /* */ sizeof(Tenant),             // CN_TENANT
  /* */ sizeof(Tenants),            // CN_TENANTS
  /* */ sizeof(Thread),             // CN_THREAD
  /* */ sizeof(ThreadQueue),        // CN_THREADQUEUE
  /* */ sizeof(ThreadVec),          // CN_THREADVEC
  /* */ sizeof(Timer),              // CN_TIMER
  /* */ sizeof(Heap), // correct    // CN_TINYPERSISTENT
  /* */ sizeof(Heap), // correct    // CN_TINYTRANSIENT
  /* */ sizeof(TmeVal),             // CN_TMEVAL
  /* */ sizeof(Token),              // CN_TOKEN
  /* */ sizeof(TokenVec),           // CN_TOKENVEC
  /* */ sizeof(TspVal),             // CN_TSPVAL
  /* */ sizeof(Tx),                 // CN_TX
  /* */ sizeof(TxSet),              // CN_TXSET

  /* */ 'U' - ('A' - 1),            // CN__U__
  
  /* */ 'V' - ('A' - 1),            // CN__V__
  /* */ sizeof(Vortex),             // CN_VORTEX

  /* */ 'W' - ('A' - 1),            // CN__W__
  /* */ sizeof(Wilderness),         // CN_WILDERNESS
  /* */ sizeof(Word),               // CN_WORD
  /* */ sizeof(WriteBehind),        // CN_WRITEBEHIND
  
  /* */ 'X' - ('A' - 1),            // CN__X__
  /* */ sizeof(Xmits),              // CN_XMITS
  
  /* */ 'Y' - ('A' - 1),            // CN__Y__

  /* */ 'Z' - ('A' - 1),            // CN__Z__
  
  /* */ -1
};

/*----------------------------------------------------------------------------*/
/* which heap                                                                 */
/*----------------------------------------------------------------------------*/
eHT geWhichHeap[]
{
  /* */ HT_INVALID,           // CN_NONE
  
  /* */ HT_INVALID,           // CN__A__
  /* */ HT_TRANSIENT_MEDIUM,  // CN_ACCESSPATH
  /* */ HT_TRANSIENT_TINY,    // CN_AGGTIMER
  /* */ HT_TRANSIENT_TINY,    // CN_ARG
  /* */ HT_INVALID,           // CN_ARGUMENTS
  /* */ HT_TRANSIENT_TINY,    // CN_ARTICLE
  /* */ HT_INVALID,           // CN_ATOMIC
  /* */ HT_INVALID,           // CN_ATTACHMAP

  /* */ HT_INVALID,           // CN__B__
  /* */ HT_INVALID,           // CN_BCDVAL
  /* */ HT_INVALID,           // CN_BIRTH
  /* */ HT_INVALID,           // CN_BOLVAL
  /* */ HT_TRANSIENT_MEDIUM,  // CN_BPA
  /* */ HT_TRANSIENT_MEDIUM,  // CN_BUCKETMAP
  /* */ HT_TRANSIENT_LARGE,   // CN_BUFFERCURSOR
  /* */ HT_INVALID,           // CN_BUFFERPOOL
  /* */ HT_TRANSIENT_TINY,    // CN_BYTERATE
  /* */ HT_INVALID,           // CN_BYTES_T

  /* */ HT_INVALID,           // CN__C__
  /* */ HT_TRANSIENT_MEDIUM,  // CN_CALCULATOR
  /* */ HT_INVALID,           // CN_CARRYOVER
  /* */ HT_INVALID,           // CN_CATALOG
  /* */ HT_TRANSIENT_MEDIUM,  // CN_CHARSER
  /* */ HT_TRANSIENT_MEDIUM,  // CN_CLAUSE_FROM
  /* */ HT_TRANSIENT_MEDIUM,  // CN_CLAUSE_GROUPBY
  /* */ HT_TRANSIENT_MEDIUM,  // CN_CLAUSE_JOIN
  /* */ HT_TRANSIENT_MEDIUM,  // CN_CLAUSE_LIMIT
  /* */ HT_TRANSIENT_MEDIUM,  // CN_CLAUSE_ORDERBY
  /* */ HT_TRANSIENT_MEDIUM,  // CN_CLAUSE_SELECT
  /* */ HT_TRANSIENT_MEDIUM,  // CN_CLAUSE_SUBQRY
  /* */ HT_TRANSIENT_MEDIUM,  // CN_CLAUSE_WHERE
  /* */ HT_TRANSIENT_MEDIUM,  // CN_CMD_SELECT
  /* */ HT_TRANSIENT_TINY,    // CN_COLUMN
  /* */ HT_TRANSIENT_MEDIUM,  // CN_COMMAND
  /* */ HT_TRANSIENT_TINY,    // CN_COMPILER
  /* */ HT_TRANSIENT_MEDIUM,  // CN_CONSTDEF
  /* */ HT_TRANSIENT_MEDIUM,  // CN_CONSTSET
  /* */ HT_TRANSIENT_LARGE,   // CN_CRVBUFFER
  /* */ HT_TRANSIENT_LARGE,   // CN_CRWBUFFER
  /* */ HT_TRANSIENT_LARGE,   // CN_CSVBUFFER

  /* */ HT_INVALID,           // CN__D__
  /* */ HT_TRANSIENT_MEDIUM,  // CN_DATUM
  /* */ HT_TRANSIENT_TINY,    // CN_DECACNT
  /* */ HT_TRANSIENT_TINY,    // CN_DECIDER
  /* */ HT_TRANSIENT_TINY,    // CN_DIC
  /* */ HT_TRANSIENT_TINY,    // CN_DICSTACK
  /* */ HT_TRANSIENT_TINY,    // CN_DIR
  /* */ HT_INVALID,           // CN_DTEVAL
  /* */ HT_TRANSIENT_TINY,    // CN_DYNAMO

  /* */ HT_INVALID,           // CN__E__
  /* */ HT_INVALID,           // CN_EDTVAL
  /* */ HT_TRANSIENT_LARGE,   // CN_ENCODING
  /* */ HT_INVALID,           // CN_ENT
  /* */ HT_TRANSIENT_LARGE,   // CN_ENTBUFFER
  /* */ HT_INVALID,           // CN_ENTWRK
  /* */ HT_TRANSIENT_TINY,    // CN_ENUMFLAGS
  /* */ HT_TRANSIENT_TINY,    // CN_EXEMPLAR
  /* */ HT_TRANSIENT_TINY,    // CN_EXTRACTROW

  /* */ HT_INVALID,           // CN__F__
  /* */ HT_TRANSIENT_LARGE,   // CN_FETCHBUFFER
  /* */ HT_TRANSIENT_TINY,    // CN_FIFO
  /* */ HT_TRANSIENT_TINY,    // CN_FILE
  /* */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_FETCH
  /* */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_FROM
  /* */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_GROUPBY
  /* */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_INFLATE
  /* */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_JOIN
  /* */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_LIMIT
  /* */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_ORDERBY
  /* */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_OUTPUT
  /* */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_PROJECT
  /* */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_SCAN
  /* */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_SELECT
  /* */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_SUBQRY
  /* */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_WHERE
  /* */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_ZED
  /* */ HT_INVALID,           // CN_FIPVAL
  /* */ HT_INVALID,           // CN_FLTVAL
  /* */ HT_TRANSIENT_LARGE,   // CN_FOCALOBJECTS
  /* */ HT_TRANSIENT_TINY,    // CN_FORTH
  /* */ HT_TRANSIENT_TINY,    // CN_FORMAT
  /* */ HT_INVALID,           // CN_FREEBUFFER
  /* */ HT_INVALID,           // CN_FREECHUNK
  /* */ HT_INVALID,           // CN_FREEDNOT
  /* */ HT_INVALID,           // CN_FREEITEM
  /* */ HT_TRANSIENT_SMALL,   // CN_FREQDIST

  /* */ HT_INVALID,           // CN__G__
  /* */ HT_TRANSIENT_TINY,    // CN_GGADAPTOR
  /* */ HT_TRANSIENT_SMALL,   // CN_GRID

  /* */ HT_INVALID,           // CN__H__
  /* */ HT_TRANSIENT_LARGE,   // CN_HASHBUFFER
  /* */ HT_TRANSIENT_TINY,    // CN_HASHMAP
  /* */ HT_INVALID,           // CN_HEAPBUFFER
  /* */ HT_INVALID,           // CN_HPROW

  /* */ HT_INVALID,           // CN__I__
  /* */ HT_TRANSIENT_TINY,    // CN_INDEX
  /* */ HT_TRANSIENT_MEDIUM,  // CN_INDEXDEF
  /* */ HT_TRANSIENT_TINY,    // CN_INDEXTREE
  /* */ HT_TRANSIENT_MEDIUM,  // CN_INDEXTREEVEC
  /* */ HT_TRANSIENT_MEDIUM,  // CN_IDXCOLVEC
  /* */ HT_TRANSIENT_MEDIUM,  // CN_IDXVEC
  /* */ HT_TRANSIENT_LARGE,   // CN_INFOSET
  /* */ HT_TRANSIENT_LARGE,   // CN_INGESTCHUNK
  /* */ HT_TRANSIENT_TINY,    // CN_INPUT
  /* */ HT_TRANSIENT_LARGE,   // CN_INTBUFFER
  /* */ HT_INVALID,           // CN_INTVAL
  /* */ HT_TRANSIENT_LARGE,   // CN_IOCHUNK
  /* */ HT_TRANSIENT_TINY,    // CN_IOCOPY
  /* */ HT_TRANSIENT_LARGE,   // CN_IRWBUFFER
  /* */ HT_INVALID,           // CN_IVLVAL

  /* */ HT_INVALID,           // CN__J__
  /* */ HT_TRANSIENT_LARGE,   // CN_JOB__FIND
  /* */ HT_TRANSIENT_MEDIUM,  // CN_JOB_BOOT
  /* */ HT_TRANSIENT_MEDIUM,  // CN_JOB_DISTRIBUTED
  /* */ HT_TRANSIENT_LARGE,   // CN_JOB_LOCAL
  /* */ HT_TRANSIENT_LARGE,   // CN_JOB_NODEINFO
  /* */ HT_TRANSIENT_LARGE,   // CN_JOB_OPERAND
  /* */ HT_TRANSIENT_MEDIUM,  // CN_JOB_RECEIVER
  /* */ HT_TRANSIENT_MEDIUM,  // CN_JOB_START
  /* */ HT_TRANSIENT_LARGE,   // CN_JOB_WRITEBUFFER
  /* */ HT_TRANSIENT_TINY,    // CN_JOBLIST
  /* */ HT_INVALID,           // CN_JUMBOPERSISTENT
  /* */ HT_INVALID,           // CN_JUMBOTRANSIENT
  
  /* */ HT_INVALID,           // CN__K__
  /* */ HT_INVALID,           // CN_KEY

  /* */ HT_INVALID,           // CN__L__
  /* */ HT_INVALID,           // CN_LARGEPERSISTENT
  /* */ HT_INVALID,           // CN_LARGETRANSIENT
  /* */ HT_TRANSIENT_TINY,    // CN_LEXICON
  /* */ HT_TRANSIENT_LARGE,   // CN_LOADCHUNK
  /* */ HT_TRANSIENT_LARGE,   // CN_LOADER
  /* */ HT_TRANSIENT_JUMBO,   // CN_LOADMAP
  /* */ HT_INVALID,           // CN_LOCAL
  /* */ HT_TRANSIENT_LARGE,   // CN_LOCALSCHAIN
  /* */ HT_INVALID,           // CN_LOCK
  /* */ HT_INVALID,           // CN_LOG
  /* */ HT_INVALID,           // CN_LOGENTRY

  /* */ HT_INVALID,           // CN__M__
  /* */ HT_TRANSIENT_TINY,    // CN_MARKER
  /* */ HT_INVALID,           // CN_MEDIUMPERSISTENT
  /* */ HT_INVALID,           // CN_MEDIUMTRANSIENT
  /* */ HT_TRANSIENT_TINY,    // CN_MEMTEST
  /* */ HT_TRANSIENT_MEDIUM,  // CN_METABLOCK
  /* */ HT_TRANSIENT_TINY,    // CN_METACELL
  /* */ HT_TRANSIENT_MEDIUM,  // CN_METAGRAPH
  /* */ HT_TRANSIENT_MEDIUM,  // CN_METAINHERIT
  /* */ HT_TRANSIENT_TINY,    // CN_METAINHERITS
  /* */ HT_TRANSIENT_TINY,    // CN_METAMEMBER
  /* */ HT_TRANSIENT_MEDIUM,  // CN_METATYPE
  /* */ HT_TRANSIENT_TINY,    // CN_MUTEX

  /* */ HT_INVALID,           // CN__N__
  /* */ HT_INVALID,           // CN_NEXTGID
  /* */ HT_TRANSIENT_MEDIUM,  // CN_NMSTR
  /* */ HT_TRANSIENT_TINY,    // CN_NMLIST
  /* */ HT_TRANSIENT_TINY,    // CN_NMVAL
  /* */ HT_TRANSIENT_TINY,    // CN_NODE
  /* */ HT_TRANSIENT_TINY,    // CN_NOW
  /* */ HT_TRANSIENT_LARGE,   // CN_NUMBAT

  /* */ HT_INVALID,           // CN__O__
  /* */ HT_TRANSIENT_SMALL,   // CN_OBJECTSINPOOL
  /* */ HT_TRANSIENT_MEDIUM,  // CN_OP
  /* */ HT_TRANSIENT_TINY,    // CN_OPERANDQUEUE
  /* */ HT_TRANSIENT_MEDIUM,  // CN_OPSPEC

  /* */ HT_INVALID,           // CN__P__
  /* */ HT_TRANSIENT_MEDIUM,  // CN_PATHVEC
  /* */ HT_TRANSIENT_MEDIUM,  // CN_PERUSE
  /* */ HT_TRANSIENT_MEDIUM,  // CN_PIPELINE
  /* */ HT_TRANSIENT_SMALL,   // CN_PIPELINESPEC
  /* */ HT_TRANSIENT_MEDIUM,  // CN_PLAN
  /* */ HT_TRANSIENT_MEDIUM,  // CN_PLANNER
  /* */ HT_TRANSIENT_TINY,    // CN_PREDICATE
  /* */ HT_TRANSIENT_MEDIUM,  // CN_PREDICATEVEC
  /* */ HT_TRANSIENT_MEDIUM,  // CN_PROFILE
  /* */ HT_TRANSIENT_TINY,    // CN_PSXTHD
  /* */ HT_TRANSIENT_MEDIUM,  // CN_PTNCOLVEC
  /* */ HT_INVALID,           // CN_PTRVAL

  /* */ HT_INVALID,           // CN__Q__
  /* */ HT_TRANSIENT_TINY,    // CN_QUEUE
  /* */ HT_INVALID,           // CN_QUEUEBUFFER
  /* */ HT_TRANSIENT_TINY,    // CN_QUEUES
  
  /* */ HT_INVALID,           // CN__R__
  /* */ HT_TRANSIENT_TINY,    // CN_READAPI
  /* */ HT_TRANSIENT_MEDIUM,  // CN_RECORD
  /* */ HT_TRANSIENT_LARGE,   // CN_RECORDBUFFER
  /* */ HT_TRANSIENT_LARGE,   // CN_RECORDDATA
  /* */ HT_TRANSIENT_TINY,    // CN_REGSET
  /* */ HT_TRANSIENT_TINY,    // CN_RELOP
  /* */ HT_TRANSIENT_TINY,    // CN_REQUEST
  /* */ HT_TRANSIENT_TINY,    // CN_RESOURCETRACKER
  /* */ HT_TRANSIENT_MEDIUM,  // CN_RESPONSE
  /* */ HT_TRANSIENT_MEDIUM,  // CN_RESPONSEVEC
  /* */ HT_TRANSIENT_MEDIUM,  // CN_RESULT
  /* */ HT_INVALID,           // CN_RN_RAND
  /* */ HT_TRANSIENT_MEDIUM,  // CN_ROUTINE
  /* */ HT_TRANSIENT_LARGE,   // CN_ROW
  /* */ HT_TRANSIENT_MEDIUM,  // CN_ROWAGG
  /* */ HT_TRANSIENT_LARGE,   // CN_ROWCOMPACTOR
  /* */ HT_TRANSIENT_TINY,    // CN_ROWONCE
  /* */ HT_TRANSIENT_LARGE,   // CN_ROWSINGLE
  /* */ HT_TRANSIENT_LARGE,   // CN_ROWTRAY
  /* */ HT_INVALID,           // CN_RUCPU
  /* */ HT_INVALID,           // CN_RUDIR
  /* */ HT_INVALID,           // CN_RUFILE
  /* */ HT_INVALID,           // CN_RUMEMORY
  /* */ HT_INVALID,           // CN_RUMUTEX
  /* */ HT_INVALID,           // CN_RUPASS
  /* */ HT_INVALID,           // CN_RUSOCKET
  /* */ HT_INVALID,           // CN_RUTIME
  /* */ HT_INVALID,           // CN_RUWORK

  /* */ HT_INVALID,           // CN__S__
  /* */ HT_TRANSIENT_TINY,    // CN_SCHEMA
  /* */ HT_TRANSIENT_LARGE,   // CN_SCHEMATEST
  /* */ HT_INVALID,           // CN_SEEME
  /* */ HT_TRANSIENT_MEDIUM,  // CN_SEPROMPT
  /* */ HT_TRANSIENT_MEDIUM,  // CN_SESSION
  /* */ HT_TRANSIENT_TINY,    // CN_SESSIONARRAY
  /* */ HT_TRANSIENT_MEDIUM,  // CN_SHORTCUT
  /* */ HT_TRANSIENT_MEDIUM,  // CN_SHORTCUTVEC
  /* */ HT_INVALID,           // CN_SINCE
  /* */ HT_TRANSIENT_TINY,    // CN_SKIPLIST
  /* */ HT_TRANSIENT_TINY,    // CN_SKIPNODE
  /* */ HT_TRANSIENT_LARGE,   // CN_SLAB
  /* */ HT_TRANSIENT_TINY,    // CN_SLABALLOCATOR
  /* */ HT_INVALID,           // CN_SLEEP
  /* */ HT_INVALID,           // SN_SMALLPERSISTENT
  /* */ HT_INVALID,           // SN_SMALLTRANSIENT
  /* */ HT_INVALID,           // CN_SNBCD
  /* */ HT_INVALID,           // CN_SNBOL
  /* */ HT_INVALID,           // CN_SNDTE
  /* */ HT_INVALID,           // CN_SNFLT
  /* */ HT_INVALID,           // CN_SNINT
  /* */ HT_INVALID,           // CN_SNIVL
  /* */ HT_INVALID,           // CN_SNTME
  /* */ HT_INVALID,           // CN_SNTSP
  /* */ HT_INVALID,           // CN_SNVCH
  /* */ HT_TRANSIENT_TINY,    // CN_SOCKET
  /* */ HT_INVALID,           // CN_SPBCD
  /* */ HT_INVALID,           // CN_SPBOL
  /* */ HT_INVALID,           // CN_SPDTE
  /* */ HT_INVALID,           // CN_SPELAPSED
  /* */ HT_INVALID,           // CN_SPFLT
  /* */ HT_INVALID,           // CN_SPID
  /* */ HT_INVALID,           // CN_SPINT
  /* */ HT_INVALID,           // CN_SPIVL
  /* */ HT_INVALID,           // CN_SPSTRING
  /* */ HT_INVALID,           // CN_SPTKNVECSTR
  /* */ HT_INVALID,           // CN_SPTME
  /* */ HT_INVALID,           // CN_SPTSP
  /* */ HT_INVALID,           // CN_SPVCH
  /* */ HT_TRANSIENT_LARGE,   // CN_SRTBUFFER
  /* */ HT_INVALID,           // CN_SRTSET
  /* */ HT_TRANSIENT_LARGE,   // CN_STACK
  /* */ HT_TRANSIENT_MEDIUM,  // CN_STAGE
  /* */ HT_TRANSIENT_SMALL,   // CN_STAGESPEC
  /* */ HT_INVALID,           // CN_STEPACCUM
  /* */ HT_INVALID,           // CN_STATISTICS
  /* */ HT_TRANSIENT_LARGE,   // CN_STEPTIMING
  /* */ HT_TRANSIENT_LARGE,   // CN_STMT_SELECT
  /* */ HT_TRANSIENT_MEDIUM,  // CN_STRAND
  /* */ HT_TRANSIENT_LARGE,   // CN_STREAMDEF
  /* */ HT_TRANSIENT_LARGE,   // CN_STREAMSET
  /* */ HT_INVALID,           // CN_STRING_T
  /* */ HT_INVALID,           // CN_STZVAL
  /* */ HT_INVALID,           // CN_SYSCALL
  
  /* */ HT_INVALID,           // CN__T__
  /* */ HT_TRANSIENT_MEDIUM,  // CN_TABLE
  /* */ HT_TRANSIENT_MEDIUM,  // CN_TABLEDEF
  /* */ HT_TRANSIENT_TINY,    // CN_TABLEPTN
  /* */ HT_TRANSIENT_SMALL,   // CN_TABLEPTNVEC
  /* */ HT_TRANSIENT_MEDIUM,  // CN_TAGSET
  /* */ HT_TRANSIENT_TINY,    // CN_TASK
  /* */ HT_TRANSIENT_TINY,    // CN_TASKLIST
  /* */ HT_TRANSIENT_TINY,    // CN_TEMPNAME
  /* */ HT_TRANSIENT_TINY,    // CN_TENANT
  /* */ HT_TRANSIENT_TINY,    // CN_TENANTS
  /* */ HT_TRANSIENT_TINY,    // CN_TASKQUEUE
  /* */ HT_TRANSIENT_TINY,    // CN_THREAD
  /* */ HT_TRANSIENT_TINY,    // CN_THREADQUEUE
  /* */ HT_TRANSIENT_SMALL,   // CN_THREADVEC
  /* */ HT_TRANSIENT_TINY,    // CN_TIMER
  /* */ HT_INVALID,           // CN_TINYPERSISTENT
  /* */ HT_INVALID,           // CN_TINYTRANSIENT
  /* */ HT_INVALID,           // CN_TMEVAL
  /* */ HT_TRANSIENT_TINY,    // CN_TOKEN
  /* */ HT_TRANSIENT_LARGE,   // CN_TOKENVEC
  /* */ HT_INVALID,           // CN_TSPVAL
  /* */ HT_TRANSIENT_TINY,    // CN_TX
  /* */ HT_TRANSIENT_TINY,    // CN_TXSET
  
  /* */ HT_INVALID,           // CN__U__
  
  /* */ HT_INVALID,           // CN__V__
  /* */ HT_TRANSIENT_LARGE,   // CN_VORTEX

  /* */ HT_INVALID,           // CN__W__
  /* */ HT_INVALID,           // CN_WILDERNESS
  /* */ HT_TRANSIENT_TINY,    // CN_WORD
  
  /* */ HT_TRANSIENT_LARGE,   // CN_WRITEBEHIND

  /* */ HT_INVALID,           // CN__X__
  /* */ HT_TRANSIENT_TINY,    // CN_XMITS
  
  /* */ HT_INVALID,           // CN__Y__
  
  /* */ HT_INVALID,           // CN__Z__
  
  /* */ HT_INVALID,           // CN_ENUMS
};





/*----------------------------------------------------------------------------*/
/* sizes & heaps                                                              */
/*----------------------------------------------------------------------------*/
SizeHeap gxaSizeHeap[]
{
  { 0,                          HT_INVALID,           }, // CN_NONE

  { 'A' - ('A' - 1),            HT_INVALID,           }, // CN__A__
  { sizeof(AccessPath),         HT_TRANSIENT_MEDIUM,  }, // CN_ACCESSPATH
  { sizeof(AggTimer),           HT_TRANSIENT_TINY,    }, // CN_AGGTIMER
  { sizeof(Arg),                HT_TRANSIENT_TINY,    }, // CN_ARG
  { sizeof(Args),               HT_INVALID,           }, // CN_ARGUMENTS
  { sizeof(Article),            HT_TRANSIENT_TINY,    }, // CN_ARTICLE
  { sizeof(Atomic),             HT_INVALID,           }, // CN_ATOMIC
  { sizeof(AttachMap),          HT_INVALID,           }, // CN_ATTACHMAP

  { 'B' - ('A' - 1),            HT_INVALID,           }, // CN__B__
  { sizeof(BcdVal),             HT_INVALID,           }, // CN_BCDVAL
  { sizeof(Birth),              HT_INVALID,           }, // CN_BIRTH
  { sizeof(BolVal),             HT_INVALID,           }, // CN_BOLVAL
  { sizeof(BPA),                HT_TRANSIENT_MEDIUM,  }, // CN_BPA
  { sizeof(BucketMap),          HT_TRANSIENT_MEDIUM,  }, // CN_BUCKETMAP
  { sizeof(BufferCursor),       HT_TRANSIENT_LARGE,   }, // CN_BUFFERCURSOR
  { sizeof(BufferPool),         HT_INVALID,           }, // CN_BUFFERPOOL
  { sizeof(ByteRate),           HT_TRANSIENT_TINY,    }, // CN_BYTERATE
  { sizeof(s32),                HT_INVALID,           }, // CN_BYTES_T

  { 'C' - ('A' - 1),            HT_INVALID,           }, // CN__C__
  { sizeof(Calculator),         HT_TRANSIENT_MEDIUM,  }, // CN_CALCULATOR
  { sizeof(CarryOver),          HT_INVALID,           }, // CN_CARRYOVER
  { sizeof(Catalog),            HT_INVALID,           }, // CN_CATALOG
  { sizeof(Charser),            HT_TRANSIENT_MEDIUM,  }, // CN_CHARSER
  { sizeof(Clause_From),        HT_TRANSIENT_MEDIUM,  }, // CN_CLAUSE_FROM
  { sizeof(Clause_GroupBy),     HT_TRANSIENT_MEDIUM,  }, // CN_CLAUSE_GROUPBY
  { sizeof(Clause_Join),        HT_TRANSIENT_MEDIUM,  }, // CN_CLAUSE_JOIN
  { sizeof(Clause_Limit),       HT_TRANSIENT_MEDIUM,  }, // CN_CLAUSE_LIMIT
  { sizeof(Clause_OrderBy),     HT_TRANSIENT_MEDIUM,  }, // CN_CLAUSE_ORDERBY
  { sizeof(Clause_Select),      HT_TRANSIENT_MEDIUM,  }, // CN_CLAUSE_SELECT
  { sizeof(Clause_SubQry),      HT_TRANSIENT_MEDIUM,  }, // CN_CLAUSE_SUBQRY
  { sizeof(Clause_Where),       HT_TRANSIENT_MEDIUM,  }, // CN_CLAUSE_WHERE
  { sizeof(Cmd_Select),         HT_TRANSIENT_MEDIUM,  }, // CN_CMD_SELECT
  { sizeof(Column),             HT_TRANSIENT_TINY,    }, // CN_COLUMN
  { sizeof(Request),            HT_TRANSIENT_MEDIUM,  }, // CN_COMMAND
  { sizeof(Compiler),           HT_TRANSIENT_TINY,    }, // CN_COMPILER
  { sizeof(ConstDef),           HT_TRANSIENT_MEDIUM,  }, // CN_CONSTDEF
  { sizeof(ConstSet),           HT_TRANSIENT_MEDIUM,  }, // CN_CONSTSET
  { sizeof(CrvBuffer),          HT_TRANSIENT_LARGE,   }, // CN_CRVBUFFER
  { sizeof(CrwBuffer),          HT_TRANSIENT_LARGE,   }, // CN_CRWBUFFER
  { sizeof(CsvBuffer),          HT_TRANSIENT_LARGE,   }, // CN_CSVBUFFER

  { 'D' - ('A' - 1),            HT_INVALID,           }, // CN__D__
  { sizeof(Datum_h),            HT_TRANSIENT_MEDIUM,  }, // CN_DATUM
  { sizeof(DecaCnt),            HT_TRANSIENT_TINY,    }, // CN_DECACNT
  { sizeof(Decider),            HT_TRANSIENT_TINY,    }, // CN_DECIDER
  { sizeof(Dic),                HT_TRANSIENT_TINY,    }, // CN_DIC
  { sizeof(DicStack),           HT_TRANSIENT_TINY,    }, // CN_DICSTACK
  { sizeof(Dir),                HT_TRANSIENT_TINY,    }, // CN_DIR
  { sizeof(DteVal),             HT_INVALID,           }, // CN_DTEVAL
    { sizeof(Dynamo),             HT_TRANSIENT_TINY,  }, // CN_DYNAMO

  { 'E' - ('A' - 1),            HT_INVALID,           }, // CN__E__
  { sizeof(EdtVal),             HT_INVALID,           }, // CN_EDTVAL
  { sizeof(Encoding),           HT_TRANSIENT_LARGE,   }, // CN_ENCODING
  { sizeof(Ent),                HT_INVALID,           }, // CN_ENT
  { sizeof(EntBuffer),          HT_TRANSIENT_LARGE,   }, // CN_ENTBUFFER
  { sizeof(EntWrk),             HT_INVALID,           }, // CN_ENTWRK
  { sizeof(EnumFlags),          HT_TRANSIENT_TINY,    }, // CN_ENUMFLAGS
  { sizeof(Exemplar),           HT_TRANSIENT_TINY,    }, // CN_EXEMPLAR
    { sizeof(ExtractRow),         HT_TRANSIENT_TINY,  }, // CN_EXTRACTROW

  { 'F' - ('A' - 1),            HT_INVALID,           }, // CN__F__
  { sizeof(FetchBuffer),        HT_TRANSIENT_LARGE,   }, // CN_FETCHBUFFER
  { sizeof(Fifo),               HT_TRANSIENT_TINY,    }, // CN_FIFO
  { sizeof(File),               HT_TRANSIENT_TINY,    }, // CN_FILE
  { sizeof(Filter_Fetch),       HT_TRANSIENT_MEDIUM,  }, // CN_FILTER_FETCH
  { sizeof(Filter_From),        HT_TRANSIENT_MEDIUM,  }, // CN_FILTER_FROM
  { sizeof(Filter_GroupBy),     HT_TRANSIENT_MEDIUM,  }, // CN_FILTER_GROUPBY
  { sizeof(Filter_Inflate),     HT_TRANSIENT_MEDIUM,  }, // CN_FILTER_INFLATE
  { sizeof(Filter_Join),        HT_TRANSIENT_MEDIUM,  }, // CN_FILTER_JOIN
  { sizeof(Filter_Limit),       HT_TRANSIENT_MEDIUM,  }, // CN_FILTER_LIMIT
  { sizeof(Filter_OrderBy),     HT_TRANSIENT_MEDIUM,  }, // CN_FILTER_ORDERBY
  { sizeof(Filter_Output),      HT_TRANSIENT_MEDIUM,  }, // CN_FILTER_OUTPUT
  { sizeof(Filter_Project),     HT_TRANSIENT_MEDIUM,  }, // CN_FILTER_PROJECT
  { sizeof(Filter_Scan),        HT_TRANSIENT_MEDIUM,  }, // CN_FILTER_SCAN
  { sizeof(Filter_Select),      HT_TRANSIENT_MEDIUM,  }, // CN_FILTER_SELECT
  { sizeof(Filter_SubQry),      HT_TRANSIENT_MEDIUM,  }, // CN_FILTER_SUBQRY
  { sizeof(Filter_Where),       HT_TRANSIENT_MEDIUM,  }, // CN_FILTER_WHERE
  { sizeof(Filter_Zed),         HT_TRANSIENT_MEDIUM,  }, // CN_FILTER_ZED
  { sizeof(FipVal),             HT_INVALID,           }, // CN_FIPVAL
  { sizeof(FltVal),             HT_INVALID,           }, // CN_FLTVAL
  { sizeof(FocalObjects),       HT_TRANSIENT_LARGE,   }, // CN_FOCALOBJECTS
  { sizeof(Forth),              HT_TRANSIENT_TINY,    }, // CN_FORTH
  { sizeof(Format),             HT_TRANSIENT_TINY,    }, // CN_FORMAT
  { sizeof(PoolBuffer),         HT_INVALID,           }, // CN_FREEBUFFER
  { sizeof(FreeChunk),          HT_INVALID,           }, // CN_FREECHUNK
  { 0,                          HT_INVALID,           }, // CN_FREEDNOT
  { 0,                          HT_INVALID,           }, // CN_FREEITEM
  { sizeof(FreqDist),           HT_TRANSIENT_SMALL,   }, // CN_FREQDIST

  { 'G' - ('A' - 1),            HT_INVALID,           }, // CN__G__
  { sizeof(GGadaptor),          HT_TRANSIENT_TINY,    }, // CN_GGADAPTOR
  { sizeof(Grid),               HT_TRANSIENT_SMALL,   }, // CN_GRID

  { 'H' - ('A' - 1),            HT_INVALID,           }, // CN__H__
  { sizeof(HashBuffer),         HT_TRANSIENT_LARGE,   }, // CN_HASHBUFFER
  { sizeof(HashMap),            HT_TRANSIENT_TINY,    }, // CN_HASHMAP
  { sizeof(HeapBuffer),         HT_INVALID,           }, // CN_HEAPBUFFER
    { sizeof(SP_Row),             HT_INVALID,         }, // CN_HPROW

  { 'I' - ('A' - 1),            HT_INVALID,           }, // CN__I__
  { sizeof(Index),              HT_TRANSIENT_TINY,    }, // CN_INDEX
  { sizeof(IndexDef),           HT_TRANSIENT_MEDIUM,  }, // CN_INDEXDEF
  { sizeof(IndexTree),          HT_TRANSIENT_TINY,    }, // CN_INDEXTREE
  { sizeof(IndexTreeVec),       HT_TRANSIENT_MEDIUM,  }, // CN_INDEXTREEVEC
  { sizeof(IdxColVec),          HT_TRANSIENT_MEDIUM,  }, // CN_IDXCOLVEC
  { sizeof(IdxVec),             HT_TRANSIENT_MEDIUM,  }, // CN_IDXVEC
  { sizeof(InfoSet),            HT_TRANSIENT_LARGE,   }, // CN_INFOSET
  { sizeof(IngestChunk),        HT_TRANSIENT_LARGE,   }, // CN_INGESTCHUNK
  { sizeof(Input),              HT_TRANSIENT_TINY,    }, // CN_INPUT
  { sizeof(IntBuffer),          HT_TRANSIENT_LARGE,   }, // CN_INTBUFFER
  { sizeof(IntVal),             HT_INVALID,           }, // CN_INTVAL
  { sizeof(IoChunk),            HT_TRANSIENT_LARGE,   }, // CN_IOCHUNK
  { sizeof(IoCopy),             HT_TRANSIENT_TINY,    }, // CN_IOCOPY
  { sizeof(IrwBuffer),          HT_TRANSIENT_LARGE,   }, // CN_IRWBUFFER
  { sizeof(IvlVal),             HT_INVALID,           }, // CN_IVLVAL

  { 'J' - ('A' - 1),            HT_INVALID,           }, // CN__J__
  { sizeof(Job__Find),          HT_TRANSIENT_LARGE,   }, // CN_JOB__FIND
  { sizeof(Job_Boot),           HT_TRANSIENT_MEDIUM,  }, // CN_JOB_BOOT
  { sizeof(Job_Distributed),    HT_TRANSIENT_MEDIUM,  }, // CN_JOB_DISTRIBUTED
  { sizeof(Job_Local),          HT_TRANSIENT_LARGE,   }, // CN_JOB_LOCAL
  { sizeof(Job_NodeInfo),       HT_TRANSIENT_LARGE,   }, // CN_JOB_NODEINFO
  { sizeof(Job_Operand),        HT_TRANSIENT_LARGE,   }, // CN_JOB_OPERAND
  { sizeof(Job_Receiver),       HT_TRANSIENT_MEDIUM,  }, // CN_JOB_RECEIVER
  { sizeof(Job_Start),          HT_TRANSIENT_MEDIUM,  }, // CN_JOB_START
  { sizeof(Job_WriteBuffer),    HT_TRANSIENT_LARGE,   }, // CN_JOB_WRITEBUFFER
  { sizeof(JobList),            HT_TRANSIENT_TINY,    }, // CN_JOBLIST
  { sizeof(Heap), /*correct*/   HT_INVALID,           }, // CN_JUMBOPERSISTENT
  { sizeof(Heap), /*correct*/   HT_INVALID,           }, // CN_JUMBOTRANSIENT

  { 'K' - ('A' - 1),            HT_INVALID,           }, // CN__K__
  { sizeof(Key),                HT_INVALID,           }, // CN_KEY

  { 'L' - ('A' - 1),            HT_INVALID,           }, // CN__L__
  { sizeof(Heap), /*correct*/   HT_INVALID,           }, // CN_LARGEPERSISTENT
  { sizeof(Heap), /*correct*/   HT_INVALID,           }, // CN_LARGETRANSIENT
  { sizeof(Lexicon),            HT_TRANSIENT_TINY,    }, // CN_LEXICON
  { sizeof(LoadChunk),          HT_TRANSIENT_LARGE,   }, // CN_LOADCHUNK
  { sizeof(Loader),             HT_TRANSIENT_LARGE,   }, // CN_LOADER
  { sizeof(LoadMap),            HT_TRANSIENT_JUMBO,   }, // CN_LOADMAP
  { sizeof(Local),              HT_INVALID,           }, // CN_LOCAL
  { sizeof(LocalChain),         HT_TRANSIENT_LARGE,   }, // CN_LOCALSCHAIN
  { sizeof(Lock),               HT_INVALID,           }, // CN_LOCK
  { sizeof(Log),                HT_INVALID,           }, // CN_LOG
  { sizeof(LogEntry),           HT_INVALID,           }, // CN_LOGENTRY

  { 'M' - ('A' - 1),            HT_INVALID,           }, // CN__M__
  { sizeof(Marker),             HT_TRANSIENT_TINY,    }, // CN_MARKER
  { sizeof(Heap), /*correct*/   HT_INVALID,           }, // CN_MEDIUMPERSISTENT
  { sizeof(Heap), /*correct*/   HT_INVALID,           }, // CN_MEDIUMTRANSIENT
  { sizeof(MemTest),            HT_TRANSIENT_TINY,    }, // CN_MEMTEST
  { sizeof(MetaBlock),          HT_TRANSIENT_MEDIUM,  }, // CN_METABLOCK
  { sizeof(MetaCell),           HT_TRANSIENT_TINY,    }, // CN_METACELL
  { sizeof(MetaGraph),          HT_TRANSIENT_MEDIUM,  }, // CN_METAGRAPH
  { sizeof(MetaInherit),        HT_TRANSIENT_MEDIUM,  }, // CN_METAINHERIT
  { sizeof(MetaInherits),       HT_TRANSIENT_TINY,    }, // CN_METAINHERITS
  { sizeof(MetaMember),         HT_TRANSIENT_TINY,    }, // CN_METAMEMBER
  { sizeof(MetaType),           HT_TRANSIENT_MEDIUM,  }, // CN_METATYPE
  { sizeof(Mutex),              HT_TRANSIENT_TINY,    }, // CN_MUTEX

  { 'N' - ('A' - 1),            HT_INVALID,           }, // CN__N__
  { sizeof(NextGid),            HT_INVALID,           }, // CN_NEXTGID
  { sizeof(NmStr),              HT_TRANSIENT_MEDIUM,  }, // CN_NMSTR
  { sizeof(NmStrList),          HT_TRANSIENT_TINY,    }, // CN_NMLIST
  { sizeof(NmVal),              HT_TRANSIENT_TINY,    }, // CN_NMVAL
  { sizeof(Node),               HT_TRANSIENT_TINY,    }, // CN_NODE
  { sizeof(Now),                HT_TRANSIENT_TINY,    }, // CN_NOW
  { sizeof(Numbat),             HT_TRANSIENT_LARGE,   }, // CN_NUMBAT

  { 'O' - ('A' - 1),            HT_INVALID,           }, // CN__O__
  { sizeof(ObjectsInPool),      HT_TRANSIENT_SMALL,   }, // CN_OBJECTSINPOOL
  { sizeof(Op),                 HT_TRANSIENT_MEDIUM,  }, // CN_OP
  { sizeof(OperandQueue),       HT_TRANSIENT_TINY,    }, // CN_OPERANDQUEUE
  { sizeof(OpSpec),             HT_TRANSIENT_MEDIUM,  }, // CN_OPSPEC

  { 'P' - ('A' - 1),            HT_INVALID,           }, // CN__P__
  { sizeof(PathVec),            HT_TRANSIENT_MEDIUM,  }, // CN_PATHVEC
  { sizeof(Peruse),             HT_TRANSIENT_MEDIUM,  }, // CN_PERUSE
  { sizeof(Pipeline),           HT_TRANSIENT_MEDIUM,  }, // CN_PIPELINE
  { sizeof(PipelineSpec),       HT_TRANSIENT_SMALL,   }, // CN_PIPELINESPEC
  { sizeof(Plan),               HT_TRANSIENT_MEDIUM,  }, // CN_PLAN
  { sizeof(Planner),            HT_TRANSIENT_MEDIUM,  }, // CN_PLANNER
  { sizeof(Predicate),          HT_TRANSIENT_TINY,    }, // CN_PREDICATE
  { sizeof(PredicateVec),       HT_TRANSIENT_MEDIUM,  }, // CN_PREDICATEVEC
  { sizeof(Profile),            HT_TRANSIENT_MEDIUM,  }, // CN_PROFILE
  { sizeof(PsxThd),             HT_TRANSIENT_TINY,    }, // CN_PSXTHD
  { sizeof(PtnColVec),          HT_TRANSIENT_MEDIUM,  }, // CN_PTNCOLVEC
  { sizeof(PtrVal),             HT_INVALID,           }, // CN_PTRVAL

  { 'Q' - ('A' - 1),            HT_INVALID,           }, // CN__Q__
  { sizeof(Queue),              HT_TRANSIENT_TINY,    }, // CN_QUEUE
  { sizeof(QueueBuffer),        HT_INVALID,           }, // CN_QUEUEBUFFER
  { sizeof(Queues),             HT_TRANSIENT_TINY,    }, // CN_QUEUES

  { 'R' - ('A' - 1),            HT_INVALID,           }, // CN__R__
  { sizeof(ReadApi),            HT_TRANSIENT_TINY,    }, // CN_READAPI
  { sizeof(Record),             HT_TRANSIENT_MEDIUM,  }, // CN_RECORD
  { sizeof(RecordBuffer),       HT_TRANSIENT_LARGE,   }, // CN_RECORDBUFFER
  { sizeof(RecordData),         HT_TRANSIENT_LARGE,   }, // CN_RECORDDATA
  { sizeof(RegSet),             HT_TRANSIENT_TINY,    }, // CN_REGSET
  { sizeof(RelOp),              HT_TRANSIENT_TINY,    }, // CN_RELOP
  { sizeof(Request),            HT_TRANSIENT_TINY,    }, // CN_REQUEST
  { sizeof(ResourceTracker),    HT_TRANSIENT_TINY,    }, // CN_RESOURCETRACKER
  { sizeof(Response),           HT_TRANSIENT_MEDIUM,  }, // CN_RESPONSE
  { sizeof(ResponseVec),        HT_TRANSIENT_MEDIUM,  }, // CN_RESPONSEVEC
  { sizeof(Result),             HT_TRANSIENT_MEDIUM,  }, // CN_RESULT
  { sizeof(Rand),               HT_INVALID,           }, // CN_RN_RAND
  { sizeof(Routine),            HT_TRANSIENT_MEDIUM,  }, // CN_ROUTINE
  { sizeof(Row),                HT_TRANSIENT_LARGE,   }, // CN_ROW
  { sizeof(Row),/*correct*/     HT_TRANSIENT_MEDIUM,  }, // CN_ROWAGG
  { sizeof(RowCompactor),       HT_TRANSIENT_LARGE,   }, // CN_ROWCOMPACTOR
  { sizeof(RowOnce),            HT_TRANSIENT_TINY,    }, // CN_ROWONCE
  { sizeof(RowSingle),          HT_TRANSIENT_LARGE,   }, // CN_ROWSINGLE
  { sizeof(RowTray),            HT_TRANSIENT_LARGE,   }, // CN_ROWTRAY
  { 0,/*sizeof(RU_Cpu)*/        HT_INVALID,           }, // CN_RUCPU
  { 0,/*sizeof(RU_Dir)*/        HT_INVALID,           }, // CN_RUDIR
  { 0,/*sizeof(RU_File)*/       HT_INVALID,           }, // CN_RUFILE
  { sizeof(RU_Memory),          HT_INVALID,           }, // CN_RUMEMORY
  { 0,/*sizeof(RU_Mutex)*/      HT_INVALID,           }, // CN_RUMUTEX
  { 0,/*sizeof(RU_Pass)*/       HT_INVALID,           }, // CN_RUPASS
  { 0,/*sizeof(RU_Socket)*/     HT_INVALID,           }, // CN_RUSOCKET
  { 0,/*sizeof(RU_Time)*/       HT_INVALID,           }, // CN_RUTIME
  { 0,/*sizeof(RU_Word)*/       HT_INVALID,           }, // CN_RUWORK

  { 'S' - ('A' - 1),            HT_INVALID,           }, // CN__S__
  { sizeof(Schema),             HT_TRANSIENT_TINY,    }, // CN_SCHEMA
  { sizeof(SchemaTest),         HT_TRANSIENT_LARGE,   }, // CN_SCHEMATEST
  { sizeof(SeeMe),              HT_INVALID,           }, // CN_SEEME
  { sizeof(SePrompt),           HT_TRANSIENT_MEDIUM,  }, // CN_SEPROMPT
  { sizeof(Session),            HT_TRANSIENT_MEDIUM,  }, // CN_SESSION
  { sizeof(SessionArray),       HT_TRANSIENT_TINY,    }, // CN_SESSIONARRAY
  { sizeof(ShortCut),           HT_TRANSIENT_MEDIUM,  }, // CN_SHORTCUT
  { sizeof(ShortCutVec),        HT_TRANSIENT_MEDIUM,  }, // CN_SHORTCUTVEC
  { sizeof(Birth),              HT_INVALID,           }, // CN_SINCE
  { sizeof(SkipListNL),         HT_TRANSIENT_TINY,    }, // CN_SKIPLIST
  { sizeof(SkipNode),           HT_TRANSIENT_TINY,    }, // CN_SKIPNODE
  { sizeof(Slab),               HT_TRANSIENT_LARGE,   }, // CN_SLAB
  { sizeof(SlabAllocator),      HT_TRANSIENT_TINY,    }, // CN_SLABALLOCATOR
  { sizeof(Sleep),              HT_INVALID,           }, // CN_SLEEP
  { sizeof(Heap),/*correct*/    HT_INVALID,           }, // SN_SMALLPERSISTENT
  { sizeof(Heap),/*correct*/    HT_INVALID,           }, // SN_SMALLTRANSIENT
  { sizeof(SN_Bcd),             HT_INVALID,           }, // CN_SNBCD
  { sizeof(SN_Bol),             HT_INVALID,           }, // CN_SNBOL
  { sizeof(SN_Dte),             HT_INVALID,           }, // CN_SNDTE
  { sizeof(SN_Flt),             HT_INVALID,           }, // CN_SNFLT
  { sizeof(SN_Int),             HT_INVALID,           }, // CN_SNINT
  { sizeof(SN_Ivl),             HT_INVALID,           }, // CN_SNIVL
  { sizeof(SN_Tme),             HT_INVALID,           }, // CN_SNTME
  { sizeof(SN_Tsp),             HT_INVALID,           }, // CN_SNTSP
  { sizeof(SN_Vch),             HT_INVALID,           }, // CN_SNVCH
  { sizeof(Socket),             HT_TRANSIENT_TINY,    }, // CN_SOCKET
  { sizeof(SP_Bcd),             HT_INVALID,           }, // CN_SPBCD
  { sizeof(SP_Bol),             HT_INVALID,           }, // CN_SPBOL
  { sizeof(SP_Dte),             HT_INVALID,           }, // CN_SPDTE
  { sizeof(SP_Elapsed),         HT_INVALID,           }, // CN_SPELAPSED
  { sizeof(SP_Flt),             HT_INVALID,           }, // CN_SPFLT
  { sizeof(SP_Gid),             HT_INVALID,           }, // CN_SPID
  { sizeof(SP_Int),             HT_INVALID,           }, // CN_SPINT
  { sizeof(SP_Ivl),             HT_INVALID,           }, // CN_SPIVL
  { sizeof(SP_String),          HT_INVALID,           }, // CN_SPSTRING
  { sizeof(SP_TknVecStr),       HT_INVALID,           }, // CN_SPTKNVECSTR
  { sizeof(SP_Tme),             HT_INVALID,           }, // CN_SPTME
  { sizeof(SP_Tsp),             HT_INVALID,           }, // CN_SPTSP
  { sizeof(SP_Vch),             HT_INVALID,           }, // CN_SPVCH
  { sizeof(SrtBuffer),          HT_TRANSIENT_LARGE,   }, // CN_SRTBUFFER
  { sizeof(SrtSet),             HT_INVALID,           }, // CN_SRTSET
  { sizeof(Stack),              HT_TRANSIENT_LARGE,   }, // CN_STACK
  { sizeof(Stage),              HT_TRANSIENT_MEDIUM,  }, // CN_STAGE
  { sizeof(StageSpec),          HT_TRANSIENT_SMALL,   }, // CN_STAGESPEC
  { sizeof(Statistics),         HT_INVALID,           }, // CN_STEPACCUM
  { sizeof(StepAccum),          HT_INVALID,           }, // CN_STATISTICS
  { sizeof(StepTiming),         HT_TRANSIENT_LARGE,   }, // CN_STEPTIMING
  { sizeof(Stmt_Select),        HT_TRANSIENT_LARGE,   }, // CN_STMT_SELECT
  { sizeof(Strand),             HT_TRANSIENT_MEDIUM,  }, // CN_STRAND
  { sizeof(StreamDef),          HT_TRANSIENT_LARGE,   }, // CN_STREAMDEF
  { sizeof(StreamSet),          HT_TRANSIENT_LARGE,   }, // CN_STREAMSET
  { sizeof(s32),                HT_INVALID,           }, // CN_STRING_T
  { sizeof(StzVal),             HT_INVALID,           }, // CN_STZVAL
  { sizeof(SysCall),            HT_INVALID,           }, // CN_SYSCALL

  { 'T' - ('A' - 1),            HT_INVALID,           }, // CN__T__
  { sizeof(Table),              HT_TRANSIENT_MEDIUM,  }, // CN_TABLE
  { sizeof(TableDef),           HT_TRANSIENT_MEDIUM,  }, // CN_TABLEDEF
  { sizeof(TablePtn),           HT_TRANSIENT_TINY,    }, // CN_TABLEPTN
  { sizeof(TablePtnVec),        HT_TRANSIENT_SMALL,   }, // CN_TABLEPTNVEC
  { sizeof(TagSet),             HT_TRANSIENT_MEDIUM,  }, // CN_TAGSET
  { sizeof(Task),               HT_TRANSIENT_TINY,    }, // CN_TASK
  { sizeof(TaskList),           HT_TRANSIENT_TINY,    }, // CN_TASKLIST
  { sizeof(TaskQueue),          HT_TRANSIENT_TINY,    }, // CN_TEMPNAME
  { sizeof(TempName),           HT_TRANSIENT_TINY,    }, // CN_TENANT
  { sizeof(Tenant),             HT_TRANSIENT_TINY,    }, // CN_TENANTS
  { sizeof(Tenants),            HT_TRANSIENT_TINY,    }, // CN_TASKQUEUE
  { sizeof(Thread),             HT_TRANSIENT_TINY,    }, // CN_THREAD
  { sizeof(ThreadQueue),        HT_TRANSIENT_TINY,    }, // CN_THREADQUEUE
  { sizeof(ThreadVec),          HT_TRANSIENT_SMALL,   }, // CN_THREADVEC
  { sizeof(Timer),              HT_TRANSIENT_TINY,    }, // CN_TIMER
  { sizeof(Heap),/*correct*/    HT_INVALID,           }, // CN_TINYPERSISTENT
  { sizeof(Heap),/*correct*/    HT_INVALID,           }, // CN_TINYTRANSIENT
  { sizeof(TmeVal),             HT_INVALID,           }, // CN_TMEVAL
  { sizeof(Token),              HT_TRANSIENT_TINY,    }, // CN_TOKEN
  { sizeof(TokenVec),           HT_TRANSIENT_LARGE,   }, // CN_TOKENVEC
  { sizeof(TspVal),             HT_INVALID,           }, // CN_TSPVAL
  { sizeof(Tx),                 HT_TRANSIENT_TINY,    }, // CN_TX
  { sizeof(TxSet),              HT_TRANSIENT_TINY,    }, // CN_TXSET

  { 'U' - ('A' - 1),            HT_INVALID,           }, // CN__U__

  { 'V' - ('A' - 1),            HT_INVALID,           }, // CN__V__
  { sizeof(Vortex),             HT_TRANSIENT_LARGE,   }, // CN_VORTEX

  { 'W' - ('A' - 1),            HT_INVALID,           }, // CN__W__
  { sizeof(Wilderness),         HT_INVALID,           }, // CN_WILDERNESS
  { sizeof(Word),               HT_TRANSIENT_TINY,    }, // CN_WORD
  { sizeof(WriteBehind),        HT_TRANSIENT_LARGE,   }, // CN_WRITEBEHIND

  { 'X' - ('A' - 1),            HT_INVALID,           }, // CN__X__
  { sizeof(Xmits),              HT_TRANSIENT_TINY,    }, // CN_XMITS

  { 'Y' - ('A' - 1),            HT_INVALID,           }, // CN__Y__

  { 'Z' - ('A' - 1),            HT_INVALID,           }, // CN__Z__

  { -1,                         HT_INVALID,           }, // CN_ENUMS
  
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
