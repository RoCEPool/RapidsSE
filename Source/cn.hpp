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
    /* 28 */ CN_PROXY,
    /* 29 */ CN_PROXYSET,
    /* 30 */ CN_QUEUE,
    /* 31 */ CN_QUEUEBUFFER,
    /* 32 */ CN_ROW,
    /* 33 */ CN_ROWBUFFER,
    /* 34 */ CN_ROWCURSOR,
    /* 35 */ CN_ROWSTRAND,
    /* 36 */ CN_SCANBUFFER,
    /* 37 */ CN_SEHISTORY,
    /* 38 */ CN_SEITF,
    /* 39 */ CN_SEITFCURSOR,
    /* 40 */ CN_SEITFLIST,
    /* 41 */ CN_SEPROMPT,
    /* 42 */ CN_SINCE,
    /* 43 */ CN_SKIPCURSOR,
    /* 44 */ CN_SKIPLIST,
    /* 45 */ CN_SKIPNODE,
    /* 46 */ CN_SMALLHEAP,
    /* 47 */ CN_SOCKET,
    /* 48 */ CN_STACK,
    /* 49 */ CN_STEPACCUM,
    /* 50 */ CN_STEPARRAY,
    /* 51 */ CN_SYSCALL,
    /* 52 */ CN_TABLE,
    /* 53 */ CN_TASK,
    /* 54 */ CN_TASKCURSOR,
    /* 55 */ CN_TASKLIST,
    /* 56 */ CN_TASKSET,
    /* 57 */ CN_THREAD,
    /* 58 */ CN_THREADCURSOR,
    /* 59 */ CN_THREADLIST,
    /* 60 */ CN_TIME,
    /* 61 */ CN_TIMER,
    /* 62 */ CN_TOKENISER,
    /* 63 */ CN_TX,
    /* 64 */ CN_TXARRAY,
    /* 65 */ CN_TXCOLLECTION,
    /* 66 */ CN_TXCURSOR,
    /* 67 */ CN_TXMAP,
    /* 68 */ CN_TXREF,
    /* 69 */ CN_WORD,
    /* 70 */ CN_ENUMS
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
    /* 28 */ "PROXY",
    /* 29 */ "PROXYSET",
    /* 30 */ "QUEUE",
    /* 31 */ "QUEUEBUFFER",
    /* 32 */ "ROW",
    /* 33 */ "ROWBUFFER",
    /* 34 */ "ROWCURSOR",
    /* 35 */ "ROWSTRAND",
    /* 36 */ "SCANBUFFER",
    /* 37 */ "SEHISTORY",
    /* 38 */ "SEITF",
    /* 39 */ "SEITFCURSOR",
    /* 40 */ "SEITFLIST",
    /* 41 */ "SEPROMPT",
    /* 42 */ "SINCE",
    /* 43 */ "SKIPCURSOR",
    /* 44 */ "SKIPLIST",
    /* 45 */ "SKIPNODE",
    /* 46 */ "SMALLHEAP",
    /* 47 */ "SOCKET",
    /* 48 */ "STACK",
    /* 49 */ "STEPACCUM",
    /* 50 */ "STEPARRAY",
    /* 51 */ "SYSCALL",
    /* 52 */ "TABLE",
    /* 53 */ "TASK",
    /* 54 */ "TASKCURSOR",
    /* 55 */ "TASKLIST",
    /* 56 */ "TASKSET",
    /* 57 */ "THREAD",
    /* 58 */ "THREADCURSOR",
    /* 59 */ "THREADLIST",
    /* 60 */ "TIME",
    /* 61 */ "TIMER",
    /* 62 */ "TOKENISER",
    /* 63 */ "TX",
    /* 64 */ "TXARRAY",
    /* 65 */ "TXCOLLECTION",
    /* 66 */ "TXCURSOR",
    /* 67 */ "TXMAP",
    /* 68 */ "TXREF",
    /* 69 */ "WORD",
    /* 70 */ "ENUMS"
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