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
    /* 23 */ CN_LOADER,
    /* 24 */ CN_LOCK,
    /* 25 */ CN_MEDIUMHEAP,
    /* 26 */ CN_NODE,
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
    /* 37 */ CN_SEITF,
    /* 38 */ CN_SEITFCURSOR,
    /* 39 */ CN_SEITFLIST,
    /* 40 */ CN_SMALLHEAP,
    /* 41 */ CN_SOCKET,
    /* 42 */ CN_STACK,
    /* 43 */ CN_SYSCALL,
    /* 44 */ CN_TABLE,
    /* 45 */ CN_TASK,
    /* 46 */ CN_TASKCURSOR,
    /* 47 */ CN_TASKLIST,
    /* 48 */ CN_TASKSET,
    /* 49 */ CN_THREAD,
    /* 50 */ CN_THREADLIST,
    /* 51 */ CN_TIMER,
    /* 52 */ CN_TX,
    /* 53 */ CN_TXMAP,
    /* 54 */ CN_TXLIST,
    /* 55 */ CN_WORD,
    /* 56 */ CN_ENUMS
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
    /* 23 */ "LOADER",
    /* 24 */ "LOCK",
    /* 25 */ "MEDIUMHEAP",
    /* 26 */ "NODE",
    /* 27 */ "PARTITION",
    /* 28 */ "PROXY",
    /* 28 */ "PROXYSET",
    /* 30 */ "QUEUE",
    /* 31 */ "QUEUEBUFFER",
    /* 32 */ "ROW",
    /* 33 */ "ROWBUFFER",
    /* 34 */ "ROWCURSOR",
    /* 35 */ "ROWSTRAND",
    /* 36 */ "SCANBUFFER",
    /* 37 */ "SEITF",
    /* 38 */ "SEITFCURSOR",
    /* 39 */ "SEITFLIST",
    /* 40 */ "SMALLHEAP",
    /* 41 */ "SOCKET",
    /* 42 */ "STACK",
    /* 43 */ "SYSCALL",
    /* 44 */ "TABLE",
    /* 45 */ "TASK",
    /* 46 */ "TASKCURSOR",
    /* 47 */ "TASKLIST",
    /* 48 */ "TASKSET",
    /* 49 */ "THREAD",
    /* 50 */ "THREADLIST",
    /* 51 */ "TIMER",
    /* 52 */ "TX",
    /* 53 */ "TXMAP",
    /* 54 */ "TXLIST",
    /* 55 */ "WORD",
    /* 56 */ "ENUMS"
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