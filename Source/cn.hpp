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
    /*  3 */ CN_S8K,
    
    /*  4 */ CN_INTVAL,
    /*  5 */ CN_OBJVAL,
    /*  6 */ CN_PTRVAL,
    /*  7 */ CN_STRVAL,
    /*  8 */ CN_VALREG,
    /*  9 */ CN_VALSET,
    /* 10 */ CN_VALWSP,
    
    /* 11 */ CN_ARGUMENTS,
    /* 12 */ CN_ATOMIC,
    /* 13 */ CN_ATTACHMAP,
    
    /* 14 */ CN_BUFFER,
    /* 15 */ CN_BUFFERPOOL,
    /* 16 */ CN_BPA,
    
    /* 17 */ CN_DIC,
    /* 18 */ CN_DICSTACK,
    /* 19 */ CN_DIR,
    
    /* 20 */ CN_FILE,
    /* 21 */ CN_FREECHUNK,
    
    /* 22 */ CN_GRAPH,
    /* 23 */ CN_GRAPHCURSOR,
    /* 24 */ CN_GRAPHNODE,
    /* 25 */ CN_GRID,
  
    /* 26 */ CN_HEAPBUFFER,
    
    /* 27 */ CN_INDEX,
    
    /* 28 */ CN_JUMBOHEAP,
    
    /* 29 */ CN_KEYCPP,
    /* 30 */ CN_KEYVAL,
    
    /* 31 */ CN_LOADCHUNK,
    /* 32 */ CN_LOADER,
    /* 33 */ CN_LOADERSUB,
    /* 34 */ CN_LOCK,
    
    /* 35 */ CN_MEDIUMHEAP,

    /* 36 */ CN_NODE,
    /* 37 */ CN_NODECURSOR,
    
    /* 38 */ CN_PARSER,
    /* 39 */ CN_PARTITION,
    /* 40 */ CN_PARTITIONS,
    /* 41 */ CN_PROXY,
    /* 42 */ CN_PROXYSET,

    /* 43 */ CN_QUEUE,
    /* 44 */ CN_QUEUEBUFFER,
    
    /* 45 */ CN_REGFRAME,
    /* 46 */ CN_RING,
    /* 47 */ CN_ROW,
    /* 48 */ CN_ROWBUFFER,
    /* 49 */ CN_ROWCURSOR,
    /* 50 */ CN_ROWSTRAND,
    /* 51 */ CN_ROWWSP,
    
    /* 52 */ CN_SCANBUFFER,
    /* 53 */ CN_SCRATCH,
    /* 54 */ CN_LOADMAP,
    /* 55 */ CN_SEHISTORY,
    /* 56 */ CN_SEITF,
    /* 57 */ CN_SEITFCURSOR,
    /* 58 */ CN_SEITFLIST,
    /* 59 */ CN_SEPROMPT,
    /* 60 */ CN_SINCE,
    /* 61 */ CN_SKIPCURSOR,
    /* 62 */ CN_SKIPLIST,
    /* 63 */ CN_SKIPNODE,
    /* 64 */ CN_SMALLHEAP,
    /* 65 */ CN_SOCKET,
    /* 66 */ CN_STACK,
    /* 67 */ CN_STEPACCUM,
    /* 68 */ CN_STEPARRAY,
    /* 69 */ CN_SYSCALL,
  
    /* 70 */ CN_TABLE,
    /* 71 */ CN_TASK,
    /* 72 */ CN_TASKCURSOR,
    /* 73 */ CN_TASKLIST,
    /* 74 */ CN_TASKSET,
    /* 75 */ CN_THREAD,
    /* 76 */ CN_THREADCURSOR,
    /* 77 */ CN_THREADLIST,
    /* 78 */ CN_TIME,
    /* 79 */ CN_TIMER,
    /* 80 */ CN_TOKENISER,
    /* 81 */ CN_TX,
    /* 82 */ CN_TXARRAY,
    /* 83 */ CN_TXCOLLECTION,
    /* 84 */ CN_TXCURSOR,
    /* 85 */ CN_TXMAP,
    /* 86 */ CN_TXREF,
  
    /* 87 */ CN_WORD,
    
    /* 88 */ CN_ZSMALL_,
    /* 89 */ CN_ZMEDIUM_,
    /* 90 */ CN_ZLARGE_,
    /* 91 */ CN_ZJUMBO_,
  
    /* 92 */ CN_ENUMS
  };
  
  static const char* const pCN[]
  {
    /*  0 */ "NONE",
    
    /*  1 */ "S32",
    /*  2 */ "S256",
    /*  3 */ "S8K",
    /*  4 */ "INTVAL",
    /*  5 */ "OBJVAL",
    /*  6 */ "PTRVAL",
    /*  7 */ "STRVAL",
    /*  8 */ "VALREG",
    /*  9 */ "VALSET",
    /* 10 */ "VALWSP",
    
    /* 11 */ "ARGUMENTS",
    /* 12 */ "ATOMIC",
    /* 13 */ "ATTACHMAP",
    
    /* 14 */ "BUFFER",
    /* 15 */ "BUFFERPOOL",
    /* 16 */ "BPA",
    
    /* 17 */ "DIC",
    /* 18 */ "DICSTACK",
    /* 19 */ "DIR",
    
    /* 20 */ "FILE",
    /* 21 */ "FREECHUNK",
    
    /* 22 */ "GRAPH",
    /* 23 */ "GRAPHCURSOR",
    /* 24 */ "GRAPHNODE",
    /* 25 */ "GRID",
    
    /* 26 */ "HEAPBUFFER",
    
    /* 27 */ "INDEX",
    
    /* 28 */ "JUMBOHEAP",
    
    /* 29 */ "KEYCPP",
    /* 30 */ "KEYVAL",
    
    /* 31 */ "LOADCHUNK",
    /* 32 */ "LOADER",
    /* 33 */ "LOADERSUB",
    /* 34 */ "LOCK",
  
    /* 35 */ "MEDIUMHEAP",
    
    /* 36 */ "NODE",
    /* 37 */ "NODECURSOR",
    
    /* 38 */ "PARSER",
    /* 39 */ "PARTITION",
    /* 40 */ "PARTITIONS",
    /* 41 */ "PROXY",
    /* 42 */ "PROXYSET",
    
    /* 43 */ "QUEUE",
    /* 44 */ "QUEUEBUFFER",
  
    /* 45 */ "REGFRAME",
    /* 46 */ "RING",
    /* 47 */ "ROW",
    /* 48 */ "ROWBUFFER",
    /* 49 */ "ROWCURSOR",
    /* 50 */ "ROWSTRAND",
    /* 51 */ "ROWWSP",
  
    /* 52 */ "SCANBUFFER",
    /* 53 */ "SCRATCH",
    /* 54 */ "LOADMAP",
    /* 55 */ "SEHISTORY",
    /* 56 */ "SEITF",
    /* 57 */ "SEITFCURSOR",
    /* 58 */ "SEITFLIST",
    /* 59 */ "SEPROMPT",
    /* 60 */ "SINCE",
    /* 61 */ "SKIPCURSOR",
    /* 62 */ "SKIPLIST",
    /* 63 */ "SKIPNODE",
    /* 64 */ "SMALLHEAP",
    /* 65 */ "SOCKET",
    /* 66 */ "STACK",
    /* 67 */ "STEPACCUM",
    /* 68 */ "STEPARRAY",
    /* 69 */ "SYSCALL",
  
    /* 70 */ "TABLE",
    /* 71 */ "TASK",
    /* 72 */ "TASKCURSOR",
    /* 73 */ "TASKLIST",
    /* 74 */ "TASKSET",
    /* 75 */ "THREAD",
    /* 76 */ "THREADCURSOR",
    /* 77 */ "THREADLIST",
    /* 78 */ "TIME",
    /* 79 */ "TIMER",
    /* 80 */ "TOKENISER",
    /* 81 */ "TX",
    /* 82 */ "TXARRAY",
    /* 83 */ "TXCOLLECTION",
    /* 84 */ "TXCURSOR",
    /* 85 */ "TXMAP",
    /* 86 */ "TXREF",

    /* 87 */ "WORD",
    
    /* 88 */ "ZSMALL_",
    /* 89 */ "ZMEDIUM_",
    /* 90 */ "ZLARGE_",
    /* 91 */ "ZJUMBO_",
  
    /* 92 */ "ENUMS"
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
