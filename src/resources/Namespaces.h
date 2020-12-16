/*============================================================================*/
/* Namespaces.h -- Namespaces and Forward Class declarations                  */
/*============================================================================*/
#pragma once

// resources

namespace Resources              {                                                 }
namespace Resources         {                                                 }
namespace Resources           {                                                 }
namespace Resources         {                                                 }
namespace Resources       { class ForthCell;            class Focal;
                              class Header;          class Object;
                              class Statistics;                               }
namespace Resources             { template <int BYTES> class String_t;
                              typedef String_t<32>     s32;
                              typedef String_t<256>    s256;
                              typedef String_t<C_2K>   s2K;   // note: 2K not 2KB
                              typedef String_t<C_4K>   s4K;   // note: 4K not 4KB
                              typedef String_t<C_16KB> s16KB; // note: this is 16KB
                              typedef String_t<C_24KB> s24KB; // note: this is 17KB
                              typedef String_t<C_48K>  s48K;  // note: 48K not 24KB
                              typedef String_t<C_1M>   s1M;   // note: 1M not 1MB

                              typedef String_t<C_MAX_STATEMENT> sSTMT;
                              typedef String_t<C_MAX_VCH>       sVCH;
  
                              template <int BYTES> class Bytes_t;
                              typedef Bytes_t<32>      b32;
                              typedef Bytes_t<256>     b256;
                              typedef Bytes_t<C_4K>    b4K;   // note: K, not KB
                              typedef Bytes_t<C_8K>    b8K;   // note: K, not KB
                              typedef Bytes_t<C_32K>   b32K;  // note: K, not KB
                              typedef Bytes_t<C_256K>  b256K; // note: K, not KB
                                                                              }
namespace Resources             {                                                 }
namespace Resources            { class ReqTuple;                                 }
namespace Resources           {                                                 }
namespace Resources              {                                                 }
namespace Resources           { class Lock;            class LockTracker;       }
namespace Resources         {                                                 }
namespace Resources      {                                                 }
namespace Resources              { class BufferPool;      class HeapBuffer;
                              class HeapMem;         class PoolBuffer;        }
namespace Resources         {                                                 }


using namespace Resources;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 

namespace Arguments         {                                                 }
namespace Chunks            { class Format;                                   }
namespace CompactBuffers    { class Crw;             class CrwBuffer;
                              class RowCompactor;    class RowInflator;
                              class HeadLine;
                              class OverLine;        class TailLine;          }
namespace CompilerOperators {                                                 }
namespace ControlData       {                                                 }
namespace DataBuffers       { class DataBuffer;      class CsvBuffer;
                              class RowBuffer;       class RowBuffer;         }
namespace Datums            { class Datum_h;
                              template <i8 SPACE> class Datum_t;              }
namespace DynamoCompiler    { class Dynamo;                                   }
namespace Entries           {                                                 }
namespace Expressions       { class Expr;            class ExprVec;
                              class LogicUnit;                                }
namespace Facts             { class Fact;            class Kbase;
                              class Method;        
                              class Question;        class Transform;         }

namespace Folders           {                                                 }
namespace Gadgets           { class EnumFlags;       class TagSet;            }
namespace Grids             { class Grid;            class Node;              }

namespace Indexing          { class EntBuffer;       class IndexTree;         }
namespace InfoData          { class InfoSetBase;                              }
namespace Interface         { class SessionArray;                             }
namespace Jobs              { class Job;             
                              class JobList;         class Job_CreateTable;
                              class Job_CreateIndex; class XmitOperand;
                              class Job_Distributed;                          }
namespace LexicalAnalysis   { class Token;           class Charser;
                              class Input;           class TokenVec;          }
namespace Load              {                                                 }
namespace Logging           { class Log;             class LogEntry;          }
namespace MetaData          { class Table;           class IdxColVec;
                              class Index;
                              class BucketMap;       class PtnMap;            }
namespace Metrics           {                                                 }
namespace Oblique           { class AM;                                       }
namespace Operands          { class Operand;                                  }
namespace Parser            { class PlanNode;        class PlanNodeBuilder;   }
namespace Partitioning      {
                              class PtnStream;       class TablePtn;          }
namespace Persistence       { class CarryOver;                                }
namespace Pipelines         { class Xmits;           class Op;
                              class Pipeline;        class Stage;
                              class StageVec;        class Strand;            }
namespace PipelineSpecs     { class OpSpec;          class PipelineSpec;
                              class StageSpec;                                }
namespace Plans             { class Planner;                                  }
namespace Prompt            {   class SePrompt;                               }
namespace ReadConsistency   { class Crv;                                      }
namespace ResourceUsage     { class RU_Cpu;          class RU_Memory;         }
namespace Rows              {                                                 }
namespace SenVM             { class Dic;             class DicStack;
                              class Forth;           class Word;              }
namespace Sessions          { class Session;         class SessionState;      }
namespace SortBuffers       {                                                 }
namespace SqlValues         {                                                 }
namespace Steps             { class Step;            class StepTiming;        }
namespace StorageEngine     { class AttachMap;       class Engine;            }
namespace Structures        { class Article;         class Queue;             }
namespace Tasks             { class Task;                                     }
namespace Threads           { class Scheduler;       class Thread;
                              class Queues;                                   }
namespace Transactions      { class Tx;              class TxSet;             }
namespace Utilities         { class BPA;             class PoolHelper;        }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

using namespace Arguments;
using namespace Chunks;
using namespace CompactBuffers;
using namespace CompilerOperators;
using namespace ControlData;
using namespace DataBuffers;
using namespace Datums;
using namespace DynamoCompiler;
using namespace Entries;
using namespace Expressions;
using namespace Facts;
using namespace Folders;
using namespace Gadgets;
using namespace Grids;
using namespace Indexing;
using namespace InfoData;
using namespace Interface;
using namespace Jobs;
using namespace LexicalAnalysis;
using namespace Load;
using namespace Logging;
using namespace MetaData;
using namespace Metrics;
using namespace Oblique;
using namespace Operands;
using namespace Parser;
using namespace Partitioning;
using namespace Persistence;
using namespace Pipelines;
using namespace PipelineSpecs;
using namespace Plans;
using namespace Prompt;
using namespace ReadConsistency;
using namespace ResourceUsage;
using namespace Rows;
using namespace SenVM;
using namespace Sessions;
using namespace SortBuffers;
using namespace SqlValues;
using namespace Steps;
using namespace StorageEngine;
using namespace Structures;
using namespace Tasks;
using namespace Threads;
using namespace Transactions;
using namespace Utilities;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
