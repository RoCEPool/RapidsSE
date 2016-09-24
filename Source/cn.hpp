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
    /*  8 */ CN_VALSET,
    /*  9 */ CN_VALWSP,
    
    /* 10 */ CN_ARGUMENTS,
    /* 11 */ CN_ATOMIC,
    /* 12 */ CN_ATTACHMAP,
    
    /* 13 */ CN_BUFFER,
    /* 14 */ CN_BUFFERPOOL,
    /* 15 */ CN_BUFFERPOOLANALYZER,
    
    /* 16 */ CN_DICTIONARY,
    /* 17 */ CN_DICTIONARYSTACK,
    /* 18 */ CN_DIR,
    
    /* 19 */ CN_FILE,
    /* 20 */ CN_FREECHUNK,
    
    /* 21 */ CN_GRID,
    
    /* 22 */ CN_HEAPBUFFER,
    
    /* 23 */ CN_INDEX,
    
    /* 24 */ CN_LARGEHEAP,
    
    /* 25 */ CN_LOADER,
    /* 26 */ CN_LOADERSUB,
    /* 27 */ CN_LOCK,
    
    /* 28 */ CN_MEDIUMHEAP,
    
    /* 29 */ CN_NODE,
    /* 30 */ CN_NODECURSOR,
    
    /* 31 */ CN_PARSER,
    /* 32 */ CN_PARTITION,
    /* 33 */ CN_PARTITIONS,
    /* 34 */ CN_PROXY,
    /* 35 */ CN_PROXYSET,
    
    /* 36 */ CN_QUEUE,
    /* 37 */ CN_QUEUEBUFFER,
    
    /* 38 */ CN_RING,
    /* 39 */ CN_ROW,
    /* 40 */ CN_ROWBUFFER,
    /* 41 */ CN_ROWCURSOR,
    /* 42 */ CN_ROWSTRAND,
    /* 43 */ CN_ROWWSP,
    
    /* 44 */ CN_SCANBUFFER,
    /* 45 */ CN_SEHISTORY,
    /* 46 */ CN_SEITF,
    /* 47 */ CN_SEITFCURSOR,
    /* 48 */ CN_SEITFLIST,
    /* 49 */ CN_SEPROMPT,
    /* 50 */ CN_SINCE,
    /* 51 */ CN_SKIPCURSOR,
    /* 52 */ CN_SKIPLIST,
    /* 53 */ CN_SKIPNODE,
    /* 54 */ CN_SMALLHEAP,
    /* 55 */ CN_SOCKET,
    /* 56 */ CN_STACK,
    /* 57 */ CN_STEPACCUM,
    /* 58 */ CN_STEPARRAY,
    /* 59 */ CN_SYSCALL,
    
    /* 60 */ CN_TABLE,
    /* 61 */ CN_TASK,
    /* 62 */ CN_TASKCURSOR,
    /* 63 */ CN_TASKLIST,
    /* 64 */ CN_TASKSET,
    /* 65 */ CN_THREAD,
    /* 66 */ CN_THREADCURSOR,
    /* 67 */ CN_THREADLIST,
    /* 68 */ CN_TIME,
    /* 69 */ CN_TIMER,
    /* 70 */ CN_TOKENISER,
    /* 71 */ CN_TX,
    /* 72 */ CN_TXARRAY,
    /* 73 */ CN_TXCOLLECTION,
    /* 74 */ CN_TXCURSOR,
    /* 75 */ CN_TXMAP,
    /* 76 */ CN_TXREF,

    /* 77 */ CN_WORD,
  
    /* 78 */ CN_ENUMS
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
    /*  8 */ "VALSET",
    /*  9 */ "VALWSP",
    
    /* 10 */ "ARGUMENTS",
    /* 11 */ "ATOMIC",
    /* 12 */ "ATTACHMAP",
    
    /* 13 */ "BUFFER",
    /* 14 */ "BUFFERPOOL",
    /* 15 */ "BUFFERPOOLANALYZER",
    
    /* 16 */ "DICTIONARY",
    /* 17 */ "DICTIONARYSTACK",
    /* 18 */ "DIR",
    
    /* 19 */ "FILE",
    /* 20 */ "FREECHUNK",
    
    /* 21 */ "GRID",
    
    /* 22 */ "HEAPBUFFER",
    
    /* 23 */ "INDEX",
    
    /* 24 */ "LARGEHEAP",
    /* 25 */ "LOADER",
    /* 26 */ "LOADERSUB",
    /* 27 */ "LOCK",
    
    /* 28 */ "MEDIUMHEAP",
    
    /* 29 */ "NODE",
    /* 30 */ "NODECURSOR",
    
    /* 31 */ "PARSER",
    /* 32 */ "PARTITION",
    /* 33 */ "PARTITIONS",
    /* 34 */ "PROXY",
    /* 35 */ "PROXYSET",
    
    /* 36 */ "QUEUE",
    /* 37 */ "QUEUEBUFFER",
  
    /* 38 */ "RING",
    /* 39 */ "ROW",
    /* 40 */ "ROWBUFFER",
    /* 41 */ "ROWCURSOR",
    /* 42 */ "ROWSTRAND",
    /* 43 */ "ROWWSP",
    
    /* 44 */ "SCANBUFFER",
    /* 45 */ "SEHISTORY",
    /* 46 */ "SEITF",
    /* 47 */ "SEITFCURSOR",
    /* 48 */ "SEITFLIST",
    /* 49 */ "SEPROMPT",
    /* 50 */ "SINCE",
    /* 51 */ "SKIPCURSOR",
    /* 52 */ "SKIPLIST",
    /* 53 */ "SKIPNODE",
    /* 54 */ "SMALLHEAP",
    /* 55 */ "SOCKET",
    /* 56 */ "STACK",
    /* 57 */ "STEPACCUM",
    /* 58 */ "STEPARRAY",
    /* 59 */ "SYSCALL",
  
    /* 60 */ "TABLE",
    /* 61 */ "TASK",
    /* 62 */ "TASKCURSOR",
    /* 63 */ "TASKLIST",
    /* 64 */ "TASKSET",
    /* 65 */ "THREAD",
    /* 66 */ "THREADCURSOR",
    /* 67 */ "THREADLIST",
    /* 68 */ "TIME",
    /* 69 */ "TIMER",
    /* 70 */ "TOKENISER",
    /* 71 */ "TX",
    /* 72 */ "TXARRAY",
    /* 74 */ "TXCOLLECTION",
    /* 75 */ "TXCURSOR",
    /* 76 */ "TXMAP",
    /* 77 */ "TXREF",
  
    /* 78 */ "WORD",
  
    /* 79 */ "ENUMS"
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