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
  
    /* 33 */ CN_DIC,
    /*    */ CN_DICPATH,
    /* 34 */ CN_DICSTACK,
    /* 35 */ CN_DIR,
  
    /* 36 */ CN_ENTRY,
    /* 37 */ CN_ENTRYBUFFER,
    /* 38 */ CN_ENTRYVEC,
    
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
    /*    */ CN_JOB_CREATECATALOG,
    /*    */ CN_JOB_CREATEINDEX,
    /*    */ CN_JOB_CREATESCHEMA,
    /*    */ CN_JOB_CREATETABLE,
    /*    */ CN_JOB_CREATETENANT,
    /*    */ CN_JOB_DROPCATALOG,
    /*    */ CN_JOB_DROPINDEX,
    /*    */ CN_JOB_DROPSCHEMA,
    /*    */ CN_JOB_DROPTABLE,
    /*    */ CN_JOB_DROPTENANT,
    /*    */ CN_JOB_G35,
    /*    */ CN_JOB_LOADTABLE,
    /*    */ CN_JOB_SANITY,
    /*    */ CN_JOB_SPACE,
    /*    */ CN_JOB_STATUS,
    
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
    /*109 */ CN_SEHISTORY,
    /*110 */ CN_SEITF,
    /*111 */ CN_SEITFCURSOR,
    /*112 */ CN_SEITFLIST,
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
    /*132 */ CN_TASKCURSOR,
    /*    */ CN_TASKFIFO,
    /*133 */ CN_TASKLIST,
    /*134 */ CN_TASKQUEUE,
    /*135 */ CN_SERVERSCHEDULER,
    /*136 */ CN_TASKSET,
    /*137 */ CN_THREAD,
    /*138 */ CN_THREADCURSOR,
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
    /*  0 */ "NONE"_Stz,     // CN_
  
    /*  1 */ "BCDVAL"_Stz,   // CN_
    /*  2 */ "BINVAL"_Stz,   // CN_
    /*  3 */ "BOLVAL"_Stz,   // CN_
    /*  4 */ "DTEVAL"_Stz,   // CN_
    /*  5 */ "ENTWRK"_Stz,   // CN_
    /*  6 */ "FLTVAL"_Stz,   // CN_
    /*  7 */ "INTVAL"_Stz,   // CN_
    /*  8 */ "IPVAL"_Stz,    // CN_
    /*  9 */ "IVLVAL"_Stz,   // CN_
    /* 10 */ "OBJVAL"_Stz,   // CN_
    /* 11 */ "PTRVAL"_Stz,   // CN_
    /* 12 */ "STRVAL"_Stz,   // CN_
    /* 13 */ "STRVAR"_Stz,   // CN_
    /* 14 */ "TMEVAL"_Stz,   // CN_
    /* 15 */ "TSPVAL"_Stz,   // CN_
    /* 16 */ "VALREG"_Stz,   // CN_
    /* 17 */ "VALSET"_Stz,   // CN_
    /* 18 */ "VALWRK"_Stz,   // CN_
    
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
    
    /* 33 */ "DIC"_Stz,                // CN_DIC
    /*    */ "DICPATH"_Stz,            // CN_DICPATH
    /* 34 */ "DICSTACK"_Stz,           // CN_DICSTACK
    /* 35 */ "DIR"_Stz,                // CN_DIR
    
    /* 36 */ "ENTRY"_Stz,              // CN_ENTRY
    /* 37 */ "ENTRYBUFFER"_Stz,        // CN_ENTRYBUFFER
    /* 38 */ "ENTRYVEC"_Stz,           // CN_ENTRYVEC
  
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
    /*    */ "JOB_CREATECATALOG"_Stz,  // CN_JOB_CREATECATALOG
    /*    */ "JOB_CREATEINDEX"_Stz,    // CN_JOB_CREATEINDEX
    /*    */ "JOB_CREATESCHEMA"_Stz,   // CN_JOB_CREATESCHEMA
    /*    */ "JOB_CREATETABLE"_Stz,    // CN_JOB_CREATETABLE
    /*    */ "JOB_CREATETENANT"_Stz,   // CN_JOB_CREATETENANT
    /*    */ "JOB_DROPCATALOG"_Stz,    // CN_JOB_DROPCATALOG
    /*    */ "JOB_DROPINDEX"_Stz,      // CN_JOB_DROPINDEX
    /*    */ "JOB_DROPSCHEMA"_Stz,     // CN_JOB_DROPSCHEMA
    /*    */ "JOB_DROPTABLE"_Stz,      // CN_JOB_DROPTABLE
    /*    */ "JOB_DROPTENANT"_Stz,     // CN_JOB_DROPTENANT
    /*    */ "JOB_G35"_Stz,            // CN_JOB_G35
    /*    */ "JOB_LOADTABLE"_Stz,      // CN_JOB_LOADTABLE
    /*    */ "JOB_SANITY"_Stz,         // CN_JOB_SANITY
    /*    */ "JOB_SPACE"_Stz,          // CN_JOB_SPACE
    /*    */ "JOB_STATUS"_Stz,         // CN_JOB_STATUS
    /*    */ "JOBS"_Stz,               // CN_JOBS
    /* 59 */ "JUMBOPERSISTENT"_Stz,    // CN_JUMBOPERSISTENT
    /* 60 */ "JUMBOTRANSIENT"_Stz,     // CN_JUMBOTRANSIENT
    
    /* 61 */ "KEY"_Stz,                // CN_
    /* 62 */ "KEYVAL"_Stz,             // CN_
    /* 63 */ "KEYVEC"_Stz,             // CN_
    /* 64 */ "KEYWRK"_Stz,             // CN_
  
    /* 65 */ "LARGEPERSISTENT"_Stz,    // CN_
    /* 66 */ "LARGETRANSIENT"_Stz,     // CN_
    
    /* 67 */ "LINEITEM"_Stz,           // CN_
    /* 68 */ "LOADCHUNK"_Stz,          // CN_
    /* 69 */ "LOADER"_Stz,             // CN_
    /* 70 */ "LOADERSUB"_Stz,          // CN_
    /* 71 */ "LOADMAP"_Stz,            // CN_
    /* 72 */ "LOCK"_Stz,               // CN_

    /* 73 */ "MEDIUMPERSISTENT"_Stz,   // CN_MEDIUMPERSISTENT
    /* 74 */ "MEDIUMTRANSIENT"_Stz,    // CN_MEDIUMTRANSIENT
    
    /* 75 */ "NODE"_Stz,               // CN_NODE
  
    /* 77 */ "OP"_Stz,                 // CN_OP
  
    /* 78 */ "PARSER"_Stz,             // CN_
    /* 79 */ "PARTITION"_Stz,          // CN_
    /* 80 */ "PARTITIONSCHEDULER"_Stz, // CN_
    /* 81 */ "PARTITIONVEC"_Stz,       // CN_
    /* 82 */ "PATHVEC"_Stz,            // CN_
    /* 83 */ "PERFORMANCEBUFFER"_Stz,  // CN_
    /* 84 */ "PERFORMANCEENTRY"_Stz,   // CN_
    /* 85 */ "PERFORMANCETRACE"_Stz,   // CN_
    /* 86 */ "PLAN"_Stz,               // CN_
    /* 87 */ "PLANNER"_Stz,            // CN_
    /* 88 */ "PLANLIST"_Stz,           // CN_
    /* 89 */ "POSIXTHREAD"_Stz,        // CN_
    /* 90 */ "PREDICATE"_Stz,          // CN_
    /* 91 */ "PREDICATEVEC"_Stz,       // CN_
    /* 92 */ "PROXY"_Stz,              // CN_
    /* 93 */ "PROXYSET"_Stz,           // CN_
    /* 94 */ "PTNCOLVEC"_Stz,          // CN_
    
    /* 95 */ "QUEUE"_Stz,              // CN_
    /* 96 */ "QUEUEBUFFER"_Stz,        // CN_
    /*    */ "QUEUES"_Stz,             // CN_
  
    /* 97 */ "RBCURSOR"_Stz,           // CN_
    /*    */ "REGFRAME"_Stz,           // CN_
    
    /*101 */ "RING"_Stz,               // CN_
    /*102 */ "ROARRAY"_Stz,            // CN_
    /*103 */ "ROWBUFFER"_Stz,          // CN_
    /*104 */ "ROWCURSOR"_Stz,          // CN_
    /*105 */ "ROWONCE"_Stz,            // CN_
    /*106 */ "ROWWRK"_Stz,             // CN_
  
    /*107 */ "SCANBUFFER"_Stz,         // CN_
    /*108 */ "SCRATCH"_Stz,            // CN_
    /*109 */ "SEHISTORY"_Stz,   // CN_
    /*110 */ "SEITF"_Stz,   // CN_
    /*111 */ "SEITFCURSOR"_Stz,   // CN_
    /*112 */ "SEITFLIST"_Stz,   // CN_
    /*113 */ "SELCURSOR"_Stz,   // CN_
    /*114 */ "SEMAPHORE"_Stz,   // CN_
    /*115 */ "SEPROMPT"_Stz,   // CN_
    /*116 */ "SINCE"_Stz,   // CN_
    /*117 */ "SKIPCURSOR"_Stz,   // CN_
    /*118 */ "SKIPLIST"_Stz,   // CN_
    /*119 */ "SKIPNODE"_Stz,   // CN_
    /*120 */ "SLAB"_Stz,   // CN_
    /*121 */ "SLABALLOCATOR"_Stz,   // CN_
    /*122 */ "SLOTS"_Stz,   // CN_
    /*123 */ "SMALLPERSISTENT"_Stz,   // CN_
    /*124 */ "SMALLTRANSIENT"_Stz,   // CN_
    /*125 */ "SOCKET"_Stz,   // CN_
    /*126 */ "STACK"_Stz,   // CN_
    /*127 */ "STEPACCUM"_Stz,   // CN_
    /*128 */ "STEPARRAY"_Stz,   // CN_
    /*129 */ "SYSCALL"_Stz,   // CN_
  
    /*130 */ "TABLE"_Stz,   // CN_
    /*131 */ "TASK"_Stz,   // CN_
    /*132 */ "TASKCURSOR"_Stz,   // CN_
    /*    */ "TASKFIFO"_Stz,   // CN_
    /*133 */ "TASKLIST"_Stz,   // CN_
    /*134 */ "TASKQUEUE"_Stz,   // CN_
    /*135 */ "SERVERSCHEDULER"_Stz,   // CN_
    /*136 */ "TASKSET"_Stz,   // CN_
    /*137 */ "THREAD"_Stz,   // CN_
    /*138 */ "THREADCURSOR"_Stz,   // CN_
    /*139 */ "THREADLIST"_Stz,   // CN_
    /*140 */ "THREADQUEUE"_Stz,   // CN_
    /*    */ "THREADVECTOR"_Stz,   // CN_
    /*141 */ "TIME"_Stz,   // CN_
    /*142 */ "TIMER"_Stz,   // CN_
    /*143 */ "TOKEN"_Stz,   // CN_
    /*144 */ "TOKENISER"_Stz,   // CN_
    /*145 */ "TOKENARY"_Stz,   // CN_
    /*146 */ "TOKENVEC"_Stz,   // CN_
    /*147 */ "TX"_Stz,   // CN_
    /*148 */ "TXSET"_Stz,   // CN_
    
    /*149 */ "WILDERNESS"_Stz,   // CN_
    /*150 */ "WORD"_Stz,   // CN_
    /*    */ "WORKINGBUFFER"_Stz,   // CN_
    /*    */ "WORKINGROW"_Stz,   // CN_
    
    /*151 */ "ZSMALL_"_Stz,   // CN_
    /*152 */ "ZMEDIUM_"_Stz,   // CN_
    /*153 */ "ZLARGE_"_Stz,   // CN_
    /*154 */ "ZJUMBO_"_Stz,   // CN_
  
    /*155 */ "ENUMS"_Stz   // CN_
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
