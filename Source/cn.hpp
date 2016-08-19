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
    /* 26 */ CN_PROXY,
    /* 27 */ CN_PROXYSET,
    /* 28 */ CN_QUEUE,
    /* 29 */ CN_QUEUEBUFFER,
    /* 30 */ CN_ROW,
    /* 31 */ CN_ROWCURSOR,
    /* 32 */ CN_SEGMENTBUFFER,
    /* 33 */ CN_SEITF,
    /* 34 */ CN_SEITFCURSOR,
    /* 35 */ CN_SEITFLIST,
    /* 36 */ CN_SMALLHEAP,
    /* 37 */ CN_SOCKET,
    /* 38 */ CN_STACK,
    /* 39 */ CN_STRAND,
    /* 40 */ CN_SYSCALL,
    /* 41 */ CN_TABLE,
    /* 42 */ CN_TASK,
    /* 43 */ CN_TASKCURSOR,
    /* 44 */ CN_TASKLIST,
    /* 45 */ CN_TASKSET,
    /* 46 */ CN_THREAD,
    /* 47 */ CN_THREADLIST,
    /* 48 */ CN_TIMER,
    /* 49 */ CN_TX,
    /* 50 */ CN_TXMAP,
    /* 51 */ CN_TXLIST,
    /* 52 */ CN_WORD,
    /* 53 */ CN_ENUMS
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
    /* 26 */ "PROXY",
    /* 27 */ "PROXYSET",
    /* 28 */ "QUEUE",
    /* 29 */ "QUEUEBUFFER",
    /* 30 */ "ROW",
    /* 31 */ "ROWCURSOR",
    /* 32 */ "SEGMENTBUFFER",
    /* 33 */ "SEITF",
    /* 34 */ "SEITFCURSOR",
    /* 35 */ "SEITFLIST",
    /* 36 */ "SMALLHEAP",
    /* 37 */ "SOCKET",
    /* 38 */ "STACK",
    /* 39 */ "STRAND",
    /* 40 */ "SYSCALL",
    /* 41 */ "TABLE",
    /* 42 */ "TASK",
    /* 43 */ "TASKCURSOR",
    /* 44 */ "TASKLIST",
    /* 45 */ "TASKSET",
    /* 46 */ "THREAD",
    /* 47 */ "THREADLIST",
    /* 48 */ "TIMER",
    /* 49 */ "TX",
    /* 50 */ "TXMAP",
    /* 51 */ "TXLIST",
    /* 52 */ "WORD",
    /* 53 */ "ENUMS"
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