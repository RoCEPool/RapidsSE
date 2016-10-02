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
    /* 16 */ CN_BUFFERPOOLANALYZER,
    
    /* 17 */ CN_DICTIONARY,
    /* 18 */ CN_DICTIONARYSTACK,
    /* 19 */ CN_DIR,
    
    /* 20 */ CN_FILE,
    /* 21 */ CN_FREECHUNK,
    
    /*    */ CN_GRAPH,
    /*    */ CN_GRAPHNODE,
    /* 22 */ CN_GRID,
  
    /* 23 */ CN_HEAPBUFFER,
    
    /* 24 */ CN_INDEX,
    
    /* 25 */ CN_JUMBOHEAP,
    
    /* 26 */ CN_KEYCPP,
    /* 27 */ CN_KEYVAL,
    
    /*    */ CN_LOADCHUNK,
    /* 28 */ CN_LOADER,
    /* 29 */ CN_LOADERSUB,
    /* 30 */ CN_LOCK,
    
    /* 31 */ CN_MEDIUMHEAP,
    
    /* 32 */ CN_NODE,
    /* 33 */ CN_NODECURSOR,
    
    /* 34 */ CN_PARSER,
    /* 35 */ CN_PARTITION,
    /* 36 */ CN_PARTITIONS,
    /* 37 */ CN_PROXY,
    /* 38 */ CN_PROXYSET,

    /* 39 */ CN_QUEUE,
    /* 40 */ CN_QUEUEBUFFER,
    
    /* 41 */ CN_REGFRAME,
    /* 42 */ CN_RING,
    /* 43 */ CN_ROW,
    /* 44 */ CN_ROWBUFFER,
    /* 45 */ CN_ROWCURSOR,
    /* 46 */ CN_ROWSTRAND,
    /* 47 */ CN_ROWWSP,
    
    /* 48 */ CN_SCANBUFFER,
    /* 49 */ CN_LOADMAP,
    /* 50 */ CN_SEHISTORY,
    /* 51 */ CN_SEITF,
    /* 52 */ CN_SEITFCURSOR,
    /* 53 */ CN_SEITFLIST,
    /* 54 */ CN_SEPROMPT,
    /* 55 */ CN_SINCE,
    /* 56 */ CN_SKIPCURSOR,
    /* 57 */ CN_SKIPLIST,
    /* 58 */ CN_SKIPNODE,
    /* 59 */ CN_SMALLHEAP,
    /* 60 */ CN_SOCKET,
    /* 61 */ CN_STACK,
    /* 62 */ CN_STEPACCUM,
    /* 63 */ CN_STEPARRAY,
    /* 64 */ CN_SYSCALL,
    
    /* 65 */ CN_TABLE,
    /* 66 */ CN_TASK,
    /* 67 */ CN_TASKCURSOR,
    /* 68 */ CN_TASKLIST,
    /* 69 */ CN_TASKSET,
    /* 70 */ CN_THREAD,
    /* 71 */ CN_THREADCURSOR,
    /* 72 */ CN_THREADLIST,
    /* 73 */ CN_TIME,
    /* 74 */ CN_TIMER,
    /* 75 */ CN_TOKENISER,
    /* 76 */ CN_TX,
    /* 77 */ CN_TXARRAY,
    /* 78 */ CN_TXCOLLECTION,
    /* 79 */ CN_TXCURSOR,
    /* 80 */ CN_TXMAP,
    /* 81 */ CN_TXREF,
    
    /* 82 */ CN_WORD,
    
    /* 84 */ CN_ZSMALL_,
    /* 85 */ CN_ZMEDIUM_,
    /* 86 */ CN_ZLARGE_,
    /* 87 */ CN_ZJUMBO_,
  
    /* 88 */ CN_ENUMS
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
    /* 16 */ "BUFFERPOOLANALYZER",
    
    /* 17 */ "DICTIONARY",
    /* 18 */ "DICTIONARYSTACK",
    /* 19 */ "DIR",
    
    /* 20 */ "FILE",
    /* 21 */ "FREECHUNK",
    
    /*    */ "GRAPH",
    /*    */ "GRAPHNODE",
    /* 22 */ "GRID",
    
    /* 23 */ "HEAPBUFFER",
    
    /* 24 */ "INDEX",
    
    /* 25 */ "JUMBOHEAP",
    
    /* 26 */ "KEYCPP",
    /* 27 */ "KEYVAL",
    
    /*    */ "LOADCHUNK",
    /* 28 */ "LOADER",
    /* 29 */ "LOADERSUB",
    /* 30 */ "LOCK",
  
    /* 31 */ "MEDIUMHEAP",
    
    /* 32 */ "NODE",
    /* 33 */ "NODECURSOR",

    /* 34 */ "PARSER",
    /* 35 */ "PARTITION",
    /* 36 */ "PARTITIONS",
    /* 37 */ "PROXY",
    /* 38 */ "PROXYSET",
    
    /* 39 */ "QUEUE",
    /* 40 */ "QUEUEBUFFER",
  
    /* 41 */ "REGFRAME",
    /* 42 */ "RING",
    /* 43 */ "ROW",
    /* 44 */ "ROWBUFFER",
    /* 45 */ "ROWCURSOR",
    /* 46 */ "ROWSTRAND",
    /* 47 */ "ROWWSP",
  
    /* 48 */ "SCANBUFFER",
    /* 49 */ "SEEKMAP",
    /* 50 */ "SEHISTORY",
    /* 51 */ "SEITF",
    /* 52 */ "SEITFCURSOR",
    /* 53 */ "SEITFLIST",
    /* 54 */ "SEPROMPT",
    /* 55 */ "SINCE",
    /* 56 */ "SKIPCURSOR",
    /* 57 */ "SKIPLIST",
    /* 58 */ "SKIPNODE",
    /* 59 */ "SMALLHEAP",
    /* 60 */ "SOCKET",
    /* 61 */ "STACK",
    /* 62 */ "STEPACCUM",
    /* 63 */ "STEPARRAY",
    /* 64 */ "SYSCALL",
  
    /* 65 */ "TABLE",
    /* 66 */ "TASK",
    /* 67 */ "TASKCURSOR",
    /* 68 */ "TASKLIST",
    /* 69 */ "TASKSET",
    /* 70 */ "THREAD",
    /* 71 */ "THREADCURSOR",
    /* 73 */ "THREADLIST",
    /* 74 */ "TIME",
    /* 75 */ "TIMER",
    /* 76 */ "TOKENISER",
    /* 77 */ "TX",
    /* 78 */ "TXARRAY",
    /* 79 */ "TXCOLLECTION",
    /* 80 */ "TXCURSOR",
    /* 81 */ "TXMAP",
    /* 82 */ "TXREF",
    
    /* 83 */ "WORD",
    
    /* 84 */ "ZSMALL_",
    /* 85 */ "ZMEDIUM_",
    /* 86 */ "ZLARGE_",
    /* 87 */ "ZJUMBO_",
  
    /* 88 */ "ENUMS"
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