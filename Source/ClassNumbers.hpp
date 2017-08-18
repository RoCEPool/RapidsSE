/*============================================================================*/
/* ClassNumbers.hpp - class numbers                                           */
/*============================================================================*/
#pragma once

#include "Base.h"

#include "Stz.hpp"

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
    
    /*    */ CN_BITARRAY,
    /*    */ CN_BITMAP,
    /* 26 */ CN_BUFFERPOOL,
    /* 27 */ CN_BPA,
  
    /* 28 */ CN_COLUMN,
    /* 29 */ CN_COLUMNVEC,
  
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
  
    /*    */ CN_KEY,
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
    /*    */ CN_PARTITIONSCHEDULER,
    /*    */ CN_PARTITIONVEC,
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
    /*    */ CN_TASKQUEUE,
    /*113 */ CN_SERVERSCHEDULER,
    /*114 */ CN_TASKSET,
    /*115 */ CN_THREAD,
    /*116 */ CN_THREADCURSOR,
    /*117 */ CN_THREADLIST,
    /*    */ CN_THREADQUEUE,
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
    
    /*    */ "ACCESSPATH"_Stz,
    /*    */ "ACCUM"_Stz,
    /*    */ "ACCUMVEC"_Stz,
    /* 12 */ "ARG"_Stz,
    /* 12 */ "ARGUMENTS"_Stz,
    /* 13 */ "ATOMIC"_Stz,
    /* 14 */ "ATTACHMAP"_Stz,
    
    /*    */ "BITARRAY"_Stz,
    /*    */ "BITMAP"_Stz,
    /* 15 */ "BUFFERPOOL"_Stz,
    /* 16 */ "BPA"_Stz,
  
    /*    */ "COLUMN"_Stz,
    /*    */ "COLUMNVEC"_Stz,
    
    /* 18 */ "DIC"_Stz,
    /* 19 */ "DICPATH"_Stz,
    /* 20 */ "DIR"_Stz,
    
    /* 21 */ "ENTRY"_Stz,
    /*    */ "ENTRYBUFFER"_Stz,
    /*    */ "ENTRYVEC"_Stz,
    /* 22 */ "FILE"_Stz,
    /* 23 */ "FREEBUFFER"_Stz,
    /* 24 */ "FREECHUNK"_Stz,
    /*    */ "FREEDNOT"_Stz,
    
    /* 25 */ "GRAPH"_Stz,
    /* 26 */ "GRAPHCURSOR"_Stz,
    /* 27 */ "GRAPHNODE"_Stz,
    /* 28 */ "GRID"_Stz,
    /*    */ "GROUPBY"_Stz,
  
    /* 29 */ "HEAPBUFFER"_Stz,
    
    /* 30 */ "INDEX"_Stz,
    /*    */ "INDEXDATA"_Stz,
    /*    */ "IDXCOLVEC"_Stz,
    /*    */ "IDXDATAVEC"_Stz,
    /*    */ "IDXVEC"_Stz,
    /* 33 */ "INDEXLEVELS"_Stz,

    /* 34 */ "JUMBOHEAP"_Stz,
    
    /*    */ "KEY"_Stz,
    /*    */ "KEYVAL"_Stz,
    /*    */ "KEYVEC"_Stz,
    /* 35 */ "KEYWRK"_Stz,
  
    /* 36 */ "LINEITEM"_Stz,
    /* 36 */ "LOADCHUNK"_Stz,
    /* 37 */ "LOADER"_Stz,
    /* 38 */ "LOADERSUB"_Stz,
    /* 39 */ "LOADMAP"_Stz,
    /* 40 */ "LOCK"_Stz,

    /* 41 */ "MEDIUMHEAP"_Stz,
    
    /* 42 */ "NODE"_Stz,
    /* 43 */ "NODECURSOR"_Stz,
  
    /* 43 */ "OP"_Stz,
  
    /* 44 */ "PARSER"_Stz,
    /* 45 */ "PARTITION"_Stz,
    /*    */ "PARTITIONSCHEDULER"_Stz,
    /*    */ "PARTITIONVEC"_Stz,
    /*    */ "PATHVEC"_Stz,
    /* 46 */ "PLAN"_Stz,
    /*    */ "PLANNER"_Stz,
    /* 47 */ "PLANLIST"_Stz,
    /*    */ "POSIXTHREAD"_Stz,
    /* 48 */ "PREDICATE"_Stz,
    /*    */ "PREDICATEVEC"_Stz,
    /* 48 */ "PROXY"_Stz,
    /* 49 */ "PROXYSET"_Stz,
    /*    */ "PTNCOLVEC"_Stz,
    
    /* 50 */ "QUEUE"_Stz,
    /* 51 */ "QUEUEBUFFER"_Stz,
  
    /* 52 */ "RBCURSOR"_Stz,
    /* 53 */ "REGFRAME"_Stz,
    /* 54 */ "RING"_Stz,
    /*    */ "ROARRAY"_Stz,
    /* 55 */ "ROW"_Stz,
    /* 56 */ "ROWBUFFER"_Stz,
    /* 57 */ "ROWCURSOR"_Stz,
  
    /* 60 */ "SCANBUFFER"_Stz,
    /* 61 */ "SCRATCH"_Stz,
    /* 62 */ "SEHISTORY"_Stz,
    /* 63 */ "SEITF"_Stz,
    /* 64 */ "SEITFCURSOR"_Stz,
    /* 65 */ "SEITFLIST"_Stz,
    /* 66 */ "SELCURSOR"_Stz,
    /* 67 */ "SEMAPHORE"_Stz,
    /* 67 */ "SEPROMPT"_Stz,
    /* 68 */ "SINCE"_Stz,
    /* 69 */ "SKIPCURSOR"_Stz,
    /* 70 */ "SKIPLIST"_Stz,
    /* 71 */ "SKIPNODE"_Stz,
    /* 72 */ "PARTITIONS"_Stz,
    /* 73 */ "SMALLHEAP"_Stz,
    /* 74 */ "SOCKET"_Stz,
    /* 75 */ "STACK"_Stz,
    /* 76 */ "STEPACCUM"_Stz,
    /* 77 */ "STEPARRAY"_Stz,
    /* 78 */ "SYSCALL"_Stz,
  
    /* 79 */ "TABLE"_Stz,
    /* 80 */ "TASK"_Stz,
    /* 81 */ "TASKCURSOR"_Stz,
    /* 82 */ "TASKLIST"_Stz,
    /*    */ "TASKQUEUE"_Stz,
    /*    */ "SERVERSCHEDULER"_Stz,
    /* 83 */ "TASKSET"_Stz,
    /* 84 */ "THREAD"_Stz,
    /* 85 */ "THREADCURSOR"_Stz,
    /* 86 */ "THREADLIST"_Stz,
    /*    */ "THREADQUEUE"_Stz,
    /* 87 */ "TIME"_Stz,
    /* 88 */ "TIMER"_Stz,
    /* 89 */ "TOKEN"_Stz,
    /* 89 */ "TOKENISER"_Stz,
    /*    */ "TOKENARY"_Stz,
    /*    */ "TOKENVEC"_Stz,
    /* 90 */ "TX"_Stz,
    /* 91 */ "TXARRAY"_Stz,
    /* 92 */ "TXCOLLECTION"_Stz,
    /* 93 */ "TXCURSOR"_Stz,
    /* 94 */ "TXMAP"_Stz,
    /* 95 */ "TXREF"_Stz,

    /* 96 */ "WORD"_Stz,
    
    /* 97 */ "ZSMALL_"_Stz,
    /* 98 */ "ZMEDIUM_"_Stz,
    /* 99 */ "ZLARGE_"_Stz,
    /*100 */ "ZJUMBO_"_Stz,
  
    /*101 */ "ENUMS"_Stz
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
