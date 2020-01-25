/*============================================================================*/
/* ForwardClasses.h -- Forward Class declarations                             */
/*============================================================================*/
#pragma once

namespace Alignment     {                                                     }
namespace Arguments     {                                                     }
namespace Assembler     {                                                     }
namespace Base          {                                                     }
namespace Chunks        { class Format;                                       }
namespace ClassData     {                                                     }
namespace CompactBuffers{ class Crw; class CrwBuffer; class CsvBuffer;
                          class RowCompactor;
                          class HeadLine; class OverLine; class TailLine;     }
namespace CompilerOperators {                                                 }
namespace CompilerStream{                                                     }
namespace ControlData   {                                                     }
namespace Correctness   { class Cell; class Focal; class Header; class Object;
                          class Statistics;                                   }
namespace DataBuffers   {                                                     }
namespace DataOperators {                                                     }
namespace Datums        { class Datum_h;
                          template <i8 SPACE> class Datum_t;                  }
namespace Debugging     {                                                     }
namespace Entries       {                                                     }
namespace Enums         {                                                     }
namespace Esses         { template <int BYTES> class String_t;
                          typedef String_t<32>     s32;
                          typedef String_t<256>    s256;
                          typedef String_t<C_4K>   s4K;   // note: 4K not 4KB
                          typedef String_t<C_16KB> s16KB; // note: this is 16KB
                          typedef String_t<C_17KB> s17KB; // note: this is 17KB
                          typedef String_t<C_48K>  s48K;  // note: 48K not 48KB
                          typedef String_t<C_1M>   s1M;   // note: 1M not 1MB
                          typedef String_t<C_4K>   sVCH;
  
                          template <int BYTES> class Bytes_t;
                          typedef Bytes_t<32>      b32;
                          typedef Bytes_t<256>     b256;
                          typedef Bytes_t<C_4K>    b4K;   // note: 4K not 4KB
                                                                              }
namespace Focals        { class ReqTuple;                                     }
namespace Folders       {                                                     }
namespace Gadgets       { class EnumFlags;                                    }
namespace GoldenGate    {                                                     }
namespace Grids         { class Grid; class Node;                             }
namespace Host          {                                                     }
namespace IndexTrees    { class EntBuffer; class IndexTree;                   }
namespace Interface     { class SessionArray;                                 }
namespace Jobs          { class Job; struct JobClass; class JobList;
                          class Job_CreateTable; class Job_CreateIndex;
                          class Job_Operand; class Job_Universal;             }
namespace Library       {                                                     }
namespace Locking       { class Lock; class LockTracker;                      }
namespace Logging       { class Log; class LogEntry;                          }
namespace MetaData      { class Table; class IdxColVec; class Index;          }
namespace Partitioning  { class BucketMap;  class TablePtn;                   }
namespace Persistence   { class CarryOver;                                    }
namespace Pipelines     { class OpSpec; class StageSpec; class StageVec;      }
namespace Plans         {                                                     }
namespace Pool          { class BufferPool; class HeapBuffer; class HeapMem;
                          class PoolBuffer;                                   }
namespace Prompt        { class SePrompt;                                     }
namespace Oblique       { class AM;                                           }
namespace Operands      { class Operand;                                      }
namespace Operators     { class Op; class Pipeline;                           }
namespace ReadConsistency {                                                   }
namespace Records       {                                                     }
namespace Reflection    { class MetaClass; class MetaGraph;
                          class MemberSet; class MemberVar;                   }
namespace Resources     { class Resource; class ResourceTracker; class PsxThd;}
namespace Registers     { class RegSet;                                       }
namespace Rows          {                                                     }
namespace Scheduling    {                                                     }
namespace SenVM         { class Dic; class DicStack; class Forth; class Word; }
namespace Services      {                                                     }
namespace Sessions      { class Request; class Session; class SessionState;   }
namespace SharedMemory  {                                                     }
namespace SortBuffers   {                                                     }
namespace SqlValues     {                                                     }
namespace Steps         { class Step; class StepTiming;                       }
namespace StorageEngine { class AttachMap; class Engine;                      }
namespace Strings       {                                                     }
namespace Structures    { class Article; class Queue;                         }
namespace Tasks         { class Task;                                         }
namespace Threads       { class Scheduler;  class Thread; class Queues;       }
namespace TokenStream   { class Input; class Parser; class Tokeniser;
                          class TokenVec;                                     }
namespace Transactions  { class Tx; class TxSet;                              }
namespace Utilities     { class BPA; class PoolHelper;                        }
namespace Verbs         {                                                     }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

using namespace Alignment;
using namespace Arguments;
using namespace Assembler;
using namespace Base;
using namespace Chunks;
using namespace ClassData;
using namespace CompactBuffers;
using namespace CompilerOperators;
using namespace CompilerStream;
using namespace ControlData;
using namespace Correctness;
using namespace DataBuffers;
using namespace DataOperators;
using namespace Datums;
using namespace Debugging;
using namespace Pipelines;
using namespace Entries;
using namespace Enums;
using namespace Esses;
using namespace Focals;
using namespace Folders;
using namespace Gadgets;
using namespace GoldenGate;
using namespace Grids;
using namespace Host;
using namespace IndexTrees;
using namespace Interface;
using namespace Jobs;
using namespace Library;
using namespace Locking;
using namespace Logging;
using namespace MetaData;
using namespace Oblique;
using namespace Operands;
using namespace Operators;
using namespace Partitioning;
using namespace Persistence;
using namespace Plans;
using namespace Pool;
using namespace Prompt;
using namespace ReadConsistency;
using namespace Records;
using namespace Reflection;
using namespace Registers;
using namespace Resources;
using namespace Rows;
using namespace Scheduling;
using namespace SenVM;
using namespace Services;
using namespace Sessions;
using namespace SharedMemory;
using namespace SortBuffers;
using namespace SqlValues;
using namespace Steps;
using namespace StorageEngine;
using namespace Strings;
using namespace Structures;
using namespace Tasks;
using namespace Threads;
using namespace TokenStream;
using namespace Transactions;
using namespace Utilities;
using namespace Verbs;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
