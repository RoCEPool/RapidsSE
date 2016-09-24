/*============================================================================*/
/* ClassSizes.cpp -- Class Sizes                                              */
/*============================================================================*/

#include "StorageEngine.h"
#include "SePrompt.hpp"

i8 giClassSize[]
{
  /*  0 */ 0,

  /*  1 */ sizeof(s32),
  /*  1 */ sizeof(s256),
  /*  1 */ sizeof(s8K),
  
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
  /* 22 */ sizeof(LoaderSub),
  /* 23 */ sizeof(Lock),
  /* 24 */ sizeof(Heap),
  /* 25 */ sizeof(Heap),
  /* 26 */ sizeof(Node),
  /* 27 */ sizeof(NodeCursor),
  /* 28 */ sizeof(Parser),
  /* 29 */ sizeof(Partition),
  /* 30 */ sizeof(Partitions),
  /* 31 */ sizeof(Proxy),
  /* 32 */ sizeof(ProxySet),
  /* 33 */ sizeof(Queue),
  /* 34 */ sizeof(QueueBuffer),
  /* 35 */ sizeof(Ring),
  /* 36 */ sizeof(Row),
  /* 37 */ sizeof(RowBuffer),
  /* 38 */ sizeof(RowCursor),
  /* 39 */ sizeof(RowStrand),
  /* 40 */ sizeof(RowWsp),
  /* 41 */ sizeof(ScanBuffer),
  /* 42 */ sizeof(SeHistory),
  /* 43 */ sizeof(SEitf),
  /* 44 */ sizeof(SEitfCursor),
  /* 45 */ sizeof(SEitfList),
  /* 46 */ sizeof(SePrompt),
  /* 47 */ sizeof(Since),
  /* 48 */ sizeof(SkipCursor),
  /* 49 */ sizeof(SkipList),
  /* 50 */ sizeof(SkipNode),
  /* 51 */ sizeof(Heap),
  /* 52 */ sizeof(Socket),
  /* 53 */ sizeof(Stack),
  /* 54 */ sizeof(StepAccum),
  /* 55 */ sizeof(StepArray),
  /* 56 */ sizeof(SysCall),
  /* 57 */ sizeof(Table),
  /* 58 */ sizeof(Task),
  /* 59 */ sizeof(TaskCursor),
  /* 60 */ sizeof(TaskList),
  /* 61 */ sizeof(TaskSet),
  /* 62 */ sizeof(Thread),
  /* 63 */ sizeof(ThreadCursor),
  /* 64 */ sizeof(ThreadList),
  /* 65 */ sizeof(Time),
  /* 66 */ sizeof(Timer),
  /* 67 */ sizeof(Tokeniser),
  /* 68 */ sizeof(Tx),
  /* 69 */ sizeof(TxArray),
  /* 70 */ sizeof(TxCollection),
  /* 71 */ sizeof(TxCursor),
  /* 72 */ sizeof(TxMap),
  /* 73 */ sizeof(TxRef),
  /* 74 */ sizeof(Word),
  /* 75 */ -1
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/