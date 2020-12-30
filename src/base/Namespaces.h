/*============================================================================*/
/* Namespaces.h -- Namespaces and Forward Class declarations                  */
/*============================================================================*/
#pragma once

// resources





// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


namespace STORE    { class Crw;             class CrwBuffer;
                              class RowCompactor;    class RowInflator;
                              class HeadLine;        class Encoding_v2;
                     class SP_Encoding_v2;
                      class Packing_v2;
                              class OverLine;        class TailLine;          }
namespace STORE       { class DataBuffer;      class CsvBuffer;
                              class RowBuffer;       class RowBuffer;         }


namespace STORE    { class Dynamo;                                   }
namespace STORE       { class LogicUnit;                                }

namespace STORE             { class Grid;            class Node;              }

namespace STORE          { class EntBuffer;       class IndexTree;         }
namespace STORE          { class InfoSetBase;                              }
namespace STORE         { class SessionArray;                             }
namespace STORE              { class Job;
                              class JobList;         class Job_CreateTable;
                              class Job_CreateIndex; class XmitOperand;
                              class Job_Distributed;                          }
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
namespace STORE   { class Crv;                                      }
namespace STORE              { class Row;                                      }
namespace STORE             { class Dic;             class DicStack;
                              class Forth;           class Word;              }
namespace STORE          { class Session;         class SessionState;      }
namespace STORE             { class Step;            class StepTiming;        }
namespace STORE     { class AttachMap;       class Engine;            }


namespace STORE             { class Task;                                     }
namespace STORE           { class Scheduler;       class Thread;
                              class Queues;                                   }
namespace STORE      { class Tx;              class TxSet;             }


using namespace STORE;


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
