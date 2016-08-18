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
    /* 18 */ CN_GLOBALDICTIONARY,
    /* 19 */ CN_GRIDMODEL,
    /* 20 */ CN_HEAPBUFFER,
    /* 21 */ CN_INDEX,
    /* 22 */ CN_LOCK,
    /* 23 */ CN_MEDIUMHEAP,
    /* 24 */ CN_NODE,
    /* 25 */ CN_PROXY,
    /* 26 */ CN_PROXYSET,
    /* 27 */ CN_QUEUE,
    /* 28 */ CN_QUEUEBUFFER,
    /* 29 */ CN_ROW,
    /* 30 */ CN_ROWCURSOR,
    /* 31 */ CN_SEGMENTBUFFER,
    /* 32 */ CN_SEITF,
    /* 33 */ CN_SEITFCURSOR,
    /* 34 */ CN_SEITFLIST,
    /* 35 */ CN_SMALLHEAP,
    /* 36 */ CN_SOCKET,
    /* 37 */ CN_STACK,
    /* 38 */ CN_STRAND,
    /* 39 */ CN_SYSCALL,
    /* 40 */ CN_TABLE,
    /* 41 */ CN_TASK,
    /* 42 */ CN_TASKLIST,
    /* 43 */ CN_TASKSET,
    /* 44 */ CN_THREAD,
    /* 45 */ CN_THREADLIST,
    /* 46 */ CN_TIMER,
    /* 47 */ CN_TX,
    /* 48 */ CN_TXMAP,
    /* 49 */ CN_TXLIST,
    /* 50 */ CN_WORD,
    /* 51 */ CN_ENUMS
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
    /* 18 */ "GLOBALDICTIONARY",
    /* 19 */ "GRIDMODEL",
    /* 20 */ "HEAPBUFFER",
    /* 21 */ "INDEX",
    /* 22 */ "LOCL",
    /* 23 */ "MEDIUMHEAP",
    /* 24 */ "NODE",
    /* 25 */ "PROXY",
    /* 26 */ "PROXYSET",
    /* 27 */ "QUEUE",
    /* 28 */ "QUEUEBUFFER",
    /* 29 */ "ROW",
    /* 30 */ "ROWCURSOR",
    /* 31 */ "SEGMENTBUFFER",
    /* 32 */ "SEITF",
    /* 33 */ "SEITFCURSOR",
    /* 34 */ "SEITFLIST",
    /* 35 */ "SMALLHEAP",
    /* 36 */ "SOCKET",
    /* 37 */ "STACK",
    /* 38 */ "STRAND",
    /* 39 */ "SYSCALL",
    /* 40 */ "TABLE",
    /* 41 */ "TASK",
    /* 42 */ "TASKLIST",
    /* 43 */ "TASKSET",
    /* 44 */ "THREAD",
    /* 45 */ "THREADLIST",
    /* 46 */ "TIMER",
    /* 47 */ "TX",
    /* 48 */ "TXMAP",
    /* 49 */ "TXLIST",
    /* 50 */ "WORD",
    /* 51 */ "ENUMS"
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