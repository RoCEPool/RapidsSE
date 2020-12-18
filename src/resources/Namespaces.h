/*============================================================================*/
/* Namespaces.h -- Namespaces and Forward Class declarations                  */
/*============================================================================*/
#pragma once

// resources




// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 

namespace Arguments         {                                                 }
namespace Chunks            { class Format;                                   }
namespace CompactBuffers    { class Crw;             class CrwBuffer;
                              class RowCompactor;    class RowInflator;
                              class HeadLine;        class Encoding_v2;
class SP_Encoding_v2;
class Packing_v2;
                              class OverLine;        class TailLine;          }
namespace CompilerOperators {                                                 }
namespace ControlData       {                                                 }
namespace DataBuffers       { class DataBuffer;      class CsvBuffer;
                              class RowBuffer;       class RowBuffer;         }
namespace Datums            { class Datum_h;
class SN_Bcd;
class SN_Bol;
class SN_Dte;
class SN_Flt;
class SN_Int;
class SN_Ivl;
class SN_Tme;
class SN_Tsp;

class SP_Bcd;
class SP_Bol;
class SP_Dte;
class SP_Flt;
class SP_Int;
class SP_Ivl;
class SP_Tme;
class SP_Tsp;
                              template <i8 SPACE> class Datum_t;              }
namespace Plans    { class Dynamo;                                   }
namespace Entries           {                                                 }
namespace Expressions       { class Expr;            class ExprVec;
                              class LogicUnit;                                }
namespace Facts             { class Fact;            class Kbase;
                              class Method;        
                              class Question;        class Transform;         }

namespace Folders           {                                                 }
namespace Resources           { class EnumFlags;       class TagSet;
class Birth; class Now;
}
namespace Grids             { class Grid;            class Node;              }

namespace Indexing          { class EntBuffer;       class IndexTree;         }
namespace InfoData          { class InfoSetBase;                              }
namespace Interface         { class SessionArray;                             }
namespace Plans              { class Job;             
                              class JobList;         class Job_CreateTable;
                              class Job_CreateIndex; class XmitOperand;
                              class Job_Distributed;                          }
namespace LexicalAnalysis   { class Token;           class Charser;
                              class Input;           class TokenVec;          }
namespace Load              {                                                 }
namespace StorageEngine           { class Log;             class LogEntry;          }
namespace MetaData          { class Table;           class IdxColVec;
                              class Index;
                              class BucketMap;       class PtnMap;            }
namespace Metrics           {                                                 }
namespace Oblique           { class AM;                                       }
namespace Chunks          { class Operand;                                  }
namespace Parser            { class PlanNode;        class PlanNodeBuilder;   }
namespace Partitioning      {
                              class PtnStream;       class TablePtn;          }
namespace Plans         { class Xmits;           class Op;
                              class Pipeline;        class Stage;
                              class StageVec;        class Strand;            }
namespace Plans     { class OpSpec;          class PipelineSpec;
                              class StageSpec;                                }
namespace Plans             { class Planner;                                  }
namespace Prompt            {   class SePrompt;                               }
namespace ReadConsistency   { class Crv;                                      }
namespace Resources     { class RU_Cpu;          class RU_Memory;         }
namespace Rows              { class Row;                                      }
namespace SenVM             { class Dic;             class DicStack;
                              class Forth;           class Word;              }
namespace Sessions          { class Session;         class SessionState;      }
namespace SortBuffers       {                                                 }
namespace SqlValues         {                                                 }
namespace Steps             { class Step;            class StepTiming;        }
namespace StorageEngine     { class AttachMap;       class Engine;            }
namespace Resources        { class Article;         class Queue;             }
namespace Tasking             { class Task;                                     }
namespace Tasking           { class Scheduler;       class Thread;
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
using namespace Plans;
using namespace Entries;
using namespace Expressions;
using namespace Facts;
using namespace Folders;
using namespace Resources;
using namespace Grids;
using namespace Indexing;
using namespace InfoData;
using namespace Interface;
using namespace Plans;
using namespace LexicalAnalysis;
using namespace Load;
using namespace StorageEngine;
using namespace MetaData;
using namespace Metrics;
using namespace Oblique;
using namespace Chunks;
using namespace Parser;
using namespace Partitioning;
using namespace Plans;
using namespace Plans;
using namespace Plans;
using namespace Prompt;
using namespace ReadConsistency;
using namespace Resources;
using namespace Rows;
using namespace SenVM;
using namespace Sessions;
using namespace SortBuffers;
using namespace SqlValues;
using namespace Steps;
using namespace StorageEngine;
using namespace Resources;
using namespace Tasking;
using namespace Tasking;
using namespace Transactions;
using namespace Utilities;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
