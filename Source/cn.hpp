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
    /* 20 */ CN_GRID,
    /* 21 */ CN_HEAPBUFFER,
    /* 22 */ CN_INDEX,
    /* 23 */ CN_LOADER,
    /* 24 */ CN_LOCK,
    /* 25 */ CN_MEDIUMHEAP,
    /* 26 */ CN_NODE,
    /* 27 */ CN_NODECURSOR,
    /* 28 */ CN_PARTITION,
    /* 29 */ CN_PROXY,
    /* 30 */ CN_PROXYSET,
    /* 31 */ CN_QUEUE,
    /* 32 */ CN_QUEUEBUFFER,
    /* 33 */ CN_ROW,
    /* 34 */ CN_ROWBUFFER,
    /* 35 */ CN_ROWCURSOR,
    /* 36 */ CN_ROWSTRAND,
    /* 37 */ CN_SCANBUFFER,
    /* 38 */ CN_SEITF,
    /* 39 */ CN_SEITFCURSOR,
    /* 40 */ CN_SEITFLIST,
    /* 41 */ CN_SMALLHEAP,
    /* 42 */ CN_SOCKET,
    /* 43 */ CN_STACK,
    /* 44 */ CN_SYSCALL,
    /* 45 */ CN_TABLE,
    /* 46 */ CN_TASK,
    /* 47 */ CN_TASKCURSOR,
    /* 48 */ CN_TASKLIST,
    /* 49 */ CN_TASKSET,
    /* 50 */ CN_THREAD,
    /* 51 */ CN_THREADCURSOR,
    /* 52 */ CN_THREADLIST,
    /* 53 */ CN_TIMER,
    /* 54 */ CN_TX,
    /* 55 */ CN_TXMAP,
    /* 56 */ CN_TXLIST,
    /* 57 */ CN_WORD,
    /* 58 */ CN_ENUMS
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
    /* 20 */ "GRID",
    /* 21 */ "HEAPBUFFER",
    /* 22 */ "INDEX",
    /* 23 */ "LOADER",
    /* 24 */ "LOCK",
    /* 25 */ "MEDIUMHEAP",
    /* 26 */ "NODE",
    /* 27 */ "NODECURSOR",
    /* 28 */ "PARTITION",
    /* 29 */ "PROXY",
    /* 30 */ "PROXYSET",
    /* 31 */ "QUEUE",
    /* 32 */ "QUEUEBUFFER",
    /* 33 */ "ROW",
    /* 34 */ "ROWBUFFER",
    /* 35 */ "ROWCURSOR",
    /* 36 */ "ROWSTRAND",
    /* 37 */ "SCANBUFFER",
    /* 38 */ "SEITF",
    /* 39 */ "SEITFCURSOR",
    /* 40 */ "SEITFLIST",
    /* 41 */ "SMALLHEAP",
    /* 42 */ "SOCKET",
    /* 43 */ "STACK",
    /* 44 */ "SYSCALL",
    /* 45 */ "TABLE",
    /* 46 */ "TASK",
    /* 47 */ "TASKCURSOR",
    /* 48 */ "TASKLIST",
    /* 49 */ "TASKSET",
    /* 50 */ "THREAD",
    /* 51 */ "THREADCURSOR",
    /* 52 */ "THREADLIST",
    /* 53 */ "TIMER",
    /* 54 */ "TX",
    /* 55 */ "TXMAP",
    /* 56 */ "TXLIST",
    /* 57 */ "WORD",
    /* 58 */ "ENUMS"
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