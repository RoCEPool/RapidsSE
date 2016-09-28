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
  /*  5 */ sizeof(ValReg),
  /*  6 */ sizeof(ValWsp), // correct
  /*  7 */ sizeof(ValWsp), // correct
  
  /*  8 */ sizeof(Arguments),
  /*  9 */ sizeof(Atomic),
  /* 10 */ sizeof(AttachMap),
  /* 11 */ sizeof(Buffer),
  /* 12 */ sizeof(BufferPool),
  /* 13 */ sizeof(BufferPoolAnalyzer),
  /* 14 */ sizeof(Dictionary),
  /* 15 */ sizeof(DictionaryStack),
  /* 16 */ sizeof(Dir),
  /* 17 */ sizeof(File),
  /* 18 */ sizeof(FreeChunk),
  /* 19 */ sizeof(Grid),
  /* 20 */ sizeof(HeapBuffer),
  /* 21 */ sizeof(Index),
  /*    */ sizeof(KeyCpp),
  /*    */ sizeof(KeyVal),
  /* 22 */ sizeof(Loader),
  /* 23 */ sizeof(LoaderSub),
  /* 24 */ sizeof(Lock),
  /* 25 */ sizeof(Heap),
  /* 26 */ sizeof(Heap),
  /* 27 */ sizeof(Node),
  /* 28 */ sizeof(NodeCursor),
  /* 29 */ sizeof(Parser),
  /* 30 */ sizeof(Partition),
  /* 31 */ sizeof(Partitions),
  /* 32 */ sizeof(Proxy),
  /* 33 */ sizeof(ProxySet),
  /* 34 */ sizeof(Queue),
  /* 35 */ sizeof(QueueBuffer),
  /*    */ sizeof(RegFrame),
  /* 36 */ sizeof(Ring),
  /* 37 */ sizeof(Row),
  /* 38 */ sizeof(RowBuffer),
  /* 39 */ sizeof(RowCursor),
  /* 40 */ sizeof(RowStrand),
  /* 41 */ sizeof(RowWsp),
  /* 42 */ sizeof(ScanBuffer),
  /* 43 */ sizeof(SeHistory),
  /* 44 */ sizeof(SEitf),
  /* 45 */ sizeof(SEitfCursor),
  /* 46 */ sizeof(SEitfList),
  /* 47 */ sizeof(SePrompt),
  /* 48 */ sizeof(Since),
  /* 49 */ sizeof(SkipCursor),
  /* 50 */ sizeof(SkipList),
  /* 51 */ sizeof(SkipNode),
  /* 52 */ sizeof(Heap),
  /* 53 */ sizeof(Socket),
  /*    */ sizeof(SkipMap),
  /* 54 */ sizeof(Stack),
  /* 55 */ sizeof(StepAccum),
  /* 56 */ sizeof(StepArray),
  /* 57 */ sizeof(SysCall),
  /* 58 */ sizeof(Table),
  /* 59 */ sizeof(Task),
  /* 60 */ sizeof(TaskCursor),
  /* 61 */ sizeof(TaskList),
  /* 62 */ sizeof(TaskSet),
  /* 63 */ sizeof(Thread),
  /* 64 */ sizeof(ThreadCursor),
  /* 65 */ sizeof(ThreadList),
  /* 66 */ sizeof(Time),
  /* 67 */ sizeof(Timer),
  /* 68 */ sizeof(Tokeniser),
  /* 69 */ sizeof(Tx),
  /* 70 */ sizeof(TxArray),
  /* 71 */ sizeof(TxCollection),
  /* 72 */ sizeof(TxCursor),
  /* 73 */ sizeof(TxMap),
  /* 74 */ sizeof(TxRef),
  /* 75 */ sizeof(Word),
  /* 76 */ -1
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/