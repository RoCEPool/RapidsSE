/*============================================================================*/
/* Finalize.cpp -- Embedded Class Finalizers                                  */
/*============================================================================*/

#include "StorageEngine.h"
#include "SePrompt.hpp"

// Object's : Class::vFinalize
// Object's with no vUndefined defined :Finalize::vUndefined
// all embedded objects should have a vFinalize

pFINALIZE *gpFinalize[]
{
  /*  0 */ &Finalize::vUndefined,         // CN_NONE
  
  /*  1 */ &Finalize::vUndefined,         // CN_BCDVAL
  /*  3 */ &Finalize::vUndefined,         // CN_BOLVAL
  /*  4 */ &Finalize::vUndefined,         // CN_DTEVAL
  /*    */ &Finalize::vUndefined,         // CN_FIPVAL
  /*  6 */ &Finalize::vUndefined,         // CN_FLTVAL
  /*  7 */ &Finalize::vUndefined,         // CN_INTVAL
  /*  9 */ &Finalize::vUndefined,         // CN_IVLVAL
  /* 11 */ &Finalize::vUndefined,         // CN_PTRVAL
  /* 12 */ &Finalize::vUndefined,         // CN_STRVAL
  /* 14 */ &Finalize::vUndefined,         // CN_TMEVAL
  /* 15 */ &Finalize::vUndefined,         // CN_TSPVAL
  
  /* 19 */ &Finalize::vUndefined,         // CN_ACCESSPATH
  /*    */ &Finalize::vUndefined,         // CN_ACTION
  /*    */ &Finalize::vUndefined,         // CN_ACTIONSEQUENCE
  /*    */ &Finalize::vUndefined,         // CN_ACTIVITYMAP
  /*    */ &Finalize::vUndefined,         // CN_ACTIVITYMARK
  /*    */ &Finalize::vUndefined,         // CN_APISTATE
  /* 22 */ &Finalize::vUndefined,         // CN_ARG
  /* 23 */ &Finalize::vUndefined,         // CN_ARGUMENTS
  /* 24 */ &Finalize::vUndefined,         // CN_ATOMIC
  /* 25 */ &Finalize::vUndefined,         // CN_ATTACHMAP

  /* 29 */ &Finalize::vUndefined,         // CN_BPA
  /* 28 */ &Finalize::vUndefined,         // CN_BUFFERPOOL
  /*    */ &Finalize::vUndefined,         // CN_BYTERATE
  
  /* 30 */ &Finalize::vUndefined,         // CN_CARRYOVER
  /* 31 */ &Finalize::vUndefined,         // CN_COLUMN
  /* 32 */ &Finalize::vUndefined,         // CN_COLUMNVEC
  /*    */ &Finalize::vUndefined,         // CN_COMMAND
  /*    */ &Finalize::vUndefined,         // CN_COMPILER
  /*    */ &Finalize::vUndefined,         // CN_COMPLETER

  /*    */ &Finalize::vUndefined,         // CN_DECIDER
  /* 33 */ &Dic::vFinalize,               // CN_DIC
  /*    */ &Finalize::vUndefined,         // CN_DICPATH
  /* 34 */ &Finalize::vUndefined,         // CN_DICSTACK
  /* 35 */ &Finalize::vUndefined,         // CN_DIR
  
  /* 36 */ &Finalize::vUndefined,         // CN_ENTRY
  /* 37 */ &Finalize::vUndefined,         // CN_ENTRYBUFFER
  /*    */ &Finalize::vUndefined,         // CN_ENUMFLAGS
  /*    */ &Finalize::vUndefined,         // CN_EXPORTBUFFER

  /*    */ &Finalize::vUndefined,         // CN_FETCHBUFFER
  /* 39 */ &Finalize::vUndefined,         // CN_FILE
  /* 40 */ &Finalize::vUndefined,         // CN_FORMATTERS
  /* 41 */ &Finalize::vUndefined,         // CN_FREEBUFFER
  /* 42 */ &Finalize::vUndefined,         // CN_FREECHUNK
  /* 43 */ &Finalize::vUndefined,         // CN_FREEDNOT
  /* 44 */ &Finalize::vUndefined,         // CN_FREEENTRY
  
  /*    */ &Finalize::vUndefined,         // CN_GENVAR
  /* 48 */ &Finalize::vUndefined,         // CN_GRID
  
  /*    */ &Finalize::vUndefined,         // CN_HASHBUFFER
  /*    */ &Finalize::vUndefined,         // CN_HASHMAP
  /* 50 */ &Finalize::vUndefined,         // CN_HEAPBUFFER
  
  /* 51 */ &Index::vFinalize,             // CN_INDEX
  /* 52 */ &Finalize::vUndefined,         // CN_INDEXTREE
  /* 53 */ &Finalize::vUndefined,         // CN_INDEXTREEVEC
  /* 54 */ &Finalize::vUndefined,         // CN_IDXCOLVEC
  /* 55 */ &Finalize::vUndefined,         // CN_IDXVEC
  /*    */ &Finalize::vUndefined,         // CN_INGESTCHUNK
  /*    */ &Finalize::vUndefined,         // CN_IOCHUNK
  /*    */ &Finalize::vUndefined,         // CN_IOCOPY
  /* 57 */ &Finalize::vUndefined,         // CN_ITEM
  /* 58 */ &Finalize::vUndefined,         // CN_ITEMBUFFER
  
  /*    */ &Finalize::vUndefined,         // CN_JOB__BEGUN
  /*    */ &Finalize::vUndefined,         // CN_JOB__CANCEL
  /*    */ &Finalize::vUndefined,         // CN_JOB__CANCELLED
  /*    */ &Finalize::vUndefined,         // CN_JOB__END
  /*    */ &Finalize::vUndefined,         // CN_JOB__ENDED
  /*    */ &Finalize::vUndefined,         // CN_JOB__EXECUTE
  /*    */ &Finalize::vUndefined,         // CN_JOB__EXECUTED
  /*    */ &Finalize::vUndefined,         // CN_JOB__FIND
  /*    */ &Finalize::vUndefined,         // CN_JOB_ALARM
  /*    */ &Finalize::vUndefined,         // CN_JOB_ANALYZEINDEX
  /*    */ &Finalize::vUndefined,         // CN_JOB_ANALYZETABLE
  /*    */ &Finalize::vUndefined,         // CN_JOB_ANSWER
  /*    */ &Finalize::vUndefined,         // CN_JOB_BOOT
  /*    */ &Finalize::vUndefined,         // CN_JOB_CAT
  /*    */ &Finalize::vUndefined,         // CN_JOB_CF
  /*    */ &Finalize::vUndefined,         // CN_JOB_COMPACTINDEX
  /*    */ &Finalize::vUndefined,         // CN_JOB_CONNECTCLUSTER
  /*    */ &Finalize::vUndefined,         // CN_JOB_CONNECTOR
  /*    */ &Finalize::vUndefined,         // CN_JOB_COPYBUFFER
  /*    */ &Finalize::vUndefined,         // CN_JOB_CREATECATALOG
  /*    */ &Finalize::vUndefined,         // CN_JOB_CREATEINDEX
  /*    */ &Finalize::vUndefined,         // CN_JOB_CREATESCHEMA
  /*    */ &Finalize::vUndefined,         // CN_JOB_CREATETABLE
  /*    */ &Finalize::vUndefined,         // CN_JOB_CREATETENANT
  /*    */ &Finalize::vUndefined,         // CN_JOB_DOCOL
  /*    */ &Finalize::vUndefined,         // CN_JOB_DROPCATALOG
  /*    */ &Finalize::vUndefined,         // CN_JOB_DROPINDEX
  /*    */ &Finalize::vUndefined,         // CN_JOB_DROPSCHEMA
  /*    */ &Finalize::vUndefined,         // CN_JOB_DROPTABLE
  /*    */ &Finalize::vUndefined,         // CN_JOB_DROPTENANT
  /*    */ &Finalize::vUndefined,         // CN_JOB_DUMPINDEX
  /*    */ &Finalize::vUndefined,         // CN_JOB_DUMPTABLE
  /*    */ &Finalize::vUndefined,         // CN_JOB_EXPORT
  /*    */ &Finalize::vUndefined,         // CN_JOB_FETCHROWS
  /*    */ &Finalize::vUndefined,         // CN_JOB_GENERATE
  /*    */ &Finalize::vUndefined,         // CN_JOB_HEALTH
  /*    */ &Finalize::vUndefined,         // CN_JOB_INGEST
  /*    */ &Finalize::vUndefined,         // CN_JOB_INSERTROW
  /*    */ &Finalize::vUndefined,         // CN_JOB_INSERTTABLE
  /*    */ &Finalize::vUndefined,         // CN_JOB_KIDEXPORT
  /*    */ &Finalize::vUndefined,         // CN_JOB_LF
  /*    */ &Finalize::vUndefined,         // CN_JOB_LISTCATALOGS
  /*    */ &Finalize::vUndefined,         // CN_JOB_LISTCOLUMNS
  /*    */ &Finalize::vUndefined,         // CN_JOB_LISTIDXCOLUMNS
  /*    */ &Finalize::vUndefined,         // CN_JOB_LISTINDEXES
  /*    */ &Finalize::vUndefined,         // CN_JOB_LISTPARTCOLUMNS
  /*    */ &Finalize::vUndefined,         // CN_JOB_LISTPARTITIONS
  /*    */ &Finalize::vUndefined,         // CN_JOB_LISTSCHEMAS
  /*    */ &Finalize::vUndefined,         // CN_JOB_LISTTABLES
  /*    */ &Finalize::vUndefined,         // CN_JOB_LISTTENANTS
  /*    */ &Finalize::vUndefined,         // CN_JOB_LOADTABLE
  /*    */ &Finalize::vUndefined,         // CN_JOB_MONITOR
  /*    */ &Finalize::vUndefined,         // CN_JOB_NETHOG
  /*    */ &Finalize::vUndefined,         // CN_JOB_NODEINFO
  /*    */ &Finalize::vUndefined,         // CN_JOB_PERIODIC
  /*    */ &Finalize::vUndefined,         // CN_JOB_PING
  /*    */ &Finalize::vUndefined,         // CN_JOB_PUBLISH
  /*    */ &Finalize::vUndefined,         // CN_JOB_PURGE
  /*    */ &Finalize::vUndefined,         // CN_JOB_PWF
  /*    */ &Finalize::vUndefined,         // CN_JOB_RECEIVER
  /*    */ &Finalize::vUndefined,         // CN_JOB_REPEATER
  /*    */ &Finalize::vUndefined,         // CN_JOB_REPORT
  /*    */ &Finalize::vUndefined,         // CN_JOB_SANITY
  /*    */ &Finalize::vUndefined,         // CN_JOB_SELECTTABLE
  /*    */ &Finalize::vUndefined,         // CN_JOB_SHOWCLUSTER
  /*    */ &Finalize::vUndefined,         // CN_JOB_SINK
  /*    */ &Finalize::vUndefined,         // CN_JOB_SIZEINDEX
  /*    */ &Finalize::vUndefined,         // CN_JOB_SIZETABLE
  /*    */ &Finalize::vUndefined,         // CN_JOB_SPACE
  /*    */ &Finalize::vUndefined,         // CN_JOB_START
  /*    */ &Finalize::vUndefined,         // CN_JOB_STARTCLUSTER
  /*    */ &Finalize::vUndefined,         // CN_JOB_STATUS
  /*    */ &Finalize::vUndefined,         // CN_JOB_SUBEXPORTER
  /*    */ &Finalize::vUndefined,         // CN_JOB_SUBFETCH
  /*    */ &Finalize::vUndefined,         // CN_JOB_SUBINGESTER
  /*    */ &Finalize::vUndefined,         // CN_JOB_SUBINSERTER
  /*    */ &Finalize::vUndefined,         // CN_JOB_SUBLOADER
  /*    */ &Finalize::vUndefined,         // CN_JOB_SUBPUBLISH
  /*    */ &Finalize::vUndefined,         // CN_JOB_SUBPURGER
  /*    */ &Finalize::vUndefined,         // CN_JOB_SUBTRUNCATE
  /*    */ &Finalize::vUndefined,         // CN_JOB_SUBWALKER
  /*    */ &Finalize::vUndefined,         // CN_JOB_SUPERVISOR
  /*    */ &Finalize::vUndefined,         // CN_JOB_TABLESIZES
  /*    */ &Finalize::vUndefined,         // CN_JOB_TRUNCATETABLE
  /*    */ &Finalize::vUndefined,         // CN_JOB_USESCHEMA
  /*    */ &Finalize::vUndefined,         // CN_JOB_VERIFYINDEX
  /*    */ &Finalize::vUndefined,         // CN_JOB_VISUALIZEINDEX
  /*    */ &Finalize::vUndefined,         // CN_JOB_WAITCLUSTER
  /*    */ &Finalize::vUndefined,         // CN_JOB_WALK
  /*    */ &Finalize::vUndefined,         // CN_JOB_WRITEBUFFER
  /*    */ &Finalize::vUndefined,         // CN_JOB_WRITEROW
  
  /*    */ &Finalize::vUndefined,         // CN_JOBLIST
  
  /* 59 */ &Finalize::vUndefined,         // CN_JUMBOPERSISTENT
  /* 60 */ &Finalize::vUndefined,         // CN_JUMBOTRANSIENT
  
  /* 61 */ &Finalize::vUndefined,         // CN_KEY
  /* 64 */ &Finalize::vUndefined,         // CN_KEYWRK
  
  /* 65 */ &Finalize::vUndefined,         // CN_LARGEPERSISTENT
  /* 66 */ &Finalize::vUndefined,         // CN_LARGETRANSIENT
  /*    */ &Finalize::vUndefined,         // CN_LOADCHUNK
  /* 71 */ &Finalize::vUndefined,         // CN_LOADMAP
  /*    */ &Finalize::vUndefined,         // CN_LOCAL
  /*    */ &Finalize::vUndefined,         // CN_LOCALSET
  /* 72 */ &Finalize::vUndefined,         // CN_LOCK
  
  /* 73 */ &Finalize::vUndefined,         // CN_MEDIUMPERSISTENT
  /* 74 */ &Finalize::vUndefined,         // CN_MEDIUMTRANSIENT
  /*    */ &Finalize::vUndefined,         // CN_MEMTEST
  /*114 */ &Finalize::vUndefined,         // CN_MUTEX
  
  /* 75 */ &Finalize::vUndefined,         // CN_NODE
  /*    */ &Finalize::vUndefined,         // CN_NOW
  
  /* 77 */ &Finalize::vUndefined,         // CN_OP
  
  /* 78 */ &Finalize::vUndefined,         // CN_PARSER
  /* 82 */ &Finalize::vUndefined,         // CN_PATHVEC
  /*    */ &Finalize::vUndefined,         // CN_PIPE
  /* 86 */ &Finalize::vUndefined,         // CN_PLAN
  /* 87 */ &Finalize::vUndefined,         // CN_PLANNER
  /* 90 */ &Finalize::vUndefined,         // CN_PREDICATE
  /* 91 */ &Finalize::vUndefined,         // CN_PREDICATEVEC
  /* 89 */ &Finalize::vUndefined,         // CN_PTHREAD
  /* 94 */ &Finalize::vUndefined,         // CN_PTNCOLVEC
  
  /* 95 */ &Finalize::vUndefined,         // CN_QUEUE
  /* 96 */ &Finalize::vUndefined,         // CN_QUEUEBUFFER
  /*    */ &Finalize::vUndefined,         // CN_QUEUES
  
  /*    */ &Finalize::vUndefined,         // CN_RECORDBUFFER
  /*    */ &Finalize::vUndefined,         // CN_RESOURCETRACKER
  /*102 */ &Finalize::vUndefined,         // CN_ROWAGG
  /*    */ &Finalize::vUndefined,         // CN_ROWBUFFER
  /*    */ &Finalize::vUndefined,         // CN_ROWDECODER
  /*    */ &Finalize::vUndefined,         // CN_ROWENCODER
  /*105 */ &Finalize::vUndefined,         // CN_ROWONCE
  /*    */ &Finalize::vUndefined,         // CN_ROWTRAY
  /*106 */ &Finalize::vUndefined,         // CN_ROWWRK
  
  /*    */ &Finalize::vUndefined,         // CN_SCHEMATEST
  /*    */ &Finalize::vUndefined,         // CN_SEEME
  /*115 */ &Finalize::vUndefined,         // CN_SEPROMPT
  /*    */ &Finalize::vUndefined,         // CN_SESSION
  /*    */ &Finalize::vUndefined,         // CN_SESSIONSTATE
  /*116 */ &Finalize::vUndefined,         // CN_SINCE
  /*118 */ &Finalize::vUndefined,         // CN_SKIPLIST
  /*119 */ &Finalize::vUndefined,         // CN_SKIPNODE
  /*120 */ &Finalize::vUndefined,         // CN_SLAB
  /*121 */ &Finalize::vUndefined,         // CN_SLABALLOCATOR
  /*122 */ &Slots::vFinalize,             // CN_SLOTS
  /*123 */ &Finalize::vUndefined,         // SN_SMALLPERSISTENT
  /*124 */ &Finalize::vUndefined,         // SN_SMALLTRANSIENT
  /*125 */ &Finalize::vUndefined,         // CN_SOCKET
  /*126 */ &Finalize::vUndefined,         // CN_STACK
  /*    */ &Finalize::vUndefined,         // CN_STATISTICS
  /*127 */ &Finalize::vUndefined,         // CN_STEPACCUM
  /*128 */ &Finalize::vUndefined,         // CN_STEPTIMING
  /* 13 */ &Finalize::vUndefined,         // CN_STRING_T
  /*129 */ &Finalize::vUndefined,         // CN_SYSCALL
  
  /*130 */ &Table::vFinalize,             // CN_TABLE
  /*    */ &Finalize::vUndefined,         // CN_TABLEPTN
  /*    */ &Finalize::vUndefined,         // CN_TABLEPTNVEC
  /*131 */ &Finalize::vUndefined,         // CN_TASK
  /*134 */ &Finalize::vUndefined,         // CN_TASKLIST
  /*    */ &Finalize::vUndefined,         // CN_TASKQUEUE
  /*    */ &Finalize::vUndefined,         // CN_TEMPNAME
  /*137 */ &Finalize::vUndefined,         // CN_THREAD
  /*    */ &Finalize::vUndefined,         // CN_THREADVEC
  /*141 */ &Finalize::vUndefined,         // CN_TIME
  /*142 */ &Finalize::vUndefined,         // CN_TIMER
  /*143 */ &Finalize::vUndefined,         // CN_TOKEN
  /*144 */ &Finalize::vUndefined,         // CN_TOKENISER
  /*145 */ &Finalize::vUndefined,         // CN_TOKENARY
  /*146 */ &Finalize::vUndefined,         // CN_TOKENVEC
  /*147 */ &Finalize::vUndefined,         // CN_TX
  /*148 */ &Finalize::vUndefined,         // CN_TXSET
  
  /*149 */ &Finalize::vUndefined,         // CN_WILDERNESS
  /*150 */ &Finalize::vUndefined,         // CN_WORD
  
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
