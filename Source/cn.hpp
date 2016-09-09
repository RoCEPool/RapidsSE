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
    /* 16 */ CN_DICTIONARYSTACK,
    /* 17 */ CN_DIR,
    /* 18 */ CN_FILE,
    /* 19 */ CN_FREECHUNK,
    /* 20 */ CN_GRID,
    /* 21 */ CN_HEAPBUFFER,
    /* 22 */ CN_INDEX,
    /* 23 */ CN_LOADER,
    /* 24 */ CN_LOCK,
    /* 25 */ CN_MEDIUMHEAP,
    /* 26 */ CN_NODE,
    /* 27 */ CN_NODECURSOR,
    /* 28 */ CN_PARTITION,
    /* 29 */ CN_PARTITIONS,
    /* 30 */ CN_PROXY,
    /* 31 */ CN_PROXYSET,
    /* 32 */ CN_QUEUE,
    /* 33 */ CN_QUEUEBUFFER,
    /* 34 */ CN_RING,
    /* 35 */ CN_ROW,
    /* 36 */ CN_ROWBUFFER,
    /* 37 */ CN_ROWCURSOR,
    /* 38 */ CN_ROWSTRAND,
    /* 39 */ CN_SCANBUFFER,
    /* 40 */ CN_SEHISTORY,
    /* 41 */ CN_SEITF,
    /* 42 */ CN_SEITFCURSOR,
    /* 43 */ CN_SEITFLIST,
    /* 44 */ CN_SEPROMPT,
    /* 45 */ CN_SINCE,
    /* 46 */ CN_SKIPCURSOR,
    /* 47 */ CN_SKIPLIST,
    /* 48 */ CN_SKIPNODE,
    /* 49 */ CN_SMALLHEAP,
    /* 50 */ CN_SOCKET,
    /* 51 */ CN_STACK,
    /* 52 */ CN_STEPACCUM,
    /* 53 */ CN_STEPARRAY,
    /* 54 */ CN_SYSCALL,
    /* 55 */ CN_TABLE,
    /* 56 */ CN_TASK,
    /* 57 */ CN_TASKCURSOR,
    /* 58 */ CN_TASKLIST,
    /* 59 */ CN_TASKSET,
    /* 60 */ CN_THREAD,
    /* 61 */ CN_THREADCURSOR,
    /* 62 */ CN_THREADLIST,
    /* 63 */ CN_TIME,
    /* 64 */ CN_TIMER,
    /* 65 */ CN_TOKENISER,
    /* 66 */ CN_TX,
    /* 67 */ CN_TXARRAY,
    /* 68 */ CN_TXCOLLECTION,
    /* 69 */ CN_TXCURSOR,
    /* 70 */ CN_TXMAP,
    /* 71 */ CN_TXREF,
    /* 72 */ CN_WORD,
    /* 73 */ CN_ENUMS
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
    /* 16 */ "DICTIONARYSTACK",
    /* 17 */ "DIR",
    /* 18 */ "FILE",
    /* 19 */ "FREECHUNK",
    /* 20 */ "GRID",
    /* 21 */ "HEAPBUFFER",
    /* 22 */ "INDEX",
    /* 23 */ "LOADER",
    /* 24 */ "LOCK",
    /* 25 */ "MEDIUMHEAP",
    /* 26 */ "NODE",
    /* 27 */ "NODECURSOR",
    /* 28 */ "PARTITION",
    /* 29 */ "PARTITIONS",
    /* 30 */ "PROXY",
    /* 31 */ "PROXYSET",
    /* 32 */ "QUEUE",
    /* 33 */ "QUEUEBUFFER",
    /* 34 */ "RING",
    /* 35 */ "ROW",
    /* 36 */ "ROWBUFFER",
    /* 37 */ "ROWCURSOR",
    /* 38 */ "ROWSTRAND",
    /* 39 */ "SCANBUFFER",
    /* 40 */ "SEHISTORY",
    /* 41 */ "SEITF",
    /* 42 */ "SEITFCURSOR",
    /* 43 */ "SEITFLIST",
    /* 44 */ "SEPROMPT",
    /* 45 */ "SINCE",
    /* 46 */ "SKIPCURSOR",
    /* 47 */ "SKIPLIST",
    /* 48 */ "SKIPNODE",
    /* 49 */ "SMALLHEAP",
    /* 50 */ "SOCKET",
    /* 51 */ "STACK",
    /* 52 */ "STEPACCUM",
    /* 53 */ "STEPARRAY",
    /* 54 */ "SYSCALL",
    /* 55 */ "TABLE",
    /* 56 */ "TASK",
    /* 57 */ "TASKCURSOR",
    /* 58 */ "TASKLIST",
    /* 59 */ "TASKSET",
    /* 60 */ "THREAD",
    /* 61 */ "THREADCURSOR",
    /* 62 */ "THREADLIST",
    /* 63 */ "TIME",
    /* 64 */ "TIMER",
    /* 65 */ "TOKENISER",
    /* 66 */ "TX",
    /* 67 */ "TXARRAY",
    /* 68 */ "TXCOLLECTION",
    /* 69 */ "TXCURSOR",
    /* 70 */ "TXMAP",
    /* 71 */ "TXREF",
    /* 72 */ "WORD",
    /* 73 */ "ENUMS"
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