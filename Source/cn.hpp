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
    /* 13 */ CN_PARSER,
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
    /* 40 */ CN_SEPROMPT,
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
    /* 55 */ CN_TXARRAY,
    /* 56 */ CN_TXCOLLECTION,
    /* 57 */ CN_TXCURSOR,
    /* 58 */ CN_TXMAP,
    /* 59 */ CN_TXREF,
    /* 60 */ CN_WORD,
    /* 61 */ CN_ENUMS
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
    /* 13 */ "PARSER"
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
    /* 40 */ "SEPROMPT",
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
    /* 55 */ "TXARRAY",
    /* 56 */ "TXCOLLECTION",
    /* 57 */ "TXCURSOR",
    /* 58 */ "TXMAP",
    /* 59 */ "TXREF",
    /* 60 */ "WORD",
    /* 61 */ "ENUMS"
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