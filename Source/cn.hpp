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
    /* 30 */ CN_SEGMENTBUFFER,
    /* 31 */ CN_SEITF,
    /* 32 */ CN_SEITFCURSOR,
    /* 33 */ CN_SEITFLIST,
    /* 34 */ CN_SMALLHEAP,
    /* 35 */ CN_SOCKET,
    /* 36 */ CN_STACK,
    /* 37 */ CN_STRAND,
    /* 38 */ CN_SYSCALL,
    /* 39 */ CN_TABLE,
    /* 40 */ CN_TASK,
    /* 41 */ CN_TASKLIST,
    /* 42 */ CN_TASKSET,
    /* 43 */ CN_THREAD,
    /* 44 */ CN_THREADLIST,
    /* 45 */ CN_TIMER,
    /* 46 */ CN_TX,
    /* 47 */ CN_TXMAP,
    /* 48 */ CN_TXLIST,
    /* 49 */ CN_WORD,
    /* 50 */ CN_ENUMS
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
    /* 30 */ "SEGMENTBUFFER",
    /* 31 */ "SEITF",
    /* 32 */ "SEITFCURSOR",
    /* 33 */ "SEITFLIST",
    /* 34 */ "SMALLHEAP",
    /* 35 */ "SOCKET",
    /* 36 */ "STACK",
    /* 37 */ "STRAND",
    /* 38 */ "SYSCALL",
    /* 39 */ "TABLE",
    /* 40 */ "TASK",
    /* 41 */ "TASKLIST",
    /* 42 */ "TASKSET",
    /* 43 */ "THREAD",
    /* 44 */ "THREADLIST",
    /* 45 */ "TIMER",
    /* 46 */ "TX",
    /* 47 */ "TXMAP",
    /* 48 */ "TXLIST",
    /* 49 */ "WORD",
    /* 50 */ "ENUMS"
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