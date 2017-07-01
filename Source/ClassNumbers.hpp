/*============================================================================*/
/* ClassNumbers.hpp - class numbers                                           */
/*============================================================================*/
#pragma once

#include "Base.h"

#include "Str.hpp"

namespace Base
{
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
    
    /* 26 */ CN_BUFFERPOOL,
    /* 27 */ CN_BPA,
  
    /* 28 */ CN_COLUMN,
    /* 29 */ CN_COLUMNVEC,
  
    /* 30 */ CN_DATA,
    /* 31 */ CN_DIC,
    /* 32 */ CN_DICPATH,
    /* 33 */ CN_DIR,
  
    /* 34 */ CN_ENTRY,
    /* 35 */ CN_ENTRYBUFFER,
    /* 36 */ CN_ENTRYVEC,
    
    /* 37 */ CN_FILE,
    /* 38 */ CN_FREEBUFFER,
    /* 39 */ CN_FREECHUNK,
    /* 40 */ CN_FREEDNOT,
    
    /* 41 */ CN_GRAPH,
    /* 42 */ CN_GRAPHCURSOR,
    /* 43 */ CN_GRAPHNODE,
    /* 44 */ CN_GRID,
    /* 45 */ CN_GROUPBY,
  
    /* 46 */ CN_HEAPBUFFER,
    
    /* 47 */ CN_INDEX,
    /* 48 */ CN_INDEXDATA,
    /* 49 */ CN_IDXCOLVEC,
    /* 50 */ CN_IDXDATAVEC,
    /* 51 */ CN_IDXVEC,
    /* 52 */ CN_INDEXLEVELS,
  
    /* 53 */ CN_JUMBOHEAP,
  
    /* 54 */ CN_KEYVAL,
    /* 55 */ CN_KEYVEC,
    /* 56 */ CN_KEYWRK,
  
    /* 57 */ CN_LINEITEM,
    /* 58 */ CN_LOADCHUNK,
    /* 59 */ CN_LOADER,
    /* 60 */ CN_LOADERSUB,
    /* 61 */ CN_LOADMAP,
    /* 62 */ CN_LOCK,
    
    /* 63 */ CN_MEDIUMHEAP,

    /* 64 */ CN_NODE,
    /* 65 */ CN_NODECURSOR,
  
    /* 66 */ CN_OP,
  
    /* 67 */ CN_PARSER,
    /* 68 */ CN_PARTITION,
    /* 69 */ CN_PATHVEC,
    /* 70 */ CN_PLAN,
    /* 71 */ CN_PLANNER,
    /* 72 */ CN_PLANLIST,
    /* 73 */ CN_POSIXTHREAD,
    /* 74 */ CN_PREDICATE,
    /* 75 */ CN_PREDICATEVEC,
    /* 76 */ CN_PROXY,
    /* 77 */ CN_PROXYSET,
    /* 78 */ CN_PTNCOLVEC,
    
    /* 79 */ CN_QUEUE,
    /* 80 */ CN_QUEUEBUFFER,
    
    /* 81 */ CN_RBCURSOR,
    /* 82 */ CN_REGFRAME,
    /* 83 */ CN_RING,
    /* 84 */ CN_ROARRAY,
    /* 85 */ CN_ROW,
    /* 86 */ CN_ROWBUFFER,
    /* 87 */ CN_ROWCURSOR,
    /* 88 */ CN_EBSTRAND,
  
    /* 89 */ CN_SCANBUFFER,
    /* 90 */ CN_SCRATCH,
    /* 91 */ CN_SEHISTORY,
    /* 92 */ CN_SEITF,
    /* 93 */ CN_SEITFCURSOR,
    /* 94 */ CN_SEITFLIST,
    /* 95 */ CN_SELCURSOR,
    /* 96 */ CN_SEMAPHORE,
    /* 97 */ CN_SEPROMPT,
    /* 98 */ CN_SINCE,
    /* 99 */ CN_SKIPCURSOR,
    /*100 */ CN_SKIPLIST,
    /*101 */ CN_SKIPNODE,
    /*102 */ CN_SLOTS,
    /*103 */ CN_SMALLHEAP,
    /*104 */ CN_SOCKET,
    /*105 */ CN_STACK,
    /*106 */ CN_STEPACCUM,
    /*107 */ CN_STEPARRAY,
    /*108 */ CN_SYSCALL,

    /*109 */ CN_TABLE,
    /*110 */ CN_TASK,
    /*111 */ CN_TASKCURSOR,
    /*112 */ CN_TASKLIST,
    /*113 */ CN_SCHEDULER,
    /*114 */ CN_TASKSET,
    /*115 */ CN_THREAD,
    /*116 */ CN_THREADCURSOR,
    /*117 */ CN_THREADLIST,
    /*118 */ CN_TIME,
    /*119 */ CN_TIMER,
    /*120 */ CN_TOKEN,
    /*121 */ CN_TOKENISER,
    /*122 */ CN_TOKENARY,
    /*123 */ CN_TOKENVEC,
    /*124 */ CN_TX,
    /*125 */ CN_TXARRAY,
    /*126 */ CN_TXCOLLECTION,
    /*127 */ CN_TXCURSOR,
    /*128 */ CN_TXMAP,
    /*129 */ CN_TXREF,
  
    /*130 */ CN_WORD,
    
    /*131 */ CN_ZSMALL_,
    /*132 */ CN_ZMEDIUM_,
    /*133 */ CN_ZLARGE_,
    /*134 */ CN_ZJUMBO_,
  
    /*135 */ CN_ENUMS
  };
  
  static Str* aCN[]
  {
    /*  0 */ "NONE"_Str,
    
  
    /*  1 */ "BCDVAL"_Str,
    /*  2 */ "BINVAL"_Str,
    /*  3 */ "BOLVAL"_Str,
    /*  4 */ "DTEVAL"_Str,
    /*  5 */ "ENTWRK"_Str,
    /*  6 */ "FLTVAL"_Str,
    /*  7 */ "INTVAL"_Str,
    /*  8 */ "IPVAL"_Str,
    /*  9 */ "IVLVAL"_Str,
    /* 10 */ "OBJVAL"_Str,
    /* 11 */ "PTRVAL"_Str,
    /* 12 */ "STRVAL"_Str,
    /* 13 */ "STRVAR"_Str,
    /* 14 */ "TMEVAL"_Str,
    /* 15 */ "TSPVAL"_Str,
    /* 16 */ "VALREG"_Str,
    /* 17 */ "VALSET"_Str,
    /* 18 */ "VALWRK"_Str,
    
    /*    */ "ACCESSPATH"_Str,
    /*    */ "ACCUM"_Str,
    /*    */ "ACCUMVEC"_Str,
    /* 12 */ "ARG"_Str,
    /* 12 */ "ARGUMENTS"_Str,
    /* 13 */ "ATOMIC"_Str,
    /* 14 */ "ATTACHMAP"_Str,
    
    /* 15 */ "BUFFERPOOL"_Str,
    /* 16 */ "BPA"_Str,
  
    /*    */ "COLUMN"_Str,
    /*    */ "COLUMNVEC"_Str,
    
    /* 17 */ "DATA"_Str,
    /* 18 */ "DIC"_Str,
    /* 19 */ "DICPATH"_Str,
    /* 20 */ "DIR"_Str,
    
    /* 21 */ "ENTRY"_Str,
    /*    */ "ENTRYBUFFER"_Str,
    /*    */ "ENTRYVEC"_Str,
    /* 22 */ "FILE"_Str,
    /* 23 */ "FREEBUFFER"_Str,
    /* 24 */ "FREECHUNK"_Str,
    /*    */ "FREEDNOT"_Str,
    
    /* 25 */ "GRAPH"_Str,
    /* 26 */ "GRAPHCURSOR"_Str,
    /* 27 */ "GRAPHNODE"_Str,
    /* 28 */ "GRID"_Str,
    /*    */ "GROUPBY"_Str,
  
    /* 29 */ "HEAPBUFFER"_Str,
    
    /* 30 */ "INDEX"_Str,
    /*    */ "INDEXDATA"_Str,
    /*    */ "IDXCOLVEC"_Str,
    /*    */ "IDXDATAVEC"_Str,
    /*    */ "IDXVEC"_Str,
    /* 33 */ "INDEXLEVELS"_Str,

    /* 34 */ "JUMBOHEAP"_Str,
    
    /*    */ "KEYVAL"_Str,
    /*    */ "KEYVEC"_Str,
    /* 35 */ "KEYWRK"_Str,
  
    /* 36 */ "LINEITEM"_Str,
    /* 36 */ "LOADCHUNK"_Str,
    /* 37 */ "LOADER"_Str,
    /* 38 */ "LOADERSUB"_Str,
    /* 39 */ "LOADMAP"_Str,
    /* 40 */ "LOCK"_Str,

    /* 41 */ "MEDIUMHEAP"_Str,
    
    /* 42 */ "NODE"_Str,
    /* 43 */ "NODECURSOR"_Str,
  
    /* 43 */ "OP"_Str,
  
    /* 44 */ "PARSER"_Str,
    /* 45 */ "PARTITION"_Str,
    /*    */ "PATHVEC"_Str,
    /* 46 */ "PLAN"_Str,
    /*    */ "PLANNER"_Str,
    /* 47 */ "PLANLIST"_Str,
    /*    */ "POSIXTHREAD"_Str,
    /* 48 */ "PREDICATE"_Str,
    /*    */ "PREDICATEVEC"_Str,
    /* 48 */ "PROXY"_Str,
    /* 49 */ "PROXYSET"_Str,
    /*    */ "PTNCOLVEC"_Str,
    
    /* 50 */ "QUEUE"_Str,
    /* 51 */ "QUEUEBUFFER"_Str,
  
    /* 52 */ "RBCURSOR"_Str,
    /* 53 */ "REGFRAME"_Str,
    /* 54 */ "RING"_Str,
    /*    */ "ROARRAY"_Str,
    /* 55 */ "ROW"_Str,
    /* 56 */ "ROWBUFFER"_Str,
    /* 57 */ "ROWCURSOR"_Str,
    /* 58 */ "EBSTRAND"_Str,
  
    /* 60 */ "SCANBUFFER"_Str,
    /* 61 */ "SCRATCH"_Str,
    /* 62 */ "SEHISTORY"_Str,
    /* 63 */ "SEITF"_Str,
    /* 64 */ "SEITFCURSOR"_Str,
    /* 65 */ "SEITFLIST"_Str,
    /* 66 */ "SELCURSOR"_Str,
    /* 67 */ "SEMAPHORE"_Str,
    /* 67 */ "SEPROMPT"_Str,
    /* 68 */ "SINCE"_Str,
    /* 69 */ "SKIPCURSOR"_Str,
    /* 70 */ "SKIPLIST"_Str,
    /* 71 */ "SKIPNODE"_Str,
    /* 72 */ "PARTITIONS"_Str,
    /* 73 */ "SMALLHEAP"_Str,
    /* 74 */ "SOCKET"_Str,
    /* 75 */ "STACK"_Str,
    /* 76 */ "STEPACCUM"_Str,
    /* 77 */ "STEPARRAY"_Str,
    /* 78 */ "SYSCALL"_Str,
  
    /* 79 */ "TABLE"_Str,
    /* 80 */ "TASK"_Str,
    /* 81 */ "TASKCURSOR"_Str,
    /* 82 */ "TASKLIST"_Str,
    /*    */ "SCHEDULER"_Str,
    /* 83 */ "TASKSET"_Str,
    /* 84 */ "THREAD"_Str,
    /* 85 */ "THREADCURSOR"_Str,
    /* 86 */ "THREADLIST"_Str,
    /* 87 */ "TIME"_Str,
    /* 88 */ "TIMER"_Str,
    /* 89 */ "TOKEN"_Str,
    /* 89 */ "TOKENISER"_Str,
    /*    */ "TOKENARY"_Str,
    /*    */ "TOKENVEC"_Str,
    /* 90 */ "TX"_Str,
    /* 91 */ "TXARRAY"_Str,
    /* 92 */ "TXCOLLECTION"_Str,
    /* 93 */ "TXCURSOR"_Str,
    /* 94 */ "TXMAP"_Str,
    /* 95 */ "TXREF"_Str,

    /* 96 */ "WORD"_Str,
    
    /* 97 */ "ZSMALL_"_Str,
    /* 98 */ "ZMEDIUM_"_Str,
    /* 99 */ "ZLARGE_"_Str,
    /*100 */ "ZJUMBO_"_Str,
  
    /*101 */ "ENUMS"_Str
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
