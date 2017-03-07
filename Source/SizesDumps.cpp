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
  
  
  /*  1 */ &Dump::vInvalid,         // CN_BINVAL
  /*  2 */ &Dump::vInvalid,         // CN_ENTWSP
  /*  3 */ &Dump::vInvalid,         // CN_INTVAL
  /*  4 */ &Dump::vInvalid,         // CN_IPVAL
  /*  5 */ &Dump::vInvalid,         // CN_OBJVAL
  /*  6 */ &Dump::vInvalid,         // CN_PTRVAL
  /*  7 */ &Dump::vInvalid,         // CN_STRVAL
  /*  8 */ &Dump::vInvalid,         // CN_STRVAR
  /*  9 */ &Dump::vInvalid,         // CN_VALREG
  /* 10 */ &Dump::vInvalid,         // CN_VALSET
  /* 11 */ &Dump::vInvalid,         // CN_VALWSP
  
  /* 12 */ &Dump::vInvalid,         // CN_ARGUMENTS
  /* 13 */ &Dump::vInvalid,         // CN_ATOMIC
  /* 14 */ &AttachMap::vDump,       // CN_ATTACHMAP

  /* 15 */ &Dump::vInvalid,         // CN_BUFFERPOOL
  /* 16 */ &Dump::vInvalid,         // CN_BPA

  /* 17 */ &Dump::vDummy,           // CN_DATA
  /* 18 */ &Dic::vDump,             // CN_DIC
  /* 19 */ &DicPath::vDump,         // CN_DICPATH
  /* 20 */ &Dump::vInvalid,         // CN_DIR
  
  /* 21 */ &Dump::vDummy,           // CN_ENTRY
  
  /* 22 */ &Dump::vInvalid,         // CN_FILE
  /* 23 */ &Dump::vDummy,           // CN_FREEBUFFER
  /* 24 */ &Dump::vDummy,           // CN_FREECHUNK
  
  /* 25 */ &Graph::vDump,           // CN_GRAPH
  /* 26 */ &GraphCursor::vDump,     // CN_GRAPH
  /* 27 */ &GraphNode::vDump,       // CN_GRAPHNODE
  /* 28 */ &Grid::vDump,            // CN_GRID

  /* 29 */ &HeapBuffer::vDump,      // CN_HEAPBUFFER
  
  /* 30 */ &Dump::vDummy,           // CN_INDEX
  /* 31 */ &Dump::vDummy,           // CN_INDEXBUFFER
  /* 32 */ //&IndexDefn::vDump,       // CN_INDEXDEFN
  /* 33 */ &Dump::vDummy,           // CN_INDEXLEVELS

  /* 34 */ &Dump::vDummy,           // CN_JUMBOHEAP

  /* 35 */ &Dump::vInvalid,         // CN_KEYWSP
  
  /* 36 */ &LoadChunk::vDump,       // CN_LOADCHUNK
  /* 37 */ &Loader::vDump,          // CN_LOADER
  /* 38 */ &LoaderSub::vDump,       // CN_LOADERSUB
  /* 39 */ &LoadMap::vDump,         // CN_LOADMAP
  /* 40 */ &Dump::vInvalid,         // CN_LOCK
  
  /* 41 */ &Heap::vDump,            // CN_MEDIUMHEAP
  
  /* 42 */ &Node::vDump,            // CN_NODE
  /* 43 */ &NodeCursor::vDump,      // CN_NODECURSOR
  
  /* 44 */ &Parser::vDump,          // CN_PARSER
  /* 45 */ &Partition::vDump,       // CN_PARTITION
  /* 46 */ &Plan::vDump,            // CN_PLAN
  /* 47 */ &PlanList::vDump,        // CN_PLANLIST
  /* 48 */ &Dump::vUndefined,       // CN_PROXY
  /* 49 */ &Dump::vUndefined,       // CN_PROXYSET
  
  /* 50 */ &Queue::vDump,           // CN_QUEUE
  /* 51 */ &QueueBuffer::vDump,     // CN_QUEUEBUFFER

  /* 52 */ &Dump::vUndefined,       // CN_RBCURSOR
  /* 53 */ &RegFrame::vDump,        // CN_REGFRAME
  /* 54 */ &Dump::vUndefined,       // CN_RING
  /* 55 */ &Dump::vUndefined,       // CN_ROW
  /* 56 */ &RowBuffer::vDump,       // CN_ROWBUFFER
  /* 57 */ &Dump::vUndefined,       // CN_ROWCURSOR
  /* 58 */ &RowStrand::vDump,       // CN_ROWSTRAND
  /* 59 */ &Dump::vUndefined,       // CN_ROWWSP
  
  /* 60 */ &Dump::vUndefined,       // CN_SCANBUFFER
  /* 61 */ &Dump::vUndefined,       // CN_SCRATCH
  /* 62 */ &Dump::vUndefined,       // CN_SEHISTORY
  /* 63 */ &SEitf::vDump,           // CN_SEITF
  /* 64 */ &Dump::vUndefined,       // CN_SEITFCURSOR
  /* 65 */ &Dump::vUndefined,       // CN_SEITFLIST
  /* 66 */ &Dump::vUndefined,       // CN_SELCURSOR
  /* 67 */ &SePrompt::vDump,        // CN_SEPROMPT
  /* 68 */ &Dump::vUndefined,       // CN_SINCE
  /* 69 */ &Dump::vUndefined,       // CN_SKIPCURSOR
  /* 70 */ &Dump::vUndefined,       // CN_SKIPLIST
  /* 71 */ &Dump::vUndefined,       // CN_SKIPNODE
  /* 72 */ &Dump::vUndefined,       // CN_SLOTS
  /* 73 */ &Heap::vDump,            // CN_SMALLHEAP
  /* 74 */ &Socket::vDump,          // CN_SOCKET
  /* 75 */ &Stack::vDump,           // CN_STACK
  /* 76 */ &Dump::vInvalid,         // CN_STEPACCUM
  /* 77 */ &StepArray::vDump,       // CN_STEPARRAY
  /* 78 */ &Dump::vInvalid,         // CN_SYSCALL

  /* 79 */ &Table::vDump,           // CN_TABLE
  /* 80 */ &Task::vDump,            // CN_TASK
  /* 81 */ &Dump::vUndefined,       // CN_TASKCURSOR
  /* 82 */ &TaskList::vDump,        // CN_TASKLIST
  /* 83 */ &Dump::vUndefined,       // CN_TASKSET
  /* 84 */ &Dump::vUndefined,       // CN_THREAD
  /* 85 */ &Dump::vUndefined,       // CN_THREADCURSOR
  /* 86 */ &Dump::vUndefined,       // CN_THREADLIST
  /* 87 */ &Dump::vUndefined,       // CN_TIME
  /* 88 */ &Dump::vInvalid,         // CN_TIMER
  /* 89 */ &Tokeniser::vDump,       // CN_TOKENISER
  /* 90 */ &Tx::vDump,              // CN_TX
  /* 91 */ &TxArray::vDump,         // CN_TXARRAY
  /* 92 */ &Dump::vUndefined,       // CN_TXCOLLECTION
  /* 93 */ &Dump::vUndefined,       // CN_TXCURSOR
  /* 94 */ &Dump::vUndefined,       // CN_TXMAP
  /* 95 */ &Dump::vUndefined,       // CN_TXREF

  /* 96 */ &Word::vDump,            // CN_WORD
  
  /* 97 */ &Dump::vUndefined,       // CN_ZSMALL_
  /* 98 */ &Dump::vUndefined,       // CN_ZMEDIUM_
  /* 99 */ &Dump::vUndefined,       // CN_ZLARGE_
  /*100 */ &Dump::vUndefined,       // CN_ZJUMBO_
  
  /*101 */ &Dump::vEnums            // CN_ENUMS
};

i8 giClassSize[]
{
  /*  0 */ 0,
  
  
  /*  1 */ sizeof(BinVal),             // CN_BINVAL
  /*  2 */ 0,                          // CN_ENTWSP
  /*  3 */ sizeof(IntVal),             // CN_INTVAL
  /*  4 */ sizeof(PtrVal),             // CN_IPVAL
  /*  5 */ sizeof(ObjVal),             // CN_OBJVAL
  /*  6 */ sizeof(PtrVal),             // CN_PTRVAL
  /*  7 */ sizeof(StrVal),             // CN_STRVAL
  /*  8 */ sizeof(s32),                // CN_STRVAR
  /*  9 */ sizeof(ValReg),             // CN_VALREG
  /* 10 */ sizeof(ValWsp), // correct  // CN_VALSET
  /* 11 */ sizeof(ValWsp),             // CN_VALWSP
  
  /* 12 */ sizeof(Arguments),          // CN_ARGUMENTS
  /* 13 */ sizeof(Atomic),             // CN_ATOMIC
  /* 14 */ sizeof(AttachMap),          // CN_ATTACHMAP
  
  /* 15 */ sizeof(BufferPool),         // CN_BUFFERPOOL
  /* 16 */ sizeof(BPA),                // CN_BPA
  
  /* 17 */ sizeof(Data),               // CN_DATA
  /* 18 */ sizeof(Dic),                // CN_DIC
  /* 19 */ sizeof(DicPath),            // CN_DICPATH
  /* 20 */ sizeof(Dir),                // CN_DIR
  
  /* 21 */ 0,                          // CN_ENTRY
  
  /* 22 */ sizeof(File),               // CN_FILE
  /* 23 */ sizeof(Buffer),             // CN_FREEBUFFER
  /* 24 */ sizeof(FreeChunk),          // CN_FREECHUNK
  
  /* 25 */ sizeof(Graph),              // CN_GRAPH
  /* 26 */ sizeof(GraphCursor),        // CN_GRAPHCURSOR
  /* 27 */ sizeof(GraphNode),          // CN_GRAPHNODE
  /* 28 */ sizeof(Grid),               // CN_GRID
  
  /* 29 */ sizeof(HeapBuffer),         // CN_HEAPBUFFER
  
  /* 30 */ 0,                          // CN_INDEX
  /* 31 */ 0,                          // CN_INDEXBUFFER
  /* 32 */ //sizeof(IndexDefn),          // CN_INDEXDEFN
  /* 33 */ 0,                          // CN_INDEXLEVELS
  
  /* 34 */ sizeof(Heap), // correct    // CN_JUMBOHEAP
  
  /* 35 */ sizeof(KeyWsp),             // CN_KEYWSP
  
  /* 36 */ sizeof(LoadChunk),          // CN_LOADCHUNK
  /* 37 */ sizeof(Loader),             // CN_LOADER
  /* 38 */ sizeof(LoaderSub),          // CN_LOADERSUB
  /* 39 */ sizeof(LoadMap),            // CN_LOADMAP
  /* 40 */ sizeof(Lock),               // CN_LOCK
  
  /* 41 */ sizeof(Heap), // correct    // CN_MEDIUMHEAP
  
  /* 42 */ sizeof(Node),               // CN_NODE
  /* 43 */ sizeof(NodeCursor),         // CN_NODECURSOR
  
  /* 44 */ sizeof(Parser),             // CN_PARSER
  /* 45 */ sizeof(Partition),          // CN_PARTITION
  /* 46 */ sizeof(Plan),               // CN_PLAN
  /* 47 */ sizeof(PlanList),           // CN_PLANLIST
  /* 48 */ sizeof(Proxy),              // CN_PROXY
  /* 49 */ sizeof(ProxySet),           // CN_PROXYSET
  
  /* 50 */ sizeof(Queue),              // CN_QUEUE
  /* 51 */ sizeof(QueueBuffer),        // CN_QUEUEBUFFER
  
  /* 52 */ sizeof(RbCursor),           // CN_RBCURSOR
  /* 53 */ sizeof(RegFrame),           // CN_REGFRAME
  /* 54 */ sizeof(Ring),               // CN_RING
  /* 55 */ sizeof(Row),                // CN_ROW
  /* 56 */ sizeof(RowBuffer),          // CN_ROWBUFFER
  /* 57 */ 0,                          // CN_ROWCURSOR
  /* 58 */ sizeof(RowStrand),          // CN_ROWSTRAND
  /* 59 */ sizeof(RowWsp),             // CN_ROWWSP
  
  /* 60 */ sizeof(ScanBuffer),         // CN_SCANBUFFER
  /* 61 */ sizeof(Scratch),            // CN_SCRATCH
  /* 62 */ sizeof(SeHistory),          // CN_SEHISTORY
  /* 63 */ sizeof(SEitf),              // CN_SEITF
  /* 64 */ sizeof(SEitfCursor),        // CN_SEITFCURSOR
  /* 65 */ sizeof(SEitfList),          // CN_SEITFLIST
  /* 66 */ sizeof(SelCursor),          // CN_SELCURSOR
  /* 67 */ sizeof(SePrompt),           // CN_SEPROMPT
  /* 68 */ sizeof(Since),              // CN_SINCE
  /* 69 */ sizeof(SkipCursor),         // CN_SKIPCURSOR
  /* 70 */ sizeof(SkipList),           // CN_SKIPLIST
  /* 71 */ sizeof(SkipNode),           // CN_SKIPNODE
  /* 72 */ sizeof(Slots),              // CN_SLOTS
  /* 73 */ sizeof(Heap), // correct    // CN_SMALLHEAP
  /* 74 */ sizeof(Socket),             // CN_SOCKET
  /* 75 */ sizeof(Stack),              // CN_STACK
  /* 76 */ sizeof(StepAccum),          // CN_STEPACCUM
  /* 77 */ sizeof(StepArray),          // CN_STEPARRAY
  /* 78 */ sizeof(SysCall),            // CN_SYSCALL
  
  /* 79 */ sizeof(Table),              // CN_TABLE
  /* 80 */ sizeof(Task),               // CN_TASK
  /* 81 */ sizeof(TaskCursor),         // CN_TASKCURSOR
  /* 82 */ sizeof(TaskList),           // CN_TASKLIST
  /* 83 */ sizeof(TaskSet),            // CN_TASKSET
  /* 84 */ sizeof(Thread),             // CN_THREAD
  /* 85 */ sizeof(ThreadCursor),       // CN_THREADCURSOR
  /* 86 */ sizeof(ThreadList),         // CN_THREADLIST
  /* 87 */ sizeof(Time),               // CN_TIME
  /* 88 */ sizeof(Timer),              // CN_TIMER
  /* 89 */ sizeof(Tokeniser),          // CN_TOKENISER
  /* 90 */ sizeof(Tx),                 // CN_TX
  /* 91 */ sizeof(TxArray),            // CN_TXARRAY
  /* 92 */ sizeof(TxCollection),       // CN_TXCOLLECTION
  /* 93 */ sizeof(TxCursor),           // CN_TXCURSOR
  /* 94 */ sizeof(TxMap),              // CN_TXMAP
  /* 95 */ sizeof(TxRef),              // CN_TXREF

  /* 96 */ sizeof(Word),               // CN_WORD

  /* 97 */ C_SIZE_SMALL_OBJECT  - 256, // CN_ZSMALL_
  /* 98 */ C_SIZE_MEDIUM_OBJECT - 256, // CN_ZMEDIUM_
  /* 99 */ C_SIZE_POOL_BUFFER   - 256, // CN_ZLARGE_
  /*100 */ C_SIZE_JUMBO_OBJECT  - 256, // CN_ZJUMBO_

  /*101 */ -1
};

eHT geWhichHeap[]
{
  /*  0 */ HT_INVALID,
  
  /*  1 */ HT_INVALID, // CN_BINVAL
  /*  2 */ HT_INVALID, // CN_ENTWSP,
  /*  3 */ HT_INVALID, // CN_INTVAL
  /*  4 */ HT_INVALID, // CN_IPVAL
  /*  5 */ HT_INVALID, // CN_OBJVAL
  /*  6 */ HT_INVALID, // CN_PTRVAL
  /*  7 */ HT_INVALID, // CN_STRVAL
  /*  8 */ HT_INVALID, // CN_STRVAR
  /*  9 */ HT_INVALID, // CN_VALREG
  /* 10 */ HT_INVALID, // CN_VALSET
  /* 11 */ HT_INVALID, // CN_VALWSP
  
  /* 12 */ HT_INVALID, // CN_ARGUMENTS
  /* 13 */ HT_INVALID, // CN_ATOMIC
  /* 14 */ HT_LARGE,   // CN_ATTACHMAP
  
  /* 15 */ HT_INVALID, // CN_BUFFERPOOL
  /* 16 */ HT_INVALID, // CN_BPA
  
  /* 17 */ HT_SMALL,   // CN_DATA
  /* 18 */ HT_MEDIUM,  // CN_DIC
  /* 19 */ HT_INVALID, // CN_DICPATH
  /* 20 */ HT_SMALL,   // CN_DIR

  /* 21 */ HT_INVALID, // CN_ENTRY
  
  /* 22 */ HT_SMALL,   // CN_FILE
  /* 23 */ HT_INVALID, // CN_FREEBUFFER
  /* 24 */ HT_INVALID, // CN_FREECHUNK
  
  /* 25 */ HT_SMALL,   // CN_GRAPH
  /* 26 */ HT_SMALL,   // CN_GRAPHCURSOR
  /* 27 */ HT_SMALL,   // CN_GRAPHNODE
  /* 28 */ HT_INVALID, // CN_GRID

  /* 31 */ HT_LARGE,   // CN_HEAPBUFFER
  
  /* 32 */ HT_INVALID, // CN_INDEX
  /* 33 */ HT_LARGE,   // CN_INDEXBUFFER
  /* 34 */ //HT_INVALID, // CN_INDEXDEFN
  /* 35 */ HT_MEDIUM, // CN_INDEXLEVELS

  /* 36 */ HT_INVALID, // CN_JUMBOHEAP
  
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
void Dump::vDummy(eCN eClass, Object* pObject, const char* aTab)
{
  ASSERT(eClass != CN_NONE);
  ASSERT(pObject != nullptr);
  ASSERT(aTab != nullptr);
  // do nothing
  
  vTrace("\n");
}

/*----------------------------------------------------------------------------*/
/* dummy dump                                                                 */
/*----------------------------------------------------------------------------*/
void Dump::vEnums(eCN eClass, Object* pObject, const char* aTab)
{
  ASSERT(eClass == CN_ENUMS);
  ASSERT(pObject != nullptr);
  ASSERT(aTab != nullptr);
  // do nothing
}

/*----------------------------------------------------------------------------*/
/* dump this attachmap                                                        */
/*----------------------------------------------------------------------------*/
void Dump::vInvalid(eCN eClass, Object* pObject, const char* aTab)
{
  ASSERT(eClass != CN_ENUMS);
  ASSERT(pObject != nullptr);
  ASSERT(aTab != nullptr);
  vTrace("%sInvalid Class %s in Buffer Pool\n"_Str, aTab, aCN[eClass]);
}

/*----------------------------------------------------------------------------*/
/* dump not defined                                                           */
/*----------------------------------------------------------------------------*/
void Dump::vUndefined(eCN eClass, Object* pObject, const char* aTab)
{
  ASSERT(eClass != CN_NONE);
  ASSERT(pObject != nullptr);
  ASSERT(aTab != nullptr);
  vTrace("%s%s: **dump function not defined for class**\n"_Str, aTab, aCN[eClass]);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
