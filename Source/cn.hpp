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
    
    /* 22 */ CN_GRID,
  
    /* 23 */ CN_HEAPBUFFER,
    
    /* 24 */ CN_INDEX,
    
    /* 25 */ CN_JUMBOHEAP,
    
    /* 26 */ CN_LOADER,
    /* 27 */ CN_LOADERSUB,
    /* 28 */ CN_LOCK,
    
    /* 29 */ CN_MEDIUMHEAP,
    
    /* 30 */ CN_NODE,
    /* 31 */ CN_NODECURSOR,
    
    /* 32 */ CN_PARSER,
    /* 33 */ CN_PARTITION,
    /* 34 */ CN_PARTITIONS,
    /* 35 */ CN_PROXY,
    /* 36 */ CN_PROXYSET,

    /* 37 */ CN_QUEUE,
    /* 38 */ CN_QUEUEBUFFER,
    
    /*    */ CN_REGFRAME,
    /* 39 */ CN_RING,
    /* 40 */ CN_ROW,
    /* 41 */ CN_ROWBUFFER,
    /* 42 */ CN_ROWCURSOR,
    /* 43 */ CN_ROWSTRAND,
    /* 44 */ CN_ROWWSP,
    
    /* 45 */ CN_SCANBUFFER,
    /* 46 */ CN_SEHISTORY,
    /* 47 */ CN_SEITF,
    /* 48 */ CN_SEITFCURSOR,
    /* 49 */ CN_SEITFLIST,
    /* 50 */ CN_SEPROMPT,
    /* 51 */ CN_SINCE,
    /* 52 */ CN_SKIPCURSOR,
    /* 53 */ CN_SKIPLIST,
    /* 54 */ CN_SKIPNODE,
    /* 55 */ CN_SMALLHEAP,
    /* 56 */ CN_SOCKET,
    /* 57 */ CN_STACK,
    /* 58 */ CN_STEPACCUM,
    /* 59 */ CN_STEPARRAY,
    /* 60 */ CN_SYSCALL,
    
    /* 61 */ CN_TABLE,
    /* 62 */ CN_TASK,
    /* 63 */ CN_TASKCURSOR,
    /* 64 */ CN_TASKLIST,
    /* 65 */ CN_TASKSET,
    /* 66 */ CN_THREAD,
    /* 67 */ CN_THREADCURSOR,
    /* 68 */ CN_THREADLIST,
    /* 69 */ CN_TIME,
    /* 70 */ CN_TIMER,
    /* 71 */ CN_TOKENISER,
    /* 72 */ CN_TX,
    /* 73 */ CN_TXARRAY,
    /* 74 */ CN_TXCOLLECTION,
    /* 75 */ CN_TXCURSOR,
    /* 76 */ CN_TXMAP,
    /* 77 */ CN_TXREF,
    
    /* 78 */ CN_WORD,
  
    /* 79 */ CN_ENUMS
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
    
    /* 22 */ "GRID",
    
    /* 23 */ "HEAPBUFFER",
    
    /* 24 */ "INDEX",
    
    /* 25 */ "JUMBOHEAP",
    
    /* 26 */ "LOADER",
    /* 27 */ "LOADERSUB",
    /* 28 */ "LOCK",
    
    /* 29 */ "MEDIUMHEAP",
    
    /* 30 */ "NODE",
    /* 31 */ "NODECURSOR",

    /* 32 */ "PARSER",
    /* 33 */ "PARTITION",
    /* 34 */ "PARTITIONS",
    /* 35 */ "PROXY",
    /* 36 */ "PROXYSET",

    /* 37 */ "QUEUE",
    /* 38 */ "QUEUEBUFFER",
  
    /*    */ "REGFRAME",
    /* 39 */ "RING",
    /* 40 */ "ROW",
    /* 41 */ "ROWBUFFER",
    /* 42 */ "ROWCURSOR",
    /* 43 */ "ROWSTRAND",
    /* 44 */ "ROWWSP",
    
    /* 45 */ "SCANBUFFER",
    /* 46 */ "SEHISTORY",
    /* 47 */ "SEITF",
    /* 48 */ "SEITFCURSOR",
    /* 49 */ "SEITFLIST",
    /* 50 */ "SEPROMPT",
    /* 51 */ "SINCE",
    /* 52 */ "SKIPCURSOR",
    /* 53 */ "SKIPLIST",
    /* 54 */ "SKIPNODE",
    /* 55 */ "SMALLHEAP",
    /* 56 */ "SOCKET",
    /* 57 */ "STACK",
    /* 58 */ "STEPACCUM",
    /* 59 */ "STEPARRAY",
    /* 60 */ "SYSCALL",
  
    /* 61 */ "TABLE",
    /* 62 */ "TASK",
    /* 63 */ "TASKCURSOR",
    /* 64 */ "TASKLIST",
    /* 65 */ "TASKSET",
    /* 66 */ "THREAD",
    /* 67 */ "THREADCURSOR",
    /* 68 */ "THREADLIST",
    /* 69 */ "TIME",
    /* 70 */ "TIMER",
    /* 71 */ "TOKENISER",
    /* 72 */ "TX",
    /* 73 */ "TXARRAY",
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