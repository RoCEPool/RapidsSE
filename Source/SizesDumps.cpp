/*============================================================================*/
/* Dump.cpp -- Class Dumps                                                    */
/*============================================================================*/

#include "StorageEngine.h"
#include "SePrompt.hpp"

// Object's : Class::vDump
// Object's with no vDump defined : Dump::vUndefined
// Header   : Dump::vInvalid
// Objects we don't want to dump (eg Word) : Dump::vDummy

pDUMPFUN *gpDump[]
{
  /*  0 */ &Dump::vInvalid,         // CN_NONE
  
  
  /*  1 */ &Dump::vInvalid,         // CN_BCDVAL
  /*  2 */ &Dump::vInvalid,         // CN_BINVAL
  /*  3 */ &Dump::vInvalid,         // CN_BOLVAL
  /*  4 */ &Dump::vInvalid,         // CN_DTEVAL
  /*  5 */ &Dump::vInvalid,         // CN_ENTWRK
  /*  6 */ &Dump::vInvalid,         // CN_FLTVAL
  /*  7 */ &Dump::vInvalid,         // CN_INTVAL
  /*  8 */ &Dump::vInvalid,         // CN_IPVAL
  /*  9 */ &Dump::vInvalid,         // CN_IVLVAL
  /* 10 */ &Dump::vInvalid,         // CN_OBJVAL
  /* 11 */ &Dump::vInvalid,         // CN_PTRVAL
  /* 12 */ &Dump::vInvalid,         // CN_STRVAL
  /* 13 */ &Dump::vInvalid,         // CN_STRVAR
  /* 14 */ &Dump::vInvalid,         // CN_TMEVAL
  /* 15 */ &Dump::vInvalid,         // CN_TSPVAL
  /* 16 */ &Dump::vInvalid,         // CN_VALREG
  /* 17 */ &Dump::vInvalid,         // CN_VALSET
  /* 18 */ &Dump::vInvalid,         // CN_VALWRK
  
  /* 19 */ &Dump::vInvalid,         // CN_ACCESSPATH
  /* 20 */ &Dump::vInvalid,         // CN_ACCUM
  /* 21 */ &Dump::vInvalid,         // CN_ACCUMVEC
  /* 22 */ &Dump::vInvalid,         // CN_ARG
  /* 23 */ &Dump::vInvalid,         // CN_ARGUMENTS
  /* 24 */ &Dump::vInvalid,         // CN_ATOMIC
  /* 25 */ &AttachMap::vDump,       // CN_ATTACHMAP

  /* 26 */ &Dump::vDummy,           // CN_BITARRAY
  /* 27 */ &Dump::vDummy,           // CN_BITMAP
  /* 28 */ &Dump::vInvalid,         // CN_BUFFERPOOL
  /* 29 */ &Dump::vInvalid,         // CN_BPA
  
  /* 30 */ &Dump::vDummy,           // CN_COLUMN
  /* 31 */ &Dump::vDummy,           // CN_COLUMNVEC

  /* 32 */ &Dic::vDump,             // CN_DIC
  /* 33 */ &DicPath::vDump,         // CN_DICPATH
  /* 34 */ &Dump::vInvalid,         // CN_DIR
  
  /* 35 */ &Dump::vDummy,           // CN_ENTRY
  /* 36 */ &Dump::vDummy,           // CN_ENTRYBUFFER
  /* 37 */ &Dump::vInvalid,         // CN_ENTRYVEC
  
  /* 38 */ &Dump::vInvalid,         // CN_FILE
  /* 39 */ &Dump::vDummy,           // CN_FREEBUFFER
  /* 40 */ &Dump::vDummy,           // CN_FREECHUNK
  /* 41 */ &Dump::vDummy,           // CN_FREEDNOT
  
  /* 42 */ &Graph::vDump,           // CN_GRAPH
  /* 43 */ &GraphCursor::vDump,     // CN_GRAPH
  /* 44 */ &GraphNode::vDump,       // CN_GRAPHNODE
  /* 45 */ &Grid::vDump,            // CN_GRID
  /* 46 */ &Dump::vDummy,           // CN_GROUPBY

  /* 47 */ &HeapBuffer::vDump,      // CN_HEAPBUFFER
  
  /* 48 */ &Dump::vDummy,           // CN_INDEX
  /* 49 */ &Dump::vDummy,           // CN_INDEXDATA
  /* 50 */ &Dump::vDummy,           // CN_IDXCOLVEC
  /* 51 */ &Dump::vDummy,           // CN_IDXDATAVEC
  /* 52 */ &Dump::vDummy,           // CN_IDXVEC
  /* 53 */ &Dump::vDummy,           // CN_INDEXLEVELS

  /* 54 */ &Dump::vDummy,           // CN_JUMBOHEAP

  /* 55 */ &Dump::vDummy,           // CN_KEY
  /* 56 */ &Dump::vDummy,           // CN_KEYVAL
  /* 57 */ &Dump::vDummy,           // CN_KEYVEC
  /* 58 */ &Dump::vInvalid,         // CN_KEYWRK
  
  /*    */ &Dump::vDummy,           // CN_LARGEHEAP
  /* 59 */ &LoadChunk::vDump,       // CN_LINEITEM
  /* 60 */ &LoadChunk::vDump,       // CN_LOADCHUNK
  /* 61 */ &Loader::vDump,          // CN_LOADER
  /* 62 */ &LoaderSub::vDump,       // CN_LOADERSUB
  /* 63 */ &LoadMap::vDump,         // CN_LOADMAP
  /* 64 */ &Dump::vInvalid,         // CN_LOCK
  
  /* 65 */ &Heap::vDump,            // CN_MEDIUMHEAP
  
  /* 66 */ &Node::vDump,            // CN_NODE
  /* 67 */ &NodeCursor::vDump,      // CN_NODECURSOR
  
  /* 68 */ &Dump::vUndefined,       // CN_OP
  
  /* 69 */ &Parser::vDump,          // CN_PARSER
  /* 70 */ &Partition::vDump,       // CN_PARTITION
  /* 71 */ &Dump::vDummy,           // CN_PARTITIONSCHEDULER
  /* 72 */ &Dump::vDummy,           // CN_PARTITIONVEC
  /* 73 */ &Dump::vInvalid,         // CN_PATHVEC
  /* 74 */ &Plan::vDump,            // CN_PLAN
  /* 75 */ &Dump::vUndefined,       // CN_PLANNER
  /* 76 */ &PlanList::vDump,        // CN_PLANLIST
  /* 77 */ &Dump::vUndefined,       // CN_PREDICATE
  /* 78 */ &Dump::vUndefined,       // CN_PREDVEC
  /* 79 */ &Dump::vUndefined,       // CN_POSIXTHREAD
  /* 80 */ &Dump::vUndefined,       // CN_PROXY
  /* 81 */ &Dump::vUndefined,       // CN_PROXYSET
  /* 82 */ &Dump::vDummy,           // CN_PTNCOLVEC
  
  /* 83 */ &Queue::vDump,           // CN_QUEUE
  /* 84 */ &QueueBuffer::vDump,     // CN_QUEUEBUFFER

  /* 85 */ &Dump::vUndefined,       // CN_RBCURSOR
  /* 86 */ &RegFrame::vDump,        // CN_REGFRAME
  /* 87 */ &Dump::vUndefined,       // CN_RING
  /* 88 */ &Dump::vUndefined,       // CN_ROARRAY
  /* 89 */ &Dump::vUndefined,       // CN_ROWBUFFER
  /* 90 */ &Dump::vUndefined,       // CN_ROWCURSOR
  /* 91 */ &Dump::vUndefined,       // CN_ROWONCE
  
  /* 92 */ &Dump::vUndefined,       // CN_SCANBUFFER
  /* 93 */ &Dump::vUndefined,       // CN_SCRATCH
  /* 94 */ &Dump::vUndefined,       // CN_SEHISTORY
  /* 95 */ &SEitf::vDump,           // CN_SEITF
  /* 96 */ &Dump::vUndefined,       // CN_SEITFCURSOR
  /* 97 */ &Dump::vUndefined,       // CN_SEITFLIST
  /* 98 */ &Dump::vUndefined,       // CN_SELCURSOR
  /* 99 */ &Dump::vUndefined,       // CN_SEMAPHORE
  /*100 */ &SePrompt::vDump,        // CN_SEPROMPT
  /*101 */ &Dump::vUndefined,       // CN_SINCE
  /*102 */ &Dump::vUndefined,       // CN_SKIPCURSOR
  /*103 */ &Dump::vUndefined,       // CN_SKIPLIST
  /*104 */ &Dump::vUndefined,       // CN_SKIPNODE
  /*105 */ &Dump::vUndefined,       // CN_SLAB
  /*106 */ &Dump::vUndefined,       // CN_SLABALLOCATOR
  /*107 */ &Slots::vDump,           // CN_SLOTS
  /*108 */ &Heap::vDump,            // CN_SMALLHEAP
  /*109 */ &Socket::vDump,          // CN_SOCKET
  /*110 */ &Stack::vDump,           // CN_STACK
  /*111 */ &Dump::vInvalid,         // CN_STEPACCUM
  /*112 */ &StepArray::vDump,       // CN_STEPARRAY
  /*113 */ &Dump::vInvalid,         // CN_SYSCALL

  /*114 */ &Table::vDump,           // CN_TABLE
  /*115 */ &Task::vDump,            // CN_TASK
  /*116 */ &Dump::vUndefined,       // CN_TASKCURSOR
  /*117 */ &TaskList::vDump,        // CN_TASKLIST
  /*118 */ &Dump::vUndefined,       // CN_TASKQUEUE
  /*119 */ &Dump::vUndefined,       // CN_SERVERSCHEDULER
  /*120 */ &Dump::vUndefined,       // CN_TASKSET
  /*121 */ &Dump::vUndefined,       // CN_THREAD
  /*122 */ &Dump::vUndefined,       // CN_THREADCURSOR
  /*123 */ &Dump::vUndefined,       // CN_THREADLIST
  /*124 */ &Dump::vUndefined,       // CN_THREADQUEUE
  /*125 */ &Dump::vUndefined,       // CN_TIME
  /*126 */ &Dump::vInvalid,         // CN_TIMER
  /*127 */ &Token::vDump,           // CN_TOKEN
  /*128 */ &Tokeniser::vDump,       // CN_TOKENISER
  /*129 */ &Dump::vInvalid,         // CN_TOKENARY
  /*130 */ &Dump::vInvalid,         // CN_TOKENVEC
  /*131 */ &Tx::vDump,              // CN_TX
  /*132 */ &Dump::vUndefined,       // CN_TXSET

  /*133 */ &Word::vDump,            // CN_WORD
  
  /*134 */ &Dump::vUndefined,       // CN_ZSMALL_
  /*135 */ &Dump::vUndefined,       // CN_ZMEDIUM_
  /*136 */ &Dump::vUndefined,       // CN_ZLARGE_
  /*137 */ &Dump::vUndefined,       // CN_ZJUMBO_
  
  /*138 */ &Dump::vEnums            // CN_ENUMS
};

i8 giClassSize[]
{
  /*  0 */ 0,                          // CN_NONE
  
  /*  1 */ 0,                          // CN_BCDVAL
  /*  2 */ sizeof(BinVal),             // CN_BINVAL
  /*  3 */ 0,                          // CN_BOLVAL
  /*  4 */ 0,                          // CN_DTEVAL
  /*  5 */ 0,                          // CN_ENTWRK
  /*  6 */ 0,                          // CN_FLTVAL
  /*  7 */ sizeof(IntVal),             // CN_INTVAL
  /*  8 */ sizeof(PtrVal),             // CN_IPVAL
  /*  9 */ 0,                          // CN_IVLVAL
  /* 10 */ sizeof(ObjVal),             // CN_OBJVAL
  /* 11 */ sizeof(PtrVal),             // CN_PTRVAL
  /* 12 */ sizeof(StrVal),             // CN_STRVAL
  /* 13 */ sizeof(s32),                // CN_STRVAR
  /* 14 */ 0,                          // CN_TMEVAL
  /* 15 */ 0,                          // CN_TSPVAL
  /* 16 */ sizeof(ValReg),             // CN_VALREG
  /* 17 */ sizeof(ValSet),             // CN_VALSET
  /* 18 */ sizeof(ValWrk),             // CN_VALWRK
  
  /* 19 */ sizeof(AccessPath),         // CN_ACCESSPATH
  /* 20 */ sizeof(Accum),              // CN_ACCUM
  /* 21 */ sizeof(AccumVec),           // CN_ACCUMVec
  /* 22 */ 0,                          // CN_ARG
  /* 23 */ sizeof(Arguments),          // CN_ARGUMENTS
  /* 24 */ sizeof(Atomic),             // CN_ATOMIC
  /* 25 */ sizeof(AttachMap),          // CN_ATTACHMAP
  
  /* 26 */ sizeof(BitArray),           // CN_BITARRAY
  /* 27 */ sizeof(BitArray),           // CN_BITMAP
  /* 28 */ sizeof(BufferPool),         // CN_BUFFERPOOL
  /* 29 */ sizeof(BPA),                // CN_BPA

  /* 30 */ sizeof(Column),             // CN_COLUMN
  /* 31 */ sizeof(ColumnVec),          // CN_COLUMNVEC
  
  /* 32 */ sizeof(Dic),                // CN_DIC
  /* 33 */ sizeof(DicPath),            // CN_DICPATH
  /* 34 */ sizeof(Dir),                // CN_DIR
  
  /* 35 */ sizeof(Entry),              // CN_ENTRY
  /* 36 */ 0,                          // CN_ENTRYBUFFER
  /* 37 */ sizeof(EntryVec),           // CN_ENTRYVEC
  
  /* 38 */ sizeof(File),               // CN_FILE
  /* 39 */ sizeof(Buffer),             // CN_FREEBUFFER
  /* 40 */ sizeof(FreeChunk),          // CN_FREECHUNK
  /* 41 */ 0,                          // CN_FREEDNOT
  
  /* 42 */ sizeof(Graph),              // CN_GRAPH
  /* 43 */ sizeof(GraphCursor),        // CN_GRAPHCURSOR
  /* 44 */ sizeof(GraphNode),          // CN_GRAPHNODE
  /* 45 */ sizeof(Grid),               // CN_GRID
  /* 46 */ sizeof(GroupBy),            // CN_GROUPBY
  
  /* 47 */ sizeof(HeapBuffer),         // CN_HEAPBUFFER
  
  /* 48 */ sizeof(Index),              // CN_INDEX
  /* 49 */ sizeof(IndexData),          // CN_INDEXDATA
  /* 50 */ sizeof(IdxColVec),          // CN_IDXCOLVEC
  /* 51 */ sizeof(IdxDataVec),         // CN_IDXDATAVEC
  /* 52 */ sizeof(IdxVec),             // CN_IDXVEC
  /* 53 */ 0,                          // CN_INDEXLEVELS
  
  /* 54 */ sizeof(Heap), // correct    // CN_JUMBOHEAP
  
  /* 55 */ 0,                          // CN_KEY
  /* 56 */ sizeof(KeyVal),             // CN_KEYVAL
  /* 57 */ 0,                          // CN_KEYVEC
  /* 58 */ sizeof(KeyWrk),             // CN_KEYWRK
  
  /*    */ sizeof(Heap), // correct    // CN_LARGEHEAP
  /* 59 */ sizeof(LineItem),           // CN_LINEITEM
  /* 60 */ sizeof(LoadChunk),          // CN_LOADCHUNK
  /* 61 */ sizeof(Loader),             // CN_LOADER
  /* 62 */ sizeof(LoaderSub),          // CN_LOADERSUB
  /* 63 */ sizeof(LoadMap),            // CN_LOADMAP
  /* 64 */ sizeof(Lock),               // CN_LOCK
  
  /* 65 */ sizeof(Heap), // correct    // CN_MEDIUMHEAP
  
  /* 66 */ sizeof(Node),               // CN_NODE
  /* 67 */ sizeof(NodeCursor),         // CN_NODECURSOR
  
  /* 68 */ sizeof(Op),                 // CN_OP
  
  /* 69 */ sizeof(Parser),             // CN_PARSER
  /* 70 */ sizeof(Partition),          // CN_PARTITION
  /* 71 */ sizeof(PartitionScheduler), // CN_PARTITIONSCHEDULER
  /* 72 */ sizeof(PartitionVec),       // CN_PARTITIONVEC
  /* 73 */ sizeof(PathVec),            // CN_PATHVEC
  /* 74 */ sizeof(Plan),               // CN_PLAN
  /* 75 */ sizeof(Planner),            // CN_PLANNER
  /* 76 */ sizeof(PlanList),           // CN_PLANLIST
  /* 77 */ sizeof(PosixThread),        // CN_POSIXTHREAD
  /* 78 */ sizeof(Predicate),          // CN_PREDICATE
  /* 79 */ sizeof(PredicateVec),       // CN_PREDICATEVEC
  /* 80 */ sizeof(Proxy),              // CN_PROXY
  /* 81 */ sizeof(ProxySet),           // CN_PROXYSET
  /* 82 */ sizeof(PtnColVec),          // CN_PTNCOLVEC
  
  /* 83 */ sizeof(Queue),              // CN_QUEUE
  /* 84 */ sizeof(QueueBuffer),        // CN_QUEUEBUFFER
  
  /* 85 */ sizeof(RbCursor),           // CN_RBCURSOR
  /* 86 */ sizeof(RegFrame),           // CN_REGFRAME
  /* 87 */ sizeof(Ring),               // CN_RING
  /* 88 */ 0,                          // CN_ROARRAY
  /* 89 */ 0,                          // CN_ROWBUFFER
  /* 90 */ sizeof(RowCursor),          // CN_ROWCURSOR
  /* 91 */ 0,                          // CN_ROWONCE
  
  /* 92 */ sizeof(ScanBuffer),         // CN_SCANBUFFER
  /* 93 */ sizeof(Scratch),            // CN_SCRATCH
  /* 94 */ sizeof(SeHistory),          // CN_SEHISTORY
  /* 95 */ sizeof(SEitf),              // CN_SEITF
  /* 96 */ sizeof(SEitfCursor),        // CN_SEITFCURSOR
  /* 97 */ sizeof(SEitfList),          // CN_SEITFLIST
  /* 98 */ sizeof(SelCursor),          // CN_SELCURSOR
  /* 99 */ sizeof(Semaphore),          // CN_SEMAPHORE
  /*100 */ sizeof(SePrompt),           // CN_SEPROMPT
  /*101 */ sizeof(Since),              // CN_SINCE
  /*102 */ sizeof(SkipCursor),         // CN_SKIPCURSOR
  /*103 */ sizeof(SkipList),           // CN_SKIPLIST
  /*104 */ sizeof(SkipNode),           // CN_SKIPNODE
  /*105 */ 0,                          // CN_SLAB
  /*106 */ 0,                          // CN_SLABALLOCATOR
  /*107 */ sizeof(Slots),              // CN_SLOTS
  /*108 */ sizeof(Heap), // correct    // CN_SMALLHEAP
  /*109 */ sizeof(Socket),             // CN_SOCKET
  /*110 */ sizeof(Stack),              // CN_STACK
  /*111 */ sizeof(StepAccum),          // CN_STEPACCUM
  /*112 */ sizeof(StepArray),          // CN_STEPARRAY
  /*113 */ sizeof(SysCall),            // CN_SYSCALL

  /*114 */ sizeof(Table),              // CN_TABLE
  /*115 */ sizeof(Task),               // CN_TASK
  /*116 */ sizeof(TaskCursor),         // CN_TASKCURSOR
  /*117 */ sizeof(TaskList),           // CN_TASKLIST
  /*118 */ sizeof(TaskQueue),          // CN_TASKQUEUE
  /*119 */ sizeof(ServerScheduler),    // CN_SERVERSCHEDULER
  /*120 */ sizeof(TaskSet),            // CN_TASKSET
  /*121 */ sizeof(Thread),             // CN_THREAD
  /*122 */ sizeof(ThreadCursor),       // CN_THREADCURSOR
  /*123 */ sizeof(ThreadList),         // CN_THREADLIST
  /*124 */ sizeof(ThreadQueue),        // CN_THREADQUEUE
  /*125 */ sizeof(Time),               // CN_TIME
  /*126 */ sizeof(Timer),              // CN_TIMER
  /*127 */ sizeof(Token),              // CN_TOKEN
  /*128 */ sizeof(Tokeniser),          // CN_TOKENISER
  /*129 */ sizeof(TokenAry),           // CN_TOKENARY
  /*130 */ sizeof(TokenVec),           // CN_TOKENVEC
  /*131 */ sizeof(Tx),                 // CN_TX
  /*132 */ 0,                          // CN_TXSET

  /*133 */ sizeof(Word),               // CN_WORD

  /*134 */ C_SIZE_SMALL_OBJECT  - 256, // CN_ZSMALL_
  /*135 */ C_SIZE_MEDIUM_OBJECT - 256, // CN_ZMEDIUM_
  /*136 */ C_SIZE_POOL_BUFFER   - 256, // CN_ZLARGE_
  /*137 */ C_SIZE_JUMBO_OBJECT  - 256, // CN_ZJUMBO_

  /*138 */ -1
};

eHT geWhichHeap[]
{
  /*  0 */ HT_INVALID,
  
  /*  1 */ HT_INVALID, // CN_BCDVAL
  /*  2 */ HT_INVALID, // CN_BINVAL
  /*  3 */ HT_INVALID, // CN_BOLVAL
  /*  4 */ HT_INVALID, // CN_DTEVAL
  /*  5 */ HT_INVALID, // CN_ENTWRK,
  /*  6 */ HT_INVALID, // CN_FLTVAL
  /*  7 */ HT_INVALID, // CN_INTVAL
  /*  8 */ HT_INVALID, // CN_IPVAL
  /*  9 */ HT_INVALID, // CN_IVLVAL
  /* 10 */ HT_INVALID, // CN_OBJVAL
  /* 11 */ HT_INVALID, // CN_PTRVAL
  /* 12 */ HT_INVALID, // CN_STRVAL
  /* 13 */ HT_INVALID, // CN_STRVAR
  /* 14 */ HT_INVALID, // CN_TMEVAL
  /* 15 */ HT_INVALID, // CN_TSPVAL
  /* 16 */ HT_INVALID, // CN_VALREG
  /* 17 */ HT_MEDIUM,  // CN_VALSET
  /* 18 */ HT_INVALID, // CN_VALWRK
  
  /* 19 */ HT_MEDIUM,  // CN_ACCESSPATH
  /* 20 */ HT_SMALL,   // CN_ACCUM
  /* 21 */ HT_MEDIUM,  // CN_ACCUMVEC
  /* 22 */ HT_SMALL,   // CN_ARG
  /* 23 */ HT_INVALID, // CN_ARGUMENTS
  /* 24 */ HT_INVALID, // CN_ATOMIC
  /* 25 */ HT_INVALID, // CN_ATTACHMAP
  
  /* 26 */ HT_SMALL,   // CN_BITARRAY
  /* 27 */ HT_LARGE_ALT,   // CN_BITMAP
  /* 28 */ HT_INVALID, // CN_BUFFERPOOL
  /* 29 */ HT_INVALID, // CN_BPA

  /* 30 */ HT_SMALL,   // CN_COLUMN
  /* 31 */ HT_MEDIUM,  // CN_COLUMNVEC
  
  /* 32 */ HT_MEDIUM,  // CN_DIC
  /* 33 */ HT_INVALID, // CN_DICPATH
  /* 34 */ HT_SMALL,   // CN_DIR

  /* 35 */ HT_INVALID, // CN_ENTRY
  /* 36 */ HT_LARGE_ALT,   // CN_ENTRYBUFFER
  /* 37 */ HT_INVALID, // CN_ENTRYVEC
  
  /* 38 */ HT_SMALL,   // CN_FILE
  /* 39 */ HT_INVALID, // CN_FREEBUFFER
  /* 40 */ HT_INVALID, // CN_FREECHUNK
  /* 41 */ HT_INVALID, // CN_FREEDNOT
  
  /* 42 */ HT_SMALL,   // CN_GRAPH
  /* 43 */ HT_SMALL,   // CN_GRAPHCURSOR
  /* 44 */ HT_SMALL,   // CN_GRAPHNODE
  /* 45 */ HT_INVALID, // CN_GRID
  /* 46 */ HT_SMALL,   // CN_GROUPBY

  /* 47 */ HT_INVALID, // CN_HEAPBUFFER
  
  /* 48 */ HT_INVALID, // CN_INDEX
  /* 49 */ HT_SMALL,   // CN_INDEXDATA
  /* 50 */ HT_MEDIUM,  // CN_IDXCOLVEC
  /* 51 */ HT_SMALL,   // CN_IDXDATAVEC
  /* 52 */ HT_SMALL,   // CN_IDXVEC
  /* 53 */ HT_MEDIUM,  // CN_INDEXLEVELS

  /* 54 */ HT_INVALID, // CN_JUMBOHEAP
  
  /* 55 */ HT_INVALID, // CN_KEY
  /* 56 */ HT_SMALL,   // CN_KEYVAL
  /* 57 */ HT_MEDIUM,  // CN_KEYVEC
  /* 58 */ HT_INVALID, // CN_KEYWRK

  /*  */   HT_INVALID, // CN_LARGEHEAP
  /* 59 */ HT_INVALID, // CN_LINEITEM
  /* 60 */ HT_LARGE_ALT,   // CN_LOADCHUNK
  /* 61 */ HT_MEDIUM,  // CN_LOADER
  /* 62 */ HT_MEDIUM,  // CN_LOADERSUB
  /* 63 */ HT_JUMBO,   // CN_LOADMAP
  /* 64 */ HT_INVALID, // CN_LOCK
  
  /* 65 */ HT_INVALID, // CN_MEDIUMHEAP
  
  /* 66 */ HT_SMALL,   // CN_NODE
  /* 67 */ HT_SMALL,   // CN_NODECURSOR
  
  /* 68 */ HT_SMALL,   // CN_OP
  
  /* 69 */ HT_MEDIUM,  // CN_PARSER
  /* 70 */ HT_SMALL,   // CN_PARTITION
  /* 71 */ HT_SMALL,   // CN_PARTITIONSCHEDULER
  /* 72 */ HT_MEDIUM,  // CN_PARTITIONVEC
  /* 73 */ HT_MEDIUM,  // CN_PATHVEC
  /* 74 */ HT_MEDIUM,  // CN_PLAN
  /* 75 */ HT_MEDIUM,  // CN_PLANNER
  /* 76 */ HT_SMALL,   // CN_PLANLIST
  /* 77 */ HT_SMALL,   // CN_PREDICATE
  /* 78 */ HT_MEDIUM,  // CN_PREDICATEVEC
  /* 79 */ HT_SMALL,   // CN_POSIXTHREAD
  /* 80 */ HT_INVALID, // CN_PROXY
  /* 81 */ HT_INVALID, // CN_PROXYSET
  /* 82 */ HT_MEDIUM,  // CN_PTNCOLVEC

  /* 83 */ HT_SMALL,   // CN_QUEUE
  /* 84 */ HT_LARGE_BUFFER,   // CN_QUEUEBUFFER
  
  /* 85 */ HT_SMALL,   // CN_RBCURSOR
  /* 86 */ HT_LARGE_ALT,   // CN_REGFRAME
  /* 87 */ HT_INVALID, // CN_RING
  /* 88 */ HT_MEDIUM,  // CN_ROARRAY
  /* 89 */ HT_LARGE_BUFFER,   // CN_ROWBUFFER
  /* 90 */ HT_SMALL,   // CN_ROWCURSOR
  /* 91 */ HT_SMALL,   // CN_ROWONCE
  
  /* 92 */ HT_LARGE_ALT,   // CN_SCANBUFFER
  /* 93 */ HT_LARGE_ALT,   // CN_SCRATCH
  /* 94 */ HT_INVALID, // CN_SEHISTORY
  /* 95 */ HT_MEDIUM,  // CN_SEITF
  /* 96 */ HT_SMALL,   // CN_SEITFCURSOR
  /* 97 */ HT_INVALID, // CN_SEITFLIST
  /* 98 */ HT_SMALL,   // CN_SELCURSOR
  /* 99 */ HT_SMALL,   // CN_SEMAPHORE
  /*100 */ HT_LARGE_ALT,   // CN_SEPROMPT
  /*101 */ HT_INVALID, // CN_SINCE
  /*102 */ HT_INVALID, // CN_SKIPCURSOR
  /*103 */ HT_SMALL,   // CN_SKIPLIST
  /*104 */ HT_SMALL,   // CN_SKIPNODE
  /*105 */ HT_LARGE_ALT,   // CN_SLAB
  /*106 */ HT_SMALL,   // CN_SLABALLOCATOR
  /*107 */ HT_INVALID, // CN_SLOTS
  /*108 */ HT_INVALID, // CN_SMALLHEAP
  /*109 */ HT_SMALL,   // CN_SOCKET
  /*110 */ HT_LARGE_ALT,   // CN_STACK
  /*111 */ HT_INVALID, // CN_STEPACCUM
  /*112 */ HT_LARGE_ALT,   // CN_STEPARRAY
  /*113 */ HT_INVALID, // CN_SYSCALL
  
  /*114 */ HT_INVALID, // CN_TABLE
  /*115 */ HT_MEDIUM,  // CN_TASK
  /*116 */ HT_INVALID, // CN_TASKCURSOR
  /*117 */ HT_SMALL,   // CN_TASKLIST
  /*118 */ HT_SMALL,   // CN_TASKQUEUE
  /*119 */ HT_MEDIUM,  // CN_SERVERSCHEDULER
  /*120 */ HT_SMALL,   // CN_TASKSET
  /*121 */ HT_INVALID, // CN_THREAD
  /*122 */ HT_SMALL,   // CN_THREADCURSOR
  /*123 */ HT_INVALID, // CN_THREADLIST
  /*124 */ HT_INVALID, // CN_THREADQUEUE
  /*125 */ HT_INVALID, // CN_TIME
  /*126 */ HT_SMALL,   // CN_TIMER
  /*127 */ HT_SMALL,   // CN_TOKEN
  /*128 */ HT_MEDIUM,  // CN_TOKENISER
  /*129 */ HT_INVALID, // CN_TOKENARY
  /*130 */ HT_SMALL,   // CN_TOKENVEC
  /*131 */ HT_SMALL,   // CN_TX
  /*132 */ HT_SMALL,   // CN_TXSET
  
  /*133 */ HT_MEDIUM,  // CN_WORD
  
  /*134 */ HT_INVALID, // CN_ZSMALL_
  /*135 */ HT_INVALID, // CN_ZMEDIUM_
  /*136 */ HT_INVALID, // CN_ZLARGE_
  /*137 */ HT_INVALID, // CN_ZJUMBO_

  /*138 */ HT_INVALID, // CN_ENUMS
};

/*----------------------------------------------------------------------------*/
/* dummy dump                                                                 */
/*----------------------------------------------------------------------------*/
void Dump::vDummy(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass != CN_NONE);
  ASSERT(pObject != nullptr);
  ASSERT(aTab != nullptr);
  // do nothing
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* dump enums                                                                 */
/*----------------------------------------------------------------------------*/
void Dump::vEnums(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass == CN_ENUMS);
  ASSERT(pObject != nullptr);
  ASSERT(aTab != nullptr);
  // do nothing
}

/*----------------------------------------------------------------------------*/
/* inavlid dump                                                               */
/*----------------------------------------------------------------------------*/
void Dump::vInvalid(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass != CN_ENUMS);
  ASSERT(pObject != nullptr);
  ASSERT(aTab != nullptr);
  vTrace("%sInvalid Class %s in Buffer Pool\n"_Stz, aTab, aCN[eClass]);
}

/*----------------------------------------------------------------------------*/
/* dump not defined                                                           */
/*----------------------------------------------------------------------------*/
void Dump::vUndefined(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass != CN_NONE);
  ASSERT(pObject != nullptr);
  ASSERT(aTab != nullptr);
  vTrace("%s%s: **dump function not defined for class**\n"_Stz, aTab, aCN[eClass]);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
