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
    
    /* 17 */ CN_DICTIONARY,
    /* 18 */ CN_DICTIONARYSTACK,
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
    /* 53 */ CN_LOADMAP,
    /* 54 */ CN_SEHISTORY,
    /* 55 */ CN_SEITF,
    /* 56 */ CN_SEITFCURSOR,
    /* 57 */ CN_SEITFLIST,
    /* 58 */ CN_SEPROMPT,
    /* 59 */ CN_SINCE,
    /* 60 */ CN_SKIPCURSOR,
    /* 61 */ CN_SKIPLIST,
    /* 62 */ CN_SKIPNODE,
    /* 63 */ CN_SMALLHEAP,
    /* 64 */ CN_SOCKET,
    /* 65 */ CN_STACK,
    /* 66 */ CN_STEPACCUM,
    /* 67 */ CN_STEPARRAY,
    /* 68 */ CN_SYSCALL,
  
    /* 69 */ CN_TABLE,
    /* 70 */ CN_TASK,
    /* 71 */ CN_TASKCURSOR,
    /* 72 */ CN_TASKLIST,
    /* 73 */ CN_TASKSET,
    /* 74 */ CN_THREAD,
    /* 75 */ CN_THREADCURSOR,
    /* 76 */ CN_THREADLIST,
    /* 77 */ CN_TIME,
    /* 78 */ CN_TIMER,
    /* 79 */ CN_TOKENISER,
    /* 80 */ CN_TX,
    /* 81 */ CN_TXARRAY,
    /* 82 */ CN_TXCOLLECTION,
    /* 83 */ CN_TXCURSOR,
    /* 84 */ CN_TXMAP,
    /* 85 */ CN_TXREF,
    
    /* 86 */ CN_WORD,
    
    /* 87 */ CN_ZSMALL_,
    /* 88 */ CN_ZMEDIUM_,
    /* 89 */ CN_ZLARGE_,
    /* 90 */ CN_ZJUMBO_,
  
    /* 91 */ CN_ENUMS
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
    /* 53 */ "SEEKMAP",
    /* 54 */ "SEHISTORY",
    /* 55 */ "SEITF",
    /* 56 */ "SEITFCURSOR",
    /* 57 */ "SEITFLIST",
    /* 58 */ "SEPROMPT",
    /* 59 */ "SINCE",
    /* 60 */ "SKIPCURSOR",
    /* 61 */ "SKIPLIST",
    /* 62 */ "SKIPNODE",
    /* 63 */ "SMALLHEAP",
    /* 64 */ "SOCKET",
    /* 65 */ "STACK",
    /* 66 */ "STEPACCUM",
    /* 67 */ "STEPARRAY",
    /* 68 */ "SYSCALL",
  
    /* 69 */ "TABLE",
    /* 70 */ "TASK",
    /* 71 */ "TASKCURSOR",
    /* 72 */ "TASKLIST",
    /* 73 */ "TASKSET",
    /* 74 */ "THREAD",
    /* 75 */ "THREADCURSOR",
    /* 76 */ "THREADLIST",
    /* 77 */ "TIME",
    /* 78 */ "TIMER",
    /* 79 */ "TOKENISER",
    /* 80 */ "TX",
    /* 81 */ "TXARRAY",
    /* 82 */ "TXCOLLECTION",
    /* 83 */ "TXCURSOR",
    /* 84 */ "TXMAP",
    /* 85 */ "TXREF",

    /* 86 */ "WORD",
    
    /* 87 */ "ZSMALL_",
    /* 88 */ "ZMEDIUM_",
    /* 89 */ "ZLARGE_",
    /* 90 */ "ZJUMBO_",
  
    /* 91 */ "ENUMS"
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