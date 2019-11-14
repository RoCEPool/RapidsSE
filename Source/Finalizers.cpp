/*============================================================================*/
/* Finalize.cpp -- Embedded Class Finalizers                                  */
/*============================================================================*/

#include "Oblique.h"

// Object's : Class::vFinalize
// Object's with no vUndefined defined :Finalize::vUndefined
// all embedded objects should have a vFinalize

pFINALIZE *gpFinalize[]
{
  /*  0 */ &Finalize::vUndefined,         // CN_NONE
  
  /*    */ &Finalize::vUndefined,         // CN_BIRTH
  /*    */ &Finalize::vUndefined,         // CN_FIPVAL
  /*    */ &Finalize::vUndefined,         // CN_FREQDIST
  /*  7 */ &Finalize::vUndefined,         // CN_INTVAL
  /* 11 */ &Finalize::vUndefined,         // CN_PTRVAL
  /* 12 */ &Finalize::vUndefined,         // CN_STRVAL

  /*  6 */ &Finalize::vUndefined,         // CN_FLTVAL
  /*  9 */ &Finalize::vUndefined,         // CN_IVLVAL

  /* 19 */ &Finalize::vUndefined,         // CN_ACCESSPATH
  /*    */ &Finalize::vUndefined,         // CN_AGGOP
  /* 22 */ &Finalize::vUndefined,         // CN_ARG
  /* 23 */ &Finalize::vUndefined,         // CN_ARGUMENTS
  /*    */ &Finalize::vUndefined,         // CN_ARTICLE
  /* 24 */ &Finalize::vUndefined,         // CN_ATOMIC
  /* 25 */ &Finalize::vUndefined,         // CN_ATTACHMAP

  /* 29 */ &Finalize::vUndefined,         // CN_BOTHOP
  /* 29 */ &Finalize::vUndefined,         // CN_BPA
  /* 29 */ &Finalize::vUndefined,         // CN_BUCKETMAP
  /*    */ &Finalize::vUndefined,         // CN_BUFFERCURSOR
  /* 28 */ &Finalize::vUndefined,         // CN_BUFFERPOOL
  /*    */ &Finalize::vUndefined,         // CN_BYTERATE
  /*    */ &Finalize::vUndefined,         // CN_BYTES_T

  /*    */ &Finalize::vUndefined,         // CN_CALCULATOR
  /*    */ &Finalize::vUndefined,         // CN_CANCELMARKER
  /* 30 */ &Finalize::vUndefined,         // CN_CARRYOVER
  /*    */ &Finalize::vUndefined,         // CN_CATALOG
  /*    */ &Finalize::vUndefined,         // CN_CLAUSE_FROM
  /*    */ &Finalize::vUndefined,         // CN_CLAUSE_GROUPBY
  /*    */ &Finalize::vUndefined,         // CN_CLAUSE_JOIN
  /*    */ &Finalize::vUndefined,         // CN_CLAUSE_LIMIT
  /*    */ &Finalize::vUndefined,         // CN_CLAUSE_ORDERBY
  /*    */ &Finalize::vUndefined,         // CN_CLAUSE_SELECT
  /*    */ &Finalize::vUndefined,         // CN_CLAUSE_SUBQRY
  /*    */ &Finalize::vUndefined,         // CN_CLAUSE_WHERE
  /*    */ &Finalize::vUndefined,         // CN_CMD_SELECT
  /*    */ &Finalize::vUndefined,         // CN_COLUMN
  /* 32 */ &Finalize::vUndefined,         // CN_COLUMNVEC
  /*    */ &Finalize::vUndefined,         // CN_COMBINEOP
  /*    */ &Finalize::vUndefined,         // CN_COMMAND
  /*    */ &Finalize::vUndefined,         // CN_COMPILER
  /*    */ &Finalize::vUndefined,         // CN_CONSUMEOP
  /*    */ &Finalize::vUndefined,         // CN_COUNTOP
  /*    */ &Finalize::vUndefined,         // CN_CRWBUFFER
  /*    */ &Finalize::vUndefined,         // CN_CSVBUFFER
  
  /*    */ &Finalize::vUndefined,         // CN_DECACNT
  /*    */ &Finalize::vUndefined,         // CN_DECIDER
  /*    */ &Finalize::vUndefined,         // CN_DELAYOP
  /* 33 */ &Dic::vFinalize,               // CN_DIC
  /*    */ &Finalize::vUndefined,         // CN_DICPATH
  /* 34 */ &Finalize::vUndefined,         // CN_DICSTACK
  /* 35 */ &Finalize::vUndefined,         // CN_DIR
  /*    */ &Finalize::vUndefined,         // CN_DISTINCTOP
  /*    */ &Finalize::vUndefined,         // CN_DTMVAL
  /*    */ &Finalize::vUndefined,         // CN_DYNAMO

  /*    */ &Finalize::vUndefined,         // CN_EARLYMARKER
  /*    */ &Finalize::vUndefined,         // CN_ENCODING
  /*    */ &Finalize::vUndefined,         // CN_ENDMARKER
  /* 37 */ &Finalize::vUndefined,         // CN_ENMSET
  /* 36 */ &Finalize::vUndefined,         // CN_ENT
  /* 64 */ &Finalize::vUndefined,         // CN_ENTWRK
  /*    */ &Finalize::vUndefined,         // CN_ENUMFLAGS
  /*    */ &Finalize::vUndefined,         // CN_ENUMSS
  /*    */ &Finalize::vUndefined,         // CN_EXPROP

  /*    */ &Finalize::vUndefined,         // CN_FETCHBUFFER
  /*    */ &Finalize::vUndefined,         // CN_FETCHOP
  /*    */ &Finalize::vUndefined,         // CN_FIFO
  /*    */ &Finalize::vUndefined,         // CN_FILE
  /*    */ &Finalize::vUndefined,         // CN_FILTEROP
  /*    */ &Finalize::vUndefined,         // CN_FILTER_FETCH
  /*    */ &Finalize::vUndefined,         // CN_FILTER_FROM
  /*    */ &Finalize::vUndefined,         // CN_FILTER_GROUPBY
  /*    */ &Finalize::vUndefined,         // CN_FILTER_INFLATE
  /*    */ &Finalize::vUndefined,         // CN_FILTER_JOIN
  /*    */ &Finalize::vUndefined,         // CN_FILTER_LIMIT
  /*    */ &Finalize::vUndefined,         // CN_FILTER_ORDERBY
  /*    */ &Finalize::vUndefined,         // CN_FILTER_OUTPUT
  /*    */ &Finalize::vUndefined,         // CN_FILTER_PROJECT
  /*    */ &Finalize::vUndefined,         // CN_FILTER_SCAN
  /*    */ &Finalize::vUndefined,         // CN_FILTER_SELECT
  /*    */ &Finalize::vUndefined,         // CN_FILTER_SUBQRY
  /*    */ &Finalize::vUndefined,         // CN_FILTER_WHERE
  /*    */ &Finalize::vUndefined,         // CN_FILTER_ZED
  /*    */ &Finalize::vUndefined,         // CN_FOCALOBJECTS
  /*    */ &Finalize::vUndefined,         // CN_FORTH
  /* 39 */ &Finalize::vUndefined,         // CN_FORMATTERS
  /* 41 */ &Finalize::vUndefined,         // CN_FREEBUFFER
  /* 42 */ &Finalize::vUndefined,         // CN_FREECHUNK
  /* 43 */ &Finalize::vUndefined,         // CN_FREEDNOT
  /* 44 */ &Finalize::vUndefined,         // CN_FREEITEM
  
  /*    */ &Finalize::vUndefined,         // CN_GENERATEOP
  /*    */ &Finalize::vUndefined,         // CN_GENVAR
  /*    */ &Finalize::vUndefined,         // CN_GETKEYOP
  /* 48 */ &Finalize::vUndefined,         // CN_GRID
  
  /*    */ &Finalize::vUndefined,         // CN_HASHBUFFER
  /*    */ &Finalize::vUndefined,         // CN_HASHMAP
  /*    */ &Finalize::vUndefined,         // CN_HASHOP
  /* 50 */ &Finalize::vUndefined,         // CN_HEAPBUFFER
  /* 50 */ &Finalize::vUndefined,         // CN_HPROW

  /* 51 */ &Index::vFinalize,             // CN_INDEX
  /* 52 */ &Finalize::vUndefined,         // CN_INDEXDEF
  /* 52 */ &Finalize::vUndefined,         // CN_INDEXTREE
  /* 53 */ &Finalize::vUndefined,         // CN_INDEXTREEVEC
  /*    */ &Finalize::vUndefined,         // CN_INFLATEOP
  /*    */ &Finalize::vUndefined,         // CN_INPUT
  /* 54 */ &Finalize::vUndefined,         // CN_IDXCOLVEC
  /* 55 */ &Finalize::vUndefined,         // CN_IDXVEC
  /*    */ &Finalize::vUndefined,         // CN_INGESTCHUNK
  /*    */ &Finalize::vUndefined,         // CN_INSERTOP
  /*    */ &Finalize::vUndefined,         // CN_INTBUFFER
  /*    */ &Finalize::vUndefined,         // CN_INTERSECTOP
  /*    */ &Finalize::vUndefined,         // CN_IOCHUNK
  /*    */ &Finalize::vUndefined,         // CN_IOCOPY
  /*    */ &Finalize::vUndefined,         // CN_IRWBUFFER

  /*    */ &Finalize::vUndefined,         // CN_JOB__BEGUN
  /*    */ &Finalize::vUndefined,         // CN_JOB__CANCEL
  /*    */ &Finalize::vUndefined,         // CN_JOB__CANCELLED
  /*    */ &Finalize::vUndefined,         // CN_JOB__END
  /*    */ &Finalize::vUndefined,         // CN_JOB__ENDED
  /*    */ &Finalize::vUndefined,         // CN_JOB__EXECUTE
  /*    */ &Finalize::vUndefined,         // CN_JOB__EXECUTED
  /*    */ &Finalize::vUndefined,         // CN_JOB__FIND
  /*    */ &Finalize::vUndefined,         // CN_JOB_ALARM
  /*    */ &Finalize::vUndefined,         // CN_JOB_ANSWER
  /*    */ &Finalize::vUndefined,         // CN_JOB_BOOT

  /*    */ &Finalize::vUndefined,         // CN_JOB_CONNECTOR
  /*    */ &Finalize::vUndefined,         // CN_JOB_COPYBUFFER
  /*    */ &Finalize::vUndefined,         // CN_JOB_FETCHROWS
  /*    */ &Finalize::vUndefined,         // CN_JOB_INSERTROW
  /*    */ &Finalize::vUndefined,         // CN_JOB_INSERTTABLE

  /*    */ &Finalize::vUndefined,         // CN_JOB_LOADTABLE
  /*    */ &Finalize::vUndefined,         // CN_JOB_NODEINFO
  /*    */ &Finalize::vUndefined,         // CN_JOB_OPERAND
  /*    */ &Finalize::vUndefined,         // CN_JOB_PERIODIC
  /*    */ &Finalize::vUndefined,         // CN_JOB_RECEIVER
  /*    */ &Finalize::vUndefined,         // CN_JOB_SANITY
  /*    */ &Finalize::vUndefined,         // CN_JOB_SELECT
  /*    */ &Finalize::vUndefined,         // CN_JOB_START
  /*    */ &Finalize::vUndefined,         // CN_JOB_SUBFETCH
  /*    */ &Finalize::vUndefined,         // CN_JOB_SUBINFLATE
  /*    */ &Finalize::vUndefined,         // CN_JOB_SUBINGESTER
  /*    */ &Finalize::vUndefined,         // CN_JOB_SUBINSERTER
  /*    */ &Finalize::vUndefined,         // CN_JOB_SUBLOADER
  /*    */ &Finalize::vUndefined,         // CN_JOB_SUBTRUNCATE
  /*    */ &Finalize::vUndefined,         // CN_JOB_SUPERVISOR
  /*    */ &Finalize::vUndefined,         // CN_JOB_UNIVERSAL
  /*    */ &Finalize::vUndefined,         // CN_JOB_WAKER
  /*    */ &Finalize::vUndefined,         // CN_JOB_WRITEBUFFER
  /*    */ &Finalize::vUndefined,         // CN_JOB_WRITEROW
  
  /*    */ &Finalize::vUndefined,         // CN_JOBLIST
  /*    */ &Finalize::vUndefined,         // CN_JOINOP
  /* 59 */ &Finalize::vUndefined,         // CN_JUMBOPERSISTENT
  /* 60 */ &Finalize::vUndefined,         // CN_JUMBOTRANSIENT
  
  /* 61 */ &Finalize::vUndefined,         // CN_KEY
  
  /* 65 */ &Finalize::vUndefined,         // CN_LARGEPERSISTENT
  /* 66 */ &Finalize::vUndefined,         // CN_LARGETRANSIENT
  /*    */ &Finalize::vUndefined,         // CN_LEXICON
  /*    */ &Finalize::vUndefined,         // CN_LIMITOP
  /*    */ &Finalize::vUndefined,         // CN_LOADCHUNK
  /* 71 */ &Finalize::vUndefined,         // CN_LOADMAP
  /*    */ &Finalize::vUndefined,         // CN_LOCAL
  /*    */ &Finalize::vUndefined,         // CN_LOCALSCHAIN
  /* 72 */ &Finalize::vUndefined,         // CN_LOCK
  /*    */ &Finalize::vUndefined,         // CN_LOG
  /*    */ &Finalize::vUndefined,         // CN_LOGENTRY

  /* 73 */ &Finalize::vUndefined,         // CN_MEDIUMPERSISTENT
  /* 74 */ &Finalize::vUndefined,         // CN_MEDIUMTRANSIENT
  /*    */ &Finalize::vUndefined,         // CN_MEMTEST
  /*    */ &Finalize::vUndefined,         // CN_MERGEOP
  /*114 */ &Finalize::vUndefined,         // CN_MUTEX
  
  /*    */ &Finalize::vUndefined,         // CN_NEXTID
  /*    */ &Finalize::vUndefined,         // CN_NMSTR
  /*    */ &Finalize::vUndefined,         // CN_NMSTRLIST
  /*    */ &Finalize::vUndefined,         // CN_NMVAL
  /* 75 */ &Finalize::vUndefined,         // CN_NODE
  /*    */ &Finalize::vUndefined,         // CN_NOW
  /*    */ &Finalize::vUndefined,         // CN_NUMBAT

  /* 28 */ &Finalize::vUndefined,         // CN_OP_TOCLAUSE
  /* 28 */ &Finalize::vUndefined,         // CN_OP_TOCOMMAND
  /* 28 */ &Finalize::vUndefined,         // CN_OP_TODO
  /* 28 */ &Finalize::vUndefined,         // CN_OP_TOFILTER
  /* 28 */ &Finalize::vUndefined,         // CN_OPERANDQUEUE
  /* 28 */ &Finalize::vUndefined,         // CN_OPSPEC
  /* 28 */ &Finalize::vUndefined,         // CN_OPSPECVEC
  /* 28 */ &Finalize::vUndefined,         // CN_OPVEC
  /*    */ &Finalize::vUndefined,         // CN_OUTPUTOP

  /* 78 */ &Finalize::vUndefined,         // CN_PARSER
  /* 82 */ &Finalize::vUndefined,         // CN_PATHVEC
  /* 82 */ &Finalize::vUndefined,         // CN_PAUSEMARKER
  /* 82 */ &Finalize::vUndefined,         // CN_PERUSE
  /*    */ &Finalize::vUndefined,         // CN_PIPE
  /* 34 */ &Finalize::vUndefined,         // CN_PIPELINE
  /*    */ &Finalize::vUndefined,         // CN_PIPELINESPEC
  /* 86 */ &Finalize::vUndefined,         // CN_PLAN
  /* 87 */ &Finalize::vUndefined,         // CN_PLANNER
  /* 90 */ &Finalize::vUndefined,         // CN_PREDICATE
  /* 91 */ &Finalize::vUndefined,         // CN_PREDICATEVEC
  /* 91 */ &Finalize::vUndefined,         // CN_PROJECTOP
  /* 89 */ &Finalize::vUndefined,         // CN_PSXTHD
  /* 94 */ &Finalize::vUndefined,         // CN_PTNCOLVEC
  
  /* 95 */ &Finalize::vUndefined,         // CN_QUEUE
  /* 96 */ &Finalize::vUndefined,         // CN_QUEUEBUFFER
  /*    */ &Finalize::vUndefined,         // CN_QUEUES
  
  /*    */ &Finalize::vUndefined,         // CN_RECORD
  /*    */ &Finalize::vUndefined,         // CN_RECORDBUFFER
  /*    */ &Finalize::vUndefined,         // CN_REDUCEOP
  /*    */ &Finalize::vUndefined,         // CN_REFBUFFER
  /*    */ &Finalize::vUndefined,         // CN_REFSET
  /*    */ &Finalize::vUndefined,         // CN_REGSET
  /* 77 */ &Finalize::vUndefined,         // CN_RELOP
  /* 77 */ &Finalize::vUndefined,         // CN_REQUEST
  /*    */ &Finalize::vUndefined,         // CN_RESOURCETRACKER
  /*    */ &Finalize::vUndefined,         // CN_RESPONSE
  /*    */ &Finalize::vUndefined,         // CN_RESPONSEVEC
  /*    */ &Finalize::vUndefined,         // CN_ROW
  /*102 */ &Finalize::vUndefined,         // CN_ROWAGG
  /*    */ &Finalize::vUndefined,         // CN_ROWCOMPACTOR
  /*105 */ &Finalize::vUndefined,         // CN_ROWONCE
  /*    */ &Finalize::vUndefined,         // CN_ROWTRAY
  
  /*    */ &Finalize::vUndefined,         // CN_SCANOP
  /*    */ &Finalize::vUndefined,         // CN_SCHEMA
  /*    */ &Finalize::vUndefined,         // CN_SCHEMATEST
  /*    */ &Finalize::vUndefined,         // CN_SEEME
  /*    */ &Finalize::vUndefined,         // CN_SEMANTIC
  /*115 */ &Finalize::vUndefined,         // CN_SEPROMPT
  /*    */ &Finalize::vUndefined,         // CN_SESSION
  /*    */ &Finalize::vUndefined,         // CN_SESSIONARRAY
  /*116 */ &Finalize::vUndefined,         // CN_SHORTCUT
  /*116 */ &Finalize::vUndefined,         // CN_SHORTCUTVEC
  /*116 */ &Finalize::vUndefined,         // CN_SINCE
  /*118 */ &Finalize::vUndefined,         // CN_SKIPLIST
  /*119 */ &Finalize::vUndefined,         // CN_SKIPNODE
  /*120 */ &Finalize::vUndefined,         // CN_SLAB
  /*121 */ &Finalize::vUndefined,         // CN_SLABALLOCATOR
  /*    */ &Finalize::vUndefined,         // CN_SLEEP
  /*122 */ &Slots::vFinalize,             // CN_SLOTS
  /*123 */ &Finalize::vUndefined,         // SN_SMALLPERSISTENT
  /*124 */ &Finalize::vUndefined,         // SN_SMALLTRANSIENT
  /*    */ &Finalize::vUndefined,         // CN_SNBCD
  /*    */ &Finalize::vUndefined,         // CN_SNBOL
  /*    */ &Finalize::vUndefined,         // CN_SNDTE
  /*    */ &Finalize::vUndefined,         // CN_SNFLT
  /*    */ &Finalize::vUndefined,         // CN_SNINT
  /*    */ &Finalize::vUndefined,         // CN_SNIVL
  /*    */ &Finalize::vUndefined,         // CN_SNTME
  /*    */ &Finalize::vUndefined,         // CN_SNTSP
  /*    */ &Finalize::vUndefined,         // CN_SNVCH
  /*125 */ &Finalize::vUndefined,         // CN_SOCKET
  /*125 */ &Finalize::vUndefined,         // CN_SORTOP
  /*    */ &Finalize::vUndefined,         // CN_SPBCD
  /*    */ &Finalize::vUndefined,         // CN_SPBOL
  /*    */ &Finalize::vUndefined,         // CN_SPDTE
  /*    */ &Finalize::vUndefined,         // CN_SPELAPSED
  /*    */ &Finalize::vUndefined,         // CN_SPFLT
  /*    */ &Finalize::vUndefined,         // CN_SPID
  /*    */ &Finalize::vUndefined,         // CN_SPINT
  /*    */ &Finalize::vUndefined,         // CN_SPIVL
  /*    */ &Finalize::vUndefined,         // CN_SPSTRING
  /*    */ &Finalize::vUndefined,         // CN_SPTKNVECSTR
  /*    */ &Finalize::vUndefined,         // CN_SPTME
  /*    */ &Finalize::vUndefined,         // CN_SPTSP
  /*    */ &Finalize::vUndefined,         // CN_SPVCH
  /*    */ &Finalize::vUndefined,         // CN_SRTBUFFER
  /*    */ &Finalize::vUndefined,         // CN_SRTSET
  /*126 */ &Finalize::vUndefined,         // CN_STACK
  /*126 */ &Finalize::vUndefined,         // CN_STAGE
  /*126 */ &Finalize::vUndefined,         // CN_STAGENDE
  /*126 */ &Finalize::vUndefined,         // CN_STAGENDEVEC
  /*126 */ &Finalize::vUndefined,         // CN_STAGEPTN
  /*126 */ &Finalize::vUndefined,         // CN_STAGEPTNVEC
  /*126 */ &Finalize::vUndefined,         // CN_STAGESPEC
  /*126 */ &Finalize::vUndefined,         // CN_STAGESPECVEC
  /*126 */ &Finalize::vUndefined,         // CN_STAGEVEC
  /*    */ &Finalize::vUndefined,         // CN_STARTMARKER
  /*    */ &Finalize::vUndefined,         // CN_STARTOP
  /*    */ &Finalize::vUndefined,         // CN_STATISTICS
  /*127 */ &Finalize::vUndefined,         // CN_STEPACCUM
  /*128 */ &Finalize::vUndefined,         // CN_STEPTIMING
  /*128 */ &Finalize::vUndefined,         // CN_STMT_SELECT
  /* 13 */ &Finalize::vUndefined,         // CN_STREAMDEF
  /* 13 */ &Finalize::vUndefined,         // CN_STRING_T
  /*    */ &Finalize::vUndefined,         // CN_SUPERAGGOP
  /*    */ &Finalize::vUndefined,         // CN_SUPERCOUNTOP
  /*    */ &Finalize::vUndefined,         // CN_SUPERMERGEOP
  /*129 */ &Finalize::vUndefined,         // CN_SYSCALL
  
  /*130 */ &Table::vFinalize,             // CN_TABLE
  /*    */ &Finalize::vUndefined,         // CN_TABLEDEF
  /*    */ &Finalize::vUndefined,         // CN_TABLEPTN
  /*    */ &Finalize::vUndefined,         // CN_TABLEPTNVEC
  /*    */ &Finalize::vUndefined,         // CN_TAKEAWAYOP
  /*131 */ &Finalize::vUndefined,         // CN_TASK
  /*134 */ &Finalize::vUndefined,         // CN_TASKLIST
  /*    */ &Finalize::vUndefined,         // CN_TASKQUEUE
  /*    */ &Finalize::vUndefined,         // CN_TEMPNAME
  /*    */ &Finalize::vUndefined,         // CN_TENANT
  /*    */ &Finalize::vUndefined,         // CN_TENANTS
  /*137 */ &Finalize::vUndefined,         // CN_THREAD
  /*    */ &Finalize::vUndefined,         // CN_THREADQUEUE
  /*    */ &Finalize::vUndefined,         // CN_THREADVEC
  /*142 */ &Finalize::vUndefined,         // CN_TIMEMARKER
  /*142 */ &Finalize::vUndefined,         // CN_TIMER
  /*    */ &Finalize::vUndefined,         // CN_TOCSVOP
  /*    */ &Finalize::vUndefined,         // CN_TOFILEOP
  /*143 */ &Finalize::vUndefined,         // CN_TOKEN
  /*144 */ &Finalize::vUndefined,         // CN_TOKENISER
  /*146 */ &Finalize::vUndefined,         // CN_TOKENVEC
  /*147 */ &Finalize::vUndefined,         // CN_TX
  /*148 */ &Finalize::vUndefined,         // CN_TXSET
  
  /* 40 */ &Finalize::vUndefined,         // CN_WHEREOP
  /*149 */ &Finalize::vUndefined,         // CN_WILDERNESS
  /*150 */ &Finalize::vUndefined,         // CN_WORD
  
  /*150 */ &Finalize::vUndefined,         // CN_XMITOP

  /*116 */ &Finalize::vUndefined,         // CN_ZEDOP
  
  /*151 */ &Finalize::vUndefined,         // CN_ZSMALL_
  /*152 */ &Finalize::vUndefined,         // CN_ZMEDIUM_
  /*153 */ &Finalize::vUndefined,         // CN_ZLARGE_
  /*154 */ &Finalize::vUndefined,         // CN_ZJUMBO_
  
  /*155 */ &Finalize::vEnums              // CN_ENUMS
};

/*----------------------------------------------------------------------------*/
/* dump enums                                                                 */
/*----------------------------------------------------------------------------*/
void Finalize::vEnums(eCN eClass, Object* pObject)
{
  ASSERT(eClass == CN_ENUMS);
  ASSERT(pObject != nullptr);
  // do nothing
}

/*----------------------------------------------------------------------------*/
/* finalize not defined                                                       */
/*----------------------------------------------------------------------------*/
void Finalize::vUndefined(eCN eClass, Object* pObject)
{
  ASSERT(eClass != CN_NONE);
  vTrace("%s: **finalize function not defined for class**\n"_Stz, aCN[eClass]);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
