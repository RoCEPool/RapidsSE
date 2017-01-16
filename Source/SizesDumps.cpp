/*============================================================================*/
/* Dump.cpp -- Class Dumps                                                    */
/*============================================================================*/

#include "StorageEngine.h"
#include "SePrompt.hpp"

// Object's : Class::vDump
// Object's with no vDump defined : Dump::vUndefined
// Header : Dump::vInvalid
// Objects we don't want to dump (eg Word) : Dump::vDummy

pDUMPFUN *gpDump[]
{
  /*  0 */ &Dump::vInvalid,         // CN_NONE
  
  /*  1 */ &Dump::vInvalid,         // CN_S32
  /*  2 */ &Dump::vInvalid,         // CN_S256
  /*  3 */ &Dump::vInvalid,         // CN_S48K
  /*    */ &Dump::vInvalid,         // CN_S1M
  
  /*  4 */ &Dump::vInvalid,         // CN_INTVAL
  /*  5 */ &Dump::vInvalid,         // CN_IPVAL
  /*  6 */ &Dump::vInvalid,         // CN_OBJVAL
  /*  7 */ &Dump::vInvalid,         // CN_PTRVAL
  /*  8 */ &Dump::vInvalid,         // CN_STRVAL
  /*  9 */ &Dump::vInvalid,         // CN_VALREG
  /* 10 */ &Dump::vInvalid,         // CN_VALSET
  /* 11 */ &Dump::vInvalid,         // CN_VALWSP
  
  /* 12 */ &Dump::vInvalid,         // CN_ARGUMENTS
  /* 13 */ &Dump::vInvalid,         // CN_ATOMIC
  /* 14 */ &AttachMap::vDump,       // CN_ATTACHMAP

  /* 15 */ &Dump::vDummy,           // CN_FREEBUFFER
  /* 16 */ &Dump::vInvalid,         // CN_BUFFERPOOL
  /* 17 */ &Dump::vInvalid,         // CN_BPA

  /* 18 */ &Dic::vDump,             // CN_DIC
  /* 19 */ &DicStack::vDump,        // CN_DICSTACK
  /* 20 */ &Dump::vInvalid,         // CN_DIR
  
  /* 21 */ &Dump::vInvalid,         // CN_FILE
  /* 22 */ &Dump::vDummy,           // CN_FREECHUNK
  
  /* 23 */ &Graph::vDump,           // CN_GRAPH
  /* 24 */ &GraphCursor::vDump,     // CN_GRAPH
  /* 25 */ &GraphNode::vDump,       // CN_GRAPHNODE
  /* 26 */ &Grid::vDump,            // CN_GRID

  /* 27 */ &HeapBuffer::vDump,      // CN_HEAPBUFFER
  
  /* 28 */ &Index::vDump,           // CN_INDEX
  
  /* 29 */ &Dump::vInvalid,         // CN_JUMBOHEAP

  /* 30 */ &KeyCpp::vDump,          // CN_KEYCPP
  /* 31 */ &KeyVal::vDump,          // CN_KEYVAL
  
  /* 32 */ &LoadChunk::vDump,       // CN_LOADCHUNK
  /* 33 */ &Loader::vDump,          // CN_LOADER
  /* 34 */ &LoaderSub::vDump,       // CN_LOADERSUB
  /* 35 */ &LoadMap::vDump,         // CN_LOADMAP
  /* 36 */ &Dump::vInvalid,         // CN_LOCK
  
  /* 37 */ &Heap::vDump,            // CN_MEDIUMHEAP
  
  /* 38 */ &Node::vDump,            // CN_NODE
  /* 39 */ &NodeCursor::vDump,      // CN_NODECURSOR
  
  /* 40 */ &Dump::vDummy,           // CN_PARSER
  /* 41 */ &Partition::vDump,       // CN_PARTITION
  /* 42 */ &Dump::vUndefined,       // CN_PARTITIONS
  /* 43 */ &Dump::vUndefined,       // CN_PROXY
  /* 44 */ &Dump::vUndefined,       // CN_PROXYSET

  /* 45 */ &Queue::vDump,           // CN_QUEUE
  /* 46 */ &QueueBuffer::vDump,     // CN_QUEUEBUFFER

  /* 47 */ &RegFrame::vDump,        // CN_REGFRAME
  /* 48 */ &Dump::vUndefined,       // CN_RING
  /* 49 */ &Dump::vUndefined,       // CN_ROW
  /* 50 */ &RowBuffer::vDump,       // CN_ROWBUFFER
  /* 51 */ &Dump::vUndefined,       // CN_ROWCURSOR
  /* 52 */ &RowStrand::vDump,       // CN_ROWSTRAND
  /* 53 */ &Dump::vUndefined,       // CN_ROWWSP
  
  /* 54 */ &Dump::vUndefined,       // CN_SCANBUFFER
  /* 55 */ &Dump::vUndefined,       // CN_SCRATCH
  /* 56 */ &Dump::vUndefined,       // CN_SEHISTORY
  /* 57 */ &SEitf::vDump,           // CN_SEITF
  /* 58 */ &Dump::vUndefined,       // CN_SEITFCURSOR
  /* 59 */ &Dump::vUndefined,       // CN_SEITFLIST
  /* 60 */ &Dump::vDummy,           // CN_SEPROMPT
  /* 61 */ &Dump::vUndefined,       // CN_SINCE
  /* 62 */ &Dump::vUndefined,       // CN_SKIPCURSOR
  /* 63 */ &Dump::vUndefined,       // CN_SKIPLIST
  /* 64 */ &Dump::vUndefined,       // CN_SKIPNODE
  /* 65 */ &Heap::vDump,            // CN_SMALLHEAP
  /* 66 */ &Dump::vUndefined,       // CN_SOCKET
  /* 67 */ &Dump::vUndefined,       // CN_STACK
  /* 68 */ &Dump::vInvalid,         // CN_STEPACCUM
  /* 69 */ &StepArray::vDump,       // CN_STEPARRAY
  /* 70 */ &Dump::vInvalid,         // CN_SYSCALL

  /* 71 */ &Table::vDump,           // CN_TABLE
  /* 72 */ &Task::vDump,            // CN_TASK
  /* 73 */ &Dump::vUndefined,       // CN_TASKCURSOR
  /* 74 */ &TaskList::vDump,        // CN_TASKLIST
  /* 75 */ &Dump::vUndefined,       // CN_TASKSET
  /* 76 */ &Dump::vUndefined,       // CN_THREAD
  /* 77 */ &Dump::vUndefined,       // CN_THREADCURSOR
  /* 78 */ &Dump::vUndefined,       // CN_THREADLIST
  /* 79 */ &Dump::vUndefined,       // CN_TIME
  /* 80 */ &Dump::vInvalid,         // CN_TIMER
  /* 81 */ &Tokeniser::vDump,       // CN_TOKENISER
  /* 82 */ &Tx::vDump,              // CN_TX
  /* 83 */ &TxArray::vDump,         // CN_TXARRAY
  /* 84 */ &Dump::vUndefined,       // CN_TXCOLLECTION
  /* 85 */ &Dump::vUndefined,       // CN_TXCURSOR
  /* 86 */ &Dump::vUndefined,       // CN_TXMAP
  /* 87 */ &Dump::vUndefined,       // CN_TXREF

  /* 88 */ &Word::vDump,            // CN_WORD
  
  /* 89 */ &Dump::vUndefined,       // CN_ZSMALL_
  /* 90 */ &Dump::vUndefined,       // CN_ZMEDIUM_
  /* 91 */ &Dump::vUndefined,       // CN_ZLARGE_
  /* 92 */ &Dump::vUndefined,       // CN_ZJUMBO_

  /* 93 */ &Dump::vEnums            // CN_ENUMS
};

i8 giClassSize[]
{
  /*  0 */ 0,
  
  /*  1 */ sizeof(s32),                // CN_S32
  /*  2 */ sizeof(s256),               // CN_S256
  /*  3 */ sizeof(s48K),                // CN_S48K
  /*    */ sizeof(s1M),                // CN_S1M
  
  /*  4 */ sizeof(IntVal),             // CN_INTVAL
  /*  5 */ sizeof(PtrVal),             // CN_IPVAL
  /*  6 */ sizeof(ObjVal),             // CN_OBJVAL
  /*  7 */ sizeof(PtrVal),             // CN_PTRVAL
  /*  8 */ sizeof(StrVal),             // CN_STRVAL
  /*  9 */ sizeof(ValReg),             // CN_VALREG
  /* 10 */ sizeof(ValWsp), // correct  // CN_VALSET
  /* 11 */ sizeof(ValWsp),             // CN_VALWSP
  
  /* 12 */ sizeof(Arguments),          // CN_ARGUMENTS
  /* 13 */ sizeof(Atomic),             // CN_ATOMIC
  /* 14 */ sizeof(AttachMap),          // CN_ATTACHMAP
  
  /* 15 */ sizeof(Buffer),             // CN_FREEBUFFER
  /* 16 */ sizeof(BufferPool),         // CN_BUFFERPOOL
  /* 17 */ sizeof(BPA),                // CN_BPA
  
  /* 18 */ sizeof(Dic),                // CN_DIC
  /* 19 */ sizeof(DicStack),           // CN_DICSTACK
  /* 20 */ sizeof(Dir),                // CN_DIR
  
  /* 21 */ sizeof(File),               // CN_FILE
  /* 22 */ sizeof(FreeChunk),          // CN_FREECHUNK
  
  /* 23 */ sizeof(Graph),              // CN_GRAPH
  /* 24 */ sizeof(GraphCursor),        // CN_GRAPHCURSOR
  /* 25 */ sizeof(GraphNode),          // CN_GRAPHNODE
  /* 26 */ sizeof(Grid),               // CN_GRID
  
  /* 27 */ sizeof(HeapBuffer),         // CN_HEAPBUFFER
  
  /* 28 */ sizeof(Index),              // CN_INDEX
  
  /* 29 */ sizeof(Heap), // correct    // CN_JUMBOHEAP
  
  /* 30 */ sizeof(KeyCpp),             // CN_KEYCPP
  /* 31 */ sizeof(KeyVal),             // CN_KEYVAL
  
  /* 32 */ sizeof(LoadChunk),          // CN_LOADCHUNK
  /* 33 */ sizeof(Loader),             // CN_LOADER
  /* 34 */ sizeof(LoaderSub),          // CN_LOADERSUB
  /* 35 */ sizeof(LoadMap),            // CN_LOADMAP
  /* 36 */ sizeof(Lock),               // CN_LOCK
  
  /* 37 */ sizeof(Heap), // correct    // CN_MEDIUMHEAP
  
  /* 38 */ sizeof(Node),               // CN_NODE
  /* 39 */ sizeof(NodeCursor),         // CN_NODECURSOR
  
  /* 40 */ sizeof(Parser),             // CN_PARSER
  /* 41 */ sizeof(Partition),          // CN_PARTITION
  /* 42 */ sizeof(Partitions),         // CN_PARTITIONS
  /* 43 */ sizeof(Proxy),              // CN_PROXY
  /* 44 */ sizeof(ProxySet),           // CN_PROXYSET
  
  /* 45 */ sizeof(Queue),              // CN_QUEUE
  /* 46 */ sizeof(QueueBuffer),        // CN_QUEUEBUFFER
  
  /* 47 */ sizeof(RegFrame),           // CN_REGFRAME
  /* 48 */ sizeof(Ring),               // CN_RING
  /* 49 */ sizeof(Row),                // CN_ROW
  /* 50 */ sizeof(RowBuffer),          // CN_ROWBUFFER
  /* 51 */ sizeof(RowCursor),          // CN_ROWCURSOR
  /* 52 */ sizeof(RowStrand),          // CN_ROWSTRAND
  /* 53 */ sizeof(RowWsp),             // CN_ROWWSP

  /* 54 */ sizeof(ScanBuffer),         // CN_SCANBUFFER
  /* 55 */ sizeof(Scratch),            // CN_SCRATCH
  /* 56 */ sizeof(SeHistory),          // CN_SEHISTORY
  /* 57 */ sizeof(SEitf),              // CN_SEITF
  /* 58 */ sizeof(SEitfCursor),        // CN_SEITFCURSOR
  /* 59 */ sizeof(SEitfList),          // CN_SEITFLIST
  /* 60 */ sizeof(SePrompt),           // CN_SEPROMPT
  /* 61 */ sizeof(Since),              // CN_SINCE
  /* 62 */ sizeof(SkipCursor),         // CN_SKIPCURSOR
  /* 63 */ sizeof(SkipList),           // CN_SKIPLIST
  /* 64 */ sizeof(SkipNode),           // CN_SKIPNODE
  /* 65 */ sizeof(Heap), // correct    // CN_SMALLHEAP
  /* 66 */ sizeof(Socket),             // CN_SOCKET
  /* 67 */ sizeof(Stack),              // CN_STACK
  /* 68 */ sizeof(StepAccum),          // CN_STEPACCUM
  /* 69 */ sizeof(StepArray),          // CN_STEPARRAY
  /* 70 */ sizeof(SysCall),            // CN_SYSCALL
  
  /* 71 */ sizeof(Table),              // CN_TABLE
  /* 72 */ sizeof(Task),               // CN_TASK
  /* 73 */ sizeof(TaskCursor),         // CN_TASKCURSOR
  /* 74 */ sizeof(TaskList),           // CN_TASKLIST
  /* 75 */ sizeof(TaskSet),            // CN_TASKSET
  /* 76 */ sizeof(Thread),             // CN_THREAD
  /* 77 */ sizeof(ThreadCursor),       // CN_THREADCURSOR
  /* 78 */ sizeof(ThreadList),         // CN_THREADLIST
  /* 79 */ sizeof(Time),               // CN_TIME
  /* 80 */ sizeof(Timer),              // CN_TIMER
  /* 81 */ sizeof(Tokeniser),          // CN_TOKENISER
  /* 82 */ sizeof(Tx),                 // CN_TX
  /* 83 */ sizeof(TxArray),            // CN_TXARRAY
  /* 84 */ sizeof(TxCollection),       // CN_TXCOLLECTION
  /* 85 */ sizeof(TxCursor),           // CN_TXCURSOR
  /* 86 */ sizeof(TxMap),              // CN_TXMAP
  /* 87 */ sizeof(TxRef),              // CN_TXREF

  /* 88 */ sizeof(Word),               // CN_WORD
  
  /* 89 */ C_SIZE_SMALL_OBJECT  - 256, // CN_ZSMALL_
  /* 90 */ C_SIZE_MEDIUM_OBJECT - 256, // CN_ZMEDIUM_
  /* 91 */ C_SIZE_POOL_BUFFER   - 256, // CN_ZLARGE_
  /* 92 */ C_SIZE_JUMBO_OBJECT  - 256, // CN_ZJUMBO_
  
  /* 93 */ -1
};

eHT geWhichHeap[]
{
  /*  0 */ HT_INVALID,
  
  /*  1 */ HT_INVALID, // CN_S32
  /*  2 */ HT_INVALID, // CN_S256
  /*  3 */ HT_INVALID, // CN_S48K
  /*    */ HT_INVALID, // CN_S1M
  
  /*  4 */ HT_INVALID, // CN_INTVAL
  /*  5 */ HT_INVALID, // CN_IPVAL
  /*  6 */ HT_INVALID, // CN_OBJVAL
  /*  7 */ HT_INVALID, // CN_PTRVAL
  /*  8 */ HT_INVALID, // CN_STRVAL
  /*  9 */ HT_INVALID, // CN_VALREG
  /* 10 */ HT_INVALID, // CN_VALSET
  /* 11 */ HT_INVALID, // CN_VALWSP
  
  /* 12 */ HT_INVALID, // CN_ARGUMENTS
  /* 13 */ HT_INVALID, // CN_ATOMIC
  /* 14 */ HT_LARGE,   // CN_ATTACHMAP
  
  /* 15 */ HT_INVALID, // CN_FREEBUFFER
  /* 16 */ HT_INVALID, // CN_BUFFERPOOL
  /* 17 */ HT_INVALID, // CN_BPA
  
  /* 18 */ HT_MEDIUM,  // CN_DIC
  /* 19 */ HT_INVALID, // CN_DICSTACK
  /* 20 */ HT_SMALL,   // CN_DIR
  
  /* 21 */ HT_SMALL,   // CN_FILE
  /* 22 */ HT_INVALID, // CN_FREECHUNK
  
  /* 23 */ HT_SMALL,   // CN_GRAPH
  /* 24 */ HT_SMALL,   // CN_GRAPHCURSOR
  /* 25 */ HT_SMALL,   // CN_GRAPHNODE
  /* 26 */ HT_INVALID, // CN_GRID
  
  /* 27 */ HT_LARGE,   // CN_HEAPBUFFER
  
  /* 28 */ HT_INVALID, // CN_INDEX
  
  /* 29 */ HT_INVALID, // CN_JUMBOHEAP
  
  /* 30 */ HT_LARGE,   // CN_KEYCPP
  /* 31 */ HT_LARGE,   // CN_KEYVAL
  
  /* 32 */ HT_LARGE,   // CN_LOADCHUNK
  /* 33 */ HT_MEDIUM,  // CN_LOADER
  /* 34 */ HT_SMALL,   // CN_LOADERSUB
  /* 35 */ HT_JUMBO,   // CN_LOADMAP
  /* 36 */ HT_INVALID, // CN_LOCK
  
  /* 37 */ HT_INVALID, // CN_MEDIUMHEAP
  
  /* 38 */ HT_SMALL,   // CN_NODE
  /* 39 */ HT_SMALL,   // CN_NODECURSOR
  
  /* 40 */ HT_MEDIUM,  // CN_PARSER
  /* 41 */ HT_INVALID, // CN_PARTITION
  /* 42 */ HT_INVALID, // CN_PARTITIONS
  /* 43 */ HT_INVALID, // CN_PROXY
  /* 44 */ HT_INVALID, // CN_PROXYSET

  /* 45 */ HT_SMALL,   // CN_QUEUE
  /* 46 */ HT_LARGE,   // CN_QUEUEBUFFER
  
  /* 47 */ HT_LARGE,   // CN_REGFRAME
  /* 48 */ HT_INVALID, // CN_RING
  /* 49 */ HT_INVALID, // CN_ROW
  /* 50 */ HT_LARGE,   // CN_ROWBUFFER
  /* 51 */ HT_SMALL,   // CN_ROWCURSOR
  /* 52 */ HT_SMALL,   // CN_ROWSTRAND
  /* 53 */ HT_INVALID, // CN_ROWWSP
  
  /* 54 */ HT_LARGE,   // CN_SCANBUFFER
  /* 55 */ HT_LARGE,   // CN_SCRATCH
  /* 56 */ HT_INVALID, // CN_SEHISTORY
  /* 57 */ HT_MEDIUM,  // CN_SEITF
  /* 58 */ HT_SMALL,   // CN_SEITFCURSOR
  /* 59 */ HT_INVALID, // CN_SEITFLIST
  /* 60 */ HT_LARGE,   // CN_SEPROMPT
  /* 61 */ HT_INVALID, // CN_SINCE
  /* 62 */ HT_INVALID, // CN_SKIPCURSOR
  /* 63 */ HT_SMALL,   // CN_SKIPLIST
  /* 64 */ HT_SMALL,   // CN_SKIPNODE
  /* 65 */ HT_INVALID, // CN_SMALLHEAP
  /* 66 */ HT_SMALL,   // CN_SOCKET
  /* 67 */ HT_LARGE,   // CN_STACK
  /* 68 */ HT_INVALID, // CN_STEPACCUM
  /* 69 */ HT_LARGE,   // CN_STEPARRAY
  /* 70 */ HT_INVALID, // CN_SYSCALL
  
  /* 71 */ HT_INVALID, // CN_TABLE
  /* 72 */ HT_MEDIUM,  // CN_TASK
  /* 73 */ HT_INVALID, // CN_TASKCURSOR
  /* 74 */ HT_SMALL,   // CN_TASKLIST
  /* 75 */ HT_INVALID, // CN_TASKSET
  /* 76 */ HT_INVALID, // CN_THREAD
  /* 77 */ HT_SMALL,   // CN_THREADCURSOR
  /* 78 */ HT_INVALID, // CN_THREADLIST
  /* 79 */ HT_INVALID, // CN_TIME
  /* 80 */ HT_SMALL,   // CN_TIMER
  /* 81 */ HT_MEDIUM,  // CN_TOKENISER
  /* 82 */ HT_SMALL,   // CN_TX
  /* 83 */ HT_LARGE,   // CN_TXARRAY
  /* 84 */ HT_INVALID, // CN_TXCOLLECTION
  /* 85 */ HT_SMALL,   // CN_TXCURSOR
  /* 86 */ HT_INVALID, // CN_TXMAP
  /* 87 */ HT_SMALL,   // CN_TXREF

  /* 88 */ HT_MEDIUM,  // CN_WORD
  
  /* 89 */ HT_INVALID, // CN_ZSMALL_
  /* 90 */ HT_INVALID, // CN_ZMEDIUM_
  /* 91 */ HT_INVALID, // CN_ZLARGE_
  /* 92 */ HT_INVALID, // CN_ZJUMBO_

  /* 93 */ HT_INVALID, // CN_ENUMS
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
