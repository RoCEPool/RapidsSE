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
    /* 32 */ CN_SEITFLIST,
    /* 33 */ CN_SMALLHEAP,
    /* 34 */ CN_SOCKET,
    /* 35 */ CN_STACK,
    /* 36 */ CN_STRAND,
    /* 37 */ CN_SYSCALL,
    /* 38 */ CN_TABLE,
    /* 39 */ CN_TASK,
    /* 40 */ CN_TASKLIST,
    /* 41 */ CN_TASKSET,
    /* 42 */ CN_THREAD,
    /* 43 */ CN_THREADLIST,
    /* 44 */ CN_TX,
    /* 45 */ CN_TXMAP,
    /* 46 */ CN_TXLIST,
    /* 47 */ CN_WORD,
    /* 48 */ CN_ENUMS
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
    /* 32 */ "SEITFLIST",
    /* 33 */ "SMALLHEAP",
    /* 34 */ "SOCKET",
    /* 35 */ "STACK",
    /* 36 */ "STRAND",
    /* 37 */ "SYSCALL",
    /* 38 */ "TABLE",
    /* 39 */ "TASK",
    /* 40 */ "TASKLIST",
    /* 41 */ "TASKSET",
    /* 42 */ "THREAD",
    /* 43 */ "THREADLIST",
    /* 44 */ "TX",
    /* 45 */ "TXMAP",
    /* 46 */ "TXLIST",
    /* 47 */ "WORD",
    /* 48 */ "ENUMS"
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