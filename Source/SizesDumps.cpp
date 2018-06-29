/*============================================================================*/
/* Dump.cpp -- Class Dumps                                                    */
/*============================================================================*/

#include "StorageEngine.h"
#include "SePrompt.hpp"

// Object's : Class::vDump
// Object's with no vDump defined : Dump::vUndefined
// Header   : Dump::vInvalid
// Objects we don't want to dump (eg Word) : Dump::vDummy

/*----------------------------------------------------------------------------*/
/* dump functions                                                             */
/*----------------------------------------------------------------------------*/
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
  
  /* 30 */ &Dump::vDummy,           // CN_CARRYOVER
  /* 31 */ &Dump::vDummy,           // CN_COLUMN
  /* 32 */ &Dump::vDummy,           // CN_COLUMNVEC

  /* 33 */ &Dic::vDump,             // CN_DIC
  /*    */ &Dump::vDummy,           // CN_DICPATH
  /* 34 */ &DicStack::vDump,        // CN_DICSTACK
  /* 35 */ &Dump::vInvalid,         // CN_DIR
  
  /* 36 */ &Dump::vDummy,           // CN_ENTRY
  /* 37 */ &Dump::vDummy,           // CN_ENTRYBUFFER
  /* 38 */ &Dump::vInvalid,         // CN_ENTRYVEC
  
  /* 39 */ &Dump::vInvalid,         // CN_FILE
  /* 40 */ &Dump::vDummy,           // CN_FORMATTERS
  /* 41 */ &Dump::vDummy,           // CN_FREEBUFFER
  /* 42 */ &Dump::vDummy,           // CN_FREECHUNK
  /* 43 */ &Dump::vDummy,           // CN_FREEDNOT
  /* 44 */ &Dump::vDummy,           // CN_FREEENTRY
  
  /* 45 */ &Graph::vDump,           // CN_GRAPH
  /* 46 */ &GraphCursor::vDump,     // CN_GRAPH
  /* 47 */ &GraphNode::vDump,       // CN_GRAPHNODE
  /* 48 */ &Grid::vDump,            // CN_GRID
  /* 49 */ &Dump::vDummy,           // CN_GROUPBY

  /* 50 */ &HeapBuffer::vDump,      // CN_HEAPBUFFER
  
  /* 51 */ &Dump::vDummy,           // CN_INDEX
  /* 52 */ &Dump::vDummy,           // CN_INDEXTREE
  /* 53 */ &Dump::vDummy,           // CN_INDEXTREEVEC
  /* 54 */ &Dump::vDummy,           // CN_IDXCOLVEC
  /* 55 */ &Dump::vDummy,           // CN_IDXVEC
  /* 56 */ &Dump::vDummy,           // CN_INDEXLEVELS
  /* 57 */ &Dump::vInvalid,         // CN_ITEM
  /* 58 */ &Dump::vInvalid,         // CN_ITEMBUFFER
  /*    */ &Dump::vDummy,           // CN_JOB__BEGUN
  /*    */ &Dump::vDummy,           // CN_JOB__END
  /*    */ &Dump::vDummy,           // CN_JOB__ENDED
  /*    */ &Dump::vDummy,           // CN_JOB__ERROR
  /*    */ &Dump::vDummy,           // CN_JOB__EXECUTE
  /*    */ &Dump::vDummy,           // CN_JOB__EXECUTED
  
  /*    */ &Dump::vDummy,           // CN_JOB_ANALYZEINDEX
  /*    */ &Dump::vDummy,           // CN_JOB_ANALYZETABLE
  /*    */ &Dump::vDummy,           // CN_JOB_CREATECATALOG
  /*    */ &Dump::vDummy,           // CN_JOB_CREATEINDEX
  /*    */ &Dump::vDummy,           // CN_JOB_CREATESCHEMA
  /*    */ &Dump::vDummy,           // CN_JOB_CREATETABLE
  /*    */ &Dump::vDummy,           // CN_JOB_CREATETENANT
  /*    */ &Dump::vDummy,           // CN_JOB_DROPCATALOG
  /*    */ &Dump::vDummy,           // CN_JOB_DROPINDEX
  /*    */ &Dump::vDummy,           // CN_JOB_DROPSCHEMA
  /*    */ &Dump::vDummy,           // CN_JOB_DROPTABLE
  /*    */ &Dump::vDummy,           // CN_JOB_DROPTENANT
  /*    */ &Dump::vDummy,           // CN_JOB_G35
  /*    */ &Dump::vDummy,           // CN_JOB_LOADTABLE
  /*    */ &Dump::vDummy,           // CN_JOB_SPACE
  
  /*    */ &Jobs::vDump,            // CN_JOBS
  
  /* 59 */ &Heap::vDump,            // CN_JUMBOPERSISTENT
  /* 60 */ &Heap::vDump,            // CN_JUMBOTRANSIENT
  
  /* 61 */ &Dump::vDummy,           // CN_KEY
  /* 62 */ &Dump::vDummy,           // CN_KEYVAL
  /* 63 */ &Dump::vDummy,           // CN_KEYVEC
  /* 64 */ &Dump::vInvalid,         // CN_KEYWRK
  
  /* 65 */ &Heap::vDump,            // CN_LARGEPERSISTENT
  /* 66 */ &Heap::vDump,            // CN_LARGETRANSIENT
  
  /* 67 */ &LoadChunk::vDump,       // CN_LINEITEM
  /* 68 */ &LoadChunk::vDump,       // CN_LOADCHUNK
  /* 69 */ &Loader::vDump,          // CN_LOADER
  /* 70 */ &LoaderSub::vDump,       // CN_LOADERSUB
  /* 71 */ &LoadMap::vDump,         // CN_LOADMAP
  /* 72 */ &Dump::vInvalid,         // CN_LOCK
  
  /* 73 */ &Heap::vDump,            // CN_MEDIUMPERSISTENT
  /* 74 */ &Heap::vDump,            // CN_MEDIUMTRANSIENT
  
  /* 75 */ &Node::vDump,            // CN_NODE
  
  /* 77 */ &Dump::vUndefined,       // CN_OP
  
  /* 78 */ &Parser::vDump,          // CN_PARSER
  /* 80 */ &Dump::vDummy,           // CN_PARTITIONSCHEDULER
  /* 82 */ &Dump::vInvalid,         // CN_PATHVEC
  /* 83 */ &Dump::vInvalid,         // CN_PERFORMANCEBUFFER
  /* 84 */ &Dump::vInvalid,         // CN_PERFORMANCEENTRY
  /* 85 */ &Dump::vInvalid,         // CN_PERFORMANCETRACE
  /* 86 */ &Plan::vDump,            // CN_PLAN
  /* 87 */ &Dump::vUndefined,       // CN_PLANNER
  /* 88 */ &PlanList::vDump,        // CN_PLANLIST
  /* 89 */ &Dump::vUndefined,       // CN_PREDICATE
  /* 90 */ &Dump::vUndefined,       // CN_PREDVEC
  /* 91 */ &Dump::vUndefined,       // CN_POSIXTHREAD
  /* 92 */ &Dump::vUndefined,       // CN_PROXY
  /* 93 */ &Dump::vUndefined,       // CN_PROXYSET
  /* 94 */ &Dump::vDummy,           // CN_PTNCOLVEC
  
  /* 95 */ &Queue::vDump,           // CN_QUEUE
  /* 96 */ &QueueBuffer::vDump,     // CN_QUEUEBUFFER
  /*    */ &Dump::vUndefined,       // CN_QUEUES

  /* 97 */ &Dump::vUndefined,       // CN_RBCURSOR
  /*    */ &RegFrame::vDump,        // CN_REGFRAME

  /*101 */ &Dump::vUndefined,       // CN_RING
  /*102 */ &Dump::vUndefined,       // CN_ROARRAY
  /*103 */ &Dump::vUndefined,       // CN_ROWBUFFER
  /*104 */ &Dump::vUndefined,       // CN_ROWCURSOR
  /*105 */ &Dump::vUndefined,       // CN_ROWONCE
  /*106 */ &Dump::vUndefined,       // CN_ROWWRK
  
  /*107 */ &Dump::vUndefined,       // CN_SCANBUFFER
  /*108 */ &Dump::vUndefined,       // CN_SCRATCH
  /*109 */ &Dump::vUndefined,       // CN_SEHISTORY
  /*110 */ &SEitf::vDump,           // CN_SEITF
  /*111 */ &Dump::vUndefined,       // CN_SEITFCURSOR
  /*112 */ &Dump::vUndefined,       // CN_SEITFLIST
  /*113 */ &Dump::vUndefined,       // CN_SELCURSOR
  /*114 */ &Dump::vUndefined,       // CN_SEMAPHORE
  /*115 */ &SePrompt::vDump,        // CN_SEPROMPT
  /*116 */ &Dump::vUndefined,       // CN_SINCE
  /*117 */ &Dump::vUndefined,       // CN_SKIPCURSOR
  /*118 */ &Dump::vUndefined,       // CN_SKIPLIST
  /*119 */ &Dump::vUndefined,       // CN_SKIPNODE
  /*120 */ &Dump::vUndefined,       // CN_SLAB
  /*121 */ &Dump::vUndefined,       // CN_SLABALLOCATOR
  /*122 */ &Slots::vDump,           // CN_SLOTS
  /*123 */ &Heap::vDump,            // SN_SMALLPERSISTENT
  /*124 */ &Heap::vDump,            // SN_SMALLTRANSIENT
  /*125 */ &Socket::vDump,          // CN_SOCKET
  /*126 */ &Stack::vDump,           // CN_STACK
  /*127 */ &Dump::vInvalid,         // CN_STEPACCUM
  /*128 */ &StepArray::vDump,       // CN_STEPARRAY
  /*129 */ &Dump::vInvalid,         // CN_SYSCALL

  /*130 */ &Table::vDump,           // CN_TABLE
  /*    */ &TablePtn::vDump,        // CN_TABLEPTN
  /*    */ &Dump::vDummy,           // CN_TABLEPTNVEC
  /*131 */ &Task::vDump,            // CN_TASK
  /*132 */ &Dump::vUndefined,       // CN_TASKCURSOR
  /*    */ &Dump::vUndefined,       // CN_TASKFIFO
  /*133 */ &TaskList::vDump,        // CN_TASKLIST
  /*134 */ &Dump::vUndefined,       // CN_TASKQUEUE
  /*134 */ &Dump::vUndefined,       // CN_SERVERSCHEDULER
  /*136 */ &Dump::vUndefined,       // CN_TASKSET
  /*137 */ &Dump::vUndefined,       // CN_THREAD
  /*138 */ &Dump::vUndefined,       // CN_THREADCURSOR
  /*139 */ &Dump::vUndefined,       // CN_THREADLIST
  /*140 */ &Dump::vUndefined,       // CN_THREADQUEUE
  /*    */ &Dump::vUndefined,       // CN_THREADVECTOR
  /*141 */ &Dump::vUndefined,       // CN_TIME
  /*142 */ &Dump::vInvalid,         // CN_TIMER
  /*143 */ &Token::vDump,           // CN_TOKEN
  /*144 */ &Tokeniser::vDump,       // CN_TOKENISER
  /*145 */ &Dump::vInvalid,         // CN_TOKENARY
  /*146 */ &Dump::vInvalid,         // CN_TOKENVEC
  /*147 */ &Tx::vDump,              // CN_TX
  /*148 */ &Dump::vUndefined,       // CN_TXSET

  /*149 */ &Dump::vInvalid,         // CN_WILDERNESS
  /*150 */ &Word::vDump,            // CN_WORD
  /*    */ &Dump::vUndefined,       // CN_WORKINGBUFFER
  /*    */ &Dump::vUndefined,       // CN_WORKINGROW
  
  /*151 */ &Dump::vUndefined,       // CN_ZSMALL_
  /*152 */ &Dump::vUndefined,       // CN_ZMEDIUM_
  /*153 */ &Dump::vUndefined,       // CN_ZLARGE_
  /*154 */ &Dump::vUndefined,       // CN_ZJUMBO_
  
  /*155 */ &Dump::vEnums            // CN_ENUMS
};

/*----------------------------------------------------------------------------*/
/* class sizes                                                                */
/*----------------------------------------------------------------------------*/
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
  /* 22 */ sizeof(Arg),                // CN_ARG
  /* 23 */ sizeof(Arguments),          // CN_ARGUMENTS
  /* 24 */ sizeof(Atomic),             // CN_ATOMIC
  /* 25 */ sizeof(AttachMap),          // CN_ATTACHMAP
  
  /* 26 */ sizeof(BitArray),           // CN_BITARRAY
  /* 27 */ sizeof(BitArray),           // CN_BITMAP
  /* 28 */ sizeof(BufferPool),         // CN_BUFFERPOOL
  /* 29 */ sizeof(BPA),                // CN_BPA

  /* 30 */ sizeof(CarryOver),          // CN_CARRYOVER
  /* 31 */ sizeof(Column),             // CN_COLUMN
  /* 32 */ sizeof(ColumnVec),          // CN_COLUMNVEC
  
  /* 33 */ sizeof(Dic),                // CN_DIC
  /* 34 */ sizeof(0),                  // CN_DICSTACK
  /* 34 */ sizeof(DicStack),           // CN_DICSTACK
  /* 35 */ sizeof(Dir),                // CN_DIR
  
  /* 36 */ sizeof(Entry),              // CN_ENTRY
  /* 37 */ sizeof(EntryBuffer),        // CN_ENTRYBUFFER
  /* 38 */ sizeof(EntryVec),           // CN_ENTRYVEC
  
  /* 39 */ sizeof(File),               // CN_FILE
  /* 40 */ sizeof(Formatters),         // CN_FORMATTERS
  /* 41 */ sizeof(Buffer),             // CN_FREEBUFFER
  /* 42 */ sizeof(FreeChunk),          // CN_FREECHUNK
  /* 43 */ 0,                          // CN_FREEDNOT
  /* 44 */ 0,                          // CN_FREEENTRY
  
  /* 45 */ sizeof(Graph),              // CN_GRAPH
  /* 46 */ sizeof(GraphCursor),        // CN_GRAPHCURSOR
  /* 47 */ sizeof(GraphNode),          // CN_GRAPHNODE
  /* 48 */ sizeof(Grid),               // CN_GRID
  /* 49 */ sizeof(GroupBy),            // CN_GROUPBY
  
  /* 50 */ sizeof(HeapBuffer),         // CN_HEAPBUFFER
  
  /* 51 */ sizeof(Index),              // CN_INDEX
  /* 52 */ sizeof(IndexTree),          // CN_INDEXTREE
  /* 53 */ sizeof(IndexTreeVec),       // CN_INDEXTREEVEC
  /* 54 */ sizeof(IdxColVec),          // CN_IDXCOLVEC
  /* 55 */ sizeof(IdxVec),             // CN_IDXVEC
  /* 56 */ 0,                          // CN_INDEXLEVELS
  /* 57 */ sizeof(Item),               // CN_ITEM
  /* 58 */ sizeof(ItemBuffer),         // CN_ITEMBUFFER
  
  /*    */ sizeof(Job__Begun),         // CN_JOB__BEGUN
  /*    */ sizeof(Job__End),           // CN_JOB__END
  /*    */ sizeof(Job__Ended),         // CN_JOB__ENDED
  /*    */ sizeof(Job__Error),         // CN_JOB__ERROR
  /*    */ sizeof(Job__Execute),       // CN_JOB__EXECUTE
  /*    */ sizeof(Job__Executed),      // CN_JOB__EXECUTED

  /*    */ sizeof(Job_CreateTable),   // CN_JOB_ANALYZEINDEX
  /*    */ sizeof(Job_CreateTable),   // CN_JOB_ANALYZETABLE
  /*    */ sizeof(Job_CreateCatalog),   // CN_JOB_CREATECATALOG
  /*    */ sizeof(Job_CreateTable),   // CN_JOB_CREATEINDEX
  /*    */ sizeof(Job_CreateSchema),  // CN_JOB_CREATESCHEMA
  /*    */ sizeof(Job_CreateTable),   // CN_JOB_CREATETABLE
  /*    */ sizeof(Job_CreateTenant),   // CN_JOB_CREATETENANT
  /*    */ sizeof(Job_CreateTable),   // CN_JOB_DROPCATALOG
  /*    */ sizeof(Job_CreateTable),   // CN_JOB_DROPINDEX
  /*    */ sizeof(Job_DropSchema),    // CN_JOB_DROPSCHEMA
  /*    */ sizeof(Job_DropTable),     // CN_JOB_DROPTABLE
  /*    */ sizeof(Job_CreateTable),   // CN_JOB_DROPTENANT
  /*    */ sizeof(Job_G35),           // CN_JOB_G35
  /*    */ sizeof(Job_CreateTable),   // CN_JOB_LOADTABLE
  /*    */ sizeof(Job_Space),         // CN_JOB_SPACE
                  
  /*    */ sizeof(Jobs),               // CN_JOBS
  
  /* 59 */ sizeof(Heap), // correct    // CN_JUMBOPERSISTENT
  /* 60 */ sizeof(Heap), // correct    // CN_JUMBOTRANSIENT
  
  /* 61 */ 0,                          // CN_KEY
  /* 62 */ sizeof(KeyVal),             // CN_KEYVAL
  /* 63 */ 0,                          // CN_KEYVEC
  /* 64 */ sizeof(KeyWrk),             // CN_KEYWRK
  
  /* 65 */ sizeof(Heap), // correct    // CN_LARGEPERSISTENT
  /* 66 */ sizeof(Heap), // correct    // CN_LARGETRANSIENT
  
  /* 67 */ sizeof(LineItem),           // CN_LINEITEM
  /* 68 */ sizeof(LoadChunk),          // CN_LOADCHUNK
  /* 69 */ sizeof(Loader),             // CN_LOADER
  /* 70 */ sizeof(LoaderSub),          // CN_LOADERSUB
  /* 71 */ sizeof(LoadMap),            // CN_LOADMAP
  /* 72 */ sizeof(Lock),               // CN_LOCK
  
  /* 73 */ sizeof(Heap), // correct    // CN_MEDIUMPERSISTENT
  /* 74 */ sizeof(Heap), // correct    // CN_MEDIUMTRANSIENT
  
  /* 75 */ sizeof(Node),               // CN_NODE
  
  /* 77 */ sizeof(Op),                 // CN_OP
  
  /* 78 */ sizeof(Parser),             // CN_PARSER
  /* 80 */ sizeof(PartitionScheduler), // CN_PARTITIONSCHEDULER
  /* 82 */ sizeof(PathVec),            // CN_PATHVEC
  /* 83 */ sizeof(PerformanceBuffer),  // CN_PERFORMANCEBUFFER
  /* 84 */ sizeof(PerformanceEntry),   // CN_PERFORMANCEENTRY
  /* 85 */ sizeof(PerformanceTrace),   // CN_PERFORMANCETRACE
  /* 86 */ sizeof(Plan),               // CN_PLAN
  /* 87 */ sizeof(Planner),            // CN_PLANNER
  /* 88 */ sizeof(PlanList),           // CN_PLANLIST
  /* 89 */ sizeof(PosixThread),        // CN_POSIXTHREAD
  /* 90 */ sizeof(Predicate),          // CN_PREDICATE
  /* 91 */ sizeof(PredicateVec),       // CN_PREDICATEVEC
  /* 92 */ sizeof(Proxy),              // CN_PROXY
  /* 93 */ sizeof(ProxySet),           // CN_PROXYSET
  /* 94 */ sizeof(PtnColVec),          // CN_PTNCOLVEC
  
  /* 95 */ sizeof(Queue),              // CN_QUEUE
  /* 96 */ sizeof(QueueBuffer),        // CN_QUEUEBUFFER
  /*    */ sizeof(Queues),             // CN_QUEUES
  
  /* 97 */ sizeof(RbCursor),           // CN_RBCURSOR
  /*    */ sizeof(RegFrame),           // CN_REGFRAME
  
  /*101 */ sizeof(Ring),               // CN_RING
  /*102 */ 0,                          // CN_ROARRAY
  /*103 */ 0,                          // CN_ROWBUFFER
  /*104 */ sizeof(RowCursor),          // CN_ROWCURSOR
  /*105 */ sizeof(RowOnce),            // CN_ROWONCE
  /*106 */ 0,                          // CN_ROWWRK
  
  /*107 */ sizeof(ScanBuffer),         // CN_SCANBUFFER
  /*108 */ sizeof(Scratch),            // CN_SCRATCH
  /*109 */ sizeof(SeHistory),          // CN_SEHISTORY
  /*110 */ sizeof(SEitf),              // CN_SEITF
  /*111 */ sizeof(SEitfCursor),        // CN_SEITFCURSOR
  /*112 */ sizeof(SEitfList),          // CN_SEITFLIST
  /*113 */ sizeof(SelCursor),          // CN_SELCURSOR
  /*114 */ sizeof(Semaphore),          // CN_SEMAPHORE
  /*115 */ sizeof(SePrompt),           // CN_SEPROMPT
  /*116 */ sizeof(Since),              // CN_SINCE
  /*117 */ sizeof(SkipCursor),         // CN_SKIPCURSOR
  /*118 */ sizeof(SkipList),           // CN_SKIPLIST
  /*119 */ sizeof(SkipNode),           // CN_SKIPNODE
  /*120 */ 0,                          // CN_SLAB
  /*121 */ sizeof(SlabAllocator),      // CN_SLABALLOCATOR
  /*122 */ sizeof(Slots),              // CN_SLOTS
  /*123 */ sizeof(Heap), // correct    // SN_SMALLPERSISTENT
  /*124 */ sizeof(Heap), // correct    // SN_SMALLTRANSIENT
  /*125 */ sizeof(Socket),             // CN_SOCKET
  /*126 */ sizeof(Stack),              // CN_STACK
  /*127 */ sizeof(StepAccum),          // CN_STEPACCUM
  /*128 */ sizeof(StepArray),          // CN_STEPARRAY
  /*129 */ sizeof(SysCall),            // CN_SYSCALL

  /*130 */ sizeof(Table),              // CN_TABLE
  /*    */ sizeof(TablePtn),           // CN_TABLEPTN
  /*    */ sizeof(TablePtnVec),        // CN_TABLEPTNVEC
  /*131 */ sizeof(Task),               // CN_TASK
  /*132 */ sizeof(TaskCursor),         // CN_TASKCURSOR
  /*    */ sizeof(TaskFifo),           // CN_TASKFIFO
  /*133 */ sizeof(TaskList),           // CN_TASKLIST
  /*134 */ sizeof(TaskQueue),          // CN_TASKQUEUE
  /*135 */ sizeof(ServerScheduler),    // CN_SERVERSCHEDULER
  /*136 */ sizeof(TaskSet),            // CN_TASKSET
  /*137 */ sizeof(Thread),             // CN_THREAD
  /*138 */ sizeof(ThreadCursor),       // CN_THREADCURSOR
  /*139 */ sizeof(ThreadList),         // CN_THREADLIST
  /*140 */ sizeof(ThreadQueue),        // CN_THREADQUEUE
  /*    */ sizeof(ThreadVector),       // CN_THREADVECTOR
  /*141 */ sizeof(Time),               // CN_TIME
  /*142 */ sizeof(Timer),              // CN_TIMER
  /*143 */ sizeof(Token),              // CN_TOKEN
  /*144 */ sizeof(Tokeniser),          // CN_TOKENISER
  /*145 */ sizeof(TokenAry),           // CN_TOKENARY
  /*146 */ sizeof(TokenVec),           // CN_TOKENVEC
  /*147 */ sizeof(Tx),                 // CN_TX
  /*148 */ sizeof(TxSet),              // CN_TXSET

  /*149 */ sizeof(Wilderness),         // CN_WILDERNESS
  /*150 */ sizeof(Word),               // CN_WORD
  /*    */ sizeof(WorkingBuffer),      // CN_WORKINGBUFFER
  /*100 */ 0,                          // CN_WORKINGROW
  
  /*151 */ C_SIZE_SMALL_OBJECT  - 256, // CN_ZSMALL_
  /*152 */ C_SIZE_MEDIUM_OBJECT - 256, // CN_ZMEDIUM_
  /*153 */ C_SIZE_POOL_BUFFER   - 256, // CN_ZLARGE_
  /*154 */ C_SIZE_JUMBO_OBJECT  - 256, // CN_ZJUMBO_

  /*155 */ -1
};

/*----------------------------------------------------------------------------*/
/* which heap                                                                 */
/*----------------------------------------------------------------------------*/
eHT geWhichHeap[]
{
  /*  0 */ HT_INVALID,           // CN_NONE
  
  /*  1 */ HT_INVALID,           // CN_BCDVAL
  /*  2 */ HT_INVALID,           // CN_BINVAL
  /*  3 */ HT_INVALID,           // CN_BOLVAL
  /*  4 */ HT_INVALID,           // CN_DTEVAL
  /*  5 */ HT_INVALID,           // CN_ENTWRK,
  /*  6 */ HT_INVALID,           // CN_FLTVAL
  /*  7 */ HT_INVALID,           // CN_INTVAL
  /*  8 */ HT_INVALID,           // CN_IPVAL
  /*  9 */ HT_INVALID,           // CN_IVLVAL
  /* 10 */ HT_INVALID,           // CN_OBJVAL
  /* 11 */ HT_INVALID,           // CN_PTRVAL
  /* 12 */ HT_INVALID,           // CN_STRVAL
  /* 13 */ HT_INVALID,           // CN_STRVAR
  /* 14 */ HT_INVALID,           // CN_TMEVAL
  /* 15 */ HT_INVALID,           // CN_TSPVAL
  /* 16 */ HT_INVALID,           // CN_VALREG
  /* 17 */ HT_TRANSIENT_MEDIUM,  // CN_VALSET
  /* 18 */ HT_INVALID,           // CN_VALWRK
  
  /* 19 */ HT_TRANSIENT_MEDIUM,  // CN_ACCESSPATH
  /* 20 */ HT_TRANSIENT_SMALL,   // CN_ACCUM
  /* 21 */ HT_TRANSIENT_MEDIUM,  // CN_ACCUMVEC
  /* 22 */ HT_TRANSIENT_SMALL,   // CN_ARG
  /* 23 */ HT_INVALID,           // CN_ARGUMENTS
  /* 24 */ HT_INVALID,           // CN_ATOMIC
  /* 25 */ HT_INVALID,           // CN_ATTACHMAP
  
  /* 26 */ HT_TRANSIENT_SMALL,   // CN_BITARRAY
  /* 27 */ HT_TRANSIENT_LARGE,   // CN_BITMAP
  /* 28 */ HT_INVALID,           // CN_BUFFERPOOL
  /* 29 */ HT_INVALID,           // CN_BPA

  /* 30 */ HT_INVALID,           // CN_CARRYOVER
  /* 31 */ HT_TRANSIENT_SMALL,   // CN_COLUMN
  /* 32 */ HT_TRANSIENT_MEDIUM,  // CN_COLUMNVEC
  
  /* 33 */ HT_TRANSIENT_MEDIUM,  // CN_DIC
  /*    */ HT_INVALID,           // CN_DICPATH
  /* 34 */ HT_INVALID,           // CN_DICSTACK
  /* 35 */ HT_TRANSIENT_SMALL,   // CN_DIR

  /* 36 */ HT_INVALID,           // CN_ENTRY
  /* 37 */ HT_TRANSIENT_LARGE,   // CN_ENTRYBUFFER
  /* 38 */ HT_INVALID,           // CN_ENTRYVEC
  
  /* 39 */ HT_TRANSIENT_SMALL,   // CN_FILE
  /* 40 */ HT_TRANSIENT_SMALL,   // CN_FORMATTERS
  /* 41 */ HT_INVALID,           // CN_FREEBUFFER
  /* 42 */ HT_INVALID,           // CN_FREECHUNK
  /* 43 */ HT_INVALID,           // CN_FREEDNOT
  /* 44 */ HT_INVALID,           // CN_FREEENTRY
  
  /* 45 */ HT_TRANSIENT_SMALL,   // CN_GRAPH
  /* 46 */ HT_TRANSIENT_SMALL,   // CN_GRAPHCURSOR
  /* 47 */ HT_TRANSIENT_SMALL,   // CN_GRAPHNODE
  /* 48 */ HT_TRANSIENT_LARGE,   // CN_GRID
  /* 49 */ HT_TRANSIENT_SMALL,   // CN_GROUPBY

  /* 50 */ HT_INVALID,           // CN_HEAPBUFFER
  
  /* 51 */ HT_INVALID,           // CN_INDEX
  /* 52 */ HT_TRANSIENT_SMALL,   // CN_INDEXTREE
  /* 53 */ HT_TRANSIENT_MEDIUM,  // CN_INDEXTREEVEC
  /* 54 */ HT_TRANSIENT_MEDIUM,  // CN_IDXCOLVEC
  /* 55 */ HT_TRANSIENT_MEDIUM,  // CN_IDXVEC
  /* 56 */ HT_TRANSIENT_MEDIUM,  // CN_INDEXLEVELS
  /* 57 */ HT_INVALID,           // CN_ITEM
  /* 58 */ HT_INVALID,           // CN_ITEMBUFFER

  /*    */ HT_TRANSIENT_SMALL,   // CN_JOB__BEGUN
  /*    */ HT_TRANSIENT_SMALL,   // CN_JOB__END
  /*    */ HT_TRANSIENT_SMALL,   // CN_JOB__ENDED
  /*    */ HT_TRANSIENT_SMALL,   // CN_JOB__ERROR
  /*    */ HT_TRANSIENT_SMALL,   // CN_JOB__EXECUTE
  /*    */ HT_TRANSIENT_SMALL,   // CN_JOB__EXECUTED
  
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_ANALYZEINDEX
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_ANALYZETABLE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_CREATECATALOG
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_CREATEINDEX
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_CREATESCHEMA
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_CREATETABLE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_CREATETENANT
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_DROPCATALOG
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_DROPINDEX
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_DROPSCHEMA
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_DROPTABLE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_DROPTENANT
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_G35
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_LOADTABLE
  /*    */ HT_TRANSIENT_LARGE,   // CN_JOB_SPACE
  
  /*    */ HT_TRANSIENT_SMALL,   // CN_JOBS
  
  /* 59 */ HT_INVALID,           // CN_JUMBOPERSISTENT
  /* 60 */ HT_INVALID,           // CN_JUMBOTRANSIENT
  
  /* 61 */ HT_INVALID,           // CN_KEY
  /* 62 */ HT_TRANSIENT_SMALL,   // CN_KEYVAL
  /* 63 */ HT_TRANSIENT_MEDIUM,  // CN_KEYVEC
  /* 64 */ HT_INVALID,           // CN_KEYWRK

  /* 65 */   HT_INVALID,           // CN_LARGEPERSISTENT
  /* 66 */   HT_INVALID,           // CN_LARGETRANSIENT
  
  /* 67 */ HT_INVALID,           // CN_LINEITEM
  /* 68 */ HT_TRANSIENT_LARGE,   // CN_LOADCHUNK
  /* 69 */ HT_TRANSIENT_MEDIUM,  // CN_LOADER
  /* 70 */ HT_TRANSIENT_MEDIUM,  // CN_LOADERSUB
  /* 71 */ HT_TRANSIENT_JUMBO,   // CN_LOADMAP
  /* 72 */ HT_INVALID,           // CN_LOCK
  
  /* 73 */ HT_INVALID,           // CN_MEDIUMPERSISTENT
  /* 74 */ HT_INVALID,           // CN_MEDIUMTRANSIENT
  
  /* 75 */ HT_TRANSIENT_SMALL,   // CN_NODE
  
  /* 77 */ HT_TRANSIENT_SMALL,   // CN_OP
  
  /* 78 */ HT_TRANSIENT_MEDIUM,  // CN_PARSER
  /* 80 */ HT_TRANSIENT_SMALL,   // CN_PARTITIONSCHEDULER
  /* 82 */ HT_TRANSIENT_MEDIUM,  // CN_PATHVEC
  /* 83 */ HT_TRANSIENT_LARGE,   // CN_PERFORMANCEBUFFER
  /* 84 */ HT_INVALID,           // CN_PERFORMANCEENTRY
  /* 85 */ HT_TRANSIENT_SMALL,   // CN_PERFORMANCETRACE
  /* 86 */ HT_TRANSIENT_MEDIUM,  // CN_PLAN
  /* 87 */ HT_TRANSIENT_MEDIUM,  // CN_PLANNER
  /* 88 */ HT_TRANSIENT_SMALL,   // CN_PLANLIST
  /* 89 */ HT_TRANSIENT_SMALL,   // CN_PREDICATE
  /* 90 */ HT_TRANSIENT_MEDIUM,  // CN_PREDICATEVEC
  /* 91 */ HT_TRANSIENT_SMALL,   // CN_POSIXTHREAD
  /* 92 */ HT_INVALID,           // CN_PROXY
  /* 93 */ HT_INVALID,           // CN_PROXYSET
  /* 94 */ HT_TRANSIENT_MEDIUM,  // CN_PTNCOLVEC

  /* 95 */ HT_TRANSIENT_SMALL,   // CN_QUEUE
  /* 96 */ HT_INVALID,           // CN_QUEUEBUFFER
  /*    */ HT_TRANSIENT_SMALL,   // CN_QUEUES
  
  /* 97 */ HT_TRANSIENT_SMALL,   // CN_RBCURSOR
  /*    */ HT_TRANSIENT_LARGE,   // CN_REGFRAME
  
  /*101 */ HT_INVALID,           // CN_RING
  /*102 */ HT_TRANSIENT_MEDIUM,  // CN_ROARRAY
  /*103 */ HT_INVALID,           // CN_ROWBUFFER
  /*104 */ HT_TRANSIENT_SMALL,   // CN_ROWCURSOR
  /*105 */ HT_TRANSIENT_SMALL,   // CN_ROWONCE
  /*106 */ HT_INVALID,           // CN_ROWWRK
  
  /*107 */ HT_TRANSIENT_LARGE,   // CN_SCANBUFFER
  /*108 */ HT_TRANSIENT_LARGE,   // CN_SCRATCH
  /*109 */ HT_INVALID,           // CN_SEHISTORY
  /*110 */ HT_TRANSIENT_MEDIUM,  // CN_SEITF
  /*111 */ HT_TRANSIENT_SMALL,   // CN_SEITFCURSOR
  /*112 */ HT_INVALID,           // CN_SEITFLIST
  /*113 */ HT_TRANSIENT_SMALL,   // CN_SELCURSOR
  /*114 */ HT_TRANSIENT_SMALL,   // CN_SEMAPHORE
  /*115 */ HT_TRANSIENT_LARGE,   // CN_SEPROMPT
  /*116 */ HT_INVALID,           // CN_SINCE
  /*117 */ HT_INVALID,           // CN_SKIPCURSOR
  /*118 */ HT_TRANSIENT_SMALL,   // CN_SKIPLIST
  /*119 */ HT_TRANSIENT_SMALL,   // CN_SKIPNODE
  /*120 */ HT_TRANSIENT_LARGE,   // CN_SLAB
  /*121 */ HT_TRANSIENT_SMALL,   // CN_SLABALLOCATOR
  /*122 */ HT_INVALID,           // CN_SLOTS
  /*123 */ HT_INVALID,           // SN_SMALLPERSISTENT
  /*124 */ HT_INVALID,           // SN_SMALLTRANSIENT
  /*125 */ HT_TRANSIENT_SMALL,   // CN_SOCKET
  /*126 */ HT_TRANSIENT_LARGE,   // CN_STACK
  /*127 */ HT_INVALID,           // CN_STEPACCUM
  /*128 */ HT_TRANSIENT_LARGE,   // CN_STEPARRAY
  /*129 */ HT_INVALID,           // CN_SYSCALL
  
  /*130 */ HT_INVALID,           // CN_TABLE
  /*    */ HT_TRANSIENT_SMALL,   // CN_TABLEPTN
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_TABLEPTNVEC
  /*131 */ HT_TRANSIENT_MEDIUM,  // CN_TASK
  /*132 */ HT_INVALID,           // CN_TASKCURSOR
  /*    */ HT_TRANSIENT_SMALL,   // CN_TASKFIFO
  /*133 */ HT_TRANSIENT_SMALL,   // CN_TASKLIST
  /*134 */ HT_TRANSIENT_SMALL,   // CN_TASKQUEUE
  /*135 */ HT_TRANSIENT_MEDIUM,  // CN_SERVERSCHEDULER
  /*136 */ HT_TRANSIENT_SMALL,   // CN_TASKSET
  /*137 */ HT_TRANSIENT_SMALL,   // CN_THREAD
  /*138 */ HT_TRANSIENT_SMALL,   // CN_THREADCURSOR
  /*139 */ HT_INVALID,           // CN_THREADLIST
  /*140 */ HT_INVALID,           // CN_THREADQUEUE
  /*    */ HT_TRANSIENT_MEDIUM,  // CN_THREADVECTOR
  /*141 */ HT_INVALID,           // CN_TIME
  /*142 */ HT_TRANSIENT_SMALL,   // CN_TIMER
  /*143 */ HT_TRANSIENT_SMALL,   // CN_TOKEN
  /*144 */ HT_TRANSIENT_MEDIUM,  // CN_TOKENISER
  /*145 */ HT_INVALID,           // CN_TOKENARY
  /*146 */ HT_TRANSIENT_SMALL,   // CN_TOKENVEC
  /*147 */ HT_TRANSIENT_SMALL,   // CN_TX
  /*148 */ HT_TRANSIENT_SMALL,   // CN_TXSET
  
  /*149 */ HT_INVALID,           // CN_WILDERNESS
  /*150 */ HT_TRANSIENT_MEDIUM,  // CN_WORD
  /*    */ HT_TRANSIENT_LARGE,   // CN_WORKINGBUFFER
  /*    */ HT_INVALID,           // CN_WORKINGROW
  
  /*151 */ HT_INVALID,           // CN_ZSMALL_
  /*152 */ HT_INVALID,           // CN_ZMEDIUM_
  /*153 */ HT_INVALID,           // CN_ZLARGE_
  /*154 */ HT_INVALID,           // CN_ZJUMBO_

  /*155 */ HT_INVALID,           // CN_ENUMS
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
