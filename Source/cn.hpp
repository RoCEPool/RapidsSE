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
    /* 13 */ CN_CHARRED,
    /* 14 */ CN_DICTIONARY,
    /* 15 */ CN_DIR,
    /* 16 */ CN_FILE,
    /* 17 */ CN_FREECHUNK,
    /* 18 */ CN_GLOBALDICTIONARY,
    /* 19 */ CN_GRID,
    /* 20 */ CN_HEAPBUFFER,
    /* 21 */ CN_INDEX,
    /* 22 */ CN_LOADER,
    /* 23 */ CN_LOCK,
    /* 24 */ CN_MEDIUMHEAP,
    /* 25 */ CN_NODE,
    /* 26 */ CN_NODECURSOR,
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
    /* 50 */ CN_THREADCURSOR,
    /* 51 */ CN_THREADLIST,
    /* 52 */ CN_TIMER,
    /* 53 */ CN_TX,
    /* 54 */ CN_TXARRAY,
    /* 55 */ CN_TXCOLLECTION,
    /* 56 */ CN_TXCURSOR,
    /* 57 */ CN_TXMAP,
    /* 58 */ CN_TXREF,
    /* 59 */ CN_WORD,
    /* 60 */ CN_ENUMS
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
    /* 13 */ "CHARRED"
    /* 14 */ "DICTIONARY",
    /* 15 */ "DIR",
    /* 16 */ "FILE",
    /* 17 */ "FREECHUNK",
    /* 18 */ "GLOBALDICTIONARY",
    /* 19 */ "GRID",
    /* 20 */ "HEAPBUFFER",
    /* 21 */ "INDEX",
    /* 22 */ "LOADER",
    /* 23 */ "LOCK",
    /* 24 */ "MEDIUMHEAP",
    /* 25 */ "NODE",
    /* 26 */ "NODECURSOR",
    /* 27 */ "PARTITION",
    /* 28 */ "PROXY",
    /* 29 */ "PROXYSET",
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
    /* 50 */ "THREADCURSOR",
    /* 51 */ "THREADLIST",
    /* 52 */ "TIMER",
    /* 53 */ "TX",
    /* 54 */ "TXARRAY",
    /* 55 */ "TXCOLLECTION",
    /* 56 */ "TXCURSOR",
    /* 57 */ "TXMAP",
    /* 58 */ "TXREF",
    /* 59 */ "WORD",
    /* 60 */ "ENUMS"
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