/*============================================================================*/
/* Dump.cpp -- Class Dumps                                                    */
/*============================================================================*/

#include "StorageEngine.h"

// Object's : Class::vDump
// Object's with no vDump defined : Dump::vUndefined
// Header : Dump::vInvalid
// Objects we don't want to dump (eg Word) : Dump::vDummy

pDUMPFUN *gpDump[]
{
  /*  0 */ &Dump::vInvalid,    // CN_NONE
  
  /*  1 */ &Dump::vInvalid,    // CN_S32
  /*  2 */ &Dump::vInvalid,    // CN_S256
  /*  3 */ &Dump::vInvalid,    // CN_S8K
  /*  4 */ &Dump::vInvalid,    // CN_INTVAL
  /*  5 */ &Dump::vInvalid,    // CN_OBJVAL
  /*  6 */ &Dump::vInvalid,    // CN_PTRVAL
  /*  7 */ &Dump::vInvalid,    // CN_STRVAL
  /*  8 */ &Dump::vInvalid,    // CN_VALREG
  /*  9 */ &Dump::vInvalid,    // CN_VALSET
  /* 10 */ &Dump::vInvalid,    // CN_VALWSP
  
  /* 11 */ &Dump::vInvalid,    // CN_ARGUMENTS
  /* 12 */ &Dump::vInvalid,    // CN_ATOMIC
  /* 13 */ &AttachMap::vDump,          // CN_ATTACHMAP
  
  /* 14 */ &Dump::vDummy,      // CN_BUFFER
  /* 15 */ &Dump::vInvalid,    // CN_BUFFERPOOL
  /* 16 */ &Dump::vInvalid,    // CN_BUFFERPOOLANALYZER

  /* 17 */ &Dictionary::vDump,         // CN_DICTIONARY
  /* 18 */ &DictionaryStack::vDump,    // CN_DICTIONARYSTACK
  /* 19 */ &Dump::vInvalid,    // CN_DIR
  
  /* 20 */ &Dump::vInvalid,    // CN_FILE
  /* 21 */ &Dump::vDummy,      // CN_FREECHUNK
  
  /* 22 */ &Grid::vDump,               // CN_GRID

  /* 23 */ &HeapBuffer::vDump,         // CN_HEAPBUFFER
  
  /* 24 */ &Index::vDump,              // CN_INDEX
  
  /* 25 */ &Dump::vInvalid,    // CN_JUMBOHEAP

  /* 26 */ &KeyCpp::vDump,             // CN_KEYCPP
  /* 27 */ &KeyVal::vDump,             // CN_KEYVAL
  
  /*    */ &LoadChunk::vDump,          // CN_LOADCHUNK
  /* 28 */ &Loader::vDump,             // CN_LOADER
  /* 29 */ &LoaderSub::vDump,          // CN_LOADERSUB
  /* 30 */ &Dump::vInvalid,    // CN_LOCK
  
  /* 31 */ &Heap::vDump,               // CN_MEDIUMHEAP
  
  /* 32 */ &Node::vDump,               // CN_NODE
  /* 33 */ &NodeCursor::vDump,         // CN_NODECURSOR
  
  /* 34 */ &Dump::vDummy,      // CN_PARSER
  /* 35 */ &Partition::vDump,          // CN_PARTITION
  /* 36 */ &Dump::vUndefined,  // CN_PARTITIONS
  /* 37 */ &Dump::vUndefined,  // CN_PROXY
  /* 38 */ &Dump::vUndefined,  // CN_PROXYSET

  /* 39 */ &Queue::vDump,              // CN_QUEUE
  /* 40 */ &QueueBuffer::vDump,        // CN_QUEUEBUFFER
  
  /* 41 */ &RegFrame::vDump,           // CN_REGFRAME
  /* 42 */ &Dump::vUndefined,  // CN_RING
  /* 43 */ &Dump::vUndefined,  // CN_ROW
  /* 44 */ &RowBuffer::vDump,          // CN_ROWBUFFER
  /* 45 */ &Dump::vUndefined,  // CN_ROWCURSOR
  /* 46 */ &RowStrand::vDump,          // CN_ROWSTRAND
  /* 47 */ &Dump::vUndefined,  // CN_ROWWSP
  
  /* 48 */ &Dump::vUndefined,  // CN_SCANBUFFER
  /* 49 */ &Dump::vUndefined,  // CN_SEHISTORY
  /* 50 */ &SEitf::vDump,              // CN_SEITF
  /* 51 */ &Dump::vUndefined,  // CN_SEITFCURSOR
  /* 52 */ &Dump::vUndefined,  // CN_SEITFLIST
  /* 53 */ &Dump::vDummy,      // CN_SEPROMPT
  /* 54 */ &Dump::vUndefined,  // CN_SINCE
  /* 55 */ &Dump::vUndefined,  // CN_SKIPCURSOR
  /* 56 */ &Dump::vUndefined,  // CN_SKIPLIST
  /* 57 */ &LoadMap::vDump,            // CN_LOADMAP
  /* 58 */ &Dump::vUndefined,  // CN_SKIPNODE
  /* 59 */ &Heap::vDump,               // CN_SMALLHEAP
  /* 60 */ &Dump::vUndefined,  // CN_SOCKET
  /* 61 */ &Dump::vUndefined,  // CN_STACK
  /* 62 */ &Dump::vInvalid,    // CN_STEPACCUM
  /* 63 */ &StepArray::vDump,          // CN_STEPARRAY
  /* 64 */ &Dump::vInvalid,    // CN_SYSCALL
  
  /* 65 */ &Table::vDump,              // CN_TABLE
  /* 66 */ &Task::vDump,               // CN_TASK
  /* 67 */ &Dump::vUndefined,  // CN_TASKCURSOR
  /* 68 */ &TaskList::vDump,           // CN_TASKLIST
  /* 69 */ &Dump::vUndefined,  // CN_TASKSET
  /* 70 */ &Dump::vUndefined,  // CN_THREAD
  /* 71 */ &Dump::vUndefined,  // CN_THREADCURSOR
  /* 72 */ &Dump::vUndefined,  // CN_THREADLIST
  /* 73 */ &Dump::vUndefined,  // CN_TIME
  /* 74 */ &Dump::vInvalid,    // CN_TIMER
  /* 75 */ &Tokeniser::vDump,          // CN_TOKENISER
  /* 76 */ &Tx::vDump,                 // CN_TX
  /* 77 */ &TxArray::vDump,            // CN_TXARRAY
  /* 78 */ &Dump::vUndefined,  // CN_TXCOLLECTION
  /* 79 */ &Dump::vUndefined,  // CN_TXCURSOR
  /* 80 */ &Dump::vUndefined,  // CN_TXMAP
  /* 81 */ &Dump::vUndefined,  // CN_TXREF
  
  /* 82 */ &Dump::vDummy,      // CN_WORD

  /* 83 */ &Dump::vEnums       // CN_ENUMS
};

/*----------------------------------------------------------------------------*/
/* dummy dump                                                                 */
/*----------------------------------------------------------------------------*/
void Dump::vDummy(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass != CN_NONE);
  ASSERT(pObject != nullptr);
  ASSERT(pTab != nullptr);
  // do nothing
}

/*----------------------------------------------------------------------------*/
/* dummy dump                                                                 */
/*----------------------------------------------------------------------------*/
void Dump::vEnums(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_ENUMS);
  ASSERT(pObject != nullptr);
  ASSERT(pTab != nullptr);
  // do nothing
}

/*----------------------------------------------------------------------------*/
/* dump this attachmap                                                        */
/*----------------------------------------------------------------------------*/
void Dump::vInvalid(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass != CN_ENUMS);
  ASSERT(pObject != nullptr);
  ASSERT(pTab != nullptr);
  vTrace("%sInvalid Class %s in Buffer Pool\n", pTab, pCN[eClass]);
}

/*----------------------------------------------------------------------------*/
/* dump not defined                                                           */
/*----------------------------------------------------------------------------*/
void Dump::vUndefined(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass != CN_NONE);
  ASSERT(pObject != nullptr);
  ASSERT(pTab != nullptr);
  vTrace("%sDump function not defined for class %s\n", pTab, pCN[eClass]);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/