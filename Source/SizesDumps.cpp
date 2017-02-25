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
  
  
  /*  5 */ &Dump::vInvalid,         // CN_BINVAL
  /*    */ &Dump::vInvalid,         // CN_ENTWSP
  /*  6 */ &Dump::vInvalid,         // CN_INTVAL
  /*  7 */ &Dump::vInvalid,         // CN_IPVAL
  /*  8 */ &Dump::vInvalid,         // CN_OBJVAL
  /*  9 */ &Dump::vInvalid,         // CN_PTRVAL
  /*  1 */ &Dump::vInvalid,         // CN_STRFXD
  /* 10 */ &Dump::vInvalid,         // CN_STRVAL
  /* 11 */ &Dump::vInvalid,         // CN_VALREG
  /* 12 */ &Dump::vInvalid,         // CN_VALSET
  /* 13 */ &Dump::vInvalid,         // CN_VALWSP
  
  /* 14 */ &Dump::vInvalid,         // CN_ARGUMENTS
  /* 15 */ &Dump::vInvalid,         // CN_ATOMIC
  /* 16 */ &AttachMap::vDump,       // CN_ATTACHMAP

  /* 17 */ &Dump::vInvalid,         // CN_BUFFERPOOL
  /* 18 */ &Dump::vInvalid,         // CN_BPA

  /* 19 */ &Dump::vDummy,           // CN_DATA
  /* 20 */ &Dic::vDump,             // CN_DIC
  /* 21 */ &DicPath::vDump,         // CN_DICPATH
  /* 22 */ &Dump::vInvalid,         // CN_DIR
  
  /* 23 */ &Dump::vDummy,           // CN_ENTRY
  
  /* 24 */ &Dump::vInvalid,         // CN_FILE
  /* 25 */ &Dump::vDummy,           // CN_FREEBUFFER
  /* 26 */ &Dump::vDummy,           // CN_FREECHUNK
  
  /* 27 */ &Graph::vDump,           // CN_GRAPH
  /* 28 */ &GraphCursor::vDump,     // CN_GRAPH
  /* 29 */ &GraphNode::vDump,       // CN_GRAPHNODE
  /* 30 */ &Grid::vDump,            // CN_GRID

  /* 31 */ &HeapBuffer::vDump,      // CN_HEAPBUFFER
  
  /* 32 */ &Dump::vDummy,           // CN_INDEX
  /* 33 */ &Dump::vDummy,           // CN_INDEXBUFFER
  /* 34 */ &IndexDefn::vDump,       // CN_INDEXDEFN
  /* 35 */ &Dump::vDummy,           // CN_INDEXLEVELS

  /* 36 */ &Dump::vDummy,           // CN_JUMBOHEAP

  /* 37 */ &Dump::vInvalid,         // CN_KEYCPP
  /* 38 */ &Dump::vInvalid,         // CN_KEYWSP
  
  /* 39 */ &LoadChunk::vDump,       // CN_LOADCHUNK
  /* 40 */ &Loader::vDump,          // CN_LOADER
  /* 41 */ &LoaderSub::vDump,       // CN_LOADERSUB
  /* 42 */ &LoadMap::vDump,         // CN_LOADMAP
  /* 43 */ &Dump::vInvalid,         // CN_LOCK
  
  /* 44 */ &Heap::vDump,            // CN_MEDIUMHEAP
  
  /* 45 */ &Node::vDump,            // CN_NODE
  /* 46 */ &NodeCursor::vDump,      // CN_NODECURSOR
  
  /* 47 */ &Parser::vDump,          // CN_PARSER
  /* 48 */ &Partition::vDump,       // CN_PARTITION
  /* 49 */ &Plan::vDump,            // CN_PLAN
  /* 50 */ &PlanList::vDump,        // CN_PLANLIST
  /* 51 */ &Dump::vUndefined,       // CN_PROXY
  /* 52 */ &Dump::vUndefined,       // CN_PROXYSET
  
  /* 53 */ &Queue::vDump,           // CN_QUEUE
  /* 54 */ &QueueBuffer::vDump,     // CN_QUEUEBUFFER

  /* 55 */ &Dump::vUndefined,       // CN_RBCURSOR
  /* 56 */ &RegFrame::vDump,        // CN_REGFRAME
  /* 57 */ &Dump::vUndefined,       // CN_RING
  /* 58 */ &Dump::vUndefined,       // CN_ROW
  /* 59 */ &RowBuffer::vDump,       // CN_ROWBUFFER
  /* 60 */ &Dump::vUndefined,       // CN_ROWCURSOR
  /* 61 */ &RowStrand::vDump,       // CN_ROWSTRAND
  /* 62 */ &Dump::vUndefined,       // CN_ROWWSP
  
  /* 63 */ &Dump::vUndefined,       // CN_SCANBUFFER
  /* 64 */ &Dump::vUndefined,       // CN_SCRATCH
  /* 65 */ &Dump::vUndefined,       // CN_SEHISTORY
  /* 66 */ &SEitf::vDump,           // CN_SEITF
  /* 67 */ &Dump::vUndefined,       // CN_SEITFCURSOR
  /* 68 */ &Dump::vUndefined,       // CN_SEITFLIST
  /* 69 */ &Dump::vUndefined,       // CN_SELCURSOR
  /* 70 */ &SePrompt::vDump,        // CN_SEPROMPT
  /* 71 */ &Dump::vUndefined,       // CN_SINCE
  /* 72 */ &Dump::vUndefined,       // CN_SKIPCURSOR
  /* 73 */ &Dump::vUndefined,       // CN_SKIPLIST
  /* 74 */ &Dump::vUndefined,       // CN_SKIPNODE
  /* 75 */ &Dump::vUndefined,       // CN_SLOTS
  /* 76 */ &Heap::vDump,            // CN_SMALLHEAP
  /* 77 */ &Socket::vDump,          // CN_SOCKET
  /* 78 */ &Stack::vDump,           // CN_STACK
  /* 79 */ &Dump::vInvalid,         // CN_STEPACCUM
  /* 80 */ &StepArray::vDump,       // CN_STEPARRAY
  /* 81 */ &Dump::vInvalid,         // CN_SYSCALL

  /* 82 */ &Table::vDump,           // CN_TABLE
  /* 83 */ &Task::vDump,            // CN_TASK
  /* 84 */ &Dump::vUndefined,       // CN_TASKCURSOR
  /* 85 */ &TaskList::vDump,        // CN_TASKLIST
  /* 86 */ &Dump::vUndefined,       // CN_TASKSET
  /* 87 */ &Dump::vUndefined,       // CN_THREAD
  /* 88 */ &Dump::vUndefined,       // CN_THREADCURSOR
  /* 89 */ &Dump::vUndefined,       // CN_THREADLIST
  /* 90 */ &Dump::vUndefined,       // CN_TIME
  /* 91 */ &Dump::vInvalid,         // CN_TIMER
  /* 92 */ &Tokeniser::vDump,       // CN_TOKENISER
  /* 93 */ &Tx::vDump,              // CN_TX
  /* 94 */ &TxArray::vDump,         // CN_TXARRAY
  /* 95 */ &Dump::vUndefined,       // CN_TXCOLLECTION
  /* 96 */ &Dump::vUndefined,       // CN_TXCURSOR
  /* 97 */ &Dump::vUndefined,       // CN_TXMAP
  /* 98 */ &Dump::vUndefined,       // CN_TXREF

  /* 99 */ &Word::vDump,            // CN_WORD
  
  /*100 */ &Dump::vUndefined,       // CN_ZSMALL_
  /*101 */ &Dump::vUndefined,       // CN_ZMEDIUM_
  /*102 */ &Dump::vUndefined,       // CN_ZLARGE_
  /*103 */ &Dump::vUndefined,       // CN_ZJUMBO_
  
  /*104 */ &Dump::vEnums            // CN_ENUMS
};

i8 giClassSize[]
{
  /*  0 */ 0,
  
  
  /*  5 */ sizeof(BinVal),             // CN_BINVAL
  /*    */ 0,                          // CN_ENTWSP
  /*  6 */ sizeof(IntVal),             // CN_INTVAL
  /*  7 */ sizeof(PtrVal),             // CN_IPVAL
  /*  8 */ sizeof(ObjVal),             // CN_OBJVAL
  /*  9 */ sizeof(PtrVal),             // CN_PTRVAL
  /*  1 */ sizeof(s32),                // CN_STRFXD
  /* 10 */ sizeof(StrVal),             // CN_STRVAL
  /* 11 */ sizeof(ValReg),             // CN_VALREG
  /* 12 */ sizeof(ValWsp), // correct  // CN_VALSET
  /* 13 */ sizeof(ValWsp),             // CN_VALWSP
  
  /* 14 */ sizeof(Arguments),          // CN_ARGUMENTS
  /* 15 */ sizeof(Atomic),             // CN_ATOMIC
  /* 16 */ sizeof(AttachMap),          // CN_ATTACHMAP
  
  /* 17 */ sizeof(BufferPool),         // CN_BUFFERPOOL
  /* 18 */ sizeof(BPA),                // CN_BPA
  
  /* 19 */ sizeof(Data),               // CN_DATA
  /* 20 */ sizeof(Dic),                // CN_DIC
  /* 21 */ sizeof(DicPath),            // CN_DICPATH
  /* 22 */ sizeof(Dir),                // CN_DIR
  
  /* 23 */ 0,                          // CN_ENTRY
  
  /* 24 */ sizeof(File),               // CN_FILE
  /* 25 */ sizeof(Buffer),             // CN_FREEBUFFER
  /* 26 */ sizeof(FreeChunk),          // CN_FREECHUNK
  
  /* 27 */ sizeof(Graph),              // CN_GRAPH
  /* 28 */ sizeof(GraphCursor),        // CN_GRAPHCURSOR
  /* 29 */ sizeof(GraphNode),          // CN_GRAPHNODE
  /* 30 */ sizeof(Grid),               // CN_GRID
  
  /* 31 */ sizeof(HeapBuffer),         // CN_HEAPBUFFER
  
  /* 32 */ 0,                          // CN_INDEX
  /* 33 */ 0,                          // CN_INDEXBUFFER
  /* 34 */ sizeof(IndexDefn),          // CN_INDEXDEFN
  /* 35 */ 0,                          // CN_INDEXLEVELS
  
  /* 36 */ sizeof(Heap), // correct    // CN_JUMBOHEAP
  
  /* 37 */ 0,                          // CN_KEYCPP
  /* 38 */ sizeof(KeyWsp),             // CN_KEYWSP
  
  /* 39 */ sizeof(LoadChunk),          // CN_LOADCHUNK
  /* 40 */ sizeof(Loader),             // CN_LOADER
  /* 41 */ sizeof(LoaderSub),          // CN_LOADERSUB
  /* 42 */ sizeof(LoadMap),            // CN_LOADMAP
  /* 43 */ sizeof(Lock),               // CN_LOCK
  
  /* 44 */ sizeof(Heap), // correct    // CN_MEDIUMHEAP
  
  /* 45 */ sizeof(Node),               // CN_NODE
  /* 46 */ sizeof(NodeCursor),         // CN_NODECURSOR
  
  /* 47 */ sizeof(Parser),             // CN_PARSER
  /* 48 */ sizeof(Partition),          // CN_PARTITION
  /* 49 */ sizeof(Plan),               // CN_PLAN
  /* 50 */ sizeof(PlanList),           // CN_PLANLIST
  /* 51 */ sizeof(Proxy),              // CN_PROXY
  /* 52 */ sizeof(ProxySet),           // CN_PROXYSET
  
  /* 53 */ sizeof(Queue),              // CN_QUEUE
  /* 54 */ sizeof(QueueBuffer),        // CN_QUEUEBUFFER
  
  /* 55 */ sizeof(RbCursor),           // CN_RBCURSOR
  /* 56 */ sizeof(RegFrame),           // CN_REGFRAME
  /* 57 */ sizeof(Ring),               // CN_RING
  /* 58 */ sizeof(Row),                // CN_ROW
  /* 59 */ sizeof(RowBuffer),          // CN_ROWBUFFER
  /* 60 */ 0,                          // CN_ROWCURSOR
  /* 61 */ sizeof(RowStrand),          // CN_ROWSTRAND
  /* 62 */ sizeof(RowWsp),             // CN_ROWWSP
  
  /* 63 */ sizeof(ScanBuffer),         // CN_SCANBUFFER
  /* 64 */ sizeof(Scratch),            // CN_SCRATCH
  /* 65 */ sizeof(SeHistory),          // CN_SEHISTORY
  /* 66 */ sizeof(SEitf),              // CN_SEITF
  /* 67 */ sizeof(SEitfCursor),        // CN_SEITFCURSOR
  /* 68 */ sizeof(SEitfList),          // CN_SEITFLIST
  /* 69 */ sizeof(SelCursor),          // CN_SELCURSOR
  /* 70 */ sizeof(SePrompt),           // CN_SEPROMPT
  /* 71 */ sizeof(Since),              // CN_SINCE
  /* 72 */ sizeof(SkipCursor),         // CN_SKIPCURSOR
  /* 73 */ sizeof(SkipList),           // CN_SKIPLIST
  /* 74 */ sizeof(SkipNode),           // CN_SKIPNODE
  /* 75 */ sizeof(Slots),              // CN_SLOTS
  /* 76 */ sizeof(Heap), // correct    // CN_SMALLHEAP
  /* 77 */ sizeof(Socket),             // CN_SOCKET
  /* 78 */ sizeof(Stack),              // CN_STACK
  /* 79 */ sizeof(StepAccum),          // CN_STEPACCUM
  /* 80 */ sizeof(StepArray),          // CN_STEPARRAY
  /* 81 */ sizeof(SysCall),            // CN_SYSCALL
  
  /* 82 */ sizeof(Table),              // CN_TABLE
  /* 83 */ sizeof(Task),               // CN_TASK
  /* 84 */ sizeof(TaskCursor),         // CN_TASKCURSOR
  /* 85 */ sizeof(TaskList),           // CN_TASKLIST
  /* 86 */ sizeof(TaskSet),            // CN_TASKSET
  /* 87 */ sizeof(Thread),             // CN_THREAD
  /* 88 */ sizeof(ThreadCursor),       // CN_THREADCURSOR
  /* 89 */ sizeof(ThreadList),         // CN_THREADLIST
  /* 90 */ sizeof(Time),               // CN_TIME
  /* 91 */ sizeof(Timer),              // CN_TIMER
  /* 92 */ sizeof(Tokeniser),          // CN_TOKENISER
  /* 93 */ sizeof(Tx),                 // CN_TX
  /* 94 */ sizeof(TxArray),            // CN_TXARRAY
  /* 95 */ sizeof(TxCollection),       // CN_TXCOLLECTION
  /* 96 */ sizeof(TxCursor),           // CN_TXCURSOR
  /* 97 */ sizeof(TxMap),              // CN_TXMAP
  /* 98 */ sizeof(TxRef),              // CN_TXREF

  /* 99 */ sizeof(Word),               // CN_WORD

  /*100 */ C_SIZE_SMALL_OBJECT  - 256, // CN_ZSMALL_
  /*101 */ C_SIZE_MEDIUM_OBJECT - 256, // CN_ZMEDIUM_
  /*102 */ C_SIZE_POOL_BUFFER   - 256, // CN_ZLARGE_
  /*103 */ C_SIZE_JUMBO_OBJECT  - 256, // CN_ZJUMBO_

  /*104 */ -1
};

eHT geWhichHeap[]
{
  /*  0 */ HT_INVALID,
  
  
  /*  5 */ HT_INVALID, // CN_BINVAL
  /*    */ HT_INVALID, // CN_ENTWSP,
  /*  6 */ HT_INVALID, // CN_INTVAL
  /*  7 */ HT_INVALID, // CN_IPVAL
  /*  8 */ HT_INVALID, // CN_OBJVAL
  /*  9 */ HT_INVALID, // CN_PTRVAL
  /*  1 */ HT_INVALID, // CN_STRFXD
  /* 10 */ HT_INVALID, // CN_STRVAL
  /* 11 */ HT_INVALID, // CN_VALREG
  /* 12 */ HT_INVALID, // CN_VALSET
  /* 13 */ HT_INVALID, // CN_VALWSP
  
  /* 14 */ HT_INVALID, // CN_ARGUMENTS
  /* 15 */ HT_INVALID, // CN_ATOMIC
  /* 16 */ HT_LARGE,   // CN_ATTACHMAP
  
  /* 17 */ HT_INVALID, // CN_BUFFERPOOL
  /* 18 */ HT_INVALID, // CN_BPA
  
  /* 19 */ HT_SMALL,   // CN_DATA
  /* 20 */ HT_MEDIUM,  // CN_DIC
  /* 21 */ HT_INVALID, // CN_DICPATH
  /* 22 */ HT_SMALL,   // CN_DIR

  /* 23 */ HT_INVALID, // CN_ENTRY
  
  /* 24 */ HT_SMALL,   // CN_FILE
  /* 25 */ HT_INVALID, // CN_FREEBUFFER
  /* 26 */ HT_INVALID, // CN_FREECHUNK
  
  /* 27 */ HT_SMALL,   // CN_GRAPH
  /* 28 */ HT_SMALL,   // CN_GRAPHCURSOR
  /* 29 */ HT_SMALL,   // CN_GRAPHNODE
  /* 30 */ HT_INVALID, // CN_GRID

  /* 31 */ HT_LARGE,   // CN_HEAPBUFFER
  
  /* 32 */ HT_INVALID, // CN_INDEX
  /* 33 */ HT_LARGE,   // CN_INDEXBUFFER
  /* 34 */ HT_INVALID, // CN_INDEXDEFN
  /* 35 */ HT_INVALID, // CN_INDEXLEVELS

  /* 36 */ HT_INVALID, // CN_JUMBOHEAP
  
  /* 37 */ HT_LARGE,   // CN_KEYCPP
  /* 38 */ HT_INVALID, // CN_KEYWSP

  /* 39 */ HT_LARGE,   // CN_LOADCHUNK
  /* 40 */ HT_MEDIUM,  // CN_LOADER
  /* 41 */ HT_MEDIUM,  // CN_LOADERSUB
  /* 42 */ HT_JUMBO,   // CN_LOADMAP
  /* 43 */ HT_INVALID, // CN_LOCK
  
  /* 44 */ HT_INVALID, // CN_MEDIUMHEAP
  
  /* 45 */ HT_SMALL,   // CN_NODE
  /* 46 */ HT_SMALL,   // CN_NODECURSOR
  
  /* 47 */ HT_MEDIUM,  // CN_PARSER
  /* 48 */ HT_INVALID, // CN_PARTITION
  /* 49 */ HT_MEDIUM,  // CN_PLAN
  /* 50 */ HT_SMALL,   // CN_PLANLIST
  /* 51 */ HT_INVALID, // CN_PROXY
  /* 52 */ HT_INVALID, // CN_PROXYSET

  /* 53 */ HT_SMALL,   // CN_QUEUE
  /* 54 */ HT_LARGE,   // CN_QUEUEBUFFER
  
  /* 55 */ HT_SMALL,   // CN_RBCURSOR
  /* 56 */ HT_LARGE,   // CN_REGFRAME
  /* 57 */ HT_INVALID, // CN_RING
  /* 58 */ HT_INVALID, // CN_ROW
  /* 59 */ HT_LARGE,   // CN_ROWBUFFER
  /* 60 */ HT_SMALL,   // CN_ROWCURSOR
  /* 61 */ HT_SMALL,   // CN_ROWSTRAND
  /* 62 */ HT_INVALID, // CN_ROWWSP
  
  /* 63 */ HT_LARGE,   // CN_SCANBUFFER
  /* 64 */ HT_LARGE,   // CN_SCRATCH
  /* 65 */ HT_INVALID, // CN_SEHISTORY
  /* 66 */ HT_MEDIUM,  // CN_SEITF
  /* 67 */ HT_SMALL,   // CN_SEITFCURSOR
  /* 68 */ HT_INVALID, // CN_SEITFLIST
  /* 69 */ HT_SMALL,   // CN_SELCURSOR
  /* 70 */ HT_LARGE,   // CN_SEPROMPT
  /* 71 */ HT_INVALID, // CN_SINCE
  /* 72 */ HT_INVALID, // CN_SKIPCURSOR
  /* 73 */ HT_SMALL,   // CN_SKIPLIST
  /* 74 */ HT_SMALL,   // CN_SKIPNODE
  /* 75 */ HT_INVALID, // CN_SLOTS
  /* 76 */ HT_INVALID, // CN_SMALLHEAP
  /* 77 */ HT_SMALL,   // CN_SOCKET
  /* 78 */ HT_LARGE,   // CN_STACK
  /* 79 */ HT_INVALID, // CN_STEPACCUM
  /* 80 */ HT_LARGE,   // CN_STEPARRAY
  /* 81 */ HT_INVALID, // CN_SYSCALL
  
  /* 82 */ HT_INVALID, // CN_TABLE
  /* 83 */ HT_MEDIUM,  // CN_TASK
  /* 84 */ HT_INVALID, // CN_TASKCURSOR
  /* 85 */ HT_SMALL,   // CN_TASKLIST
  /* 86 */ HT_INVALID, // CN_TASKSET
  /* 87 */ HT_INVALID, // CN_THREAD
  /* 88 */ HT_SMALL,   // CN_THREADCURSOR
  /* 89 */ HT_INVALID, // CN_THREADLIST
  /* 90 */ HT_INVALID, // CN_TIME
  /* 91 */ HT_SMALL,   // CN_TIMER
  /* 92 */ HT_MEDIUM,  // CN_TOKENISER
  /* 93 */ HT_SMALL,   // CN_TX
  /* 94 */ HT_LARGE,   // CN_TXARRAY
  /* 95 */ HT_INVALID, // CN_TXCOLLECTION
  /* 96 */ HT_SMALL,   // CN_TXCURSOR
  /* 97 */ HT_INVALID, // CN_TXMAP
  /* 98 */ HT_SMALL,   // CN_TXREF
  
  /* 99 */ HT_MEDIUM,  // CN_WORD
  
  /*100 */ HT_INVALID, // CN_ZSMALL_
  /*101 */ HT_INVALID, // CN_ZMEDIUM_
  /*102 */ HT_INVALID, // CN_ZLARGE_
  /*103 */ HT_INVALID, // CN_ZJUMBO_

  /*104 */ HT_INVALID, // CN_ENUMS
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
