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
    
    /*    */ CN_BINVAL,
    /*  5 */ CN_INTVAL,
    /*  6 */ CN_IPVAL,
    /*  7 */ CN_OBJVAL,
    /*  8 */ CN_PTRVAL,
    /*  8 */ CN_STRVAL,
    /* 10 */ CN_VALREG,
    /* 11 */ CN_VALSET,
    /* 12 */ CN_VALWSP,
    
    /* 13 */ CN_ARGUMENTS,
    /* 14 */ CN_ATOMIC,
    /* 15 */ CN_ATTACHMAP,
    
    /* 16 */ CN_BUFFERPOOL,
    /* 17 */ CN_BPA,
  
    /* 18 */ CN_DIC,
    /* 19 */ CN_DICPATH,
    /* 20 */ CN_DIR,
    
    /* 21 */ CN_FILE,
    /* 22 */ CN_FREEBUFFER,
    /* 23 */ CN_FREECHUNK,
    
    /* 24 */ CN_GRAPH,
    /* 25 */ CN_GRAPHCURSOR,
    /* 26 */ CN_GRAPHNODE,
    /* 27 */ CN_GRID,
  
    /* 28 */ CN_HEAPBUFFER,
    
    /* 29 */ CN_INDEX,
    
    /* 30 */ CN_JUMBOHEAP,
    
    /* 31 */ CN_KEYCPP,
    /* 32 */ CN_KEYVAL,
    
    /* 33 */ CN_LOADCHUNK,
    /* 34 */ CN_LOADER,
    /* 35 */ CN_LOADERSUB,
    /* 36 */ CN_LOADMAP,
    /* 37 */ CN_LOCK,
    
    /* 38 */ CN_MEDIUMHEAP,

    /* 39 */ CN_NODE,
    /* 40 */ CN_NODECURSOR,
    
    /* 41 */ CN_PARSER,
    /* 42 */ CN_PARTITION,
    /*    */ CN_PLAN,
    /*    */ CN_PLANLIST,
    /* 44 */ CN_PROXY,
    /* 45 */ CN_PROXYSET,
    
    /* 46 */ CN_QUEUE,
    /* 47 */ CN_QUEUEBUFFER,
    
    /*    */ CN_RBCURSOR,
    /* 48 */ CN_REGFRAME,
    /* 49 */ CN_RING,
    /* 50 */ CN_ROW,
    /* 51 */ CN_ROWBUFFER,
    /* 52 */ CN_ROWCURSOR,
    /* 53 */ CN_ROWSTRAND,
    /* 54 */ CN_ROWWSP,
    
    /* 55 */ CN_SCANBUFFER,
    /* 56 */ CN_SCRATCH,
    /* 57 */ CN_SEHISTORY,
    /* 58 */ CN_SEITF,
    /* 59 */ CN_SEITFCURSOR,
    /* 60 */ CN_SEITFLIST,
    /*    */ CN_SELCURSOR,
    /* 61 */ CN_SEPROMPT,
    /* 62 */ CN_SINCE,
    /* 63 */ CN_SKIPCURSOR,
    /* 64 */ CN_SKIPLIST,
    /* 65 */ CN_SKIPNODE,
    /* 43 */ CN_SLOTS,
    /* 66 */ CN_SMALLHEAP,
    /* 67 */ CN_SOCKET,
    /* 68 */ CN_STACK,
    /* 69 */ CN_STEPACCUM,
    /* 70 */ CN_STEPARRAY,
    /* 71 */ CN_SYSCALL,
  
    /* 72 */ CN_TABLE,
    /* 73 */ CN_TASK,
    /* 74 */ CN_TASKCURSOR,
    /* 75 */ CN_TASKLIST,
    /* 76 */ CN_TASKSET,
    /* 77 */ CN_THREAD,
    /* 78 */ CN_THREADCURSOR,
    /* 79 */ CN_THREADLIST,
    /* 80 */ CN_TIME,
    /* 81 */ CN_TIMER,
    /* 82 */ CN_TOKENISER,
    /* 83 */ CN_TX,
    /* 84 */ CN_TXARRAY,
    /* 85 */ CN_TXCOLLECTION,
    /* 86 */ CN_TXCURSOR,
    /* 87 */ CN_TXMAP,
    /* 88 */ CN_TXREF,
  
    /* 89 */ CN_WORD,
    
    /* 90 */ CN_ZSMALL_,
    /* 91 */ CN_ZMEDIUM_,
    /* 92 */ CN_ZLARGE_,
    /* 93 */ CN_ZJUMBO_,
  
    /* 94 */ CN_ENUMS
  };
  
  static const char* const pCN[]
  {
    /*  0 */ "NONE",
    
    /*  1 */ "S32",
    /*  2 */ "S256",
    /*  3 */ "S8K",
    /*  4 */ "S1M",
    
    /*    */ "BINVAL",
    /*  5 */ "INTVAL",
    /*  6 */ "IPVAL",
    /*  7 */ "OBJVAL",
    /*  8 */ "PTRVAL",
    /*  9 */ "STRVAL",
    /* 10 */ "VALREG",
    /* 11 */ "VALSET",
    /* 12 */ "VALWSP",
    
    /* 13 */ "ARGUMENTS",
    /* 14 */ "ATOMIC",
    /* 15 */ "ATTACHMAP",
    
    /* 16 */ "BUFFERPOOL",
    /* 17 */ "BPA",
    
    /* 18 */ "DIC",
    /* 19 */ "DICPATH",
    /* 20 */ "DIR",
    
    /* 21 */ "FILE",
    /* 22 */ "FREEBUFFER",
    /* 23 */ "FREECHUNK",
    
    /* 24 */ "GRAPH",
    /* 25 */ "GRAPHCURSOR",
    /* 26 */ "GRAPHNODE",
    /* 27 */ "GRID",
    
    /* 28 */ "HEAPBUFFER",
    
    /* 29 */ "INDEX",
    
    /* 30 */ "JUMBOHEAP",
    
    /* 31 */ "KEYCPP",
    /* 32 */ "KEYVAL",
  
    /* 33 */ "LOADCHUNK",
    /* 34 */ "LOADER",
    /* 35 */ "LOADERSUB",
    /* 36 */ "LOADMAP",
    /* 37 */ "LOCK",

    /* 38 */ "MEDIUMHEAP",
    
    /* 39 */ "NODE",
    /* 40 */ "NODECURSOR",
    
    /* 41 */ "PARSER",
    /* 42 */ "PARTITION",
    /*    */ "PLAN",
    /*    */ "PLANLIST",
    /* 44 */ "PROXY",
    /* 45 */ "PROXYSET",
    
    /* 46 */ "QUEUE",
    /* 47 */ "QUEUEBUFFER",
  
    /*    */ "RBCURSOR",
    /* 48 */ "REGFRAME",
    /* 49 */ "RING",
    /* 50 */ "ROW",
    /* 51 */ "ROWBUFFER",
    /* 52 */ "ROWCURSOR",
    /* 53 */ "ROWSTRAND",
    /* 54 */ "ROWWSP",
  
    /* 55 */ "SCANBUFFER",
    /* 56 */ "SCRATCH",
    /* 57 */ "SEHISTORY",
    /* 58 */ "SEITF",
    /* 59 */ "SEITFCURSOR",
    /* 60 */ "SEITFLIST",
    /*    */ "SELCURSOR",
    /* 61 */ "SEPROMPT",
    /* 62 */ "SINCE",
    /* 63 */ "SKIPCURSOR",
    /* 64 */ "SKIPLIST",
    /* 65 */ "SKIPNODE",
    /*    */ "PARTITIONS",
    /* 66 */ "SMALLHEAP",
    /* 67 */ "SOCKET",
    /* 68 */ "STACK",
    /* 69 */ "STEPACCUM",
    /* 70 */ "STEPARRAY",
    /* 71 */ "SYSCALL",
  
    /* 72 */ "TABLE",
    /* 73 */ "TASK",
    /* 74 */ "TASKCURSOR",
    /* 75 */ "TASKLIST",
    /* 76 */ "TASKSET",
    /* 77 */ "THREAD",
    /* 78 */ "THREADCURSOR",
    /* 79 */ "THREADLIST",
    /* 80 */ "TIME",
    /* 81 */ "TIMER",
    /* 82 */ "TOKENISER",
    /* 83 */ "TX",
    /* 84 */ "TXARRAY",
    /* 85 */ "TXCOLLECTION",
    /* 86 */ "TXCURSOR",
    /* 87 */ "TXMAP",
    /* 88 */ "TXREF",

    /* 89 */ "WORD",
    
    /* 90 */ "ZSMALL_",
    /* 91 */ "ZMEDIUM_",
    /* 92 */ "ZLARGE_",
    /* 93 */ "ZJUMBO_",
  
    /* 94 */ "ENUMS"
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
