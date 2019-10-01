/*============================================================================*/
/* ForwardClasses.h -- Forward Class declarations                             */
/*============================================================================*/
#pragma once

namespace Alignment     {                                                     }
namespace Assembler     {                                                     }
namespace Base          {                                                     }
namespace Chunks        { class Formatters;                                   }
namespace ClassData     {                                                     }
namespace Commands      { class Input; class Parser;
                          class Tokeniser; class TokenVec;                    }
namespace CompilerOperators {                                                 }
namespace CompilerStream{                                                     }
namespace ControlData   {                                                     }
namespace Correctness   {                                                     }
namespace DataOperators {                                                     }
namespace Debugging     {                                                     }
namespace Dynamics      { class OpSpec; class StageSpec;                      }
namespace Enums         {                                                     }
namespace Esses         { template <int BYTES> class String_t;
                          typedef String_t<32>     s32;
                          typedef String_t<256>    s256;
                          typedef String_t<C_4K>   s4K;   // note this is 4K not 4KB
                          typedef String_t<C_16KB> s16KB; // note this is 16KB
                          typedef String_t<C_17KB> s17KB; // note this is 17KB
                          typedef String_t<C_48K>  s48K;  // note this is 48K not 48KB
                          typedef String_t<C_1M>   s1M;   // note this is 1M not 1MB
                          typedef String_t<C_4K>   sVCH;
  
                          template <int BYTES> class Bytes_t;
                          typedef Bytes_t<32>      b32;
                          typedef Bytes_t<256>     b256;
                          typedef Bytes_t<C_4K>    b4K;   // note this is 4K not 4KB
                                                                              }
namespace Focals        {                                                     }
namespace Forth         { class Cell; class Dic; class ForthWord;             }
namespace Gadgets       { class EnumFlags;                                    }
namespace Grids         { class Grid; class Node;                             }
namespace Host          {                                                     }
namespace Indexes       { class IndexTree;                                    }
namespace Interface     { class ApiState;                                     }
namespace Jobs          { class Job; struct JobClass;  class JobList;
                                     class Job_CreateTable;
                                     class Job_CreateIndex;
                                     class Job_Dynamic;
                                     class Job_Xmit;                          }
namespace Library       {                                                     }
namespace Locking       { class Lock; class LockTracker;                      }
namespace Logging       { class Log; class LogEntry;                          }
namespace MetaData      { class Table; class Index;                           }
namespace Partitioning  { class BucketMap;  class TablePtn;                   }
namespace Persistence   { class CarryOver;                                    }
namespace Plans         {                                                     }
namespace Pool          { class BufferPool; class HeapMem; class PoolBuffer;  }
namespace Prompt        { class SePrompt;                                     }
namespace Oblique       {                                                     }
namespace Operators     { class Op; class Pipeline;                             }
namespace Resources     { class Resource; class ResourceTracker;
                          class Pthread;                                      }
namespace Rows          { class CrwBuffer;                                    }
namespace ScanSprint    {                                                     }
namespace Scheduling    { class Queues; class Scheduler;                      }
namespace Services      {                                                     }
namespace Sessions      { class Request; class Session; class SessionState;   }
namespace Steps         { class Step; class StepTiming;                       }
namespace StorageEngine { class AttachMap; class AM; class Engine;            }
namespace Streams       { class Operand;                                      }
namespace Strings       {                                                     }
namespace Structures    { class Queue;                                        }
namespace Tasks         { class Task;                                         }
namespace Threads       { class Thread;                                       }
namespace Transactions  { class Tx; class TxSet;                              }
namespace Transform     {                                                     }
namespace Utilities     { class BPA;                                          }
namespace Verbs         {                                                     }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

using namespace Alignment;
using namespace Assembler;
using namespace Base;
using namespace Chunks;
using namespace ClassData;
using namespace Commands;
using namespace CompilerOperators;
using namespace CompilerStream;
using namespace ControlData;
using namespace Correctness;
using namespace DataOperators;
using namespace Debugging;
using namespace Dynamics;
using namespace Enums;
using namespace Esses;
using namespace Focals;
using namespace Forth;
using namespace Gadgets;
using namespace Grids;
using namespace Host;
using namespace Indexes;
using namespace Interface;
using namespace Jobs;
using namespace Library;
using namespace Locking;
using namespace Logging;
using namespace MetaData;
using namespace Oblique;
using namespace Operators;
using namespace Partitioning;
using namespace Persistence;
using namespace Plans;
using namespace Pool;
using namespace Prompt;
using namespace Resources;
using namespace Rows;
using namespace ScanSprint;
using namespace Scheduling;
using namespace Services;
using namespace Sessions;
using namespace Steps;
using namespace StorageEngine;
using namespace Streams;
using namespace Strings;
using namespace Structures;
using namespace Tasks;
using namespace Threads;
using namespace Transactions;
using namespace Transform;
using namespace Utilities;
using namespace Verbs;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
