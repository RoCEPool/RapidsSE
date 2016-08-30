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
    /*  8 */ CN_ATTACHMAP,
    /*  9 */ CN_BUFFER,
    /* 10 */ CN_BUFFERPOOL,
    /* 11 */ CN_BUFFERPOOLANALYZER,
    /* 12 */ CN_BUFFERPOOLITER,
    /* 13 */ CN_DICTIONARY,
    /* 14 */ CN_DIR,
    /* 15 */ CN_FILE,
    /* 16 */ CN_FREECHUNK,
    /* 17 */ CN_GLOBALDICTIONARY,
    /* 18 */ CN_GRID,
    /* 19 */ CN_HEAPBUFFER,
    /* 20 */ CN_INDEX,
    /* 21 */ CN_LOADER,
    /* 22 */ CN_LOCK,
    /* 23 */ CN_MEDIUMHEAP,
    /* 24 */ CN_NODE,
    /* 25 */ CN_NODECURSOR,
    /* 26 */ CN_PARTITION,
    /* 27 */ CN_PROXY,
    /* 28 */ CN_PROXYSET,
    /* 29 */ CN_QUEUE,
    /* 30 */ CN_QUEUEBUFFER,
    /* 31 */ CN_ROW,
    /* 32 */ CN_ROWBUFFER,
    /* 33 */ CN_ROWCURSOR,
    /* 34 */ CN_ROWSTRAND,
    /* 35 */ CN_SCANBUFFER,
    /* 36 */ CN_SEITF,
    /* 37 */ CN_SEITFCURSOR,
    /* 38 */ CN_SEITFLIST,
    /* 39 */ CN_SMALLHEAP,
    /* 40 */ CN_SOCKET,
    /* 41 */ CN_STACK,
    /* 42 */ CN_SYSCALL,
    /* 43 */ CN_TABLE,
    /* 44 */ CN_TASK,
    /* 45 */ CN_TASKCURSOR,
    /* 46 */ CN_TASKLIST,
    /* 47 */ CN_TASKSET,
    /* 48 */ CN_THREAD,
    /* 49 */ CN_THREADCURSOR,
    /* 50 */ CN_THREADLIST,
    /* 51 */ CN_TIMER,
    /* 52 */ CN_TX,
    /* 53 */ CN_TXARRAY,
    /* 54 */ CN_TXCOLLECTION,
    /* 55 */ CN_TXCURSOR,
    /* 56 */ CN_TXMAP,
    /* 57 */ CN_TXREF,
    /* 58 */ CN_WORD,
    /* 59 */ CN_ENUMS
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
    /*  8 */ "ATTACHMAP",
    /*  9 */ "BUFFER",
    /* 10 */ "BUFFERPOOL",
    /* 11 */ "BUFFERPOOLANALYZER",
    /* 12 */ "BUFFERPOOLITER",
    /* 13 */ "DICTIONARY",
    /* 14 */ "DIR",
    /* 15 */ "FILE",
    /* 16 */ "FREECHUNK",
    /* 17 */ "GLOBALDICTIONARY",
    /* 18 */ "GRID",
    /* 19 */ "HEAPBUFFER",
    /* 20 */ "INDEX",
    /* 21 */ "LOADER",
    /* 22 */ "LOCK",
    /* 23 */ "MEDIUMHEAP",
    /* 24 */ "NODE",
    /* 25 */ "NODECURSOR",
    /* 26 */ "PARTITION",
    /* 27 */ "PROXY",
    /* 28 */ "PROXYSET",
    /* 29 */ "QUEUE",
    /* 30 */ "QUEUEBUFFER",
    /* 31 */ "ROW",
    /* 32 */ "ROWBUFFER",
    /* 33 */ "ROWCURSOR",
    /* 34 */ "ROWSTRAND",
    /* 35 */ "SCANBUFFER",
    /* 36 */ "SEITF",
    /* 37 */ "SEITFCURSOR",
    /* 38 */ "SEITFLIST",
    /* 39 */ "SMALLHEAP",
    /* 40 */ "SOCKET",
    /* 41 */ "STACK",
    /* 42 */ "SYSCALL",
    /* 43 */ "TABLE",
    /* 44 */ "TASK",
    /* 45 */ "TASKCURSOR",
    /* 46 */ "TASKLIST",
    /* 47 */ "TASKSET",
    /* 48 */ "THREAD",
    /* 49 */ "THREADCURSOR",
    /* 50 */ "THREADLIST",
    /* 51 */ "TIMER",
    /* 52 */ "TX",
    /* 53 */ "TXARRAY",
    /* 54 */ "TXCOLLECTION",
    /* 55 */ "TXCURSOR",
    /* 56 */ "TXMAP",
    /* 57 */ "TXREF",
    /* 58 */ "WORD",
    /* 59 */ "ENUMS"
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
    eCN  eClass(void) const;
    void vClass(eCN eClassNo);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/