/*============================================================================*/
/* ForwardClasses.h -- Forward Class declarations                             */
/*============================================================================*/
#pragma once

namespace Alignment     {                                                     }
namespace Assembler     {                                                     }
namespace Base          {                                                     }
namespace Chunks        { class Formatters;                                   }
namespace ClassData     {                                                     }
namespace Commands      { class Command; class Parser; class Tokeniser;       }
namespace ControlData   {                                                     }
namespace Correctness   {                                                     }
namespace Debugging     {                                                     }
namespace Enums         {                                                     }
namespace Forth         { class Cell; class Dic; class ForthWord;             }
namespace Grids         { class Grid; class Node;                             }
namespace Indexes       { class IndexTree;                                    }
namespace Interface     { class ApiState;                                     }
namespace Jobs          { class Job; struct JobClass;  class JobList;
                                     class Job_CreateTable;
                                     class Job_CreateIndex;
                                     class Job_Dynamic;                       }
namespace Library       { class Lock; class LockTracker;                      }
namespace Logging       {                                                     }
namespace MetaData      {                                                     }
namespace Measurement   { class StepTiming;                                   }
namespace Partitioning  { class TablePtn;                                     }
namespace Persistence   { class CarryOver;                                    }
namespace Plans         {                                                     }
namespace Pool          { class BufferPool; class HeapMem; class PoolBuffer;  }
namespace Prompt        { class SePrompt;                                     }
namespace PushDown      { class Op;                                           }
namespace Rows          { class CrwBuffer;                                    }
namespace ScanSprint    {                                                     }
namespace Scheduling    { class Queues; class Scheduler;                      }
namespace Sessions      { class Session; class SessionState;                  }
namespace Steps         { class Step;                                         }
namespace StorageEngine { class AM; class Engine;                             }
namespace Structures    {                                                     }
namespace System        {                                                     }
namespace Tasks         { class Task;                                         }
namespace Threads       { class Thread;                                       }
namespace Transactions  { class Tx; class TxSet;                              }
namespace Transform     {                                                     }
namespace Utilities     { class BPA;                                          }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

using namespace Alignment;
using namespace Assembler;
using namespace Base;
using namespace Chunks;
using namespace ClassData;
using namespace Commands;
using namespace ControlData;
using namespace Correctness;
using namespace Debugging;
using namespace Enums;
using namespace Forth;
using namespace Grids;
using namespace Indexes;
using namespace Interface;
using namespace Jobs;
using namespace Library;
using namespace Logging;
using namespace MetaData;
using namespace Measurement;
using namespace Partitioning;
using namespace Persistence;
using namespace Plans;
using namespace Pool;
using namespace Prompt;
using namespace PushDown;
using namespace Rows;
using namespace ScanSprint;
using namespace Scheduling;
using namespace Sessions;
using namespace Steps;
using namespace StorageEngine;
using namespace Structures;
using namespace System;
using namespace Tasks;
using namespace Threads;
using namespace Transactions;
using namespace Transform;
using namespace Utilities;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
