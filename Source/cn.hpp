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
    /*  8 */ CN_ATTACHMAP,
    /*  9 */ CN_BUFFER,
    /* 10 */ CN_BUFFERPOOL,
    /* 11 */ CN_BUFFERPOOLANALYZER,
    /* 12 */ CN_BUFFERPOOLITER,
    /* 13 */ CN_PARSER,
    /* 14 */ CN_DICTIONARY,
    /* 15 */ CN_DIR,
    /* 16 */ CN_FILE,
    /* 17 */ CN_FREECHUNK,
    /* 18 */ CN_GRID,
    /* 19 */ CN_HEAPBUFFER,
    /* 20 */ CN_INDEX,
    /* 21 */ CN_LOADER,
    /* 22 */ CN_LOCK,
    /* 23 */ CN_MEDIUMHEAP,
    /* 24 */ CN_NODE,
    /* 25 */ CN_NODECURSOR,
    /* 26 */ CN_PARTITION,
    /* 27 */ CN_PROXY,
    /* 28 */ CN_PROXYSET,
    /* 29 */ CN_QUEUE,
    /* 30 */ CN_QUEUEBUFFER,
    /* 31 */ CN_ROW,
    /* 32 */ CN_ROWBUFFER,
    /* 33 */ CN_ROWCURSOR,
    /* 34 */ CN_ROWSTRAND,
    /* 35 */ CN_SCANBUFFER,
    /* 36 */ CN_SEHISTORY,
    /* 37 */ CN_SEITF,
    /* 38 */ CN_SEITFCURSOR,
    /* 39 */ CN_SEITFLIST,
    /* 40 */ CN_SEPROMPT,
    /* 41 */ CN_SINCE,
    /* 42 */ CN_SKIPCURSOR,
    /* 43 */ CN_SKIPLIST,
    /* 44 */ CN_SKIPNODE,
    /* 45 */ CN_SMALLHEAP,
    /* 46 */ CN_SOCKET,
    /* 47 */ CN_STACK,
    /* 48 */ CN_SYSCALL,
    /* 49 */ CN_TABLE,
    /* 50 */ CN_TASK,
    /* 51 */ CN_TASKCURSOR,
    /* 52 */ CN_TASKLIST,
    /* 53 */ CN_TASKSET,
    /* 54 */ CN_THREAD,
    /* 55 */ CN_THREADCURSOR,
    /* 56 */ CN_THREADLIST,
    /* 57 */ CN_TIME,
    /* 58 */ CN_TIMER,
    /* 59 */ CN_TOKENISER,
    /* 60 */ CN_TX,
    /* 61 */ CN_TXARRAY,
    /* 62 */ CN_TXCOLLECTION,
    /* 63 */ CN_TXCURSOR,
    /* 64 */ CN_TXMAP,
    /* 65 */ CN_TXREF,
    /* 66 */ CN_WORD,
    /* 67 */ CN_ENUMS
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
    /*  8 */ "ATTACHMAP",
    /*  9 */ "BUFFER",
    /* 10 */ "BUFFERPOOL",
    /* 11 */ "BUFFERPOOLANALYZER",
    /* 12 */ "BUFFERPOOLITER",
    /* 13 */ "PARSER"
    /* 14 */ "DICTIONARY",
    /* 15 */ "DIR",
    /* 16 */ "FILE",
    /* 17 */ "FREECHUNK",
    /* 18 */ "GRID",
    /* 19 */ "HEAPBUFFER",
    /* 20 */ "INDEX",
    /* 21 */ "LOADER",
    /* 22 */ "LOCK",
    /* 23 */ "MEDIUMHEAP",
    /* 24 */ "NODE",
    /* 25 */ "NODECURSOR",
    /* 26 */ "PARTITION",
    /* 27 */ "PROXY",
    /* 28 */ "PROXYSET",
    /* 29 */ "QUEUE",
    /* 30 */ "QUEUEBUFFER",
    /* 31 */ "ROW",
    /* 32 */ "ROWBUFFER",
    /* 33 */ "ROWCURSOR",
    /* 34 */ "ROWSTRAND",
    /* 35 */ "SCANBUFFER",
    /* 36 */ "SEHISTORY",
    /* 37 */ "SEITF",
    /* 38 */ "SEITFCURSOR",
    /* 39 */ "SEITFLIST",
    /* 40 */ "SEPROMPT",
    /* 41 */ "SINCE",
    /* 42 */ "SKIPCURSOR",
    /* 43 */ "SKIPLIST",
    /* 44 */ "SKIPNODE",
    /* 45 */ "SMALLHEAP",
    /* 46 */ "SOCKET",
    /* 47 */ "STACK",
    /* 48 */ "SYSCALL",
    /* 49 */ "TABLE",
    /* 50 */ "TASK",
    /* 51 */ "TASKCURSOR",
    /* 52 */ "TASKLIST",
    /* 53 */ "TASKSET",
    /* 54 */ "THREAD",
    /* 55 */ "THREADCURSOR",
    /* 56 */ "THREADLIST",
    /* 57 */ "TIME",
    /* 58 */ "TIMER",
    /* 59 */ "TOKENISER",
    /* 60 */ "TX",
    /* 61 */ "TXARRAY",
    /* 62 */ "TXCOLLECTION",
    /* 63 */ "TXCURSOR",
    /* 64 */ "TXMAP",
    /* 65 */ "TXREF",
    /* 66 */ "WORD",
    /* 67 */ "ENUMS"
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