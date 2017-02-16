/*============================================================================*/
/* cn.hpp - class number                                                      */
/*============================================================================*/
#pragma once

#include "Magic.h"

namespace Magic
{
  enum eCN // Class Number
  {
    //
    // if you change this enum, change Task::vBoot()
    //
    
    /*  0 */ CN_NONE=0,
    
    /*  1 */ CN_S32,
    /*  2 */ CN_S256,
    /*  3 */ CN_S48K,
    /*  4 */ CN_S1M,
    
    /*  5 */ CN_BINVAL,
    /*    */ CN_ENTWSP,
    /*  6 */ CN_INTVAL,
    /*  7 */ CN_IPVAL,
    /*  8 */ CN_OBJVAL,
    /*  9 */ CN_PTRVAL,
    /* 10 */ CN_STRVAL,
    /* 11 */ CN_VALREG,
    /* 12 */ CN_VALSET,
    /* 13 */ CN_VALWSP,
    
    /* 14 */ CN_ARGUMENTS,
    /* 15 */ CN_ATOMIC,
    /* 16 */ CN_ATTACHMAP,
    
    /* 17 */ CN_BUFFERPOOL,
    /* 18 */ CN_BPA,
  
    /* 19 */ CN_DATA,
    /* 20 */ CN_DIC,
    /* 21 */ CN_DICPATH,
    /* 22 */ CN_DIR,
  
    /* 23 */ CN_ENTRY,
    
    /* 24 */ CN_FILE,
    /* 25 */ CN_FREEBUFFER,
    /* 26 */ CN_FREECHUNK,
    
    /* 27 */ CN_GRAPH,
    /* 28 */ CN_GRAPHCURSOR,
    /* 29 */ CN_GRAPHNODE,
    /* 30 */ CN_GRID,
  
    /* 31 */ CN_HEAPBUFFER,
    
    /* 32 */ CN_INDEX,
    /* 33 */ CN_INDEXBUFFER,
    /* 34 */ CN_INDEXDEFN,
    /* 35 */ CN_INDEXLEVELS,
  
    /* 36 */ CN_JUMBOHEAP,
    
    /* 37 */ CN_KEYCPP,         // no longer used
    /* 38 */ CN_KEYVAL,         // no longer used
  
    /* 39 */ CN_LOADCHUNK,
    /* 40 */ CN_LOADER,
    /* 41 */ CN_LOADERSUB,
    /* 42 */ CN_LOADMAP,
    /* 43 */ CN_LOCK,
    
    /* 44 */ CN_MEDIUMHEAP,

    /* 45 */ CN_NODE,
    /* 46 */ CN_NODECURSOR,
    
    /* 47 */ CN_PARSER,
    /* 48 */ CN_PARTITION,
    /* 49 */ CN_PLAN,
    /* 50 */ CN_PLANLIST,
    /* 51 */ CN_PROXY,
    /* 52 */ CN_PROXYSET,
    
    /* 53 */ CN_QUEUE,
    /* 54 */ CN_QUEUEBUFFER,
    
    /* 55 */ CN_RBCURSOR,
    /* 56 */ CN_REGFRAME,
    /* 57 */ CN_RING,
    /* 58 */ CN_ROW,
    /* 59 */ CN_ROWBUFFER,
    /* 60 */ CN_ROWCURSOR,
    /* 61 */ CN_ROWSTRAND,
    /* 62 */ CN_ROWWSP,
    
    /* 63 */ CN_SCANBUFFER,
    /* 64 */ CN_SCRATCH,
    /* 65 */ CN_SEHISTORY,
    /* 66 */ CN_SEITF,
    /* 67 */ CN_SEITFCURSOR,
    /* 68 */ CN_SEITFLIST,
    /* 69 */ CN_SELCURSOR,
    /* 70 */ CN_SEPROMPT,
    /* 71 */ CN_SINCE,
    /* 72 */ CN_SKIPCURSOR,
    /* 73 */ CN_SKIPLIST,
    /* 74 */ CN_SKIPNODE,
    /* 75 */ CN_SLOTS,
    /* 76 */ CN_SMALLHEAP,
    /* 77 */ CN_SOCKET,
    /* 78 */ CN_STACK,
    /* 79 */ CN_STEPACCUM,
    /* 80 */ CN_STEPARRAY,
    /* 81 */ CN_SYSCALL,
  
    /* 82 */ CN_TABLE,
    /* 83 */ CN_TASK,
    /* 84 */ CN_TASKCURSOR,
    /* 85 */ CN_TASKLIST,
    /* 86 */ CN_TASKSET,
    /* 87 */ CN_THREAD,
    /* 88 */ CN_THREADCURSOR,
    /* 89 */ CN_THREADLIST,
    /* 90 */ CN_TIME,
    /* 91 */ CN_TIMER,
    /* 92 */ CN_TOKENISER,
    /* 93 */ CN_TX,
    /* 94 */ CN_TXARRAY,
    /* 95 */ CN_TXCOLLECTION,
    /* 96 */ CN_TXCURSOR,
    /* 97 */ CN_TXMAP,
    /* 98 */ CN_TXREF,
  
    /* 99 */ CN_WORD,
    
    /*100 */ CN_ZSMALL_,
    /*101 */ CN_ZMEDIUM_,
    /*102 */ CN_ZLARGE_,
    /*103 */ CN_ZJUMBO_,
  
    /*104 */ CN_ENUMS
  };
  
  static const char* const pCN[]
  {
    /*  0 */ "NONE",
    
    /*  1 */ "S32",
    /*  2 */ "S256",
    /*  3 */ "S8K",
    /*  4 */ "S1M",
    
    /*  5 */ "BINVAL",
    /*    */ "ENTWSP",
    /*  6 */ "INTVAL",
    /*  7 */ "IPVAL",
    /*  8 */ "OBJVAL",
    /*  9 */ "PTRVAL",
    /* 10 */ "STRVAL",
    /* 11 */ "VALREG",
    /* 12 */ "VALSET",
    /* 13 */ "VALWSP",
    
    /* 14 */ "ARGUMENTS",
    /* 15 */ "ATOMIC",
    /* 16 */ "ATTACHMAP",
    
    /* 17 */ "BUFFERPOOL",
    /* 18 */ "BPA",
    
    /* 19 */ "DATA",
    /* 20 */ "DIC",
    /* 21 */ "DICPATH",
    /* 22 */ "DIR",
    
    /* 23 */ "ENTRY",
    /* 24 */ "FILE",
    /* 25 */ "FREEBUFFER",
    /* 26 */ "FREECHUNK",
    
    /* 27 */ "GRAPH",
    /* 28 */ "GRAPHCURSOR",
    /* 29 */ "GRAPHNODE",
    /* 30 */ "GRID",
    
    /* 31 */ "HEAPBUFFER",
    
    /* 32 */ "INDEX",
    /* 33 */ "INDEXBUFFER",
    /* 34 */ "INDEXDEFN",
    /* 35 */ "INDEXLEVELS",
  
    /* 36 */ "JUMBOHEAP",
    
    /* 37 */ "KEYCPP",
    /* 38 */ "KEYVAL",
  
    /* 39 */ "LOADCHUNK",
    /* 40 */ "LOADER",
    /* 41 */ "LOADERSUB",
    /* 42 */ "LOADMAP",
    /* 43 */ "LOCK",

    /* 44 */ "MEDIUMHEAP",
    
    /* 45 */ "NODE",
    /* 46 */ "NODECURSOR",
    
    /* 47 */ "PARSER",
    /* 48 */ "PARTITION",
    /* 49 */ "PLAN",
    /* 50 */ "PLANLIST",
    /* 51 */ "PROXY",
    /* 52 */ "PROXYSET",
    
    /* 53 */ "QUEUE",
    /* 54 */ "QUEUEBUFFER",
  
    /* 55 */ "RBCURSOR",
    /* 56 */ "REGFRAME",
    /* 57 */ "RING",
    /* 58 */ "ROW",
    /* 59 */ "ROWBUFFER",
    /* 60 */ "ROWCURSOR",
    /* 61 */ "ROWSTRAND",
    /* 62 */ "ROWWSP",
  
    /* 63 */ "SCANBUFFER",
    /* 64 */ "SCRATCH",
    /* 65 */ "SEHISTORY",
    /* 66 */ "SEITF",
    /* 67 */ "SEITFCURSOR",
    /* 68 */ "SEITFLIST",
    /* 69 */ "SELCURSOR",
    /* 70 */ "SEPROMPT",
    /* 71 */ "SINCE",
    /* 72 */ "SKIPCURSOR",
    /* 73 */ "SKIPLIST",
    /* 74 */ "SKIPNODE",
    /* 75 */ "PARTITIONS",
    /* 76 */ "SMALLHEAP",
    /* 77 */ "SOCKET",
    /* 78 */ "STACK",
    /* 79 */ "STEPACCUM",
    /* 80 */ "STEPARRAY",
    /* 81 */ "SYSCALL",
  
    /* 82 */ "TABLE",
    /* 83 */ "TASK",
    /* 84 */ "TASKCURSOR",
    /* 85 */ "TASKLIST",
    /* 86 */ "TASKSET",
    /* 87 */ "THREAD",
    /* 88 */ "THREADCURSOR",
    /* 89 */ "THREADLIST",
    /* 90 */ "TIME",
    /* 91 */ "TIMER",
    /* 92 */ "TOKENISER",
    /* 93 */ "TX",
    /* 94 */ "TXARRAY",
    /* 95 */ "TXCOLLECTION",
    /* 96 */ "TXCURSOR",
    /* 97 */ "TXMAP",
    /* 98 */ "TXREF",

    /* 99 */ "WORD",
    
    /*100 */ "ZSMALL_",
    /*101 */ "ZMEDIUM_",
    /*102 */ "ZLARGE_",
    /*103 */ "ZJUMBO_",
  
    /*104 */ "ENUMS"
  };
  
  class cn
  {
  private:
    eCN m_eClassNo;
    //u4  m_uFiller;
    
  public:
    cn(void);
    cn(eCN eClassNo);
    ~cn(void);
    
    void operator =(eCN eClassNo);
    bool operator ==(eCN eClassNo) const;
    eCN  eClass(void) const;
    void vClass(eCN eClassNo);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
