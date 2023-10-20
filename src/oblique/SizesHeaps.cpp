/*============================================================================*/
/* SizesHeaps.cpp -- Class Sizes and Heaps                                    */
/*============================================================================*/

#include "BASE.h"

#include "eBO.h"
#include "eCC.h"
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
#include "eYX.h"
#include "eRG.h"
#include "eRO.h"
#include "eRU.h"
#include "eTP.h"
#include "eTT.h"
#include "eUK.h"
#include "eXK.h"

#include "SizesHeaps.hpp"

#include "backend/Backend.hpp"

#include "CmdLineArgs.hpp"

#include "DdlLock.hpp"
#include "DicLock.hpp"

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


#include "BufferMap.hpp"

#include "ChildExit.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "ExitOperand.hpp"
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
#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "Table.hpp"

#include "Node.hpp"
#include "Node_I.hpp"
#include "Grid.hpp"
#include "Arg.hpp"
#include "RelOp.hpp"
#include "Predicate.hpp"
#include "PredicateVec.hpp"

#include "ExtendedText.hpp"
#include "ExtendedTextBuffer.hpp"

#include "AccessPath.hpp"
#include "Api.hpp"
#include "AttachMap.hpp"

#include "Exemplar.hpp"

#include "InternalTables.hpp"

#include "Job.hpp"
#include "JobList.hpp"
#include "JobReport.hpp"
#include "Jobs.hpp"

#include "LibInfo.hpp"
#include "Lit.hpp"
#include "LitRecogniser.hpp"
#include "Locator.hpp"
#include "LogicUnit.hpp"

#include "Metadata.hpp"
#include "MetaGet.hpp"

#include "Parcel.hpp"
#include "Numbering.hpp"
#include "Output.hpp"
#include "Op.hpp"
#include "OpSpec.hpp"

#include "PathIdentifier.hpp"
#include "VertexSpec.hpp"
#include "Pipeline.hpp"

#include "Dynamo.hpp"

#include "Procedure.hpp"

#include "ReapLock.hpp"
#include "Dissector.hpp"
#include "Ribbon.hpp"
#include "RibbonLine.hpp"
#include "Optimiser.hpp"

#include "Card.hpp"

#include "Board.hpp"
#include "Bench.hpp"

#include "Scanner.hpp"

#include "SeApi.hpp"

#include "OpSketch.hpp"
#include "PtnDef.hpp"
#include "ShortCut.hpp"
#include "SeDeputy.hpp"
#include "SePrompt.hpp"
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

#include "ToLglPtnCrw.hpp"
#include "ToLglPtnRow.hpp"
#include "TraceLock.hpp"
#include "TraceMultiLock.hpp"
#include "Tub.hpp"

#include "UnitInt.hpp"
#include "UT_SkipList.hpp"

#include "Parcel.hpp"
#include "Sends.hpp"

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
#include "Vertex.hpp"
#include "SubCall.hpp"
#include "SubReturn.hpp"
#include "Deck.hpp"
#include "DeckSpec.hpp"
#include "AdaptorWL.hpp"
#include "PipeIn.hpp"
#include "EvMgr.hpp"
#include "MarkerChain.hpp"
#include "MarkerMgr.hpp"
#include "RejectJob.hpp"
#include "Port.hpp"
#include "LocalStrs.hpp"
#include "LocalVars.hpp"
#include "PayStr.hpp"
#include "PayStrBuffer.hpp"
#include "PermGen.hpp"
#include "CardIter.hpp"
#include "ParamSpec.hpp"
#include "ParamsSpec.hpp"
#include "Scope.hpp"
#include "XF_Fold.hpp"
#include "XF_Flatten_Smooth.hpp"
#include "XF_Flatten.hpp"
#include "CarryForward.hpp"
#include "Equivalence.hpp"
#include "XF_Flatten_Smoosh.hpp"
#include "XF_Flatten_Join_Ordering.hpp"
#include "SawOpCards.hpp"
#include "CorrelatedJoin.hpp"
#include "XF_Flatten_Left_subQuery.hpp"

#include "ArgDef.hpp"
#include "OpDef.hpp"
#include "PipelineDef.hpp"
#include "StageDef.hpp"
#include "XF_Synthesise_code.hpp"
#include "Ppy.hpp"
#include "Symbol.hpp"
#include "Weaver.hpp"

#include "XF.hpp"
#include "XF_Epilog_Add_hash_cast.hpp"
#include "XF_Epilog_Add_inflate.hpp"
#include "XF_Epilog_Append_synthesis.hpp"
#include "XF_Epilog_Craft_StreamDef.hpp"
#include "XF_Epilog_Determine_Columns.hpp"
#include "XF_Epilog_Fuse_project.hpp"
#include "XF_Epilog_Hash_Columns.hpp"
#include "XF_Epilog_ItemStrip_colNm.hpp"
#include "XF_Epilog_Itemise_Columns.hpp"
#include "XF_Epilog_Map_Columns.hpp"
#include "XF_Epilog_Promote_filter.hpp"
#include "XF_Epilog_Reinstate_stage.hpp"
#include "XF_Epilog_X_stmCol_To_colNm.hpp"
#include "XF_Epilog_X_tblCol_To_stmCol.hpp"
#include "XF_Flatten_10_F.hpp"
#include "XF_Flatten_24_W_A_C_ro.hpp"
#include "XF_Flatten_32_W_A_NC_I.hpp"
#include "XF_Flatten_34_W_A_NC_ro.hpp"
#include "XF_Flatten_40_W_L_C_E.hpp"
#include "XF_Flatten_52_W_L_NC_I_exists.hpp"
#include "XF_Flatten_52_W_L_NC_I_join.hpp"
#include "XF_Flatten_53_W_L_NC_NI_exists.hpp"
#include "XF_Flatten_53_W_L_NC_NI_join.hpp"
#include "XF_Flatten_74_H_A_NC_ro.hpp"
#include "XF_Flatten_Purge_where.hpp"
#include "XF_Flatten_Delete_flat.hpp"
#include "XF_Flatten_Derive_bounds.hpp"
#include "XF_Flatten_Anchor_table.hpp"
#include "XF_Flatten_Predicate_Pull_Up.hpp"
#include "XF_Flatten_Predicate_Reposition.hpp"
#include "XF_Flatten_Update_projection.hpp"
#include "XF_Flatten_X_stmCol_To_tblCol.hpp"
#include "XF_Optimise_Cull_bound.hpp"
#include "XF_Optimise_Cut_sort.hpp"
#include "XF_Optimise_Derive_streamKey.hpp"
#include "XF_Optimise_Duplicate_filter.hpp"
#include "XF_Optimise_Early_filter.hpp"
#include "XF_Optimise_Early_projection.hpp"
#include "XF_Optimise_Late_projection.hpp"
#include "XF_Optimise_Reduce_gbyAgg_key.hpp"
#include "XF_Optimise_Static_calc.hpp"
#include "XF_Optimise_aggreg_To_sort_ordAgg.hpp"
#include "XF_Optimise_from_To_OP.hpp"
#include "XF_Optimise_gbyAgg_To_ordAgg.hpp"
#include "XF_Optimise_max_To_top.hpp"
#include "XF_Optimise_scan_To_indexScan.hpp"
#include "XF_Optimise_sortDesc_limit_To_top.hpp"
#include "XF_Optimise_sort_limit_All_Levels.hpp"
#include "XF_Prolog_Analyse_index.hpp"
#include "XF_Prolog_Analyse_injected.hpp"
#include "XF_Prolog_Analyse_emitted.hpp"
#include "XF_Prolog_Analyse_subQuery.hpp"
#include "XF_Prolog_Analyse_table.hpp"
#include "XF_Prolog_Ensure_colType.hpp"
#include "XF_Prolog_Ensure_level.hpp"
#include "XF_Prolog_Erase_construction.hpp"
#include "XF_Prolog_Move_on.hpp"
#include "XF_Flatten_Predicate_Pull_Up.hpp"
#include "XF_Prolog_Reduce_not.hpp"
#include "XF_Prolog_Review_as.hpp"
#include "XF_Prolog_Validate_Board.hpp"
#include "XF_Prolog_X_board_To_sqyNm.hpp"
#include "XF_Prolog_X_colNm_By_stmCol.hpp"
#include "XF_Prolog_X_colNm_To_stmCol.hpp"
#include "XF_Prolog_X_rpnCalc_To_boolPred.hpp"
#include "XF_Prolog_X_scan_To_from.hpp"
#include "XF_Synthesise_pipelineDef.hpp"

#include "XC.hpp"

#include "CardUsage.hpp"
#include "ColMap.hpp"
#include "CardImage.hpp"
#include "CardHistory.hpp"
#include "SketchTkn.hpp"
#include "ColHsh.hpp"
#include "ColInj.hpp"
#include "ShortForm.hpp"
#include "ColPpg.hpp"
#include "MatrixPrint.hpp"


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
#include "parser/common/TypeMap1D.hpp"
#include "parser/common/TypeMap2D.hpp"
#include "parser/common/ExtractMap.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/* sizes & heaps                                                              */
/*----------------------------------------------------------------------------*/
#define FILL 0,0,0,0,0,0,0

SizeHeap gxaSizeHeap[]
{
  {CN_NONE,                     0,                              HT_INVALID   , FILL },

  {CN__A__,                     'A' - ('A' - 1),                HT_INVALID   , FILL },
  {CN_ACCESSPATH,               sizeof(AccessPath),             HT_T_MEDIUM  , FILL },
  {CN_ACTION,                   sizeof(Action),                 HT_T_TINY    , FILL },
  {CN_ADAPTORWL,                sizeof(AdaptorWL),              HT_T_TINY    , FILL },
  {CN_AGGTIMER,                 sizeof(AggTimer),               HT_T_TINY    , FILL },
  {CN_AM,                       0,                              HT_INVALID   , FILL },
  {CN_ANSWERWL,                 sizeof(AnswerWL),               HT_T_TINY    , FILL },
  {CN_API,                      sizeof(Api),                    HT_INVALID   , FILL },
  {CN_ARG,                      sizeof(Arg),                    HT_T_TINY    , FILL },
  {CN_ARGDEF,                   sizeof(ArgDef),                 HT_T_SMALL   , FILL },
  {CN_ARRANGE,                  sizeof(Arrange),                HT_T_TINY    , FILL },
  {CN_ATOMIC,                   sizeof(Atomic),                 HT_INVALID   , FILL },
  {CN_ATTACHMAP,                sizeof(AttachMap),              HT_INVALID   , FILL },

  {CN__B__,                     'B' - ('A' - 1),                HT_INVALID   , FILL },
  {CN_BACKEND,                  sizeof(Backend),/*yes*/     HT_T_MEDIUM  ,     FILL },
  {CN_BALLOON,                  sizeof(Balloon),                HT_T_TINY    , FILL },
  {CN_BCDVAL,                   sizeof(TypeValueArea),/*yes*/   HT_INVALID   , FILL },
  {CN_BENCH,                    sizeof(Bench),                  HT_T_SMALL   , FILL },
  {CN_BIRTH,                    sizeof(Birth),                  HT_INVALID   , FILL },
  { CN_BOARD,                    sizeof(Board),              HT_T_SMALL   ,FILL },
  { CN_BOLVAL,                   sizeof(TypeValueArea),/*yes*/   HT_INVALID   ,FILL },
  { CN_BUFFERMAP,                sizeof(BufferMap),          HT_INVALID   ,FILL },
  { CN_BUFFERPOOL,               sizeof(BufferPool),         HT_INVALID   ,FILL },
  { CN_BYTERATE,                 sizeof(ByteRate),           HT_T_TINY    ,FILL },
  { CN_BYTES_T,                  sizeof(s32),                HT_INVALID   ,FILL },

  { CN__C__,                     'C' - ('A' - 1),            HT_INVALID   ,FILL },
  { CN_CARD,                     sizeof(Card),               HT_T_SMALL   ,FILL },
  { CN_CARDCTL,                  sizeof(CardCtl),            HT_T_LARGE   ,FILL },
  { CN_CARDHISTORY,              sizeof(CardHistory),        HT_T_SMALL   , FILL },
  { CN_CARDIMAGE,                sizeof(CardImage),          HT_T_SMALL   , FILL },
  { CN_CARDITER,                 sizeof(CardIter),           HT_T_MEDIUM  , FILL },
  { CN_CARDITERMEMBERS,          sizeof(CardIterMembers),    HT_T_SMALL   , FILL },
  {CN_CARDUSAGE,                sizeof(CardUsage),                HT_T_SMALL   , FILL },
  {CN_CARRYFORWARD,             sizeof(CarryForward),             HT_T_TINY    , FILL },
  {CN_CATALOG,                  sizeof(Catalog),                  HT_INVALID   , FILL },
  {CN_CHARSER,                  sizeof(Charser),                  HT_T_LARGE   , FILL },
  {CN_CHILDEXIT,                sizeof(ChildExit),                HT_T_TINY    , FILL },
  {CN_CLASSPLACEHOLDER,         sizeof(ClassPlaceHolder),         HT_T_LARGE   , FILL },
  {CN_CMDLINEARGS,              sizeof(CmdLineArgs),              HT_INVALID   , FILL },
  {CN_CODEBLOCK,                sizeof(CodeBlock),                HT_T_MEDIUM  , FILL },
  {CN_COLHSH,                   sizeof(ColHsh),                   HT_T_MEDIUM  , FILL },
  {CN_COLINJ,                   sizeof(ColInj),                   HT_T_MEDIUM  , FILL },
  {CN_COLMAP,                   sizeof(ColMap),                   HT_T_MEDIUM  , FILL },
  {CN_COLPPG,                   sizeof(ColPpg),                   HT_T_MEDIUM  , FILL },
  {CN_COLUMN,                   sizeof(Column),                   HT_T_TINY    , FILL },
  {CN_COMBINEDSRC,              sizeof(CombinedSrc),              HT_T_LARGE   , FILL },
  {CN_CONDITIONALQUERYNODEITER, sizeof(ConditionalQueryNodeIter), HT_T_TINY,     FILL },
  {CN_CORRELATEDJOIN,           sizeof(CorrelatedJoin),           HT_T_TINY    , FILL },
  {CN_CRVBUFFER,                sizeof(CrvBuffer),                HT_T_LARGE   , FILL },
  {CN_CRWBUFFER,                sizeof(CrwBuffer),                HT_T_LARGE   , FILL },
  {CN_CRWTABLEPTN,              sizeof(CrwTablePtn),              HT_T_TINY    , FILL },
  {CN_CRWTOROW,                 sizeof(CrwToRow),                 HT_T_TINY    , FILL },
  {CN_CSVBUFFER,                sizeof(CsvBuffer),                HT_T_LARGE   , FILL },
  {CN_CSVMETA,                  sizeof(CsvMeta),                  HT_T_TINY    , FILL },

  { CN__D__,                     'D' - ('A' - 1),            HT_INVALID   ,FILL },
  { CN_DATATRANSIENT,            sizeof(Heap), /*correct*/   HT_INVALID   ,FILL },
  { CN_DDLLOCK,                  sizeof(DdlLock),            HT_T_TINY    ,FILL },
  { CN_DECACNT,                  sizeof(DecaCnt),            HT_T_TINY    ,FILL },
  { CN_DECIDER,                  sizeof(Decider),            HT_T_TINY    ,FILL },
  { CN_DECK,                     sizeof(Deck),               HT_T_SMALL   ,FILL },
  { CN_DECKSPEC,                 sizeof(DeckSpec),           HT_T_TINY    ,FILL },
  { CN_DEFINITIONNODE,           sizeof(DefinitionNode),     HT_V_TINY    ,FILL },
  { CN_DEREFMETRICS,             sizeof(DerefMetrics),       HT_T_TINY    ,FILL },
  { CN_DIC,                      sizeof(Dic),                HT_T_TINY    ,FILL },
  { CN_DICLOCK,                  sizeof(DicLock),            HT_T_TINY    ,FILL },
  { CN_DICSTACK,                 sizeof(DicStack),           HT_T_TINY    ,FILL },
  { CN_DIR,                      sizeof(Dir),                HT_T_TINY    ,FILL },
  { CN_DISSECTOR,                sizeof(Dissector),              HT_T_TINY    , FILL },
  { CN_DTEVAL,                   sizeof(TypeValueArea),/*yes*/   HT_INVALID   ,FILL },
  { CN_DYNAMIC,                  sizeof(Dynamic),            HT_T_LARGE    ,FILL },
  { CN_DYNAMO,                   sizeof(Dynamo),             HT_T_LARGE    ,FILL },

  { CN__E__,                     'E' - ('A' - 1),            HT_INVALID   ,FILL },
  { CN_EDTVAL,                   sizeof(TypeValueArea),/*yes*/   HT_INVALID   ,FILL },
  { CN_ENCODING,                 sizeof(Encoding),           HT_T_MEDIUM  ,FILL },
  { CN_ENT,                      sizeof(Ent),                HT_INVALID   ,FILL },
  { CN_ENTBUFFER,                sizeof(EntBuffer),          HT_T_LARGE   ,FILL },
  { CN_ENTBUFSORTER,             sizeof(EntBufSorter),       HT_T_LARGE   ,FILL },
  { CN_ENTWRK,                   sizeof(EntWrk),             HT_INVALID   ,FILL },
  { CN_ENUMFLAGS,                sizeof(ClassFlags),         HT_T_TINY    ,FILL },
  { CN_EQUIVALENCE,              sizeof(Equivalence),            HT_T_TINY    , FILL },
  { CN_EVENTHANDLERS,            sizeof(EventHandlers),      HT_T_TINY    ,FILL },
  { CN_EVMGR,                    sizeof(EvMgr),              HT_T_TINY    ,FILL },
  {CN_EXEMPLAR,                  sizeof(Exemplar),               HT_T_TINY    , FILL },
  {CN_EXITOPERAND,               sizeof(ExitOperand),            HT_T_TINY    , FILL },
  {CN_EXTENDEDTEXT,             sizeof(ExtendedText),           HT_T_TINY    , FILL },
  {CN_EXTENDEDTEXTBUFFER,       sizeof(ExtendedTextBuffer),     HT_T_SMALL   , FILL },
  {CN_EXTRACTMAP,               sizeof(ExtractMap),             HT_T_TINY    , FILL },

  {CN__F__,                     'F' - ('A' - 1),                HT_INVALID   , FILL },
  {CN_FIFO,                     sizeof(Fifo),                   HT_T_TINY    , FILL },
  {CN_FILE,                     sizeof(File),                   HT_T_TINY    , FILL },
  {CN_FILEMAP,                  sizeof(FileMap),                HT_INVALID   , FILL },
  {CN_FIPVAL,                   sizeof(TypeValueArea), /*yes*/  HT_INVALID   , FILL },
  {CN_FLTVAL,                   sizeof(TypeValueArea), /*yes*/  HT_INVALID   , FILL },
  {CN_FORMAT,                   sizeof(Format),                 HT_T_TINY    , FILL },
  {CN_FREEPOOLBUFFER,           sizeof(PoolBuffer),/*yes*/  HT_INVALID   ,     FILL },
  {CN_FREECHUNK,                sizeof(FreeChunk),              HT_INVALID   , FILL },
  {CN_FREEDNOT,                 sizeof(FreedNot),               HT_INVALID   , FILL },
  {CN_FREEITEM,                 0,                              HT_INVALID   , FILL },
  {CN_FREQDIST,                 sizeof(FreqDist),               HT_T_SMALL   , FILL },

  {CN__G__,                     'G' - ('A' - 1),                HT_INVALID   , FILL },
  {CN_GRID,                     sizeof(Grid),                   HT_T_TINY    , FILL },

  { CN__H__,                     'H' - ('A' - 1),            HT_INVALID   ,FILL },
  { CN_HASHMAP,                  sizeof(HashMap),            HT_T_SMALL   ,FILL },
  { CN_HASHMAPENTRY,             sizeof(HashMapEntry),       HT_T_TINY    ,FILL },
  { CN_HASHMAPSLOT,              sizeof(HashMapSlot),        HT_T_LARGE   ,FILL },
  { CN_HEAPBUFFER,               sizeof(HeapBuffer),         HT_INVALID   ,FILL },
  { CN_HEAPMEM,                  sizeof(Heaps),              HT_INVALID   ,FILL },

  { CN__I__,                     'I' - ('A' - 1),            HT_INVALID   ,FILL },
  { CN_INDEX,                    sizeof(Index),              HT_T_TINY    ,FILL },
  { CN_INDEXDEF,                 sizeof(IndexDef),           HT_T_SMALL   ,FILL },
  { CN_INDEXSTREAM,              sizeof(IndexStream),        HT_T_TINY    ,FILL },
  { CN_INDEXTREE,                sizeof(IndexTree),          HT_T_TINY    ,FILL },
  { CN_INDEXTREEVEC,             sizeof(IndexTreeVec),       HT_T_TINY    ,FILL },
  { CN_IDXVEC,                   sizeof(IdxVec),             HT_T_TINY    ,FILL },
  { CN_INFOSET,                  sizeof(InfoSet),            HT_T_LARGE   ,FILL },
  { CN_INGESTCHUNK,              sizeof(IngestChunk),        HT_T_LARGE   ,FILL },
  { CN_INPUT,                    sizeof(Input),              HT_T_TINY    ,FILL },
  { CN_INTERNALTABLES,           sizeof(InternalTables),     HT_T_TINY    ,FILL },
  { CN_INTSET,                   sizeof(IntSet),             HT_T_TINY    ,FILL },
  { CN_INTSTACK,                 sizeof(IntStack),           HT_T_TINY    ,FILL },
  { CN_INTVAL,                   sizeof(TypeValueArea),/*yes*/   HT_INVALID   ,FILL },
  {CN_IOCHUNK,                  sizeof(IoChunk),                HT_T_LARGE   , FILL },
  {CN_IOCOPY,                   sizeof(IoCopy),                 HT_T_TINY    , FILL },
  {CN_IVLVAL,                   sizeof(TypeValueArea),/*yes*/   HT_INVALID   , FILL },

  {CN__J__,                     'J' - ('A' - 1),                HT_INVALID   , FILL },
  {CN_JOB,                      sizeof(Job),                    HT_T_MEDIUM  , FILL },
  {CN_JOBLIST,                  sizeof(JobList),                HT_T_TINY    , FILL },
  {CN_JOBMETRICS,               sizeof(JobMetrics),             HT_T_LARGE   , FILL },
  {CN_JOBREPORT,                sizeof(JobReport),              HT_T_TINY    , FILL },
  {CN_JOBS,                     sizeof(Jobs),                   HT_T_TINY    , FILL },

  {CN__K__,                     'K' - ('A' - 1),                HT_INVALID   , FILL },
  {CN_KEY,                      sizeof(Key),                    HT_INVALID   , FILL },
  {CN_KID,                      sizeof(Kid),                    HT_T_TINY    , FILL },

  {CN__L__,                     'L' - ('A' - 1),                HT_INVALID   , FILL },
  {CN_LARGEPERSISTENT,          sizeof(Heap), /*correct*/   HT_INVALID   ,     FILL },
  {CN_LARGETRANSIENT,           sizeof(Heap), /*correct*/   HT_INVALID   ,     FILL },
  {CN_LARGEVIRTUAL,             sizeof(Heap), /*correct*/   HT_INVALID   ,     FILL },
  {CN_LGLBUFSET,                sizeof(LglBufSet),              HT_T_LARGE   , FILL },
  { CN_LGLPTNMAP,                sizeof(LglPtnMap),              HT_T_LARGE   , FILL },
  { CN_LIBINFO,                  sizeof(LibInfo),                HT_T_MEDIUM   , FILL },
  { CN_LICENCE,                  sizeof(Licence),            HT_T_TINY    ,FILL },
  { CN_LIMITS,                   sizeof(Limits),             HT_T_TINY    ,FILL },
  { CN_LISTEXONODE_t,            sizeof(ListExoNode_t<Object>), HT_T_TINY ,FILL },
  { CN_LISTEXOITER_t,            sizeof(ListExoIter_t<Object>), HT_T_TINY ,FILL },
  { CN_LIT,                      sizeof(Lit),                HT_T_TINY    ,FILL },
  { CN_LITRECOGNISER,            sizeof(LitRecogniser),      HT_T_TINY    ,FILL },
  { CN_LOADCHUNK,                sizeof(LoadChunk),          HT_T_LARGE   ,FILL },
  { CN_LOCALCHAIN,               sizeof(LocalFrame),         HT_T_LARGE   ,FILL },
  { CN_LOCALDIR,                 sizeof(LocalDir),           HT_INVALID   ,FILL },
  { CN_LOCALFILE,                sizeof(LocalFile),          HT_INVALID   ,FILL },
  { CN_LOCALSTRS,                sizeof(LocalStrs),         HT_T_TINY     , FILL },
  {CN_LOCALVARS,                sizeof(LocalVars),          HT_T_SMALL  , FILL },
  {CN_LOCATION,                 sizeof(BASE::Location),     HT_T_TINY    , FILL },
  {CN_LOCATOR,                  sizeof(Locator),            HT_T_TINY    , FILL },
  {CN_LOCK,                     sizeof(Lock),               HT_INVALID   , FILL },
  {CN_LOCKS,                    sizeof(Locks),              HT_T_MEDIUM  , FILL },
  {CN_LOCKSET,                  0,                          HT_INVALID   , FILL },
  {CN_LOGICUNIT,                sizeof(LogicUnit),          HT_T_MEDIUM  , FILL },

  {CN__M__,                     'M' - ('A' - 1),            HT_INVALID   , FILL },
  {CN_MARK,                     sizeof(Mark),               HT_T_TINY    , FILL },
  {CN_MARKER,                   sizeof(Marker),             HT_T_TINY    , FILL },
  {CN_MARKERCHAIN,              sizeof(MarkerChain),        HT_T_TINY    , FILL },
  {CN_MARKERMGR,                sizeof(MarkerMgr),          HT_T_TINY    , FILL },
  {CN_MATRIXPRINT,              sizeof(MatrixPrint),        HT_T_LARGE   , FILL },
  {CN_MEDIUMPERSISTENT,         sizeof(Heap), /*correct*/   HT_INVALID   , FILL },
  {CN_MEDIUMTRANSIENT,          sizeof(Heap), /*correct*/   HT_INVALID   , FILL },
  {CN_MEDIUMVIRTUAL,            sizeof(Heap), /*correct*/   HT_INVALID   , FILL },
  {CN_MEMTEST,                  sizeof(MemTest),            HT_T_TINY    , FILL },
  {CN_METADATA,                 sizeof(Metadata),           HT_T_TINY    , FILL },
  {CN_METAGET,                  sizeof(MetaGet),            HT_T_TINY    , FILL },
  {CN_METRICSWSP,               sizeof(MetricsWsp),         HT_INVALID   , FILL },
  {CN_MUTEX,                    sizeof(Mutex),              HT_T_TINY    , FILL },

  {CN__N__,                     'N' - ('A' - 1),            HT_INVALID   , FILL },
  {CN_NAMEDBUFFER,              sizeof(NamedBuffer),        HT_T_LARGE   , FILL },
  {CN_NEXTGID,                  sizeof(NextGid),            HT_INVALID   , FILL },
  {CN_NMLSTR,                   sizeof(NmlStr),             HT_T_MEDIUM  , FILL },
  {CN_NODE,                     sizeof(Node),               HT_T_TINY    , FILL },
  {CN_NODE_I,                   sizeof(Node_I),             HT_T_TINY    , FILL },
  {CN_NOW,                      sizeof(Now),                HT_T_TINY    , FILL },
  { CN_NUMBAT,                   sizeof(Numbat),             HT_T_LARGE   ,FILL },
  { CN_NUMBERING,                sizeof(Numbering),          HT_T_MEDIUM  ,FILL },

  { CN__O__,                     'O' - ('A' - 1),            HT_INVALID   ,FILL },
  {CN_OBJBUFFER,                sizeof(ObjBuffer),          HT_T_LARGE   , FILL },
  {CN_OBJECTSNAPSHOT,           sizeof(ObjectSnapshot),     HT_T_SMALL   , FILL },
  {CN_OBJTRACKER,               sizeof(ObjectTracker),      HT_T_MEDIUM  , FILL },
  {CN_OUTPUT,                   sizeof(Output),             HT_T_TINY    , FILL },
  {CN_OP,                       sizeof(Op),                 HT_T_TINY    , FILL },
  {CN_OPDEF,                    sizeof(OpDef),              HT_T_TINY    , FILL },
  {CN_OPENFILES,                sizeof(OpenFiles),          HT_T_TINY    , FILL },
  {CN_OPERANDQUEUE,             sizeof(OperandQueue),       HT_T_TINY    , FILL },
  {CN_OPERANDWATCH,             sizeof(OperandWatch),       HT_T_SMALL   , FILL },
  {CN_OPSKETCH,                 sizeof(OpSketch),           HT_T_MEDIUM  , FILL },
  {CN_OPMETRICS,                sizeof(OpMetrics),          HT_T_TINY    , FILL },
  {CN_OPSPEC,                   sizeof(OpSpec),             HT_T_MEDIUM  , FILL },
  {CN_OPTIMISER,                sizeof(Optimiser),          HT_T_LARGE   , FILL },
  {CN_OTHEROPERAND,             sizeof(OtherOperand),       HT_T_TINY    , FILL },

  {CN__P__,                     'P' - ('A' - 1),            HT_INVALID   , FILL },
  {CN_PAGEFILE,                 sizeof(PageFile),           HT_T_TINY    , FILL },
  {CN_PAGEINBUFFER,             sizeof(CrwBuffer),          HT_T_LARGE   ,FILL },
  {CN_PAGINGMETRICS,            sizeof(PagingMetrics),      HT_T_LARGE   , FILL },
  {CN_PARAMSPEC,                sizeof(ParamSpec),          HT_T_TINY    , FILL },
  {CN_PARAMSSPEC,               sizeof(ParamsSpec),         HT_T_TINY    , FILL },
  {CN_PARCEL,                   sizeof(Parcel),             HT_T_TINY    , FILL },
  {CN_PATHIDENTIFIER,           sizeof(PathIdentifier),     HT_T_MEDIUM  , FILL },
  { CN_PATHVEC,                  sizeof(PathVec),            HT_T_MEDIUM  ,FILL },
  { CN_PAYSTR,                   sizeof(PayStr),             HT_INVALID   ,FILL },
  { CN_PAYSTRBUFFER,             sizeof(PayStrBuffer),       HT_T_LARGE   ,FILL },
  { CN_PDS,                      sizeof(Pds),                HT_T_LARGE   , FILL },
  { CN_PDSBUFFERIDQUEUE,         sizeof(PdsBufferIdQueue),   HT_T_TINY    , FILL },
  { CN_PDSCURSOR,                sizeof(PdsCursor),          HT_T_TINY    , FILL },
  { CN_PERMGEN,                  sizeof(PermGen),            HT_T_LARGE   ,FILL },
  { CN_PERMIT,                   sizeof(Permit),             HT_T_TINY    ,FILL },
  { CN_PERUSE,                   sizeof(Peruse),             HT_T_MEDIUM  ,FILL },
  { CN_PIPEIN,                   sizeof(PipeIn),             HT_T_TINY    ,FILL },
  { CN_PIPELINE,                 sizeof(Pipeline),           HT_T_TINY    ,FILL },
  { CN_PIPELINEDEF,              sizeof(PipelineDef),        HT_T_LARGE   , FILL },
  { CN_PIPELINEMETRICS,          sizeof(PipelineMetrics),    HT_T_TINY    ,FILL },
  { CN_PLANDEFINITIONPARSER,     sizeof(PlanDefinitionParser),   HT_V_MEDIUM,   FILL },
  {CN_PLANNODE,             sizeof(PlanNode),               HT_V_TINY    , FILL },
  {CN_PLANNODEBUILDER,      sizeof(PlanNodeBuilder),        HT_T_TINY    , FILL },
  {CN_PLANNODEBUILDERLIST,  sizeof(PlanNodeBuilderList),    HT_T_TINY    , FILL },
  {CN_PLANNODEBUILDERSTACK, sizeof(PlanNodeBuilderStack),   HT_T_MEDIUM,   FILL },
  {CN_PLANNODELIST,         sizeof(PlanNodeList),           HT_T_TINY    , FILL },
  {CN_PLANOPERAND,          sizeof(PlanOperand),            HT_T_TINY    , FILL },
  {CN_POT,                  sizeof(Pot),                    HT_T_LARGE   , FILL },
  {CN_PORT,                 sizeof(Port),                   HT_T_TINY    , FILL },
  {CN_PPY,                  sizeof(Ppy),                    HT_T_TINY    , FILL },
  {CN_PREDICATE,            sizeof(Predicate),              HT_T_TINY    , FILL },
  {CN_PREDICATEVEC,         sizeof(PredicateVec),           HT_T_MEDIUM  , FILL },
  {CN_PRIMITIVENODE,        sizeof(PrimitiveNode),          HT_V_TINY    , FILL },
  {CN_PROCEDURE,            sizeof(Procedure),              HT_T_MEDIUM  , FILL },
  {CN_PROFILE,              sizeof(Profile),                HT_T_MEDIUM  , FILL },
  {CN_PSXTHD,               sizeof(PsxThd),                 HT_T_TINY    , FILL },
  {CN_PTNDEF,               sizeof(PtnDef),                 HT_T_MEDIUM  , FILL },
  {CN_PTNID,                sizeof(LglPtn),                 HT_T_TINY    , FILL },
  {CN_PTRVAL,               sizeof(TypeValueArea),/*yes*/   HT_INVALID   , FILL },

  { CN__Q__,                     'Q' - ('A' - 1),                HT_INVALID   , FILL },
  { CN_QUERYNODE,                sizeof(QueryNode),              HT_V_TINY    , FILL },
  { CN_QUERYNODELIST,            sizeof(QueryNodeList),          HT_T_TINY    , FILL },
  { CN_QUERYNODESTACK,           sizeof(QueryNodeStack),         HT_T_MEDIUM  , FILL },
  { CN_QUEUE,                    sizeof(Queue),                  HT_T_TINY    , FILL },
  { CN_QUEUEBUFFER,              sizeof(QueueBuffer),            HT_INVALID   , FILL },

  {CN__R__,                     'R' - ('A' - 1),                HT_INVALID   , FILL },
  {CN_READER,                   sizeof(Reader),                 HT_T_TINY    , FILL },
  {CN_REAPLOCK,                 sizeof(ReapLock),               HT_T_TINY    , FILL },
  {CN_RECORD,                   sizeof(Record),                 HT_T_MEDIUM  , FILL },
  { CN_RECVWL,                   sizeof(RecvWL),             HT_T_TINY    ,FILL },
  { CN_REJECTJOB,                sizeof(RejectJob),          HT_T_TINY    ,FILL },
  { CN_RELOP,                    sizeof(RelOp),              HT_T_TINY    ,FILL },
  { CN_RESOURCES,                sizeof(Resources),          HT_T_TINY    ,FILL },
  { CN_RIBBON,                   sizeof(Ribbon),             HT_T_SMALL   ,FILL },
  { CN_RIBBONLINE,               sizeof(RibbonLine),         HT_T_SMALL   ,FILL },
  { CN_RLUDIARY,                 sizeof(RluDiary),           HT_T_LARGE   ,FILL },
  { CN_RN_RAND,                  sizeof(Rand),               HT_INVALID   ,FILL },
  { CN_ROOTNODE,                 sizeof(RootNode),           HT_V_TINY    ,FILL },
  { CN_ROW,                      sizeof(Row),                HT_T_LARGE   ,FILL },
  { CN_ROWAGG,                   sizeof(Row),/*correct*/     HT_T_MEDIUM  ,FILL },
  { CN_ROWBOX,                   0,                          HT_INVALID   ,FILL },
  { CN_ROWBUFFER,                sizeof(RowBuffer),          HT_T_LARGE   ,FILL },
  { CN_ROWCOMPACTOR,             sizeof(RowCompactor),       HT_T_SMALL   ,FILL },
  { CN_ROWONCE,                  sizeof(RowOnce),            HT_T_TINY    ,FILL },
  { CN_ROWPIX,                   sizeof(RowPix),             HT_T_MEDIUM  ,FILL },
  { CN_ROWSINGLE,                sizeof(RowSingle),          HT_T_MEDIUM  ,FILL },
  { CN_ROWSINGLELIST,            sizeof(RowSingleList),      HT_T_TINY    ,FILL },
  { CN_ROWTRAY,                  sizeof(RowTray),            HT_T_LARGE   ,FILL },
  { CN_RUCPU,                    sizeof(RU_Cpu),             HT_INVALID   ,FILL },
  { CN_RUDIR,                    0,/*sizeof(RU_Dir)*/        HT_INVALID   ,FILL },
  { CN_RUFILE,                   0,/*sizeof(RU_File)*/       HT_INVALID   ,FILL },
  { CN_RUMEMORY,                 sizeof(RU_Memory),          HT_INVALID   ,FILL },
  { CN_RUMUTEX,                  0,/*sizeof(RU_Mutex)*/      HT_INVALID   ,FILL },
  { CN_RUPASS,                   0,/*sizeof(RU_Pass)*/       HT_INVALID   ,FILL },
  { CN_RUSOCKET,                 0,/*sizeof(RU_Socket)*/     HT_INVALID   ,FILL },
  { CN_RUTIME,                   0,/*sizeof(RU_Time)*/       HT_INVALID   ,FILL },
  { CN_RUWORK,                   0,/*sizeof(RU_Word)*/       HT_INVALID   ,FILL },

  { CN__S__,                     'S' - ('A' - 1),            HT_INVALID   ,FILL },
  { CN_SAWOPCARDS,               sizeof(SawOpCards),         HT_T_TINY    ,FILL },
  { CN_SCANNER,                  sizeof(Scanner),            HT_T_TINY    ,FILL },
  { CN_SCHEDULE,                 sizeof(Schedule),           HT_T_TINY    ,FILL },
  { CN_SCHEDULER,                sizeof(Scheduler),          HT_INVALID   ,FILL },
  { CN_SCHEMA,                   sizeof(Schema),             HT_T_TINY    ,FILL },
  { CN_SCOPE,                    sizeof(Scope),              HT_T_TINY    ,FILL },
  { CN_SEAPI,                    sizeof(SeApi),              HT_T_LARGE   ,FILL },
  { CN_SEDEPUTY,        sizeof(SeDeputy),           HT_T_MEDIUM  , FILL },
  { CN_SEEME,           sizeof(SeeMe),              HT_INVALID   , FILL },
  { CN_SENCELL,         sizeof(SenCell),            HT_INVALID   , FILL },
  { CN_SENCOMPILER,     sizeof(SenCompiler),        HT_T_TINY    , FILL },
  { CN_SENDS,           sizeof(Sends),              HT_T_TINY    , FILL },
  { CN_SENDWL,          sizeof(SendWL),             HT_T_TINY    , FILL },
  { CN_SEPROMPT,        sizeof(SePrompt),           HT_T_MEDIUM  , FILL },
  { CN_SHORTCUT,        sizeof(ShortCut),           HT_T_MEDIUM  , FILL },
  { CN_SHORTCUTVEC,     sizeof(ShortCutVec),        HT_T_MEDIUM  , FILL },
  { CN_SHORTFORM,       sizeof(ShortForm),          HT_T_LARGE  , FILL },
  {CN_SINCE,           sizeof(Birth),              HT_INVALID   , FILL },
  {CN_SKETCHTKN,       sizeof(SketchTkn),          HT_T_TINY    , FILL },
  {CN_SKIPLIST,        sizeof(SkipListNL),         HT_T_TINY    , FILL },
  {CN_SKIPLISTNODE,    sizeof(SkipListNode),       HT_INVALID   , FILL },
  {CN_SKIPLISTBLOCK,   sizeof(SkipListBlock),      HT_INVALID   , FILL },
  {CN_SKIPNODE,        sizeof(SkipNode),           HT_T_TINY    , FILL },
  {CN_SLAB,            sizeof(Slab),               HT_T_LARGE   , FILL },
  {CN_SLABALLOCATOR,   sizeof(SlabAllocator),      HT_T_TINY    , FILL },
  {CN_SMALLPERSISTENT, sizeof(Heap),/*correct*/    HT_INVALID   , FILL },
  {CN_SMALLTRANSIENT,  sizeof(Heap),/*correct*/    HT_INVALID   , FILL },
  {CN_SMALLVIRTUAL,    sizeof(Heap),/*correct*/    HT_INVALID   , FILL },
  {CN_SNBCD,           sizeof(SN_Bcd),             HT_INVALID   , FILL },
  {CN_SNBOL,           sizeof(SN_Bol),             HT_INVALID   , FILL },
  {CN_SNDTE,           sizeof(SN_Dte),             HT_INVALID   , FILL },
  {CN_SNFLT,           sizeof(SN_Flt),             HT_INVALID   , FILL },
  {CN_SNINT,           sizeof(SN_Int),             HT_INVALID   , FILL },
  {CN_SNIVL,           sizeof(SN_Ivl),             HT_INVALID   , FILL },
  {CN_SNTME,           sizeof(SN_Tme),             HT_INVALID   , FILL },
  {CN_SNTSP,           sizeof(SN_Tsp),             HT_INVALID   , FILL },
  {CN_SNVCH,           sizeof(SN_Vch),             HT_INVALID   , FILL },
  {CN_SOCKET,          sizeof(Socket),             HT_T_TINY    , FILL },
  { CN_SPELAPSED,                sizeof(SP_Elapsed),         HT_INVALID   ,FILL },
  { CN_SPID,                     sizeof(SP_Gid),             HT_INVALID   ,FILL },
  { CN_SPSTRING,                 sizeof(SP_String),          HT_INVALID   ,FILL },
  { CN_SPMEDIUM,                 sizeof(SPmedium),           HT_INVALID   ,FILL },
  { CN_SPSMALL,                  sizeof(SPsmall),            HT_INVALID   ,FILL },
  { CN_SQLPLANNER,               sizeof(SqlPlanner),         HT_V_MEDIUM  ,FILL },
  { CN_STACK,                    sizeof(TypedStack), /*yes*/ HT_T_MEDIUM  ,FILL },
  { CN_STAGE,                    sizeof(Stage),              HT_T_SMALL   ,FILL },
  { CN_STAGEDEF,                 sizeof(StageDef),           HT_T_MEDIUM  ,FILL },
  { CN_STAGEMETRICS,             sizeof(StageMetrics),       HT_T_TINY    ,FILL },
  { CN_STAGESPEC,                sizeof(StageSpec),          HT_T_TINY    ,FILL },
  { CN_STARTER,                  sizeof(Starter),            HT_T_TINY    ,FILL },
  { CN_STATEMENT,                sizeof(Statement),          HT_T_MEDIUM  ,FILL },
  { CN_STATS,                    sizeof(Stats),              HT_T_TINY    ,FILL },
  { CN_STENCIL,                  sizeof(Stencil),            HT_T_LARGE   ,FILL },
  { CN_STEP,                     0,                          HT_INVALID   ,FILL },
  { CN_STEPACCUM,                sizeof(StepAccum),          HT_INVALID   ,FILL },
  { CN_STEPS,                    sizeof(Steps),              HT_T_LARGE   ,FILL },
  { CN_STMTLIST,                 sizeof(StmtList),           HT_T_TINY    ,FILL },
  { CN_STREAM,                   sizeof(Stream),             HT_T_SMALL   ,FILL },
  { CN_STREAMDEF,                sizeof(StreamDef),          HT_T_LARGE   ,FILL },
  { CN_STREAMDEFRHT,             sizeof(StreamDefRht),       HT_T_LARGE   ,FILL },
  { CN_STREAMMETRICS,            sizeof(StreamMetrics),      HT_T_TINY    ,FILL },
  { CN_STREAMSET,                sizeof(StreamSet),          HT_T_LARGE   ,FILL },
  { CN_STRING_T,                 sizeof(s32),                HT_INVALID   ,FILL },
  { CN_STRLIB,                   0,/*yes*/                   HT_INVALID   ,FILL },
  { CN_STZLIST,                  sizeof(StzList),            HT_T_TINY    ,FILL },
  { CN_STZVAL,                   sizeof(TypeValueArea),/*yes*/   HT_INVALID   ,FILL },
  { CN_SUBCALL,                  sizeof(SubCall),            HT_T_TINY    ,FILL },
  { CN_SUBQUERY,                 sizeof(SubQuery),           HT_T_TINY    ,FILL },
  { CN_SUBRETURN,                sizeof(SubReturn),          HT_T_TINY    ,FILL },
  { CN_SYMBOL,                   sizeof(Symbol),             HT_T_SMALL   ,FILL },
  { CN_SYSCALL,                  sizeof(SysCall),            HT_INVALID   ,FILL },
  { CN_SYSMETRICS,               sizeof(SysMetrics),         HT_T_TINY    ,FILL },
  { CN_SYSREPORT,                sizeof(SysReport),          HT_T_TINY    ,FILL },

  { CN__T__,                     'T' - ('A' - 1),            HT_INVALID   ,FILL },
  { CN_TABLE,                    sizeof(Table),              HT_T_LARGE   ,FILL },
  { CN_TABLEDEF,                 sizeof(Table),              HT_T_MEDIUM  ,FILL },
  { CN_TABLEPTN,                 sizeof(TablePtn),           HT_T_TINY    ,FILL },
  { CN_TABLEPTNVEC,              sizeof(TablePtnVec),        HT_T_TINY    ,FILL },
  { CN_TAGSET,                   sizeof(TagSet),             HT_T_MEDIUM  ,FILL },
  { CN_TASK,                     sizeof(Task),               HT_T_MEDIUM  ,FILL },
  { CN_TASKLIST,                 sizeof(TaskList),           HT_T_TINY    ,FILL },
  { CN_TASKQUEUE,                sizeof(TaskQueue),          HT_T_TINY    ,FILL },
  { CN_TEMPNAME,                 sizeof(TempName),           HT_T_TINY    ,FILL },
  { CN_TENANT,                   sizeof(Tenant),             HT_T_TINY    ,FILL },
  { CN_TENANTS,                  sizeof(Tenants),            HT_T_TINY    ,FILL },
  { CN_TEXT,                     sizeof(Text),               HT_T_TINY    ,FILL },
  { CN_TEXTLARGE,                sizeof(TextLarge),          HT_T_LARGE   ,FILL },
  { CN_TEXTMEDIUM,               sizeof(TextMedium),         HT_T_MEDIUM  ,FILL },
  { CN_TEXTSMALL,                sizeof(TextSmall),          HT_T_SMALL   ,FILL },
  { CN_TEXTSTACK,                sizeof(TextStack),          HT_T_MEDIUM  ,FILL },
  { CN_TEXTTINY,                 sizeof(TextTiny),           HT_T_TINY    ,FILL },
  { CN_THING,                    sizeof(Thing),              HT_T_TINY    ,FILL },
  { CN_THREAD,                   sizeof(Thread),             HT_T_TINY    ,FILL },
  { CN_THREADQUEUE,              sizeof(ThreadQueue),        HT_T_TINY    ,FILL },
  { CN_THREADSCRATCH,            sizeof(ThreadScratch),      HT_T_TINY    ,FILL },
  { CN_THREADVEC,                sizeof(ThreadVec),          HT_T_MEDIUM  ,FILL },
  { CN_TIMER,                    sizeof(Timer),              HT_T_TINY    ,FILL },
  { CN_TINYPERSISTENT,           sizeof(Heap),/*correct*/    HT_INVALID   ,FILL },
  { CN_TINYTRANSIENT,            sizeof(Heap),/*correct*/    HT_INVALID   ,FILL },
  { CN_TINYVIRTUAL,              sizeof(Heap),/*correct*/    HT_INVALID   ,FILL },
  { CN_TMEVAL,                   sizeof(TypeValueArea),/*yes*/   HT_INVALID   ,FILL },
  { CN_TOKEN,                    sizeof(Token),                  HT_T_TINY    , FILL },
  { CN_TOKENVEC,                 sizeof(TokenVec),               HT_T_LARGE   , FILL },
  { CN_TOKEYHSHROW,              sizeof(ToKeyHshRow),            HT_T_MEDIUM  , FILL },
  { CN_TOLGLPTNCRW,              sizeof(ToLglPtnCrw),            HT_T_LARGE   , FILL },
  { CN_TOLGLPTNNST,              sizeof(ToLglPtnNst),            HT_T_MEDIUM  , FILL },
  { CN_TOLGLPTNROW,              sizeof(ToLglPtnRow),            HT_T_LARGE   , FILL },
  { CN_TOTUP,                    sizeof(Totup),                  HT_T_MEDIUM  , FILL },
  { CN_TRACELOCK,                sizeof(TraceLock),              HT_T_TINY    , FILL },
  { CN_TRACEMULTILOCK,           sizeof(TraceMultiLock),         HT_T_TINY    , FILL },
  { CN_TSPVAL,                   sizeof(TypeValueArea),/*yes*/   HT_INVALID   , FILL },
  { CN_TUB,                      sizeof(Tub),                    HT_T_TINY    , FILL },
  { CN_TX,                       sizeof(Tx),                     HT_T_TINY    , FILL },
  { CN_TXSET,                    sizeof(TxSet),                  HT_T_TINY    , FILL },
  { CN_TYPEMAP1D,                sizeof(TypeMap1D),              HT_T_TINY    , FILL },
  { CN_TYPEMAP2D,                sizeof(TypeMap2D),    HT_T_TINY    , FILL },

  {CN__U__,                     'U' - ('A' - 1),             HT_INVALID   , FILL },
  {CN_UNITINT,                  sizeof(UnitInt),            HT_T_TINY    , FILL },
  {CN_UT_SKIPLIST,              sizeof(UT_SkipList),        HT_T_LARGE   , FILL },

  {CN__V__,                     'V' - ('A' - 1),            HT_INVALID   , FILL },
  {CN_VERBOSE,                  sizeof(Verbose),            HT_T_TINY    , FILL },
  {CN_VERTEX,                   sizeof(Vertex),             HT_T_SMALL   , FILL },
  {CN_VERTEXSPEC,               sizeof(VertexSpec),         HT_T_TINY    , FILL },

  {CN__W__,                     'W' - ('A' - 1),                      HT_INVALID   , FILL },
  {CN_WAKEUP, sizeof(Wakeup),                                         HT_INVALID   , FILL },
  {CN_WEAVER,                      sizeof(Weaver),                      HT_T_LARGE   , FILL },
  {CN_WILDERNESS,                  sizeof(Wilderness),                  HT_INVALID   , FILL },
  {CN_WORD,                        sizeof(Word),                        HT_T_TINY    , FILL },
  {CN_WSPCONVERGENT_t,             0,                                   HT_INVALID   , FILL },

  {CN__X__,                    'X' - ('A' - 1),                         HT_INVALID   , FILL },
  {CN_XC,                                sizeof(XC),                                HT_T_TINY    , FILL },
  {CN_XF,                                sizeof(XF),                                HT_T_TINY    , FILL },
  {CN_XF_EPILOG_ADD_HASH_CAST,           sizeof(XF_Epilog_Add_hash_cast),           HT_T_TINY    , FILL },
  {CN_XF_EPILOG_ADD_INFLATE,             sizeof(XF_Epilog_Add_inflate),             HT_T_TINY    , FILL },
  {CN_XF_EPILOG_APPEND_SYNTHESIS,        sizeof(XF_Epilog_Append_synthesis),        HT_T_TINY    , FILL },
  {CN_XF_EPILOG_CRAFT_STREAMDEF,         sizeof(XF_Epilog_Craft_StreamDef),         HT_T_TINY    , FILL },
  {CN_XF_EPILOG_DETERMINE_COLUMNS,       sizeof(XF_Epilog_Determine_Columns),       HT_T_TINY    , FILL },
  {CN_XF_EPILOG_FUSE_PROJECT,            sizeof(XF_Epilog_Fuse_project),            HT_T_TINY    , FILL },
  {CN_XF_EPILOG_HASH_COLUMNS,            sizeof(XF_Epilog_Hash_Columns),            HT_T_TINY    , FILL },
  {CN_XF_EPILOG_ITEMISE_COLUMNS,         sizeof(XF_Epilog_Itemise_Columns),         HT_T_TINY    , FILL },
  {CN_XF_EPILOG_ITEMSTRIP_COLNM,         sizeof(XF_Epilog_ItemStrip_colNm),         HT_T_TINY    , FILL },
  {CN_XF_EPILOG_MAP_COLUMNS,             sizeof(XF_Epilog_Map_Columns),             HT_T_TINY    , FILL },
  {CN_XF_EPILOG_PROMOTE_FILTER,          sizeof(XF_Epilog_Promote_filter),          HT_T_TINY    , FILL },
  {CN_XF_EPILOG_REINSTATE_STAGE,         sizeof(XF_Epilog_Reinstate_stage),         HT_T_TINY    , FILL },
  {CN_XF_EPILOG_X_STMCOL_TO_COLNM,       sizeof(XF_Epilog_X_stmCol_To_colNm),       HT_T_TINY    , FILL },
  {CN_XF_EPILOG_X_TBLCOL_TO_STMCOL,      sizeof(XF_Epilog_X_tblCol_To_stmCol),      HT_T_TINY    , FILL },
  {CN_XF_FLATTEN,                        sizeof(XF_Flatten),                        HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_10_F,                   sizeof(XF_Flatten_10_F),                   HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_24_W_A_C_RO,            sizeof(XF_Flatten_24_W_A_C_ro),            HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_32_W_A_NC_I,            sizeof(XF_Flatten_32_W_A_NC_I),            HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_34_W_A_NC_RO,           sizeof(XF_Flatten_34_W_A_NC_ro),           HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_40_W_L_C_E,             sizeof(XF_Flatten_40_W_L_C_E),             HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_52_W_L_NC_I_EXISTS,     sizeof(XF_Flatten_52_W_L_NC_I_exists),     HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_52_W_L_NC_I_JOIN,       sizeof(XF_Flatten_52_W_L_NC_I_join),       HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS,    sizeof(XF_Flatten_53_W_L_NC_NI_exists),    HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_53_W_L_NC_NI_JOIN,      sizeof(XF_Flatten_53_W_L_NC_NI_join),      HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_74_H_A_NC_RO,           sizeof(XF_Flatten_74_H_A_NC_ro),           HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_PURGE_WHERE,            sizeof(XF_Flatten_Purge_where),            HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_DELETE_FLAT,            sizeof(XF_Flatten_Delete_flat),            HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_DERIVE_BOUNDS,          sizeof(XF_Flatten_Derive_bounds),          HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_ANCHOR_TABLE,           sizeof(XF_Flatten_Anchor_table),           HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_JOIN_ORDERING,          sizeof(XF_Flatten_Join_Ordering),          HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_LEFT_SUBQUERY,          sizeof(XF_Flatten_Left_subQuery),          HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_PREDICATE_PULLUP,       sizeof(XF_Flatten_Predicate_Pull_Up),      HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_PREDICATE_REPOSITION,   sizeof(XF_Flatten_Predicate_Reposition),   HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_SMOOSH,                 sizeof(XF_Flatten_Smoosh),                 HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_SMOOTH,                 sizeof(XF_Flatten_Smooth),                 HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_UPDATE_PROJECTION,      sizeof(XF_Flatten_Update_projection),      HT_T_TINY    , FILL },
  {CN_XF_FLATTEN_X_STMCOL_TO_TBLCOL,     sizeof(XF_Flatten_X_stmCol_To_tblCol),     HT_T_TINY    , FILL },
  {CN_XF_FOLD,                           sizeof(XF_Fold),                           HT_T_TINY    , FILL },
  {CN_XF_OPTIMISE_AGGREG_TO_SORT_ORDAGG, sizeof(XF_Optimise_aggreg_To_sort_ordAgg), HT_T_TINY    , FILL },
  {CN_XF_OPTIMISE_CULL_BOUND,            sizeof(XF_Optimise_Cull_bound),            HT_T_TINY    , FILL },
  {CN_XF_OPTIMISE_CUT_SORT,              sizeof(XF_Optimise_Cut_sort),              HT_T_TINY    , FILL },
  {CN_XF_OPTIMISE_DERIVE_STREAM_KEY,     sizeof(XF_Optimise_Derive_streamKey),      HT_T_TINY    , FILL },
  {CN_XF_OPTIMISE_DUPLICATE_FILTER,      sizeof(XF_Optimise_Duplicate_filter),      HT_T_TINY    , FILL },
  {CN_XF_OPTIMISE_EARLY_FILTER,          sizeof(XF_Optimise_Early_filter),          HT_T_TINY    , FILL },
  {CN_XF_OPTIMISE_EARLY_PROJECTION,      sizeof(XF_Optimise_Early_projection),      HT_T_TINY    , FILL },
  {CN_XF_OPTIMISE_FROM_TO_OP,            sizeof(XF_Optimise_from_To_OP),            HT_T_TINY    , FILL },
  {CN_XF_OPTIMISE_GBYAGG_TO_ORDAGG,      sizeof(XF_Optimise_gbyAgg_To_ordAgg),      HT_T_TINY    , FILL },
  {CN_XF_OPTIMISE_LATE_PROJECTION,       sizeof(XF_Optimise_Late_projection),       HT_T_TINY    , FILL },
  {CN_XF_OPTIMISE_MAX_TO_TOP,            sizeof(XF_Optimise_max_To_top),            HT_T_TINY    , FILL },
  {CN_XF_OPTIMISE_REDUCE_GBYAGG_KEY,     sizeof(XF_Optimise_Reduce_gbyAgg_key),     HT_T_TINY    , FILL },
  {CN_XF_OPTIMISE_SCAN_TO_INDEXSCAN,     sizeof(XF_Optimise_scan_To_indexScan),     HT_T_TINY    , FILL },
  {CN_XF_OPTIMISE_SORTDESC_LIMIT_TO_TOP, sizeof(XF_Optimise_sortDesc_limit_to_top), HT_T_TINY    , FILL },
  {CN_XF_OPTIMISE_SORT_LIMIT_ALL_LEVELS, sizeof(XF_Optimise_sort_limit_All_Levels), HT_T_TINY    , FILL },
  {CN_XF_OPTIMISE_STATIC_CALC,           sizeof(XF_Optimise_Static_calc),           HT_T_TINY    , FILL },
  {CN_XF_PROLOG_ANALYSE_EMITTED,         sizeof(XF_Prolog_Analyse_emitted),         HT_T_TINY    , FILL },
  {CN_XF_PROLOG_ANALYSE_INDEX,           sizeof(XF_Prolog_Analyse_index),           HT_T_TINY    , FILL },
  {CN_XF_PROLOG_ANALYSE_INJECTED,      sizeof(XF_Prolog_Analyse_injected),      HT_T_TINY    , FILL },
  {CN_XF_PROLOG_ANALYSE_SUBQUERY,      sizeof(XF_Prolog_Analyse_subQuery),      HT_T_TINY    , FILL },
  {CN_XF_PROLOG_ANALYSE_TABLE,         sizeof(XF_Prolog_Analyse_table),         HT_T_TINY    , FILL },
  {CN_XF_PROLOG_ENSURE_COLTYPE,        sizeof(XF_Prolog_Ensure_colType),        HT_T_TINY    , FILL },
  {CN_XF_PROLOG_ENSURE_LEVEL,          sizeof(XF_Prolog_Ensure_level),          HT_T_TINY    , FILL },
  {CN_XF_PROLOG_ERASE_CONSTRUCTION,    sizeof(XF_Prolog_Erase_construction),    HT_T_TINY    , FILL },
  {CN_XF_PROLOG_MOVE_ON,               sizeof(XF_Prolog_Move_on),               HT_T_TINY    , FILL },
  {CN_XF_PROLOG_REDUCE_NOT,            sizeof(XF_Prolog_Reduce_not),            HT_T_TINY    , FILL },
  {CN_XF_PROLOG_REVIEW_AS,             sizeof(XF_Prolog_Review_as),             HT_T_TINY    , FILL },
  {CN_XF_PROLOG_VALIDATE_BOARD,        sizeof(XF_Prolog_Validate_Board),        HT_T_TINY    , FILL },
  {CN_XF_PROLOG_X_BOARD_TO_SQYNM,      sizeof(XF_Prolog_X_board_To_sqyNm),      HT_T_TINY    , FILL },
  {CN_XF_PROLOG_X_COLNM_BY_STMCOL,     sizeof(XF_Prolog_X_colNm_By_stmCol),     HT_T_TINY    , FILL },
  {CN_XF_PROLOG_X_COLNM_TO_STMCOL,     sizeof(XF_Prolog_X_colNm_To_stmCol),     HT_T_TINY    , FILL },
  {CN_XF_PROLOG_X_RPNCALC_TO_BOOLPRED, sizeof(XF_Prolog_X_rpnCalc_To_boolPred), HT_T_TINY    , FILL },
  {CN_XF_PROLOG_X_SCAN_TO_FROM,        sizeof(XF_Prolog_X_scan_To_from),        HT_T_TINY    , FILL },
  {CN_XF_SYNTHESISE_CODE,              sizeof(XF_Synthesise_code),              HT_T_TINY    , FILL },
  {CN_XF_SYNTHESISE_PIPELINEDEF,       sizeof(XF_Synthesise_pipelineDef),       HT_T_TINY    , FILL },

  {CN__Y__,                    'Y' - ('A' - 1),                                 HT_INVALID   , FILL },

  {CN__Z__,                    'Z' - ('A' - 1),                                 HT_INVALID   , FILL },

  {CN_ENUMS,                           C_VARGS_END,                             HT_INVALID   , FILL }
};

#undef FILL

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*

 MASTER as at 27th June 2023

Transient Tiny Heap 1KB Object Summary:
CRWTABLEPTN 12     (size      64B sum   768B use  6%)  *half again!
DECKSPEC 13     (size      96B sum    1KB use  9%)  *half again!
DIC 383    (size     336B sum  125KB use 32%)  *half again!
EVENTHANDLERS 41     (size     608B sum   24KB use 59%)
EVMGR 41     (size     632B sum   25KB use 61%)
FILE 4      (size     496B sum    1KB use 48%)  *half again!
GRID 1      (size     376B sum   376B use 36%)  *half again!
INDEXTREEVEC 12     (size     160B sum    1KB use 15%)  *half again!
IDXVEC 55     (size     160B sum    8KB use 15%)  *half again!
INPUT 1      (size     192B sum   192B use 18%)  *half again!
INTERNALTABLES 1      (size     144B sum   144B use 14%)  *half again!
INTSTACK 1      (size      32B sum    32B use  3%)  *half again!
JOBLIST 6      (size     232B sum    1KB use 22%)  *half again!
JOBS 1      (size      72B sum    72B use  7%)  *half again!
LISTEXONODE_t 1167   (size      48B sum   54KB use  4%)  *half again!
MARKER 39     (size     104B sum    3KB use 10%)  *half again!
MARKERCHAIN 13     (size     240B sum    3KB use 23%)  *half again!
MARKERMGR 1      (size     344B sum   344B use 33%)  *half again!
MUTEX 14     (size     528B sum    7KB use 51%)
NODE 1      (size     736B sum   736B use 71%)
NODE_I 54     (size      48B sum    2KB use  4%)  *half again!
NOW 54     (size      72B sum    3KB use  7%)  *half again!
OUTPUT 1      (size     464B sum   464B use 45%)  *half again!
OP 41     (size     520B sum   20KB use 50%)
OPERANDQUEUE 123    (size     256B sum   30KB use 25%)  *half again!
PIPEIN 41     (size      48B sum    1KB use  4%)  *half again!
PIPELINE 13     (size     448B sum    5KB use 43%)  *half again!
PLANNODEBUILDER 585    (size     968B sum  553KB use 94%)
PLANNODEBUILDERLIST 370    (size     232B sum   83KB use 22%)  *half again!
PLANNODELIST 377    (size     232B sum   85KB use 22%)  *half again!
PSXTHD 22     (size     376B sum    8KB use 36%)  *half again!
QUEUE 1      (size     232B sum   232B use 22%)  *half again!
REJECTJOB 16     (size      48B sum   768B use  4%)  *half again!
SCHEDULE 1      (size     320B sum   320B use 31%)  *half again!
SENCOMPILER 54     (size     192B sum   10KB use 18%)  *half again!
SKIPLIST 393    (size      88B sum   33KB use  8%)  *half again!
SKIPNODE 5459   (size      48B sum  255KB use  4%)  *half again!
SOCKET 2      (size     856B sum    1KB use 83%)
STAGESPEC 13     (size     224B sum    2KB use 21%)  *half again!
STARTER 1      (size     248B sum   248B use 24%)  *half again!
STATS 41     (size     240B sum    9KB use 23%)  *half again!
TABLE 55     (size     552B sum   29KB use 53%)
TABLEPTN 12     (size     256B sum    3KB use 25%)  *half again!
TABLEPTNVEC 55     (size     544B sum   29KB use 53%)
TASKLIST 6      (size     272B sum    1KB use 26%)  *half again!
TASKQUEUE 1      (size     224B sum   224B use 21%)  *half again!
TEXT 342    (size      40B sum   13KB use  3%)  *half again!
TEXTTINY 342    (size     992B sum  331KB use 96%)
THREAD 22     (size     720B sum   15KB use 70%)
THREADQUEUE 1      (size     264B sum   264B use 25%)  *half again!
THREADSCRATCH 22     (size     216B sum    4KB use 21%)  *half again!
TUB 1      (size      56B sum    56B use  5%)  *half again!
TXSET 2      (size     280B sum   560B use 27%)  *half again!
VERTEXSPEC 13     (size     192B sum    2KB use 18%)  *half again!
WORD 5093   (size     320B sum    1MB use 31%)  *half again!
[15436 objects ; 15MB of blocks]

Transient Small Heap 4KB Object Summary:
DECK 13     (size    2112B sum   26KB use 51%)
OBJECTSNAPSHOT 2      (size    3288B sum    6KB use 80%)
OPERANDWATCH 41     (size    1784B sum   71KB use 43%)
STAGE 13     (size    2120B sum   26KB use 51%)
STREAM 13     (size    2216B sum   28KB use 54%)
TASK 54     (size    3360B sum  177KB use 82%)
VERTEX 13     (size    2128B sum   27KB use 51%)
[149 objects ; 596KB of blocks]

Transient Medium Heap 64KB Object Summary:
CODEBLOCK 5038   (size   65496B sum  314MB use 99%)
JOB 14     (size   43360B sum  592KB use 66%)
LOCALSTRS 54     (size   65320B sum    3MB use 99%)
LOCALVARS 54     (size   65416B sum    3MB use 99%)
LOGICUNIT 54     (size   26032B sum    1MB use 39%)
NUMBERING 1      (size   13048B sum   12KB use 19%)
OPSPEC 41     (size   38552B sum    1MB use 58%)
PTNDEF 55     (size   51880B sum    2MB use 79%)
SEDEPUTY 1      (size    4144B sum    4KB use  6%)
SEPROMPT 1      (size   46752B sum   45KB use 71%)
STACK 162    (size   65368B sum   10MB use 99%)
TAGSET 596    (size   38216B sum   21MB use 58%)
THREADVEC 2      (size   33000B sum   64KB use 50%)
TOTUP 6      (size    6536B sum   38KB use  9%)
[6079 objects ; 379MB of blocks]

Transient Large Heap 1020KB Object Summary:
CHARSER 4      (size   96728B sum  377KB use  9%)
LGLPTNMAP 1      (size   65776B sum   64KB use  6%)
PORT 1      (size     728B sum   728B use  0%)  *into medium
STREAMDEF 55     (size   84776B sum    4MB use  8%)
TOKENVEC 55     (size  403552B sum   21MB use 38%)
[116 objects ; 115MB of blocks]

*/
