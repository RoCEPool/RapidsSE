/*============================================================================*/
/* Finalize.cpp -- Embedded Class Finalizers                                  */
/*============================================================================*/

#include "StorageEngine.h"
#include "SePrompt.hpp"

// Object's : Class::vFinalize
// Object's with no vUndefined defined :Finalize::vUndefined
// all embedded objects should have a vFinalize

pFINALIZE *gpFinalize[]
{
  /*  0 */ &Finalize::vUndefined,         // CN_NONE
  
  
  /*  1 */ &Finalize::vUndefined,         // CN_BCDVAL
  /*  2 */ &Finalize::vUndefined,         // CN_BINVAL
  /*  3 */ &Finalize::vUndefined,         // CN_BOLVAL
  /*  4 */ &Finalize::vUndefined,         // CN_DTEVAL
  /*  5 */ &Finalize::vUndefined,         // CN_ENTWRK
  /*  6 */ &Finalize::vUndefined,         // CN_FLTVAL
  /*  7 */ &Finalize::vUndefined,         // CN_INTVAL
  /*  8 */ &Finalize::vUndefined,         // CN_IPVAL
  /*  9 */ &Finalize::vUndefined,         // CN_IVLVAL
  /* 10 */ &Finalize::vUndefined,         // CN_OBJVAL
  /* 11 */ &Finalize::vUndefined,         // CN_PTRVAL
  /* 12 */ &Finalize::vUndefined,         // CN_STRVAL
  /* 13 */ &Finalize::vUndefined,         // CN_STRVAR
  /* 14 */ &Finalize::vUndefined,         // CN_TMEVAL
  /* 15 */ &Finalize::vUndefined,         // CN_TSPVAL
  /* 16 */ &Finalize::vUndefined,         // CN_VALREG
  /* 17 */ &Finalize::vUndefined,         // CN_VALSET
  /* 18 */ &Finalize::vUndefined,         // CN_VALWRK
  
  /* 19 */ &Finalize::vUndefined,         // CN_ACCESSPATH
  /* 20 */ &Finalize::vUndefined,         // CN_ACCUM
  /* 21 */ &Finalize::vUndefined,         // CN_ACCUMVEC
  /* 22 */ &Finalize::vUndefined,         // CN_ARG
  /* 23 */ &Finalize::vUndefined,         // CN_ARGUMENTS
  /* 24 */ &Finalize::vUndefined,         // CN_ATOMIC
  /* 25 */ &Finalize::vUndefined,         // CN_ATTACHMAP
  
  /* 26 */ &Finalize::vUndefined,         // CN_BITARRAY
  /* 27 */ &Finalize::vUndefined,         // CN_BITMAP
  /* 28 */ &Finalize::vUndefined,         // CN_BUFFERPOOL
  /* 29 */ &Finalize::vUndefined,         // CN_BPA
  
  /* 30 */ &Finalize::vUndefined,         // CN_CARRYOVER
  /* 31 */ &Finalize::vUndefined,         // CN_COLUMN
  /* 32 */ &Finalize::vUndefined,         // CN_COLUMNVEC
  
  /* 33 */ &Dic::vFinalize,               // CN_DIC
  /* 34 */ &Finalize::vUndefined,         // CN_DICPATH
  /* 35 */ &Finalize::vUndefined,         // CN_DIR
  
  /* 36 */ &Finalize::vUndefined,         // CN_ENTRY
  /* 37 */ &Finalize::vUndefined,         // CN_ENTRYBUFFER
  /* 38 */ &Finalize::vUndefined,         // CN_ENTRYVEC

  /* 39 */ &Finalize::vUndefined,         // CN_FILE
  /* 40 */ &Finalize::vUndefined,         // CN_FORMATTERS
  /* 41 */ &Finalize::vUndefined,         // CN_FREEBUFFER
  /* 42 */ &Finalize::vUndefined,         // CN_FREECHUNK
  /* 43 */ &Finalize::vUndefined,         // CN_FREEDNOT
  
  /* 44 */ &Finalize::vUndefined,         // CN_GRAPH
  /* 45 */ &Finalize::vUndefined,         // CN_GRAPH
  /* 46 */ &Finalize::vUndefined,         // CN_GRAPHNODE
  /* 47 */ &Finalize::vUndefined,         // CN_GRID
  /* 48 */ &Finalize::vUndefined,         // CN_GROUPBY
  
  /* 49 */ &Finalize::vUndefined,         // CN_HEAPBUFFER
  
  /* 50 */ &Index::vFinalize,             // CN_INDEX
  /* 51 */ &Finalize::vUndefined,         // CN_INDEXDATA
  /* 52 */ &Finalize::vUndefined,         // CN_IDXCOLVEC
  /* 53 */ &Finalize::vUndefined,         // CN_IDXDATAVEC
  /* 54 */ &Finalize::vUndefined,         // CN_IDXVEC
  /* 55 */ &Finalize::vUndefined,         // CN_INDEXLEVELS
  /* 56 */ &Finalize::vUndefined,         // CN_ITEM
  /* 57 */ &Finalize::vUndefined,         // CN_ITEMBUFFER
  
  /* 58 */ &Finalize::vUndefined,         // CN_JUMBOPERSISTENT
  /* 59 */ &Finalize::vUndefined,         // CN_JUMBOTRANSIENT
  
  /* 60 */ &Finalize::vUndefined,         // CN_KEY
  /* 61 */ &Finalize::vUndefined,         // CN_KEYVAL
  /* 62 */ &Finalize::vUndefined,         // CN_KEYVEC
  /* 63 */ &Finalize::vUndefined,         // CN_KEYWRK
  
  /* 64 */ &Finalize::vUndefined,         // CN_LARGEPERSISTENT
  /* 65 */ &Finalize::vUndefined,         // CN_LARGETRANSIENT
  
  /* 66 */ &Finalize::vUndefined,         // CN_LINEITEM
  /* 67 */ &Finalize::vUndefined,         // CN_LOADCHUNK
  /* 68 */ &Loader::vFinalize,            // CN_LOADER
  /* 69 */ &Finalize::vUndefined,         // CN_LOADERSUB
  /* 70 */ &Finalize::vUndefined,         // CN_LOADMAP
  /* 71 */ &Finalize::vUndefined,         // CN_LOCK
  
  /* 72 */ &Finalize::vUndefined,         // CN_MEDIUMPERSISTENT
  /* 73 */ &Finalize::vUndefined,         // CN_MEDIUMTRANSIENT
  
  /* 74 */ &Finalize::vUndefined,         // CN_NODE
  /* 75 */ &Finalize::vUndefined,         // CN_NODECURSOR
  
  /* 76 */ &Finalize::vUndefined,         // CN_OP
  
  /* 77 */ &Finalize::vUndefined,         // CN_PARSER
  /* 78 */ &Finalize::vUndefined,         // CN_PARTITION
  /* 79 */ &Finalize::vUndefined,         // CN_PARTITIONSCHEDULER
  /* 80 */ &Finalize::vUndefined,         // CN_PARTITIONVEC
  /* 81 */ &Finalize::vUndefined,         // CN_PATHVEC
  /* 82 */ &Finalize::vUndefined,         // CN_PERFORMANCEBUFFER
  /* 83 */ &Finalize::vUndefined,         // CN_PERFORMANCEENTRY
  /* 84 */ &Finalize::vUndefined,         // CN_PERFORMANCETRACE
  /* 85 */ &Finalize::vUndefined,         // CN_PLAN
  /* 86 */ &Finalize::vUndefined,         // CN_PLANNER
  /* 87 */ &Finalize::vUndefined,         // CN_PLANLIST
  /* 88 */ &Finalize::vUndefined,         // CN_POSIXTHREAD
  /* 89 */ &Finalize::vUndefined,         // CN_PREDICATE
  /* 90 */ &Finalize::vUndefined,         // CN_PREDICATEVEC
  /* 91 */ &Finalize::vUndefined,         // CN_PROXY
  /* 92 */ &Finalize::vUndefined,         // CN_PROXYSET
  /* 93 */ &Finalize::vUndefined,         // CN_PTNCOLVEC
  
  /* 94 */ &Finalize::vUndefined,         // CN_QUEUE
  /* 95 */ &Finalize::vUndefined,         // CN_QUEUEBUFFER
  
  /* 96 */ &Finalize::vUndefined,         // CN_RBCURSOR
  /* 97 */ &Finalize::vUndefined,         // CN_REGFRAME
  /* 98 */ &Finalize::vUndefined,         // CN_RESULTBUFFER
  /* 99 */ &Finalize::vUndefined,         // CN_RESULTROW
  /*100 */ &Finalize::vUndefined,         // CN_RING
  /*101 */ &Finalize::vUndefined,         // CN_ROARRAY
  /*102 */ &Finalize::vUndefined,         // CN_ROWBUFFER
  /*103 */ &Finalize::vUndefined,         // CN_ROWCURSOR
  /*104 */ &Finalize::vUndefined,         // CN_ROWONCE
  
  /*105 */ &Finalize::vUndefined,         // CN_SCANBUFFER
  /*106 */ &Finalize::vUndefined,         // CN_SCRATCH
  /*107 */ &Finalize::vUndefined,         // CN_SEHISTORY
  /*108 */ &Finalize::vUndefined,         // CN_SEITF
  /*109 */ &Finalize::vUndefined,         // CN_SEITFCURSOR
  /*110 */ &Finalize::vUndefined,         // CN_SEITFLIST
  /*111 */ &SelCursor::vFinalize,         // CN_SELCURSOR
  /*112 */ &Finalize::vUndefined,         // CN_SEMAPHORE
  /*113 */ &Finalize::vUndefined,         // CN_SEPROMPT
  /*114 */ &Finalize::vUndefined,         // CN_SINCE
  /*115 */ &Finalize::vUndefined,         // CN_SKIPCURSOR
  /*116 */ &Finalize::vUndefined,         // CN_SKIPLIST
  /*117 */ &Finalize::vUndefined,         // CN_SKIPNODE
  /*118 */ &Finalize::vUndefined,         // CN_SLAB
  /*119 */ &Finalize::vUndefined,         // CN_SLABALLOCATOR
  /*120 */ &Slots::vFinalize,             // CN_SLOTS
  /*121 */ &Finalize::vUndefined,         // SN_SMALLPERSISTENT
  /*122 */ &Finalize::vUndefined,         // SN_SMALLTRANSIENT
  /*123 */ &Finalize::vUndefined,         // CN_SOCKET
  /*124 */ &Finalize::vUndefined,         // CN_STACK
  /*125 */ &Finalize::vUndefined,         // CN_STEPACCUM
  /*126 */ &Finalize::vUndefined,         // CN_STEPARRAY
  /*127 */ &Finalize::vUndefined,         // CN_SYSCALL
  
  /*128 */ &Table::vFinalize,             // CN_TABLE
  /*129 */ &Finalize::vUndefined,         // CN_TASK
  /*130 */ &Finalize::vUndefined,         // CN_TASKCURSOR
  /*131 */ &Finalize::vUndefined,         // CN_TASKLIST
  /*132 */ &Finalize::vUndefined,         // CN_TASKQUEUE
  /*133 */ &Finalize::vUndefined,         // CN_SERVERSCHEDULER
  /*134 */ &Finalize::vUndefined,         // CN_TASKSET
  /*135 */ &Finalize::vUndefined,         // CN_THREAD
  /*136 */ &Finalize::vUndefined,         // CN_THREADCURSOR
  /*137 */ &Finalize::vUndefined,         // CN_THREADLIST
  /*138 */ &Finalize::vUndefined,         // CN_THREADQUEUE
  /*139 */ &Finalize::vUndefined,         // CN_TIME
  /*140 */ &Finalize::vUndefined,         // CN_TIMER
  /*141 */ &Finalize::vUndefined,         // CN_TOKEN
  /*142 */ &Finalize::vUndefined,         // CN_TOKENISER
  /*143 */ &Finalize::vUndefined,         // CN_TOKENARY
  /*144 */ &Finalize::vUndefined,         // CN_TOKENVEC
  /*145 */ &Finalize::vUndefined,         // CN_TX
  /*146 */ &Finalize::vUndefined,         // CN_TXSET
  
  /*147 */ &Finalize::vUndefined,         // CN_WORD
  
  /*148 */ &Finalize::vUndefined,         // CN_ZSMALL_
  /*149 */ &Finalize::vUndefined,         // CN_ZMEDIUM_
  /*150 */ &Finalize::vUndefined,         // CN_ZLARGE_
  /*151 */ &Finalize::vUndefined,         // CN_ZJUMBO_
  
  /*152 */ &Finalize::vEnums              // CN_ENUMS
};

/*----------------------------------------------------------------------------*/
/* dump enums                                                                 */
/*----------------------------------------------------------------------------*/
void Finalize::vEnums(eCN eClass, Object* pObject)
{
  ASSERT(eClass == CN_ENUMS);
  ASSERT(pObject != nullptr);
  // do nothing
}

/*----------------------------------------------------------------------------*/
/* finalize not defined                                                       */
/*----------------------------------------------------------------------------*/
void Finalize::vUndefined(eCN eClass, Object* pObject)
{
  ASSERT(eClass != CN_NONE);
  vTrace("%s: **finalize function not defined for class**\n"_Stz, aCN[eClass]);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
