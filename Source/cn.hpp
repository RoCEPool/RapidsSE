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
    
    /* 24 */ CN_LOADER,
    /* 25 */ CN_LOADERSUB,
    /* 26 */ CN_LOCK,
    
    /* 27 */ CN_MEDIUMHEAP,
    
    /* 28 */ CN_NODE,
    /* 29 */ CN_NODECURSOR,
    
    /* 30 */ CN_PARSER,
    /* 31 */ CN_PARTITION,
    /* 32 */ CN_PARTITIONS,
    /* 33 */ CN_PROXY,
    /* 34 */ CN_PROXYSET,
    
    /* 35 */ CN_QUEUE,
    /* 36 */ CN_QUEUEBUFFER,
    
    /* 37 */ CN_RING,
    /* 38 */ CN_ROW,
    /* 39 */ CN_ROWBUFFER,
    /* 40 */ CN_ROWCURSOR,
    /* 41 */ CN_ROWSTRAND,
    /* 42 */ CN_ROWWSP,
    
    /* 43 */ CN_SCANBUFFER,
    /* 44 */ CN_SEHISTORY,
    /* 45 */ CN_SEITF,
    /* 46 */ CN_SEITFCURSOR,
    /* 47 */ CN_SEITFLIST,
    /* 48 */ CN_SEPROMPT,
    /* 49 */ CN_SINCE,
    /* 50 */ CN_SKIPCURSOR,
    /* 51 */ CN_SKIPLIST,
    /* 52 */ CN_SKIPNODE,
    /* 53 */ CN_SMALLHEAP,
    /* 54 */ CN_SOCKET,
    /* 55 */ CN_STACK,
    /* 56 */ CN_STEPACCUM,
    /* 57 */ CN_STEPARRAY,
    /* 58 */ CN_SYSCALL,
    
    /* 59 */ CN_TABLE,
    /* 60 */ CN_TASK,
    /* 61 */ CN_TASKCURSOR,
    /* 62 */ CN_TASKLIST,
    /* 63 */ CN_TASKSET,
    /* 64 */ CN_THREAD,
    /* 65 */ CN_THREADCURSOR,
    /* 66 */ CN_THREADLIST,
    /* 67 */ CN_TIME,
    /* 68 */ CN_TIMER,
    /* 69 */ CN_TOKENISER,
    /* 70 */ CN_TX,
    /* 71 */ CN_TXARRAY,
    /* 72 */ CN_TXCOLLECTION,
    /* 73 */ CN_TXCURSOR,
    /* 74 */ CN_TXMAP,
    /* 75 */ CN_TXREF,
  
    /* 76 */ CN_WORD,
    
    /* 77 */ CN_ENUMS
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
    
    /* 24 */ "LOADER",
    /* 25 */ "LOADERSUB",
    /* 26 */ "LOCK",
    
    /* 27 */ "MEDIUMHEAP",
    
    /* 28 */ "NODE",
    /* 29 */ "NODECURSOR",
    
    /* 30 */ "PARSER",
    /* 31 */ "PARTITION",
    /* 32 */ "PARTITIONS",
    /* 33 */ "PROXY",
    /* 34 */ "PROXYSET",
    
    /* 35 */ "QUEUE",
    /* 36 */ "QUEUEBUFFER",
  
    /* 37 */ "RING",
    /* 38 */ "ROW",
    /* 39 */ "ROWBUFFER",
    /* 40 */ "ROWCURSOR",
    /* 41 */ "ROWSTRAND",
    /* 42 */ "ROWWSP",
    
    /* 43 */ "SCANBUFFER",
    /* 44 */ "SEHISTORY",
    /* 45 */ "SEITF",
    /* 46 */ "SEITFCURSOR",
    /* 47 */ "SEITFLIST",
    /* 48 */ "SEPROMPT",
    /* 49 */ "SINCE",
    /* 50 */ "SKIPCURSOR",
    /* 51 */ "SKIPLIST",
    /* 52 */ "SKIPNODE",
    /* 53 */ "SMALLHEAP",
    /* 54 */ "SOCKET",
    /* 55 */ "STACK",
    /* 56 */ "STEPACCUM",
    /* 57 */ "STEPARRAY",
    /* 58 */ "SYSCALL",
  
    /* 59 */ "TABLE",
    /* 60 */ "TASK",
    /* 61 */ "TASKCURSOR",
    /* 62 */ "TASKLIST",
    /* 63 */ "TASKSET",
    /* 64 */ "THREAD",
    /* 65 */ "THREADCURSOR",
    /* 66 */ "THREADLIST",
    /* 67 */ "TIME",
    /* 68 */ "TIMER",
    /* 69 */ "TOKENISER",
    /* 70 */ "TX",
    /* 71 */ "TXARRAY",
    /* 72 */ "TXCOLLECTION",
    /* 73 */ "TXCURSOR",
    /* 74 */ "TXMAP",
    /* 75 */ "TXREF",
  
    /* 76 */ "WORD",
  
    /* 77 */ "ENUMS"
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