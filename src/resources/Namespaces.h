/*============================================================================*/
/* Namespaces.h -- Namespaces and Forward Class declarations                  */
/*============================================================================*/
#pragma once

// resources




// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 

namespace Arguments         {                                                 }
namespace Arguments            { class Format;                                   }
namespace Arguments    { class Crw;             class CrwBuffer;
                              class RowCompactor;    class RowInflator;
                              class HeadLine;        class Encoding_v2;
class SP_Encoding_v2;
class Packing_v2;
                              class OverLine;        class TailLine;          }
namespace CompilerOperators {                                                 }
namespace Arguments       {                                                 }
namespace Arguments       { class DataBuffer;      class CsvBuffer;
                              class RowBuffer;       class RowBuffer;         }
namespace Arguments            { class Datum_h;
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
namespace Arguments           {                                                 }
namespace Expressions       { class Expr;            class ExprVec;
                              class LogicUnit;                                }
namespace Facts             { class Fact;            class Kbase;
                              class Method;        
                              class Question;        class Transform;         }

namespace Arguments           {                                                 }
namespace Resources           { class EnumFlags;       class TagSet;
class Birth; class Now;
}
namespace Arguments             { class Grid;            class Node;              }

namespace Arguments          { class EntBuffer;       class IndexTree;         }
namespace Arguments          { class InfoSetBase;                              }
namespace Expressions         { class SessionArray;                             }
namespace Plans              { class Job;             
                              class JobList;         class Job_CreateTable;
                              class Job_CreateIndex; class XmitOperand;
                              class Job_Distributed;                          }
namespace Resources   { class Token;           class Charser;
                              class Input;           class TokenVec;          }
namespace Load              {                                                 }
namespace StorageEngine           { class Log;             class LogEntry;          }
namespace Arguments          { class Table;           class IdxColVec;
                              class Index;
                              class BucketMap;       class PtnMap;            }
namespace Expressions           {                                                 }
namespace Oblique           { class AM;                                       }
namespace Arguments          { class Operand;                                  }
namespace Parser            { //class PlanNode;        class PlanNodeBuilder;
                                                                              }
namespace Arguments      {
                              class PtnStream;       class TablePtn;          }
namespace Plans         { class Xmits;           class Op;
                              class Pipeline;        class Stage;
                              class StageVec;        class Strand;            }
namespace Plans     { class OpSpec;          class PipelineSpec;
                              class StageSpec;                                }
namespace Plans             { class Planner;                                  }
namespace Prompt            {   class SePrompt;                               }
namespace Arguments   { class Crv;                                      }
namespace Resources     { class RU_Cpu;          class RU_Memory;         }
namespace Arguments              { class Row;                                      }
namespace Arguments             { class Dic;             class DicStack;
                              class Forth;           class Word;              }
namespace Arguments          { class Session;         class SessionState;      }
namespace Arguments       {                                                 }
namespace SqlValues         {                                                 }
namespace Expressions             { class Step;            class StepTiming;        }
namespace StorageEngine     { class AttachMap;       class Engine;            }
namespace Resources        { class Article;         class Queue;
class Heap; class PoolHelper; class ObjectSnapshot;            }
namespace Expressions             { class Task;                                     }
namespace Expressions           { class Scheduler;       class Thread;
                              class Queues;                                   }
namespace Arguments      { class Tx;              class TxSet;             }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

using namespace Arguments;
using namespace Arguments;
using namespace Arguments;
using namespace CompilerOperators;
using namespace Arguments;
using namespace Arguments;
using namespace Arguments;
using namespace Plans;
using namespace Arguments;
using namespace Expressions;
using namespace Facts;
using namespace Arguments;
using namespace Resources;
using namespace Arguments;
using namespace Arguments;
using namespace Arguments;
using namespace Expressions;
using namespace Plans;
using namespace Resources;
using namespace Load;
using namespace StorageEngine;
using namespace Arguments;
using namespace Expressions;
using namespace Oblique;
using namespace Arguments;
using namespace Parser;
using namespace Arguments;
using namespace Plans;
using namespace Plans;
using namespace Plans;
using namespace Prompt;
using namespace Arguments;
using namespace Resources;
using namespace Arguments;
using namespace Arguments;
using namespace Arguments;
using namespace Arguments;
using namespace SqlValues;
using namespace Expressions;
using namespace StorageEngine;
using namespace Resources;
using namespace Expressions;
using namespace Expressions;
using namespace Arguments;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
