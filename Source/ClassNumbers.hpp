/*============================================================================*/
/* ClassNumbers.hpp - class numbers                                           */
/*============================================================================*/
#pragma once

#include "Base.h"

#include "Stz.hpp"

namespace Base
{

/*----------------------------------------------------------------------------*/
/* class numbers                                                              */
/*----------------------------------------------------------------------------*/
  enum eCN // Class Number
  {
    /*  0 */ CN_NONE=0,
    
    /*  1 */ CN_BCDVAL,
    /*  2 */ CN_BINVAL,
    /*  3 */ CN_BOLVAL,
    /*  4 */ CN_DTEVAL,
    /*  5 */ CN_ENTWRK,
    /*  6 */ CN_FLTVAL,
    /*  7 */ CN_INTVAL,
    /*  8 */ CN_IPVAL,
    /*  9 */ CN_IVLVAL,
    /* 10 */ CN_OBJVAL,
    /* 11 */ CN_PTRVAL,
    /* 12 */ CN_STRVAL,
    /* 13 */ CN_STRVAR,
    /* 14 */ CN_TMEVAL,
    /* 15 */ CN_TSPVAL,
    /* 16 */ CN_VALREG,
    /* 17 */ CN_VALSET,
    /* 18 */ CN_VALWRK,
  
    /* 19 */ CN_ACCESSPATH,
    /* 20 */ CN_ACCUM,
    /* 21 */ CN_ACCUMVEC,
    /* 22 */ CN_ARG,
    /* 23 */ CN_ARGUMENTS,
    /* 24 */ CN_ATOMIC,
    /* 25 */ CN_ATTACHMAP,
    
    /* 26 */ CN_BITARRAY,
    /* 27 */ CN_BITMAP,
    /* 28 */ CN_BUFFERPOOL,
    /* 29 */ CN_BPA,
  
    /* 30 */ CN_CARRYOVER,
    /* 31 */ CN_COLUMN,
    /* 32 */ CN_COLUMNVEC,
  
    /* 33 */ CN_DIC,
    /* 34 */ CN_DICPATH,
    /* 35 */ CN_DIR,
  
    /* 36 */ CN_ENTRY,
    /* 37 */ CN_ENTRYBUFFER,
    /* 38 */ CN_ENTRYVEC,
    
    /* 39 */ CN_FILE,
    /* 40 */ CN_FORMATTERS,
    /* 41 */ CN_FREEBUFFER,
    /* 42 */ CN_FREECHUNK,
    /* 43 */ CN_FREEDNOT,
    /* 44 */ CN_FREEENTRY,
    
    /* 45 */ CN_GRAPH,
    /* 46 */ CN_GRAPHCURSOR,
    /* 47 */ CN_GRAPHNODE,
    /* 48 */ CN_GRID,
    /* 49 */ CN_GROUPBY,
  
    /* 50 */ CN_HEAPBUFFER,
    
    /* 51 */ CN_INDEX,
    /* 52 */ CN_INDEXDATA,
    /* 53 */ CN_IDXCOLVEC,
    /* 54 */ CN_IDXDATAVEC,
    /* 55 */ CN_IDXVEC,
    /* 56 */ CN_INDEXLEVELS,
    /* 57 */ CN_ITEM,
    /* 58 */ CN_ITEMBUFFER,
  
    /* 59 */ CN_JUMBOPERSISTENT,
    /* 60 */ CN_JUMBOTRANSIENT,
  
    /* 61 */ CN_KEY,
    /* 62 */ CN_KEYVAL,
    /* 63 */ CN_KEYVEC,
    /* 64 */ CN_KEYWRK,
  
    /* 65 */ CN_LARGEPERSISTENT,
    /* 66 */ CN_LARGETRANSIENT,
    
    /* 67 */ CN_LINEITEM,
    /* 68 */ CN_LOADCHUNK,
    /* 69 */ CN_LOADER,
    /* 70 */ CN_LOADERSUB,
    /* 71 */ CN_LOADMAP,
    /* 72 */ CN_LOCK,
    
    /* 73 */ CN_MEDIUMPERSISTENT,
    /* 74 */ CN_MEDIUMTRANSIENT,

    /* 75 */ CN_NODE,
    /* 76 */ CN_NODECURSOR,
  
    /* 77 */ CN_OP,

    /* 78 */ CN_PARSER,
    /* 79 */ CN_PARTITION,
    /* 80 */ CN_PARTITIONSCHEDULER,
    /* 81 */ CN_PARTITIONVEC,
    /* 82 */ CN_PATHVEC,
    /* 83 */ CN_PERFORMANCEBUFFER,
    /* 84 */ CN_PERFORMANCEENTRY,
    /* 85 */ CN_PERFORMANCETRACE,
    /* 86 */ CN_PLAN,
    /* 87 */ CN_PLANNER,
    /* 88 */ CN_PLANLIST,
    /* 89 */ CN_POSIXTHREAD,
    /* 90 */ CN_PREDICATE,
    /* 91 */ CN_PREDICATEVEC,
    /* 92 */ CN_PROXY,
    /* 93 */ CN_PROXYSET,
    /* 94 */ CN_PTNCOLVEC,
  
    /* 95 */ CN_QUEUE,
    /* 96 */ CN_QUEUEBUFFER,
    
    /* 97 */ CN_RBCURSOR,
    /*    */ CN_REGFRAME,
    
    /*101 */ CN_RING,
    /*102 */ CN_ROARRAY,
    /*103 */ CN_ROWBUFFER,
    /*104 */ CN_ROWCURSOR,
    /*105 */ CN_ROWONCE,
    /*106 */ CN_ROWWRK,
  
    /*107 */ CN_SCANBUFFER,
    /*108 */ CN_SCRATCH,
    /*109 */ CN_SEHISTORY,
    /*110 */ CN_SEITF,
    /*111 */ CN_SEITFCURSOR,
    /*112 */ CN_SEITFLIST,
    /*113 */ CN_SELCURSOR,
    /*114 */ CN_SEMAPHORE,
    /*115 */ CN_SEPROMPT,
    /*116 */ CN_SINCE,
    /*117 */ CN_SKIPCURSOR,
    /*118 */ CN_SKIPLIST,
    /*119 */ CN_SKIPNODE,
    /*120 */ CN_SLAB,
    /*121 */ CN_SLABALLOCATOR,
    /*122 */ CN_SLOTS,
    /*123 */ CN_SMALLPERSISTENT,
    /*124 */ CN_SMALLTRANSIENT,
    /*125 */ CN_SOCKET,
    /*126 */ CN_STACK,
    /*127 */ CN_STEPACCUM,
    /*128 */ CN_STEPARRAY,
    /*129 */ CN_SYSCALL,
    
    /*130 */ CN_TABLE,
    /*131 */ CN_TASK,
    /*132 */ CN_TASKCURSOR,
    /*133 */ CN_TASKLIST,
    /*134 */ CN_TASKQUEUE,
    /*135 */ CN_SERVERSCHEDULER,
    /*136 */ CN_TASKSET,
    /*137 */ CN_THREAD,
    /*138 */ CN_THREADCURSOR,
    /*139 */ CN_THREADLIST,
    /*140 */ CN_THREADQUEUE,
    /*141 */ CN_TIME,
    /*142 */ CN_TIMER,
    /*143 */ CN_TOKEN,
    /*144 */ CN_TOKENISER,
    /*145 */ CN_TOKENARY,
    /*146 */ CN_TOKENVEC,
    /*147 */ CN_TX,
    /*148 */ CN_TXSET,

    /*149 */ CN_WILDERNESS,
    /*150 */ CN_WORD,
    /*    */ CN_WORKINGBUFFER,
    /*    */ CN_WORKINGROW,
        
    /*151 */ CN_ZSMALL_,
    /*152 */ CN_ZMEDIUM_,
    /*153 */ CN_ZLARGE_,
    /*154 */ CN_ZJUMBO_,
  
    /*155 */ CN_ENUMS
  };
  
/*----------------------------------------------------------------------------*/
/* class names                                                                */
/*----------------------------------------------------------------------------*/
  static Stz* aCN[]
  {
    /*  0 */ "NONE"_Stz,
  
    /*  1 */ "BCDVAL"_Stz,
    /*  2 */ "BINVAL"_Stz,
    /*  3 */ "BOLVAL"_Stz,
    /*  4 */ "DTEVAL"_Stz,
    /*  5 */ "ENTWRK"_Stz,
    /*  6 */ "FLTVAL"_Stz,
    /*  7 */ "INTVAL"_Stz,
    /*  8 */ "IPVAL"_Stz,
    /*  9 */ "IVLVAL"_Stz,
    /* 10 */ "OBJVAL"_Stz,
    /* 11 */ "PTRVAL"_Stz,
    /* 12 */ "STRVAL"_Stz,
    /* 13 */ "STRVAR"_Stz,
    /* 14 */ "TMEVAL"_Stz,
    /* 15 */ "TSPVAL"_Stz,
    /* 16 */ "VALREG"_Stz,
    /* 17 */ "VALSET"_Stz,
    /* 18 */ "VALWRK"_Stz,
    
    /* 19 */ "ACCESSPATH"_Stz,
    /* 20 */ "ACCUM"_Stz,
    /* 21 */ "ACCUMVEC"_Stz,
    /* 22 */ "ARG"_Stz,
    /* 23 */ "ARGUMENTS"_Stz,
    /* 24 */ "ATOMIC"_Stz,
    /* 25 */ "ATTACHMAP"_Stz,
    
    /* 26 */ "BITARRAY"_Stz,
    /* 27 */ "BITMAP"_Stz,
    /* 28 */ "BUFFERPOOL"_Stz,
    /* 29 */ "BPA"_Stz,
  
    /* 30 */ "CARRYOVER"_Stz,
    /* 31 */ "COLUMN"_Stz,
    /* 32 */ "COLUMNVEC"_Stz,
    
    /* 33 */ "DIC"_Stz,
    /* 34 */ "DICPATH"_Stz,
    /* 35 */ "DIR"_Stz,
    
    /* 36 */ "ENTRY"_Stz,
    /* 37 */ "ENTRYBUFFER"_Stz,
    /* 38 */ "ENTRYVEC"_Stz,
  
    /* 39 */ "FILE"_Stz,
    /* 40 */ "FORMATTERS"_Stz,
    /* 41 */ "FREEBUFFER"_Stz,
    /* 42 */ "FREECHUNK"_Stz,
    /* 43 */ "FREEDNOT"_Stz,
    /* 44 */ "FREEENTRY"_Stz,
    
    /* 45 */ "GRAPH"_Stz,
    /* 46 */ "GRAPHCURSOR"_Stz,
    /* 47 */ "GRAPHNODE"_Stz,
    /* 48 */ "GRID"_Stz,
    /* 49 */ "GROUPBY"_Stz,
  
    /* 50 */ "HEAPBUFFER"_Stz,
    
    /* 51 */ "INDEX"_Stz,
    /* 52 */ "INDEXDATA"_Stz,
    /* 53 */ "IDXCOLVEC"_Stz,
    /* 54 */ "IDXDATAVEC"_Stz,
    /* 55 */ "IDXVEC"_Stz,
    /* 56 */ "INDEXLEVELS"_Stz,
    /* 57 */ "ITEM"_Stz,
    /* 58 */ "ITEMBUFFER"_Stz,

    /* 59 */ "JUMBOPERSISTENT"_Stz,
    /* 60 */ "JUMBOTRANSIENT"_Stz,
    
    /* 61 */ "KEY"_Stz,
    /* 62 */ "KEYVAL"_Stz,
    /* 63 */ "KEYVEC"_Stz,
    /* 64 */ "KEYWRK"_Stz,
  
    /* 65 */ "LARGEPERSISTENT"_Stz,
    /* 66 */ "LARGETRANSIENT"_Stz,
    
    /* 67 */ "LINEITEM"_Stz,
    /* 68 */ "LOADCHUNK"_Stz,
    /* 69 */ "LOADER"_Stz,
    /* 70 */ "LOADERSUB"_Stz,
    /* 71 */ "LOADMAP"_Stz,
    /* 72 */ "LOCK"_Stz,

    /* 73 */ "MEDIUMPERSISTENT"_Stz,
    /* 74 */ "MEDIUMTRANSIENT"_Stz,
    
    /* 75 */ "NODE"_Stz,
    /* 76 */ "NODECURSOR"_Stz,
  
    /* 77 */ "OP"_Stz,
  
    /* 78 */ "PARSER"_Stz,
    /* 79 */ "PARTITION"_Stz,
    /* 80 */ "PARTITIONSCHEDULER"_Stz,
    /* 81 */ "PARTITIONVEC"_Stz,
    /* 82 */ "PATHVEC"_Stz,
    /* 83 */ "PERFORMANCEBUFFER"_Stz,
    /* 84 */ "PERFORMANCEENTRY"_Stz,
    /* 85 */ "PERFORMANCETRACE"_Stz,
    /* 86 */ "PLAN"_Stz,
    /* 87 */ "PLANNER"_Stz,
    /* 88 */ "PLANLIST"_Stz,
    /* 89 */ "POSIXTHREAD"_Stz,
    /* 90 */ "PREDICATE"_Stz,
    /* 91 */ "PREDICATEVEC"_Stz,
    /* 92 */ "PROXY"_Stz,
    /* 93 */ "PROXYSET"_Stz,
    /* 94 */ "PTNCOLVEC"_Stz,
    
    /* 95 */ "QUEUE"_Stz,
    /* 96 */ "QUEUEBUFFER"_Stz,
  
    /* 97 */ "RBCURSOR"_Stz,
    /*    */ "REGFRAME"_Stz,
    
    /*101 */ "RING"_Stz,
    /*102 */ "ROARRAY"_Stz,
    /*103 */ "ROWBUFFER"_Stz,
    /*104 */ "ROWCURSOR"_Stz,
    /*105 */ "ROWONCE"_Stz,
    /*106 */ "ROWWRK"_Stz,
  
    /*107 */ "SCANBUFFER"_Stz,
    /*108 */ "SCRATCH"_Stz,
    /*109 */ "SEHISTORY"_Stz,
    /*110 */ "SEITF"_Stz,
    /*111 */ "SEITFCURSOR"_Stz,
    /*112 */ "SEITFLIST"_Stz,
    /*113 */ "SELCURSOR"_Stz,
    /*114 */ "SEMAPHORE"_Stz,
    /*115 */ "SEPROMPT"_Stz,
    /*116 */ "SINCE"_Stz,
    /*117 */ "SKIPCURSOR"_Stz,
    /*118 */ "SKIPLIST"_Stz,
    /*119 */ "SKIPNODE"_Stz,
    /*120 */ "SLAB"_Stz,
    /*121 */ "SLABALLOCATOR"_Stz,
    /*122 */ "SLOTS"_Stz,
    /*123 */ "SMALLPERSISTENT"_Stz,
    /*124 */ "SMALLTRANSIENT"_Stz,
    /*125 */ "SOCKET"_Stz,
    /*126 */ "STACK"_Stz,
    /*127 */ "STEPACCUM"_Stz,
    /*128 */ "STEPARRAY"_Stz,
    /*129 */ "SYSCALL"_Stz,
  
    /*130 */ "TABLE"_Stz,
    /*131 */ "TASK"_Stz,
    /*132 */ "TASKCURSOR"_Stz,
    /*133 */ "TASKLIST"_Stz,
    /*134 */ "TASKQUEUE"_Stz,
    /*135 */ "SERVERSCHEDULER"_Stz,
    /*136 */ "TASKSET"_Stz,
    /*137 */ "THREAD"_Stz,
    /*138 */ "THREADCURSOR"_Stz,
    /*139 */ "THREADLIST"_Stz,
    /*140 */ "THREADQUEUE"_Stz,
    /*141 */ "TIME"_Stz,
    /*142 */ "TIMER"_Stz,
    /*143 */ "TOKEN"_Stz,
    /*144 */ "TOKENISER"_Stz,
    /*145 */ "TOKENARY"_Stz,
    /*146 */ "TOKENVEC"_Stz,
    /*147 */ "TX"_Stz,
    /*148 */ "TXSET"_Stz,
    
    /*149 */ "WILDERNESS"_Stz,
    /*150 */ "WORD"_Stz,
    /*    */ "WORKINGBUFFER"_Stz,
    /*    */ "WORKINGROW"_Stz,
    
    /*151 */ "ZSMALL_"_Stz,
    /*152 */ "ZMEDIUM_"_Stz,
    /*153 */ "ZLARGE_"_Stz,
    /*154 */ "ZJUMBO_"_Stz,
  
    /*155 */ "ENUMS"_Stz
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
