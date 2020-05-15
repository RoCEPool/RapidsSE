/*============================================================================*/
/* SizesDumps.cpp -- Class Sizes ands Dumps                                   */
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
  /* */ sizeof(TagSet),           // CN_TAGSET
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
  /* */ HT_TRANSIENT_TINY,   // CN_AGGTIMER
  /* */ HT_TRANSIENT_TINY,   // CN_ARG
  /* */ HT_INVALID,           // CN_ARGUMENTS
  /* */ HT_TRANSIENT_TINY,   // CN_ARTICLE
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
  /* */ HT_TRANSIENT_TINY,   // CN_ENUMFLAGS
  /* */ HT_TRANSIENT_TINY,   // CN_EXEMPLAR
  /* */ HT_TRANSIENT_TINY,   // CN_EXTRACTROW

  /* */ HT_INVALID,           // CN__F__
  /* */ HT_TRANSIENT_LARGE,   // CN_FETCHBUFFER
  /* */ HT_TRANSIENT_TINY,   // CN_FIFO
  /* */ HT_TRANSIENT_TINY,   // CN_FILE
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
  /* */ HT_TRANSIENT_TINY,   // CN_FORTH
  /* */ HT_TRANSIENT_TINY,   // CN_FORMAT
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
  /* */ HT_TRANSIENT_TINY,   // CN_LEXICON
  /* */ HT_TRANSIENT_LARGE,   // CN_LOADCHUNK
  /* */ HT_TRANSIENT_LARGE,   // CN_LOADER
  /* */ HT_TRANSIENT_JUMBO,   // CN_LOADMAP
  /* */ HT_INVALID,           // CN_LOCAL
  /* */ HT_TRANSIENT_LARGE,   // CN_LOCALSCHAIN
  /* */ HT_INVALID,           // CN_LOCK
  /* */ HT_INVALID,           // CN_LOG
  /* */ HT_INVALID,           // CN_LOGENTRY

  /* */ HT_INVALID,           // CN__M__
  /* */ HT_TRANSIENT_TINY,   // CN_MARKER
  /* */ HT_INVALID,           // CN_MEDIUMPERSISTENT
  /* */ HT_INVALID,           // CN_MEDIUMTRANSIENT
  /* */ HT_TRANSIENT_TINY,   // CN_MEMTEST
  /* */ HT_TRANSIENT_MEDIUM,  // CN_METABLOCK
  /* */ HT_TRANSIENT_TINY,   // CN_METACELL
  /* */ HT_TRANSIENT_MEDIUM,  // CN_METAGRAPH
  /* */ HT_TRANSIENT_MEDIUM,  // CN_METAINHERIT
  /* */ HT_TRANSIENT_TINY,   // CN_METAINHERITS
  /* */ HT_TRANSIENT_TINY,   // CN_METAMEMBER
  /* */ HT_TRANSIENT_MEDIUM,  // CN_METATYPE
  /* */ HT_TRANSIENT_TINY,   // CN_MUTEX

  /* */ HT_INVALID,           // CN__N__
  /* */ HT_INVALID,           // CN_NEXTGID
  /* */ HT_TRANSIENT_MEDIUM,  // CN_NMSTR
  /* */ HT_TRANSIENT_TINY,   // CN_NMLIST
  /* */ HT_TRANSIENT_TINY,   // CN_NMVAL
  /* */ HT_TRANSIENT_TINY,   // CN_NODE
  /* */ HT_TRANSIENT_TINY,   // CN_NOW
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
  /* */ HT_TRANSIENT_TINY,   // CN_PREDICATE
  /* */ HT_TRANSIENT_MEDIUM,  // CN_PREDICATEVEC
  /* */ HT_TRANSIENT_MEDIUM,  // CN_PROFILE
  /* */ HT_TRANSIENT_TINY,   // CN_PSXTHD
  /* */ HT_TRANSIENT_MEDIUM,  // CN_PTNCOLVEC
  /* */ HT_INVALID,           // CN_PTRVAL

  /* */ HT_INVALID,           // CN__Q__
  /* */ HT_TRANSIENT_TINY,   // CN_QUEUE
  /* */ HT_INVALID,           // CN_QUEUEBUFFER
  /* */ HT_TRANSIENT_TINY,   // CN_QUEUES
  
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
  /* */ HT_TRANSIENT_TINY,   // CN_ROWONCE
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
  /* */ HT_TRANSIENT_TINY,   // CN_SCHEMA
  /* */ HT_TRANSIENT_LARGE,   // CN_SCHEMATEST
  /* */ HT_INVALID,           // CN_SEEME
  /* */ HT_TRANSIENT_MEDIUM,   // CN_SEPROMPT
  /* */ HT_TRANSIENT_MEDIUM,   // CN_SESSION
  /* */ HT_TRANSIENT_TINY,     // CN_SESSIONARRAY
  /* */ HT_TRANSIENT_MEDIUM,  // CN_SHORTCUT
  /* */ HT_TRANSIENT_MEDIUM,  // CN_SHORTCUTVEC
  /* */ HT_INVALID,           // CN_SINCE
  /* */ HT_TRANSIENT_TINY,   // CN_SKIPLIST
  /* */ HT_TRANSIENT_TINY,   // CN_SKIPNODE
  /* */ HT_TRANSIENT_LARGE,   // CN_SLAB
  /* */ HT_TRANSIENT_TINY,   // CN_SLABALLOCATOR
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
  /* */ HT_TRANSIENT_TINY,   // CN_SOCKET
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
  /* */ HT_TRANSIENT_TINY,   // CN_TASKLIST
  /* */ HT_TRANSIENT_TINY,   // CN_TEMPNAME
  /* */ HT_TRANSIENT_TINY,   // CN_TENANT
  /* */ HT_TRANSIENT_TINY,   // CN_TENANTS
  /* */ HT_TRANSIENT_TINY,   // CN_TASKQUEUE
  /* */ HT_TRANSIENT_TINY,   // CN_THREAD
  /* */ HT_TRANSIENT_TINY,   // CN_THREADQUEUE
  /* */ HT_TRANSIENT_SMALL,  // CN_THREADVEC
  /* */ HT_TRANSIENT_TINY,   // CN_TIMER
  /* */ HT_INVALID,           // CN_TINYPERSISTENT
  /* */ HT_INVALID,           // CN_TINYTRANSIENT
  /* */ HT_INVALID,           // CN_TMEVAL
  /* */ HT_TRANSIENT_TINY,   // CN_TOKEN
  /* */ HT_TRANSIENT_LARGE,   // CN_TOKENVEC
  /* */ HT_INVALID,           // CN_TSPVAL
  /* */ HT_TRANSIENT_TINY,   // CN_TX
  /* */ HT_TRANSIENT_TINY,   // CN_TXSET
  
  /* */ HT_INVALID,           // CN__U__
  
  /* */ HT_INVALID,           // CN__V__
  /* */ HT_TRANSIENT_LARGE,   // CN_VORTEX

  /* */ HT_INVALID,           // CN__W__
  /* */ HT_INVALID,           // CN_WILDERNESS
  /* */ HT_TRANSIENT_TINY,   // CN_WORD
  
  /* */ HT_TRANSIENT_LARGE,   // CN_WRITEBEHIND

  /* */ HT_INVALID,           // CN__X__
  /* */ HT_TRANSIENT_TINY,   // CN_XMITS
  
  /* */ HT_INVALID,           // CN__Y__
  
  /* */ HT_INVALID,           // CN__Z__
  
  /* */ HT_INVALID,           // CN_ENUMS
};


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
