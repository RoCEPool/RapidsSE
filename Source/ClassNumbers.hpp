/*============================================================================*/
/* ClassNumbers.hpp - class numbers                                           */
/*============================================================================*/
#pragma once

#include "Base.h"

#include "Stz.hpp"

namespace Base
{

/*----------------------------------------------------------------------------*/
/* class numbers                                                              */
/*----------------------------------------------------------------------------*/
enum eCN : u2 // Class Number
  {
    /*  0 */ CN_NONE=0,
    
    /*  1 */ CN_BCDVAL,
    /*  2 */ CN_BINVAL,
    /*  3 */ CN_BOLVAL,
    /*  4 */ CN_DTEVAL,
    /*  5 */ CN_ENTWRK,
    /*  6 */ CN_FLTVAL,
    /*  7 */ CN_INTVAL,
    /*  8 */ CN_IPVAL,
    /*  9 */ CN_IVLVAL,
    /* 10 */ CN_OBJVAL,
    /* 11 */ CN_PTRVAL,
    /* 12 */ CN_STRVAL,
    /* 13 */ CN_STRVAR,
    /* 14 */ CN_TMEVAL,
    /* 15 */ CN_TSPVAL,
    /* 16 */ CN_VALREG,
    /* 17 */ CN_VALSET,
    /* 18 */ CN_VALWRK,
  
    /* 19 */ CN_ACCESSPATH,
    /* 20 */ CN_ACCUM,
    /* 21 */ CN_ACCUMVEC,
    /* 22 */ CN_ARG,
    /* 23 */ CN_ARGUMENTS,
    /* 24 */ CN_ATOMIC,
    /* 25 */ CN_ATTACHMAP,
    
    /* 26 */ CN_BITARRAY,
    /* 27 */ CN_BITMAP,
    /* 28 */ CN_BUFFERPOOL,
    /* 29 */ CN_BPA,
  
    /* 30 */ CN_CARRYOVER,
    /* 31 */ CN_COLUMN,
    /* 32 */ CN_COLUMNVEC,
    /*    */ CN_COMMANDPROCESSOR,
  
    /* 33 */ CN_DIC,
    /*    */ CN_DICPATH,
    /* 34 */ CN_DICSTACK,
    /* 35 */ CN_DIR,
  
    /* 36 */ CN_ENTRY,
    /* 37 */ CN_ENTRYBUFFER,
    /* 38 */ CN_ENTRYVEC,
    /*    */ CN_FETCHBUFFER,
    
    /* 39 */ CN_FILE,
    /* 40 */ CN_FORMATTERS,
    /* 41 */ CN_FREEBUFFER,
    /* 42 */ CN_FREECHUNK,
    /* 43 */ CN_FREEDNOT,
    /* 44 */ CN_FREEENTRY,
    
    /* 45 */ CN_GRAPH,
    /* 46 */ CN_GRAPHCURSOR,
    /* 47 */ CN_GRAPHNODE,
    /* 48 */ CN_GRID,
    /* 49 */ CN_GROUPBY,
  
    /* 50 */ CN_HEAPBUFFER,
    
    /* 51 */ CN_INDEX,
    /* 52 */ CN_INDEXTREE,
    /* 53 */ CN_INDEXTREEVEC,
    /* 54 */ CN_IDXCOLVEC,
    /* 55 */ CN_IDXVEC,
    /* 56 */ CN_INDEXLEVELS,
    /* 57 */ CN_ITEM,
    /* 58 */ CN_ITEMBUFFER,
    
    /*    */ CN_JOB__BEGUN,
    /*    */ CN_JOB__CANCEL,
    /*    */ CN_JOB__CANCELLED,
    /*    */ CN_JOB__END,
    /*    */ CN_JOB__ENDED,
    /*    */ CN_JOB__EXECUTE,
    /*    */ CN_JOB__EXECUTED,
    
    /*    */ CN_JOB_ANALYZEINDEX,
    /*    */ CN_JOB_ANALYZETABLE,
    /*    */ CN_JOB_ANSWER,
    /*    */ CN_JOB_BOOT,
    /*    */ CN_JOB_CF,
    /*    */ CN_JOB_COMPACTINDEX,
    /*    */ CN_JOB_CONNECTOR,
    /*    */ CN_JOB_CREATECATALOG,
    /*    */ CN_JOB_CREATEINDEX,
    /*    */ CN_JOB_CREATESCHEMA,
    /*    */ CN_JOB_CREATETABLE,
    /*    */ CN_JOB_CREATETENANT,
    /*    */ CN_JOB_DOCOL,
    /*    */ CN_JOB_DROPCATALOG,
    /*    */ CN_JOB_DROPINDEX,
    /*    */ CN_JOB_DROPSCHEMA,
    /*    */ CN_JOB_DROPTABLE,
    /*    */ CN_JOB_DROPTENANT,
    /*    */ CN_JOB_DUMPINDEX,
    /*    */ CN_JOB_FETCHROWS,
    /*    */ CN_JOB_LF,
    /*    */ CN_JOB_LISTCATALOGS,
    /*    */ CN_JOB_LISTCOLUMNS,
    /*    */ CN_JOB_LISTIDXCOLUMNS,
    /*    */ CN_JOB_LISTPARTCOLUMNS,
    /*    */ CN_JOB_LISTSCHEMAS,
    /*    */ CN_JOB_LISTTABLES,
    /*    */ CN_JOB_LISTTENANTS,
    /*    */ CN_JOB_LOADBUFFER,
    /*    */ CN_JOB_LOADTABLE,
    /*    */ CN_JOB_PROMPT,
    /*    */ CN_JOB_PWF,
    /*    */ CN_JOB_RECEIVER,
    /*    */ CN_JOB_SANITY,
    /*    */ CN_JOB_SIZEINDEX,
    /*    */ CN_JOB_SIZETABLE,
    /*    */ CN_JOB_SPACE,
    /*    */ CN_JOB_START,
    /*    */ CN_JOB_STATUS,
    /*    */ CN_JOB_SUBFETCH,
    /*    */ CN_JOB_SUBLOADER,
    /*    */ CN_JOB_SUBTRUNCATE,
    /*    */ CN_JOB_TABLESIZES,
    /*    */ CN_JOB_TRUNCATETABLE,
    /*    */ CN_JOB_USESCHEMA,
    /*    */ CN_JOB_VERIFYINDEX,
    /*    */ CN_JOB_VISUALIZEINDEX,
    
    /*    */ CN_JOBS,
    /* 59 */ CN_JUMBOPERSISTENT,
    /* 60 */ CN_JUMBOTRANSIENT,
  
    /* 61 */ CN_KEY,
    /* 62 */ CN_KEYVAL,
    /* 63 */ CN_KEYVEC,
    /* 64 */ CN_KEYWRK,
  
    /* 65 */ CN_LARGEPERSISTENT,
    /* 66 */ CN_LARGETRANSIENT,
    
    /* 67 */ CN_LINEITEM,
    /* 68 */ CN_LOADCHUNK,
    /* 69 */ CN_LOADER,
    /* 70 */ CN_LOADERSUB,
    /* 71 */ CN_LOADMAP,
    /* 72 */ CN_LOCK,
    
    /* 73 */ CN_MEDIUMPERSISTENT,
    /* 74 */ CN_MEDIUMTRANSIENT,

    /* 75 */ CN_NODE,
  
    /* 77 */ CN_OP,

    /* 78 */ CN_PARSER,
    /* 80 */ CN_PARTITIONSCHEDULER,

    /* 82 */ CN_PATHVEC,
    /* 83 */ CN_PERFORMANCEBUFFER,
    /* 84 */ CN_PERFORMANCEENTRY,
    /* 85 */ CN_PERFORMANCETRACE,
    /* 86 */ CN_PLAN,
    /* 87 */ CN_PLANNER,
    /* 88 */ CN_PLANLIST,
    /* 89 */ CN_POSIXTHREAD,
    /* 90 */ CN_PREDICATE,
    /* 91 */ CN_PREDICATEVEC,
    /* 92 */ CN_PROXY,
    /* 93 */ CN_PROXYSET,
    /* 94 */ CN_PTNCOLVEC,
  
    /* 95 */ CN_QUEUE,
    /* 96 */ CN_QUEUEBUFFER,
    /*    */ CN_QUEUES,
  
    /* 97 */ CN_RBCURSOR,
    /*    */ CN_REGFRAME,
    
    /*101 */ CN_RING,
    /*102 */ CN_ROARRAY,
    /*103 */ CN_ROWBUFFER,
    /*104 */ CN_ROWCURSOR,
    /*105 */ CN_ROWONCE,
    /*106 */ CN_ROWWRK,
  
    /*107 */ CN_SCANBUFFER,
    /*108 */ CN_SCRATCH,

    /*110 */ CN_SEITF,

    /*113 */ CN_SELCURSOR,
    /*114 */ CN_SEMAPHORE,
    /*115 */ CN_SEPROMPT,
    /*116 */ CN_SINCE,
    /*117 */ CN_SKIPCURSOR,
    /*118 */ CN_SKIPLIST,
    /*119 */ CN_SKIPNODE,
    /*120 */ CN_SLAB,
    /*121 */ CN_SLABALLOCATOR,
    /*122 */ CN_SLOTS,
    /*123 */ CN_SMALLPERSISTENT,
    /*124 */ CN_SMALLTRANSIENT,
    /*125 */ CN_SOCKET,
    /*126 */ CN_STACK,
    /*127 */ CN_STEPACCUM,
    /*128 */ CN_STEPARRAY,
    /*129 */ CN_SYSCALL,
    
    /*130 */ CN_TABLE,
    /*    */ CN_TABLEPTN,
    /*    */ CN_TABLEPTNVEC,
    /*131 */ CN_TASK,
    /*    */ CN_TASKFIFO,
    /*133 */ CN_TASKLIST,
    /*134 */ CN_TASKQUEUE,
    /*135 */ CN_SERVERSCHEDULER,

    /*137 */ CN_THREAD,

    /*139 */ CN_THREADLIST,
    /*140 */ CN_THREADQUEUE,
    /*    */ CN_THREADVECTOR,
    /*141 */ CN_TIME,
    /*142 */ CN_TIMER,
    /*143 */ CN_TOKEN,
    /*144 */ CN_TOKENISER,
    /*145 */ CN_TOKENARY,
    /*146 */ CN_TOKENVEC,
    /*147 */ CN_TX,
    /*148 */ CN_TXSET,

    /*149 */ CN_WILDERNESS,
    /*150 */ CN_WORD,
    /*    */ CN_WORKINGBUFFER,
    /*    */ CN_WORKINGROW,
        
    /*151 */ CN_ZSMALL_,
    /*152 */ CN_ZMEDIUM_,
    /*153 */ CN_ZLARGE_,
    /*154 */ CN_ZJUMBO_,
  
    /*155 */ CN_ENUMS
  };
  
/*----------------------------------------------------------------------------*/
/* class names                                                                */
/*----------------------------------------------------------------------------*/
  static Stz* aCN[]
  {
    /*  0 */ "NONE"_Stz,     // CN_NONE
  
    /*  1 */ "BCDVAL"_Stz,   // CN_BCDVAL
    /*  2 */ "BINVAL"_Stz,   // CN_BINVAL
    /*  3 */ "BOLVAL"_Stz,   // CN_BOLVAL
    /*  4 */ "DTEVAL"_Stz,   // CN_DTEVAL
    /*  5 */ "ENTWRK"_Stz,   // CN_ENTWRK
    /*  6 */ "FLTVAL"_Stz,   // CN_FLTVAL
    /*  7 */ "INTVAL"_Stz,   // CN_INTVAL
    /*  8 */ "IPVAL"_Stz,    // CN_IPVAL
    /*  9 */ "IVLVAL"_Stz,   // CN_IVLVAL
    /* 10 */ "OBJVAL"_Stz,   // CN_OBJVAL
    /* 11 */ "PTRVAL"_Stz,   // CN_PTRVAL
    /* 12 */ "STRVAL"_Stz,   // CN_STRVAL
    /* 13 */ "STRVAR"_Stz,   // CN_STRVAR
    /* 14 */ "TMEVAL"_Stz,   // CN_TMEVAL
    /* 15 */ "TSPVAL"_Stz,   // CN_TSPVAL
    /* 16 */ "VALREG"_Stz,   // CN_VALREG
    /* 17 */ "VALSET"_Stz,   // CN_VALSET
    /* 18 */ "VALWRK"_Stz,   // CN_VALWRK
    
    /* 19 */ "ACCESSPATH"_Stz,         // CN_ACCESSPATH
    /* 20 */ "ACCUM"_Stz,              // CN_ACCUM
    /* 21 */ "ACCUMVEC"_Stz,           // CN_ACCUMVEC
    /* 22 */ "ARG"_Stz,                // CN_ARG
    /* 23 */ "ARGUMENTS"_Stz,          // CN_ARGUMENTS
    /* 24 */ "ATOMIC"_Stz,             // CN_ATOMIC
    /* 25 */ "ATTACHMAP"_Stz,          // CN_ATTACHMAP
    
    /* 26 */ "BITARRAY"_Stz,           // CN_BITARRAY
    /* 27 */ "BITMAP"_Stz,             // CN_BITMAP
    /* 28 */ "BUFFERPOOL"_Stz,         // CN_BUFFERPOOL
    /* 29 */ "BPA"_Stz,                // CN_BPA
  
    /* 30 */ "CARRYOVER"_Stz,          // CN_CARRYOVER
    /* 31 */ "COLUMN"_Stz,             // CN_COLUMN
    /* 32 */ "COLUMNVEC"_Stz,          // CN_COLUMNVEC
    /*    */ "COMMANDPROCESSOR"_Stz,   // CN_COMMANDPROCESSOR
    
    /* 33 */ "DIC"_Stz,                // CN_DIC
    /*    */ "DICPATH"_Stz,            // CN_DICPATH
    /* 34 */ "DICSTACK"_Stz,           // CN_DICSTACK
    /* 35 */ "DIR"_Stz,                // CN_DIR
    
    /* 36 */ "ENTRY"_Stz,              // CN_ENTRY
    /* 37 */ "ENTRYBUFFER"_Stz,        // CN_ENTRYBUFFER
    /* 38 */ "ENTRYVEC"_Stz,           // CN_ENTRYVEC
    
    /* 38 */ "FETCHBUFFER"_Stz,        // CN_FETCHBUFFER
    /* 39 */ "FILE"_Stz,               // CN_FILE
    /* 40 */ "FORMATTERS"_Stz,         // CN_FORMATTERS
    /* 41 */ "FREEBUFFER"_Stz,         // CN_FREEBUFFER
    /* 42 */ "FREECHUNK"_Stz,          // CN_FREECHUNK
    /* 43 */ "FREEDNOT"_Stz,           // CN_FREEDNOT
    /* 44 */ "FREEENTRY"_Stz,          // CN_FREEENTRY
    
    /* 45 */ "GRAPH"_Stz,              // CN_GRAPH
    /* 46 */ "GRAPHCURSOR"_Stz,        // CN_GRAPHCURSOR
    /* 47 */ "GRAPHNODE"_Stz,          // CN_GRAPHNODE
    /* 48 */ "GRID"_Stz,               // CN_GRID
    /* 49 */ "GROUPBY"_Stz,            // CN_GROUPBY
  
    /* 50 */ "HEAPBUFFER"_Stz,         // CN_HEAPBUFFER
    
    /* 51 */ "INDEX"_Stz,              // CN_INDEX
    /* 52 */ "INDEXTREE"_Stz,          // CN_INDEXTREE
    /* 53 */ "INDEXTREEVEC"_Stz,       // CN_INDEXTREEVEC
    /* 54 */ "IDXCOLVEC"_Stz,          // CN_IDXCOLVEC
    /* 55 */ "IDXVEC"_Stz,             // CN_IDXVEC
    /* 56 */ "INDEXLEVELS"_Stz,        // CN_INDEXLEVELS
    /* 57 */ "ITEM"_Stz,               // CN_ITEM
    /* 58 */ "ITEMBUFFER"_Stz,         // CN_ITEMBUFFER
    
    /*    */ "JOB__BEGUN"_Stz,         // CN_JOB__BEGUN
    /*    */ "JOB__CANCEL"_Stz,        // CN_JOB__CANCEL
    /*    */ "JOB__CANCELLED"_Stz,     // CN_JOB__CANCELLED
    /*    */ "JOB__END"_Stz,           // CN_JOB__END
    /*    */ "JOB__ENDED"_Stz,         // CN_JOB__ENDED
    /*    */ "JOB__EXECUTE"_Stz,       // CN_JOB__EXECUTE
    /*    */ "JOB__EXECUTED"_Stz,      // CN_JOB__EXECUTED
    
    /*    */ "JOB_ANALYZEINDEX"_Stz,   // CN_JOB_ANALYZEINDEX
    /*    */ "JOB_ANALYZETABLE"_Stz,   // CN_JOB_ANALYZETABLE
    /*    */ "JOB_ANSWER"_Stz,         // CN_JOB_ANSWER
    /*    */ "JOB_BOOT"_Stz,           // CN_JOB_BOOT
    /*    */ "JOB_CF"_Stz,             // CN_JOB_CF
    /*    */ "JOB_COMPACTINDEX"_Stz,   // CN_JOB_COMPACTINDEX
    /*    */ "JOB_CONNECTOR"_Stz,      // CN_JOB_CONNECTOR
    /*    */ "JOB_CREATECATALOG"_Stz,  // CN_JOB_CREATECATALOG
    /*    */ "JOB_CREATEINDEX"_Stz,    // CN_JOB_CREATEINDEX
    /*    */ "JOB_CREATESCHEMA"_Stz,   // CN_JOB_CREATESCHEMA
    /*    */ "JOB_CREATETABLE"_Stz,    // CN_JOB_CREATETABLE
    /*    */ "JOB_CREATETENANT"_Stz,   // CN_JOB_CREATETENANT
    /*    */ "JOB_DOCOL"_Stz,          // CN_JOB_DOCOL
    /*    */ "JOB_DROPCATALOG"_Stz,    // CN_JOB_DROPCATALOG
    /*    */ "JOB_DROPINDEX"_Stz,      // CN_JOB_DROPINDEX
    /*    */ "JOB_DROPSCHEMA"_Stz,     // CN_JOB_DROPSCHEMA
    /*    */ "JOB_DROPTABLE"_Stz,      // CN_JOB_DROPTABLE
    /*    */ "JOB_DROPTENANT"_Stz,     // CN_JOB_DROPTENANT
    /*    */ "JOB_DUMPINDEX"_Stz,      // CN_JOB_DUMPINDEX
    /*    */ "CN_JOB_FETCHROWS"_Stz,   // CN_JOB_FETCHROWS
    /*    */ "JOB_LF"_Stz,             // CN_JOB_LF
    /*    */ "JOB_LISTCATALOGS"_Stz,   // CN_JOB_LISTCATALOGS
    /*    */ "JOB_LISTCOLUMNS"_Stz,    // CN_JOB_LISTCOLUMNS
    /*    */ "JOB_LISTIDXCOLUMNS"_Stz, // CN_JOB_LISTIDXCOLUMNS
    /*    */ "JOB_LISTPARTCOLUMNS"_Stz,// CN_JOB_LISTPARTCOLUMNS
    /*    */ "JOB_LISTSCHEMAS"_Stz,    // CN_JOB_LISTSCHEMAS
    /*    */ "JOB_LISTTABLES"_Stz,     // CN_JOB_LISTTABLES
    /*    */ "JOB_LISTTENANTS"_Stz,    // CN_JOB_LISTTENANTS
    /*    */ "JOB_LOADBUFFER"_Stz,     // CN_JOB_LOADBUFFER
    /*    */ "JOB_LOADTABLE"_Stz,      // CN_JOB_LOADTABLE
    /*    */ "JOB_PROMPT"_Stz,         // CN_JOB_PROMPT
    /*    */ "JOB_PWF"_Stz,            // CN_JOB_PWF
    /*    */ "JOB_RECEIVER"_Stz,       // CN_JOB_RECEIVER
    /*    */ "JOB_SANITY"_Stz,         // CN_JOB_SANITY
    /*    */ "JOB_SIZEINDEX"_Stz,      // CN_JOB_SIZEINDEX
    /*    */ "JOB_SIZETABLE"_Stz,      // CN_JOB_SIZETABLE
    /*    */ "JOB_SPACE"_Stz,          // CN_JOB_SPACE
    /*    */ "JOB_START"_Stz,          // CN_JOB_START
    /*    */ "JOB_STATUS"_Stz,         // CN_JOB_STATUS
    /*    */ "JOB_SUBFETCH"_Stz,       // CN_JOB_SUBFETCH
    /*    */ "JOB_SUBLOADER"_Stz,      // CN_JOB_SUBLOADER
    /*    */ "JOB_SUBTRUNCATE"_Stz,    // CN_JOB_SUBTRUNCATE
    /*    */ "JOB_TABLESIZES"_Stz,     // CN_JOB_TABLESIZES
    /*    */ "JOB_TRUNCATETABLE"_Stz,  // CN_JOB_TRUNCATETABLE
    /*    */ "JOB_USESCHEMA"_Stz,      // CN_JOB_USESCHEMA
    /*    */ "JOB_VERIFYINDEX"_Stz,    // CN_JOB_VERIFYINDEX
    /*    */ "JOB_VISUALIZEINDEX"_Stz, // CN_JOB_VISUALIZEINDEX
    
    /*    */ "JOBS"_Stz,               // CN_JOBS
    /* 59 */ "JUMBOPERSISTENT"_Stz,    // CN_JUMBOPERSISTENT
    /* 60 */ "JUMBOTRANSIENT"_Stz,     // CN_JUMBOTRANSIENT
    
    /* 61 */ "KEY"_Stz,                // CN_KEY
    /* 62 */ "KEYVAL"_Stz,             // CN_KEYVAL
    /* 63 */ "KEYVEC"_Stz,             // CN_KEYVEC
    /* 64 */ "KEYVEC"_Stz,             // CN_KEYVEC
  
    /* 65 */ "LARGEPERSISTENT"_Stz,    // CN_LARGEPERSISTENT
    /* 66 */ "LARGETRANSIENT"_Stz,     // CN_LARGETRANSIENT
    /* 67 */ "LINEITEM"_Stz,           // CN_LINEITEM
    /* 68 */ "LOADCHUNK"_Stz,          // CN_LOADCHUNK
    /* 69 */ "LOADER"_Stz,             // CN_LOADER
    /* 70 */ "LOADERSUB"_Stz,          // CN_LOADERSUB
    /* 71 */ "LOADMAP"_Stz,            // CN_LOADMAP
    /* 72 */ "LOCK"_Stz,               // CN_LOCK

    /* 73 */ "MEDIUMPERSISTENT"_Stz,   // CN_MEDIUMPERSISTENT
    /* 74 */ "MEDIUMTRANSIENT"_Stz,    // CN_MEDIUMTRANSIENT
    
    /* 75 */ "NODE"_Stz,               // CN_NODE
  
    /* 77 */ "OP"_Stz,                 // CN_OP
  
    /* 78 */ "PARSER"_Stz,             // CN_PARSER
    /* 79 */ "PARTITION"_Stz,          // CN_PARTITION
    /* 80 */ "PARTITIONSCHEDULER"_Stz, // CN_PARTITIONSCHEDULER
    /* 81 */ "PARTITIONVEC"_Stz,       // CN_PARTITIONVEC
    /* 82 */ "PATHVEC"_Stz,            // CN_PATHVEC
    /* 83 */ "PERFORMANCEBUFFER"_Stz,  // CN_PERFORMANCEBUFFER
    /* 84 */ "PERFORMANCEENTRY"_Stz,   // CN_PERFORMANCEENTRY
    /* 85 */ "PERFORMANCETRACE"_Stz,   // CN_PERFORMANCETRACE
    /* 86 */ "PLAN"_Stz,               // CN_PLAN
    /* 87 */ "PLANNER"_Stz,            // CN_PLANNER
    /* 88 */ "PLANLIST"_Stz,           // CN_PLANNER
    /* 89 */ "POSIXTHREAD"_Stz,        // CN_POSIXTHREAD
    /* 90 */ "PREDICATE"_Stz,          // CN_PREDICATE
    /* 91 */ "PREDICATEVEC"_Stz,       // CN_PREDICATEVEC
    /* 92 */ "PROXY"_Stz,              // CN_PROXY
    /* 93 */ "PROXYSET"_Stz,           // CN_PROXYSET
    /* 94 */ "PTNCOLVEC"_Stz,          // CN_PTNCOLVEC
    
    /* 95 */ "QUEUE"_Stz,              // CN_QUEUE
    /* 96 */ "QUEUEBUFFER"_Stz,        // CN_QUEUEBUFFER
    /*    */ "QUEUES"_Stz,             // CN_QUEUES
    
    /* 97 */ "RBCURSOR"_Stz,           // CN_RBCURSOR
    /*    */ "REGFRAME"_Stz,           // CN_REGFRAME
    /*101 */ "RING"_Stz,               // CN_RING
    /*102 */ "ROARRAY"_Stz,            // CN_ROARRAY
    /*103 */ "ROWBUFFER"_Stz,          // CN_ROWBUFFER
    /*104 */ "ROWCURSOR"_Stz,          // CN_ROWCURSOR
    /*105 */ "ROWONCE"_Stz,            // CN_ROWONCE
    /*106 */ "ROWWRK"_Stz,             // CN_ROWWRK
  
    /*107 */ "SCANBUFFER"_Stz,         // CN_SCANBUFFER
    /*108 */ "SCRATCH"_Stz,            // CN_SCRATCH
    /*110 */ "SEITF"_Stz,              // CN_SEITF

    /*113 */ "SELCURSOR"_Stz,          // CN_SELCURSOR
    /*114 */ "SEMAPHORE"_Stz,          // CN_SEMAPHORE
    /*115 */ "SEPROMPT"_Stz,           // CN_SEPROMPT
    /*116 */ "SINCE"_Stz,              // CN_SINCE
    /*117 */ "SKIPCURSOR"_Stz,         // CN_SKIPCURSOR
    /*118 */ "SKIPLIST"_Stz,           // CN_SKIPLIST
    /*119 */ "SKIPNODE"_Stz,           // CN_SKIPNODE
    /*120 */ "SLAB"_Stz,               // CN_SLAB
    /*121 */ "SLABALLOCATOR"_Stz,      // CN_SLABALLOCATOR
    /*122 */ "SLOTS"_Stz,              // CN_SLOTS
    /*123 */ "SMALLPERSISTENT"_Stz,    // CN_SMALLPERSISTENT
    /*124 */ "SMALLTRANSIENT"_Stz,     // CN_SMALLTRANSIENT
    /*125 */ "SOCKET"_Stz,             // CN_SOCKET
    /*126 */ "STACK"_Stz,              // CN_STACK
    /*127 */ "STEPACCUM"_Stz,          // CN_STEPACCUM
    /*128 */ "STEPARRAY"_Stz,          // CN_STEPARRAY
    /*129 */ "SYSCALL"_Stz,            // CN_SYSCALL
  
    /*130 */ "TABLE"_Stz,              // CN_TABLE
    /*131 */ "TASK"_Stz,               // CN_TASK
    /*    */ "TASKFIFO"_Stz,           // CN_TASKFIFO
    /*133 */ "TASKLIST"_Stz,           // CN_TASKLIST
    /*134 */ "TASKQUEUE"_Stz,          // CN_TASKQUEUE
    /*135 */ "SERVERSCHEDULER"_Stz,    // CN_SERVERSCHEDULER

    /*137 */ "THREAD"_Stz,             // CN_THREAD

    /*139 */ "THREADLIST"_Stz,         // CN_THREADLIST
    /*140 */ "THREADQUEUE"_Stz,        // CN_THREADQUEUE
    /*    */ "THREADVECTOR"_Stz,       // CN_THREADVECTOR
    /*141 */ "TIME"_Stz,               // CN_TIME
    /*142 */ "TIMER"_Stz,              // CN_TIMER
    /*143 */ "TOKEN"_Stz,              // CN_TOKEN
    /*144 */ "TOKENISER"_Stz,          // CN_TOKENISER
    /*145 */ "TOKENARY"_Stz,           // CN_TOKENARY
    /*146 */ "TOKENVEC"_Stz,           // CN_TOKENVEC
    /*147 */ "TX"_Stz,                 // CN_TX
    /*148 */ "TXSET"_Stz,              // CN_TXSET
    
    /*149 */ "WILDERNESS"_Stz,         // CN_WILDERNESS
    /*150 */ "WORD"_Stz,               // CN_WORD
    /*    */ "WORKINGBUFFER"_Stz,      // CN_WORKINGBUFFER
    /*    */ "WORKINGROW"_Stz,         // CN_"WORKINGROW"
    
    /*151 */ "ZSMALL_"_Stz,            // CN_ZSMALL_
    /*152 */ "ZMEDIUM_"_Stz,           // CN_ZMEDIUM_
    /*153 */ "ZLARGE_"_Stz,            // CN_ZLARGE_
    /*154 */ "ZJUMBO_"_Stz,            // CN_ZJUMBO_
  
    /*155 */ "ENUMS"_Stz               // CN_ENUMS
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
