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
  
  /*    */ &Finalize::vUndefined,         // CN_CARRYOVER
  /* 30 */ &Finalize::vUndefined,         // CN_COLUMN
  /* 31 */ &Finalize::vUndefined,         // CN_COLUMNVEC
  
  /* 32 */ &Dic::vFinalize,               // CN_DIC
  /* 33 */ &Finalize::vUndefined,         // CN_DICPATH
  /* 34 */ &Finalize::vUndefined,         // CN_DIR
  
  /* 35 */ &Finalize::vUndefined,         // CN_ENTRY
  /* 36 */ &Finalize::vUndefined,         // CN_ENTRYBUFFER
  /* 37 */ &Finalize::vUndefined,         // CN_ENTRYVEC

  /* 38 */ &Finalize::vUndefined,         // CN_FILE
  /*    */ &Finalize::vUndefined,         // CN_FORMATTERS
  /* 39 */ &Finalize::vUndefined,         // CN_FREEBUFFER
  /* 40 */ &Finalize::vUndefined,         // CN_FREECHUNK
  /* 41 */ &Finalize::vUndefined,         // CN_FREEDNOT
  
  /* 42 */ &Finalize::vUndefined,         // CN_GRAPH
  /* 43 */ &Finalize::vUndefined,         // CN_GRAPH
  /* 44 */ &Finalize::vUndefined,         // CN_GRAPHNODE
  /* 45 */ &Finalize::vUndefined,         // CN_GRID
  /* 46 */ &Finalize::vUndefined,         // CN_GROUPBY
  
  /* 47 */ &Finalize::vUndefined,         // CN_HEAPBUFFER
  
  /* 48 */ &Index::vFinalize,             // CN_INDEX
  /* 49 */ &Finalize::vUndefined,         // CN_INDEXDATA
  /* 50 */ &Finalize::vUndefined,         // CN_IDXCOLVEC
  /* 51 */ &Finalize::vUndefined,         // CN_IDXDATAVEC
  /* 52 */ &Finalize::vUndefined,         // CN_IDXVEC
  /* 53 */ &Finalize::vUndefined,         // CN_INDEXLEVELS
  
  /*    */ &Finalize::vUndefined,         // CN_JUMBOPERSISTENT
  /* 54 */ &Finalize::vUndefined,         // CN_JUMBOTRANSIENT
  
  /* 55 */ &Finalize::vUndefined,         // CN_KEY
  /* 56 */ &Finalize::vUndefined,         // CN_KEYVAL
  /* 57 */ &Finalize::vUndefined,         // CN_KEYVEC
  /* 58 */ &Finalize::vUndefined,         // CN_KEYWRK
  
  /*    */ &Finalize::vUndefined,         // CN_LARGEPERSISTENT
  /*    */ &Finalize::vUndefined,         // CN_LARGETRANSIENT
  
  /* 59 */ &Finalize::vUndefined,         // CN_LINEITEM
  /* 60 */ &Finalize::vUndefined,         // CN_LOADCHUNK
  /* 61 */ &Loader::vFinalize,            // CN_LOADER
  /* 62 */ &Finalize::vUndefined,         // CN_LOADERSUB
  /* 63 */ &Finalize::vUndefined,         // CN_LOADMAP
  /* 64 */ &Finalize::vUndefined,         // CN_LOCK
  
  /*    */ &Finalize::vUndefined,         // CN_MEDIUMPERSISTENT
  /*    */ &Finalize::vUndefined,         // CN_MEDIUMTRANSIENT
  
  /* 66 */ &Finalize::vUndefined,         // CN_NODE
  /* 67 */ &Finalize::vUndefined,         // CN_NODECURSOR
  
  /* 68 */ &Finalize::vUndefined,         // CN_OP
  
  /* 69 */ &Finalize::vUndefined,         // CN_PARSER
  /* 70 */ &Finalize::vUndefined,         // CN_PARTITION
  /* 71 */ &Finalize::vUndefined,         // CN_PARTITIONSCHEDULER
  /* 72 */ &Finalize::vUndefined,         // CN_PARTITIONVEC
  /* 73 */ &Finalize::vUndefined,         // CN_PATHVEC
  /*    */ &Finalize::vUndefined,         // CN_PERFORMANCETRACE
  /* 74 */ &Finalize::vUndefined,         // CN_PLAN
  /* 75 */ &Finalize::vUndefined,         // CN_PLANNER
  /* 76 */ &Finalize::vUndefined,         // CN_PLANLIST
  /* 77 */ &Finalize::vUndefined,         // CN_POSIXTHREAD
  /* 78 */ &Finalize::vUndefined,         // CN_PREDICATE
  /* 79 */ &Finalize::vUndefined,         // CN_PREDICATEVEC
  /* 80 */ &Finalize::vUndefined,         // CN_PROXY
  /* 81 */ &Finalize::vUndefined,         // CN_PROXYSET
  /* 82 */ &Finalize::vUndefined,         // CN_PTNCOLVEC
  
  /* 83 */ &Finalize::vUndefined,         // CN_QUEUE
  /* 84 */ &Finalize::vUndefined,         // CN_QUEUEBUFFER
  
  /* 85 */ &Finalize::vUndefined,         // CN_RBCURSOR
  /* 86 */ &Finalize::vUndefined,         // CN_REGFRAME
  /* 87 */ &Finalize::vUndefined,         // CN_RING
  /* 88 */ &Finalize::vUndefined,         // CN_ROARRAY
  /* 89 */ &Finalize::vUndefined,         // CN_ROWBUFFER
  /* 90 */ &Finalize::vUndefined,         // CN_ROWCURSOR
  /* 91 */ &Finalize::vUndefined,         // CN_ROWONCE
  
  /* 92 */ &Finalize::vUndefined,         // CN_SCANBUFFER
  /* 93 */ &Finalize::vUndefined,         // CN_SCRATCH
  /* 94 */ &Finalize::vUndefined,         // CN_SEHISTORY
  /* 95 */ &Finalize::vUndefined,         // CN_SEITF
  /* 96 */ &Finalize::vUndefined,         // CN_SEITFCURSOR
  /* 97 */ &Finalize::vUndefined,         // CN_SEITFLIST
  /* 98 */ &SelCursor::vFinalize,         // CN_SELCURSOR
  /* 99 */ &Finalize::vUndefined,         // CN_SEMAPHORE
  /*100 */ &Finalize::vUndefined,         // CN_SEPROMPT
  /*101 */ &Finalize::vUndefined,         // CN_SINCE
  /*102 */ &Finalize::vUndefined,         // CN_SKIPCURSOR
  /*103 */ &Finalize::vUndefined,         // CN_SKIPLIST
  /*104 */ &Finalize::vUndefined,         // CN_SKIPNODE
  /*105 */ &Finalize::vUndefined,         // CN_SLAB
  /*106 */ &Finalize::vUndefined,         // CN_SLABALLOCATOR
  /*107 */ &Slots::vFinalize,             // CN_SLOTS
  /*    */ &Finalize::vUndefined,         // SN_SMALLPERSISTENT
  /*108 */ &Finalize::vUndefined,         // SN_SMALLTRANSIENT
  /*109 */ &Finalize::vUndefined,         // CN_SOCKET
  /*110 */ &Finalize::vUndefined,         // CN_STACK
  /*111 */ &Finalize::vUndefined,         // CN_STEPACCUM
  /*112 */ &Finalize::vUndefined,         // CN_STEPARRAY
  /*113 */ &Finalize::vUndefined,         // CN_SYSCALL
  
  /*114 */ &Table::vFinalize,             // CN_TABLE
  /*115 */ &Finalize::vUndefined,         // CN_TASK
  /*116 */ &Finalize::vUndefined,         // CN_TASKCURSOR
  /*117 */ &Finalize::vUndefined,         // CN_TASKLIST
  /*118 */ &Finalize::vUndefined,         // CN_TASKQUEUE
  /*119 */ &Finalize::vUndefined,         // CN_SERVERSCHEDULER
  /*120 */ &Finalize::vUndefined,         // CN_TASKSET
  /*121 */ &Finalize::vUndefined,         // CN_THREAD
  /*122 */ &Finalize::vUndefined,         // CN_THREADCURSOR
  /*123 */ &Finalize::vUndefined,         // CN_THREADLIST
  /*124 */ &Finalize::vUndefined,         // CN_THREADQUEUE
  /*125 */ &Finalize::vUndefined,         // CN_TIME
  /*126 */ &Finalize::vUndefined,         // CN_TIMER
  /*127 */ &Finalize::vUndefined,         // CN_TOKEN
  /*128 */ &Finalize::vUndefined,         // CN_TOKENISER
  /*129 */ &Finalize::vUndefined,         // CN_TOKENARY
  /*130 */ &Finalize::vUndefined,         // CN_TOKENVEC
  /*131 */ &Finalize::vUndefined,         // CN_TX
  /*132 */ &Finalize::vUndefined,         // CN_TXSET
  
  /*133 */ &Finalize::vUndefined,         // CN_WORD
  
  /*134 */ &Finalize::vUndefined,         // CN_ZSMALL_
  /*135 */ &Finalize::vUndefined,         // CN_ZMEDIUM_
  /*136 */ &Finalize::vUndefined,         // CN_ZLARGE_
  /*137 */ &Finalize::vUndefined,         // CN_ZJUMBO_
  
  /*138 */ &Finalize::vEnums              // CN_ENUMS
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
