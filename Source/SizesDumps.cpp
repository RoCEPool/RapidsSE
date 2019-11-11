/*============================================================================*/
/* Dump.cpp -- Class Dumps                                                    */
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
  /*  0 */ &Dump::vInvalid,         // CN_NONE
  
  /*    */ &Dump::vInvalid,         // CN_BIRTH
  /*    */ &Dump::vInvalid,         // CN_FIPVAL
  /*    */ &Dump::vInvalid,         // CN_FREQDIST
  /*  7 */ &Dump::vInvalid,         // CN_INTVAL
  /* 11 */ &Dump::vInvalid,         // CN_PTRVAL
  /* 12 */ &Dump::vInvalid,         // CN_STRVAL

  /*  6 */ &Dump::vInvalid,         // CN_FLTVAL
  /*  9 */ &Dump::vInvalid,         // CN_IVLVAL

  /* 19 */ &Dump::vInvalid,         // CN_ACCESSPATH
  /*    */ &Dump::vDummy,           // CN_AGGOP
  /* 22 */ &Dump::vInvalid,         // CN_ARG
  /* 23 */ &Dump::vInvalid,         // CN_ARGUMENTS
  /*    */ &Dump::vUndefined,       // CN_ARTICLE
  /* 24 */ &Dump::vInvalid,         // CN_ATOMIC
  /* 25 */ &AttachMap::vDump,       // CN_ATTACHMAP

  /* 29 */ &Dump::vInvalid,         // CN_BOTHOP
  /* 29 */ &Dump::vInvalid,         // CN_BPA
  /* 29 */ &Dump::vInvalid,         // CN_BUCKETMAP
  /*    */ &Dump::vInvalid,         // CN_BUFFERCURSOR
  /* 28 */ &Dump::vInvalid,         // CN_BUFFERPOOL
  /*    */ &Dump::vInvalid,         // CN_BYTERATE
  /*    */ &Dump::vInvalid,         // CN_BYTES_T

  /*    */ &Dump::vUndefined,       // CN_CALCULATOR
  /*    */ &Dump::vUndefined,       // CN_CANCELMARKER
  /* 30 */ &Dump::vDummy,           // CN_CARRYOVER
  /*    */ &Dump::vDummy,           // CN_CATALOG
  /*    */ &Dump::vDummy,           // CN_CATALOG
  /* 31 */ &Dump::vDummy,           // CN_CLAUSE_FROM
  /* 31 */ &Dump::vDummy,           // CN_CLAUSE_GROUPBY
  /* 31 */ &Dump::vDummy,           // CN_CLAUSE_JOIN
  /* 31 */ &Dump::vDummy,           // CN_CLAUSE_LIMIT
  /* 31 */ &Dump::vDummy,           // CN_CLAUSE_ORDERBY
  /* 31 */ &Dump::vDummy,           // CN_CLAUSE_SELECT
  /* 31 */ &Dump::vDummy,           // CN_CLAUSE_SUBQRY
  /* 31 */ &Dump::vDummy,           // CN_CLAUSE_WHERE
  /* 31 */ &Dump::vDummy,           // CN_CMD_SELECT
  /* 32 */ &Dump::vDummy,           // CN_COLUMNVEC
  /* 32 */ &Dump::vDummy,           // CN_COMBINEOP
  /*    */ &Dump::vDummy,           // CN_COMMAND
  /*    */ &Dump::vDummy,           // CN_COMPILER
  /*    */ &Dump::vDummy,           // CN_COMPLETER
  /*    */ &Dump::vDummy,           // CN_CONSUMEOP
  /*    */ &Dump::vDummy,           // CN_COUNTOP
  /*    */ &Dump::vUndefined,       // CN_CRWBUFFER
  /*    */ &Dump::vUndefined,       // CN_CSVBUFFER

  /*    */ &Dump::vDummy,           // CN_DECACNT
  /*    */ &Dump::vDummy,           // CN_DECIDER
  /*    */ &Dump::vDummy,           // CN_DELAYOP
  /* 33 */ &Dic::vDump,             // CN_DIC
  /*    */ &Dump::vDummy,           // CN_DICPATH
  /* 34 */ &DicStack::vDump,        // CN_DICSTACK
  /* 35 */ &Dump::vInvalid,         // CN_DIR
  /*    */ &Dump::vDummy,           // CN_DISTINCTOP
  /*    */ &Dump::vDummy,           // CN_DTMVAL
  /*    */ &Dump::vDummy,           // CN_DYNAMO

  /*    */ &Dump::vUndefined,       // CN_EARLYMARKER
  /*    */ &Dump::vUndefined,       // CN_ENCODING
  /*    */ &Dump::vUndefined,       // CN_ENDMARKER
  /*    */ &Dump::vDummy,           // CN_ENMSET
  /* 36 */ &Dump::vDummy,           // CN_ENT
  /* 37 */ &Dump::vDummy,           // CN_ENTBUFFER
  /* 64 */ &Dump::vInvalid,         // CN_ENTWRK
  /*    */ &Dump::vDummy,           // CN_ENUMFLAGS
  /*    */ &Dump::vDummy,           // CN_EXPORTBUFFER
  /*    */ &Dump::vDummy,           // CN_EXPROP

  /* 38 */ &Dump::vInvalid,         // CN_FETCHBUFFER
  /* 38 */ &Dump::vInvalid,         // CN_FETCHOP
  /* 39 */ &Dump::vInvalid,         // CN_FIFO
  /* 39 */ &Dump::vInvalid,         // CN_FILE
  /*    */ &Dump::vInvalid,         // CN_FILTEROP
  /* 31 */ &Dump::vDummy,           // CN_FILTER_FETCH
  /* 31 */ &Dump::vDummy,           // CN_FILTER_FROM
  /* 31 */ &Dump::vDummy,           // CN_FILTER_GROUPBY
  /* 31 */ &Dump::vDummy,           // CN_FILTER_INFLATE
  /* 31 */ &Dump::vDummy,           // CN_FILTER_JOIN
  /* 31 */ &Dump::vDummy,           // CN_FILTER_LIMIT
  /* 31 */ &Dump::vDummy,           // CN_FILTER_ORDERBY
  /* 31 */ &Dump::vDummy,           // CN_FILTER_OUTPUT
  /* 31 */ &Dump::vDummy,           // CN_FILTER_PROJECT
  /* 31 */ &Dump::vDummy,           // CN_FILTER_SCAN
  /* 31 */ &Dump::vDummy,           // CN_FILTER_SELECT
  /* 31 */ &Dump::vDummy,           // CN_FILTER_SUBQRY
  /* 31 */ &Dump::vDummy,           // CN_FILTER_WHERE
  /* 31 */ &Dump::vDummy,           // CN_FILTER_ZED
  /* 31 */ &Dump::vDummy,           // CN_FOCALOBJECTS
  /* 31 */ &Dump::vDummy,           // CN_FORTH
  /* 40 */ &Dump::vDummy,           // CN_FORMATTERS
  /* 41 */ &Dump::vDummy,           // CN_FREEBUFFER
  /* 42 */ &Dump::vDummy,           // CN_FREECHUNK
  /* 43 */ &Dump::vDummy,           // CN_FREEDNOT
  /* 44 */ &Dump::vDummy,           // CN_FREEITEM
  
  /*    */ &Dump::vInvalid,         // CN_GENERATEOP
  /*    */ &Dump::vInvalid,         // CN_GENVAR
  /*    */ &Dump::vInvalid,         // CN_GETKEYOP
  /* 48 */ &Grid::vDump,            // CN_GRID
  
  /*    */ &Dump::vDummy,           // CN_HASHBUFFER
  /*    */ &Dump::vDummy,           // CN_HASHMAP
  /*    */ &Dump::vDummy,           // CN_HASHOP

  /* 50 */ &HeapBuffer::vDump,      // CN_HEAPBUFFER
  /* 51 */ &Dump::vDummy,           // CN_HPROW

  /* 51 */ &Dump::vDummy,           // CN_INDEX
  /* 52 */ &Dump::vDummy,           // CN_INDEXTREE
  /* 53 */ &Dump::vDummy,           // CN_INDEXTREEVEC
  /*    */ &Dump::vDummy,           // CN_INFLATEOP
  /*    */ &Dump::vDummy,           // CN_INPUT
  /* 54 */ &Dump::vDummy,           // CN_IDXCOLVEC
  /* 55 */ &Dump::vDummy,           // CN_IDXVEC
  /*    */ &Dump::vDummy,           // CN_INGESTCHUNK
  /*    */ &Dump::vDummy,           // CN_INSERTOP
  /*    */ &Dump::vDummy,           // CN_INTBUFFER
  /*    */ &Dump::vDummy,           // CN_INTERSECTOP
  /*    */ &Dump::vDummy,           // CN_IOCHUNK
  /*    */ &Dump::vDummy,           // CN_IOCOPY
  /*    */ &Dump::vDummy,           // CN_IRWBUFFER

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
  /*    */ &Dump::vDummy,           // CN_JOB_CONNECTCLUSTER
  /*    */ &Dump::vDummy,           // CN_JOB_CONNECTOR
  /*    */ &Dump::vDummy,           // CN_JOB_COPYBUFFER
  /*    */ &Dump::vDummy,           // CN_JOB_CREATEINDEX
  /*    */ &Dump::vDummy,           // CN_JOB_CREATETABLE
  /*    */ &Dump::vDummy,           // CN_JOB_DEFINENODE
  /*    */ &Dump::vDummy,           // CN_JOB_EXPORT
  /*    */ &Dump::vDummy,           // CN_JOB_FETCHROWS
  /*    */ &Dump::vDummy,           // CN_JOB_HEALTH
  /*    */ &Dump::vDummy,           // CN_JOB_INSERTROW
  /*    */ &Dump::vDummy,           // CN_JOB_INSERTTABLE
  /*    */ &Dump::vDummy,           // CN_JOB_KIDEXPORT
  /*    */ &Dump::vDummy,           // CN_JOB_LOADTABLE
  /*    */ &Dump::vDummy,           // CN_JOB_NETHOG
  /*    */ &Dump::vDummy,           // CN_JOB_NODEINFO
  /*    */ &Dump::vDummy,           // CN_JOB_OPERAND
  /*    */ &Dump::vDummy,           // CN_JOB_PERIODIC
  /*    */ &Dump::vDummy,           // CN_JOB_PIPELINE
  /*    */ &Dump::vDummy,           // CN_JOB_PUBLISH
  /*    */ &Dump::vDummy,           // CN_JOB_PURGE
  /*    */ &Dump::vDummy,           // CN_JOB_RECEIVER
  /*    */ &Dump::vDummy,           // CN_JOB_SANITY
  /*    */ &Dump::vDummy,           // CN_JOB_SELECT
  /*    */ &Dump::vDummy,           // CN_JOB_START
  /*    */ &Dump::vDummy,           // CN_JOB_STARTCLUSTER
  /*    */ &Dump::vDummy,           // CN_JOB_EXPORT
  /*    */ &Dump::vDummy,           // CN_JOB_SUBFETCH
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
  /*    */ &Dump::vDummy,           // CN_JOB_UNIVERSAL
  /*    */ &Dump::vDummy,           // CN_JOB_WAITCLUSTER
  /*    */ &Dump::vDummy,           // CN_JOB_WAKER
  /*    */ &Dump::vDummy,           // CN_JOB_WALK
  /*    */ &Dump::vDummy,           // CN_JOB_WRITEBUFFER
  /*    */ &Dump::vDummy,           // CN_JOB_WRITEROW

  /*    */ &JobList::vDump,         // CN_JOBLIST
  /*    */ &Dump::vDummy,           // CN_JOINOP
  /* 59 */ &Heap::vDump,            // CN_JUMBOPERSISTENT
  /* 60 */ &Heap::vDump,            // CN_JUMBOTRANSIENT
  
  /* 61 */ &Dump::vDummy,           // CN_KEY
  
  /* 65 */ &Heap::vDump,            // CN_LARGEPERSISTENT
  /* 66 */ &Heap::vDump,            // CN_LARGETRANSIENT
  /*    */ &Dump::vDummy,           // CN_LEXICON
  /*    */ &Dump::vDummy,           // CN_LIMITOP
  /*    */ &Dump::vDummy,           // CN_LOADCHUNK
  /* 71 */ &LoadMap::vDump,         // CN_LOADMAP
  /*    */ &Dump::vInvalid,         // CN_LOCAL
  /*    */ &LocalChain::vDump,        // CN_LOCALSCHAIN
  /* 72 */ &Dump::vInvalid,         // CN_LOCK
  /*    */ &Dump::vInvalid,         // CN_LOG
  /*    */ &Dump::vInvalid,         // CN_LOGENTRY

  /* 73 */ &Heap::vDump,            // CN_MEDIUMPERSISTENT
  /* 74 */ &Heap::vDump,            // CN_MEDIUMTRANSIENT
  /*    */ &Dump::vUndefined,       // CN_MEMTEST
  /*    */ &Dump::vUndefined,       // CN_MERGEOP
  /*114 */ &Dump::vUndefined,       // CN_MUTEX

  /*    */ &Dump::vUndefined,       // CN_NEXTID
  /*    */ &Dump::vUndefined,       // CN_NMSTR
  /*    */ &Dump::vUndefined,       // CN_NMSTRLIST
  /*    */ &Dump::vUndefined,       // CN_NMVAL
  /* 75 */ &Node::vDump,            // CN_NODE
  /*    */ &Dump::vUndefined,       // CN_NOW
  /*    */ &Dump::vUndefined,       // CN_NUMBAT

  /* 28 */ &Dump::vInvalid,         // CN_OP_TOCLAUSE
  /* 28 */ &Dump::vInvalid,         // CN_OP_TOCOMMAND
  /* 28 */ &Dump::vInvalid,         // CN_OP_TODO
  /* 28 */ &Dump::vInvalid,         // CN_OP_TOFILTER
  /* 28 */ &Dump::vInvalid,         // CN_OPERANDQUEUE
  /* 28 */ &Dump::vInvalid,         // CN_OPSPEC
  /* 28 */ &Dump::vInvalid,         // CN_OPSPECVEC
  /* 28 */ &Dump::vInvalid,         // CN_OPVEC
  /*    */ &Dump::vUndefined,       // CN_OUTPUTOP

  /* 78 */ &Parser::vDump,          // CN_PARSER
  /* 82 */ &Dump::vInvalid,         // CN_PATHVEC
  /* 82 */ &Dump::vInvalid,         // CN_PAUSEMARKER
  /* 82 */ &Dump::vInvalid,         // CN_PERUSE
  /*    */ &Dump::vInvalid,         // CN_PIPE
  /*    */ &Dump::vDummy,           // CN_PIPELINE
  /*    */ &Dump::vDummy,           // CN_PIPELINESPEC
  /* 86 */ &Plan::vDump,            // CN_PLAN
  /* 87 */ &Dump::vUndefined,       // CN_PLANNER
  /* 89 */ &Dump::vUndefined,       // CN_PREDICATE
  /* 90 */ &Dump::vUndefined,       // CN_PREDVEC
  /* 90 */ &Dump::vUndefined,       // CN_PROJECTOP
  /* 91 */ &Dump::vUndefined,       // CN_PSXTHD
  /* 94 */ &Dump::vDummy,           // CN_PTNCOLVEC
  
  /* 95 */ &Queue::vDump,           // CN_QUEUE
  /* 96 */ &QueueBuffer::vDump,     // CN_QUEUEBUFFER
  /*    */ &Dump::vUndefined,       // CN_QUEUES

  /*    */ &Dump::vUndefined,       // CN_RECORD
  /*    */ &Dump::vUndefined,       // CN_RECORDBUFFER
  /*    */ &Dump::vUndefined,       // CN_REDUCEOP
  /*    */ &Dump::vUndefined,       // CN_REFBUFFER
  /*    */ &Dump::vUndefined,       // CN_REFSET
  /*    */ &Dump::vUndefined,       // CN_REGSET
  /* 77 */ &Dump::vUndefined,       // CN_RELOP
  /* 77 */ &Dump::vUndefined,       // CN_REQUEST
  /*    */ &LocalChain::vDump,      // CN_RESOURCETRACKER
  /*    */ &Dump::vUndefined,       // CN_RESPONSE
  /*    */ &Dump::vUndefined,       // CN_RESPONSEVEC
  /*    */ &Dump::vUndefined,       // CN_ROW
  /*102 */ &Dump::vUndefined,       // CN_ROWAGG
  /*    */ &Dump::vUndefined,       // CN_ROWCOMPACTOR
  /*105 */ &Dump::vUndefined,       // CN_ROWONCE
  /*    */ &Dump::vUndefined,       // CN_ROWTRAY
  
  /*    */ &Dump::vUndefined,       // CN_SCANOP
  /*    */ &Dump::vUndefined,       // CN_SCHEMA
  /*    */ &Dump::vUndefined,       // CN_SCHEMATEST
  /*    */ &Dump::vUndefined,       // CN_SEEME
  /*    */ &Dump::vUndefined,       // CN_SEMANTIC
  /*115 */ &SePrompt::vDump,        // CN_SEPROMPT
  /*    */ &Session::vDump,         // CN_SESSION
  /*    */ &Dump::vUndefined,       // CN_SESSIONARRAY
  /*116 */ &Dump::vUndefined,       // CN_SHORTCUT
  /*116 */ &Dump::vUndefined,       // CN_SHORTCUTVEC
  /*116 */ &Dump::vUndefined,       // CN_SINCE
  /*118 */ &Dump::vUndefined,       // CN_SKIPLIST
  /*119 */ &Dump::vUndefined,       // CN_SKIPNODE
  /*120 */ &Dump::vUndefined,       // CN_SLAB
  /*121 */ &Dump::vUndefined,       // CN_SLABALLOCATOR
  /*    */ &Dump::vUndefined,       // CN_SLEEP
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
  /*    */ &Dump::vInvalid,         // CN_SORTOP
  /*    */ &Dump::vInvalid,         // CN_SPBCD
  /*    */ &Dump::vInvalid,         // CN_SPBOL
  /*    */ &Dump::vInvalid,         // CN_SPDTE
  /*    */ &Dump::vInvalid,         // CN_SPELAPSED
  /*    */ &Dump::vInvalid,         // CN_SPFLT
  /*    */ &Dump::vInvalid,         // CN_SPID
  /*    */ &Dump::vInvalid,         // CN_SPINT
  /*    */ &Dump::vInvalid,         // CN_SPIVL
  /*    */ &Dump::vInvalid,         // CN_SPSTRING
  /*    */ &Dump::vInvalid,         // CN_SPTKNVECSTR
  /*    */ &Dump::vInvalid,         // CN_SPTME
  /*    */ &Dump::vInvalid,         // CN_SPTSP
  /*    */ &Dump::vInvalid,         // CN_SPVCH
  /*    */ &Dump::vInvalid,         // CN_SRTBUFFER
  /*    */ &Dump::vInvalid,         // CN_SRTSET
  /*126 */ &Stack::vDump,           // CN_STACK
  /*126 */ &Dump::vInvalid,         // CN_STAGE
  /*126 */ &Dump::vInvalid,         // CN_STAGENDE
  /*126 */ &Dump::vInvalid,         // CN_STAGENDEVEC
  /*126 */ &Dump::vInvalid,         // CN_STAGEPTN
  /*126 */ &Dump::vInvalid,         // CN_STAGEPTNVEC
  /*126 */ &Dump::vInvalid,         // CN_STAGESPEC
  /*126 */ &Dump::vInvalid,         // CN_STAGESPECVEC
  /*126 */ &Dump::vInvalid,         // CN_STAGEVEC
  /*    */ &Dump::vInvalid,         // CN_STARTMARKER
  /*    */ &Dump::vInvalid,         // CN_STARTOP
  /*    */ &Dump::vInvalid,         // CN_STATISTICS
  /*127 */ &Dump::vInvalid,         // CN_STEPACCUM
  /*128 */ &StepTiming::vDump,      // CN_STEPTIMING
  /* 13 */ &Dump::vInvalid,         // CN_STMT_SELECT
  /* 13 */ &Dump::vInvalid,         // CN_STREAMDEF
  /* 13 */ &Dump::vInvalid,         // CN_STRING_T
  /*    */ &Dump::vInvalid,         // CN_SUPERAGGOP
  /*    */ &Dump::vInvalid,         // CN_SUPERCOUNTOP
  /*    */ &Dump::vInvalid,         // CN_SUPERMERGEOP
  /*129 */ &Dump::vInvalid,         // CN_SYSCALL

  /*130 */ &Table::vDump,           // CN_TABLE
  /*    */ &Dump::vDummy,           // CN_TABLEDEF
  /*    */ &TablePtn::vDump,        // CN_TABLEPTN
  /*    */ &Dump::vDummy,           // CN_TABLEPTNVEC
  /*    */ &Dump::vDummy,           // CN_TAKEAWAYOP
  /*131 */ &Task::vDump,            // CN_TASK
  /*    */ &Dump::vUndefined,       // CN_TASKLIST
  /*134 */ &Dump::vUndefined,       // CN_TASKQUEUE
  /*    */ &Dump::vUndefined,       // CN_TEMPNAME
  /*    */ &Dump::vUndefined,       // CN_TENANT
  /*    */ &Dump::vUndefined,       // CN_TENANTS
  /*137 */ &Dump::vUndefined,       // CN_THREAD
  /*137 */ &Dump::vUndefined,       // CN_THREADQUEUE
  /*    */ &Dump::vUndefined,       // CN_THREADVEC
  /*142 */ &Dump::vInvalid,         // CN_TIMEMARKER
  /*142 */ &Dump::vInvalid,         // CN_TIMER
  /*    */ &Dump::vInvalid,         // CN_TOCSVOP
  /*    */ &Dump::vInvalid,         // CN_TOFILEOP
  /*143 */ &Token::vDump,           // CN_TOKEN
  /*144 */ &Tokeniser::vDump,       // CN_TOKENISER
  /*146 */ &Dump::vInvalid,         // CN_TOKENVEC
  /*147 */ &Tx::vDump,              // CN_TX
  /*148 */ &Dump::vUndefined,       // CN_TXSET

  /* 39 */ &Dump::vInvalid,         // CN_WHEREOP
  /*149 */ &Dump::vInvalid,         // CN_WILDERNESS
  /*150 */ &Word::vDump,       // CN_WORD

  /*148 */ &Dump::vUndefined,       // CN_XMITOP

  /*116 */ &Dump::vUndefined,       // CN_ZEDOP
  
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
  
  /*    */ sizeof(Birth),              // CN_BIRTH
  /*    */ sizeof(FipVal),             // CN_FIPVAL
  /*  7 */ sizeof(FreqDist),           // CN_FREQDIST
  /*  7 */ sizeof(IntVal),             // CN_INTVAL
  /* 11 */ sizeof(PtrVal),             // CN_PTRVAL
  /* 12 */ sizeof(StrVal),             // CN_STRVAL
 
  /*  6 */ 0,                          // CN_FLTVAL
  /*  9 */ 0,                          // CN_IVLVAL

  /* 19 */ sizeof(AccessPath),         // CN_ACCESSPATH
  /*    */ sizeof(AggOp),              // CN_AGGOP
  /* 22 */ sizeof(Arg),                // CN_ARG
  /* 23 */ sizeof(Arguments),          // CN_ARGUMENTS
  /*    */ sizeof(Article),            // CN_ARTICLE
  /* 24 */ sizeof(Atomic),             // CN_ATOMIC
  /* 25 */ sizeof(AttachMap),          // CN_ATTACHMAP

  /* 29 */ sizeof(BothOp),             // CN_BOTHOP
  /* 29 */ sizeof(BPA),                // CN_BPA
  /* 29 */ sizeof(BucketMap),          // CN_BUCKETMAP
  /*    */ sizeof(BufferCursor),       // CN_BUFFERCURSOR
  /* 28 */ sizeof(BufferPool),         // CN_BUFFERPOOL
  /* 28 */ sizeof(ByteRate),           // CN_BYTERATE
  /* 28 */ sizeof(s32),                // CN_BYTES_T

  /*    */ sizeof(Calculator),         // CN_CALCULATOR
  /*    */ sizeof(CancelMarker),       // CN_CANCELMARKER
  /* 30 */ sizeof(CarryOver),          // CN_CARRYOVER
  /*    */ sizeof(Catalog),            // CN_CATALOG
  /*    */ sizeof(Clause_From),        // CN_CLAUSE_FROM
  /*    */ sizeof(Clause_GroupBy),     // CN_CLAUSE_GROUPBY
  /*    */ sizeof(Clause_Join),        // CN_CLAUSE_JOIN
  /*    */ sizeof(Clause_Limit),       // CN_CLAUSE_LIMIT
  /*    */ sizeof(Clause_OrderBy),     // CN_CLAUSE_ORDERBY
  /*    */ sizeof(Clause_Select),      // CN_CLAUSE_SELECT
  /*    */ sizeof(Clause_SubQry),      // CN_CLAUSE_SUBQRY
  /*    */ sizeof(Clause_Where),       // CN_CLAUSE_WHERE
  /*    */ sizeof(Cmd_Select),         // CN_CMD_SELECT
  /* 31 */ sizeof(Column),             // CN_COLUMN
  /* 32 */ sizeof(ColumnVec),          // CN_COLUMNVEC
  /* 32 */ sizeof(CombineOp),          // CN_COMBINEOP
  /* 32 */ sizeof(Request),            // CN_COMMAND
  /* 32 */ sizeof(Compiler),           // CN_COMPILER
  /* 32 */ sizeof(Completer),          // CN_COMPLETER
  /* 32 */ sizeof(Completer),          // CN_CONSUMEOP
  /* 32 */ sizeof(CountOp),            // CN_COUNTOP
  /*    */ sizeof(CrwBuffer),          // CN_CRWBUFFER
  /*    */ sizeof(CsvBuffer),          // CN_CSVBUFFER

  /*    */ sizeof(DecaCnt),            // CN_DECACNT
  /*    */ sizeof(Decider),            // CN_DECIDER
  /*    */ sizeof(DelayOp),            // CN_DELAYOP
  /* 33 */ sizeof(Dic),                // CN_DIC
  /* 34 */ sizeof(DicPath),            // CN_DICPATH
  /* 34 */ sizeof(DicStack),           // CN_DICSTACK
  /* 35 */ sizeof(Dir),                // CN_DIR
  /* 35 */ sizeof(DistinctOp),         // CN_DISTINCTOP
  /*    */ sizeof(DtmVal),             // CN_DTMVAL
  /*    */ sizeof(Dynamo),             // CN_DYNAMO

  /*    */ sizeof(EarlyMarker),        // CN_EARLYMARKER
  /*    */ sizeof(Encoding),           // CN_ENCODING
  /*    */ sizeof(EndMarker),          // CN_ENDMARKER
  /*    */ sizeof(EnameSet),             // CN_ENMSET
  /* 36 */ sizeof(Ent),                // CN_ENT
  /* 37 */ sizeof(EntBuffer),          // CN_ENTBUFFER
  /* 64 */ sizeof(EntWrk),             // CN_ENTWRK
  /*    */ sizeof(EnumFlags),          // CN_ENUMFLAGS
  /* 32 */ sizeof(ExportBuffer),       // CN_EXPORTBUFFER
  /* 32 */ sizeof(ExprOp),             // CN_EXPROP

  /*    */ sizeof(FetchBuffer),        // CN_FETCHBUFFER
  /*    */ sizeof(FetchOp),            // CN_FETCHOP
  /* 39 */ sizeof(Fifo),               // CN_FIFO
  /* 39 */ sizeof(File),               // CN_FILE
  /*    */ sizeof(FilterOp),           // CN_FILTEROP
  /*    */ sizeof(Filter_Fetch),       // CN_FILTER_FETCH
  /*    */ sizeof(Filter_From),        // CN_FILTER_FROM
  /*    */ sizeof(Filter_GroupBy),     // CN_FILTER_GROUPBY
  /*    */ sizeof(Filter_Inflate),     // CN_FILTER_INFLATE
  /*    */ sizeof(Filter_Join),        // CN_FILTER_JOIN
  /*    */ sizeof(Filter_Limit),       // CN_FILTER_LIMIT
  /*    */ sizeof(Filter_OrderBy),     // CN_FILTER_ORDERBY
  /*    */ sizeof(Filter_Output),      // CN_FILTER_OUTPUT
  /*    */ sizeof(Filter_Project),     // CN_FILTER_PROJECT
  /*    */ sizeof(Filter_Scan),        // CN_FILTER_SCAN
  /*    */ sizeof(Filter_Select),      // CN_FILTER_SELECT
  /*    */ sizeof(Filter_SubQry),      // CN_FILTER_SUBQRY
  /*    */ sizeof(Filter_Where),       // CN_FILTER_WHERE
  /*    */ sizeof(Filter_Zed),         // CN_FILTER_ZED
  /*    */ sizeof(FocalObjects),       // CN_FOCALOBJECTS
  /*    */ sizeof(Forth),         // CN_FORTH
  /* 40 */ sizeof(Formatters),         // CN_FORMATTERS
  /* 41 */ sizeof(PoolBuffer),         // CN_FREEBUFFER
  /* 42 */ sizeof(FreeChunk),          // CN_FREECHUNK
  /* 43 */ 0,                          // CN_FREEDNOT
  /* 44 */ 0,                          // CN_FREEITEM
  
  /*    */ sizeof(GenerateOp),         // CN_GENERATEOP
  /*    */ sizeof(GenVar),             // CN_GENVAR
  /*    */ sizeof(GetKeyOp),           // CN_GETKEYOP
  /* 48 */ sizeof(Grid),               // CN_GRID
  
  /*    */ sizeof(HashBuffer),         // CN_HASHBUFFER
  /*    */ sizeof(HashMap),            // CN_HASHMAP
  /*    */ sizeof(HashOp),             // CN_HASHOP
  /* 50 */ sizeof(HeapBuffer),         // CN_HEAPBUFFER
  /* 50 */ sizeof(SP_Row),             // CN_HPROW

  /* 51 */ sizeof(Index),              // CN_INDEX
  /* 52 */ sizeof(IndexTree),          // CN_INDEXTREE
  /* 53 */ sizeof(IndexTreeVec),       // CN_INDEXTREEVEC
  /*    */ sizeof(InflateOp),          // CN_INFLATEOP
  /*    */ sizeof(Input),              // CN_INPUT
  /* 54 */ sizeof(IdxColVec),          // CN_IDXCOLVEC
  /* 55 */ sizeof(IdxVec),             // CN_IDXVEC
  /*    */ sizeof(IngestChunk),        // CN_INGESTCHUNK
  /*    */ sizeof(InsertOp),           // CN_INSERTOP
  /*    */ sizeof(IntBuffer),          // CN_INTBUFFER
  /*    */ sizeof(IntersectOp),        // CN_INTERSECTOP
  /*    */ sizeof(IoChunk),            // CN_IOCHUNK
  /*    */ sizeof(IoCopy),             // CN_IOCOPY
  /*    */ sizeof(IrwBuffer),          // CN_IRWBUFFER

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
  /*    */ sizeof(Job_ConnectCluster), // CN_JOB_CONNECTCLUSTER
  /*    */ sizeof(Job_Connector),      // CN_JOB_CONNECTOR
  /*    */ sizeof(Job_CopyBuffer),     // CN_JOB_COPYBUFFER
  /*    */ sizeof(Job_CreateIndex),    // CN_JOB_CREATEINDEX
  /*    */ sizeof(Job_CreateTable),    // CN_JOB_CREATETABLE
  /*    */ sizeof(Job_DefineNode),     // CN_JOB_DEFINENODE
  /*    */ sizeof(Job_Export),         // CN_JOB_EXPORT
  /*    */ sizeof(Job_FetchRows),      // CN_JOB_FETCHROWS
  /*    */ sizeof(Job_Health),         // CN_JOB_HEALTH
  /*    */ sizeof(Job_InsertRow),      // CN_JOB_INSERTROW
  /*    */ sizeof(Job_InsertTable),    // CN_JOB_INSERTTABLE
  /*    */ sizeof(Job_KidExport),      // CN_JOB_KIDEXPORT
  /*    */ sizeof(Job_LoadTable),      // CN_JOB_LOADTABLE
  /*    */ sizeof(Job_NetHog),         // CN_JOB_NETHOG
  /*    */ sizeof(Job_NodeInfo),       // CN_JOB_NODEINFO
  /*    */ sizeof(Job_Operand),        // CN_JOB_OPERAND
  /*    */ sizeof(Job_Periodic),       // CN_JOB_PERIODIC
  /*    */ sizeof(Job_Pipeline),       // CN_JOB_PIPELINE
  /*    */ sizeof(Job_Publish),        // CN_JOB_PUBLISH
  /*    */ sizeof(Job_Purge),          // CN_JOB_PURGE
  /*    */ sizeof(Job_Receiver),       // CN_JOB_RECEIVER
  /*    */ sizeof(Job_Sanity),         // CN_JOB_SANITY
  /*    */ sizeof(Job_Select),         // CN_JOB_SELECT
  /*    */ sizeof(Job_Start),          // CN_JOB_START
  /*    */ sizeof(Job_StartCluster),   // CN_JOB_STARTCLUSTER
  /*    */ sizeof(Job_SubExporter),    // CN_JOB_SUBEXPORTER
  /*    */ sizeof(Job_SubFetch),       // CN_JOB_SUBFETCH
  /*    */ sizeof(Job_SubFetch),     // CN_JOB_SUBINFLATE
  /*    */ sizeof(Job_SubFetch),    // CN_JOB_SUBINGESTER
  /*    */ sizeof(Job_SubInserter),    // CN_JOB_SUBINSERTER
  /*    */ sizeof(Job_SubLoader),      // CN_JOB_SUBLOADER
  /*    */ sizeof(Job_SubPublish),     // CN_JOB_SUBPUBLISH
  /*    */ sizeof(Job_SubPurger),      // CN_JOB_SUBPURGER
  /*    */ sizeof(Job_SubScan),        // CN_JOB_SUBSCAN
  /*    */ sizeof(Job_SubTruncate),    // CN_JOB_SUBTRUNCATE
  /*    */ sizeof(Job_SubWalker),      // CN_JOB_SUBWALKER
  /*    */ sizeof(Job_Supervisor),     // CN_JOB_SUPERVISOR
  /*    */ sizeof(Job_Universal),      // CN_JOB_UNIVERSAL
  /*    */ sizeof(Job_WaitCluster),    // CN_JOB_WAITCLUSTER
  /*    */ sizeof(Job_Waker),          // CN_JOB_WAKER
  /*    */ sizeof(Job_Walk),           // CN_JOB_WALK
  /*    */ sizeof(Job_WriteBuffer),    // CN_JOB_WRITEBUFFER
  /*    */ sizeof(Job_WriteRow),       // CN_JOB_WRITEROW
  
  /*    */ sizeof(JobList),            // CN_JOBLIST
  /*    */ sizeof(JoinOp),             // CN_JOINOP
  /* 59 */ sizeof(Heap), // correct    // CN_JUMBOPERSISTENT
  /* 60 */ sizeof(Heap), // correct    // CN_JUMBOTRANSIENT
  
  /* 61 */ sizeof(Key),                // CN_KEY
  
  /* 65 */ sizeof(Heap), // correct    // CN_LARGEPERSISTENT
  /* 66 */ sizeof(Heap), // correct    // CN_LARGETRANSIENT
  /* 66 */ sizeof(Lexicon),            // CN_LEXICON
  /* 66 */ sizeof(LimitOp),            // CN_LIMITOP
  /*    */ sizeof(LoadChunk),          // CN_LOADCHUNK
  /* 71 */ sizeof(LoadMap),            // CN_LOADMAP
  /*    */ sizeof(Local),              // CN_LOCAL
  /*    */ sizeof(LocalChain),           // CN_LOCALSCHAIN
  /* 72 */ sizeof(Lock),               // CN_LOCK
  /*    */ sizeof(Log),                // CN_LOG
  /*    */ sizeof(LogEntry),           // CN_LOGENTRY

  /* 73 */ sizeof(Heap), // correct    // CN_MEDIUMPERSISTENT
  /* 74 */ sizeof(Heap), // correct    // CN_MEDIUMTRANSIENT
  /*114 */ sizeof(MemTest),            // CN_MEMTEST
  /*    */ sizeof(MergeOp),            // CN_MERGEOP
  /*114 */ sizeof(Mutex),              // CN_MUTEX

  /*    */ sizeof(NextId),             // CN_NEXTID
  /*    */ sizeof(NmStr),              // CN_NMSTR
  /*    */ sizeof(NmStrList),          // CN_NMSTRLIST
  /*    */ sizeof(NmVal),              // CN_NMVAL
  /* 75 */ sizeof(Node),               // CN_NODE
  /*    */ sizeof(Now),                // CN_NOW
  /*    */ sizeof(Numbat),             // CN_NUMBAT

  /* 28 */ sizeof(Op_ToClause),        // CN_OP_TOCLAUSE
  /* 28 */ sizeof(Op_ToCommand),       // CN_OP_TOCOMMAND
  /* 28 */ sizeof(Op_ToDo),            // CN_OP_TODO
  /* 28 */ sizeof(Op_ToFilter),        // CN_OP_TOFILTER
  /* 28 */ sizeof(OperandQueue),       // CN_OPERANDQUEUE
  /* 28 */ sizeof(OpSpec),             // CN_OPSPEC
  /* 28 */ sizeof(OpSpecVec),          // CN_OPSPECVEC
  /* 28 */ sizeof(OpVec),              // CN_OPVEC
  /* 77 */ sizeof(OutputOp),           // CN_OUTPUTOP

  /* 78 */ sizeof(Parser),             // CN_PARSER
  /* 82 */ sizeof(PathVec),            // CN_PATHVEC
  /*    */ sizeof(PauseMarker),        // CN_PAUSEMARKER
  /*    */ sizeof(Peruse),             // CN_PERUSE
  /*    */ sizeof(Fifo),               // CN_PIPE
  /* 34 */ sizeof(Pipeline),           // CN_PIPELINE
  /*    */ sizeof(PipelineSpec),        // CN_PIPELINESPEC
  /* 86 */ sizeof(Plan),               // CN_PLAN
  /* 87 */ sizeof(Planner),            // CN_PLANNER
  /* 90 */ sizeof(Predicate),          // CN_PREDICATE
  /* 91 */ sizeof(PredicateVec),       // CN_PREDICATEVEC
  /* 91 */ sizeof(ProjectOp),          // CN_PROJECTOP
  /* 89 */ sizeof(PsxThd),            // CN_PSXTHD
  /* 94 */ sizeof(PtnColVec),          // CN_PTNCOLVEC
  
  /* 95 */ sizeof(Queue),              // CN_QUEUE
  /* 96 */ sizeof(QueueBuffer),        // CN_QUEUEBUFFER
  /*    */ sizeof(Queues),             // CN_QUEUES
  
  /*    */ sizeof(Record),             // CN_RECORD
  /*    */ sizeof(RecordBuffer),       // CN_RECORDBUFFER
  /*    */ sizeof(ReduceOp),           // CN_REDUCEOP
  /*    */ sizeof(RefBuffer),          // CN_REFBUFFER
  /*    */ sizeof(RefSet),             // CN_REFSET
  /*    */ sizeof(RegSet),             // CN_REGSET
  /* 77 */ sizeof(RelOp),              // CN_RELOP
  /* 77 */ sizeof(Request),            // CN_REQUEST
  /*    */ sizeof(ResourceTracker),    // CN_RESOURCETRACKER
  /*    */ sizeof(Response),           // CN_RESPONSE
  /*    */ sizeof(ResponseVec),        // CN_RESPONSEVEC
  /*    */ sizeof(Row),                // CN_ROW
  /*102 */ sizeof(RowAgg),             // CN_ROWAGG
  /*    */ sizeof(RowCompactor),       // CN_ROWCOMPACTOR
  /*105 */ sizeof(RowOnce),            // CN_ROWONCE
  /*105 */ sizeof(RowTray),            // CN_ROWTRAY
  
  /*    */ sizeof(ScanOp),             // CN_SCANOP
  /*    */ sizeof(Schema),             // CN_SCHEMA
  /*    */ sizeof(SchemaTest),         // CN_SCHEMATEST
  /*    */ sizeof(SeeMe),              // CN_SEEME
  /*    */ sizeof(Semantic),           // CN_SEMANTIC
  /*115 */ sizeof(SePrompt),           // CN_SEPROMPT
  /*    */ sizeof(Session),            // CN_SESSION
  /*    */ sizeof(SessionArray),       // CN_SESSIONARRAY
  /*116 */ sizeof(ShortCut),           // CN_SHORTCUT
  /*116 */ sizeof(ShortCutVec),        // CN_SHORTCUTVEC
  /*116 */ sizeof(Birth),              // CN_SINCE
  /*118 */ sizeof(SkipListNL),         // CN_SKIPLIST
  /*119 */ sizeof(SkipNode),           // CN_SKIPNODE
  /*120 */ sizeof(Slab),               // CN_SLAB
  /*121 */ sizeof(SlabAllocator),      // CN_SLABALLOCATOR
  /*    */ sizeof(Sleep),              // CN_SLEEP
  /*122 */ sizeof(Slots),              // CN_SLOTS
  /*123 */ sizeof(Heap), // correct    // SN_SMALLPERSISTENT
  /*124 */ sizeof(Heap), // correct    // SN_SMALLTRANSIENT
  /*    */ sizeof(SN_Bcd),             // CN_SNBCD
  /*    */ sizeof(SN_Bol),             // CN_SNBOL
  /*    */ sizeof(SN_Dte),             // CN_SNDTE
  /*    */ sizeof(SN_Flt),             // CN_SNFLT
  /*    */ sizeof(SN_Int),             // CN_SNINT
  /*    */ sizeof(SN_Ivl),             // CN_SNIVL
  /*    */ sizeof(SN_Tme),             // CN_SNTME
  /*    */ sizeof(SN_Tsp),             // CN_SNTSP
  /*    */ sizeof(SN_Vch),             // CN_SNVCH
  /*125 */ sizeof(Socket),             // CN_SOCKET
  /*125 */ sizeof(SortOp),             // CN_SORTOP
  /*    */ sizeof(SP_Bcd),             // CN_SPBCD
  /*    */ sizeof(SP_Bol),             // CN_SPBOL
  /*    */ sizeof(SP_Dte),             // CN_SPDTE
  /*    */ sizeof(SP_Elapsed),         // CN_SPELAPSED
  /*    */ sizeof(SP_Flt),             // CN_SPFLT
  /*    */ sizeof(SP_Id),              // CN_SPID
  /*    */ sizeof(SP_Int),             // CN_SPINT
  /*    */ sizeof(SP_Ivl),             // CN_SPIVL
  /*    */ sizeof(SP_String),          // CN_SPSTRING
  /*    */ sizeof(SP_TknVecStr),       // CN_SPTKNVECSTR
  /*    */ sizeof(SP_Tme),             // CN_SPTME
  /*    */ sizeof(SP_Tsp),             // CN_SPTSP
  /*    */ sizeof(SP_Vch),             // CN_SPVCH
  /*    */ sizeof(SrtBuffer),          // CN_SRTBUFFER
  /*    */ sizeof(SrtSet),             // CN_SRTSET
  /*126 */ sizeof(Stack),              // CN_STACK
  /*126 */ sizeof(Stage),              // CN_STAGE
  /*126 */ sizeof(StageNde),           // CN_STAGENDE
  /*126 */ sizeof(StageNdeVec),        // CN_STAGENDEVEC
  /*126 */ sizeof(StagePtn),           // CN_STAGEPTN
  /*126 */ sizeof(StagePtnVec),        // CN_STAGEPTNVEC
  /*126 */ sizeof(StageSpec),          // CN_STAGESPEC
  /*126 */ sizeof(StageSpecVec),       // CN_STAGESPECVEC
  /*126 */ sizeof(StageVec),           // CN_STAGEVEC
  /*    */ sizeof(StartMarker),        // CN_STARTMARKER
  /*    */ sizeof(StartOp),            // CN_STARTOP
  /*    */ sizeof(Statistics),         // CN_STATISTICS
  /*127 */ sizeof(StepAccum),          // CN_STEPACCUM
  /*128 */ sizeof(StepTiming),         // CN_STEPTIMING
  /*128 */ sizeof(Stmt_Select),        // CN_STMT_SELECT
  /* 13 */ sizeof(StreamDef),          // CN_STREAMDEF
  /* 13 */ sizeof(s32),                // CN_STRING_T
  /*    */ sizeof(SuperAggOp),         // CN_SUPERAGGOP
  /*    */ sizeof(SuperCountOp),       // CN_SUPERCOUNTOP
  /*    */ sizeof(SuperMergeOp),       // CN_SUPERMERGEOP
  /*129 */ sizeof(SysCall),            // CN_SYSCALL
  
  /*130 */ sizeof(Table),              // CN_TABLE
  /*130 */ sizeof(TableDef),           // CN_TABLEDEF
  /*    */ sizeof(TablePtn),           // CN_TABLEPTN
  /*    */ sizeof(TablePtnVec),        // CN_TABLEPTNVEC
  /*    */ sizeof(TakeawayOp),         // CN_TAKEAWAYOP
  /*131 */ sizeof(Task),               // CN_TASK
  /*    */ sizeof(TaskList),           // CN_TASKLIST
  /*134 */ sizeof(TaskQueue),          // CN_TASKQUEUE
  /*134 */ sizeof(TempName),           // CN_TEMPNAME
  /*    */ sizeof(Tenant),             // CN_TENANT
  /*    */ sizeof(Tenants),            // CN_TENANTS
  /*137 */ sizeof(Thread),             // CN_THREAD
  /*137 */ sizeof(ThreadQueue),        // CN_THREADQUEUE
  /*    */ sizeof(ThreadVec),          // CN_THREADVEC
  /*142 */ sizeof(TimeMarker),         // CN_TIMEMARKER
  /*142 */ sizeof(Timer),              // CN_TIMER
  /*142 */ sizeof(ToCsvOp),            // CN_TOCSVOP
  /*142 */ sizeof(ToFileOp),           // CN_TOFILEOP
  /*143 */ sizeof(Token),              // CN_TOKEN
  /*144 */ sizeof(Tokeniser),          // CN_TOKENISER
  /*146 */ sizeof(TokenVec),           // CN_TOKENVEC
  /*147 */ sizeof(Tx),                 // CN_TX
  /*148 */ sizeof(TxSet),              // CN_TXSET

  /*    */ sizeof(WhereOp),            // CN_WHEREOP
  /*149 */ sizeof(Wilderness),         // CN_WILDERNESS
  /*150 */ sizeof(Word),          // CN_WORD
  
  /*150 */ sizeof(XmitOp),             // CN_XMITOP
  
  /*116 */ sizeof(ZedOp),              // CN_ZEDOP
  
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
  
  /*  8 */ HT_INVALID,           // CN_BIRTH
  /*  8 */ HT_INVALID,           // CN_FIPVAL
  /*  8 */ HT_TRANSIENT_SMALL,   // CN_FREQDIST
  /*  7 */ HT_INVALID,           // CN_INTVAL
  /* 11 */ HT_INVALID,           // CN_PTRVAL
  /* 12 */ HT_INVALID,           // CN_STRVAL

  /*  6 */ HT_INVALID,           // CN_FLTVAL
  /*  9 */ HT_INVALID,           // CN_IVLVAL

  /* 19 */ HT_TRANSIENT_MEDIUM,  // CN_ACCESSPATH
  /*    */ HT_TRANSIENT_SMALL,   // CN_AGGOP
  /* 22 */ HT_TRANSIENT_SMALL,   // CN_ARG
  /* 23 */ HT_INVALID,           // CN_ARGUMENTS
  /*    */ HT_TRANSIENT_SMALL,   // CN_ARTICLE
  /* 24 */ HT_INVALID,           // CN_ATOMIC
  /* 25 */ HT_INVALID,           // CN_ATTACHMAP

  /* 29 */ HT_TRANSIENT_SMALL,   // CN_BOTHOP
  /* 29 */ HT_INVALID,           // CN_BPA
  /* 29 */ HT_TRANSIENT_LARGE,   // CN_BUCKETMAP
  /*    */ HT_TRANSIENT_LARGE,   // CN_BUFFERCURSOR
  /* 28 */ HT_INVALID,           // CN_BUFFERPOOL
  /* 29 */ HT_TRANSIENT_SMALL,   // CN_BYTERATE
  /* 13 */ HT_INVALID,           // CN_BYTES_T

  /*    */ HT_TRANSIENT_MEDIUM,  // CN_CALCULATOR
  /*    */ HT_TRANSIENT_SMALL,   // CN_CANCELMARKER
  /* 30 */ HT_INVALID,           // CN_CARRYOVER
  /*    */ HT_INVALID,           // CN_CATALOG
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_CLAUSE_FROM
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_CLAUSE_GROUPBY
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_CLAUSE_JOIN
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_CLAUSE_LIMIT
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_CLAUSE_ORDERBY
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_CLAUSE_SELECT
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_CLAUSE_SUBQRY
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_CLAUSE_WHERE
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_CMD_SELECT
  /*    */ HT_TRANSIENT_SMALL,   // CN_COLUMN
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_COLUMNVEC
  /*    */ HT_TRANSIENT_SMALL,   // CN_COMBINEOP
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_COMMAND
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_COMPILER
  /*    */ HT_TRANSIENT_SMALL,   // CN_COMPLETER
  /*    */ HT_TRANSIENT_SMALL,   // CN_CONSUMEOP
  /*    */ HT_TRANSIENT_SMALL,   // CN_COUNTOP
  /*    */ HT_TRANSIENT_LARGE,   // CN_CRWBUFFER
  /*    */ HT_TRANSIENT_LARGE,   // CN_CSVBUFFER

  /*    */ HT_TRANSIENT_SMALL,   // CN_DECACNT
  /*    */ HT_TRANSIENT_SMALL,   // CN_DECIDER
  /*    */ HT_TRANSIENT_SMALL,   // CN_DELAYOP
  /* 33 */ HT_TRANSIENT_MEDIUM,  // CN_DIC
  /*    */ HT_TRANSIENT_SMALL,   // CN_DICPATH
  /* 34 */ HT_TRANSIENT_SMALL,   // CN_DICSTACK
  /* 35 */ HT_TRANSIENT_SMALL,   // CN_DIR
  /* 35 */ HT_TRANSIENT_SMALL,   // CN_DISTINCTOP
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_DTMVAL
  /* 35 */ HT_TRANSIENT_SMALL,   // CN_DYNAMO

  /*    */ HT_TRANSIENT_SMALL,   // CN_EARLYMARKER
  /*    */ HT_TRANSIENT_LARGE,   // CN_ENCODING
  /*    */ HT_TRANSIENT_SMALL,   // CN_ENDMARKER
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_ENMSET
  /* 36 */ HT_INVALID,           // CN_ENT
  /* 37 */ HT_TRANSIENT_LARGE,   // CN_ENTBUFFER
  /* 64 */ HT_INVALID,           // CN_ENTWRK
  /*    */ HT_TRANSIENT_SMALL,   // CN_ENUMFLAGS
  /*    */ HT_TRANSIENT_LARGE,   // CN_EXPORTBUFFER
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_EXPROP

  /* 38 */ HT_TRANSIENT_LARGE,   // CN_FETCHBUFFER
  /* 38 */ HT_TRANSIENT_SMALL,   // CN_FETCHOP
  /* 39 */ HT_TRANSIENT_SMALL,   // CN_FIFO
  /* 39 */ HT_TRANSIENT_SMALL,   // CN_FILE
  /*    */ HT_TRANSIENT_SMALL,   // CN_FILTEROP
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_FETCH
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_FROM
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_GROUPBY
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_INFLATE
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_JOIN
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_LIMIT
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_ORDERBY
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_OUTPUT
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_PROJECT
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_SCAN
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_SELECT
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_SUBQRY
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_WHERE
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_FILTER_ZED
  /* 40 */ HT_TRANSIENT_LARGE,   // CN_FOCALOBJECTS
  /* 40 */ HT_TRANSIENT_SMALL,   // CN_FORTH
  /* 40 */ HT_TRANSIENT_SMALL,   // CN_FORMATTERS
  /* 41 */ HT_INVALID,           // CN_FREEBUFFER
  /* 42 */ HT_INVALID,           // CN_FREECHUNK
  /* 43 */ HT_INVALID,           // CN_FREEDNOT
  /* 44 */ HT_INVALID,           // CN_FREEITEM
  
  /*    */ HT_TRANSIENT_SMALL,   // CN_GENERATEOP
  /*    */ HT_INVALID,           // CN_GENVAR
  /*    */ HT_TRANSIENT_SMALL,   // CN_GETKEYOP
  /* 48 */ HT_TRANSIENT_LARGE,   // CN_GRID

  /*    */ HT_TRANSIENT_LARGE,   // CN_HASHBUFFER
  /*    */ HT_TRANSIENT_SMALL,   // CN_HASHMAP
  /*    */ HT_TRANSIENT_SMALL,   // CN_HASHOP
  /* 50 */ HT_INVALID,           // CN_HEAPBUFFER
  /* 50 */ HT_INVALID,           // CN_HPROW

  /* 51 */ HT_INVALID,           // CN_INDEX
  /* 52 */ HT_TRANSIENT_SMALL,   // CN_INDEXTREE
  /* 53 */ HT_TRANSIENT_MEDIUM,  // CN_INDEXTREEVEC
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_INFLATEOP
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_INPUT
  /* 54 */ HT_TRANSIENT_MEDIUM,  // CN_IDXCOLVEC
  /* 55 */ HT_TRANSIENT_MEDIUM,  // CN_IDXVEC
  /*    */ HT_TRANSIENT_LARGE,   // CN_INGESTCHUNK
  /*    */ HT_TRANSIENT_LARGE,   // CN_INSERTOP
  /*    */ HT_TRANSIENT_LARGE,   // CN_INTBUFFER
  /*    */ HT_TRANSIENT_SMALL,   // CN_INTERSECTOP
  /*    */ HT_TRANSIENT_LARGE,   // CN_IOCHUNK
  /*    */ HT_TRANSIENT_SMALL,   // CN_IOCOPY
  /*    */ HT_TRANSIENT_LARGE,   // CN_IRWBUFFER

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
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_CONNECTCLUSTER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_CONNECTOR
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_COPYBUFFER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_CREATEINDEX
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_CREATETABLE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_DEFINENODE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_EXPORT
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_FETCHROWS
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_HEALTH
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_INSERTROW
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_INSERTTABLE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_KIDEXPORT
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_LOADTABLE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_NETHOG
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_NODEINFO
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_OPERAND
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_PERIODIC
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_PIPELINE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_PUBLISH
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_PURGE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_RECEIVER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SANITY
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SELECT
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_START
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_STARTCLUSTER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SUBEXPORTER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SUBFETCH
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
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_UNIVERSAL
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_WAITCLUSTER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_WAKER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_WALK
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_WRITEBUFFER
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_WRITEROW
  
  /*    */ HT_TRANSIENT_SMALL,   // CN_JOBLIST
  /*    */ HT_TRANSIENT_SMALL,   // CN_JOINOP
  /* 59 */ HT_INVALID,           // CN_JUMBOPERSISTENT
  /* 60 */ HT_INVALID,           // CN_JUMBOTRANSIENT
  
  /* 61 */ HT_INVALID,           // CN_KEY

  /* 65 */ HT_INVALID,           // CN_LARGEPERSISTENT
  /* 66 */ HT_INVALID,           // CN_LARGETRANSIENT
  /* 66 */ HT_TRANSIENT_SMALL,   // CN_LEXICON
  /* 66 */ HT_TRANSIENT_SMALL,   // CN_LIMITOP
  /*    */ HT_TRANSIENT_LARGE,   // CN_LOADCHUNK
  /* 71 */ HT_TRANSIENT_JUMBO,   // CN_LOADMAP
  /*    */ HT_INVALID,           // CN_LOCAL
  /*    */ HT_TRANSIENT_LARGE,   // CN_LOCALSCHAIN
  /* 72 */ HT_INVALID,           // CN_LOCK
  /* 72 */ HT_INVALID,           // CN_LOG
  /* 72 */ HT_INVALID,           // CN_LOGENTRY

  /* 73 */ HT_INVALID,           // CN_MEDIUMPERSISTENT
  /* 74 */ HT_INVALID,           // CN_MEDIUMTRANSIENT
  /*    */ HT_TRANSIENT_SMALL,   // CN_MEMTEST
  /*    */ HT_TRANSIENT_SMALL,   // CN_MERGEOP
  /*114 */ HT_TRANSIENT_SMALL,   // CN_MUTEX

  /* 73 */ HT_INVALID,           // CN_NEXTID
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_NMSTR
  /*    */ HT_TRANSIENT_SMALL,   // CN_NMLIST
  /*    */ HT_TRANSIENT_SMALL,   // CN_NMVAL
  /* 75 */ HT_TRANSIENT_SMALL,   // CN_NODE
  /*    */ HT_TRANSIENT_SMALL,   // CN_NOW
  /*    */ HT_TRANSIENT_LARGE,   // CN_NUMBAT

  /* 29 */ HT_TRANSIENT_MEDIUM,  // CN_OP_TOCLAUSE
  /* 29 */ HT_TRANSIENT_MEDIUM,  // CN_OP_TOCOMMAND
  /* 29 */ HT_TRANSIENT_MEDIUM,  // CN_OP_TODO
  /* 29 */ HT_TRANSIENT_MEDIUM,  // CN_OP_TOFILTER
  /* 29 */ HT_TRANSIENT_SMALL,   // CN_OPERANDQUEUE
  /* 29 */ HT_TRANSIENT_MEDIUM,  // CN_OPSPEC
  /* 29 */ HT_TRANSIENT_SMALL,   // CN_OPSPECVEC
  /* 29 */ HT_TRANSIENT_SMALL,   // CN_OPVEC
  /*    */ HT_TRANSIENT_SMALL,   // CN_OUTPUTOP

  /* 78 */ HT_TRANSIENT_MEDIUM,  // CN_PARSER
  /* 82 */ HT_TRANSIENT_MEDIUM,  // CN_PATHVEC
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_PAUSEMARKER
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_PERUSE
  /*    */ HT_TRANSIENT_SMALL,   // CN_PIPE
  /* 34 */ HT_TRANSIENT_MEDIUM,  // CN_PIPELINE
  /* 35 */ HT_TRANSIENT_SMALL,   // CN_PIPELINESPEC
  /* 86 */ HT_TRANSIENT_MEDIUM,  // CN_PLAN
  /* 87 */ HT_TRANSIENT_MEDIUM,  // CN_PLANNER
  /* 89 */ HT_TRANSIENT_SMALL,   // CN_PREDICATE
  /* 90 */ HT_TRANSIENT_MEDIUM,  // CN_PREDICATEVEC
  /* 90 */ HT_TRANSIENT_SMALL,   // CN_PROJECTOP
  /* 91 */ HT_TRANSIENT_SMALL,   // CN_PSXTHD
  /* 94 */ HT_TRANSIENT_MEDIUM,  // CN_PTNCOLVEC

  /* 95 */ HT_TRANSIENT_SMALL,   // CN_QUEUE
  /* 96 */ HT_INVALID,           // CN_QUEUEBUFFER
  /*    */ HT_TRANSIENT_SMALL,   // CN_QUEUES
  
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_RECORD
  /*    */ HT_TRANSIENT_LARGE,   // CN_RECORDBUFFER
  /*    */ HT_TRANSIENT_LARGE,   // CN_REDUCEOP
  /*    */ HT_TRANSIENT_LARGE,   // CN_REFBUFFER
  /*    */ HT_INVALID,           // CN_REFSET
  /* 77 */ HT_TRANSIENT_SMALL,   // CN_REGSET
  /* 77 */ HT_TRANSIENT_SMALL,   // CN_RELOP
  /* 77 */ HT_TRANSIENT_SMALL,   // CN_REQUEST
  /*    */ HT_TRANSIENT_SMALL,   // CN_RESOURCETRACKER
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_RESPONSE
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_RESPONSEVEC
  /*    */ HT_TRANSIENT_LARGE,   // CN_ROW
  /*102 */ HT_TRANSIENT_MEDIUM,  // CN_ROWAGG
  /*    */ HT_TRANSIENT_LARGE,   // CN_ROWCOMPACTOR
  /*105 */ HT_TRANSIENT_SMALL,   // CN_ROWONCE
  /*    */ HT_TRANSIENT_LARGE,   // CN_ROWTRAY
  
  /*    */ HT_TRANSIENT_SMALL,   // CN_SCANOP
  /*107 */ HT_TRANSIENT_SMALL,   // CN_SCHEMA
  /*107 */ HT_TRANSIENT_LARGE,   // CN_SCHEMATEST
  /*    */ HT_INVALID,           // CN_SEEME
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_SEMANTIC
  /*115 */ HT_TRANSIENT_LARGE,   // CN_SEPROMPT
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_SESSION
  /*    */ HT_TRANSIENT_LARGE,   // CN_SESSIONARRAY
  /*116 */ HT_TRANSIENT_MEDIUM,  // CN_SHORTCUT
  /*116 */ HT_TRANSIENT_MEDIUM,  // CN_SHORTCUTVEC
  /*116 */ HT_INVALID,           // CN_SINCE
  /*118 */ HT_TRANSIENT_SMALL,   // CN_SKIPLIST
  /*119 */ HT_TRANSIENT_SMALL,   // CN_SKIPNODE
  /*120 */ HT_TRANSIENT_LARGE,   // CN_SLAB
  /*121 */ HT_TRANSIENT_SMALL,   // CN_SLABALLOCATOR
  /*    */ HT_INVALID,           // CN_SLEEP
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
  /*125 */ HT_TRANSIENT_SMALL,   // CN_SORTOP
  /*125 */ HT_INVALID,           // CN_SPBCD
  /*125 */ HT_INVALID,           // CN_SPBOL
  /*125 */ HT_INVALID,           // CN_SPDTE
  /*125 */ HT_INVALID,           // CN_SPELAPSED
  /*125 */ HT_INVALID,           // CN_SPFLT
  /*125 */ HT_INVALID,           // CN_SPID
  /*125 */ HT_INVALID,           // CN_SPINT
  /*125 */ HT_INVALID,           // CN_SPIVL
  /*125 */ HT_INVALID,           // CN_SPSTRING
  /*125 */ HT_INVALID,           // CN_SPTKNVECSTR
  /*125 */ HT_INVALID,           // CN_SPTME
  /*125 */ HT_INVALID,           // CN_SPTSP
  /*125 */ HT_INVALID,           // CN_SPVCH
  /*125 */ HT_TRANSIENT_LARGE,   // CN_SRTBUFFER
  /*125 */ HT_INVALID,           // CN_SRTSET
  /*126 */ HT_TRANSIENT_LARGE,   // CN_STACK
  /*126 */ HT_TRANSIENT_MEDIUM,  // CN_STAGE
  /*126 */ HT_TRANSIENT_SMALL,   // CN_STAGENDE
  /*126 */ HT_TRANSIENT_SMALL,   // CN_STAGENDEVEC
  /*126 */ HT_TRANSIENT_SMALL,   // CN_STAGEPTN
  /*126 */ HT_TRANSIENT_SMALL,   // CN_STAGEPTNVEC
  /*126 */ HT_TRANSIENT_SMALL,   // CN_STAGESPEC
  /*126 */ HT_TRANSIENT_SMALL,   // CN_STAGESPECVEC
  /*126 */ HT_TRANSIENT_SMALL,   // CN_STAGEVEC
  /*    */ HT_TRANSIENT_SMALL,   // CN_STARTMARKER
  /*    */ HT_TRANSIENT_SMALL,   // CN_STARTOP
  /*127 */ HT_INVALID,           // CN_STEPACCUM
  /*127 */ HT_INVALID,           // CN_STATISTICS
  /*128 */ HT_TRANSIENT_LARGE,   // CN_STEPTIMING
  /*128 */ HT_TRANSIENT_LARGE,   // CN_STMT_SELECT
  /*128 */ HT_TRANSIENT_LARGE,   // CN_STREAMDEF
  /* 13 */ HT_INVALID,           // CN_STRING_T
  /*    */ HT_TRANSIENT_SMALL,   // CN_SUPERAGGOP
  /*    */ HT_TRANSIENT_SMALL,   // CN_SUPERCOUNTOP
  /*    */ HT_TRANSIENT_SMALL,   // CN_SUPERMERGEOP
  /*129 */ HT_INVALID,           // CN_SYSCALL
  
  /*130 */ HT_INVALID,           // CN_TABLE
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_TABLEDEF
  /*    */ HT_TRANSIENT_SMALL,   // CN_TABLEPTN
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_TABLEPTNVEC
  /*    */ HT_TRANSIENT_SMALL,   // CN_TAKEAWAY
  /*131 */ HT_TRANSIENT_MEDIUM,  // CN_TASK
  /*    */ HT_TRANSIENT_SMALL,   // CN_TASKLIST
  /*    */ HT_TRANSIENT_SMALL,   // CN_TEMPNAME
  /*    */ HT_TRANSIENT_SMALL,   // CN_TENANT
  /*    */ HT_TRANSIENT_SMALL,   // CN_TENANTS
  /*134 */ HT_TRANSIENT_SMALL,   // CN_TASKQUEUE
  /*137 */ HT_TRANSIENT_SMALL,   // CN_THREAD
  /*137 */ HT_TRANSIENT_SMALL,   // CN_THREADQUEUE
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_THREADVEC
  /*142 */ HT_TRANSIENT_SMALL,   // CN_TIMEMARKER
  /*142 */ HT_TRANSIENT_SMALL,   // CN_TIMER
  /*    */ HT_TRANSIENT_SMALL,   // CN_TOCSVOP
  /*    */ HT_TRANSIENT_SMALL,   // CN_TOFILEOP
  /*143 */ HT_TRANSIENT_SMALL,   // CN_TOKEN
  /*144 */ HT_TRANSIENT_MEDIUM,  // CN_TOKENISER
  /*146 */ HT_TRANSIENT_MEDIUM,  // CN_TOKENVEC
  /*147 */ HT_TRANSIENT_SMALL,   // CN_TX
  /*148 */ HT_TRANSIENT_SMALL,   // CN_TXSET
  
  /* 39 */ HT_TRANSIENT_SMALL,   // CN_WHEREOP
  /*149 */ HT_INVALID,           // CN_WILDERNESS
  /*150 */ HT_TRANSIENT_MEDIUM,  // CN_WORD

  /*148 */ HT_TRANSIENT_SMALL,   // CN_XMITOP

  /*118 */ HT_TRANSIENT_SMALL,   // CN_ZEDOP
  
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
