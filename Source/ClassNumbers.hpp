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
    /*  5 */ CN_ENTWSP,
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
    /* 18 */ CN_VALWSP,
    
    /* 12 */ CN_ARGUMENTS,
    /* 13 */ CN_ATOMIC,
    /* 14 */ CN_ATTACHMAP,
    
    /* 15 */ CN_BUFFERPOOL,
    /* 16 */ CN_BPA,
  
    /*    */ CN_COLUMN,
    /*    */ CN_COLUMNVEC,
  
    /* 17 */ CN_DATA,
    /* 18 */ CN_DIC,
    /* 19 */ CN_DICPATH,
    /* 20 */ CN_DIR,
  
    /* 21 */ CN_ENTRY,
    
    /* 22 */ CN_FILE,
    /* 23 */ CN_FREEBUFFER,
    /* 24 */ CN_FREECHUNK,
    /*    */ CN_FREEDNOT,
    
    /* 25 */ CN_GRAPH,
    /* 26 */ CN_GRAPHCURSOR,
    /* 27 */ CN_GRAPHNODE,
    /* 28 */ CN_GRID,
  
    /* 29 */ CN_HEAPBUFFER,
    
    /* 30 */ CN_INDEX,
    /* 31 */ CN_INDEXBUFFER,
    /*    */ CN_IDXCOLVEC,
    /*    */ CN_IDXVEC,
    /* 33 */ CN_INDEXLEVELS,
  
    /* 34 */ CN_JUMBOHEAP,
  
    /*    */ CN_KEYVEC,
    /* 35 */ CN_KEYWSP,
  
    /* 36 */ CN_LOADCHUNK,
    /* 37 */ CN_LOADER,
    /* 38 */ CN_LOADERSUB,
    /* 39 */ CN_LOADMAP,
    /* 40 */ CN_LOCK,
    
    /* 41 */ CN_MEDIUMHEAP,

    /* 42 */ CN_NODE,
    /* 43 */ CN_NODECURSOR,
    
    /* 44 */ CN_PARSER,
    /* 45 */ CN_PARTITION,
    /* 46 */ CN_PLAN,
    /* 47 */ CN_PLANLIST,
    /*    */ CN_POSIXTHREAD,
    /* 48 */ CN_PROXY,
    /* 49 */ CN_PROXYSET,
    
    /* 50 */ CN_QUEUE,
    /* 51 */ CN_QUEUEBUFFER,
    
    /* 52 */ CN_RBCURSOR,
    /* 53 */ CN_REGFRAME,
    /* 54 */ CN_RING,
    /* 55 */ CN_ROW,
    /* 56 */ CN_ROWBUFFER,
    /* 57 */ CN_ROWCURSOR,
    /* 58 */ CN_ROWSTRAND,
    /* 59 */ CN_ROWWSP,
    
    /* 60 */ CN_SCANBUFFER,
    /* 61 */ CN_SCRATCH,
    /* 62 */ CN_SEHISTORY,
    /* 63 */ CN_SEITF,
    /* 64 */ CN_SEITFCURSOR,
    /* 65 */ CN_SEITFLIST,
    /* 66 */ CN_SELCURSOR,
    /* 67 */ CN_SEPROMPT,
    /* 68 */ CN_SINCE,
    /* 69 */ CN_SKIPCURSOR,
    /* 70 */ CN_SKIPLIST,
    /* 71 */ CN_SKIPNODE,
    /* 72 */ CN_SLOTS,
    /* 73 */ CN_SMALLHEAP,
    /* 74 */ CN_SOCKET,
    /* 75 */ CN_STACK,
    /* 76 */ CN_STEPACCUM,
    /* 77 */ CN_STEPARRAY,
    /* 78 */ CN_SYSCALL,
  
    /* 79 */ CN_TABLE,
    /* 80 */ CN_TASK,
    /* 81 */ CN_TASKCURSOR,
    /* 82 */ CN_TASKLIST,
    /* 83 */ CN_TASKSET,
    /* 84 */ CN_THREAD,
    /* 85 */ CN_THREADCURSOR,
    /* 86 */ CN_THREADLIST,
    /* 87 */ CN_TIME,
    /* 88 */ CN_TIMER,
    /* 89 */ CN_TOKENISER,
    /* 90 */ CN_TX,
    /* 91 */ CN_TXARRAY,
    /* 92 */ CN_TXCOLLECTION,
    /* 93 */ CN_TXCURSOR,
    /* 94 */ CN_TXMAP,
    /* 95 */ CN_TXREF,
  
    /* 96 */ CN_WORD,
    
    /* 97 */ CN_ZSMALL_,
    /* 98 */ CN_ZMEDIUM_,
    /* 99 */ CN_ZLARGE_,
    /*100 */ CN_ZJUMBO_,
  
    /*101 */ CN_ENUMS
  };
  
  static Str* aCN[]
  {
    /*  0 */ "NONE"_Str,
    
  
    /*  1 */ "BCDVAL"_Str,
    /*  2 */ "BINVAL"_Str,
    /*  3 */ "BOLVAL"_Str,
    /*  4 */ "DTEVAL"_Str,
    /*  5 */ "ENTWSP"_Str,
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
    /* 18 */ "VALWSP"_Str,
    
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
    /* 22 */ "FILE"_Str,
    /* 23 */ "FREEBUFFER"_Str,
    /* 24 */ "FREECHUNK"_Str,
    /*    */ "FREEDNOT"_Str,
    
    /* 25 */ "GRAPH"_Str,
    /* 26 */ "GRAPHCURSOR"_Str,
    /* 27 */ "GRAPHNODE"_Str,
    /* 28 */ "GRID"_Str,
    
    /* 29 */ "HEAPBUFFER"_Str,
    
    /* 30 */ "INDEX"_Str,
    /* 31 */ "INDEXBUFFER"_Str,
    /*    */ "IDXCOLVEC"_Str,
    /*    */ "IDXVEC"_Str,
    /* 33 */ "INDEXLEVELS"_Str,

    /* 34 */ "JUMBOHEAP"_Str,
    
    /*    */ "KEYVEC"_Str,
    /* 35 */ "KEYWSP"_Str,
  
    /* 36 */ "LOADCHUNK"_Str,
    /* 37 */ "LOADER"_Str,
    /* 38 */ "LOADERSUB"_Str,
    /* 39 */ "LOADMAP"_Str,
    /* 40 */ "LOCK"_Str,

    /* 41 */ "MEDIUMHEAP"_Str,
    
    /* 42 */ "NODE"_Str,
    /* 43 */ "NODECURSOR"_Str,
    
    /* 44 */ "PARSER"_Str,
    /* 45 */ "PARTITION"_Str,
    /* 46 */ "PLAN"_Str,
    /* 47 */ "PLANLIST"_Str,
    /*    */ "POSIXTHREAD"_Str,
    /* 48 */ "PROXY"_Str,
    /* 49 */ "PROXYSET"_Str,
    
    /* 50 */ "QUEUE"_Str,
    /* 51 */ "QUEUEBUFFER"_Str,
  
    /* 52 */ "RBCURSOR"_Str,
    /* 53 */ "REGFRAME"_Str,
    /* 54 */ "RING"_Str,
    /* 55 */ "ROW"_Str,
    /* 56 */ "ROWBUFFER"_Str,
    /* 57 */ "ROWCURSOR"_Str,
    /* 58 */ "ROWSTRAND"_Str,
    /* 59 */ "ROWWSP"_Str,
  
    /* 60 */ "SCANBUFFER"_Str,
    /* 61 */ "SCRATCH"_Str,
    /* 62 */ "SEHISTORY"_Str,
    /* 63 */ "SEITF"_Str,
    /* 64 */ "SEITFCURSOR"_Str,
    /* 65 */ "SEITFLIST"_Str,
    /* 66 */ "SELCURSOR"_Str,
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
    /* 83 */ "TASKSET"_Str,
    /* 84 */ "THREAD"_Str,
    /* 85 */ "THREADCURSOR"_Str,
    /* 86 */ "THREADLIST"_Str,
    /* 87 */ "TIME"_Str,
    /* 88 */ "TIMER"_Str,
    /* 89 */ "TOKENISER"_Str,
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
