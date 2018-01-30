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
  enum eCN // Class Number
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
    /* 34 */ CN_DICPATH,
    /* 35 */ CN_DIR,
  
    /* 36 */ CN_ENTRY,
    /* 37 */ CN_ENTRYBUFFER,
    /* 38 */ CN_ENTRYVEC,
    
    /* 39 */ CN_FILE,
    /* 40 */ CN_FORMATTERS,
    /* 41 */ CN_FREEBUFFER,
    /* 42 */ CN_FREECHUNK,
    /* 43 */ CN_FREEDNOT,
    /*    */ CN_FREEENTRY,
    
    /* 44 */ CN_GRAPH,
    /* 45 */ CN_GRAPHCURSOR,
    /* 46 */ CN_GRAPHNODE,
    /* 47 */ CN_GRID,
    /* 48 */ CN_GROUPBY,
  
    /* 49 */ CN_HEAPBUFFER,
    
    /* 50 */ CN_INDEX,
    /* 51 */ CN_INDEXDATA,
    /* 52 */ CN_IDXCOLVEC,
    /* 53 */ CN_IDXDATAVEC,
    /* 54 */ CN_IDXVEC,
    /* 55 */ CN_INDEXLEVELS,
    /* 56 */ CN_ITEM,
    /* 57 */ CN_ITEMBUFFER,
  
    /* 58 */ CN_JUMBOPERSISTENT,
    /* 59 */ CN_JUMBOTRANSIENT,
  
    /* 60 */ CN_KEY,
    /* 61 */ CN_KEYVAL,
    /* 62 */ CN_KEYVEC,
    /* 63 */ CN_KEYWRK,
  
    /* 64 */ CN_LARGEPERSISTENT,
    /* 65 */ CN_LARGETRANSIENT,
    
    /* 66 */ CN_LINEITEM,
    /* 67 */ CN_LOADCHUNK,
    /* 68 */ CN_LOADER,
    /* 69 */ CN_LOADERSUB,
    /* 70 */ CN_LOADMAP,
    /* 71 */ CN_LOCK,
    
    /* 72 */ CN_MEDIUMPERSISTENT,
    /* 73 */ CN_MEDIUMTRANSIENT,

    /* 74 */ CN_NODE,
    /* 75 */ CN_NODECURSOR,
  
    /* 76 */ CN_OP,
  
    /* 77 */ CN_PARSER,
    /* 78 */ CN_PARTITION,
    /* 79 */ CN_PARTITIONSCHEDULER,
    /* 80 */ CN_PARTITIONVEC,
    /* 81 */ CN_PATHVEC,
    /* 82 */ CN_PERFORMANCEBUFFER,
    /* 83 */ CN_PERFORMANCEENTRY,
    /* 84 */ CN_PERFORMANCETRACE,
    /* 85 */ CN_PLAN,
    /* 86 */ CN_PLANNER,
    /* 87 */ CN_PLANLIST,
    /* 88 */ CN_POSIXTHREAD,
    /* 89 */ CN_PREDICATE,
    /* 90 */ CN_PREDICATEVEC,
    /* 91 */ CN_PROXY,
    /* 92 */ CN_PROXYSET,
    /* 93 */ CN_PTNCOLVEC,
  
    /* 94 */ CN_QUEUE,
    /* 95 */ CN_QUEUEBUFFER,
    
    /* 96 */ CN_RBCURSOR,
    /* 97 */ CN_REGFRAME,
    /* 98 */ CN_RESULTBUFFER,
    /* 99 */ CN_RESULTROW,
    /*100 */ CN_RING,
    /*101 */ CN_ROARRAY,
    /*102 */ CN_ROWBUFFER,
    /*103 */ CN_ROWCURSOR,
    /*104 */ CN_ROWONCE,
  
    /*105 */ CN_SCANBUFFER,
    /*106 */ CN_SCRATCH,
    /*107 */ CN_SEHISTORY,
    /*108 */ CN_SEITF,
    /*109 */ CN_SEITFCURSOR,
    /*110 */ CN_SEITFLIST,
    /*111 */ CN_SELCURSOR,
    /*112 */ CN_SEMAPHORE,
    /*113 */ CN_SEPROMPT,
    /*114 */ CN_SINCE,
    /*115 */ CN_SKIPCURSOR,
    /*116 */ CN_SKIPLIST,
    /*117 */ CN_SKIPNODE,
    /*118 */ CN_SLAB,
    /*119 */ CN_SLABALLOCATOR,
    /*120 */ CN_SLOTS,
    /*121 */ CN_SMALLPERSISTENT,
    /*122 */ CN_SMALLTRANSIENT,
    /*123 */ CN_SOCKET,
    /*124 */ CN_STACK,
    /*125 */ CN_STEPACCUM,
    /*126 */ CN_STEPARRAY,
    /*127 */ CN_SYSCALL,
    
    /*128 */ CN_TABLE,
    /*129 */ CN_TASK,
    /*130 */ CN_TASKCURSOR,
    /*131 */ CN_TASKLIST,
    /*132 */ CN_TASKQUEUE,
    /*133 */ CN_SERVERSCHEDULER,
    /*134 */ CN_TASKSET,
    /*135 */ CN_THREAD,
    /*136 */ CN_THREADCURSOR,
    /*137 */ CN_THREADLIST,
    /*138 */ CN_THREADQUEUE,
    /*139 */ CN_TIME,
    /*140 */ CN_TIMER,
    /*141 */ CN_TOKEN,
    /*142 */ CN_TOKENISER,
    /*143 */ CN_TOKENARY,
    /*144 */ CN_TOKENVEC,
    /*145 */ CN_TX,
    /*146 */ CN_TXSET,

    /*147 */ CN_WORD,
    
    /*148 */ CN_ZSMALL_,
    /*149 */ CN_ZMEDIUM_,
    /*150 */ CN_ZLARGE_,
    /*151 */ CN_ZJUMBO_,
  
    /*152 */ CN_ENUMS
  };
  
/*----------------------------------------------------------------------------*/
/* class names                                                                */
/*----------------------------------------------------------------------------*/
  static Stz* aCN[]
  {
    /*  0 */ "NONE"_Stz,
    
  
    /*  1 */ "BCDVAL"_Stz,
    /*  2 */ "BINVAL"_Stz,
    /*  3 */ "BOLVAL"_Stz,
    /*  4 */ "DTEVAL"_Stz,
    /*  5 */ "ENTWRK"_Stz,
    /*  6 */ "FLTVAL"_Stz,
    /*  7 */ "INTVAL"_Stz,
    /*  8 */ "IPVAL"_Stz,
    /*  9 */ "IVLVAL"_Stz,
    /* 10 */ "OBJVAL"_Stz,
    /* 11 */ "PTRVAL"_Stz,
    /* 12 */ "STRVAL"_Stz,
    /* 13 */ "STRVAR"_Stz,
    /* 14 */ "TMEVAL"_Stz,
    /* 15 */ "TSPVAL"_Stz,
    /* 16 */ "VALREG"_Stz,
    /* 17 */ "VALSET"_Stz,
    /* 18 */ "VALWRK"_Stz,
    
    /* 19 */ "ACCESSPATH"_Stz,
    /* 20 */ "ACCUM"_Stz,
    /* 21 */ "ACCUMVEC"_Stz,
    /* 22 */ "ARG"_Stz,
    /* 23 */ "ARGUMENTS"_Stz,
    /* 24 */ "ATOMIC"_Stz,
    /* 25 */ "ATTACHMAP"_Stz,
    
    /* 26 */ "BITARRAY"_Stz,
    /* 27 */ "BITMAP"_Stz,
    /* 28 */ "BUFFERPOOL"_Stz,
    /* 29 */ "BPA"_Stz,
  
    /*    */ "CARRYOVER"_Stz,
    /* 30 */ "COLUMN"_Stz,
    /* 31 */ "COLUMNVEC"_Stz,
    
    /* 32 */ "DIC"_Stz,
    /* 33 */ "DICPATH"_Stz,
    /* 34 */ "DIR"_Stz,
    
    /* 35 */ "ENTRY"_Stz,
    /* 36 */ "ENTRYBUFFER"_Stz,
    /* 37 */ "ENTRYVEC"_Stz,
  
    /* 38 */ "FILE"_Stz,
    /*    */ "FORMATTERS"_Stz,
    /* 39 */ "FREEBUFFER"_Stz,
    /* 40 */ "FREECHUNK"_Stz,
    /* 41 */ "FREEDNOT"_Stz,
    /*    */ "FREEENTRY"_Stz,
    
    /* 42 */ "GRAPH"_Stz,
    /* 43 */ "GRAPHCURSOR"_Stz,
    /* 44 */ "GRAPHNODE"_Stz,
    /* 45 */ "GRID"_Stz,
    /* 46 */ "GROUPBY"_Stz,
  
    /* 47 */ "HEAPBUFFER"_Stz,
    
    /* 48 */ "INDEX"_Stz,
    /* 49 */ "INDEXDATA"_Stz,
    /* 50 */ "IDXCOLVEC"_Stz,
    /* 51 */ "IDXDATAVEC"_Stz,
    /* 52 */ "IDXVEC"_Stz,
    /* 53 */ "INDEXLEVELS"_Stz,
    /*    */ "ITEM"_Stz,
    /*    */ "ITEMBUFFER"_Stz,

    /*    */ "JUMBOPERSISTENT"_Stz,
    /*    */ "JUMBOTRANSIENT"_Stz,
    
    /* 55 */ "KEY"_Stz,
    /* 56 */ "KEYVAL"_Stz,
    /* 57 */ "KEYVEC"_Stz,
    /* 58 */ "KEYWRK"_Stz,
  
    /*    */ "LARGEPERSISTENT"_Stz,
    /*    */ "LARGETRANSIENT"_Stz,
    
    /* 59 */ "LINEITEM"_Stz,
    /* 60 */ "LOADCHUNK"_Stz,
    /* 61 */ "LOADER"_Stz,
    /* 62 */ "LOADERSUB"_Stz,
    /* 63 */ "LOADMAP"_Stz,
    /* 64 */ "LOCK"_Stz,

    /*    */ "MEDIUMPERSISTENT"_Stz,
    /*    */ "MEDIUMTRANSIENT"_Stz,
    
    /* 66 */ "NODE"_Stz,
    /* 67 */ "NODECURSOR"_Stz,
  
    /* 68 */ "OP"_Stz,
  
    /* 69 */ "PARSER"_Stz,
    /* 70 */ "PARTITION"_Stz,
    /* 71 */ "PARTITIONSCHEDULER"_Stz,
    /* 72 */ "PARTITIONVEC"_Stz,
    /* 73 */ "PATHVEC"_Stz,
    /*    */ "PERFORMANCEBUFFER"_Stz,
    /*    */ "PERFORMANCEENTRY"_Stz,
    /*    */ "PERFORMANCETRACE"_Stz,
    /* 74 */ "PLAN"_Stz,
    /* 75 */ "PLANNER"_Stz,
    /* 76 */ "PLANLIST"_Stz,
    /* 77 */ "POSIXTHREAD"_Stz,
    /* 78 */ "PREDICATE"_Stz,
    /* 79 */ "PREDICATEVEC"_Stz,
    /* 80 */ "PROXY"_Stz,
    /* 81 */ "PROXYSET"_Stz,
    /* 82 */ "PTNCOLVEC"_Stz,
    
    /* 83 */ "QUEUE"_Stz,
    /* 84 */ "QUEUEBUFFER"_Stz,
  
    /* 85 */ "RBCURSOR"_Stz,
    /* 86 */ "REGFRAME"_Stz,
    /*    */ "RESULTBUFFER"_Stz,
    /*    */ "RESULTROW"_Stz,
    /* 87 */ "RING"_Stz,
    /* 88 */ "ROARRAY"_Stz,
    /* 89 */ "ROWBUFFER"_Stz,
    /* 90 */ "ROWCURSOR"_Stz,
    /* 91 */ "ROWONCE"_Stz,
  
    /* 92 */ "SCANBUFFER"_Stz,
    /* 93 */ "SCRATCH"_Stz,
    /* 94 */ "SEHISTORY"_Stz,
    /* 95 */ "SEITF"_Stz,
    /* 96 */ "SEITFCURSOR"_Stz,
    /* 97 */ "SEITFLIST"_Stz,
    /* 98 */ "SELCURSOR"_Stz,
    /* 99 */ "SEMAPHORE"_Stz,
    /*100 */ "SEPROMPT"_Stz,
    /*101 */ "SINCE"_Stz,
    /*102 */ "SKIPCURSOR"_Stz,
    /*103 */ "SKIPLIST"_Stz,
    /*104 */ "SKIPNODE"_Stz,
    /*105 */ "SLAB"_Stz,
    /*106 */ "SLABALLOCATOR"_Stz,
    /*107 */ "SLOTS"_Stz,
    /*    */ "SMALLPERSISTENT"_Stz,
    /*    */ "SMALLTRANSIENT"_Stz,
    /*109 */ "SOCKET"_Stz,
    /*110 */ "STACK"_Stz,
    /*111 */ "STEPACCUM"_Stz,
    /*112 */ "STEPARRAY"_Stz,
    /*113 */ "SYSCALL"_Stz,
  
    /*114 */ "TABLE"_Stz,
    /*115 */ "TASK"_Stz,
    /*116 */ "TASKCURSOR"_Stz,
    /*117 */ "TASKLIST"_Stz,
    /*118 */ "TASKQUEUE"_Stz,
    /*119 */ "SERVERSCHEDULER"_Stz,
    /*120 */ "TASKSET"_Stz,
    /*121 */ "THREAD"_Stz,
    /*122 */ "THREADCURSOR"_Stz,
    /*123 */ "THREADLIST"_Stz,
    /*124 */ "THREADQUEUE"_Stz,
    /*125 */ "TIME"_Stz,
    /*126 */ "TIMER"_Stz,
    /*127 */ "TOKEN"_Stz,
    /*128 */ "TOKENISER"_Stz,
    /*129 */ "TOKENARY"_Stz,
    /*130 */ "TOKENVEC"_Stz,
    /*131 */ "TX"_Stz,
    /*132 */ "TXSET"_Stz,
    
    /*133 */ "WORD"_Stz,
    
    /*134 */ "ZSMALL_"_Stz,
    /*135 */ "ZMEDIUM_"_Stz,
    /*136 */ "ZLARGE_"_Stz,
    /*137 */ "ZJUMBO_"_Stz,
  
    /*138 */ "ENUMS"_Stz
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
