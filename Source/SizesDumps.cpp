/*============================================================================*/
/* Dump.cpp -- Class Dumps                                                    */
/*============================================================================*/

#include "StorageEngine.h"

// Object's : Class::vDump
// Object's with no vDump defined : Dump::vUndefined
// Header   : Dump::vInvalid
// Objects we don't want to dump (eg ForthWord) : Dump::vDummy

/*----------------------------------------------------------------------------*/
/* dump functions                                                             */
/*----------------------------------------------------------------------------*/
pDUMPFUN *gpDump[]
{
  /*  0 */ &Dump::vInvalid,         // CN_NONE
  
  /*  1 */ &Dump::vInvalid,         // CN_BCDVAL
  /*  3 */ &Dump::vInvalid,         // CN_BOLVAL
  /*  4 */ &Dump::vInvalid,         // CN_DTEVAL
  /*    */ &Dump::vInvalid,         // CN_FIPVAL
  /*  6 */ &Dump::vInvalid,         // CN_FLTVAL
  /*  7 */ &Dump::vInvalid,         // CN_INTVAL
  /*  9 */ &Dump::vInvalid,         // CN_IVLVAL
  /* 11 */ &Dump::vInvalid,         // CN_PTRVAL
  /* 12 */ &Dump::vInvalid,         // CN_STRVAL
  /* 14 */ &Dump::vInvalid,         // CN_TMEVAL
  /* 15 */ &Dump::vInvalid,         // CN_TSPVAL
  
  /* 19 */ &Dump::vInvalid,         // CN_ACCESSPATH
  /*    */ &Dump::vDummy,           // CN_ACTION
  /*    */ &Dump::vDummy,           // CN_ACTIONSEQUENCE
  /*    */ &Dump::vDummy,           // CN_ACTIVITYMAP
  /*    */ &Dump::vDummy,           // CN_ACTIVITYMARK
  /* 21 */ &Dump::vInvalid,         // CN_APISTATE
  /* 22 */ &Dump::vInvalid,         // CN_ARG
  /* 23 */ &Dump::vInvalid,         // CN_ARGUMENTS
  /* 24 */ &Dump::vInvalid,         // CN_ATOMIC
  /* 25 */ &AttachMap::vDump,       // CN_ATTACHMAP

  /* 29 */ &Dump::vInvalid,         // CN_BPA
  /* 28 */ &Dump::vInvalid,         // CN_BUFFERPOOL
  /*    */ &Dump::vInvalid,         // CN_BYTERATE

  /* 30 */ &Dump::vDummy,           // CN_CARRYOVER
  /*    */ &Dump::vDummy,           // CN_CATALOG
  /* 31 */ &Dump::vDummy,           // CN_COLUMN
  /* 32 */ &Dump::vDummy,           // CN_COLUMNVEC
  /*    */ &Dump::vDummy,           // CN_COMMAND
  /*    */ &Dump::vDummy,           // CN_COMPILER
  /*    */ &Dump::vDummy,           // CN_COMPLETER

  /*    */ &Dump::vDummy,           // CN_DATUM
  /*    */ &Dump::vDummy,           // CN_DECACNT
  /*    */ &Dump::vDummy,           // CN_DECIDER
  /* 33 */ &Dic::vDump,             // CN_DIC
  /*    */ &Dump::vDummy,           // CN_DICPATH
  /* 34 */ &DicStack::vDump,        // CN_DICSTACK
  /* 35 */ &Dump::vInvalid,         // CN_DIR
  
  /* 36 */ &Dump::vDummy,           // CN_ENTRY
  /* 37 */ &Dump::vDummy,           // CN_ENTRYBUFFER
  /*    */ &Dump::vDummy,           // CN_ENUMFLAGS
  /*    */ &Dump::vDummy,           // CN_EXPORTBUFFER

  /* 38 */ &Dump::vInvalid,         // CN_FETCHBUFFER
  /* 39 */ &Dump::vInvalid,         // CN_FILE
  /* 40 */ &Dump::vDummy,           // CN_FORMATTERS
  /* 41 */ &Dump::vDummy,           // CN_FREEBUFFER
  /* 42 */ &Dump::vDummy,           // CN_FREECHUNK
  /* 43 */ &Dump::vDummy,           // CN_FREEDNOT
  /* 44 */ &Dump::vDummy,           // CN_FREEENTRY
  
  /*    */ &Dump::vInvalid,         // CN_GENVAR
  /* 48 */ &Grid::vDump,            // CN_GRID
  
  /*    */ &Dump::vDummy,           // CN_HASHBUFFER
  /*    */ &Dump::vDummy,           // CN_HASHMAP

  /* 50 */ &HeapBuffer::vDump,      // CN_HEAPBUFFER
  
  /* 51 */ &Dump::vDummy,           // CN_INDEX
  /* 52 */ &Dump::vDummy,           // CN_INDEXTREE
  /* 53 */ &Dump::vDummy,           // CN_INDEXTREEVEC
  /* 54 */ &Dump::vDummy,           // CN_IDXCOLVEC
  /* 55 */ &Dump::vDummy,           // CN_IDXVEC
  /*    */ &Dump::vDummy,           // CN_INGESTCHUNK
  /*    */ &Dump::vDummy,           // CN_IOCHUNK
  /*    */ &Dump::vDummy,           // CN_IOCOPY
  /* 57 */ &Dump::vInvalid,         // CN_ITEM
  
  /*    */ &Dump::vDummy,           // CN_JOB__BEGUN
  /*    */ &Dump::vDummy,           // CN_JOB__CANCEL
  /*    */ &Dump::vDummy,           // CN_JOB__CANCELLED
  /*    */ &Dump::vDummy,           // CN_JOB__END
  /*    */ &Dump::vDummy,           // CN_JOB__ENDED
  /*    */ &Dump::vDummy,           // CN_JOB__EXECUTE
  /*    */ &Dump::vDummy,           // CN_JOB__EXECUTED
  /*    */ &Dump::vDummy,           // CN_JOB__FIND

  /*    */ &Dump::vDummy,           // CN_JOB_ALARM
  /*    */ &Dump::vDummy,           // CN_JOB_ANALYZEINDEX
  /*    */ &Dump::vDummy,           // CN_JOB_ANALYZETABLE
  /*    */ &Dump::vDummy,           // CN_JOB_ANSWER
  /*    */ &Dump::vDummy,           // CN_JOB_BOOT
  /*    */ &Dump::vDummy,           // CN_JOB_CAT
  /*    */ &Dump::vDummy,           // CN_JOB_CF
  /*    */ &Dump::vDummy,           // CN_JOB_COMPACTINDEX
  /*    */ &Dump::vDummy,           // CN_JOB_CONNECTCLUSTER
  /*    */ &Dump::vDummy,           // CN_JOB_CONNECTOR
  /*    */ &Dump::vDummy,           // CN_JOB_COPYBUFFER
  /*    */ &Dump::vDummy,           // CN_JOB_CREATECATALOG
  /*    */ &Dump::vDummy,           // CN_JOB_CREATEINDEX
  /*    */ &Dump::vDummy,           // CN_JOB_CREATESCHEMA
  /*    */ &Dump::vDummy,           // CN_JOB_CREATETABLE
  /*    */ &Dump::vDummy,           // CN_JOB_CREATETENANT
  /*    */ &Dump::vDummy,           // CN_JOB_DOCOL
  /*    */ &Dump::vDummy,           // CN_JOB_DROPCATALOG
  /*    */ &Dump::vDummy,           // CN_JOB_DROPINDEX
  /*    */ &Dump::vDummy,           // CN_JOB_DROPSCHEMA
  /*    */ &Dump::vDummy,           // CN_JOB_DROPTABLE
  /*    */ &Dump::vDummy,           // CN_JOB_DROPTENANT
  /*    */ &Dump::vDummy,           // CN_JOB_DUMPINDEX
  /*    */ &Dump::vDummy,           // CN_JOB_DUMPTABLE
  /*    */ &Dump::vDummy,           // CN_JOB_EXPORT
  /*    */ &Dump::vDummy,           // CN_JOB_FETCHROWS
  /*    */ &Dump::vDummy,           // CN_JOB_GENERATE
  /*    */ &Dump::vDummy,           // CN_JOB_HEALTH
  /*    */ &Dump::vDummy,           // CN_JOB_INDEXJOIN
  /*    */ &Dump::vDummy,           // CN_JOB_INFLATEROWS
  /*    */ &Dump::vDummy,           // CN_JOB_INGEST
  /*    */ &Dump::vDummy,           // CN_JOB_INSERTROW
  /*    */ &Dump::vDummy,           // CN_JOB_INSERTTABLE
  /*    */ &Dump::vDummy,           // CN_JOB_KIDEXPORT
  /*    */ &Dump::vDummy,           // CN_JOB_LF
  /*    */ &Dump::vDummy,           // CN_JOB_LISTCATALOGS
  /*    */ &Dump::vDummy,           // CN_JOB_LISTCOLUMNS
  /*    */ &Dump::vDummy,           // CN_JOB_LISTIDXCOLUMNS
  /*    */ &Dump::vDummy,           // CN_JOB_LISTINDEXES
  /*    */ &Dump::vDummy,           // CN_JOB_LISTPARTCOLUMNS
  /*    */ &Dump::vDummy,           // CN_JOB_LISTPARTITIONS
  /*    */ &Dump::vDummy,           // CN_JOB_LISTSCHEMAS
  /*    */ &Dump::vDummy,           // CN_JOB_LISTTABLES
  /*    */ &Dump::vDummy,           // CN_JOB_LISTTENANTS
  /*    */ &Dump::vDummy,           // CN_JOB_LOADTABLE
  /*    */ &Dump::vDummy,           // CN_JOB_MONITOR
  /*    */ &Dump::vDummy,           // CN_JOB_NETHOG
  /*    */ &Dump::vDummy,           // CN_JOB_NODEINFO
  /*    */ &Dump::vDummy,           // CN_JOB_PERIODIC
  /*    */ &Dump::vDummy,           // CN_JOB_PING
  /*    */ &Dump::vDummy,           // CN_JOB_PUBLISH
  /*    */ &Dump::vDummy,           // CN_JOB_PURGE
  /*    */ &Dump::vDummy,           // CN_JOB_PWF
  /*    */ &Dump::vDummy,           // CN_JOB_RECEIVER
  /*    */ &Dump::vDummy,           // CN_JOB_REPEATER
  /*    */ &Dump::vDummy,           // CN_JOB_REPORT
  /*    */ &Dump::vDummy,           // CN_JOB_SANITY
  /*    */ &Dump::vDummy,           // CN_JOB_SELECTTABLE
  /*    */ &Dump::vDummy,           // CN_JOB_SHOWCLUSTER
  /*    */ &Dump::vDummy,           // CN_JOB_SPACE
  /*    */ &Dump::vDummy,           // CN_JOB_SINK
  /*    */ &Dump::vDummy,           // CN_JOB_SIZEINDEX
  /*    */ &Dump::vDummy,           // CN_JOB_SIZETABLE
  /*    */ &Dump::vDummy,           // CN_JOB_START
  /*    */ &Dump::vDummy,           // CN_JOB_STARTCLUSTER
  /*    */ &Dump::vDummy,           // CN_JOB_STATUS
  /*    */ &Dump::vDummy,           // CN_JOB_EXPORT
  /*    */ &Dump::vDummy,           // CN_JOB_SUBFETCH
  /*    */ &Dump::vDummy,           // CN_JOB_SUBINDEXJOIN
  /*    */ &Dump::vDummy,           // CN_JOB_SUBINFLATE
  /*    */ &Dump::vDummy,           // CN_JOB_SUBINGESTER
  /*    */ &Dump::vDummy,           // CN_JOB_SUBINSERTER
  /*    */ &Dump::vDummy,           // CN_JOB_SUBLOADER
  /*    */ &Dump::vDummy,           // CN_JOB_SUBPUBLISH
  /*    */ &Dump::vDummy,           // CN_JOB_SUBPURGER
  /*    */ &Dump::vDummy,           // CN_JOB_SUBSCAN
  /*    */ &Dump::vDummy,           // CN_JOB_SUBTRUNCATE
  /*    */ &Dump::vDummy,           // CN_JOB_SUBWALKER
  /*    */ &Dump::vDummy,           // CN_JOB_SUPERVISOR
  /*    */ &Dump::vDummy,           // CN_JOB_TABLESIZES
  /*    */ &Dump::vDummy,           // CN_JOB_TRUNCATETABLE
  /*    */ &Dump::vDummy,           // CN_JOB_USESCHEMA
  /*    */ &Dump::vDummy,           // CN_JOB_VERIFYINDEX
  /*    */ &Dump::vDummy,           // CN_JOB_VERIFYTABLE
  /*    */ &Dump::vDummy,           // CN_JOB_VISUALIZEINDEX
  /*    */ &Dump::vDummy,           // CN_JOB_VISUALIZETABLE
  /*    */ &Dump::vDummy,           // CN_JOB_WAITCLUSTER
  /*    */ &Dump::vDummy,           // CN_JOB_WALK
  /*    */ &Dump::vDummy,           // CN_JOB_WRITEBUFFER
  /*    */ &Dump::vDummy,           // CN_JOB_WRITEROW

  /*    */ &JobList::vDump,         // CN_JOBLIST
  
  /* 59 */ &Heap::vDump,            // CN_JUMBOPERSISTENT
  /* 60 */ &Heap::vDump,            // CN_JUMBOTRANSIENT
  
  /* 61 */ &Dump::vDummy,           // CN_KEY
  /* 64 */ &Dump::vInvalid,         // CN_KEYWRK
  
  /* 65 */ &Heap::vDump,            // CN_LARGEPERSISTENT
  /* 66 */ &Heap::vDump,            // CN_LARGETRANSIENT
  /*    */ &Dump::vDummy,           // CN_LOADCHUNK
  /* 71 */ &LoadMap::vDump,         // CN_LOADMAP
  /*    */ &Dump::vInvalid,         // CN_LOCAL
  /*    */ &LocalSet::vDump,        // CN_LOCALSET
  /* 72 */ &Dump::vInvalid,         // CN_LOCK
  
  /* 73 */ &Heap::vDump,            // CN_MEDIUMPERSISTENT
  /* 74 */ &Heap::vDump,            // CN_MEDIUMTRANSIENT
  /*    */ &Dump::vUndefined,       // CN_MEMTEST
  /*114 */ &Dump::vUndefined,       // CN_MUTEX

  /*    */ &Dump::vUndefined,       // CNNMVAL
  /* 75 */ &Node::vDump,            // CN_NODE
  /*    */ &Dump::vUndefined,       // CN_NOW
  /*    */ &Dump::vUndefined,       // CN_NUMBAT

  /* 77 */ &Dump::vUndefined,       // CN_OP
  
  /* 78 */ &Parser::vDump,          // CN_PARSER
  /* 82 */ &Dump::vInvalid,         // CN_PATHVEC
  /* 82 */ &Dump::vInvalid,         // CN_PERUSE
  /*    */ &Dump::vInvalid,         // CN_PIPE
  /* 86 */ &Plan::vDump,            // CN_PLAN
  /* 87 */ &Dump::vUndefined,       // CN_PLANNER
  /* 89 */ &Dump::vUndefined,       // CN_PREDICATE
  /* 90 */ &Dump::vUndefined,       // CN_PREDVEC
  /* 91 */ &Dump::vUndefined,       // CN_PTHREAD
  /* 94 */ &Dump::vDummy,           // CN_PTNCOLVEC
  
  /* 95 */ &Queue::vDump,           // CN_QUEUE
  /* 96 */ &QueueBuffer::vDump,     // CN_QUEUEBUFFER
  /*    */ &Dump::vUndefined,       // CN_QUEUES

  /*    */ &Dump::vUndefined,       // CN_RECORD
  /*    */ &Dump::vUndefined,       // CN_RECORDBUFFER
  /*    */ &Dump::vUndefined,       // CN_REFBUFFER
  /*    */ &Dump::vUndefined,       // CN_REFSET
  /*    */ &LocalSet::vDump,        // CN_RESOURCETRACKER
  /*    */ &Dump::vUndefined,       // CN_RESPONSE
  /*    */ &Dump::vUndefined,       // CN_RESPONSEVEC
  /*    */ &Dump::vUndefined,       // CN_ROW
  /*102 */ &Dump::vUndefined,       // CN_ROWAGG
  /*    */ &Dump::vUndefined,       // CN_ROWBUFFER
  /*    */ &Dump::vUndefined,       // CN_ROWCODER
  /*105 */ &Dump::vUndefined,       // CN_ROWONCE
  /*    */ &Dump::vUndefined,       // CN_ROWTRAY
  
  /*    */ &Dump::vUndefined,       // CN_SCHEMA
  /*    */ &Dump::vUndefined,       // CN_SCHEMATEST
  /*    */ &Dump::vUndefined,       // CN_SEEME
  /*115 */ &SePrompt::vDump,        // CN_SEPROMPT
  /*    */ &Session::vDump,         // CN_SESSION
  /*116 */ &Dump::vUndefined,       // CN_SESSIONSTATE
  /*116 */ &Dump::vUndefined,       // CN_SINCE
  /*118 */ &Dump::vUndefined,       // CN_SKIPLIST
  /*119 */ &Dump::vUndefined,       // CN_SKIPNODE
  /*120 */ &Dump::vUndefined,       // CN_SLAB
  /*121 */ &Dump::vUndefined,       // CN_SLABALLOCATOR
  /*122 */ &Slots::vDump,           // CN_SLOTS
  /*123 */ &Heap::vDump,            // SN_SMALLPERSISTENT
  /*124 */ &Heap::vDump,            // SN_SMALLTRANSIENT
  /*    */ &Dump::vInvalid,         // CN_SNBCD
  /*    */ &Dump::vInvalid,         // CN_SNBOL
  /*    */ &Dump::vInvalid,         // CN_SNDTE
  /*    */ &Dump::vInvalid,         // CN_SNFLT
  /*    */ &Dump::vInvalid,         // CN_SNINT
  /*    */ &Dump::vInvalid,         // CN_SNIVL
  /*    */ &Dump::vInvalid,         // CN_SNTME
  /*    */ &Dump::vInvalid,         // CN_SNTSP
  /*    */ &Dump::vInvalid,         // CN_SNVCH
  /*125 */ &Socket::vDump,          // CN_SOCKET
  /*    */ &Dump::vInvalid,         // CN_SPBCD
  /*    */ &Dump::vInvalid,         // CN_SPBOL
  /*    */ &Dump::vInvalid,         // CN_SPDTE
  /*    */ &Dump::vInvalid,         // CN_SPFLT
  /*    */ &Dump::vInvalid,         // CN_SPINT
  /*    */ &Dump::vInvalid,         // CN_SPIVL
  /*    */ &Dump::vInvalid,         // CN_SPTME
  /*    */ &Dump::vInvalid,         // CN_SPTSP
  /*    */ &Dump::vInvalid,         // CN_SPVCH
  /*126 */ &Stack::vDump,           // CN_STACK
  /*    */ &Dump::vInvalid,         // CN_STATISTICS
  /*127 */ &Dump::vInvalid,         // CN_STEPACCUM
  /*128 */ &StepTiming::vDump,      // CN_STEPTIMING
  /* 13 */ &Dump::vInvalid,         // CN_STRING_T
  /*129 */ &Dump::vInvalid,         // CN_SYSCALL

  /*130 */ &Table::vDump,           // CN_TABLE
  /*    */ &TablePtn::vDump,        // CN_TABLEPTN
  /*    */ &Dump::vDummy,           // CN_TABLEPTNVEC
  /*131 */ &Task::vDump,            // CN_TASK
  /*    */ &Dump::vUndefined,       // CN_TASKLIST
  /*134 */ &Dump::vUndefined,       // CN_TASKQUEUE
  /*    */ &Dump::vUndefined,       // CN_TEMPNAME
  /*    */ &Dump::vUndefined,       // CN_TENANT
  /*    */ &Dump::vUndefined,       // CN_TENANTS
  /*137 */ &Dump::vUndefined,       // CN_THREAD
  /*    */ &Dump::vUndefined,       // CN_THREADVEC
  /*141 */ &Dump::vUndefined,       // CN_TIME
  /*142 */ &Dump::vInvalid,         // CN_TIMER
  /*143 */ &Token::vDump,           // CN_TOKEN
  /*144 */ &Tokeniser::vDump,       // CN_TOKENISER
  /*145 */ &Dump::vInvalid,         // CN_TOKENARY
  /*146 */ &Dump::vInvalid,         // CN_TOKENVEC
  /*147 */ &Tx::vDump,              // CN_TX
  /*148 */ &Dump::vUndefined,       // CN_TXSET

  /*149 */ &Dump::vInvalid,         // CN_WILDERNESS
  /*150 */ &ForthWord::vDump,       // CN_WORD
  
  /*151 */ &Dump::vUndefined,       // CN_ZSMALL_
  /*152 */ &Dump::vUndefined,       // CN_ZMEDIUM_
  /*153 */ &Dump::vUndefined,       // CN_ZLARGE_
  /*154 */ &Dump::vUndefined,       // CN_ZJUMBO_
  
  /*155 */ &Dump::vEnums            // CN_ENUMS
};

/*----------------------------------------------------------------------------*/
/* class sizes                                                                */
/*----------------------------------------------------------------------------*/
i8 giClassSize[]
{
  /*  0 */ 0,                          // CN_NONE
  
  /*  1 */ sizeof(BcdVal),             // CN_BCDVAL
  /*  3 */ sizeof(BolVal),             // CN_BOLVAL
  /*  4 */ sizeof(DteVal),             // CN_DTEVAL
  /*    */ sizeof(FipVal),             // CN_FIPVAL
  /*  6 */ sizeof(FltVal),             // CN_FLTVAL
  /*  7 */ sizeof(IntVal),             // CN_INTVAL
  /*  9 */ sizeof(IvlVal),             // CN_IVLVAL
  /* 11 */ sizeof(PtrVal),             // CN_PTRVAL
  /* 12 */ sizeof(StrVal),             // CN_STRVAL
  /* 14 */ sizeof(TmeVal),             // CN_TMEVAL
  /* 15 */ sizeof(TspVal),             // CN_TSPVAL
  
  /* 19 */ sizeof(AccessPath),         // CN_ACCESSPATH
  /*    */ sizeof(Action),             // CN_ACTION
  /*    */ sizeof(ActionSequence),     // CN_ACTIONSEQUENCE
  /*    */ sizeof(ActivityMark),       // CN_ACTIVITYMAP
  /*    */ sizeof(ActivityMark),       // CN_ACTIVITYMARK
  /*    */ sizeof(ApiState),           // CN_APISTATE
  /* 22 */ sizeof(Arg),                // CN_ARG
  /* 23 */ sizeof(Arguments),          // CN_ARGUMENTS
  /* 24 */ sizeof(Atomic),             // CN_ATOMIC
  /* 25 */ sizeof(AttachMap),          // CN_ATTACHMAP

  /* 29 */ sizeof(BPA),                // CN_BPA
  /* 28 */ sizeof(BufferPool),         // CN_BUFFERPOOL
  /* 28 */ sizeof(ByteRate),           // CN_BYTERATE

  /* 30 */ sizeof(CarryOver),          // CN_CARRYOVER
  /*    */ sizeof(Catalog),            // CN_CATALOG
  /* 31 */ sizeof(Column),             // CN_COLUMN
  /* 32 */ sizeof(ColumnVec),          // CN_COLUMNVEC
  /* 32 */ sizeof(Command),            // CN_COMMAND
  /* 32 */ sizeof(Compiler),           // CN_COMPILER
  /* 32 */ sizeof(Completer),          // CN_COMPLETER

  /*    */ sizeof(Datum),              // CN_DATUM
  /*    */ sizeof(DecaCnt),            // CN_DECACNT
  /*    */ sizeof(Decider),            // CN_DECIDER
  /* 33 */ sizeof(Dic),                // CN_DIC
  /* 34 */ sizeof(DicPath),            // CN_DICPATH
  /* 34 */ sizeof(DicStack),           // CN_DICSTACK
  /* 35 */ sizeof(Dir),                // CN_DIR
  
  /* 36 */ sizeof(Item),               // CN_ENTRY
  /* 37 */ sizeof(EntryBuffer),        // CN_ENTRYBUFFER
  /*    */ sizeof(EnumFlags),          // CN_ENUMFLAGS
  /* 32 */ sizeof(ExportBuffer),       // CN_EXPORTBUFFER

  /*    */ sizeof(FetchBuffer),        // CN_FETCHBUFFER
  /* 39 */ sizeof(File),               // CN_FILE
  /* 40 */ sizeof(Formatters),         // CN_FORMATTERS
  /* 41 */ sizeof(PoolBuffer),         // CN_FREEBUFFER
  /* 42 */ sizeof(FreeChunk),          // CN_FREECHUNK
  /* 43 */ 0,                          // CN_FREEDNOT
  /* 44 */ 0,                          // CN_FREEENTRY
  
  /*    */ sizeof(GenVar),             // CN_GENVAR
  /* 48 */ sizeof(Grid),               // CN_GRID
  
  /*    */ sizeof(HashBuffer),         // CN_HASHBUFFER
  /*    */ sizeof(HashMap),            // CN_HASHMAP
  /* 50 */ sizeof(HeapBuffer),         // CN_HEAPBUFFER
  
  /* 51 */ sizeof(Index),              // CN_INDEX
  /* 52 */ sizeof(IndexTree),          // CN_INDEXTREE
  /* 53 */ sizeof(IndexTreeVec),       // CN_INDEXTREEVEC
  /* 54 */ sizeof(IdxColVec),          // CN_IDXCOLVEC
  /* 55 */ sizeof(IdxVec),             // CN_IDXVEC
  /*    */ sizeof(IngestChunk),        // CN_INGESTCHUNK
  /*    */ sizeof(IoChunk),            // CN_IOCHUNK
  /*    */ sizeof(IoCopy),             // CN_IOCOPY
  /* 57 */ sizeof(Item),               // CN_ITEM
  
  /*    */ sizeof(Job__Begun),         // CN_JOB__BEGUN
  /*    */ sizeof(Job__Cancel),        // CN_JOB__CANCEL
  /*    */ sizeof(Job__Cancelled),     // CN_JOB__CANCELLED
  /*    */ sizeof(Job__End),           // CN_JOB__END
  /*    */ sizeof(Job__Ended),         // CN_JOB__ENDED
  /*    */ sizeof(Job__Execute),       // CN_JOB__EXECUTE
  /*    */ sizeof(Job__Executed),      // CN_JOB__EXECUTED
  /*    */ sizeof(Job__Find),          // CN_JOB__FIND
  
  /*    */ sizeof(Job_Alarm),          // CN_JOB_ALARM
  /*    */ sizeof(Job_AnalyzeIndex),   // CN_JOB_ANALYZEINDEX
  /*    */ sizeof(Job_AnalyzeTable),   // CN_JOB_ANALYZETABLE
  /*    */ sizeof(Job_Answer),         // CN_JOB_ANSWER
  /*    */ sizeof(Job_Boot),           // CN_JOB_BOOT
  /*    */ sizeof(Job_Cat),            // CN_JOB_CAT
  /*    */ sizeof(Job_Cf),             // CN_JOB_CF
  /*    */ sizeof(Job_CompactIndex),   // CN_JOB_COMPACTINDEX
  /*    */ sizeof(Job_ConnectCluster), // CN_JOB_CONNECTCLUSTER
  /*    */ sizeof(Job_Connector),      // CN_JOB_CONNECTOR
  /*    */ sizeof(Job_CopyBuffer),     // CN_JOB_COPYBUFFER
  /*    */ sizeof(Job_CreateCatalog),  // CN_JOB_CREATECATALOG
  /*    */ sizeof(Job_CreateIndex),    // CN_JOB_CREATEINDEX
  /*    */ sizeof(Job_CreateSchema),   // CN_JOB_CREATESCHEMA
  /*    */ sizeof(Job_CreateTable),    // CN_JOB_CREATETABLE
  /*    */ sizeof(Job_CreateTenant),   // CN_JOB_CREATETENANT
  /*    */ sizeof(Job_Docol),          // CN_JOB_DOCOL
  /*    */ sizeof(Job_DropCatalog),    // CN_JOB_DROPCATALOG
  /*    */ sizeof(Job_DropIndex),      // CN_JOB_DROPINDEX
  /*    */ sizeof(Job_DropSchema),     // CN_JOB_DROPSCHEMA
  /*    */ sizeof(Job_DropTable),      // CN_JOB_DROPTABLE
  /*    */ sizeof(Job_DropTenant),     // CN_JOB_DROPTENANT
  /*    */ sizeof(Job_DumpIndex),      // CN_JOB_DUMPINDEX
  /*    */ sizeof(Job_DumpTable),      // CN_JOB_DUMPTABLE
  /*    */ sizeof(Job_Export),         // CN_JOB_EXPORT
  /*    */ sizeof(Job_FetchRows),      // CN_JOB_FETCHROWS
  /*    */ sizeof(Job_Generate),       // CN_JOB_GENERATE
  /*    */ sizeof(Job_Health),         // CN_JOB_HEALTH
  /*    */ sizeof(Job_IndexJoin),      // CN_JOB_INDEXJOIN
  /*    */ sizeof(Job_InflateRows),    // CN_JOB_INFLATEROWS
  /*    */ sizeof(Job_Ingest),         // CN_JOB_INGEST
  /*    */ sizeof(Job_InsertRow),      // CN_JOB_INSERTROW
  /*    */ sizeof(Job_InsertTable),    // CN_JOB_INSERTTABLE
  /*    */ sizeof(Job_KidExport),      // CN_JOB_KIDEXPORT
  /*    */ sizeof(Job_Lf),             // CN_JOB_LF
  /*    */ sizeof(Job_ListCatalogs),   // CN_JOB_LISTCATALOGS
  /*    */ sizeof(Job_ListColumns),    // CN_JOB_LISTCOLUMNS
  /*    */ sizeof(Job_ListIdxColumns), // CN_JOB_LISTIDXCOLUMNS
  /*    */ sizeof(Job_ListIndexes),    // CN_JOB_LISTINDEXES
  /*    */ sizeof(Job_ListPartColumns),// CN_JOB_LISTPARTCOLUMNS
  /*    */ sizeof(Job_ListPartitions), // CN_JOB_LISTPARTITIONS
  /*    */ sizeof(Job_ListSchemas),    // CN_JOB_LISTSCHEMAS
  /*    */ sizeof(Job_ListTables),     // CN_JOB_LISTTABLES
  /*    */ sizeof(Job_ListTenants),    // CN_JOB_LISTTENANTS
  /*    */ sizeof(Job_LoadTable),      // CN_JOB_LOADTABLE
  /*    */ sizeof(Job_Monitor),        // CN_JOB_MONITOR
  /*    */ sizeof(Job_NetHog),         // CN_JOB_NETHOG
  /*    */ sizeof(Job_NodeInfo),       // CN_JOB_NODEINFO
  /*    */ sizeof(Job_Periodic),       // CN_JOB_PERIODIC
  /*    */ sizeof(Job_Ping),           // CN_JOB_PING
  /*    */ sizeof(Job_Publish),        // CN_JOB_PUBLISH
  /*    */ sizeof(Job_Purge),          // CN_JOB_PURGE
  /*    */ sizeof(Job_Pwf),            // CN_JOB_PWF
  /*    */ sizeof(Job_Receiver),       // CN_JOB_RECEIVER
  /*    */ sizeof(Job_Repeater),       // CN_JOB_REPEATER
  /*    */ sizeof(Job_Repeater),       // CN_JOB_REPORT
  /*    */ sizeof(Job_Sanity),         // CN_JOB_SANITY
  /*    */ sizeof(Job_SelectTable),    // CN_JOB_SELECTTABLE
  /*    */ sizeof(Job_ShowCluster),    // CN_JOB_SHOWCLUSTER
  /*    */ sizeof(Job_Sink),           // CN_JOB_SINK
  /*    */ sizeof(Job_SizeIndex),      // CN_JOB_SIZEINDEX
  /*    */ sizeof(Job_SizeTable),      // CN_JOB_SIZETABLE
  /*    */ sizeof(Job_Space),          // CN_JOB_SPACE
  /*    */ sizeof(Job_Start),          // CN_JOB_START
  /*    */ sizeof(Job_StartCluster),   // CN_JOB_STARTCLUSTER
  /*    */ sizeof(Job_Status),         // CN_JOB_STATUS
  /*    */ sizeof(Job_SubExporter),    // CN_JOB_SUBEXPORTER
  /*    */ sizeof(Job_SubFetch),       // CN_JOB_SUBFETCH
  /*    */ sizeof(Job_SubIndexJoin),   // CN_JOB_SUBINDEXJOIN
  /*    */ sizeof(Job_SubInflate),     // CN_JOB_SUBINFLATE
  /*    */ sizeof(Job_SubIngester),    // CN_JOB_SUBINGESTER
  /*    */ sizeof(Job_SubInserter),    // CN_JOB_SUBINSERTER
  /*    */ sizeof(Job_SubLoader),      // CN_JOB_SUBLOADER
  /*    */ sizeof(Job_SubPublish),     // CN_JOB_SUBPUBLISH
  /*    */ sizeof(Job_SubPurger),      // CN_JOB_SUBPURGER
  /*    */ sizeof(Job_SubScan),        // CN_JOB_SUBSCAN
  /*    */ sizeof(Job_SubTruncate),    // CN_JOB_SUBTRUNCATE
  /*    */ sizeof(Job_SubWalker),      // CN_JOB_SUBWALKER
  /*    */ sizeof(Job_Supervisor),     // CN_JOB_SUPERVISOR
  /*    */ sizeof(Job_TableSizes),     // CN_JOB_TABLESIZES
  /*    */ sizeof(Job_TruncateTable),  // CN_JOB_TRUNCATETABLE
  /*    */ sizeof(Job_UseSchema),      // CN_JOB_USESCHEMA
  /*    */ sizeof(Job_VerifyIndex),    // CN_JOB_VERIFYINDEX
  /*    */ sizeof(Job_VerifyTable),    // CN_JOB_VERIFYTABLE
  /*    */ sizeof(Job_VisualizeIndex), // CN_JOB_VISUALIZEINDEX
  /*    */ sizeof(Job_VisualizeTable), // CN_JOB_VISUALIZETABLE
  /*    */ sizeof(Job_WaitCluster),    // CN_JOB_WAITCLUSTER
  /*    */ sizeof(Job_Walk),           // CN_JOB_WALK
  /*    */ sizeof(Job_WriteBuffer),    // CN_JOB_WRITEBUFFER
  /*    */ sizeof(Job_WriteRow),       // CN_JOB_WRITEROW
  
  /*    */ sizeof(JobList),            // CN_JOBLIST
  
  /* 59 */ sizeof(Heap), // correct    // CN_JUMBOPERSISTENT
  /* 60 */ sizeof(Heap), // correct    // CN_JUMBOTRANSIENT
  
  /* 61 */ sizeof(KeyData),            // CN_KEY
  /* 64 */ sizeof(KeyWrk),             // CN_KEYWRK
  
  /* 65 */ sizeof(Heap), // correct    // CN_LARGEPERSISTENT
  /* 66 */ sizeof(Heap), // correct    // CN_LARGETRANSIENT
  /*    */ sizeof(LoadChunk),          // CN_LOADCHUNK
  /* 71 */ sizeof(LoadMap),            // CN_LOADMAP
  /*    */ sizeof(Local),              // CN_LOCAL
  /*    */ sizeof(LocalSet),           // CN_LOCALSET
  /* 72 */ sizeof(Lock),               // CN_LOCK
  
  /* 73 */ sizeof(Heap), // correct    // CN_MEDIUMPERSISTENT
  /* 74 */ sizeof(Heap), // correct    // CN_MEDIUMTRANSIENT
  /*114 */ sizeof(MemTest),            // CN_MEMTEST
  /*114 */ sizeof(Mutex),              // CN_MUTEX

  /*    */ 0,             // CN_NMVAL
  /* 75 */ sizeof(Node),               // CN_NODE
  /*    */ sizeof(Now),                // CN_NOW
  /*    */ sizeof(Numbat),             // CN_NUMBAT

  /* 77 */ sizeof(Op),                 // CN_OP
  
  /* 78 */ sizeof(Parser),             // CN_PARSER
  /* 82 */ sizeof(PathVec),            // CN_PATHVEC
  /*    */ sizeof(Peruse),             // CN_PERUSE
  /*    */ sizeof(Pipe),               // CN_PIPE
  /* 86 */ sizeof(Plan),               // CN_PLAN
  /* 87 */ sizeof(Planner),            // CN_PLANNER
  /* 90 */ sizeof(Predicate),          // CN_PREDICATE
  /* 91 */ sizeof(PredicateVec),       // CN_PREDICATEVEC
  /* 89 */ sizeof(Pthread),            // CN_PTHREAD
  /* 94 */ sizeof(PtnColVec),          // CN_PTNCOLVEC
  
  /* 95 */ sizeof(Queue),              // CN_QUEUE
  /* 96 */ sizeof(QueueBuffer),        // CN_QUEUEBUFFER
  /*    */ sizeof(Queues),             // CN_QUEUES
  
  /*    */ sizeof(Record),             // CN_RECORD
  /*    */ sizeof(RecordBuffer),       // CN_RECORDBUFFER
  /*    */ sizeof(RefBuffer),          // CN_REFBUFFER
  /*    */ sizeof(RefSet),             // CN_REFSET
  /*    */ sizeof(ResourceTracker),    // CN_RESOURCETRACKER
  /*    */ sizeof(Response),           // CN_RESPONSE
  /*    */ sizeof(ResponseVec),        // CN_RESPONSEVEC
  /*    */ sizeof(Row),                // CN_ROW
  /*102 */ sizeof(RowAgg),             // CN_ROWAGG
  /*    */ sizeof(RowBuffer),          // CN_ROWBUFFER
  /*    */ sizeof(RowCoder),           // CN_ROWCODER
  /*105 */ sizeof(RowOnce),            // CN_ROWONCE
  /*105 */ sizeof(RowTray),            // CN_ROWTRAY
  
  /*    */ sizeof(Schema),             // CN_SCHEMA
  /*    */ sizeof(SchemaTest),         // CN_SCHEMATEST
  /*    */ sizeof(SeeMe),              // CN_SEEME
  /*115 */ sizeof(SePrompt),           // CN_SEPROMPT
  /*    */ sizeof(Session),            // CN_SESSION
  /*    */ sizeof(SessionState),       // CN_SESSIONSTATE
  /*116 */ sizeof(Since),              // CN_SINCE
  /*118 */ sizeof(SkipListNL),         // CN_SKIPLIST
  /*119 */ sizeof(SkipNode),           // CN_SKIPNODE
  /*120 */ sizeof(Slab),               // CN_SLAB
  /*121 */ sizeof(SlabAllocator),      // CN_SLABALLOCATOR
  /*122 */ sizeof(Slots),              // CN_SLOTS
  /*123 */ sizeof(Heap), // correct    // SN_SMALLPERSISTENT
  /*124 */ sizeof(Heap), // correct    // SN_SMALLTRANSIENT
  /*    */ 0,              // CN_SNBCD
  /*    */ 0,              // CN_SNBOL
  /*    */ 0,              // CN_SNDTE
  /*    */ 0,              // CN_SNFLT
  /*    */ 0,              // CN_SNINT
  /*    */ 0,              // CN_SNIVL
  /*    */ 0,              // CN_SNTME
  /*    */ 0,              // CN_SNTSP
  /*    */ 0,              // CN_SNVCH
  /*125 */ sizeof(Socket),             // CN_SOCKET
  /*    */ sizeof(spBCD),              // CN_SPBCD
  /*    */ sizeof(spBOL),              // CN_SPBOL
  /*    */ sizeof(spDTE),              // CN_SPDTE
  /*    */ sizeof(spFLT),              // CN_SPFLT
  /*    */ sizeof(spINT),              // CN_SPINT
  /*    */ sizeof(spIVL),              // CN_SPIVL
  /*    */ sizeof(spTME),              // CN_SPTME
  /*    */ sizeof(spTSP),              // CN_SPTSP
  /*    */ sizeof(spVCH),              // CN_SPVCH
  /*126 */ sizeof(Stack),              // CN_STACK
  /*    */ sizeof(Statistics),         // CN_STATISTICS
  /*127 */ sizeof(StepAccum),          // CN_STEPACCUM
  /*128 */ sizeof(StepTiming),         // CN_STEPTIMING
  /* 13 */ sizeof(s32),                // CN_STRING_T
  /*129 */ sizeof(SysCall),            // CN_SYSCALL

  /*130 */ sizeof(Table),              // CN_TABLE
  /*    */ sizeof(TablePtn),           // CN_TABLEPTN
  /*    */ sizeof(TablePtnVec),        // CN_TABLEPTNVEC
  /*131 */ sizeof(Task),               // CN_TASK
  /*    */ sizeof(TaskList),           // CN_TASKLIST
  /*134 */ sizeof(TaskQueue),          // CN_TASKQUEUE
  /*134 */ sizeof(TempName),           // CN_TEMPNAME
  /*    */ sizeof(Tenant),             // CN_TENANT
  /*    */ sizeof(Tenants),            // CN_TENANTS
  /*137 */ sizeof(Thread),             // CN_THREAD
  /*    */ sizeof(ThreadVec),          // CN_THREADVEC
  /*141 */ sizeof(Time),               // CN_TIME
  /*142 */ sizeof(Timer),              // CN_TIMER
  /*143 */ sizeof(Token),              // CN_TOKEN
  /*144 */ sizeof(Tokeniser),          // CN_TOKENISER
  /*145 */ sizeof(TokenAry),           // CN_TOKENARY
  /*146 */ sizeof(TokenVec),           // CN_TOKENVEC
  /*147 */ sizeof(Tx),                 // CN_TX
  /*148 */ sizeof(TxSet),              // CN_TXSET

  /*149 */ sizeof(Wilderness),         // CN_WILDERNESS
  /*150 */ sizeof(ForthWord),               // CN_WORD
  
  /*151 */ C_SIZE_SMALL_OBJECT  - 256, // CN_ZSMALL_
  /*152 */ C_SIZE_MEDIUM_OBJECT - 256, // CN_ZMEDIUM_
  /*153 */ C_SIZE_POOL_BUFFER   - 256, // CN_ZLARGE_
  /*154 */ C_SIZE_JUMBO_OBJECT  - 256, // CN_ZJUMBO_

  /*155 */ -1
};

/*----------------------------------------------------------------------------*/
/* which heap                                                                 */
/*----------------------------------------------------------------------------*/
eHT geWhichHeap[]
{
  /*  0 */ HT_INVALID,           // CN_NONE
  
  /*  1 */ HT_INVALID,           // CN_BCDVAL
  /*  3 */ HT_INVALID,           // CN_BOLVAL
  /*  4 */ HT_INVALID,           // CN_DTEVAL
  /*  8 */ HT_INVALID,           // CN_FIPVAL
  /*  6 */ HT_INVALID,           // CN_FLTVAL
  /*  7 */ HT_INVALID,           // CN_INTVAL
  /*  9 */ HT_INVALID,           // CN_IVLVAL
  /* 11 */ HT_INVALID,           // CN_PTRVAL
  /* 12 */ HT_INVALID,           // CN_STRVAL
  /* 14 */ HT_INVALID,           // CN_TMEVAL
  /* 15 */ HT_INVALID,           // CN_TSPVAL
  
  /* 19 */ HT_TRANSIENT_MEDIUM,  // CN_ACCESSPATH
  /*    */ HT_INVALID,           // CN_ACTION
  /*    */ HT_INVALID,           // CN_ACTIONSEQUENCE
  /*    */ HT_INVALID,           // CN_ACTIVITYMAP
  /*    */ HT_INVALID,           // CN_ACTIVITYMARK
  /*    */ HT_TRANSIENT_LARGE,   // CN_APISTATE
  /* 22 */ HT_TRANSIENT_SMALL,   // CN_ARG
  /* 23 */ HT_INVALID,           // CN_ARGUMENTS
  /* 24 */ HT_INVALID,           // CN_ATOMIC
  /* 25 */ HT_INVALID,           // CN_ATTACHMAP

  /* 29 */ HT_INVALID,           // CN_BPA
  /* 28 */ HT_INVALID,           // CN_BUFFERPOOL
  /* 29 */ HT_TRANSIENT_SMALL,   // CN_BYTERATE

  /* 30 */ HT_INVALID,           // CN_CARRYOVER
  /*    */ HT_INVALID,           // CN_CATALOG
  /*    */ HT_TRANSIENT_SMALL,   // CN_COLUMN
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_COLUMNVEC
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_COMMAND
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_COMPILER
  /*    */ HT_TRANSIENT_SMALL,   // CN_COMPLETER

  /*    */ HT_TRANSIENT_MEDIUM,  // CN_DATUM
  /*    */ HT_TRANSIENT_SMALL,   // CN_DECACNT
  /*    */ HT_TRANSIENT_SMALL,   // CN_DECIDER
  /* 33 */ HT_TRANSIENT_MEDIUM,  // CN_DIC
  /*    */ HT_TRANSIENT_SMALL,   // CN_DICPATH
  /* 34 */ HT_TRANSIENT_SMALL,   // CN_DICSTACK
  /* 35 */ HT_TRANSIENT_SMALL,   // CN_DIR

  /* 36 */ HT_INVALID,           // CN_ENTRY
  /* 37 */ HT_TRANSIENT_LARGE,   // CN_ENTRYBUFFER
  /*    */ HT_TRANSIENT_SMALL,   // CN_ENUMFLAGS
  /*    */ HT_TRANSIENT_LARGE,   // CN_EXPORTBUFFER

  /* 38 */ HT_TRANSIENT_LARGE,   // CN_FETCHBUFFER
  /* 39 */ HT_TRANSIENT_SMALL,   // CN_FILE
  /* 40 */ HT_TRANSIENT_SMALL,   // CN_FORMATTERS
  /* 41 */ HT_INVALID,           // CN_FREEBUFFER
  /* 42 */ HT_INVALID,           // CN_FREECHUNK
  /* 43 */ HT_INVALID,           // CN_FREEDNOT
  /* 44 */ HT_INVALID,           // CN_FREEENTRY
  
  /*    */ HT_INVALID,           // CN_GENVAR
  /* 48 */ HT_TRANSIENT_LARGE,   // CN_GRID

  /*    */ HT_TRANSIENT_LARGE,   // CN_HASHBUFFER
  /*    */ HT_TRANSIENT_SMALL,   // CN_HASHMAP
  /* 50 */ HT_INVALID,           // CN_HEAPBUFFER
  
  /* 51 */ HT_INVALID,           // CN_INDEX
  /* 52 */ HT_TRANSIENT_SMALL,   // CN_INDEXTREE
  /* 53 */ HT_TRANSIENT_MEDIUM,  // CN_INDEXTREEVEC
  /* 54 */ HT_TRANSIENT_MEDIUM,  // CN_IDXCOLVEC
  /* 55 */ HT_TRANSIENT_MEDIUM,  // CN_IDXVEC
  /*    */ HT_TRANSIENT_LARGE,   // CN_INGESTCHUNK
  /*    */ HT_TRANSIENT_LARGE,   // CN_IOCHUNK
  /*    */ HT_TRANSIENT_SMALL,   // CN_IOCOPY
  /* 57 */ HT_INVALID,           // CN_ITEM

  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB__BEGUN
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB__CANCEL
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB__CANCELLED
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB__END
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB__ENDED
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB__EXECUTE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB__EXECUTED
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB__FIND

  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_ALARM
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_ANALYZEINDEX
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_ANALYZETABLE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_ANSWER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_BOOT
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_CAT
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_CF
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_COMPACTINDEX
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_CONNECTCLUSTER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_CONNECTOR
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_COPYBUFFER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_CREATECATALOG
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_CREATEINDEX
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_CREATESCHEMA
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_CREATETABLE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_CREATETENANT
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_DOCOL
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_DROPCATALOG
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_DROPINDEX
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_DROPSCHEMA
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_DROPTABLE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_DROPTENANT
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_DUMPINDEX
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_DUMPTABLE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_EXPORT
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_FETCHROWS
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_GENERATE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_HEALTH
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_INDEXJOIN
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_INFLATEROWS
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_INGEST
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_INSERTROW
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_INSERTTABLE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_KIDEXPORT
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_LF
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_LISTCATALOGS
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_LISTCOLUMNS
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_LISTIDXCOLUMNS
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_LISTINDEXES
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_LISTPARTCOLUMNS
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_LISTPARTITIONS
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_LISTSCHEMAS
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_LISTTABLES
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_LISTTENANTS
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_LOADTABLE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_MONITOR
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_NETHOG
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_NODEINFO
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_PERIODIC
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_PING
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_PUBLISH
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_PURGE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_PWF
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_RECEIVER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_REPEATER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_REPORT
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SANITY
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SELECTTABLE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SHOWCLUSTER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SINK
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SIZEINDEX
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SIZETABLE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SPACE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_START
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_STARTCLUSTER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_STATUS
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SUBEXPORTER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SUBFETCH
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SUBINDEXJOIN
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SUBINFLATE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SUBINGESTER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SUBINSERTER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SUBLOADER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SUBPUBLISH
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SUBPURGER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SUBSCAN
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SUBTRUNCATE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SUBWALKER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SUPERVISOR
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_TABLESIZES
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_TRUNCATETABLE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_USESCHEMA
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_VERIFYINDEX
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_VERIFYTABLE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_VISUALIZEINDEX
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_VISUALIZETABLE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_WAITCLUSTER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_WALK
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_WRITEBUFFER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_WRITEROW
  
  /*    */ HT_TRANSIENT_SMALL,   // CN_JOBLIST
  
  /* 59 */ HT_INVALID,           // CN_JUMBOPERSISTENT
  /* 60 */ HT_INVALID,           // CN_JUMBOTRANSIENT
  
  /* 61 */ HT_INVALID,           // CN_KEY
  /* 64 */ HT_INVALID,           // CN_KEYWRK

  /* 65 */ HT_INVALID,           // CN_LARGEPERSISTENT
  /* 66 */ HT_INVALID,           // CN_LARGETRANSIENT
  /*    */ HT_TRANSIENT_LARGE,   // CN_LOADCHUNK
  /* 71 */ HT_TRANSIENT_JUMBO,   // CN_LOADMAP
  /*    */ HT_INVALID,           // CN_LOCAL
  /*    */ HT_TRANSIENT_LARGE,   // CN_LOCALSET
  /* 72 */ HT_INVALID,           // CN_LOCK
  
  /* 73 */ HT_INVALID,           // CN_MEDIUMPERSISTENT
  /* 74 */ HT_INVALID,           // CN_MEDIUMTRANSIENT
  /*    */ HT_TRANSIENT_SMALL,   // CN_MEMTEST
  /*114 */ HT_TRANSIENT_SMALL,   // CN_MUTEX

  /*    */ HT_TRANSIENT_SMALL,   // CN_NMVAL
  /* 75 */ HT_TRANSIENT_SMALL,   // CN_NODE
  /*    */ HT_TRANSIENT_SMALL,   // CN_NOW
  /*    */ HT_TRANSIENT_LARGE,   // CN_NUMBAT

  /* 77 */ HT_TRANSIENT_SMALL,   // CN_OP
  
  /* 78 */ HT_TRANSIENT_MEDIUM,  // CN_PARSER
  /* 82 */ HT_TRANSIENT_MEDIUM,  // CN_PATHVEC
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_PERUSE
  /*    */ HT_TRANSIENT_SMALL,   // CN_PIPE
  /* 86 */ HT_TRANSIENT_MEDIUM,  // CN_PLAN
  /* 87 */ HT_TRANSIENT_MEDIUM,  // CN_PLANNER
  /* 89 */ HT_TRANSIENT_SMALL,   // CN_PREDICATE
  /* 90 */ HT_TRANSIENT_MEDIUM,  // CN_PREDICATEVEC
  /* 91 */ HT_TRANSIENT_SMALL,   // CN_PTHREAD
  /* 94 */ HT_TRANSIENT_MEDIUM,  // CN_PTNCOLVEC

  /* 95 */ HT_TRANSIENT_SMALL,   // CN_QUEUE
  /* 96 */ HT_INVALID,           // CN_QUEUEBUFFER
  /*    */ HT_TRANSIENT_SMALL,   // CN_QUEUES
  
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_RECORD
  /*    */ HT_TRANSIENT_LARGE,   // CN_RECORDBUFFER
  /*    */ HT_TRANSIENT_LARGE,   // CN_REFBUFFER
  /*    */ HT_INVALID,           // CN_REFSET
  /*    */ HT_TRANSIENT_SMALL,   // CN_RESOURCETRACKER
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_RESPONSE
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_RESPONSEVEC
  /*    */ HT_TRANSIENT_LARGE,   // CN_ROW
  /*102 */ HT_TRANSIENT_MEDIUM,  // CN_ROWAGG
  /*    */ HT_TRANSIENT_LARGE,   // CN_ROWBUFFER
  /*    */ HT_TRANSIENT_LARGE,   // CN_ROWCODER
  /*105 */ HT_TRANSIENT_SMALL,   // CN_ROWONCE
  /*    */ HT_TRANSIENT_LARGE,   // CN_ROWTRAY
  
  /*107 */ HT_TRANSIENT_SMALL,   // CN_SCHEMA
  /*107 */ HT_TRANSIENT_LARGE,   // CN_SCHEMATEST
  /*    */ HT_INVALID,           // CN_SEEME
  /*115 */ HT_TRANSIENT_LARGE,   // CN_SEPROMPT
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_SESSION
  /*    */ HT_TRANSIENT_SMALL,   // CN_SESSIONSTATE
  /*116 */ HT_INVALID,           // CN_SINCE
  /*118 */ HT_TRANSIENT_SMALL,   // CN_SKIPLIST
  /*119 */ HT_TRANSIENT_SMALL,   // CN_SKIPNODE
  /*120 */ HT_TRANSIENT_LARGE,   // CN_SLAB
  /*121 */ HT_TRANSIENT_SMALL,   // CN_SLABALLOCATOR
  /*122 */ HT_INVALID,           // CN_SLOTS
  /*123 */ HT_INVALID,           // SN_SMALLPERSISTENT
  /*124 */ HT_INVALID,           // SN_SMALLTRANSIENT
  /*125 */ HT_INVALID,           // CN_SNBCD
  /*125 */ HT_INVALID,           // CN_SNBOL
  /*125 */ HT_INVALID,           // CN_SNDTE
  /*125 */ HT_INVALID,           // CN_SNFLT
  /*125 */ HT_INVALID,           // CN_SNINT
  /*125 */ HT_INVALID,           // CN_SNIVL
  /*125 */ HT_INVALID,           // CN_SNTME
  /*125 */ HT_INVALID,           // CN_SNTSP
  /*125 */ HT_INVALID,           // CN_SNVCH
  /*125 */ HT_TRANSIENT_SMALL,   // CN_SOCKET
  /*125 */ HT_INVALID,           // CN_SPBCD
  /*125 */ HT_INVALID,           // CN_SPBOL
  /*125 */ HT_INVALID,           // CN_SPDTE
  /*125 */ HT_INVALID,           // CN_SPFLT
  /*125 */ HT_INVALID,           // CN_SPINT
  /*125 */ HT_INVALID,           // CN_SPIVL
  /*125 */ HT_INVALID,           // CN_SPTME
  /*125 */ HT_INVALID,           // CN_SPTSP
  /*125 */ HT_INVALID,           // CN_SPVCH
  /*126 */ HT_TRANSIENT_LARGE,   // CN_STACK
  /*127 */ HT_INVALID,           // CN_STEPACCUM
  /*127 */ HT_TRANSIENT_SMALL,   // CN_STATISTICS
  /*128 */ HT_TRANSIENT_LARGE,   // CN_STEPTIMING
  /* 13 */ HT_INVALID,           // CN_STRING_T
  /*129 */ HT_INVALID,           // CN_SYSCALL
  
  /*130 */ HT_INVALID,           // CN_TABLE
  /*    */ HT_TRANSIENT_SMALL,   // CN_TABLEPTN
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_TABLEPTNVEC
  /*131 */ HT_TRANSIENT_MEDIUM,  // CN_TASK
  /*    */ HT_TRANSIENT_SMALL,   // CN_TASKLIST
  /*    */ HT_TRANSIENT_SMALL,   // CN_TEMPNAME
  /*    */ HT_TRANSIENT_SMALL,   // CN_TENANT
  /*    */ HT_TRANSIENT_SMALL,   // CN_TENANTS
  /*134 */ HT_TRANSIENT_SMALL,   // CN_TASKQUEUE
  /*137 */ HT_TRANSIENT_SMALL,   // CN_THREAD
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_THREADVEC
  /*141 */ HT_INVALID,           // CN_TIME
  /*142 */ HT_TRANSIENT_SMALL,   // CN_TIMER
  /*143 */ HT_TRANSIENT_SMALL,   // CN_TOKEN
  /*144 */ HT_TRANSIENT_MEDIUM,  // CN_TOKENISER
  /*145 */ HT_INVALID,           // CN_TOKENARY
  /*146 */ HT_TRANSIENT_MEDIUM,  // CN_TOKENVEC
  /*147 */ HT_TRANSIENT_SMALL,   // CN_TX
  /*148 */ HT_TRANSIENT_SMALL,   // CN_TXSET
  
  /*149 */ HT_INVALID,           // CN_WILDERNESS
  /*150 */ HT_TRANSIENT_MEDIUM,  // CN_WORD
  
  /*151 */ HT_INVALID,           // CN_ZSMALL_
  /*152 */ HT_INVALID,           // CN_ZMEDIUM_
  /*153 */ HT_INVALID,           // CN_ZLARGE_
  /*154 */ HT_INVALID,           // CN_ZJUMBO_

  /*155 */ HT_INVALID,           // CN_ENUMS
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
