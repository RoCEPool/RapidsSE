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
  /*  3 */ &Dump::vInvalid,         // CN_S8K
  /*  4 */ &Dump::vInvalid,         // CN_INTVAL
  /*  5 */ &Dump::vInvalid,         // CN_OBJVAL
  /*  6 */ &Dump::vInvalid,         // CN_PTRVAL
  /*  7 */ &Dump::vInvalid,         // CN_STRVAL
  /*  8 */ &Dump::vInvalid,         // CN_VALREG
  /*  9 */ &Dump::vInvalid,         // CN_VALSET
  /* 10 */ &Dump::vInvalid,         // CN_VALWSP
  
  /* 11 */ &Dump::vInvalid,         // CN_ARGUMENTS
  /* 12 */ &Dump::vInvalid,         // CN_ATOMIC
  /* 13 */ &AttachMap::vDump,       // CN_ATTACHMAP

  /* 14 */ &Dump::vDummy,           // CN_BUFFER
  /* 15 */ &Dump::vInvalid,         // CN_BUFFERPOOL
  /* 16 */ &Dump::vInvalid,         // CN_BPA

  /* 17 */ &Dic::vDump,             // CN_DIC
  /* 18 */ &DicStack::vDump,        // CN_DICSTACK
  /* 19 */ &Dump::vInvalid,         // CN_DIR
  
  /* 20 */ &Dump::vInvalid,         // CN_FILE
  /* 21 */ &Dump::vDummy,           // CN_FREECHUNK
  
  /* 22 */ &Graph::vDump,           // CN_GRAPH
  /* 23 */ &GraphCursor::vDump,     // CN_GRAPH
  /* 24 */ &GraphNode::vDump,       // CN_GRAPHNODE
  /* 25 */ &Grid::vDump,            // CN_GRID

  /* 26 */ &HeapBuffer::vDump,      // CN_HEAPBUFFER
  
  /* 27 */ &Index::vDump,           // CN_INDEX
  
  /* 28 */ &Dump::vInvalid,         // CN_JUMBOHEAP

  /* 29 */ &KeyCpp::vDump,          // CN_KEYCPP
  /* 30 */ &KeyVal::vDump,          // CN_KEYVAL
  
  /* 31 */ &LoadChunk::vDump,       // CN_LOADCHUNK
  /* 32 */ &Loader::vDump,          // CN_LOADER
  /* 33 */ &LoaderSub::vDump,       // CN_LOADERSUB
  /* 34 */ &Dump::vInvalid,         // CN_LOCK
  
  /* 35 */ &Heap::vDump,            // CN_MEDIUMHEAP
  
  /* 36 */ &Node::vDump,            // CN_NODE
  /* 37 */ &NodeCursor::vDump,      // CN_NODECURSOR
  
  /* 38 */ &Dump::vDummy,           // CN_PARSER
  /* 39 */ &Partition::vDump,       // CN_PARTITION
  /* 40 */ &Dump::vUndefined,       // CN_PARTITIONS
  /* 41 */ &Dump::vUndefined,       // CN_PROXY
  /* 42 */ &Dump::vUndefined,       // CN_PROXYSET

  /* 43 */ &Queue::vDump,           // CN_QUEUE
  /* 44 */ &QueueBuffer::vDump,     // CN_QUEUEBUFFER

  /* 45 */ &RegFrame::vDump,        // CN_REGFRAME
  /* 46 */ &Dump::vUndefined,       // CN_RING
  /* 47 */ &Dump::vUndefined,       // CN_ROW
  /* 48 */ &RowBuffer::vDump,       // CN_ROWBUFFER
  /* 49 */ &Dump::vUndefined,       // CN_ROWCURSOR
  /* 50 */ &RowStrand::vDump,       // CN_ROWSTRAND
  /* 51 */ &Dump::vUndefined,       // CN_ROWWSP
  
  /* 52 */ &Dump::vUndefined,       // CN_SCANBUFFER
  /* 53 */ &Dump::vUndefined,       // CN_SCRATCH
  /* 54 */ &LoadMap::vDump,         // CN_LOADMAP
  /* 55 */ &Dump::vUndefined,       // CN_SEHISTORY
  /* 56 */ &SEitf::vDump,           // CN_SEITF
  /* 57 */ &Dump::vUndefined,       // CN_SEITFCURSOR
  /* 58 */ &Dump::vUndefined,       // CN_SEITFLIST
  /* 59 */ &Dump::vDummy,           // CN_SEPROMPT
  /* 60 */ &Dump::vUndefined,       // CN_SINCE
  /* 61 */ &Dump::vUndefined,       // CN_SKIPCURSOR
  /* 62 */ &Dump::vUndefined,       // CN_SKIPLIST
  /* 63 */ &Dump::vUndefined,       // CN_SKIPNODE
  /* 64 */ &Heap::vDump,            // CN_SMALLHEAP
  /* 65 */ &Dump::vUndefined,       // CN_SOCKET
  /* 66 */ &Dump::vUndefined,       // CN_STACK
  /* 67 */ &Dump::vInvalid,         // CN_STEPACCUM
  /* 68 */ &StepArray::vDump,       // CN_STEPARRAY
  /* 69 */ &Dump::vInvalid,         // CN_SYSCALL

  /* 70 */ &Table::vDump,           // CN_TABLE
  /* 71 */ &Task::vDump,            // CN_TASK
  /* 72 */ &Dump::vUndefined,       // CN_TASKCURSOR
  /* 73 */ &TaskList::vDump,        // CN_TASKLIST
  /* 74 */ &Dump::vUndefined,       // CN_TASKSET
  /* 75 */ &Dump::vUndefined,       // CN_THREAD
  /* 76 */ &Dump::vUndefined,       // CN_THREADCURSOR
  /* 77 */ &Dump::vUndefined,       // CN_THREADLIST
  /* 78 */ &Dump::vUndefined,       // CN_TIME
  /* 79 */ &Dump::vInvalid,         // CN_TIMER
  /* 80 */ &Tokeniser::vDump,       // CN_TOKENISER
  /* 81 */ &Tx::vDump,              // CN_TX
  /* 82 */ &TxArray::vDump,         // CN_TXARRAY
  /* 83 */ &Dump::vUndefined,       // CN_TXCOLLECTION
  /* 84 */ &Dump::vUndefined,       // CN_TXCURSOR
  /* 85 */ &Dump::vUndefined,       // CN_TXMAP
  /* 86 */ &Dump::vUndefined,       // CN_TXREF

  /* 87 */ &Dump::vDummy,           // CN_WORD
  
  /* 88 */ &Dump::vUndefined,       // CN_ZSMALL_
  /* 89 */ &Dump::vUndefined,       // CN_ZMEDIUM_
  /* 90 */ &Dump::vUndefined,       // CN_ZLARGE_
  /* 91 */ &Dump::vUndefined,       // CN_ZJUMBO_

  /* 92 */ &Dump::vEnums            // CN_ENUMS
};

i8 giClassSize[]
{
  /*  0 */ 0,
  
  /*  1 */ sizeof(s32),                // CN_S32
  /*  2 */ sizeof(s256),               // CN_S256
  /*  3 */ sizeof(s8K),                // CN_S8K
  
  /*  4 */ sizeof(IntVal),             // CN_INTVAL
  /*  5 */ sizeof(ObjVal),             // CN_OBJVAL
  /*  6 */ sizeof(PtrVal),             // CN_PTRVAL
  /*  7 */ sizeof(StrVal),             // CN_STRVAL
  /*  8 */ sizeof(ValReg),             // CN_VALREG
  /*  9 */ sizeof(ValWsp), // correct  // CN_VALSET
  /* 10 */ sizeof(ValWsp),             // CN_VALWSP
  
  /* 11 */ sizeof(Arguments),          // CN_ARGUMENTS
  /* 12 */ sizeof(Atomic),             // CN_ATOMIC
  /* 13 */ sizeof(AttachMap),          // CN_ATTACHMAP
  
  /* 14 */ sizeof(Buffer),             // CN_BUFFER
  /* 15 */ sizeof(BufferPool),         // CN_BUFFERPOOL
  /* 16 */ sizeof(BPA),                // CN_BPA
  
  /* 17 */ sizeof(Dic),                // CN_DIC
  /* 18 */ sizeof(DicStack),           // CN_DICSTACK
  /* 19 */ sizeof(Dir),                // CN_DIR
  
  /* 20 */ sizeof(File),               // CN_FILE
  /* 21 */ sizeof(FreeChunk),          // CN_FREECHUNK
  
  /* 22 */ sizeof(Graph),              // CN_GRAPH
  /* 23 */ sizeof(GraphCursor),        // CN_GRAPHCURSOR
  /* 24 */ sizeof(GraphNode),          // CN_GRAPHNODE
  /* 25 */ sizeof(Grid),               // CN_GRID
  
  /* 26 */ sizeof(HeapBuffer),         // CN_HEAPBUFFER
  
  /* 27 */ sizeof(Index),              // CN_INDEX
  
  /* 28 */ sizeof(Heap), // correct    // CN_JUMBOHEAP
  
  /* 29 */ sizeof(KeyCpp),             // CN_KEYCPP
  /* 30 */ sizeof(KeyVal),             // CN_KEYVAL
  
  /* 31 */ sizeof(LoadChunk),          // CN_LOADCHUNK
  /* 32 */ sizeof(Loader),             // CN_LOADER
  /* 33 */ sizeof(LoaderSub),          // CN_LOADERSUB
  /* 34 */ sizeof(Lock),               // CN_LOCK
  
  /* 35 */ sizeof(Heap), // correct    // CN_MEDIUMHEAP
  
  /* 36 */ sizeof(Node),               // CN_NODE
  /* 37 */ sizeof(NodeCursor),         // CN_NODECURSOR
  
  /* 38 */ sizeof(Parser),             // CN_PARSER
  /* 39 */ sizeof(Partition),          // CN_PARTITION
  /* 40 */ sizeof(Partitions),         // CN_PARTITIONS
  /* 41 */ sizeof(Proxy),              // CN_PROXY
  /* 42 */ sizeof(ProxySet),           // CN_PROXYSET
  
  /* 43 */ sizeof(Queue),              // CN_QUEUE
  /* 44 */ sizeof(QueueBuffer),        // CN_QUEUEBUFFER
  
  /* 45 */ sizeof(RegFrame),           // CN_REGFRAME
  /* 46 */ sizeof(Ring),               // CN_RING
  /* 47 */ sizeof(Row),                // CN_ROW
  /* 48 */ sizeof(RowBuffer),          // CN_ROWBUFFER
  /* 49 */ sizeof(RowCursor),          // CN_ROWCURSOR
  /* 50 */ sizeof(RowStrand),          // CN_ROWSTRAND
  /* 51 */ sizeof(RowWsp),             // CN_ROWWSP

  /* 52 */ sizeof(ScanBuffer),         // CN_SCANBUFFER
  /* 53 */ sizeof(Scratch),            // CN_SCRATCH
  /* 54 */ sizeof(LoadMap),            // CN_LOADMAP
  /* 55 */ sizeof(SeHistory),          // CN_SEHISTORY
  /* 56 */ sizeof(SEitf),              // CN_SEITF
  /* 57 */ sizeof(SEitfCursor),        // CN_SEITFCURSOR
  /* 58 */ sizeof(SEitfList),          // CN_SEITFLIST
  /* 59 */ sizeof(SePrompt),           // CN_SEPROMPT
  /* 60 */ sizeof(Since),              // CN_SINCE
  /* 61 */ sizeof(SkipCursor),         // CN_SKIPCURSOR
  /* 62 */ sizeof(SkipList),           // CN_SKIPLIST
  /* 63 */ sizeof(SkipNode),           // CN_SKIPNODE
  /* 64 */ sizeof(Heap), // correct    // CN_SMALLHEAP
  /* 65 */ sizeof(Socket),             // CN_SOCKET
  /* 66 */ sizeof(Stack),              // CN_STACK
  /* 67 */ sizeof(StepAccum),          // CN_STEPACCUM
  /* 68 */ sizeof(StepArray),          // CN_STEPARRAY
  /* 69 */ sizeof(SysCall),            // CN_SYSCALL
  
  /* 70 */ sizeof(Table),              // CN_TABLE
  /* 71 */ sizeof(Task),               // CN_TASK
  /* 72 */ sizeof(TaskCursor),         // CN_TASKCURSOR
  /* 73 */ sizeof(TaskList),           // CN_TASKLIST
  /* 74 */ sizeof(TaskSet),            // CN_TASKSET
  /* 75 */ sizeof(Thread),             // CN_THREAD
  /* 76 */ sizeof(ThreadCursor),       // CN_THREADCURSOR
  /* 77 */ sizeof(ThreadList),         // CN_THREADLIST
  /* 78 */ sizeof(Time),               // CN_TIME
  /* 79 */ sizeof(Timer),              // CN_TIMER
  /* 80 */ sizeof(Tokeniser),          // CN_TOKENISER
  /* 81 */ sizeof(Tx),                 // CN_TX
  /* 82 */ sizeof(TxArray),            // CN_TXARRAY
  /* 83 */ sizeof(TxCollection),       // CN_TXCOLLECTION
  /* 84 */ sizeof(TxCursor),           // CN_TXCURSOR
  /* 85 */ sizeof(TxMap),              // CN_TXMAP
  /* 86 */ sizeof(TxRef),              // CN_TXREF

  /* 87 */ sizeof(Word),               // CN_WORD
  
  /* 88 */ C_SIZE_SMALL_OBJECT  - 256, // CN_ZSMALL_
  /* 89 */ C_SIZE_MEDIUM_OBJECT - 256, // CN_ZMEDIUM_
  /* 90 */ C_SIZE_POOL_BUFFER   - 256, // CN_ZLARGE_
  /* 91 */ C_SIZE_JUMBO_OBJECT  - 256, // CN_ZJUMBO_
  
  /* 92 */ -1
};

eHT geWhichHeap[]
{
  /*  0 */ HT_INVALID,
  
  /*  1 */ HT_INVALID, // CN_S32
  /*  2 */ HT_INVALID, // CN_S256
  /*  3 */ HT_INVALID, // CN_S8K
  
  /*  4 */ HT_INVALID, // CN_INTVAL
  /*  5 */ HT_INVALID, // CN_OBJVAL
  /*  6 */ HT_INVALID, // CN_PTRVAL
  /*  7 */ HT_INVALID, // CN_STRVAL
  /*  8 */ HT_INVALID, // CN_VALREG
  /*  9 */ HT_INVALID, // CN_VALSET
  /* 10 */ HT_INVALID, // CN_VALWSP
  
  /* 11 */ HT_INVALID, // CN_ARGUMENTS
  /* 12 */ HT_INVALID, // CN_ATOMIC
  /* 13 */ HT_LARGE,   // CN_ATTACHMAP
  
  /* 14 */ HT_INVALID, // CN_BUFFER
  /* 15 */ HT_INVALID, // CN_BUFFERPOOL
  /* 16 */ HT_INVALID, // CN_BPA
  
  /* 17 */ HT_MEDIUM,  // CN_DIC
  /* 18 */ HT_INVALID, // CN_DICSTACK
  /* 19 */ HT_SMALL,   // CN_DIR
  
  /* 20 */ HT_SMALL,   // CN_FILE
  /* 21 */ HT_INVALID, // CN_FREECHUNK
  
  /* 22 */ HT_SMALL,   // CN_GRAPH
  /* 23 */ HT_SMALL,   // CN_GRAPHCURSOR
  /* 24 */ HT_SMALL,   // CN_GRAPHNODE
  /* 25 */ HT_INVALID, // CN_GRID
  
  /* 26 */ HT_LARGE,   // CN_HEAPBUFFER
  
  /* 27 */ HT_INVALID, // CN_INDEX
  
  /* 28 */ HT_INVALID, // CN_JUMBOHEAP
  
  /* 29 */ HT_MEDIUM,  // CN_KEYCPP
  /* 30 */ HT_MEDIUM,  // CN_KEYVAL
  
  /* 31 */ HT_LARGE,   // CN_LOADCHUNK
  /* 32 */ HT_MEDIUM,  // CN_LOADER
  /* 33 */ HT_SMALL,   // CN_LOADERSUB
  /* 34 */ HT_INVALID, // CN_LOCK
  
  /* 35 */ HT_INVALID, // CN_MEDIUMHEAP
  
  /* 36 */ HT_SMALL,   // CN_NODE
  /* 37 */ HT_SMALL,   // CN_NODECURSOR
  
  /* 38 */ HT_MEDIUM,  // CN_PARSER
  /* 39 */ HT_INVALID, // CN_PARTITION
  /* 40 */ HT_INVALID, // CN_PARTITIONS
  /* 41 */ HT_INVALID, // CN_PROXY
  /* 42 */ HT_INVALID, // CN_PROXYSET
  
  /* 43 */ HT_SMALL,   // CN_QUEUE
  /* 44 */ HT_LARGE,   // CN_QUEUEBUFFER
  
  /* 45 */ HT_MEDIUM,  // CN_REGFRAME
  /* 46 */ HT_INVALID, // CN_RING
  /* 47 */ HT_INVALID, // CN_ROW
  /* 48 */ HT_LARGE,   // CN_ROWBUFFER
  /* 49 */ HT_SMALL,   // CN_ROWCURSOR
  /* 50 */ HT_SMALL,   // CN_ROWSTRAND
  /* 51 */ HT_INVALID, // CN_ROWWSP
  
  /* 52 */ HT_LARGE,   // CN_SCANBUFFER
  /* 53 */ HT_LARGE,   // CN_SCRATCH
  /* 54 */ HT_JUMBO,   // CN_LOADMAP
  /* 55 */ HT_INVALID, // CN_SEHISTORY
  /* 56 */ HT_MEDIUM,  // CN_SEITF
  /* 57 */ HT_SMALL,   // CN_SEITFCURSOR
  /* 58 */ HT_INVALID, // CN_SEITFLIST
  /* 59 */ HT_LARGE,   // CN_SEPROMPT
  /* 60 */ HT_INVALID, // CN_SINCE
  /* 61 */ HT_INVALID, // CN_SKIPCURSOR
  /* 62 */ HT_SMALL,   // CN_SKIPLIST
  /* 63 */ HT_SMALL,   // CN_SKIPNODE
  /* 64 */ HT_INVALID, // CN_SMALLHEAP
  /* 65 */ HT_SMALL,   // CN_SOCKET
  /* 66 */ HT_INVALID, // CN_STACK
  /* 67 */ HT_INVALID, // CN_STEPACCUM
  /* 68 */ HT_LARGE,   // CN_STEPARRAY
  /* 69 */ HT_INVALID, // CN_SYSCALL
  
  /* 70 */ HT_INVALID, // CN_TABLE
  /* 71 */ HT_MEDIUM,  // CN_TASK
  /* 72 */ HT_INVALID, // CN_TASKCURSOR
  /* 73 */ HT_SMALL,   // CN_TASKLIST
  /* 74 */ HT_INVALID, // CN_TASKSET
  /* 75 */ HT_INVALID, // CN_THREAD
  /* 76 */ HT_SMALL,   // CN_THREADCURSOR
  /* 77 */ HT_INVALID, // CN_THREADLIST
  /* 78 */ HT_INVALID, // CN_TIME
  /* 79 */ HT_SMALL,   // CN_TIMER
  /* 80 */ HT_INVALID, // CN_TOKENISER
  /* 81 */ HT_SMALL,   // CN_TX
  /* 82 */ HT_LARGE,   // CN_TXARRAY
  /* 83 */ HT_INVALID, // CN_TXCOLLECTION
  /* 84 */ HT_SMALL,   // CN_TXCURSOR
  /* 85 */ HT_INVALID, // CN_TXMAP
  /* 86 */ HT_SMALL,   // CN_TXREF

  /* 87 */ HT_MEDIUM,  // CN_WORD
  
  /* 88 */ HT_INVALID, // CN_ZSMALL_
  /* 89 */ HT_INVALID, // CN_ZMEDIUM_
  /* 90 */ HT_INVALID, // CN_ZLARGE_
  /* 91 */ HT_INVALID, // CN_ZJUMBO_

  /* 92 */ HT_INVALID, // CN_ENUMS
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
