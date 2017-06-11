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
  /* 18 */ &Finalize::vUndefined,         // CN_VALWSP
  
  /*    */ &Finalize::vUndefined,         // CN_ACCESSPATH
  /* 12 */ &Finalize::vUndefined,         // CN_ARGUMENTS
  /* 13 */ &Finalize::vUndefined,         // CN_ATOMIC
  /* 14 */ &Finalize::vUndefined,         // CN_ATTACHMAP
  
  /* 15 */ &Finalize::vUndefined,         // CN_BUFFERPOOL
  /* 16 */ &Finalize::vUndefined,         // CN_BPA
  
  /*    */ &Finalize::vUndefined,         // CN_COLUMN
  /*    */ &Finalize::vUndefined,         // CN_COLUMNVEC
  
  /* 17 */ &Finalize::vUndefined,         // CN_DATA
  /* 18 */ &Dic::vFinalize,               // CN_DIC
  /* 19 */ &Finalize::vUndefined,         // CN_DICPATH
  /* 20 */ &Finalize::vUndefined,         // CN_DIR
  
  /* 21 */ &Finalize::vUndefined,         // CN_ENTRY
  /*    */ &Finalize::vUndefined,         // CN_ENTRYVEC
  
  /* 22 */ &Finalize::vUndefined,         // CN_FILE
  /* 23 */ &Finalize::vUndefined,         // CN_FREEBUFFER
  /* 24 */ &Finalize::vUndefined,         // CN_FREECHUNK
  /*    */ &Finalize::vUndefined,         // CN_FREEDNOT
  
  /* 25 */ &Finalize::vUndefined,         // CN_GRAPH
  /* 26 */ &Finalize::vUndefined,         // CN_GRAPH
  /* 27 */ &Finalize::vUndefined,         // CN_GRAPHNODE
  /* 28 */ &Finalize::vUndefined,         // CN_GRID
  
  /* 29 */ &Finalize::vUndefined,         // CN_HEAPBUFFER
  
  /* 30 */ &Index::vFinalize,             // CN_INDEX
  /* 31 */ &Finalize::vUndefined,         // CN_INDEXBUFFER
  /*    */ &Finalize::vUndefined,         // CN_INDEXDATA
  /*    */ &Finalize::vUndefined,         // CN_IDXCOLVEC
  /*    */ &Finalize::vUndefined,         // CN_IDXVEC
  /* 33 */ &Finalize::vUndefined,         // CN_INDEXLEVELS
  
  /* 34 */ &Finalize::vUndefined,         // CN_JUMBOHEAP
  
  /*    */ &Finalize::vUndefined,         // CN_KEYVAL
  /*    */ &Finalize::vUndefined,         // CN_KEYVEC
  /* 35 */ &Finalize::vUndefined,         // CN_KEYWRK
  
  /* 36 */ &Finalize::vUndefined,         // CN_LOADCHUNK
  /* 37 */ &Loader::vFinalize,            // CN_LOADER
  /* 38 */ &Finalize::vUndefined,         // CN_LOADERSUB
  /* 39 */ &Finalize::vUndefined,         // CN_LOADMAP
  /* 40 */ &Finalize::vUndefined,         // CN_LOCK
  
  /* 41 */ &Finalize::vUndefined,         // CN_MEDIUMHEAP
  
  /* 42 */ &Finalize::vUndefined,         // CN_NODE
  /* 43 */ &Finalize::vUndefined,         // CN_NODECURSOR
  
  /* 44 */ &Finalize::vUndefined,         // CN_PARSER
  /* 45 */ &Finalize::vUndefined,         // CN_PARTITION
  /* 46 */ &Finalize::vUndefined,         // CN_PLAN
  /* 47 */ &Finalize::vUndefined,         // CN_PLANLIST
  /*    */ &Finalize::vUndefined,         // CN_POSIXTHREAD
  /* 48 */ &Finalize::vUndefined,         // CN_PROXY
  /* 49 */ &Finalize::vUndefined,         // CN_PROXYSET
  /*    */ &Finalize::vUndefined,         // CN_PTNCOLVEC
  
  /* 50 */ &Finalize::vUndefined,         // CN_QUEUE
  /* 51 */ &Finalize::vUndefined,         // CN_QUEUEBUFFER
  
  /* 52 */ &Finalize::vUndefined,         // CN_RBCURSOR
  /* 53 */ &Finalize::vUndefined,         // CN_REGFRAME
  /* 54 */ &Finalize::vUndefined,         // CN_RING
  /*    */ &Finalize::vUndefined,         // CN_ROARRAY
  /* 55 */ &Finalize::vUndefined,         // CN_ROW
  /* 56 */ &Finalize::vUndefined,         // CN_ROWBUFFER
  /* 57 */ &Finalize::vUndefined,         // CN_ROWCURSOR
  /* 58 */ &Finalize::vUndefined,         // CN_EBSTRAND
  /* 59 */ &Finalize::vUndefined,         // CN_ROWWSP
  
  /* 60 */ &Finalize::vUndefined,         // CN_SCANBUFFER
  /* 61 */ &Finalize::vUndefined,         // CN_SCRATCH
  /* 62 */ &Finalize::vUndefined,         // CN_SEHISTORY
  /* 63 */ &Finalize::vUndefined,         // CN_SEITF
  /* 64 */ &Finalize::vUndefined,         // CN_SEITFCURSOR
  /* 65 */ &Finalize::vUndefined,         // CN_SEITFLIST
  /* 66 */ &SelCursor::vFinalize,         // CN_SELCURSOR
  /* 67 */ &Finalize::vUndefined,         // CN_SEPROMPT
  /* 68 */ &Finalize::vUndefined,         // CN_SINCE
  /* 69 */ &Finalize::vUndefined,         // CN_SKIPCURSOR
  /* 70 */ &Finalize::vUndefined,         // CN_SKIPLIST
  /* 71 */ &Finalize::vUndefined,         // CN_SKIPNODE
  /* 72 */ &Slots::vFinalize,             // CN_SLOTS
  /* 73 */ &Finalize::vUndefined,         // CN_SMALLHEAP
  /* 74 */ &Finalize::vUndefined,         // CN_SOCKET
  /* 75 */ &Finalize::vUndefined,         // CN_STACK
  /* 76 */ &Finalize::vUndefined,         // CN_STEPACCUM
  /* 77 */ &Finalize::vUndefined,         // CN_STEPARRAY
  /* 78 */ &Finalize::vUndefined,         // CN_SYSCALL
  
  /* 79 */ &Table::vFinalize,             // CN_TABLE
  /* 80 */ &Finalize::vUndefined,         // CN_TASK
  /* 81 */ &Finalize::vUndefined,         // CN_TASKCURSOR
  /* 82 */ &Finalize::vUndefined,         // CN_TASKLIST
  /* 83 */ &Finalize::vUndefined,         // CN_TASKSET
  /* 84 */ &Finalize::vUndefined,         // CN_THREAD
  /* 85 */ &Finalize::vUndefined,         // CN_THREADCURSOR
  /* 86 */ &Finalize::vUndefined,         // CN_THREADLIST
  /* 87 */ &Finalize::vUndefined,         // CN_TIME
  /* 88 */ &Finalize::vUndefined,         // CN_TIMER
  /* 89 */ &Finalize::vUndefined,         // CN_TOKENISER
  /*    */ &Finalize::vUndefined,         // CN_TOKENARY
  /*    */ &Finalize::vUndefined,         // CN_TOKENSET
  /* 90 */ &Finalize::vUndefined,         // CN_TX
  /* 91 */ &Finalize::vUndefined,         // CN_TXARRAY
  /* 92 */ &Finalize::vUndefined,         // CN_TXCOLLECTION
  /* 93 */ &Finalize::vUndefined,         // CN_TXCURSOR
  /* 94 */ &Finalize::vUndefined,         // CN_TXMAP
  /* 95 */ &TxRef::vFinalize,             // CN_TXREF
  
  /* 96 */ &Finalize::vUndefined,         // CN_WORD
  
  /* 97 */ &Finalize::vUndefined,         // CN_ZSMALL_
  /* 98 */ &Finalize::vUndefined,         // CN_ZMEDIUM_
  /* 99 */ &Finalize::vUndefined,         // CN_ZLARGE_
  /*100 */ &Finalize::vUndefined,         // CN_ZJUMBO_
  
  /*101 */ &Finalize::vEnums              // CN_ENUMS
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
  vTrace("%s: **finalize function not defined for class**\n"_Str, aCN[eClass]);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
