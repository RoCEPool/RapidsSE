/*============================================================================*/
/* SizesHeaps.cpp -- Class Sizes and Heaps                                    */
/*============================================================================*/

#include "BASE.h"

#include "eBO.h"
#include "eCC.h"
#include "eCP.h"
#include "eCX.h"
#include "eDL.h"
#include "eDS.h"
#include "eED.h"
#include "eFN.h"
#include "eIK.h"
#include "eJF.h"
#include "eLG.h"
#include "eMM.h"
#include "eMT.h"
#include "eNE.h"
#include "eOK.h"
#include "eOR.h"
#include "ePF.h"
#include "eYG.h"
#include "ePY.h"
#include "eYX.h"
#include "eRG.h"
#include "eRO.h"
#include "eRU.h"
#include "eST.h"
#include "eTP.h"
#include "eTT.h"
#include "eUK.h"
#include "eXK.h"

#include "SizesHeaps.hpp"

#include "backend/Backend.hpp"

#include "CmdLineArgs.hpp"

#include "DdlLock.hpp"
#include "DicLock.hpp"

#include "EachRowBuffer.hpp"

#include "IndexDef.hpp"

#include "Index.hpp"

#include "Folder.hpp"
#include "Schema.hpp"
#include "Catalog.hpp"
#include "Tenant.hpp"
#include "Tenants.hpp"
#include "Marker.hpp"
#include "Record.hpp"
#include "RowBuffer.hpp"
#include "LglBufSet.hpp"
#include "Pot.hpp"

#include "InfoSet.hpp"
#include "NamedBuffer.hpp"
#include "StreamSet.hpp"
#include "RowCoder.hpp"

#include "HeadLine.hpp"
#include "Crw.hpp"

#include "Encoding.hpp"

#include "LglPtnMap.hpp"


#include "BucketMap.hpp"
#include "BufferMap.hpp"

#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "RowCompactor.hpp"
#include "OtherOperand.hpp"
#include "PlanOperand.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "EntBufSorter.hpp"
#include "IndexTree.hpp"

#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "CrwTablePtn.hpp"
#include "Bucket.hpp"
#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "PtnStm_cursor.hpp"
#include "PtnStream.hpp"
#include "Table.hpp"

#include "Node.hpp"
#include "Node_I.hpp"
#include "Grid.hpp"
#include "Arg.hpp"
#include "RelOp.hpp"
#include "Predicate.hpp"
#include "PredicateVec.hpp"

#include "AccessPath.hpp"
#include "Api.hpp"
#include "AttachMap.hpp"

#include "Exemplar.hpp"

#include "Combined.hpp"
#include "InternalTables.hpp"

#include "Job.hpp"
#include "JobList.hpp"
#include "JobReport.hpp"
#include "Jobs.hpp"

#include "Lit.hpp"
#include "LitRecogniser.hpp"
#include "Locator.hpp"
#include "LogicUnit.hpp"

#include "Metadata.hpp"
#include "MetaGet.hpp"
#include "BucketGet.hpp"
#include "BucketInfo.hpp"

#include "Parcel.hpp"
#include "Numbering.hpp"
#include "Output.hpp"
#include "Op.hpp"
#include "OpSpec.hpp"

#include "PathIdentifier.hpp"
#include "VertexSpec.hpp"
#include "Pipeline.hpp"
#include "PlannerLock.hpp"

#include "Dynamo.hpp"

#include "Procedure.hpp"

#include "ReapLock.hpp"
#include "Dissect.hpp"
#include "Weave.hpp"
#include "Ribbon.hpp"
#include "RibbonLine.hpp"
#include "RowTablePtn.hpp"
#include "Optimiser.hpp"

#include "Card.hpp"

#include "Board.hpp"
#include "Bench.hpp"

#include "Scanner.hpp"

#include "SeApi.hpp"

#include "PtnDef.hpp"
#include "ShortCut.hpp"
#include "SeDeputy.hpp"
#include "SePrompt.hpp"
#include "Shape.hpp"
#include "Stage.hpp"
#include "StepAccum.hpp"
#include "StageSpec.hpp"
#include "Starter.hpp"
#include "Statement.hpp"
#include "Stencil.hpp"
#include "Steps.hpp"
#include "Stream.hpp"
#include "StreamDefRht.hpp"
#include "StzList.hpp"

#include "SP_Gid.hpp"
#include "SP_Elapsed.hpp"
#include "SP_String.hpp"

#include "SN_Bcd.hpp"
#include "SN_Bol.hpp"
#include "SN_Dte.hpp"
#include "SN_Flt.hpp"
#include "SN_Int.hpp"
#include "SN_Ivl.hpp"
#include "SN_Tme.hpp"
#include "SN_Tsp.hpp"
#include "SN_Vch.hpp"

#include "TccLock.hpp"
#include "ToLglPtnCrw.hpp"
#include "ToLglPtnRow.hpp"
#include "TraceLock.hpp"
#include "TraceMultiLock.hpp"
#include "Tub.hpp"

#include "UnitInt.hpp"
#include "UT_SkipList.hpp"

#include "Parcel.hpp"
#include "Sends.hpp"

#include "CodeSynth.hpp"
#include "Flatten.hpp"
#include "Permit.hpp"
#include "Totup.hpp"
#include "Socket.hpp"
#include "OperandWatch.hpp"
#include "EventHandlers.hpp"
#include "Locks.hpp"
#include "Balloon.hpp"
#include "JobMetrics.hpp"
#include "PipelineMetrics.hpp"
#include "StageMetrics.hpp"
#include "StreamMetrics.hpp"
#include "OpMetrics.hpp"
#include "SysReport.hpp"
#include "SysMetrics.hpp"
#include "DerefMetrics.hpp"
#include "ThreadScratch.hpp"
#include "PagingMetrics.hpp"
#include "Action.hpp"
#include "CardCtl.hpp"
#include "CardRegs.hpp"
#include "RuleCtl.hpp"
#include "Drafter.hpp"
#include "Vertex.hpp"
#include "SubCall.hpp"
#include "SubReturn.hpp"
#include "Deck.hpp"
#include "DeckSpec.hpp"
#include "AdaptorWL.hpp"
#include "PipeIn.hpp"
#include "EvMgr.hpp"

#include "graph/PlanConstants.hpp"
#include "graph/PlanNode.hpp"
#include "graph/PlanNodeBuilder.hpp"
#include "graph/RootNode.hpp"
#include "graph/DefinitionNode.hpp"
#include "graph/PrimitiveNode.hpp"
#include "graph/QueryNode.hpp"
#include "graph/PlanDefinitionParser.hpp"
#include "graph/ConditionalQueryNodeIter.hpp"

#include "graphTools/GraphNav.hpp"
#include "graphTools/GraphEditor.hpp"

#include "parser/stages/parser/SqlPlanner.hpp"
using namespace Parser;

/*----------------------------------------------------------------------------*/
/* sizes & heaps                                                              */
/*----------------------------------------------------------------------------*/
#define FILL ,0,0,0,0,0,0

SizeHeap gxaSizeHeap[]
{
  { CN_NONE,                     0,                          HT_INVALID   ,0 FILL },

  { CN__A__,                     'A' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_ACCESSPATH,               sizeof(AccessPath),         HT_T_MEDIUM  ,0 FILL },
  { CN_ACTION,                   sizeof(Action),             HT_T_TINY    ,0 FILL },
  { CN_ADAPTORWL,                sizeof(AdaptorWL),          HT_T_TINY    ,0 FILL },
  { CN_AGGTIMER,                 sizeof(AggTimer),           HT_T_TINY    ,0 FILL },
  { CN_AM,                       0,                          HT_INVALID   ,0 FILL },
  { CN_API,                      sizeof(Api),                HT_INVALID   ,0 FILL },
  { CN_ANSWERWL,                 sizeof(AnswerWL),           HT_T_TINY    ,0 FILL },
  { CN_ARG,                      sizeof(Arg),                HT_T_TINY    ,0 FILL },
  { CN_ARRANGE,                  sizeof(Arrange),            HT_T_TINY    ,0 FILL },
  { CN_ATOMIC,                   sizeof(Atomic),             HT_INVALID   ,0 FILL },
  { CN_ATTACHMAP,                sizeof(AttachMap),          HT_INVALID   ,0 FILL },

  { CN__B__,                     'B' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_BACKEND,                  sizeof(Backend),/*yes*/     HT_T_MEDIUM  ,0 FILL },
  { CN_BALLOON,                  sizeof(Balloon),            HT_T_TINY    ,0 FILL },
  { CN_BCDVAL,                   sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 FILL },
  { CN_BENCH,                    sizeof(Bench),              HT_T_SMALL   ,0 FILL },
  { CN_BIRTH,                    sizeof(Birth),              HT_INVALID   ,0 FILL },
  { CN_BOARD,                    sizeof(Board),              HT_T_SMALL   ,0 FILL },
  { CN_BOLVAL,                   sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 FILL },
  { CN_BUCKET,                   sizeof(Bucket),             HT_T_TINY    ,0 FILL },
  { CN_BUCKETGET,                sizeof(BucketGet),          HT_T_TINY    ,0 FILL },
  { CN_BUCKETINFO,               sizeof(BucketInfo),         HT_T_TINY    ,0 FILL },
  { CN_BUCKETMAP,                sizeof(BucketMap),          HT_T_SMALL   ,0 FILL },
  { CN_BKTMAPGET,                sizeof(BktMapGet),          HT_T_TINY    ,0 FILL },
  { CN_BUCKETVEC,                sizeof(BucketVec),          HT_T_TINY    ,0 FILL },
  { CN_BUFFERMAP,                sizeof(BufferMap),          HT_INVALID   ,0 FILL },
  { CN_BUFFERPOOL,               sizeof(BufferPool),         HT_INVALID   ,0 FILL },
  { CN_BYTERATE,                 sizeof(ByteRate),           HT_T_TINY    ,0 FILL },
  { CN_BYTES_T,                  sizeof(s32),                HT_INVALID   ,0 FILL },

  { CN__C__,                     'C' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_CARD,                     sizeof(Card),               HT_T_SMALL   ,0 FILL },
  { CN_CARDCTL,                  sizeof(CardCtl),            HT_T_SMALL   ,0 FILL },
  { CN_CARDREGS,                 sizeof(CardRegs),           HT_T_SMALL   ,0 FILL },
  { CN_CATALOG,                  sizeof(Catalog),            HT_INVALID   ,0 FILL },
  { CN_CELLSINGLE,               sizeof(CellSingle),         HT_INVALID   ,0 FILL },
  { CN_CHARSER,                  sizeof(Charser),            HT_T_LARGE   ,0 FILL },
  { CN_CLASSPLACEHOLDER,         sizeof(ClassPlaceHolder),   HT_T_LARGE   ,0 FILL },   
  { CN_CMDLINEARGS,              sizeof(CmdLineArgs),        HT_INVALID   ,0 FILL },
  { CN_CODEBLOCK,                sizeof(CodeBlock),          HT_T_SMALL   ,0 FILL },
  { CN_CODESYNTH,                sizeof(CodeSynth),          HT_T_LARGE   ,0 FILL },
  { CN_COLUMN,                   sizeof(Column),             HT_T_TINY    ,0 FILL },
  { CN_COMBINED,                 sizeof(Combined),           HT_T_LARGE   ,0 FILL },
  { CN_CONDITIONALQUERYNODEITER, sizeof(ConditionalQueryNodeIter), HT_T_TINY,0 FILL },
  { CN_CRVBUFFER,                sizeof(CrvBuffer),          HT_T_LARGE   ,0 FILL },
  { CN_CRWBUFFER,                sizeof(CrwBuffer),          HT_T_LARGE   ,0 FILL },
  { CN_CRWTABLEPTN,              sizeof(CrwTablePtn),        HT_T_TINY    ,0 FILL },
  { CN_CRWTOROW,                 sizeof(CrwToRow),           HT_T_TINY    ,0 FILL },
  { CN_CSVBUFFER,                sizeof(CsvBuffer),          HT_T_LARGE   ,0 FILL },
  { CN_CSVMETA,                  sizeof(CsvMeta),            HT_T_TINY    ,0 FILL },

  { CN__D__,                     'D' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_DATATRANSIENT,            sizeof(Heap), /*correct*/   HT_INVALID   ,0 FILL },
  { CN_DDLLOCK,                  sizeof(DdlLock),            HT_T_TINY    ,0 FILL },
  { CN_DECACNT,                  sizeof(DecaCnt),            HT_T_TINY    ,0 FILL },
  { CN_DECIDER,                  sizeof(Decider),            HT_T_TINY    ,0 FILL },
  { CN_DECK,                     sizeof(Deck),               HT_T_SMALL   ,0 FILL },
  { CN_DECKSPEC,                 sizeof(DeckSpec),           HT_T_TINY    ,0 FILL },
  { CN_DEFINITIONNODE,           sizeof(DefinitionNode),     HT_T_TINY    ,-8 FILL },
  { CN_DEREFMETRICS,             sizeof(DerefMetrics),       HT_T_TINY    ,0 FILL },
  { CN_DIC,                      sizeof(Dic),                HT_T_TINY    ,0 FILL },
  { CN_DICLOCK,                  sizeof(DicLock),            HT_T_TINY    ,0 FILL },
  { CN_DICSTACK,                 sizeof(DicStack),           HT_T_TINY    ,0 FILL },
  { CN_DIR,                      sizeof(Dir),                HT_T_TINY    ,0 FILL },
  { CN_DISSECT,                  sizeof(Dissect),            HT_T_TINY    ,0 FILL },
  { CN_DRAFTER,                  sizeof(Drafter),            HT_T_TINY    ,0 FILL },
  { CN_DTEVAL,                   sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 FILL },
  { CN_DYNAMO,                   sizeof(Dynamo),             HT_T_TINY    ,0 FILL },

  { CN__E__,                     'E' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_EACHROWBUFFER,            sizeof(EachRowBuffer),      HT_T_MEDIUM  ,0 FILL },
  { CN_EDTVAL,                   sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 FILL },
  { CN_ENCODING,                 sizeof(Encoding),           HT_T_MEDIUM  ,0 FILL },
  { CN_ENT,                      sizeof(Ent),                HT_INVALID   ,0 FILL },
  { CN_ENTBUFFER,                sizeof(EntBuffer),          HT_T_LARGE   ,0 FILL },
  { CN_ENTBUFSORTER,             sizeof(EntBufSorter),       HT_T_LARGE   ,0 FILL },
  { CN_ENTWRK,                   sizeof(EntWrk),             HT_INVALID   ,0 FILL },
  { CN_ENUMFLAGS,                sizeof(ClassFlags),         HT_T_TINY    ,0 FILL },
  { CN_EVENTHANDLERS,            sizeof(EventHandlers),      HT_T_TINY    ,0 FILL },
  { CN_EVMGR,                    sizeof(EvMgr),              HT_T_TINY    ,0 FILL },
  { CN_EXEMPLAR,                 sizeof(Exemplar),           HT_T_TINY    ,0 FILL },

  { CN__F__,                     'F' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_FIFO,                     sizeof(Fifo),               HT_T_TINY    ,0 FILL },
  { CN_FILE,                     sizeof(File),               HT_T_TINY    ,0 FILL },
  { CN_FILEMAP,                  sizeof(FileMap),            HT_INVALID   ,0 FILL },
  { CN_FIPVAL,                   sizeof(TypedCell), /*yes*/  HT_INVALID   ,0 FILL },
  { CN_FLATTEN,                  sizeof(Flatten), /*yes*/    HT_T_TINY    ,0 FILL },
  { CN_FLTVAL,                   sizeof(TypedCell), /*yes*/  HT_INVALID   ,0 FILL },
  { CN_FORMAT,                   sizeof(Format),             HT_T_TINY    ,0 FILL },
  { CN_FREEPOOLBUFFER,           sizeof(PoolBuffer),/*yes*/  HT_INVALID   ,0 FILL },
  { CN_FREECHUNK,                sizeof(FreeChunk),          HT_INVALID   ,0 FILL },
  { CN_FREEDNOT,                 sizeof(FreedNot),           HT_INVALID   ,0 FILL },
  { CN_FREEITEM,                 0,                          HT_INVALID   ,0 FILL },
  { CN_FREQDIST,                 sizeof(FreqDist),           HT_T_SMALL   ,0 FILL },

  { CN__G__,                     'G' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_GRID,                     sizeof(Grid),               HT_T_TINY    ,0 FILL },

  { CN__H__,                     'H' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_HASHMAP,                  sizeof(HashMap),            HT_T_LARGE   ,0 FILL },
  { CN_HASHMAPSLOT,              sizeof(HashMapSlot),        HT_T_TINY    ,0 FILL },
  { CN_HEAPBUFFER,               sizeof(HeapBuffer),         HT_INVALID   ,0 FILL },
  { CN_HEAPMEM,                  sizeof(Heaps),              HT_INVALID   ,0 FILL },

  { CN__I__,                     'I' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_INDEX,                    sizeof(Index),              HT_T_TINY    ,0 FILL },
  { CN_INDEXDEF,                 sizeof(IndexDef),           HT_T_SMALL   ,0 FILL },
  { CN_INDEXSTREAM,              sizeof(IndexStream),        HT_T_TINY    ,0 FILL },
  { CN_INDEXTREE,                sizeof(IndexTree),          HT_T_TINY    ,0 FILL },
  { CN_INDEXTREEVEC,             sizeof(IndexTreeVec),       HT_T_TINY    ,0 FILL },
  { CN_IDXVEC,                   sizeof(IdxVec),             HT_T_TINY    ,0 FILL },
  { CN_INFOSET,                  sizeof(InfoSet),            HT_T_LARGE   ,0 FILL },
  { CN_INGESTCHUNK,              sizeof(IngestChunk),        HT_T_LARGE   ,0 FILL },
  { CN_INPUT,                    sizeof(Input),              HT_T_TINY    ,0 FILL },
  { CN_INTERNALTABLES,           sizeof(InternalTables),     HT_T_TINY    ,0 FILL },
  { CN_INTSET,                   sizeof(IntSet),             HT_T_TINY    ,0 FILL },
  { CN_INTSTACK,                 sizeof(IntStack),           HT_T_TINY    ,0 FILL },
  { CN_INTVAL,                   sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 FILL },
  { CN_IOCHUNK,                  sizeof(IoChunk),            HT_T_LARGE   ,0 FILL },
  { CN_IOCOPY,                   sizeof(IoCopy),             HT_T_TINY    ,0 FILL },
  { CN_IVLVAL,                   sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 FILL },

  { CN__J__,                     'J' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_JOB,                      sizeof(Job),                HT_T_MEDIUM  ,0 FILL },
  { CN_JOBLIST,                  sizeof(JobList),            HT_T_TINY    ,0 FILL },
  { CN_JOBMETRICS,               sizeof(JobMetrics),         HT_T_LARGE   ,0 FILL },
  { CN_JOBREPORT,                sizeof(JobReport),          HT_T_TINY    ,0 FILL },
  { CN_JOBS,                     sizeof(Jobs),               HT_T_TINY    ,0 FILL },

  { CN__K__,                     'K' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_KEY,                      sizeof(Key),                HT_INVALID   ,0 FILL },

  { CN__L__,                     'L' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_LARGEPERSISTENT,          sizeof(Heap), /*correct*/   HT_INVALID   ,0 FILL },
  { CN_LARGETRANSIENT,           sizeof(Heap), /*correct*/   HT_INVALID   ,0 FILL },
  { CN_LARGEVIRTUAL,             sizeof(Heap), /*correct*/   HT_INVALID   ,0 FILL },
  { CN_LGLBUFSET,                sizeof(LglBufSet),          HT_T_LARGE   ,0 FILL },
  { CN_LGLPTNMAP,                sizeof(LglPtnMap),          HT_T_LARGE   ,0 FILL },
  { CN_LICENCE,                  sizeof(Licence),            HT_T_TINY    ,0 FILL },
  { CN_LIMITS,                   sizeof(Limits),             HT_T_TINY    ,0 FILL },
  { CN_LISTEXONODE_t,            sizeof(ListExoNode_t<Object>), HT_T_TINY ,0 FILL },
  { CN_LISTEXOITER_t,            sizeof(ListExoIter_t<Object>), HT_T_TINY ,0 FILL },
  { CN_LIT,                      sizeof(Lit),                HT_T_TINY    ,0 FILL },
  { CN_LITRECOGNISER,            sizeof(LitRecogniser),      HT_T_TINY    ,0 FILL },
  { CN_LOADCHUNK,                sizeof(LoadChunk),          HT_T_LARGE   ,0 FILL },
  { CN_LOCAL,                    sizeof(Local),              HT_INVALID   ,0 FILL },
  { CN_LOCALCHAIN,               sizeof(LocalChain),         HT_T_LARGE   ,0 FILL },
  { CN_LOCALDIR,                 sizeof(LocalDir),           HT_INVALID   ,0 FILL },
  { CN_LOCALFILE,                sizeof(LocalFile),          HT_INVALID   ,0 FILL },
  { CN_LOCATION,                 sizeof(BASE::Location),     HT_T_TINY    ,0 FILL },
  { CN_LOCATOR,                  sizeof(Locator),            HT_T_TINY    ,0 FILL },
  { CN_LOCK,                     sizeof(Lock),               HT_INVALID   ,0 FILL },
  { CN_LOCKS,                    sizeof(Locks),              HT_T_MEDIUM  ,0 FILL },
  { CN_LOCKSET,                  0,                          HT_INVALID   ,0 FILL },
  { CN_LOGICUNIT,                sizeof(LogicUnit),          HT_T_MEDIUM  ,0 FILL },

  { CN__M__,                     'M' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_MARKER,                   sizeof(Marker),             HT_T_TINY    ,0 FILL },
  { CN_MEDIUMPERSISTENT,         sizeof(Heap), /*correct*/   HT_INVALID   ,0 FILL },
  { CN_MEDIUMTRANSIENT,          sizeof(Heap), /*correct*/   HT_INVALID   ,0 FILL },
  { CN_MEDIUMVIRTUAL,            sizeof(Heap), /*correct*/   HT_INVALID   ,0 FILL },
  { CN_MEMTEST,                  sizeof(MemTest),            HT_T_TINY    ,0 FILL },
  { CN_METADATA,                 sizeof(Metadata),           HT_T_TINY    ,0 FILL },
  { CN_METAGET,                  sizeof(MetaGet),            HT_T_TINY    ,0 FILL },
  { CN_METRICSWSP,               sizeof(MetricsWsp),         HT_INVALID   ,0 FILL },
  { CN_MUTEX,                    sizeof(Mutex),              HT_T_TINY    ,0 FILL },

  { CN__N__,                     'N' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_NAMEDBUFFER,              sizeof(NamedBuffer),        HT_T_LARGE   ,0 FILL },
  { CN_NEXTGID,                  sizeof(NextGid),            HT_INVALID   ,0 FILL },
  { CN_NMLSTR,                   sizeof(NmlStr),             HT_T_MEDIUM  ,0 FILL },
  { CN_NODE,                     sizeof(Node),               HT_T_TINY    ,0 FILL },
  { CN_NODE_I,                   sizeof(Node_I),             HT_T_TINY    ,0 FILL },
  { CN_NOW,                      sizeof(Now),                HT_T_TINY    ,0 FILL },
  { CN_NUMBAT,                   sizeof(Numbat),             HT_T_LARGE   ,0 FILL },
  { CN_NUMBERING,                sizeof(Numbering),          HT_T_MEDIUM  ,0 FILL },

  { CN__O__,                     'O' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_OBJBUFFER,                sizeof(ObjBuffer),          HT_T_LARGE   ,0 FILL },
  { CN_OBJECTSNAPSHOT,           sizeof(ObjectSnapshot),     HT_T_SMALL   ,0 FILL },
  { CN_OUTPUT,                   sizeof(Output),             HT_T_TINY    ,0 FILL },
  { CN_OP,                       sizeof(Op),                 HT_T_TINY    ,0 FILL },
  { CN_OPENFILES,                sizeof(OpenFiles),          HT_T_TINY    ,0 FILL },
  { CN_OPERANDQUEUE,             sizeof(OperandQueue),       HT_T_TINY    ,0 FILL },
  { CN_OPERANDWATCH,             sizeof(OperandWatch),       HT_T_SMALL   ,0 FILL },
  { CN_OPMETRICS,                sizeof(OpMetrics),          HT_T_TINY    ,0 FILL },
  { CN_OPSPEC,                   sizeof(OpSpec),             HT_T_MEDIUM  ,0 FILL },
  { CN_OPTIMISER,                sizeof(Optimiser),          HT_T_TINY    ,0 FILL },
  { CN_OTHEROPERAND,             sizeof(OtherOperand),       HT_T_TINY    ,0 FILL },

  { CN__P__,                     'P' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_PAGEFILE,                 sizeof(PageFile),           HT_T_TINY    ,0 FILL },
  { CN_PAGINGMETRICS,            sizeof(PagingMetrics),      HT_T_LARGE   ,0 FILL },
  { CN_PARCEL,                   sizeof(Parcel),             HT_T_TINY    ,0 FILL },
  { CN_PATHIDENTIFIER,           sizeof(PathIdentifier),     HT_T_MEDIUM  ,0 FILL },
  { CN_PATHVEC,                  sizeof(PathVec),            HT_T_MEDIUM  ,0 FILL },
  { CN_PERMIT,                   sizeof(Permit),             HT_T_TINY    ,0 FILL },
  { CN_PERUSE,                   sizeof(Peruse),             HT_T_MEDIUM  ,0 FILL },
  { CN_PIPEIN,                   sizeof(PipeIn),             HT_T_TINY    ,0 FILL },
  { CN_PIPELINE,                 sizeof(Pipeline),           HT_T_TINY    ,0 FILL },
  { CN_PIPELINEMETRICS,          sizeof(PipelineMetrics),    HT_T_TINY    ,0 FILL },
  { CN_PLANDEFINITIONPARSER,     sizeof(PlanDefinitionParser), HT_T_MEDIUM,-8 FILL },
  { CN_PLANNERLOCK,              sizeof(PlannerLock),        HT_T_TINY    ,0 FILL },
  { CN_PLANNODE,                 sizeof(PlanNode),           HT_T_TINY    ,-8 FILL },
  { CN_PLANNODEBUILDER,          sizeof(PlanNodeBuilder),    HT_T_TINY    ,0 FILL },
  { CN_PLANNODEBUILDERLIST,      sizeof(PlanNodeBuilderList),HT_T_TINY    ,0 FILL },
  { CN_PLANNODEBUILDERSTACK,     sizeof(PlanNodeBuilderStack), HT_T_MEDIUM,0 FILL },
  { CN_PLANNODELIST,             sizeof(PlanNodeList),       HT_T_TINY    ,0 FILL },
  { CN_PLANOPERAND,              sizeof(PlanOperand),        HT_T_TINY    ,0 FILL },
  { CN_POT,                      sizeof(Pot),                HT_T_LARGE   ,0 FILL },
  { CN_PREDICATE,                sizeof(Predicate),          HT_T_TINY    ,0 FILL },
  { CN_PREDICATEVEC,             sizeof(PredicateVec),       HT_T_MEDIUM  ,0 FILL },
  { CN_PRIMITIVENODE,            sizeof(PrimitiveNode),      HT_T_TINY    ,-8 FILL },
  { CN_PROCEDURE,                sizeof(Procedure),          HT_T_MEDIUM  ,0 FILL },
  { CN_PROFILE,                  sizeof(Profile),            HT_T_MEDIUM  ,0 FILL },
  { CN_PSXTHD,                   sizeof(PsxThd),             HT_T_TINY    ,0 FILL },
  { CN_PTNDEF,                   sizeof(PtnDef),             HT_T_MEDIUM  ,0 FILL },
  { CN_PTNID,                    sizeof(LglPtn),              HT_T_TINY    ,0 FILL },
  { CN_PTNSTMCURSOR,             sizeof(PtnStm_cursor),      HT_T_TINY    ,0 FILL },
  { CN_PTNSTREAM,                sizeof(PtnStream),          HT_T_TINY    ,0 FILL },
  { CN_PTRVAL,                   sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 FILL },

  { CN__Q__,                     'Q' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_QUERYNODE,                sizeof(QueryNode),          HT_T_TINY    ,-8 FILL },
  { CN_QUERYNODELIST,            sizeof(QueryNodeList),      HT_T_TINY    ,0 FILL },
  { CN_QUERYNODESTACK,           sizeof(QueryNodeStack),     HT_T_MEDIUM  ,0 FILL },
  { CN_QUEUE,                    sizeof(Queue),              HT_T_TINY    ,0 FILL },
  { CN_QUEUEBUFFER,              sizeof(QueueBuffer),        HT_INVALID   ,0 FILL },

  { CN__R__,                     'R' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_READER,                   sizeof(Reader),             HT_T_TINY    ,0 FILL },
  { CN_REAPLOCK,                 sizeof(ReapLock),           HT_T_TINY    ,0 FILL },
  { CN_RECORD,                   sizeof(Record),             HT_T_MEDIUM  ,0 FILL },
  { CN_RECVWL,                   sizeof(RecvWL),             HT_T_TINY    ,0 FILL },
  { CN_RELOP,                    sizeof(RelOp),              HT_T_TINY    ,0 FILL },
  { CN_RESOURCES,                sizeof(Resources),          HT_T_TINY    ,0 FILL },
  { CN_RIBBON,                   sizeof(Ribbon),             HT_T_SMALL   ,0 FILL },
  { CN_RIBBONLINE,               sizeof(RibbonLine),         HT_T_SMALL   ,0 FILL },
  { CN_RLUDIARY,                 sizeof(RluDiary),           HT_T_LARGE   ,0 FILL },
  { CN_RN_RAND,                  sizeof(Rand),               HT_INVALID   ,0 FILL },
  { CN_ROOTNODE,                 sizeof(RootNode),           HT_T_TINY    ,-8 FILL },
  { CN_ROW,                      sizeof(Row),                HT_T_LARGE   ,0 FILL },
  { CN_ROWAGG,                   sizeof(Row),/*correct*/     HT_T_MEDIUM  ,0 FILL },
  { CN_ROWBOX,                   0,                          HT_INVALID   ,0 FILL },
  { CN_ROWBUFFER,                sizeof(RowBuffer),          HT_T_LARGE   ,0 FILL },
  { CN_ROWCOMPACTOR,             sizeof(RowCompactor),       HT_T_SMALL   ,0 FILL },
  { CN_ROWONCE,                  sizeof(RowOnce),            HT_T_TINY    ,0 FILL },
  { CN_ROWPIX,                   sizeof(RowPix),             HT_INVALID   ,0 FILL },
  { CN_ROWSINGLE,                sizeof(RowSingle),          HT_T_MEDIUM  ,0 FILL },
  { CN_ROWSINGLELIST,            sizeof(RowSingleList),      HT_T_TINY    ,0 FILL },
  { CN_ROWTABLEPTN,              sizeof(RowTablePtn),        HT_T_TINY    ,0 FILL },
  { CN_ROWTRAY,                  sizeof(RowTray),            HT_T_LARGE   ,0 FILL },
  { CN_RUCPU,                    sizeof(RU_Cpu),             HT_INVALID   ,0 FILL },
  { CN_RUDIR,                    0,/*sizeof(RU_Dir)*/        HT_INVALID   ,0 FILL },
  { CN_RUFILE,                   0,/*sizeof(RU_File)*/       HT_INVALID   ,0 FILL },
  { CN_RULECTL,                  sizeof(RuleCtl),            HT_T_TINY    ,0 FILL },
  { CN_RUMEMORY,                 sizeof(RU_Memory),          HT_INVALID   ,0 FILL },
  { CN_RUMUTEX,                  0,/*sizeof(RU_Mutex)*/      HT_INVALID   ,0 FILL },
  { CN_RUPASS,                   0,/*sizeof(RU_Pass)*/       HT_INVALID   ,0 FILL },
  { CN_RUSOCKET,                 0,/*sizeof(RU_Socket)*/     HT_INVALID   ,0 FILL },
  { CN_RUTIME,                   0,/*sizeof(RU_Time)*/       HT_INVALID   ,0 FILL },
  { CN_RUWORK,                   0,/*sizeof(RU_Word)*/       HT_INVALID   ,0 FILL },

  { CN__S__,                     'S' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_SCANNER,                  sizeof(Scanner),            HT_T_TINY    ,0 FILL },
  { CN_SCHEDULER,                sizeof(Scheduler),          HT_INVALID   ,0 FILL },
  { CN_SCHEMA,                   sizeof(Schema),             HT_T_TINY    ,0 FILL },
  { CN_SEAPI,                    sizeof(SeApi),              HT_T_LARGE   ,0 FILL },
  { CN_SEDEPUTY,                 sizeof(SeDeputy),           HT_T_MEDIUM  ,0 FILL },
  { CN_SEEME,                    sizeof(SeeMe),              HT_INVALID   ,0 FILL },
  { CN_SENCOMPILER,              sizeof(SenCompiler),        HT_T_TINY    ,0 FILL },
  { CN_SENDS,                    sizeof(Sends),              HT_T_TINY    ,0 FILL },
  { CN_SENDWL,                   sizeof(SendWL),             HT_T_TINY    ,0 FILL },
  { CN_SEPROMPT,                 sizeof(SePrompt),           HT_T_MEDIUM  ,0 FILL },
  { CN_SHAPE,                    sizeof(Shape),              HT_T_TINY    ,0 FILL },
  { CN_SHORTCUT,                 sizeof(ShortCut),           HT_T_MEDIUM  ,0 FILL },
  { CN_SHORTCUTVEC,              sizeof(ShortCutVec),        HT_T_MEDIUM  ,0 FILL },
  { CN_SINCE,                    sizeof(Birth),              HT_INVALID   ,0 FILL },
  { CN_SKIPLIST,                 sizeof(SkipListNL),         HT_T_TINY    ,0 FILL },
  { CN_SKIPNODE,                 sizeof(SkipNode),           HT_T_TINY    ,0 FILL },
  { CN_SLAB,                     sizeof(Slab),               HT_T_LARGE   ,0 FILL },
  { CN_SLABALLOCATOR,            sizeof(SlabAllocator),      HT_T_TINY    ,0 FILL },
  { CN_SMALLPERSISTENT,          sizeof(Heap),/*correct*/    HT_INVALID   ,0 FILL },
  { CN_SMALLTRANSIENT,           sizeof(Heap),/*correct*/    HT_INVALID   ,0 FILL },
  { CN_SMALLVIRTUAL,             sizeof(Heap),/*correct*/    HT_INVALID   ,0 FILL },
  { CN_SNBCD,                    sizeof(SN_Bcd),             HT_INVALID   ,0 FILL },
  { CN_SNBOL,                    sizeof(SN_Bol),             HT_INVALID   ,0 FILL },
  { CN_SNDTE,                    sizeof(SN_Dte),             HT_INVALID   ,0 FILL },
  { CN_SNFLT,                    sizeof(SN_Flt),             HT_INVALID   ,0 FILL },
  { CN_SNINT,                    sizeof(SN_Int),             HT_INVALID   ,0 FILL },
  { CN_SNIVL,                    sizeof(SN_Ivl),             HT_INVALID   ,0 FILL },
  { CN_SNTME,                    sizeof(SN_Tme),             HT_INVALID   ,0 FILL },
  { CN_SNTSP,                    sizeof(SN_Tsp),             HT_INVALID   ,0 FILL },
  { CN_SNVCH,                    sizeof(SN_Vch),             HT_INVALID   ,0 FILL },
  { CN_SOCKET,                   sizeof(Socket),             HT_T_TINY    ,0 FILL },
  { CN_SPELAPSED,                sizeof(SP_Elapsed),         HT_INVALID   ,0 FILL },
  { CN_SPID,                     sizeof(SP_Gid),             HT_INVALID   ,0 FILL },
  { CN_SPSTRING,                 sizeof(SP_String),          HT_INVALID   ,0 FILL },
  { CN_SPMEDIUM,                 sizeof(SPmedium),           HT_INVALID   ,0 FILL },
  { CN_SPSMALL,                  sizeof(SPsmall),            HT_INVALID   ,0 FILL },
  { CN_SQLPLANNER,               sizeof(SqlPlanner),         HT_T_MEDIUM  ,-8 FILL },
  { CN_STACK,                    sizeof(TypedStack), /*yes*/ HT_T_MEDIUM  ,0 FILL },
  { CN_STAGE,                    sizeof(Stage),              HT_T_SMALL   ,0 FILL },
  { CN_STAGEMETRICS,             sizeof(StageMetrics),       HT_T_TINY    ,0 FILL },
  { CN_STAGESPEC,                sizeof(StageSpec),          HT_T_TINY    ,0 FILL },
  { CN_STARTER,                  sizeof(Starter),            HT_T_TINY    ,0 FILL },
  { CN_STATEMENT,                sizeof(Statement),          HT_T_MEDIUM  ,0 FILL },
  { CN_STATS,                    sizeof(Stats),              HT_T_TINY    ,0 FILL },
  { CN_STENCIL,                  sizeof(Stencil),            HT_T_SMALL   ,0 FILL },
  { CN_STEPACCUM,                sizeof(StepAccum),          HT_INVALID   ,0 FILL },
  { CN_STEP,                     0,                          HT_INVALID   ,0 FILL },
  { CN_STEPS,                    sizeof(Steps),              HT_T_LARGE   ,0 FILL },
  { CN_STMTLIST,                 sizeof(StmtList),           HT_T_TINY    ,0 FILL },
  { CN_STREAM,                   sizeof(Stream),             HT_T_SMALL   ,0 FILL },
  { CN_STREAMDEF,                sizeof(StreamDef),          HT_T_LARGE   ,0 FILL },
  { CN_STREAMDEFRHT,             sizeof(StreamDefRht),       HT_T_LARGE   ,0 FILL },
  { CN_STREAMMETRICS,            sizeof(StreamMetrics),      HT_T_TINY    ,0 FILL },
  { CN_STREAMSET,                sizeof(StreamSet),          HT_T_LARGE   ,0 FILL },
  { CN_STRING_T,                 sizeof(s32),                HT_INVALID   ,0 FILL },
  { CN_STZLIST,                  sizeof(StzList),            HT_T_TINY    ,0 FILL },
  { CN_STZVAL,                   sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 FILL },
  { CN_SUBCALL,                  sizeof(SubCall),            HT_T_TINY    ,0 FILL },
  { CN_SUBRETURN,                sizeof(SubReturn),          HT_T_TINY    ,0 FILL },
  { CN_SYSCALL,                  sizeof(SysCall),            HT_INVALID   ,0 FILL },
  { CN_SYSMETRICS,               sizeof(SysMetrics),         HT_T_TINY    ,0 FILL },
  { CN_SYSREPORT,                sizeof(SysReport),          HT_T_TINY    ,0 FILL },
  { CN_SCHEDULE,                 sizeof(Schedule),           HT_T_TINY    ,0 FILL },

  { CN__T__,                     'T' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_TABLE,                    sizeof(Table),              HT_T_TINY    ,0 FILL },
  { CN_TABLEDEF,                 sizeof(Table),              HT_T_MEDIUM  ,0 FILL },
  { CN_TABLEPTN,                 sizeof(TablePtn),           HT_T_TINY    ,0 FILL },
  { CN_TABLEPTNVEC,              sizeof(TablePtnVec),        HT_T_TINY    ,0 FILL },
  { CN_TAGSET,                   sizeof(TagSet),             HT_T_MEDIUM  ,0 FILL },
  { CN_TASK,                     sizeof(Task),               HT_T_SMALL   ,0 FILL },
  { CN_TASKLIST,                 sizeof(TaskList),           HT_T_TINY    ,0 FILL },
  { CN_TEMPNAME,                 sizeof(TaskQueue),          HT_T_TINY    ,0 FILL },
  { CN_TCCLOCK,                  sizeof(TccLock),            HT_T_TINY    ,0 FILL },
  { CN_TENANT,                   sizeof(TempName),           HT_T_TINY    ,0 FILL },
  { CN_TENANTS,                  sizeof(Tenant),             HT_T_TINY    ,0 FILL },
  { CN_TASKQUEUE,                sizeof(Tenants),            HT_T_TINY    ,0 FILL },
  { CN_TEXT,                     sizeof(Text),               HT_T_TINY    ,0 FILL },
  { CN_TEXTLARGE,                sizeof(TextLarge),          HT_T_LARGE   ,-8 FILL },
  { CN_TEXTMEDIUM,               sizeof(TextMedium),         HT_T_MEDIUM  ,-8 FILL },
  { CN_TEXTSMALL,                sizeof(TextSmall),          HT_T_SMALL   ,-8 FILL },
  { CN_TEXTSTACK,                sizeof(TextStack),          HT_T_MEDIUM  ,0 FILL },
  { CN_TEXTTINY,                 sizeof(TextTiny),           HT_T_TINY    ,-8 FILL },
  { CN_THING,                    sizeof(Thing),              HT_T_TINY    ,0 FILL },
  { CN_THREAD,                   sizeof(Thread),             HT_T_TINY    ,0 FILL },
  { CN_THREADQUEUE,              sizeof(ThreadQueue),        HT_T_TINY    ,0 FILL },
  { CN_THREADSCRATCH,            sizeof(ThreadScratch),      HT_T_TINY    ,0 FILL },
  { CN_THREADVEC,                sizeof(ThreadVec),          HT_T_MEDIUM  ,0 FILL },
  { CN_TIMER,                    sizeof(Timer),              HT_T_TINY    ,0 FILL },
  { CN_TINYC,                    sizeof(TinyC),              HT_T_TINY    ,0 FILL },
  { CN_TINYPERSISTENT,           sizeof(Heap),/*correct*/    HT_INVALID   ,0 FILL },
  { CN_TINYTRANSIENT,            sizeof(Heap),/*correct*/    HT_INVALID   ,0 FILL },
  { CN_TINYVIRTUAL,              sizeof(Heap),/*correct*/    HT_INVALID   ,0 FILL },
  { CN_TMEVAL,                   sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 FILL },
  { CN_TOKEN,                    sizeof(Token),              HT_T_TINY    ,0 FILL },
  { CN_TOKENVEC,                 sizeof(TokenVec),           HT_T_LARGE   ,0 FILL },
  { CN_TOKEYHSHROW,              sizeof(ToKeyHshRow),        HT_T_MEDIUM  ,0 FILL },
  { CN_TOLGLPTNCRW,              sizeof(ToLglPtnCrw),        HT_T_LARGE   ,0 FILL },
  { CN_TOLGLPTNNST,              sizeof(ToLglPtnNst),        HT_T_MEDIUM  ,0 FILL },
  { CN_TOLGLPTNROW,              sizeof(ToLglPtnRow),        HT_T_LARGE   ,0 FILL },
  { CN_TOTUP,                    sizeof(Totup),              HT_T_MEDIUM  ,0 FILL },
  { CN_TRACELOCK,                sizeof(TraceLock),          HT_T_TINY    ,0 FILL },
  { CN_TRACEMULTILOCK,           sizeof(TraceMultiLock),     HT_T_TINY    ,0 FILL },
  { CN_TSPVAL,                   sizeof(TypedCell),/*yes*/   HT_INVALID   ,0 FILL },
  { CN_TUB,                      sizeof(Tub),                HT_T_TINY    ,0 FILL },
  { CN_TX,                       sizeof(Tx),                 HT_T_TINY    ,0 FILL },
  { CN_TXSET,                    sizeof(TxSet),              HT_T_TINY    ,0 FILL },

  { CN__U__,                     'U' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_UNITINT,                  sizeof(UnitInt),            HT_T_TINY    ,0 FILL },
  { CN_UT_SKIPLIST,              sizeof(UT_SkipList),        HT_T_LARGE   ,0 FILL },

  { CN__V__,                     'V' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_VERBOSE,                  sizeof(Verbose),            HT_T_TINY    ,0 FILL },
  { CN_VERTEX,                   sizeof(Vertex),             HT_T_SMALL   ,0 FILL },
  { CN_VERTEXSPEC,               sizeof(VertexSpec),         HT_T_TINY    ,0 FILL },

  { CN__W__,                     'W' - ('A' - 1),            HT_INVALID   ,0 FILL },
  { CN_WAKEUP,                   sizeof(Wakeup),             HT_INVALID   ,0 FILL },
  { CN_WEAVE,                    sizeof(Weave),              HT_T_LARGE   ,0 FILL },
  { CN_WILDERNESS,               sizeof(Wilderness),         HT_INVALID   ,0 FILL },
  { CN_WORD,                     sizeof(Word),               HT_T_TINY    ,0 FILL },
  { CN_WSPCONVERGENT_t,          0,                          HT_INVALID   ,0 FILL },

  { CN__X__,                     'X' - ('A' - 1),            HT_INVALID   ,0 FILL },

  { CN__Y__,                     'Y' - ('A' - 1),            HT_INVALID   ,0 FILL },

  { CN__Z__,                     'Z' - ('A' - 1),            HT_INVALID   ,0 FILL },

  { CN_ENUMS,                    C_VARGS_END,                HT_INVALID   ,0 FILL }
};

#undef FILL

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
