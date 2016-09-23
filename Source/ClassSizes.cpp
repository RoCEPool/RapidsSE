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
  /* 21 */ sizeof(Loader3),
  /* 22 */ sizeof(Loader3Sub),
  /* 23 */ sizeof(Lock),
  /* 24 */ sizeof(Heap),
  /* 25 */ sizeof(Node),
  /* 26 */ sizeof(NodeCursor),
  /* 27 */ sizeof(Parser),
  /* 28 */ sizeof(Partition),
  /* 29 */ sizeof(Partitions),
  /* 30 */ sizeof(Proxy),
  /* 31 */ sizeof(ProxySet),
  /* 32 */ sizeof(Queue),
  /* 33 */ sizeof(QueueBuffer),
  /* 34 */ sizeof(Ring),
  /* 35 */ sizeof(Row),
  /* 36 */ sizeof(RowBuffer),
  /* 37 */ sizeof(RowCursor),
  /* 38 */ sizeof(RowStrand),
  /* 39 */ sizeof(RowWsp),
  /* 40 */ sizeof(ScanBuffer),
  /* 41 */ sizeof(SeHistory),
  /* 42 */ sizeof(SEitf),
  /* 43 */ sizeof(SEitfCursor),
  /* 44 */ sizeof(SEitfList),
  /* 45 */ sizeof(SePrompt),
  /* 46 */ sizeof(Since),
  /* 47 */ sizeof(SkipCursor),
  /* 48 */ sizeof(SkipList),
  /* 49 */ sizeof(SkipNode),
  /* 50 */ sizeof(Heap),
  /* 51 */ sizeof(Socket),
  /* 52 */ sizeof(Stack),
  /* 53 */ sizeof(StepAccum),
  /* 54 */ sizeof(StepArray),
  /* 55 */ sizeof(SysCall),
  /* 56 */ sizeof(Table),
  /* 57 */ sizeof(Task),
  /* 58 */ sizeof(TaskCursor),
  /* 59 */ sizeof(TaskList),
  /* 60 */ sizeof(TaskSet),
  /* 61 */ sizeof(Thread),
  /* 62 */ sizeof(ThreadCursor),
  /* 63 */ sizeof(ThreadList),
  /* 64 */ sizeof(Time),
  /* 65 */ sizeof(Timer),
  /* 66 */ sizeof(Tokeniser),
  /* 67 */ sizeof(Tx),
  /* 68 */ sizeof(TxArray),
  /* 69 */ sizeof(TxCollection),
  /* 70 */ sizeof(TxCursor),
  /* 71 */ sizeof(TxMap),
  /* 72 */ sizeof(TxRef),
  /* 73 */ sizeof(Word),
  /* 74 */ -1
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/