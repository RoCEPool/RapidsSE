/*============================================================================*/
/* Namespaces.h -- Namespaces and Forward Class declarations                  */
/*============================================================================*/
#pragma once

// resources




// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 

namespace BASE            { class Format;                                   }
namespace STORE    { class Crw;             class CrwBuffer;
                              class RowCompactor;    class RowInflator;
                              class HeadLine;        class Encoding_v2;
class SP_Encoding_v2;
class Packing_v2;
                              class OverLine;        class TailLine;          }
namespace STORE       { class DataBuffer;      class CsvBuffer;
                              class RowBuffer;       class RowBuffer;         }
namespace BASE {
class Datum_h;
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
template <i8 SPACE> class Datum_t;
}


namespace STORE    { class Dynamo;                                   }
namespace STORE       { class LogicUnit;                                }

namespace BASE           { class EnumFlags;       class TagSet;
class Birth; class Now;
}
namespace STORE             { class Grid;            class Node;              }

namespace STORE          { class EntBuffer;       class IndexTree;         }
namespace STORE          { class InfoSetBase;                              }
namespace STORE         { class SessionArray;                             }
namespace STORE              { class Job;
                              class JobList;         class Job_CreateTable;
                              class Job_CreateIndex; class XmitOperand;
                              class Job_Distributed;                          }
namespace BASE   { class Token;           class Charser;
                              class Input;           class TokenVec;          }
namespace STORE           { class Log;             class LogEntry;          }
namespace STORE          { class Table;           class IdxColVec;
                              class Index;
                              class BucketMap;       class PtnMap;            }
namespace STORE          { class Operand;                                  }
namespace STORE      {
                              class PtnStream;       class TablePtn;          }
namespace STORE         { class Xmits;           class Op;
                              class Pipeline;        class Stage;
                              class StageVec;        class Stream;            }
namespace STORE     { class OpSpec;          class PipelineSpec;
                              class StageSpec;                                }
namespace STORE             { class Planner;                                  }
namespace PROMPT            {   class SePrompt;                               }
namespace STORE   { class Crv;                                      }
namespace BASE     { class RU_Cpu;          class RU_Memory;         }
namespace STORE              { class Row;                                      }
namespace STORE             { class Dic;             class DicStack;
                              class Forth;           class Word;              }
namespace STORE          { class Session;         class SessionState;      }
namespace STORE             { class Step;            class StepTiming;        }
namespace STORE     { class AttachMap;       class Engine;            }
namespace BASE        { class Article;         class Queue;
class Heap; class PoolHelper; class ObjectSnapshot;            }
namespace STORE             { class Task;                                     }
namespace STORE           { class Scheduler;       class Thread;
                              class Queues;                                   }
namespace STORE      { class Tx;              class TxSet;             }


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

using namespace BASE;
using namespace STORE;
using namespace PROMPT;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
