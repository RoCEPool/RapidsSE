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
    /* 22 */ CN_MEDIUMHEAP,
    /* 23 */ CN_NODE,
    /* 24 */ CN_PROXY,
    /* 25 */ CN_PROXYSET,
    /* 26 */ CN_QUEUE,
    /* 27 */ CN_QUEUEBUFFER,
    /* 28 */ CN_ROW,
    /* 29 */ CN_SEGMENTBUFFER,
    /* 30 */ CN_SEITF,
    /* 31 */ CN_SEITFLIST,
    /* 32 */ CN_SMALLHEAP,
    /* 33 */ CN_SOCKET,
    /* 34 */ CN_STACK,
    /* 35 */ CN_STRAND,
    /* 36 */ CN_SYSCALL,
    /* 37 */ CN_TABLE,
    /* 38 */ CN_TASK,
    /* 39 */ CN_TASKLIST,
    /* 40 */ CN_TASKSET,
    /* 41 */ CN_THREAD,
    /* 42 */ CN_THREADLIST,
    /* 43 */ CN_TX,
    /* 44 */ CN_TXMAP,
    /* 45 */ CN_TXLIST,
    /* 46 */ CN_WORD,
    /* 47 */ CN_ENUMS
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
    /* 22 */ "MEDIUMHEAP",
    /* 23 */ "NODE",
    /* 24 */ "PROXY",
    /* 25 */ "PROXYSET",
    /* 26 */ "QUEUE",
    /* 27 */ "QUEUEBUFFER",
    /* 28 */ "ROW",
    /* 29 */ "SEGMENTBUFFER",
    /* 30 */ "SEITF",
    /* 31 */ "SEITFLIST",
    /* 32 */ "SMALLHEAP",
    /* 33 */ "SOCKET",
    /* 34 */ "STACK",
    /* 35 */ "STRAND",
    /* 36 */ "SYSCALL",
    /* 37 */ "TABLE",
    /* 38 */ "TASK",
    /* 39 */ "TASKLIST",
    /* 40 */ "TASKSET",
    /* 41 */ "THREAD",
    /* 42 */ "THREADLIST",
    /* 43 */ "TX",
    /* 44 */ "TXMAP",
    /* 45 */ "TXLIST",
    /* 46 */ "WORD",
    /* 47 */ "ENUMS"
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