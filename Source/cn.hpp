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
    
    /*  1 */ CN_INTVAL,
    /*  2 */ CN_OBJVAL,
    /*  3 */ CN_PTRVAL,
    /*  4 */ CN_STRVAL,
    /*  5 */ CN_VALSET,
    /*  6 */ CN_VALWSP,
    
    /*  7 */ CN_ARGUMENTS,
    /*  8 */ CN_ATOMIC,
    /*  9 */ CN_ATTACHMAP,
    /* 10 */ CN_BUFFER,
    /* 11 */ CN_BUFFERPOOL,
    /* 12 */ CN_BUFFERPOOLANALYZER,
    /* 13 */ CN_BUFFERPOOLITER,
    /* 14 */ CN_PARSER,
    /* 15 */ CN_DICTIONARY,
    /* 16 */ CN_DIR,
    /* 17 */ CN_FILE,
    /* 18 */ CN_FREECHUNK,
    /* 19 */ CN_GRID,
    /* 20 */ CN_HEAPBUFFER,
    /* 21 */ CN_INDEX,
    /* 22 */ CN_LOADER,
    /* 23 */ CN_LOCK,
    /* 24 */ CN_MEDIUMHEAP,
    /* 25 */ CN_NODE,
    /* 26 */ CN_NODECURSOR,
    /* 27 */ CN_PARTITION,
    /* 28 */ CN_PARTITIONS,
    /* 29 */ CN_PROXY,
    /* 30 */ CN_PROXYSET,
    /* 31 */ CN_QUEUE,
    /* 32 */ CN_QUEUEBUFFER,
    /* 33 */ CN_RING,
    /* 34 */ CN_ROW,
    /* 35 */ CN_ROWBUFFER,
    /* 36 */ CN_ROWCURSOR,
    /* 37 */ CN_ROWSTRAND,
    /* 38 */ CN_SCANBUFFER,
    /* 39 */ CN_SEHISTORY,
    /* 40 */ CN_SEITF,
    /* 41 */ CN_SEITFCURSOR,
    /* 42 */ CN_SEITFLIST,
    /* 43 */ CN_SEPROMPT,
    /* 44 */ CN_SINCE,
    /* 45 */ CN_SKIPCURSOR,
    /* 46 */ CN_SKIPLIST,
    /* 47 */ CN_SKIPNODE,
    /* 48 */ CN_SMALLHEAP,
    /* 49 */ CN_SOCKET,
    /* 50 */ CN_STACK,
    /* 51 */ CN_STEPACCUM,
    /* 52 */ CN_STEPARRAY,
    /* 53 */ CN_SYSCALL,
    /* 54 */ CN_TABLE,
    /* 55 */ CN_TASK,
    /* 56 */ CN_TASKCURSOR,
    /* 57 */ CN_TASKLIST,
    /* 58 */ CN_TASKSET,
    /* 59 */ CN_THREAD,
    /* 60 */ CN_THREADCURSOR,
    /* 61 */ CN_THREADLIST,
    /* 62 */ CN_TIME,
    /* 63 */ CN_TIMER,
    /* 64 */ CN_TOKENISER,
    /* 65 */ CN_TX,
    /* 66 */ CN_TXARRAY,
    /* 67 */ CN_TXCOLLECTION,
    /* 68 */ CN_TXCURSOR,
    /* 69 */ CN_TXMAP,
    /* 70 */ CN_TXREF,
    /* 71 */ CN_WORD,
    /* 72 */ CN_ENUMS
  };
  
  static const char *pCN[]
  {
    /*  0 */ "NONE",
    
    /*  1 */ "INTVAL",
    /*  2 */ "OBJVAL",
    /*  3 */ "PTRVAL",
    /*  4 */ "STRVAL",
    /*  5 */ "VALSET",
    /*  6 */ "VALWSP",
    
    /*  7 */ "ARGUMENTS",
    /*  8 */ "ATOMIC",
    /*  9 */ "ATTACHMAP",
    /* 10 */ "BUFFER",
    /* 11 */ "BUFFERPOOL",
    /* 12 */ "BUFFERPOOLANALYZER",
    /* 13 */ "BUFFERPOOLITER",
    /* 14 */ "PARSER",
    /* 15 */ "DICTIONARY",
    /* 16 */ "DIR",
    /* 17 */ "FILE",
    /* 18 */ "FREECHUNK",
    /* 19 */ "GRID",
    /* 20 */ "HEAPBUFFER",
    /* 21 */ "INDEX",
    /* 22 */ "LOADER",
    /* 23 */ "LOCK",
    /* 24 */ "MEDIUMHEAP",
    /* 25 */ "NODE",
    /* 26 */ "NODECURSOR",
    /* 27 */ "PARTITION",
    /* 28 */ "PARTITIONS",
    /* 29 */ "PROXY",
    /* 30 */ "PROXYSET",
    /* 31 */ "QUEUE",
    /* 32 */ "QUEUEBUFFER",
    /* 33 */ "RING",
    /* 34 */ "ROW",
    /* 35 */ "ROWBUFFER",
    /* 36 */ "ROWCURSOR",
    /* 37 */ "ROWSTRAND",
    /* 38 */ "SCANBUFFER",
    /* 39 */ "SEHISTORY",
    /* 40 */ "SEITF",
    /* 41 */ "SEITFCURSOR",
    /* 42 */ "SEITFLIST",
    /* 43 */ "SEPROMPT",
    /* 44 */ "SINCE",
    /* 45 */ "SKIPCURSOR",
    /* 46 */ "SKIPLIST",
    /* 47 */ "SKIPNODE",
    /* 48 */ "SMALLHEAP",
    /* 49 */ "SOCKET",
    /* 50 */ "STACK",
    /* 51 */ "STEPACCUM",
    /* 52 */ "STEPARRAY",
    /* 53 */ "SYSCALL",
    /* 54 */ "TABLE",
    /* 55 */ "TASK",
    /* 56 */ "TASKCURSOR",
    /* 57 */ "TASKLIST",
    /* 58 */ "TASKSET",
    /* 59 */ "THREAD",
    /* 60 */ "THREADCURSOR",
    /* 61 */ "THREADLIST",
    /* 62 */ "TIME",
    /* 63 */ "TIMER",
    /* 64 */ "TOKENISER",
    /* 65 */ "TX",
    /* 66 */ "TXARRAY",
    /* 67 */ "TXCOLLECTION",
    /* 68 */ "TXCURSOR",
    /* 69 */ "TXMAP",
    /* 70 */ "TXREF",
    /* 71 */ "WORD",
    /* 72 */ "ENUMS"
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