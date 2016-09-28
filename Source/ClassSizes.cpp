/*============================================================================*/
/* ClassSizes.cpp -- Class Sizes                                              */
/*============================================================================*/

#include "StorageEngine.h"
#include "SePrompt.hpp"

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
  /* 16 */ sizeof(BufferPoolAnalyzer), // CN_BUFFERPOOLANALYZER
  
  /* 17 */ sizeof(Dictionary),         // CN_DICTIONARY
  /* 18 */ sizeof(DictionaryStack),    // CN_DICTIONARYSTACK
  /* 19 */ sizeof(Dir),                // CN_DIR
  
  /* 20 */ sizeof(File),               // CN_FILE
  /* 21 */ sizeof(FreeChunk),          // CN_FREECHUNK
  
  /* 22 */ sizeof(Grid),               // CN_GRID
  
  /* 23 */ sizeof(HeapBuffer),         // CN_HEAPBUFFER
  
  /* 24 */ sizeof(Index),              // CN_INDEX
  
  /* 25 */ sizeof(Heap), // correct    // CN_JUMBOHEAP
  
  /* 26 */ sizeof(KeyCpp),             // CN_KEYCPP
  /* 27 */ sizeof(KeyVal),             // CN_KEYVAL
  
  /* 28 */ sizeof(Loader),             // CN_LOADER
  /* 29 */ sizeof(LoaderSub),          // CN_LOADERSUB
  /* 30 */ sizeof(Lock),               // CN_LOCK
  
  /* 31 */ sizeof(Heap), // correct    // CN_MEDIUMHEAP
  
  /* 32 */ sizeof(Node),               // CN_NODE
  /* 33 */ sizeof(NodeCursor),         // CN_NODECURSOR
  
  /* 34 */ sizeof(Parser),             // CN_PARSER
  /* 35 */ sizeof(Partition),          // CN_PARTITION
  /* 36 */ sizeof(Partitions),         // CN_PARTITIONS
  /* 37 */ sizeof(Proxy),              // CN_PROXY
  /* 38 */ sizeof(ProxySet),           // CN_PROXYSET
  
  /* 39 */ sizeof(Queue),              // CN_QUEUE
  /* 40 */ sizeof(QueueBuffer),        // CN_QUEUEBUFFER
  
  /* 41 */ sizeof(RegFrame),           // CN_REGFRAME
  /* 42 */ sizeof(Ring),               // CN_RING
  /* 43 */ sizeof(Row),                // CN_ROW
  /* 44 */ sizeof(RowBuffer),          // CN_ROWBUFFER
  /* 45 */ sizeof(RowCursor),          // CN_ROWCURSOR
  /* 46 */ sizeof(RowStrand),          // CN_ROWSTRAND
  /* 47 */ sizeof(RowWsp),             // CN_ROWWSP
  
  /* 48 */ sizeof(ScanBuffer),         // CN_SCANBUFFER
  /* 49 */ sizeof(SeHistory),          // CN_SEHISTORY
  /* 50 */ sizeof(SEitf),              // CN_SEITF
  /* 51 */ sizeof(SEitfCursor),        // CN_SEITFCURSOR
  /* 52 */ sizeof(SEitfList),          // CN_SEITFLIST
  /* 53 */ sizeof(SePrompt),           // CN_SEPROMPT
  /* 54 */ sizeof(Since),              // CN_SINCE
  /* 55 */ sizeof(SkipCursor),         // CN_SKIPCURSOR
  /* 56 */ sizeof(SkipList),           // CN_SKIPLIST
  /* 57 */ sizeof(SkipMap),            // CN_SKIPMAP
  /* 58 */ sizeof(SkipNode),           // CN_SKIPNODE
  /* 59 */ sizeof(Heap), // correct    // CN_SMALLHEAP
  /* 60 */ sizeof(Socket),             // CN_SOCKET
  /* 61 */ sizeof(Stack),              // CN_STACK
  /* 62 */ sizeof(StepAccum),          // CN_STEPACCUM
  /* 63 */ sizeof(StepArray),          // CN_STEPARRAY
  /* 64 */ sizeof(SysCall),            // CN_SYSCALL
  
  /* 65 */ sizeof(Table),              // CN_TABLE
  /* 66 */ sizeof(Task),               // CN_TASK
  /* 67 */ sizeof(TaskCursor),         // CN_TASKCURSOR
  /* 68 */ sizeof(TaskList),           // CN_TASKLIST
  /* 69 */ sizeof(TaskSet),            // CN_TASKSET
  /* 70 */ sizeof(Thread),             // CN_THREAD
  /* 71 */ sizeof(ThreadCursor),       // CN_THREADCURSOR
  /* 72 */ sizeof(ThreadList),         // CN_THREADLIST
  /* 73 */ sizeof(Time),               // CN_TIME
  /* 74 */ sizeof(Timer),              // CN_TIMER
  /* 75 */ sizeof(Tokeniser),          // CN_TOKENISER
  /* 76 */ sizeof(Tx),                 // CN_TX
  /* 77 */ sizeof(TxArray),            // CN_TXARRAY
  /* 78 */ sizeof(TxCollection),       // CN_TXCOLLECTION
  /* 79 */ sizeof(TxCursor),           // CN_TXCURSOR
  /* 80 */ sizeof(TxMap),              // CN_TXMAP
  /* 81 */ sizeof(TxRef),              // CN_TXREF
  
  /* 82 */ sizeof(Word),               // CN_WORD
  
  /* 83 */ -1
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/