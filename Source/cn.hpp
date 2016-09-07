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
    /* 18 */ CN_GLOBALDICTIONARY,
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
    /* 57 */ CN_TIMER,
    /* 58 */ CN_TOKENISER,
    /* 59 */ CN_TX,
    /* 60 */ CN_TXARRAY,
    /* 61 */ CN_TXCOLLECTION,
    /* 62 */ CN_TXCURSOR,
    /* 63 */ CN_TXMAP,
    /* 64 */ CN_TXREF,
    /* 65 */ CN_WORD,
    /* 66 */ CN_ENUMS
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
    /* 18 */ "GLOBALDICTIONARY",
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
    /* 57 */ "TIMER",
    /* 58 */ "TOKENISER",
    /* 59 */ "TX",
    /* 60 */ "TXARRAY",
    /* 61 */ "TXCOLLECTION",
    /* 62 */ "TXCURSOR",
    /* 63 */ "TXMAP",
    /* 64 */ "TXREF",
    /* 65 */ "WORD",
    /* 66 */ "ENUMS"
  };
  
  class cn
  {
  private:
    eCN m_eClassNo;
    
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