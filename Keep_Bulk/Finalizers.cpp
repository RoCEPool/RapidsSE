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
  
  /*    */ &Finalize::vUndefined,         // CN__A__
  /* 19 */ &Finalize::vUndefined,         // CN_ACCESSPATH
  /* 19 */ &Finalize::vUndefined,         // CN_AGGTIMER
  /* 22 */ &Finalize::vUndefined,         // CN_ARG
  /* 23 */ &Finalize::vUndefined,         // CN_ARGUMENTS
  /*    */ &Finalize::vUndefined,         // CN_ARTICLE
  /* 24 */ &Finalize::vUndefined,         // CN_ATOMIC
  /* 25 */ &Finalize::vUndefined,         // CN_ATTACHMAP

  /*    */ &Finalize::vUndefined,         // CN__B__
  /*  7 */ &Finalize::vUndefined,         // CN_BCDVAL
  /*    */ &Finalize::vUndefined,         // CN_BIRTH
  /*  7 */ &Finalize::vUndefined,         // CN_BOLVAL
  /* 29 */ &Finalize::vUndefined,         // CN_BPA
  /* 29 */ &Finalize::vUndefined,         // CN_BUCKETMAP
  /*    */ &Finalize::vUndefined,         // CN_BUFFERCURSOR
  /* 28 */ &Finalize::vUndefined,         // CN_BUFFERPOOL
  /*    */ &Finalize::vUndefined,         // CN_BYTERATE
  /*    */ &Finalize::vUndefined,         // CN_BYTES_T

  /*    */ &Finalize::vUndefined,         // CN__C__
  /*    */ &Finalize::vUndefined,         // CN_CALCULATOR
  /* 30 */ &Finalize::vUndefined,         // CN_CARRYOVER
  /*    */ &Finalize::vUndefined,         // CN_CATALOG
  /* 78 */ &Finalize::vUndefined,         // CN_CHARSER
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
  /*    */ &Finalize::vUndefined,         // CN_COMMAND
  /*    */ &Finalize::vUndefined,         // CN_COMPILER
  /*    */ &Finalize::vUndefined,         // CN_CONSTDEF
  /*    */ &Finalize::vUndefined,         // CN_CONSTSET
  /*    */ &Finalize::vUndefined,         // CN_CRVBUFFER
  /*    */ &Finalize::vUndefined,         // CN_CRWBUFFER
  /*    */ &Finalize::vUndefined,         // CN_CSVBUFFER
  
  /*    */ &Finalize::vUndefined,         // CN__D__
  /*    */ &Finalize::vUndefined,         // CN_DATUM
  /*    */ &Finalize::vUndefined,         // CN_DECACNT
  /*    */ &Finalize::vUndefined,         // CN_DECIDER
  /* 33 */ &Dic::vFinalize,               // CN_DIC
  /* 34 */ &Finalize::vUndefined,         // CN_DICSTACK
  /* 35 */ &Finalize::vUndefined,         // CN_DIR
  /*  7 */ &Finalize::vUndefined,         // CN_DTEVAL
  /*    */ &Finalize::vUndefined,         // CN_DYNAMO

  /*    */ &Finalize::vUndefined,         // CN__E__
  /*  7 */ &Finalize::vUndefined,         // CN_EDTVAL
  /*    */ &Finalize::vUndefined,         // CN_ENCODING
  /* 36 */ &Finalize::vUndefined,         // CN_ENT
  /* 64 */ &Finalize::vUndefined,         // CN_ENTWRK
  /*    */ &Finalize::vUndefined,         // CN_ENUMFLAGS
  /*    */ &Finalize::vUndefined,         // CN_ENUMSS
  /*    */ &Finalize::vUndefined,         // CN_EXEMPLAR
  /*    */ &Finalize::vUndefined,         // CN_EXTRACTROW

  /*    */ &Finalize::vUndefined,         // CN__F__
  /*    */ &Finalize::vUndefined,         // CN_FETCHBUFFER
  /*    */ &Finalize::vUndefined,         // CN_FIFO
  /*    */ &Finalize::vUndefined,         // CN_FILE
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
  /*    */ &Finalize::vUndefined,         // CN_FIPVAL
  /*  7 */ &Finalize::vUndefined,         // CN_FLTVAL
  /*    */ &Finalize::vUndefined,         // CN_FOCALOBJECTS
  /*    */ &Finalize::vUndefined,         // CN_FORTH
  /* 39 */ &Finalize::vUndefined,         // CN_FORMAT
  /* 41 */ &Finalize::vUndefined,         // CN_FREEBUFFER
  /* 42 */ &Finalize::vUndefined,         // CN_FREECHUNK
  /* 43 */ &Finalize::vUndefined,         // CN_FREEDNOT
  /* 44 */ &Finalize::vUndefined,         // CN_FREEITEM
  /*    */ &Finalize::vUndefined,         // CN_FREQDIST

  /*    */ &Finalize::vUndefined,         // CN__G__
  /*    */ &Finalize::vUndefined,         // CN_GGADAPTOR
  /* 48 */ &Finalize::vUndefined,         // CN_GRID
  
  /*    */ &Finalize::vUndefined,         // CN__H__
  /*    */ &Finalize::vUndefined,         // CN_HASHBUFFER
  /*    */ &Finalize::vUndefined,         // CN_HASHMAP
  /* 50 */ &Finalize::vUndefined,         // CN_HEAPBUFFER
  /* 50 */ &Finalize::vUndefined,         // CN_HPROW

  /*    */ &Finalize::vUndefined,         // CN__I__
  /* 51 */ &Index::vFinalize,             // CN_INDEX
  /* 52 */ &Finalize::vUndefined,         // CN_INDEXDEF
  /* 52 */ &Finalize::vUndefined,         // CN_INDEXTREE
  /* 53 */ &Finalize::vUndefined,         // CN_INDEXTREEVEC
  /* 54 */ &Finalize::vUndefined,         // CN_IDXCOLVEC
  /* 55 */ &Finalize::vUndefined,         // CN_IDXVEC
  /*    */ &Finalize::vUndefined,         // CN_INFOSET
  /*    */ &Finalize::vUndefined,         // CN_INGESTCHUNK
  /*144 */ &Finalize::vUndefined,         // CN_INPUT
  /*    */ &Finalize::vUndefined,         // CN_INTBUFFER
  /*  7 */ &Finalize::vUndefined,         // CN_INTVAL
  /*    */ &Finalize::vUndefined,         // CN_IOCHUNK
  /*    */ &Finalize::vUndefined,         // CN_IOCOPY
  /*    */ &Finalize::vUndefined,         // CN_IRWBUFFER
  /*  7 */ &Finalize::vUndefined,         // CN_IVLVAL

  /*    */ &Finalize::vUndefined,         // CN__J__
  /*    */ &Finalize::vUndefined,         // CN_JOB__FIND
  /*    */ &Finalize::vUndefined,         // CN_JOB_BOOT
  /*    */ &Finalize::vUndefined,         // CN_JOB_DISTRIBUTED
  /*    */ &Finalize::vUndefined,         // CN_JOB_LOCAL
  /*    */ &Finalize::vUndefined,         // CN_JOB_NODEINFO
  /*    */ &Finalize::vUndefined,         // CN_JOB_OPERAND
  /*    */ &Finalize::vUndefined,         // CN_JOB_RECEIVER
  /*    */ &Finalize::vUndefined,         // CN_JOB_START
  /*    */ &Finalize::vUndefined,         // CN_JOB_SUBINFLATE
  /*    */ &Finalize::vUndefined,         // CN_JOB_SUBINGESTER
  /*    */ &Finalize::vUndefined,         // CN_JOB_WRITEBUFFER
  /*    */ &Finalize::vUndefined,         // CN_JOBLIST
  /* 59 */ &Finalize::vUndefined,         // CN_JUMBOPERSISTENT
  /* 60 */ &Finalize::vUndefined,         // CN_JUMBOTRANSIENT
  
  /*    */ &Finalize::vUndefined,         // CN__K__
  /* 61 */ &Finalize::vUndefined,         // CN_KEY
  
  /*    */ &Finalize::vUndefined,         // CN__L__
  /* 65 */ &Finalize::vUndefined,         // CN_LARGEPERSISTENT
  /* 66 */ &Finalize::vUndefined,         // CN_LARGETRANSIENT
  /*    */ &Finalize::vUndefined,         // CN_LEXICON
  /*    */ &Finalize::vUndefined,         // CN_LOADCHUNK
  /*    */ &Finalize::vUndefined,         // CN_LOADER
  /* 71 */ &Finalize::vUndefined,         // CN_LOADMAP
  /*    */ &Finalize::vUndefined,         // CN_LOCAL
  /*    */ &Finalize::vUndefined,         // CN_LOCALSCHAIN
  /* 72 */ &Finalize::vUndefined,         // CN_LOCK
  /*    */ &Finalize::vUndefined,         // CN_LOG
  /*    */ &Finalize::vUndefined,         // CN_LOGENTRY

  /*    */ &Finalize::vUndefined,         // CN__M__
  /*    */ &Finalize::vUndefined,         // CN_MARKER
  /* 73 */ &Finalize::vUndefined,         // CN_MEDIUMPERSISTENT
  /* 74 */ &Finalize::vUndefined,         // CN_MEDIUMTRANSIENT
  /*    */ &Finalize::vUndefined,         // CN_MEMTEST
  /*    */ &Finalize::vUndefined,         // CN_METABLOCK
  /*    */ &Finalize::vUndefined,         // CN_METACELL
  /*    */ &Finalize::vUndefined,         // CN_METAGRAPH
  /*    */ &Finalize::vUndefined,         // CN_METAINHERIT
  /*    */ &Finalize::vUndefined,         // CN_METAINHERITS
  /*    */ &Finalize::vUndefined,         // CN_METAMEMBER
  /*    */ &Finalize::vUndefined,         // CN_METATYPE
  /*114 */ &Finalize::vUndefined,         // CN_MUTEX
  
  /*    */ &Finalize::vUndefined,         // CN__N__
  /*    */ &Finalize::vUndefined,         // CN_NEXTGID
  /*    */ &Finalize::vUndefined,         // CN_NMSTR
  /*    */ &Finalize::vUndefined,         // CN_NMSTRLIST
  /*    */ &Finalize::vUndefined,         // CN_NMVAL
  /* 75 */ &Finalize::vUndefined,         // CN_NODE
  /*    */ &Finalize::vUndefined,         // CN_NOW
  /*    */ &Finalize::vUndefined,         // CN_NUMBAT

  /*    */ &Finalize::vUndefined,         // CN__O__
  /* 28 */ &Finalize::vUndefined,         // CN_OBJECTSINPOOL
  /* 28 */ &Finalize::vUndefined,         // CN_OP
  /* 28 */ &Finalize::vUndefined,         // CN_OPERANDQUEUE
  /* 28 */ &Finalize::vUndefined,         // CN_OPSPEC

  /*    */ &Finalize::vUndefined,         // CN__P__
  /* 82 */ &Finalize::vUndefined,         // CN_PATHVEC
  /* 82 */ &Finalize::vUndefined,         // CN_PERUSE
  /* 34 */ &Finalize::vUndefined,         // CN_PIPELINE
  /*    */ &Finalize::vUndefined,         // CN_PIPELINESPEC
  /* 86 */ &Finalize::vUndefined,         // CN_PLAN
  /* 87 */ &Finalize::vUndefined,         // CN_PLANNER
  /* 90 */ &Finalize::vUndefined,         // CN_PREDICATE
  /* 91 */ &Finalize::vUndefined,         // CN_PREDICATEVEC
  /* 91 */ &Finalize::vUndefined,         // CN_PROFILE
  /* 89 */ &Finalize::vUndefined,         // CN_PSXTHD
  /* 94 */ &Finalize::vUndefined,         // CN_PTNCOLVEC
  /* 11 */ &Finalize::vUndefined,         // CN_PTRVAL

  /*    */ &Finalize::vUndefined,         // CN__Q__
  /* 95 */ &Finalize::vUndefined,         // CN_QUEUE
  /* 96 */ &Finalize::vUndefined,         // CN_QUEUEBUFFER
  /*    */ &Finalize::vUndefined,         // CN_QUEUES
  
  /*    */ &Finalize::vUndefined,         // CN__R__
  /*    */ &Finalize::vUndefined,         // CN_READAPI
  /*    */ &Finalize::vUndefined,         // CN_RECORD
  /*    */ &Finalize::vUndefined,         // CN_RECORDBUFFER
  /*    */ &Finalize::vUndefined,         // CN_RECORDDATA
  /*    */ &Finalize::vUndefined,         // CN_REGSET
  /* 77 */ &Finalize::vUndefined,         // CN_RELOP
  /* 77 */ &Finalize::vUndefined,         // CN_REQUEST
  /*    */ &Finalize::vUndefined,         // CN_RESOURCETRACKER
  /*    */ &Finalize::vUndefined,         // CN_RESPONSE
  /*    */ &Finalize::vUndefined,         // CN_RESPONSEVEC
  /*    */ &Finalize::vUndefined,         // CN_RESULT
  /*    */ &Finalize::vUndefined,         // CN_RN_RAND
  /*    */ &Finalize::vUndefined,         // CN_ROUTINE
  /*    */ &Finalize::vUndefined,         // CN_ROW
  /*102 */ &Finalize::vUndefined,         // CN_ROWAGG
  /*    */ &Finalize::vUndefined,         // CN_ROWCOMPACTOR
  /*105 */ &Finalize::vUndefined,         // CN_ROWONCE
  /*    */ &Finalize::vUndefined,         // CN_ROWSINGLE
  /*    */ &Finalize::vUndefined,         // CN_ROWTRAY
  /*    */ &Finalize::vUndefined,         // CN_RUCPU
  /*    */ &Finalize::vUndefined,         // CN_RUDIR
  /*    */ &Finalize::vUndefined,         // CN_RUFILE
  /*    */ &Finalize::vUndefined,         // CN_RUMEMORY
  /*    */ &Finalize::vUndefined,         // CN_RUMUTEX
  /*    */ &Finalize::vUndefined,         // CN_RUPASS
  /*    */ &Finalize::vUndefined,         // CN_RUSOCKET
  /*    */ &Finalize::vUndefined,         // CN_RUTIME
  /*    */ &Finalize::vUndefined,         // CN_RUWORK

  /*    */ &Finalize::vUndefined,         // CN__S__
  /*    */ &Finalize::vUndefined,         // CN_SCHEMA
  /*    */ &Finalize::vUndefined,         // CN_SCHEMATEST
  /*    */ &Finalize::vUndefined,         // CN_SEEME
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
  /*126 */ &Finalize::vUndefined,         // CN_STAGESPEC
  /*    */ &Finalize::vUndefined,         // CN_STATISTICS
  /*127 */ &Finalize::vUndefined,         // CN_STEPACCUM
  /*128 */ &Finalize::vUndefined,         // CN_STEPTIMING
  /*128 */ &Finalize::vUndefined,         // CN_STMT_SELECT
  /*126 */ &Finalize::vUndefined,         // CN_STRAND
  /* 13 */ &Finalize::vUndefined,         // CN_STREAMDEF
  /* 13 */ &Finalize::vUndefined,         // CN_STREAMSET
  /* 13 */ &Finalize::vUndefined,         // CN_STRING_T
  /* 12 */ &Finalize::vUndefined,         // CN_STZVAL  
  /*129 */ &Finalize::vUndefined,         // CN_SYSCALL
  
  /*    */ &Finalize::vUndefined,         // CN__T__
  /*130 */ &Table::vFinalize,             // CN_TABLE
  /*    */ &Finalize::vUndefined,         // CN_TABLEDEF
  /*    */ &Finalize::vUndefined,         // CN_TABLEPTN
  /*    */ &Finalize::vUndefined,         // CN_TABLEPTNVEC
  /* 37 */ &Finalize::vUndefined,         // CN_TAGSET
  /*131 */ &Finalize::vUndefined,         // CN_TASK
  /*134 */ &Finalize::vUndefined,         // CN_TASKLIST
  /*    */ &Finalize::vUndefined,         // CN_TASKQUEUE
  /*    */ &Finalize::vUndefined,         // CN_TEMPNAME
  /*    */ &Finalize::vUndefined,         // CN_TENANT
  /*    */ &Finalize::vUndefined,         // CN_TENANTS
  /*137 */ &Finalize::vUndefined,         // CN_THREAD
  /*    */ &Finalize::vUndefined,         // CN_THREADQUEUE
  /*    */ &Finalize::vUndefined,         // CN_THREADVEC
  /*142 */ &Finalize::vUndefined,         // CN_TIMER
  /*  7 */ &Finalize::vUndefined,         // CN_TMEVAL
  /*143 */ &Finalize::vUndefined,         // CN_TOKEN
  /*146 */ &Finalize::vUndefined,         // CN_TOKENVEC
  /*  7 */ &Finalize::vUndefined,         // CN_TSPVAL
  /*147 */ &Finalize::vUndefined,         // CN_TX
  /*148 */ &Finalize::vUndefined,         // CN_TXSET
  
  /*    */ &Finalize::vUndefined,         // CN__U__
  
  /*    */ &Finalize::vUndefined,         // CN__V__
  /*    */ &Finalize::vUndefined,         // CN_VORTEX
  
  /*    */ &Finalize::vUndefined,         // CN__W__
  /*149 */ &Finalize::vUndefined,         // CN_WILDERNESS
  /*150 */ &Finalize::vUndefined,         // CN_WORD
  /*    */ &Finalize::vUndefined,         // CN_WRITEBEHIND
  
  /*    */ &Finalize::vUndefined,         // CN__X__
  /*    */ &Finalize::vUndefined,         // CN_XMITS
  
  /*    */ &Finalize::vUndefined,         // CN__Y__
  
  /*    */ &Finalize::vUndefined,         // CN__Z__
    
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
