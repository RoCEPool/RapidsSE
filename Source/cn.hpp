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
    /* 13 */ CN_DICTIONARY,
    /* 14 */ CN_DICTIONARYSTACK,
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
    /* 26 */ CN_PARSER,
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
    
    /*  1 */ "S32",
    /*  2 */ "S256",
    /*  3 */ "S8K",
    
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