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
    /* 25 */ CN_LOCK,
    
    /* 26 */ CN_MEDIUMHEAP,
    
    /* 27 */ CN_NODE,
    /* 28 */ CN_NODECURSOR,
    
    /* 29 */ CN_PARSER,
    /* 30 */ CN_PARTITION,
    /* 31 */ CN_PARTITIONS,
    /* 32 */ CN_PROXY,
    /* 33 */ CN_PROXYSET,
    
    /* 34 */ CN_QUEUE,
    /* 35 */ CN_QUEUEBUFFER,
    
    /* 36 */ CN_RING,
    /* 37 */ CN_ROW,
    /* 38 */ CN_ROWBUFFER,
    /* 39 */ CN_ROWCURSOR,
    /* 40 */ CN_ROWSTRAND,
    
    /* 41 */ CN_SCANBUFFER,
    /* 42 */ CN_SEHISTORY,
    /* 43 */ CN_SEITF,
    /* 44 */ CN_SEITFCURSOR,
    /* 45 */ CN_SEITFLIST,
    /* 46 */ CN_SEPROMPT,
    /* 47 */ CN_SINCE,
    /* 48 */ CN_SKIPCURSOR,
    /* 49 */ CN_SKIPLIST,
    /* 50 */ CN_SKIPNODE,
    /* 51 */ CN_SMALLHEAP,
    /* 52 */ CN_SOCKET,
    /* 53 */ CN_STACK,
    /* 54 */ CN_STEPACCUM,
    /* 55 */ CN_STEPARRAY,
    /* 56 */ CN_SYSCALL,
    
    /* 57 */ CN_TABLE,
    /* 58 */ CN_TASK,
    /* 59 */ CN_TASKCURSOR,
    /* 60 */ CN_TASKLIST,
    /* 61 */ CN_TASKSET,
    /* 62 */ CN_THREAD,
    /* 63 */ CN_THREADCURSOR,
    /* 64 */ CN_THREADLIST,
    /* 65 */ CN_TIME,
    /* 66 */ CN_TIMER,
    /* 67 */ CN_TOKENISER,
    /* 68 */ CN_TX,
    /* 69 */ CN_TXARRAY,
    /* 70 */ CN_TXCOLLECTION,
    /* 71 */ CN_TXCURSOR,
    /* 72 */ CN_TXMAP,
    /* 73 */ CN_TXREF,
    
    /* 74 */ CN_WORD,
    
    /* 75 */ CN_ENUMS
  };
  
  static const char *pCN[]
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
    /* 25 */ "LOCK",
    
    /* 26 */ "MEDIUMHEAP",
    
    /* 27 */ "NODE",
    /* 28 */ "NODECURSOR",
    
    /* 29 */ "PARSER",
    /* 30 */ "PARTITION",
    /* 31 */ "PARTITIONS",
    /* 32 */ "PROXY",
    /* 33 */ "PROXYSET",
    
    /* 34 */ "QUEUE",
    /* 35 */ "QUEUEBUFFER",
    
    /* 36 */ "RING",
    /* 37 */ "ROW",
    /* 38 */ "ROWBUFFER",
    /* 39 */ "ROWCURSOR",
    /* 40 */ "ROWSTRAND",
    
    /* 41 */ "SCANBUFFER",
    /* 42 */ "SEHISTORY",
    /* 43 */ "SEITF",
    /* 44 */ "SEITFCURSOR",
    /* 45 */ "SEITFLIST",
    /* 46 */ "SEPROMPT",
    /* 47 */ "SINCE",
    /* 48 */ "SKIPCURSOR",
    /* 49 */ "SKIPLIST",
    /* 50 */ "SKIPNODE",
    /* 51 */ "SMALLHEAP",
    /* 52 */ "SOCKET",
    /* 53 */ "STACK",
    /* 54 */ "STEPACCUM",
    /* 55 */ "STEPARRAY",
    /* 56 */ "SYSCALL",
    
    /* 57 */ "TABLE",
    /* 58 */ "TASK",
    /* 59 */ "TASKCURSOR",
    /* 60 */ "TASKLIST",
    /* 61 */ "TASKSET",
    /* 62 */ "THREAD",
    /* 63 */ "THREADCURSOR",
    /* 64 */ "THREADLIST",
    /* 65 */ "TIME",
    /* 66 */ "TIMER",
    /* 67 */ "TOKENISER",
    /* 68 */ "TX",
    /* 69 */ "TXARRAY",
    /* 70 */ "TXCOLLECTION",
    /* 71 */ "TXCURSOR",
    /* 72 */ "TXMAP",
    /* 73 */ "TXREF",
    
    /* 74 */ "WORD",
    
    /* 75 */ "ENUMS"
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