/*============================================================================*/
/* ForwardClasses.h -- Forward Class declarations                             */
/*============================================================================*/
#pragma once

namespace Assembler     {                                                     }
namespace Axioms        {                                                     }
namespace Base          {                                                     }
namespace Chunks        { class Formatters;                                   }
namespace Commands      { class Command; class Parser; class Tokeniser;       }
namespace Correctness   {                                                     }
namespace Debugging     {                                                     }
namespace Forth         { class Dic; class ForthWord;                         }
namespace Grids         { class Grid; class Node;                             }
namespace Indexes       {                                                     }
namespace Interface     { class ApiState; class RecordBuffer; class Session;  }
namespace Jobs          { class Job; class JobList;
                                     class Job_CreateTable;
                                     class Job_CreateIndex;                   }
namespace Logging       {                                                     }
namespace MetaData      {                                                     }
namespace Measurement   { class EnumFlags; class LockTracker;
                                           class ResourceTracker;
                                           class Statistics;
                                           class StepTiming;                  }
namespace Partitioning  { class TablePtn;                                     }
namespace Persistence   { class CarryOver;                                    }
namespace Plans         {                                                     }
namespace Pool          { class BufferPool; class HeapMem; class PoolBuffer;  }
namespace Prompt        { class SePrompt;                                     }
namespace PushDown      {                                                     }
namespace Rows          { class IndexTree; class CrwBuffer;                   }
namespace ScanSprint    {                                                     }
namespace Scheduling    { class Queues; class Scheduler;                      }
namespace Steps         {                                                     }
namespace StorageEngine { class AM; class Engine;                             }
namespace Structures    {                                                     }
namespace System        { class Lock;                                         }
namespace Tasks         { class Task;                                         }
namespace Threads       { class Thread;                                       }
namespace Transactions  { class Tx; class TxSet;                              }
namespace Transform     {                                                     }
namespace Utilities     {                                                     }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
