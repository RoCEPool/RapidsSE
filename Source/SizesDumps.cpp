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
  /*  4 */ &Dump::vInvalid,         // CN_S1M
  
  /*  5 */ &Dump::vInvalid,         // CN_INTVAL
  /*  6 */ &Dump::vInvalid,         // CN_IPVAL
  /*  7 */ &Dump::vInvalid,         // CN_OBJVAL
  /*  8 */ &Dump::vInvalid,         // CN_PTRVAL
  /*  9 */ &Dump::vInvalid,         // CN_STRVAL
  /* 10 */ &Dump::vInvalid,         // CN_VALREG
  /* 11 */ &Dump::vInvalid,         // CN_VALSET
  /* 12 */ &Dump::vInvalid,         // CN_VALWSP
  
  /* 13 */ &Dump::vInvalid,         // CN_ARGUMENTS
  /* 14 */ &Dump::vInvalid,         // CN_ATOMIC
  /* 15 */ &AttachMap::vDump,       // CN_ATTACHMAP

  /* 16 */ &Dump::vInvalid,         // CN_BUFFERPOOL
  /* 17 */ &Dump::vInvalid,         // CN_BPA

  /* 18 */ &Dic::vDump,             // CN_DIC
  /* 19 */ &DicStack::vDump,        // CN_DICSTACK
  /* 20 */ &Dump::vInvalid,         // CN_DIR
  
  /* 21 */ &Dump::vInvalid,         // CN_FILE
  /* 22 */ &Dump::vDummy,           // CN_FREEBUFFER
  /* 23 */ &Dump::vDummy,           // CN_FREECHUNK
  
  /* 24 */ &Graph::vDump,           // CN_GRAPH
  /* 25 */ &GraphCursor::vDump,     // CN_GRAPH
  /* 26 */ &GraphNode::vDump,       // CN_GRAPHNODE
  /* 27 */ &Grid::vDump,            // CN_GRID

  /* 28 */ &HeapBuffer::vDump,      // CN_HEAPBUFFER
  
  /* 29 */ &Index::vDump,           // CN_INDEX
  
  /* 30 */ &Dump::vDummy,           // CN_JUMBOHEAP

  /* 31 */ &KeyCpp::vDump,          // CN_KEYCPP
  /* 32 */ &KeyVal::vDump,          // CN_KEYVAL
  
  /* 33 */ &LoadChunk::vDump,       // CN_LOADCHUNK
  /* 34 */ &Loader::vDump,          // CN_LOADER
  /* 35 */ &LoaderSub::vDump,       // CN_LOADERSUB
  /* 36 */ &LoadMap::vDump,         // CN_LOADMAP
  /* 37 */ &Dump::vInvalid,         // CN_LOCK
  
  /* 38 */ &Heap::vDump,            // CN_MEDIUMHEAP
  
  /* 39 */ &Node::vDump,            // CN_NODE
  /* 40 */ &NodeCursor::vDump,      // CN_NODECURSOR
  
  /* 41 */ &Parser::vDump,          // CN_PARSER
  /* 42 */ &Partition::vDump,       // CN_PARTITION
  /* 43 */ &Dump::vUndefined,       // CN_PARTITIONS
  /* 44 */ &Dump::vUndefined,       // CN_PROXY
  /* 45 */ &Dump::vUndefined,       // CN_PROXYSET
  
  /* 46 */ &Queue::vDump,           // CN_QUEUE
  /* 47 */ &QueueBuffer::vDump,     // CN_QUEUEBUFFER

  /* 48 */ &RegFrame::vDump,        // CN_REGFRAME
  /* 49 */ &Dump::vUndefined,       // CN_RING
  /* 50 */ &Dump::vUndefined,       // CN_ROW
  /* 51 */ &RowBuffer::vDump,       // CN_ROWBUFFER
  /* 52 */ &Dump::vUndefined,       // CN_ROWCURSOR
  /* 53 */ &RowStrand::vDump,       // CN_ROWSTRAND
  /* 54 */ &Dump::vUndefined,       // CN_ROWWSP
  
  /* 55 */ &Dump::vUndefined,       // CN_SCANBUFFER
  /* 56 */ &Dump::vUndefined,       // CN_SCRATCH
  /* 57 */ &Dump::vUndefined,       // CN_SEHISTORY
  /* 58 */ &SEitf::vDump,           // CN_SEITF
  /* 59 */ &Dump::vUndefined,       // CN_SEITFCURSOR
  /* 60 */ &Dump::vUndefined,       // CN_SEITFLIST
  /* 61 */ &SePrompt::vDump,        // CN_SEPROMPT
  /* 62 */ &Dump::vUndefined,       // CN_SINCE
  /* 63 */ &Dump::vUndefined,       // CN_SKIPCURSOR
  /* 64 */ &Dump::vUndefined,       // CN_SKIPLIST
  /* 65 */ &Dump::vUndefined,       // CN_SKIPNODE
  /* 66 */ &Heap::vDump,            // CN_SMALLHEAP
  /* 67 */ &Dump::vUndefined,       // CN_SOCKET
  /* 68 */ &Stack::vDump,           // CN_STACK
  /* 69 */ &Dump::vInvalid,         // CN_STEPACCUM
  /* 70 */ &StepArray::vDump,       // CN_STEPARRAY
  /* 71 */ &Dump::vInvalid,         // CN_SYSCALL

  /* 72 */ &Table::vDump,           // CN_TABLE
  /* 73 */ &Task::vDump,            // CN_TASK
  /* 74 */ &Dump::vUndefined,       // CN_TASKCURSOR
  /* 75 */ &TaskList::vDump,        // CN_TASKLIST
  /* 76 */ &Dump::vUndefined,       // CN_TASKSET
  /* 77 */ &Dump::vUndefined,       // CN_THREAD
  /* 78 */ &Dump::vUndefined,       // CN_THREADCURSOR
  /* 79 */ &Dump::vUndefined,       // CN_THREADLIST
  /* 80 */ &Dump::vUndefined,       // CN_TIME
  /* 81 */ &Dump::vInvalid,         // CN_TIMER
  /* 82 */ &Tokeniser::vDump,       // CN_TOKENISER
  /* 83 */ &Tx::vDump,              // CN_TX
  /* 84 */ &TxArray::vDump,         // CN_TXARRAY
  /* 85 */ &Dump::vUndefined,       // CN_TXCOLLECTION
  /* 86 */ &Dump::vUndefined,       // CN_TXCURSOR
  /* 87 */ &Dump::vUndefined,       // CN_TXMAP
  /* 88 */ &Dump::vUndefined,       // CN_TXREF

  /* 89 */ &Word::vDump,            // CN_WORD
  
  /* 90 */ &Dump::vUndefined,       // CN_ZSMALL_
  /* 91 */ &Dump::vUndefined,       // CN_ZMEDIUM_
  /* 92 */ &Dump::vUndefined,       // CN_ZLARGE_
  /* 93 */ &Dump::vUndefined,       // CN_ZJUMBO_
  
  /* 94 */ &Dump::vEnums            // CN_ENUMS
};

i8 giClassSize[]
{
  /*  0 */ 0,
  
  /*  1 */ sizeof(s32),                // CN_S32
  /*  2 */ sizeof(s256),               // CN_S256
  /*  3 */ sizeof(s48K),               // CN_S48K
  /*  4 */ sizeof(s1M),                // CN_S1M
  
  /*  5 */ sizeof(IntVal),             // CN_INTVAL
  /*  6 */ sizeof(PtrVal),             // CN_IPVAL
  /*  7 */ sizeof(ObjVal),             // CN_OBJVAL
  /*  8 */ sizeof(PtrVal),             // CN_PTRVAL
  /*  9 */ sizeof(StrVal),             // CN_STRVAL
  /* 10 */ sizeof(ValReg),             // CN_VALREG
  /* 11 */ sizeof(ValWsp), // correct  // CN_VALSET
  /* 12 */ sizeof(ValWsp),             // CN_VALWSP
  
  /* 13 */ sizeof(Arguments),          // CN_ARGUMENTS
  /* 14 */ sizeof(Atomic),             // CN_ATOMIC
  /* 15 */ sizeof(AttachMap),          // CN_ATTACHMAP
  
  /* 16 */ sizeof(BufferPool),         // CN_BUFFERPOOL
  /* 17 */ sizeof(BPA),                // CN_BPA
  
  /* 18 */ sizeof(Dic),                // CN_DIC
  /* 19 */ sizeof(DicStack),           // CN_DICSTACK
  /* 20 */ sizeof(Dir),                // CN_DIR
  
  /* 21 */ sizeof(File),               // CN_FILE
  /* 22 */ sizeof(Buffer),             // CN_FREEBUFFER
  /* 23 */ sizeof(FreeChunk),          // CN_FREECHUNK
  
  /* 24 */ sizeof(Graph),              // CN_GRAPH
  /* 25 */ sizeof(GraphCursor),        // CN_GRAPHCURSOR
  /* 26 */ sizeof(GraphNode),          // CN_GRAPHNODE
  /* 27 */ sizeof(Grid),               // CN_GRID
  
  /* 28 */ sizeof(HeapBuffer),         // CN_HEAPBUFFER
  
  /* 29 */ sizeof(Index),              // CN_INDEX
  
  /* 30 */ sizeof(Heap), // correct    // CN_JUMBOHEAP
  
  /* 31 */ sizeof(KeyCpp),             // CN_KEYCPP
  /* 32 */ sizeof(KeyVal),             // CN_KEYVAL
  
  /* 33 */ sizeof(LoadChunk),          // CN_LOADCHUNK
  /* 34 */ sizeof(Loader),             // CN_LOADER
  /* 35 */ sizeof(LoaderSub),          // CN_LOADERSUB
  /* 36 */ sizeof(LoadMap),            // CN_LOADMAP
  /* 37 */ sizeof(Lock),               // CN_LOCK
  
  /* 38 */ sizeof(Heap), // correct    // CN_MEDIUMHEAP
  
  /* 39 */ sizeof(Node),               // CN_NODE
  /* 40 */ sizeof(NodeCursor),         // CN_NODECURSOR
  
  /* 41 */ sizeof(Parser),             // CN_PARSER
  /* 42 */ sizeof(Partition),          // CN_PARTITION
  /* 43 */ sizeof(Partitions),         // CN_PARTITIONS
  /* 44 */ sizeof(Proxy),              // CN_PROXY
  /* 45 */ sizeof(ProxySet),           // CN_PROXYSET
  
  /* 46 */ sizeof(Queue),              // CN_QUEUE
  /* 47 */ sizeof(QueueBuffer),        // CN_QUEUEBUFFER
  
  /* 48 */ sizeof(RegFrame),           // CN_REGFRAME
  /* 49 */ sizeof(Ring),               // CN_RING
  /* 50 */ sizeof(Row),                // CN_ROW
  /* 51 */ sizeof(RowBuffer),          // CN_ROWBUFFER
  /* 52 */ sizeof(RowCursor),          // CN_ROWCURSOR
  /* 53 */ sizeof(RowStrand),          // CN_ROWSTRAND
  /* 54 */ sizeof(RowWsp),             // CN_ROWWSP
  
  /* 55 */ sizeof(ScanBuffer),         // CN_SCANBUFFER
  /* 56 */ sizeof(Scratch),            // CN_SCRATCH
  /* 57 */ sizeof(SeHistory),          // CN_SEHISTORY
  /* 58 */ sizeof(SEitf),              // CN_SEITF
  /* 59 */ sizeof(SEitfCursor),        // CN_SEITFCURSOR
  /* 60 */ sizeof(SEitfList),          // CN_SEITFLIST
  /* 61 */ sizeof(SePrompt),           // CN_SEPROMPT
  /* 62 */ sizeof(Since),              // CN_SINCE
  /* 63 */ sizeof(SkipCursor),         // CN_SKIPCURSOR
  /* 64 */ sizeof(SkipList),           // CN_SKIPLIST
  /* 65 */ sizeof(SkipNode),           // CN_SKIPNODE
  /* 66 */ sizeof(Heap), // correct    // CN_SMALLHEAP
  /* 67 */ sizeof(Socket),             // CN_SOCKET
  /* 68 */ sizeof(Stack),              // CN_STACK
  /* 69 */ sizeof(StepAccum),          // CN_STEPACCUM
  /* 70 */ sizeof(StepArray),          // CN_STEPARRAY
  /* 71 */ sizeof(SysCall),            // CN_SYSCALL
  
  /* 72 */ sizeof(Table),              // CN_TABLE
  /* 73 */ sizeof(Task),               // CN_TASK
  /* 74 */ sizeof(TaskCursor),         // CN_TASKCURSOR
  /* 75 */ sizeof(TaskList),           // CN_TASKLIST
  /* 76 */ sizeof(TaskSet),            // CN_TASKSET
  /* 77 */ sizeof(Thread),             // CN_THREAD
  /* 78 */ sizeof(ThreadCursor),       // CN_THREADCURSOR
  /* 79 */ sizeof(ThreadList),         // CN_THREADLIST
  /* 80 */ sizeof(Time),               // CN_TIME
  /* 81 */ sizeof(Timer),              // CN_TIMER
  /* 82 */ sizeof(Tokeniser),          // CN_TOKENISER
  /* 83 */ sizeof(Tx),                 // CN_TX
  /* 84 */ sizeof(TxArray),            // CN_TXARRAY
  /* 85 */ sizeof(TxCollection),       // CN_TXCOLLECTION
  /* 86 */ sizeof(TxCursor),           // CN_TXCURSOR
  /* 87 */ sizeof(TxMap),              // CN_TXMAP
  /* 88 */ sizeof(TxRef),              // CN_TXREF

  /* 89 */ sizeof(Word),               // CN_WORD

  /* 90 */ C_SIZE_SMALL_OBJECT  - 256, // CN_ZSMALL_
  /* 91 */ C_SIZE_MEDIUM_OBJECT - 256, // CN_ZMEDIUM_
  /* 92 */ C_SIZE_POOL_BUFFER   - 256, // CN_ZLARGE_
  /* 93 */ C_SIZE_JUMBO_OBJECT  - 256, // CN_ZJUMBO_

  /* 94 */ -1
};

eHT geWhichHeap[]
{
  /*  0 */ HT_INVALID,
  
  /*  1 */ HT_INVALID, // CN_S32
  /*  2 */ HT_INVALID, // CN_S256
  /*  3 */ HT_INVALID, // CN_S48K
  /*  4 */ HT_INVALID, // CN_S1M
  
  /*  5 */ HT_INVALID, // CN_INTVAL
  /*  6 */ HT_INVALID, // CN_IPVAL
  /*  7 */ HT_INVALID, // CN_OBJVAL
  /*  8 */ HT_INVALID, // CN_PTRVAL
  /*  9 */ HT_INVALID, // CN_STRVAL
  /* 10 */ HT_INVALID, // CN_VALREG
  /* 11 */ HT_INVALID, // CN_VALSET
  /* 12 */ HT_INVALID, // CN_VALWSP
  
  /* 13 */ HT_INVALID, // CN_ARGUMENTS
  /* 14 */ HT_INVALID, // CN_ATOMIC
  /* 15 */ HT_LARGE,   // CN_ATTACHMAP
  
  /* 16 */ HT_INVALID, // CN_BUFFERPOOL
  /* 17 */ HT_INVALID, // CN_BPA
  
  /* 18 */ HT_MEDIUM,  // CN_DIC
  /* 19 */ HT_INVALID, // CN_DICSTACK
  /* 20 */ HT_SMALL,   // CN_DIR
  
  /* 21 */ HT_SMALL,   // CN_FILE
  /* 22 */ HT_INVALID, // CN_FREEBUFFER
  /* 23 */ HT_INVALID, // CN_FREECHUNK
  
  /* 24 */ HT_SMALL,   // CN_GRAPH
  /* 25 */ HT_SMALL,   // CN_GRAPHCURSOR
  /* 26 */ HT_SMALL,   // CN_GRAPHNODE
  /* 27 */ HT_INVALID, // CN_GRID

  /* 28 */ HT_LARGE,   // CN_HEAPBUFFER
  
  /* 29 */ HT_INVALID, // CN_INDEX
  
  /* 30 */ HT_INVALID, // CN_JUMBOHEAP
  
  /* 31 */ HT_LARGE,   // CN_KEYCPP
  /* 32 */ HT_LARGE,   // CN_KEYVAL

  /* 33 */ HT_LARGE,   // CN_LOADCHUNK
  /* 33 */ HT_MEDIUM,  // CN_LOADER
  /* 35 */ HT_MEDIUM,  // CN_LOADERSUB
  /* 36 */ HT_JUMBO,   // CN_LOADMAP
  /* 37 */ HT_INVALID, // CN_LOCK
  
  /* 38 */ HT_INVALID, // CN_MEDIUMHEAP
  
  /* 39 */ HT_SMALL,   // CN_NODE
  /* 40 */ HT_SMALL,   // CN_NODECURSOR
  
  /* 41 */ HT_MEDIUM,  // CN_PARSER
  /* 42 */ HT_INVALID, // CN_PARTITION
  /* 43 */ HT_INVALID, // CN_PARTITIONS
  /* 44 */ HT_INVALID, // CN_PROXY
  /* 45 */ HT_INVALID, // CN_PROXYSET

  /* 46 */ HT_SMALL,   // CN_QUEUE
  /* 47 */ HT_LARGE,   // CN_QUEUEBUFFER
  
  /* 48 */ HT_LARGE,   // CN_REGFRAME
  /* 49 */ HT_INVALID, // CN_RING
  /* 50 */ HT_INVALID, // CN_ROW
  /* 51 */ HT_LARGE,   // CN_ROWBUFFER
  /* 52 */ HT_SMALL,   // CN_ROWCURSOR
  /* 53 */ HT_SMALL,   // CN_ROWSTRAND
  /* 54 */ HT_INVALID, // CN_ROWWSP
  
  /* 55 */ HT_LARGE,   // CN_SCANBUFFER
  /* 56 */ HT_LARGE,   // CN_SCRATCH
  /* 57 */ HT_INVALID, // CN_SEHISTORY
  /* 58 */ HT_MEDIUM,  // CN_SEITF
  /* 59 */ HT_SMALL,   // CN_SEITFCURSOR
  /* 60 */ HT_INVALID, // CN_SEITFLIST
  /* 61 */ HT_LARGE,   // CN_SEPROMPT
  /* 62 */ HT_INVALID, // CN_SINCE
  /* 63 */ HT_INVALID, // CN_SKIPCURSOR
  /* 64 */ HT_SMALL,   // CN_SKIPLIST
  /* 65 */ HT_SMALL,   // CN_SKIPNODE
  /* 66 */ HT_INVALID, // CN_SMALLHEAP
  /* 67 */ HT_SMALL,   // CN_SOCKET
  /* 68 */ HT_LARGE,   // CN_STACK
  /* 69 */ HT_INVALID, // CN_STEPACCUM
  /* 70 */ HT_LARGE,   // CN_STEPARRAY
  /* 71 */ HT_INVALID, // CN_SYSCALL
  
  /* 72 */ HT_INVALID, // CN_TABLE
  /* 73 */ HT_MEDIUM,  // CN_TASK
  /* 74 */ HT_INVALID, // CN_TASKCURSOR
  /* 75 */ HT_SMALL,   // CN_TASKLIST
  /* 76 */ HT_INVALID, // CN_TASKSET
  /* 77 */ HT_INVALID, // CN_THREAD
  /* 78 */ HT_SMALL,   // CN_THREADCURSOR
  /* 79 */ HT_INVALID, // CN_THREADLIST
  /* 80 */ HT_INVALID, // CN_TIME
  /* 81 */ HT_SMALL,   // CN_TIMER
  /* 82 */ HT_MEDIUM,  // CN_TOKENISER
  /* 83 */ HT_SMALL,   // CN_TX
  /* 84 */ HT_LARGE,   // CN_TXARRAY
  /* 85 */ HT_INVALID, // CN_TXCOLLECTION
  /* 86 */ HT_SMALL,   // CN_TXCURSOR
  /* 87 */ HT_INVALID, // CN_TXMAP
  /* 88 */ HT_SMALL,   // CN_TXREF
  
  /* 89 */ HT_MEDIUM,  // CN_WORD
  
  /* 90 */ HT_INVALID, // CN_ZSMALL_
  /* 91 */ HT_INVALID, // CN_ZMEDIUM_
  /* 92 */ HT_INVALID, // CN_ZLARGE_
  /* 93 */ HT_INVALID, // CN_ZJUMBO_
  
  /* 94 */ HT_INVALID, // CN_ENUMS
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
  
  vTrace("\n");
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
  vTrace("%s%s: **dump function not defined for class**\n", pTab, pCN[eClass]);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
