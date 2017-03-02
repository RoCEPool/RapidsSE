/*============================================================================*/
/* cn.hpp - class number                                                      */
/*============================================================================*/
#pragma once

#include "Platform.h"

namespace Platform
{
  enum eCN // Class Number
  {
    //
    // if you change this enum, change Task::vBoot()
    //
    
    /*  0 */ CN_NONE=0,
    
    /*  1 */ CN_BINVAL,
    /*  2 */ CN_ENTWSP,
    /*  3 */ CN_INTVAL,
    /*  4 */ CN_IPVAL,
    /*  5 */ CN_OBJVAL,
    /*  6 */ CN_PTRVAL,
    /*  7 */ CN_STRFXD,
    /*  8 */ CN_STRVAL,
    /*  9 */ CN_VALREG,
    /* 10 */ CN_VALSET,
    /* 11 */ CN_VALWSP,
    
    /* 12 */ CN_ARGUMENTS,
    /* 13 */ CN_ATOMIC,
    /* 14 */ CN_ATTACHMAP,
    
    /* 15 */ CN_BUFFERPOOL,
    /* 16 */ CN_BPA,
  
    /* 17 */ CN_DATA,
    /* 18 */ CN_DIC,
    /* 19 */ CN_DICPATH,
    /* 20 */ CN_DIR,
  
    /* 21 */ CN_ENTRY,
    
    /* 22 */ CN_FILE,
    /* 23 */ CN_FREEBUFFER,
    /* 24 */ CN_FREECHUNK,
    
    /* 25 */ CN_GRAPH,
    /* 26 */ CN_GRAPHCURSOR,
    /* 27 */ CN_GRAPHNODE,
    /* 28 */ CN_GRID,
  
    /* 29 */ CN_HEAPBUFFER,
    
    /* 30 */ CN_INDEX,
    /* 31 */ CN_INDEXBUFFER,
    /* 32 */ CN_INDEXDEFN,
    /* 33 */ CN_INDEXLEVELS,
  
    /* 34 */ CN_JUMBOHEAP,
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
  
  static const char* const pCN[]
  {
    /*  0 */ "NONE",
    
  
    /*  1 */ "BINVAL",
    /*  2 */ "ENTWSP",
    /*  3 */ "INTVAL",
    /*  4 */ "IPVAL",
    /*  5 */ "OBJVAL",
    /*  6 */ "PTRVAL",
    /*  7 */ "STRFXD",
    /*  8 */ "STRVAL",
    /*  9 */ "VALREG",
    /* 10 */ "VALSET",
    /* 11 */ "VALWSP",
    
    /* 12 */ "ARGUMENTS",
    /* 13 */ "ATOMIC",
    /* 14 */ "ATTACHMAP",
    
    /* 15 */ "BUFFERPOOL",
    /* 16 */ "BPA",
    
    /* 17 */ "DATA",
    /* 18 */ "DIC",
    /* 19 */ "DICPATH",
    /* 20 */ "DIR",
    
    /* 21 */ "ENTRY",
    /* 22 */ "FILE",
    /* 23 */ "FREEBUFFER",
    /* 24 */ "FREECHUNK",
    
    /* 25 */ "GRAPH",
    /* 26 */ "GRAPHCURSOR",
    /* 27 */ "GRAPHNODE",
    /* 28 */ "GRID",
    
    /* 29 */ "HEAPBUFFER",
    
    /* 30 */ "INDEX",
    /* 31 */ "INDEXBUFFER",
    /* 32 */ "INDEXDEFN",
    /* 33 */ "INDEXLEVELS",

    /* 34 */ "JUMBOHEAP",
    
    /* 35 */ "KEYVAL",
  
    /* 36 */ "LOADCHUNK",
    /* 37 */ "LOADER",
    /* 38 */ "LOADERSUB",
    /* 39 */ "LOADMAP",
    /* 40 */ "LOCK",

    /* 41 */ "MEDIUMHEAP",
    
    /* 42 */ "NODE",
    /* 43 */ "NODECURSOR",
    
    /* 44 */ "PARSER",
    /* 45 */ "PARTITION",
    /* 46 */ "PLAN",
    /* 47 */ "PLANLIST",
    /* 48 */ "PROXY",
    /* 49 */ "PROXYSET",
    
    /* 50 */ "QUEUE",
    /* 51 */ "QUEUEBUFFER",
  
    /* 52 */ "RBCURSOR",
    /* 53 */ "REGFRAME",
    /* 54 */ "RING",
    /* 55 */ "ROW",
    /* 56 */ "ROWBUFFER",
    /* 57 */ "ROWCURSOR",
    /* 58 */ "ROWSTRAND",
    /* 59 */ "ROWWSP",
  
    /* 60 */ "SCANBUFFER",
    /* 61 */ "SCRATCH",
    /* 62 */ "SEHISTORY",
    /* 63 */ "SEITF",
    /* 64 */ "SEITFCURSOR",
    /* 65 */ "SEITFLIST",
    /* 66 */ "SELCURSOR",
    /* 67 */ "SEPROMPT",
    /* 68 */ "SINCE",
    /* 69 */ "SKIPCURSOR",
    /* 70 */ "SKIPLIST",
    /* 71 */ "SKIPNODE",
    /* 72 */ "PARTITIONS",
    /* 73 */ "SMALLHEAP",
    /* 74 */ "SOCKET",
    /* 75 */ "STACK",
    /* 76 */ "STEPACCUM",
    /* 77 */ "STEPARRAY",
    /* 78 */ "SYSCALL",
  
    /* 79 */ "TABLE",
    /* 80 */ "TASK",
    /* 81 */ "TASKCURSOR",
    /* 82 */ "TASKLIST",
    /* 83 */ "TASKSET",
    /* 84 */ "THREAD",
    /* 85 */ "THREADCURSOR",
    /* 86 */ "THREADLIST",
    /* 87 */ "TIME",
    /* 88 */ "TIMER",
    /* 89 */ "TOKENISER",
    /* 90 */ "TX",
    /* 91 */ "TXARRAY",
    /* 92 */ "TXCOLLECTION",
    /* 93 */ "TXCURSOR",
    /* 94 */ "TXMAP",
    /* 95 */ "TXREF",

    /* 96 */ "WORD",
    
    /* 97 */ "ZSMALL_",
    /* 98 */ "ZMEDIUM_",
    /* 99 */ "ZLARGE_",
    /*100 */ "ZJUMBO_",
  
    /*101 */ "ENUMS"
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
