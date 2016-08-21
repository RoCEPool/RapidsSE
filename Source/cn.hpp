/*============================================================================*/
/* cn.hpp - class number                                                      */
/*============================================================================*/
#pragma once

#include "Types.h"

namespace Types
{
  enum eCN // Class Number
  {
    //
    // if you change this enum, change Task::vBoot()
    //
    
    /*  0 */ CN_NONE=0,
    
    /*  1 */ CN_INTVAL,
    /*  2 */ CN_IPVAL,
    /*  3 */ CN_OBJVAL,
    /*  3 */ CN_PTRVAL,
    /*  4 */ CN_STRVAL,
    /*  5 */ CN_TASKVAL,
    /*  6 */ CN_VALSET,
    /*  7 */ CN_VALWSP,
    /*  8 */ CN_WRDVAL,
    
    /*  9 */ CN_ARGUMENTS,
    /* 10 */ CN_ATTACHMAP,
    /* 11 */ CN_BUFFER,
    /* 12 */ CN_BUFFERPOOL,
    /* 13 */ CN_BUFFERPOOLANALYZER,
    /* 14 */ CN_BUFFERPOOLITER,
    /* 15 */ CN_DICTIONARY,
    /* 16 */ CN_DIR,
    /* 17 */ CN_FILE,
    /* 18 */ CN_FREECHUNK,
    /* 19 */ CN_GLOBALDICTIONARY,
    /* 20 */ CN_GRIDMODEL,
    /* 21 */ CN_HEAPBUFFER,
    /* 22 */ CN_INDEX,
    /* 23 */ CN_LOCK,
    /* 24 */ CN_MEDIUMHEAP,
    /* 25 */ CN_NODE,
    /* 26 */ CN_PARTITION,
    /* 27 */ CN_PROXY,
    /* 28 */ CN_PROXYSET,
    /* 29 */ CN_QUEUE,
    /* 30 */ CN_QUEUEBUFFER,
    /* 31 */ CN_ROW,
    /* 32 */ CN_ROWCURSOR,
    /* 33 */ CN_SEGMENTBUFFER,
    /* 34 */ CN_SEITF,
    /* 35 */ CN_SEITFCURSOR,
    /* 36 */ CN_SEITFLIST,
    /* 37 */ CN_SMALLHEAP,
    /* 38 */ CN_SOCKET,
    /* 39 */ CN_STACK,
    /* 40 */ CN_STRAND,
    /* 41 */ CN_SYSCALL,
    /* 42 */ CN_TABLE,
    /* 43 */ CN_TASK,
    /* 44 */ CN_TASKCURSOR,
    /* 45 */ CN_TASKLIST,
    /* 46 */ CN_TASKSET,
    /* 47 */ CN_THREAD,
    /* 48 */ CN_THREADLIST,
    /* 49 */ CN_TIMER,
    /* 50 */ CN_TX,
    /* 51 */ CN_TXMAP,
    /* 52 */ CN_TXLIST,
    /* 53 */ CN_WORD,
    /* 54 */ CN_ENUMS
  };
  
  static const char *pCN[]
  {
    /*  0 */ "NONE",
    
    /*  1 */ "INTVAL",
    /*  2 */ "IPVAL",
    /*  3 */ "OBJVAL",
    /*  3 */ "PTRVAL",
    /*  4 */ "STRVAL",
    /*  5 */ "TASKVAL",
    /*  6 */ "VALSET",
    /*  7 */ "VALWSP",
    /*  8 */ "WRDVAL",
    
    /*  9 */ "ARGUMENTS",
    /* 10 */ "ATTACHMAP",
    /* 11 */ "BUFFER",
    /* 12 */ "BUFFERPOOL",
    /* 13 */ "BUFFERPOOLANALYZER",
    /* 14 */ "BUFFERPOOLITER",
    /* 15 */ "DICTIONARY",
    /* 16 */ "DIR",
    /* 17 */ "FILE",
    /* 18 */ "FREECHUNK",
    /* 19 */ "GLOBALDICTIONARY",
    /* 20 */ "GRIDMODEL",
    /* 21 */ "HEAPBUFFER",
    /* 22 */ "INDEX",
    /* 23 */ "LOCL",
    /* 24 */ "MEDIUMHEAP",
    /* 25 */ "NODE",
    /* 26 */ "PARTITION",
    /* 27 */ "PROXY",
    /* 28 */ "PROXYSET",
    /* 29 */ "QUEUE",
    /* 30 */ "QUEUEBUFFER",
    /* 31 */ "ROW",
    /* 32 */ "ROWCURSOR",
    /* 33 */ "SEGMENTBUFFER",
    /* 34 */ "SEITF",
    /* 35 */ "SEITFCURSOR",
    /* 36 */ "SEITFLIST",
    /* 37 */ "SMALLHEAP",
    /* 38 */ "SOCKET",
    /* 39 */ "STACK",
    /* 40 */ "STRAND",
    /* 41 */ "SYSCALL",
    /* 42 */ "TABLE",
    /* 43 */ "TASK",
    /* 44 */ "TASKCURSOR",
    /* 45 */ "TASKLIST",
    /* 46 */ "TASKSET",
    /* 47 */ "THREAD",
    /* 48 */ "THREADLIST",
    /* 49 */ "TIMER",
    /* 50 */ "TX",
    /* 51 */ "TXMAP",
    /* 52 */ "TXLIST",
    /* 53 */ "WORD",
    /* 54 */ "ENUMS"
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
    //void vAssertClass(eCN eClassNo) const;
    eCN  eClass(void) const;
    void vClass(eCN eClassNo);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/