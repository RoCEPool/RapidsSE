/*============================================================================*/
/* Namespaces.h -- Namespaces and Forward Class declarations                  */
/*============================================================================*/
#pragma once

namespace Alignment         {                                                 }
namespace Arguments         {                                                 }
namespace Assembler         {                                                 }
namespace Base              {                                                 }
namespace Chunks            { class Format;                                   }
namespace ClassData         {                                                 }
namespace CompactBuffers    { class Crw;             class CrwBuffer;
                              class RowCompactor;    class RowInflator;
                              class HeadLine;
                              class OverLine;        class TailLine;          }
namespace CompilerOperators {                                                 }
namespace ControlData       {                                                 }
namespace Correctness       { class Cell;            class Focal;
                              class Header;          class Object;
                              class Statistics;                               }
namespace DataBuffers       { class DataBuffer;      class CsvBuffer;
                              class RsbBuffer;                                }
namespace DataOperators     {                                                 }
namespace Datums            { class Datum_h;
                              template <i8 SPACE> class Datum_t;              }
namespace Debugging         {                                                 }
namespace DynamoCompiler    { class Dynamo;                                   }
namespace Entries           {                                                 }
namespace Enums             {                                                 }
namespace Esses             { template <int BYTES> class String_t;
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
namespace Expressions       { class AggregateRsb;    class Expr;
                              class ExprVec;         class FilterRsb;
                              class JoinRsb;         class ProjectRsb; 
                              class LogicUnit;                                }
namespace Facts             { class Fact;                                     }
namespace Focals            { class ReqTuple;                                 }
namespace Folders           {                                                 }
namespace FrontEnd          { class Clause_Project;  class Clause_From;
                              class Clause_Where;    class Clause_GroupBy;
                              class Clause_OrderBy;  class Clause_Limit;
                              class Stmt_Select;     class Stmt_SubQry;
                              class Descend;                                  }
namespace Gadgets           { class EnumFlags;       class TagSet;            }
namespace GoldenGate        {                                                 }
namespace Grids             { class Grid;            class Node;              }
namespace Host              {                                                 }
namespace Indexing          { class EntBuffer;       class IndexTree;         }
namespace InfoData          {                                                 }
namespace Interface         { class SessionArray;                             }
namespace Jobs              { class Job;             
                              class JobList;         class Job_CreateTable;
                              class Job_CreateIndex; class XmitOperand;
                              class Job_Distributed;                          }
namespace LexicalAnalysis   { class Token;           class Charser;
                              class Input;           class TokenVec;          }
namespace Library           {                                                 }
namespace Load              {                                                 }
namespace Locking           { class Lock;            class LockTracker;       }
namespace Logging           { class Log;             class LogEntry;          }
namespace LogicalPlans      {                                                 }
namespace MiddleEnd         {                                                 }
namespace MetaData          { class Table;           class IdxColVec;
                              class Index;                                    }
namespace Metrics           {                                                 }
namespace Oblique           { class AM;                                       }
namespace Operands          { class Operand;                                  }
namespace Partitioning      { class BucketMap;       class PtnMap;
                              class PtnStream;       class TablePtn;          }
namespace Persistence       { class CarryOver;                                }
namespace Pipelines         { class Xmits;           class Op;
                              class Pipeline;        class Stage;
                              class StageVec;                                 }
namespace PipelineSpecs     { class OpSpec;          class PipelineSpec;
                              class StageSpec;                                }
namespace Plans             { class Planner;                                  }
namespace Pool              { class BufferPool;      class HeapBuffer;
                              class HeapMem;         class PoolBuffer;        }
namespace Prompt            {   class SePrompt;                               }
namespace ReadConsistency   { class Crv;                                      }
namespace Records           {                                                 }
namespace Reflection        { class ConstDef;        class ConstSet;
                              class MetaBlock;       class MetaCell;
                              class MetaGraph;       class MetaInherit;
                              class MetaInherits;    class MetaMember;
                              class MetaType;                                 }
namespace ResourceUsage     { class RU_Cpu;          class RU_Memory;         }
namespace Resources         { class File;            class Resource;
                              class ResourceTracker; class PsxThd;            }
namespace Rows              {                                                 }
namespace SenVM             { class Dic;             class DicStack;
                              class Forth;           class Word;              }
namespace Sessions          { class Session;         class SessionState;      }
namespace SharedMemory      {                                                 }
namespace SortBuffers       {                                                 }
namespace SqlParsing        {                                                 }
namespace SqlValues         {                                                 }
namespace Steps             { class Step;            class StepTiming;        }
namespace StorageEngine     { class AttachMap;       class Engine;            }
namespace Strings           {                                                 }
namespace Structures        { class Article;         class Queue;
                              class Text;                                     }
namespace Tasks             { class Task;                                     }
namespace Threads           { class Scheduler;       class Thread;
                              class Queues;                                   }
namespace Transactions      { class Tx;              class TxSet;             }
namespace Utilities         { class BPA;             class PoolHelper;        }
namespace Verbs             {                                                 }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

using namespace Alignment;
using namespace Arguments;
using namespace Assembler;
using namespace Base;
using namespace Chunks;
using namespace ClassData;
using namespace CompactBuffers;
using namespace CompilerOperators;
using namespace ControlData;
using namespace Correctness;
using namespace DataBuffers;
using namespace DataOperators;
using namespace Datums;
using namespace Debugging;
using namespace DynamoCompiler;
using namespace Entries;
using namespace Enums;
using namespace Esses;
using namespace Expressions;
using namespace Facts;
using namespace Focals;
using namespace Folders;
using namespace FrontEnd;
using namespace Gadgets;
using namespace GoldenGate;
using namespace Grids;
using namespace Host;
using namespace Indexing;
using namespace InfoData;
using namespace Interface;
using namespace Jobs;
using namespace LexicalAnalysis;
using namespace Library;
using namespace Load;
using namespace Locking;
using namespace Logging;
using namespace LogicalPlans;
using namespace MiddleEnd;
using namespace MetaData;
using namespace Metrics;
using namespace Oblique;
using namespace Operands;
using namespace Partitioning;
using namespace Persistence;
using namespace Pipelines;
using namespace PipelineSpecs;
using namespace Plans;
using namespace Pool;
using namespace Prompt;
using namespace ReadConsistency;
using namespace Records;
using namespace Reflection;
using namespace ResourceUsage;
using namespace Resources;
using namespace Rows;
using namespace SenVM;
using namespace Sessions;
using namespace SharedMemory;
using namespace SortBuffers;
using namespace SqlParsing;
using namespace SqlValues;
using namespace Steps;
using namespace StorageEngine;
using namespace Strings;
using namespace Structures;
using namespace Tasks;
using namespace Threads;
using namespace Transactions;
using namespace Utilities;
using namespace Verbs;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
