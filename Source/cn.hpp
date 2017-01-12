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
    /*    */ CN_S1M,
    
    /*  4 */ CN_INTVAL,
    /*  5 */ CN_IPVAL,
    /*  6 */ CN_OBJVAL,
    /*  7 */ CN_PTRVAL,
    /*  8 */ CN_STRVAL,
    /*  9 */ CN_VALREG,
    /* 10 */ CN_VALSET,
    /* 11 */ CN_VALWSP,
    
    /* 12 */ CN_ARGUMENTS,
    /* 13 */ CN_ATOMIC,
    /* 14 */ CN_ATTACHMAP,
    
    /* 15 */ CN_BUFFER,
    /* 16 */ CN_BUFFERPOOL,
    /* 17 */ CN_BPA,
  
    /* 18 */ CN_DIC,
    /* 19 */ CN_DICSTACK,
    /* 20 */ CN_DIR,
    
    /* 21 */ CN_FILE,
    /* 22 */ CN_FREECHUNK,
    
    /* 23 */ CN_GRAPH,
    /* 24 */ CN_GRAPHCURSOR,
    /* 25 */ CN_GRAPHNODE,
    /* 26 */ CN_GRID,
  
    /* 27 */ CN_HEAPBUFFER,
    
    /* 28 */ CN_INDEX,
    
    /* 29 */ CN_JUMBOHEAP,
    
    /* 30 */ CN_KEYCPP,
    /* 31 */ CN_KEYVAL,
    
    /* 32 */ CN_LOADCHUNK,
    /* 33 */ CN_LOADER,
    /* 34 */ CN_LOADERSUB,
    /* 35 */ CN_LOADMAP,
    /* 36 */ CN_LOCK,
    
    /* 37 */ CN_MEDIUMHEAP,

    /* 38 */ CN_NODE,
    /* 39 */ CN_NODECURSOR,
    
    /* 40 */ CN_PARSER,
    /* 41 */ CN_PARTITION,
    /* 42 */ CN_PARTITIONS,
    /* 43 */ CN_PROXY,
    /* 44 */ CN_PROXYSET,

    /* 45 */ CN_QUEUE,
    /* 46 */ CN_QUEUEBUFFER,
    
    /* 47 */ CN_REGFRAME,
    /* 48 */ CN_RING,
    /* 49 */ CN_ROW,
    /* 50 */ CN_ROWBUFFER,
    /* 51 */ CN_ROWCURSOR,
    /* 52 */ CN_ROWSTRAND,
    /* 53 */ CN_ROWWSP,
    
    /* 54 */ CN_SCANBUFFER,
    /* 55 */ CN_SCRATCH,
    /* 56 */ CN_SEHISTORY,
    /* 57 */ CN_SEITF,
    /* 58 */ CN_SEITFCURSOR,
    /* 59 */ CN_SEITFLIST,
    /* 60 */ CN_SEPROMPT,
    /* 61 */ CN_SINCE,
    /* 62 */ CN_SKIPCURSOR,
    /* 63 */ CN_SKIPLIST,
    /* 64 */ CN_SKIPNODE,
    /* 65 */ CN_SMALLHEAP,
    /* 66 */ CN_SOCKET,
    /* 67 */ CN_STACK,
    /* 68 */ CN_STEPACCUM,
    /* 69 */ CN_STEPARRAY,
    /* 70 */ CN_SYSCALL,
  
    /* 71 */ CN_TABLE,
    /* 72 */ CN_TASK,
    /* 73 */ CN_TASKCURSOR,
    /* 74 */ CN_TASKLIST,
    /* 75 */ CN_TASKSET,
    /* 76 */ CN_THREAD,
    /* 77 */ CN_THREADCURSOR,
    /* 78 */ CN_THREADLIST,
    /* 79 */ CN_TIME,
    /* 80 */ CN_TIMER,
    /* 81 */ CN_TOKENISER,
    /* 82 */ CN_TX,
    /* 83 */ CN_TXARRAY,
    /* 84 */ CN_TXCOLLECTION,
    /* 85 */ CN_TXCURSOR,
    /* 86 */ CN_TXMAP,
    /* 87 */ CN_TXREF,
  
    /* 88 */ CN_WORD,
    
    /* 89 */ CN_ZSMALL_,
    /* 90 */ CN_ZMEDIUM_,
    /* 91 */ CN_ZLARGE_,
    /* 92 */ CN_ZJUMBO_,
  
    /* 93 */ CN_ENUMS
  };
  
  static const char* const pCN[]
  {
    /*  0 */ "NONE",
    
    /*  1 */ "S32",
    /*  2 */ "S256",
    /*  3 */ "S8K",
    /*    */ "S1M",
    
    /*  4 */ "INTVAL",
    /*  5 */ "IPVAL",
    /*  6 */ "OBJVAL",
    /*  7 */ "PTRVAL",
    /*  8 */ "STRVAL",
    /*  9 */ "VALREG",
    /* 10 */ "VALSET",
    /* 11 */ "VALWSP",
    
    /* 12 */ "ARGUMENTS",
    /* 13 */ "ATOMIC",
    /* 14 */ "ATTACHMAP",
    
    /* 15 */ "BUFFER",
    /* 16 */ "BUFFERPOOL",
    /* 17 */ "BPA",
    
    /* 18 */ "DIC",
    /* 19 */ "DICSTACK",
    /* 20 */ "DIR",
    
    /* 21 */ "FILE",
    /* 22 */ "FREECHUNK",
    
    /* 23 */ "GRAPH",
    /* 24 */ "GRAPHCURSOR",
    /* 25 */ "GRAPHNODE",
    /* 26 */ "GRID",
    
    /* 27 */ "HEAPBUFFER",
    
    /* 28 */ "INDEX",
    
    /* 29 */ "JUMBOHEAP",
    
    /* 30 */ "KEYCPP",
    /* 31 */ "KEYVAL",
    
    /* 32 */ "LOADCHUNK",
    /* 33 */ "LOADER",
    /* 34 */ "LOADERSUB",
    /* 35 */ "LOADMAP",
    /* 36 */ "LOCK",
  
    /* 37 */ "MEDIUMHEAP",
    
    /* 38 */ "NODE",
    /* 39 */ "NODECURSOR",
    
    /* 40 */ "PARSER",
    /* 41 */ "PARTITION",
    /* 42 */ "PARTITIONS",
    /* 43 */ "PROXY",
    /* 44 */ "PROXYSET",
    
    /* 45 */ "QUEUE",
    /* 46 */ "QUEUEBUFFER",
  
    /* 47 */ "REGFRAME",
    /* 48 */ "RING",
    /* 49 */ "ROW",
    /* 50 */ "ROWBUFFER",
    /* 51 */ "ROWCURSOR",
    /* 52 */ "ROWSTRAND",
    /* 53 */ "ROWWSP",
  
    /* 54 */ "SCANBUFFER",
    /* 55 */ "SCRATCH",
    /* 56 */ "SEHISTORY",
    /* 57 */ "SEITF",
    /* 58 */ "SEITFCURSOR",
    /* 59 */ "SEITFLIST",
    /* 60 */ "SEPROMPT",
    /* 61 */ "SINCE",
    /* 62 */ "SKIPCURSOR",
    /* 63 */ "SKIPLIST",
    /* 64 */ "SKIPNODE",
    /* 65 */ "SMALLHEAP",
    /* 66 */ "SOCKET",
    /* 67 */ "STACK",
    /* 68 */ "STEPACCUM",
    /* 69 */ "STEPARRAY",
    /* 70 */ "SYSCALL",
  
    /* 71 */ "TABLE",
    /* 72 */ "TASK",
    /* 73 */ "TASKCURSOR",
    /* 74 */ "TASKLIST",
    /* 75 */ "TASKSET",
    /* 76 */ "THREAD",
    /* 77 */ "THREADCURSOR",
    /* 78 */ "THREADLIST",
    /* 79 */ "TIME",
    /* 80 */ "TIMER",
    /* 81 */ "TOKENISER",
    /* 82 */ "TX",
    /* 83 */ "TXARRAY",
    /* 84 */ "TXCOLLECTION",
    /* 85 */ "TXCURSOR",
    /* 86 */ "TXMAP",
    /* 87 */ "TXREF",

    /* 88 */ "WORD",
    
    /* 89 */ "ZSMALL_",
    /* 90 */ "ZMEDIUM_",
    /* 91 */ "ZLARGE_",
    /* 92 */ "ZJUMBO_",
  
    /* 93 */ "ENUMS"
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
