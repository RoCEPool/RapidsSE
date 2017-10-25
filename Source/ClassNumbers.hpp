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
    
    /* 26 */ CN_BITARRAY,
    /* 27 */ CN_BITMAP,
    /* 28 */ CN_BUFFERPOOL,
    /* 29 */ CN_BPA,
  
    /* 30 */ CN_COLUMN,
    /* 31 */ CN_COLUMNVEC,
  
    /* 32 */ CN_DIC,
    /* 33 */ CN_DICPATH,
    /* 34 */ CN_DIR,
  
    /* 35 */ CN_ENTRY,
    /* 36 */ CN_ENTRYBUFFER,
    /* 37 */ CN_ENTRYVEC,
    
    /* 38 */ CN_FILE,
    /* 39 */ CN_FREEBUFFER,
    /* 40 */ CN_FREECHUNK,
    /* 41 */ CN_FREEDNOT,
    
    /* 42 */ CN_GRAPH,
    /* 43 */ CN_GRAPHCURSOR,
    /* 44 */ CN_GRAPHNODE,
    /* 45 */ CN_GRID,
    /* 46 */ CN_GROUPBY,
  
    /* 47 */ CN_HEAPBUFFER,
    
    /* 48 */ CN_INDEX,
    /* 49 */ CN_INDEXDATA,
    /* 50 */ CN_IDXCOLVEC,
    /* 51 */ CN_IDXDATAVEC,
    /* 52 */ CN_IDXVEC,
    /* 53 */ CN_INDEXLEVELS,
  
    /* 54 */ CN_JUMBOHEAP,
  
    /* 55 */ CN_KEY,
    /* 56 */ CN_KEYVAL,
    /* 57 */ CN_KEYVEC,
    /* 58 */ CN_KEYWRK,
  
    /* 59 */ CN_LINEITEM,
    /* 60 */ CN_LOADCHUNK,
    /* 61 */ CN_LOADER,
    /* 62 */ CN_LOADERSUB,
    /* 63 */ CN_LOADMAP,
    /* 64 */ CN_LOCK,
    
    /* 65 */ CN_MEDIUMHEAP,

    /* 66 */ CN_NODE,
    /* 67 */ CN_NODECURSOR,
  
    /* 68 */ CN_OP,
  
    /* 69 */ CN_PARSER,
    /* 70 */ CN_PARTITION,
    /* 71 */ CN_PARTITIONSCHEDULER,
    /* 72 */ CN_PARTITIONVEC,
    /* 73 */ CN_PATHVEC,
    /* 74 */ CN_PLAN,
    /* 75 */ CN_PLANNER,
    /* 76 */ CN_PLANLIST,
    /* 77 */ CN_POSIXTHREAD,
    /* 78 */ CN_PREDICATE,
    /* 79 */ CN_PREDICATEVEC,
    /* 80 */ CN_PROXY,
    /* 81 */ CN_PROXYSET,
    /* 82 */ CN_PTNCOLVEC,
  
    /* 83 */ CN_QUEUE,
    /* 84 */ CN_QUEUEBUFFER,
    
    /* 85 */ CN_RBCURSOR,
    /* 86 */ CN_REGFRAME,
    /* 87 */ CN_RING,
    /* 88 */ CN_ROARRAY,
    /* 89 */ CN_ROW,
    /* 90 */ CN_ROWBUFFER,
    /* 91 */ CN_ROWCURSOR,
    /*    */ CN_ROWONCE,
  
    /* 92 */ CN_SCANBUFFER,
    /* 93 */ CN_SCRATCH,
    /* 94 */ CN_SEHISTORY,
    /* 95 */ CN_SEITF,
    /* 96 */ CN_SEITFCURSOR,
    /* 97 */ CN_SEITFLIST,
    /* 98 */ CN_SELCURSOR,
    /* 99 */ CN_SEMAPHORE,
    /*100 */ CN_SEPROMPT,
    /*101 */ CN_SINCE,
    /*102 */ CN_SKIPCURSOR,
    /*103 */ CN_SKIPLIST,
    /*104 */ CN_SKIPNODE,
    /*    */ CN_SLAB,
    /*    */ CN_SLABALLOCATOR,
    /*105 */ CN_SLOTS,
    /*106 */ CN_SMALLHEAP,
    /*107 */ CN_SOCKET,
    /*108 */ CN_STACK,
    /*109 */ CN_STEPACCUM,
    /*110 */ CN_STEPARRAY,
    /*111 */ CN_SYSCALL,

    /*112 */ CN_TABLE,
    /*113 */ CN_TASK,
    /*114 */ CN_TASKCURSOR,
    /*115 */ CN_TASKLIST,
    /*116 */ CN_TASKQUEUE,
    /*117 */ CN_SERVERSCHEDULER,
    /*118 */ CN_TASKSET,
    /*119 */ CN_THREAD,
    /*120 */ CN_THREADCURSOR,
    /*121 */ CN_THREADLIST,
    /*122 */ CN_THREADQUEUE,
    /*123 */ CN_TIME,
    /*124 */ CN_TIMER,
    /*125 */ CN_TOKEN,
    /*126 */ CN_TOKENISER,
    /*127 */ CN_TOKENARY,
    /*128 */ CN_TOKENVEC,
    /*129 */ CN_TX,
    /*130 */ CN_TXARRAY,
    /*131 */ CN_TXCOLLECTION,
    /*132 */ CN_TXCURSOR,
    /*133 */ CN_TXMAP,
    /*134 */ CN_TXREF,
    /*    */ CN_TXSET,
  
    /*135 */ CN_WORD,
    
    /*136 */ CN_ZSMALL_,
    /*137 */ CN_ZMEDIUM_,
    /*138 */ CN_ZLARGE_,
    /*139 */ CN_ZJUMBO_,
  
    /*140 */ CN_ENUMS
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
  
    /* 30 */ "COLUMN"_Stz,
    /* 31 */ "COLUMNVEC"_Stz,
    
    /* 32 */ "DIC"_Stz,
    /* 33 */ "DICPATH"_Stz,
    /* 34 */ "DIR"_Stz,
    
    /* 35 */ "ENTRY"_Stz,
    /* 36 */ "ENTRYBUFFER"_Stz,
    /* 37 */ "ENTRYVEC"_Stz,
  
    /* 38 */ "FILE"_Stz,
    /* 39 */ "FREEBUFFER"_Stz,
    /* 40 */ "FREECHUNK"_Stz,
    /* 41 */ "FREEDNOT"_Stz,
    
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

    /* 54 */ "JUMBOHEAP"_Stz,
    
    /* 55 */ "KEY"_Stz,
    /* 56 */ "KEYVAL"_Stz,
    /* 57 */ "KEYVEC"_Stz,
    /* 58 */ "KEYWRK"_Stz,
  
    /* 59 */ "LINEITEM"_Stz,
    /* 60 */ "LOADCHUNK"_Stz,
    /* 61 */ "LOADER"_Stz,
    /* 62 */ "LOADERSUB"_Stz,
    /* 63 */ "LOADMAP"_Stz,
    /* 64 */ "LOCK"_Stz,

    /* 65 */ "MEDIUMHEAP"_Stz,
    
    /* 66 */ "NODE"_Stz,
    /* 67 */ "NODECURSOR"_Stz,
  
    /* 68 */ "OP"_Stz,
  
    /* 69 */ "PARSER"_Stz,
    /* 70 */ "PARTITION"_Stz,
    /* 71 */ "PARTITIONSCHEDULER"_Stz,
    /* 72 */ "PARTITIONVEC"_Stz,
    /* 73 */ "PATHVEC"_Stz,
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
    /* 87 */ "RING"_Stz,
    /* 88 */ "ROARRAY"_Stz,
    /* 89 */ "ROW"_Stz,
    /* 90 */ "ROWBUFFER"_Stz,
    /* 91 */ "ROWCURSOR"_Stz,
    /*    */ "ROWONCE"_Stz,
  
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
    /*    */ "SLAB"_Stz,
    /*    */ "SLABALLOCATOR"_Stz,
    /*105 */ "SLOTS"_Stz,
    /*106 */ "SMALLHEAP"_Stz,
    /*107 */ "SOCKET"_Stz,
    /*108 */ "STACK"_Stz,
    /*109 */ "STEPACCUM"_Stz,
    /*110 */ "STEPARRAY"_Stz,
    /*111 */ "SYSCALL"_Stz,
  
    /*112 */ "TABLE"_Stz,
    /*113 */ "TASK"_Stz,
    /*114 */ "TASKCURSOR"_Stz,
    /*115 */ "TASKLIST"_Stz,
    /*116 */ "TASKQUEUE"_Stz,
    /*117 */ "SERVERSCHEDULER"_Stz,
    /*118 */ "TASKSET"_Stz,
    /*119 */ "THREAD"_Stz,
    /*120 */ "THREADCURSOR"_Stz,
    /*121 */ "THREADLIST"_Stz,
    /*122 */ "THREADQUEUE"_Stz,
    /*123 */ "TIME"_Stz,
    /*124 */ "TIMER"_Stz,
    /*125 */ "TOKEN"_Stz,
    /*126 */ "TOKENISER"_Stz,
    /*127 */ "TOKENARY"_Stz,
    /*128 */ "TOKENVEC"_Stz,
    /*129 */ "TX"_Stz,
    /*130 */ "TXARRAY"_Stz,
    /*131 */ "TXCOLLECTION"_Stz,
    /*132 */ "TXCURSOR"_Stz,
    /*133 */ "TXMAP"_Stz,
    /*134 */ "TXREF"_Stz,
    /*    */ "TXSET"_Stz,
    
    /*135 */ "WORD"_Stz,
    
    /*136 */ "ZSMALL_"_Stz,
    /*137 */ "ZMEDIUM_"_Stz,
    /*138 */ "ZLARGE_"_Stz,
    /*139 */ "ZJUMBO_"_Stz,
  
    /*140 */ "ENUMS"_Stz
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
