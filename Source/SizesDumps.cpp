/*============================================================================*/
/* SizesDumps.cpp -- Class Sizes ands Dumps                                   */
/*============================================================================*/

#include "Oblique.h"

// Object's : Class::vDump
// Object's with no vDump defined : Dump::vUndefined
// Header   : Dump::vInvalid
// Objects we don't want to dump (eg Word) : Dump::vDummy

/*----------------------------------------------------------------------------*/
/* dump functions                                                             */
/*----------------------------------------------------------------------------*/
pDUMPFUN *gpDump[]
{
  /* */ &Dump::vInvalid,         // CN_NONE
  
  /* */ &Dump::vInvalid,         // CN__A__
  /* */ &Dump::vInvalid,         // CN_ACCESSPATH
  /* */ &Dump::vInvalid,         // CN_AGGTIMER
  /* */ &Dump::vInvalid,         // CN_ARG
  /* */ &Dump::vInvalid,         // CN_ARGUMENTS
  /* */ &Dump::vUndefined,       // CN_ARTICLE
  /* */ &Dump::vInvalid,         // CN_ATOMIC
  /* */ &AttachMap::vDump,       // CN_ATTACHMAP

  /* */ &Dump::vInvalid,         // CN__B__
  /* */ &Dump::vInvalid,         // CN_BCDVAL
  /* */ &Dump::vInvalid,         // CN_BIRTH
  /* */ &Dump::vInvalid,         // CN_BOLVAL
  /* */ &Dump::vInvalid,         // CN_BPA
  /* */ &Dump::vInvalid,         // CN_BUCKETMAP
  /* */ &Dump::vInvalid,         // CN_BUFFERCURSOR
  /* */ &Dump::vInvalid,         // CN_BUFFERPOOL
  /* */ &Dump::vInvalid,         // CN_BYTERATE
  /* */ &Dump::vInvalid,         // CN_BYTES_T

  /* */ &Dump::vInvalid,         // CN__C__
  /* */ &Dump::vUndefined,       // CN_CALCULATOR
  /* */ &Dump::vDummy,           // CN_CARRYOVER
  /* */ &Dump::vDummy,           // CN_CATALOG
  /* */ &Dump::vDummy,           // CN_CATALOG
  /* */ &Charser::vDump,          // CN_CHARSER
  /* */ &Dump::vDummy,           // CN_CLAUSE_FROM
  /* */ &Dump::vDummy,           // CN_CLAUSE_GROUPBY
  /* */ &Dump::vDummy,           // CN_CLAUSE_JOIN
  /* */ &Dump::vDummy,           // CN_CLAUSE_LIMIT
  /* */ &Dump::vDummy,           // CN_CLAUSE_ORDERBY
  /* */ &Dump::vDummy,           // CN_CLAUSE_SELECT
  /* */ &Dump::vDummy,           // CN_CLAUSE_SUBQRY
  /* */ &Dump::vDummy,           // CN_CLAUSE_WHERE
  /* */ &Dump::vDummy,           // CN_CMD_SELECT
  /* */ &Dump::vDummy,           // CN_COMMAND
  /* */ &Dump::vDummy,           // CN_COMPILER
  /* */ &Dump::vDummy,           // CN_CONSTDEF
  /* */ &Dump::vDummy,           // CN_CONSTSET
  /* */ &Dump::vUndefined,       // CN_CRVBUFFER
  /* */ &Dump::vUndefined,       // CN_CRWBUFFER
  /* */ &Dump::vUndefined,       // CN_CSVBUFFER

  /* */ &Dump::vInvalid,         // CN__D__
  /* */ &Dump::vDummy,           // CN_DATUM
  /* */ &Dump::vDummy,           // CN_DECACNT
  /* */ &Dump::vDummy,           // CN_DECIDER
  /* */ &Dic::vDump,             // CN_DIC
  /* */ &DicStack::vDump,        // CN_DICSTACK
  /* */ &Dump::vInvalid,         // CN_DIR
  /* */ &Dump::vInvalid,         // CN_DTEVAL
  /* */ &Dump::vDummy,           // CN_DYNAMO

  /* */ &Dump::vInvalid,         // CN__E__
  /* */ &Dump::vInvalid,         // CN_EDTVAL
  /* */ &Dump::vUndefined,       // CN_ENCODING
  /* */ &Dump::vDummy,           // CN_ENT
  /* */ &Dump::vDummy,           // CN_ENTBUFFER
  /* */ &Dump::vInvalid,         // CN_ENTWRK
  /* */ &Dump::vDummy,           // CN_ENUMFLAGS
  /* */ &Dump::vInvalid,         // CN_EXEMPLAR
  /* */ &Dump::vDummy,           // CN_EXTRACTROW

  /* */ &Dump::vInvalid,         // CN__F__
  /* */ &Dump::vInvalid,         // CN_FETCHBUFFER
  /* */ &Dump::vInvalid,         // CN_FIFO
  /* */ &Dump::vInvalid,         // CN_FILE
  /* */ &Dump::vDummy,           // CN_FILTER_FETCH
  /* */ &Dump::vDummy,           // CN_FILTER_FROM
  /* */ &Dump::vDummy,           // CN_FILTER_GROUPBY
  /* */ &Dump::vDummy,           // CN_FILTER_INFLATE
  /* */ &Dump::vDummy,           // CN_FILTER_JOIN
  /* */ &Dump::vDummy,           // CN_FILTER_LIMIT
  /* */ &Dump::vDummy,           // CN_FILTER_ORDERBY
  /* */ &Dump::vDummy,           // CN_FILTER_OUTPUT
  /* */ &Dump::vDummy,           // CN_FILTER_PROJECT
  /* */ &Dump::vDummy,           // CN_FILTER_SCAN
  /* */ &Dump::vDummy,           // CN_FILTER_SELECT
  /* */ &Dump::vDummy,           // CN_FILTER_SUBQRY
  /* */ &Dump::vDummy,           // CN_FILTER_WHERE
  /* */ &Dump::vDummy,           // CN_FILTER_ZED
  /* */ &Dump::vInvalid,         // CN_FIPVAL
  /* */ &Dump::vInvalid,         // CN_FLTVAL
  /* */ &Dump::vDummy,           // CN_FOCALOBJECTS
  /* */ &Dump::vDummy,           // CN_FORTH
  /* */ &Dump::vDummy,           // CN_FORMAT
  /* */ &Dump::vDummy,           // CN_FREEBUFFER
  /* */ &Dump::vDummy,           // CN_FREECHUNK
  /* */ &Dump::vDummy,           // CN_FREEDNOT
  /* */ &Dump::vDummy,           // CN_FREEITEM
  /* */ &Dump::vInvalid,         // CN_FREQDIST

  /* */ &Dump::vInvalid,         // CN__G__
  /* */ &Dump::vDummy,           // CN_GGADAPTOR
  /* */ &Grid::vDump,            // CN_GRID
  
  /* */ &Dump::vInvalid,         // CN__H__
  /* */ &Dump::vDummy,           // CN_HASHBUFFER
  /* */ &Dump::vDummy,           // CN_HASHMAP
  /* */ &HeapBuffer::vDump,      // CN_HEAPBUFFER
  /* */ &Dump::vDummy,           // CN_HPROW

  /* */ &Dump::vInvalid,         // CN__I__
  /* */ &Dump::vDummy,           // CN_INDEX
  /* */ &Dump::vDummy,           // CN_INDEXDEF
  /* */ &Dump::vDummy,           // CN_INDEXTREE
  /* */ &Dump::vDummy,           // CN_INDEXTREEVEC
  /* */ &Dump::vDummy,           // CN_IDXCOLVEC
  /* */ &Dump::vDummy,           // CN_IDXVEC
  /* */ &Dump::vDummy,           // CN_INFOSET
  /* */ &Dump::vDummy,           // CN_INGESTCHUNK
  /* */ &Input::vDump,           // CN_INPUT
  /* */ &Dump::vDummy,           // CN_INTBUFFER
  /* */ &Dump::vInvalid,         // CN_INTVAL
  /* */ &Dump::vDummy,           // CN_IOCHUNK
  /* */ &Dump::vDummy,           // CN_IOCOPY
  /* */ &Dump::vDummy,           // CN_IRWBUFFER
  /* */ &Dump::vInvalid,         // CN_IVLVAL

  /* */ &Dump::vInvalid,         // CN__J__
  /* */ &Dump::vDummy,           // CN_JOB__FIND
  /* */ &Dump::vDummy,           // CN_JOB_BOOT
  /* */ &Dump::vDummy,           // CN_JOB_DISTRIBUTED
  /* */ &Dump::vDummy,           // CN_JOB_LOCAL
  /* */ &Dump::vDummy,           // CN_JOB_NODEINFO
  /* */ &Dump::vDummy,           // CN_JOB_OPERAND
  /* */ &Dump::vDummy,           // CN_JOB_RECEIVER
  /* */ &Dump::vDummy,           // CN_JOB_START
  /* */ &Dump::vDummy,           // CN_JOB_WRITEBUFFER
  /* */ &JobList::vDump,         // CN_JOBLIST
  /* */ &Heap::vDump,            // CN_JUMBOPERSISTENT
  /* */ &Heap::vDump,            // CN_JUMBOTRANSIENT
  
  /* */ &Dump::vInvalid,         // CN__K__
  /* */ &Dump::vDummy,           // CN_KEY
  
  /* */ &Dump::vInvalid,         // CN__L__
  /* */ &Heap::vDump,            // CN_LARGEPERSISTENT
  /* */ &Heap::vDump,            // CN_LARGETRANSIENT
  /* */ &Dump::vDummy,           // CN_LEXICON
  /* */ &Dump::vDummy,           // CN_LOADCHUNK
  /* */ &Dump::vInvalid,         // CN_LOADER
  /* */ &LoadMap::vDump,         // CN_LOADMAP
  /* */ &Dump::vInvalid,         // CN_LOCAL
  /* */ &LocalChain::vDump,      // CN_LOCALSCHAIN
  /* */ &Dump::vInvalid,         // CN_LOCK
  /* */ &Dump::vInvalid,         // CN_LOG
  /* */ &Dump::vInvalid,         // CN_LOGENTRY

  /* */ &Dump::vInvalid,         // CN__M__
  /* */ &Heap::vDump,            // CN_MARKER
  /* */ &Heap::vDump,            // CN_MEDIUMPERSISTENT
  /* */ &Heap::vDump,            // CN_MEDIUMTRANSIENT
  /* */ &Dump::vUndefined,       // CN_MEMTEST
  /* */ &Dump::vUndefined,       // CN_METABLOCK
  /* */ &Dump::vUndefined,       // CN_METACELL
  /* */ &Dump::vUndefined,       // CN_METAGRAPH
  /* */ &Dump::vUndefined,       // CN_METAINHERIT
  /* */ &Dump::vUndefined,       // CN_METAINHERITS
  /* */ &Dump::vUndefined,       // CN_METAMEMBER
  /* */ &Dump::vUndefined,       // CN_METATYPE
  /* */ &Dump::vUndefined,       // CN_MUTEX

  /* */ &Dump::vInvalid,         // CN__N__
  /* */ &Dump::vUndefined,       // CN_NEXTGID
  /* */ &Dump::vUndefined,       // CN_NMSTR
  /* */ &Dump::vUndefined,       // CN_NMSTRLIST
  /* */ &Dump::vUndefined,       // CN_NMVAL
  /* */ &Node::vDump,            // CN_NODE
  /* */ &Dump::vUndefined,       // CN_NOW
  /* */ &Dump::vUndefined,       // CN_NUMBAT

  /* */ &Dump::vInvalid,         // CN__O__
  /* */ &Dump::vInvalid,         // CN_OBJECTSINPOOL
  /* */ &Dump::vInvalid,         // CN_OP
  /* */ &Dump::vInvalid,         // CN_OPERANDQUEUE
  /* */ &Dump::vInvalid,         // CN_OPSPEC

  /* */ &Dump::vInvalid,         // CN__P__
  /* */ &Dump::vInvalid,         // CN_PATHVEC
  /* */ &Dump::vInvalid,         // CN_PERUSE
  /* */ &Dump::vDummy,           // CN_PIPELINE
  /* */ &Dump::vDummy,           // CN_PIPELINESPEC
  /* */ &Plan::vDump,            // CN_PLAN
  /* */ &Dump::vUndefined,       // CN_PLANNER
  /* */ &Dump::vUndefined,       // CN_PREDICATE
  /* */ &Dump::vUndefined,       // CN_PREDICATEVEC
  /* */ &Dump::vUndefined,       // CN_PROFILE
  /* */ &Dump::vUndefined,       // CN_PSXTHD
  /* */ &Dump::vDummy,           // CN_PTNCOLVEC
  /* */ &Dump::vInvalid,         // CN_PTRVAL

  /* */ &Dump::vInvalid,         // CN__Q__
  /* */ &Queue::vDump,           // CN_QUEUE
  /* */ &QueueBuffer::vDump,     // CN_QUEUEBUFFER
  /* */ &Dump::vUndefined,       // CN_QUEUES

  /* */ &Dump::vInvalid,         // CN__R__
  /* */ &Dump::vUndefined,       // CN_READAPI
  /* */ &Dump::vUndefined,       // CN_RECORD
  /* */ &Dump::vUndefined,       // CN_RECORDBUFFER
  /* */ &Dump::vUndefined,       // CN_RECORDDATA
  /* */ &Dump::vUndefined,       // CN_REGSET
  /* */ &Dump::vUndefined,       // CN_RELOP
  /* */ &Dump::vUndefined,       // CN_REQUEST
  /* */ &LocalChain::vDump,      // CN_RESOURCETRACKER
  /* */ &Dump::vUndefined,       // CN_RESPONSE
  /* */ &Dump::vUndefined,       // CN_RESPONSEVEC
  /* */ &Dump::vUndefined,       // CN_RESULT
  /* */ &Dump::vUndefined,       // CN_RN_RAND
  /* */ &Dump::vUndefined,       // CN_ROUTINE
  /* */ &Dump::vUndefined,       // CN_ROW
  /* */ &Dump::vUndefined,       // CN_ROWAGG
  /* */ &Dump::vUndefined,       // CN_ROWCOMPACTOR
  /* */ &Dump::vUndefined,       // CN_ROWONCE
  /* */ &Dump::vUndefined,       // CN_ROWSINGLE
  /* */ &Dump::vUndefined,       // CN_ROWTRAY
  /* */ &Dump::vUndefined,       // CN_RUCPU
  /* */ &Dump::vUndefined,       // CN_RUDIR
  /* */ &Dump::vUndefined,       // CN_RUFILE
  /* */ &Dump::vUndefined,       // CN_RUMEMORY
  /* */ &Dump::vUndefined,       // CN_RUMUTEX
  /* */ &Dump::vUndefined,       // CN_RUPASS
  /* */ &Dump::vUndefined,       // CN_RUSOCKET
  /* */ &Dump::vUndefined,       // CN_RUTIME
  /* */ &Dump::vUndefined,       // CN_RUWORK

  /* */ &Dump::vInvalid,         // CN__S__
  /* */ &Dump::vUndefined,       // CN_SCHEMA
  /* */ &Dump::vUndefined,       // CN_SCHEMATEST
  /* */ &Dump::vUndefined,       // CN_SEEME
  /* */ &SePrompt::vDump,        // CN_SEPROMPT
  /* */ &Session::vDump,         // CN_SESSION
  /* */ &Dump::vUndefined,       // CN_SESSIONARRAY
  /* */ &Dump::vUndefined,       // CN_SHORTCUT
  /* */ &Dump::vUndefined,       // CN_SHORTCUTVEC
  /* */ &Dump::vUndefined,       // CN_SINCE
  /* */ &Dump::vUndefined,       // CN_SKIPLIST
  /* */ &Dump::vUndefined,       // CN_SKIPNODE
  /* */ &Dump::vUndefined,       // CN_SLAB
  /* */ &Dump::vUndefined,       // CN_SLABALLOCATOR
  /* */ &Dump::vUndefined,       // CN_SLEEP
  /* */ &Heap::vDump,            // CN_SMALLPERSISTENT
  /* */ &Heap::vDump,            // CN_SMALLTRANSIENT
  /* */ &Dump::vInvalid,         // CN_SNBCD
  /* */ &Dump::vInvalid,         // CN_SNBOL
  /* */ &Dump::vInvalid,         // CN_SNDTE
  /* */ &Dump::vInvalid,         // CN_SNFLT
  /* */ &Dump::vInvalid,         // CN_SNINT
  /* */ &Dump::vInvalid,         // CN_SNIVL
  /* */ &Dump::vInvalid,         // CN_SNTME
  /* */ &Dump::vInvalid,         // CN_SNTSP
  /* */ &Dump::vInvalid,         // CN_SNVCH
  /* */ &Socket::vDump,          // CN_SOCKET
  /* */ &Dump::vInvalid,         // CN_SPBCD
  /* */ &Dump::vInvalid,         // CN_SPBOL
  /* */ &Dump::vInvalid,         // CN_SPDTE
  /* */ &Dump::vInvalid,         // CN_SPELAPSED
  /* */ &Dump::vInvalid,         // CN_SPFLT
  /* */ &Dump::vInvalid,         // CN_SPID
  /* */ &Dump::vInvalid,         // CN_SPINT
  /* */ &Dump::vInvalid,         // CN_SPIVL
  /* */ &Dump::vInvalid,         // CN_SPSTRING
  /* */ &Dump::vInvalid,         // CN_SPTKNVECSTR
  /* */ &Dump::vInvalid,         // CN_SPTME
  /* */ &Dump::vInvalid,         // CN_SPTSP
  /* */ &Dump::vInvalid,         // CN_SPVCH
  /* */ &Dump::vInvalid,         // CN_SRTBUFFER
  /* */ &Dump::vInvalid,         // CN_SRTSET
  /* */ &Dump::vInvalid,         // CN_STACK
  /* */ &Dump::vInvalid,         // CN_STAGE
  /* */ &Dump::vInvalid,         // CN_STAGESPEC
  /* */ &Dump::vInvalid,         // CN_STATISTICS
  /* */ &Dump::vInvalid,         // CN_STEPACCUM
  /* */ &StepTiming::vDump,      // CN_STEPTIMING
  /* */ &Dump::vInvalid,         // CN_STMT_SELECT
  /* */ &Dump::vInvalid,         // CN_STRAND
  /* */ &Dump::vInvalid,         // CN_STREAMDEF
  /* */ &Dump::vInvalid,         // CN_STREAMSET
  /* */ &Dump::vInvalid,         // CN_STRING_T
  /* */ &Dump::vInvalid,         // CN_STZVAL
  /* */ &Dump::vInvalid,         // CN_SYSCALL

  /* */ &Dump::vInvalid,         // CN__T__
  /* */ &Table::vDump,           // CN_TABLE
  /* */ &Dump::vDummy,           // CN_TABLEDEF
  /* */ &TablePtn::vDump,        // CN_TABLEPTN
  /* */ &Dump::vDummy,           // CN_TABLEPTNVEC
  /* */ &Dump::vDummy,           // CN_TAGSET
  /* */ &Task::vDump,            // CN_TASK
  /* */ &Dump::vUndefined,       // CN_TASKLIST
  /* */ &Dump::vUndefined,       // CN_TASKQUEUE
  /* */ &Dump::vUndefined,       // CN_TEMPNAME
  /* */ &Dump::vUndefined,       // CN_TENANT
  /* */ &Dump::vUndefined,       // CN_TENANTS
  /* */ &Dump::vUndefined,       // CN_THREAD
  /* */ &Dump::vUndefined,       // CN_THREADQUEUE
  /* */ &Dump::vUndefined,       // CN_THREADVEC
  /* */ &Dump::vInvalid,         // CN_TIMER
  /* */ &Heap::vDump,            // CN_TINYPERSISTENT
  /* */ &Heap::vDump,            // CN_TINYTRANSIENT
  /* */ &Dump::vInvalid,         // CN_TMEVAL
  /* */ &Token::vDump,           // CN_TOKEN
  /* */ &Dump::vInvalid,         // CN_TOKENVEC
  /* */ &Dump::vInvalid,         // CN_TSPVAL
  /* */ &Tx::vDump,              // CN_TX
  /* */ &Dump::vUndefined,       // CN_TXSET

  /* */ &Dump::vInvalid,         // CN__U__
  
  /* */ &Dump::vInvalid,         // CN__V__
  /* */ &Dump::vInvalid,         // CN_VORTEX
  
  /* */ &Dump::vInvalid,         // CN__W__
  /* */ &Dump::vInvalid,         // CN_WILDERNESS
  /* */ &Word::vDump,            // CN_WORD
  /* */ &Dump::vInvalid,         // CN_WRITEBEHIND

  /* */ &Dump::vInvalid,         // CN__X__
  /* */ &Dump::vUndefined,       // CN_XMITS

  /* */ &Dump::vInvalid,         // CN__Y__
  
  /* */ &Dump::vInvalid,         // CN__Z__
  
  /* */ &Dump::vEnums            // CN_ENUMS
};

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

/*----------------------------------------------------------------------------*/
/* dummy dump                                                                 */
/*----------------------------------------------------------------------------*/
void Dump::vDummy(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass != CN_NONE);
  ASSERT(pObject != nullptr);
  ASSERT(aTab != nullptr);
  // do nothing
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* dump enums                                                                 */
/*----------------------------------------------------------------------------*/
void Dump::vEnums(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass == CN_ENUMS);
  ASSERT(pObject != nullptr);
  ASSERT(aTab != nullptr);
  // do nothing
}

/*----------------------------------------------------------------------------*/
/* invalid dump                                                               */
/*----------------------------------------------------------------------------*/
void Dump::vInvalid(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass != CN_ENUMS);
  ASSERT(pObject != nullptr);
  ASSERT(aTab != nullptr);
  vTrace("%sInvalid Class %s in Buffer Pool\n"_Stz, aTab, aCN[eClass]);
}

/*----------------------------------------------------------------------------*/
/* dump not defined                                                           */
/*----------------------------------------------------------------------------*/
void Dump::vUndefined(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass != CN_NONE);
  ASSERT(pObject != nullptr);
  ASSERT(aTab != nullptr);
  vTrace("%s%s: **dump function not defined for class**\n"_Stz, aTab, aCN[eClass]);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
