/*============================================================================*/
/* ClassSizes.cpp -- Class Sizes                                              */
/*============================================================================*/

#include "StorageEngine.h"
#include "SePrompt.hpp"

i8 giClassSize[]
{
  /*  0 */ 0,
  /*  1 */ sizeof(IntVal),
  /*  2 */ sizeof(ObjVal),
  /*  3 */ sizeof(PtrVal),
  /*  4 */ sizeof(StrVal),
  /*  5 */ sizeof(ValWsp), // correct
  /*  6 */ sizeof(ValWsp), // correct
  
  /*  7 */ sizeof(Arguments),
  /*  8 */ sizeof(Atomic),
  /*  9 */ sizeof(AttachMap),
  /* 10 */ sizeof(Buffer),
  /* 11 */ sizeof(BufferPool),
  /* 12 */ sizeof(BufferPoolAnalyzer),
  /* 13 */ sizeof(Dictionary),
  /* 14 */ sizeof(DictionaryStack),
  /* 15 */ sizeof(Dir),
  /* 16 */ sizeof(File),
  /* 17 */ sizeof(FreeChunk),
  /* 18 */ sizeof(Grid),
  /* 19 */ sizeof(HeapBuffer),
  /* 20 */ sizeof(Index),
  /* 21 */ sizeof(Loader),
  /* 22 */ sizeof(Lock),
  /* 23 */ sizeof(Heap),
  /* 24 */ sizeof(Node),
  /* 25 */ sizeof(NodeCursor),
  /* 26 */ sizeof(Parser),
  /* 27 */ sizeof(Partition),
  /* 28 */ sizeof(Partitions),
  /* 29 */ sizeof(Proxy),
  /* 30 */ sizeof(ProxySet),
  /* 31 */ sizeof(Queue),
  /* 32 */ sizeof(QueueBuffer),
  /* 33 */ sizeof(Ring),
  /* 34 */ sizeof(Row),
  /* 35 */ sizeof(RowBuffer),
  /* 36 */ sizeof(RowCursor),
  /* 37 */ sizeof(RowStrand),
  /* 38 */ sizeof(ScanBuffer),
  /* 39 */ sizeof(SeHistory),
  /* 40 */ sizeof(SEitf),
  /* 41 */ sizeof(SEitfCursor),
  /* 42 */ sizeof(SEitfList),
  /* 43 */ sizeof(SePrompt),
  /* 44 */ sizeof(Since),
  /* 45 */ sizeof(SkipCursor),
  /* 46 */ sizeof(SkipList),
  /* 47 */ sizeof(SkipNode),
  /* 48 */ sizeof(Heap),
  /* 49 */ sizeof(Socket),
  /* 50 */ sizeof(Stack),
  /* 51 */ sizeof(StepAccum),
  /* 52 */ sizeof(StepArray),
  /* 53 */ sizeof(SysCall),
  /* 54 */ sizeof(Table),
  /* 55 */ sizeof(Task),
  /* 56 */ sizeof(TaskCursor),
  /* 57 */ sizeof(TaskList),
  /* 58 */ sizeof(TaskSet),
  /* 59 */ sizeof(Thread),
  /* 60 */ sizeof(ThreadCursor),
  /* 61 */ sizeof(ThreadList),
  /* 62 */ sizeof(Time),
  /* 63 */ sizeof(Timer),
  /* 64 */ sizeof(Tokeniser),
  /* 65 */ sizeof(Tx),
  /* 66 */ sizeof(TxArray),
  /* 67 */ sizeof(TxCollection),
  /* 68 */ sizeof(TxCursor),
  /* 69 */ sizeof(TxMap),
  /* 70 */ sizeof(TxRef),
  /* 71 */ sizeof(Word)
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/