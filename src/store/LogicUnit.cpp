/*============================================================================*/
/* LogicUnit.cpp -- Logic Unit                                                */
/*============================================================================*/

#include "BASE.h"

#include "eED.h"
#include "eEO.h"
#include "eYX.h"
#include "eLR.h"

#include "RowSingle.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "TailLine.hpp"

#include "Crw.hpp"

#include "IndexDef.hpp"

#include "RowInflator.hpp"

#include "EnumGroups.h"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "Crv.hpp"
#include "CrvBuffer.hpp"
#include "CrwBuffer.hpp"
#include "TablePtn.hpp"
#include "RowOnce.hpp"
#include "IndexStream.hpp"

#include "Node.hpp"
#include "Node_I.hpp"
#include "OpSpec.hpp"
#include "Op.hpp"
#include "Job.hpp"

#include "LogicUnit.hpp"
#include "Pipeline.hpp"

#include "LIB_interface.h"
#include "DEV_interface.h"
#include "MathLib.h"

#include "Arg.hpp"
#include "RelOp.hpp"
#include "Predicate.hpp"
#include "PredicateVec.hpp"
#include "AccessPath.hpp"
#include "PathIdentifier.hpp"
#include "PtnStream.hpp"
#include "PlanOperand.hpp"
#include "Dynamo.hpp"

#include "SP_Dte.hpp"
#include "SP_Ent.hpp"

#include "SN_Dte.hpp"
#include "SN_Tme.hpp"
#include "SN_Tsp.hpp"

#include "Table.hpp"

#include "MY.hpp"


extern EoArg EoArgs[];

extern void* C_ADR_REGION;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,LogicUnit,CN_LOGICUNIT)
__aNewFunction(LogicUnit,CN_LOGICUNIT)
__vDeleteFunction(LogicUnit,CN_LOGICUNIT)
__aFooterFunction(LogicUnit)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LogicUnit::LogicUnit(void)
: Object(CN_LOGICUNIT)
, m_nRegs(0)

,      m_xpAdaptorWL(this,RF_REPAIR_DELETE,AdaptorWL::vRepair,      LR_ADAPTORWL)
,       m_xpAggTimer(this,RF_REPAIR_DELETE,AggTimer::vRepair,       LR_AGGTIMER)
,   m_xpAggTypeStack(this,RF_REPAIR_DELETE,IntStack::vRepair,       LR_AGGTYPESTACK)
,    m_xpPreExpStack(this,RF_REPAIR_DELETE,IntStack::vRepair,       LR_PREEXPSTACK)

,      m_xpBktMapGet(this,RF_REPAIR_DELETE,BktMapGet::vRepair,      LR_BKTMAP)
,         m_xpBucket(this,RF_REPAIR_DELETE,Bucket::vRepair,         LR_BUCKET)
,      m_xpBucketGet(this,RF_REPAIR_DELETE,BucketGet::vRepair,      LR_BUCKETGET)
,     m_xpBucketInfo(this,RF_REPAIR_DELETE,BucketInfo::vRepair,     LR_BUCKETINFO)

,       m_xpCombined(this,RF_REPAIR_DELETE,Combined::vRepair,       LR_COMBINED)
,      m_xpCrwBuffer(this,RF_REPAIR_DELETE,CrwBuffer::vRepairList,  LR_CRWBUFFER)
,       m_xpCrwToRow(this,RF_REPAIR_DELETE,CrwToRow::vRepair,       LR_CRWTOROW)
,      m_xpCsvBuffer(this,RF_REPAIR_DELETE,CsvBuffer::vRepairList,  LR_CSVBUFFER)

,     m_xpDataBuffer(this,RF_REPAIR_DELETE,DataBuffer::vRepair,     LR_DATABUFFER)
,           m_xpDir0(this,RF_REPAIR_DELETE,Dir::vRepair,            LR_DIR0)
,           m_xpDir1(this,RF_REPAIR_DELETE,Dir::vRepair,            LR_DIR1)
,         m_xpDynamo(this,RF_REPAIR_DELETE,Dynamo::vRepair,         LR_DYNAMO)

,    m_xpErrorMarker(this,RF_REPAIR_DELETE,Operand::vRepair,        LR_ERRORMARKER)
,       m_xpExprEval(this,RF_REPAIR_DELETE,ExprEval::vRepair,       LR_EXPEVAL)

,          m_xpFile0(this,RF_REPAIR_DELETE,File::vRepair,           LR_FILE0)
,          m_xpFile1(this,RF_REPAIR_DELETE,File::vRepair,           LR_FILE1)
,       m_xpFilter(this,RF_REPAIR_DELETE,  ExprEval::vRepair,       LR_FILTER)

,           m_xpGrid(this,RF_REPAIR_DELETE,Parcel::vRepair,         LR_GRID)

,        m_xpHandler(this,RF_REPAIR_NONE,  nullptr,                 LR_HANDLER)

,          m_xpIndex(this,RF_SET_NULL,     nullptr,                 LR_INDEX)
,       m_xpIndexDef(this,RF_SET_NULL,     nullptr,                 LR_INDEXDEF)
,    m_xpIndexStream(this,RF_REPAIR_DELETE,IndexStream::vRepair,    LR_INDEXSTREAM)
,        m_xpInfoSet(this,RF_REPAIR_DELETE,InfoSet::vRepair,        LR_INFOSET)
,     m_xpInfoSetRow(this,RF_SET_NULL,     nullptr,                 LR_INFOSETROW)
,   m_xpInfoSetTable(this,RF_SET_NULL,     nullptr,                 LR_INFOSETTABLE)
,       m_xpIntStack(this,RF_REPAIR_DELETE,IntStack::vRepair,       LR_INTSTACK)

,     m_xpJobMetrics(this,RF_REPAIR_DELETE,JobMetrics::vRepair,     LR_JOBMETRICS)

,           m_xpList(this,RF_REPAIR_DELETE,RowSingle::vRepairList,  LR_LIST)
,  m_xpLogicCompiler(this,RF_REPAIR_DELETE,LogicCompiler::vRepair,  LR_LOGICCOMPILER)

,         m_xpMarker(this,RF_SET_NULL,     nullptr,                 LR_MARKER)
,      m_xpMemoryHog(this,RF_REPAIR_DELETE,RowBuffer::vRepairList,  LR_MEMORYHOG)
,        m_xpMetaGet(this,RF_REPAIR_DELETE,MetaGet::vRepair,        LR_META)

,    m_xpNamedBuffer(this,RF_REPAIR_DELETE,NamedBuffer::vRepairList,LR_NAMEDBUFFER)
,           m_xpNode(this,RF_REPAIR_NEVER,  nullptr,                LR_NODE)
,         m_xpNode_I(this,RF_SET_NULL,     nullptr,                 LR_NODE_I)
,            m_xpNow(this,RF_REPAIR_DELETE,Now::vRepair,            LR_NOW)

,             m_xpOp(this,RF_NONE,         nullptr,                 LR_OP)
,     m_xpOperandOoO(this,RF_REPAIR_NONE,  nullptr,                 LR_OPERANDOOO)
,      m_xpOptimiser(this,RF_REPAIR_DELETE,  nullptr,               LR_OPTIMISER)

, m_xpPathIdentifier(this,RF_REPAIR_DELETE,PathIdentifier::vRepair, LR_PATHIDENTIFIER)
,         m_xpParcel(this,RF_REPAIR_DELETE,Parcel::vRepair,         LR_PARCEL)
,         m_xpPipein(this,RF_REPAIR_NONE,  nullptr,                 LR_PIPEIN)
,       m_xpPipeline(this,RF_NONE,         nullptr,                 LR_PIPELINE)
,       m_xpPipeSpec(this,RF_NONE,         nullptr,                 LR_PIPESPEC)
,    m_xpPlanOperand(this,RF_REPAIR_DELETE,PlanOperand::vRepair,    LR_PLANOPERAND)
,            m_xpPot(this,RF_REPAIR_DELETE,Pot::vRepair,            LR_POT)
,      m_xpPtnStream(this,RF_REPAIR_DELETE,PtnStream::vRepair,      LR_PTNSTREAM)

,    m_xpRecvdObject(this,RF_REPAIR_NONE,  nullptr,                 LR_RECVDOBJECT)
,      m_xpRowBuffer(this,RF_REPAIR_DELETE,RowBuffer::vRepairList,  LR_ROWBUFFER)
,        m_xpRowOnce(this,RF_REPAIR_DELETE,RowOnce::vRepair,        LR_ROWONCE)

,       m_xpSendNode(this,RF_SET_NULL,     nullptr,                 LR_SENDNODE)
,          m_xpSends(this,RF_REPAIR_NONE,  nullptr,                 LR_SENDS)
,       m_xpShardDef(this,RF_REPAIR_DELETE,ShardDef::vRepair,       LR_SHARDDEF)
,         m_xpSocket(this,RF_REPAIR_NEVER, nullptr,                 LR_SOCKET)
,          m_xpStage(this,RF_SET_NULL,     nullptr,                 LR_STAGE)
,          m_xpStats(this,RF_REPAIR_DELETE,nullptr,                 LR_STATS)
,        m_xpStencil(this,RF_REPAIR_DELETE,Stencil::vRepair,        LR_STENCIL)
,         m_xpStream(this,RF_SET_NULL,     nullptr,                 LR_STAGE)
,   m_xpStreamDefLft(this,RF_REPAIR_DELETE,StreamDef::vRepair,      LR_STREAMDEFLFT)
,   m_xpStreamDefOut(this,RF_REPAIR_DELETE,StreamDef::vRepair,      LR_STREAMDEFOUT)
,   m_xpStreamDefRht(this,RF_REPAIR_DELETE,StreamDefRht::vRepair,   LR_STREAMDEFRHT)
,      m_xpStreamSet(this,RF_REPAIR_DELETE,StreamDef::vRepair,      LR_STREAMSET)
, m_xpStreamSetTable(this,RF_SET_NULL,     nullptr,                 LR_STREAMSETTABLE)
,        m_xpSubCall(this,RF_REPAIR_DELETE,SubCall::vRepair,        LR_SUBCALL)
,        m_xpSubPipe(this,RF_NONE,         nullptr,                 LR_SUBPIPE)
,      m_xpSubReturn(this,RF_REPAIR_DELETE,SubReturn::vRepair,      LR_SUBRETURN)
,         m_xpSubset(this,RF_REPAIR_DELETE,Operand::vRepairList,    LR_SUBSET)

,          m_xpTable(this,RF_SET_NULL,     nullptr,                 LR_TABLE)
,       m_xpTablePtn(this,RF_SET_NULL,     nullptr,                 LR_TABLEPTN)
,         m_xpTagSet(this,RF_REPAIR_DELETE,TagSet::vRepair,         LR_TAGSET)
,          m_xpTimer(this,RF_REPAIR_DELETE,Timer::vRepair,          LR_TIMER)
,          m_xpTinyC(this,RF_REPAIR_DELETE,TinyC::vRepair,          LR_TINYC)
,       m_xpToBktCrw(this,RF_REPAIR_DELETE,ToBktCrw::vRepairHashSet,LR_TOBKTCRW)
,       m_xpToBktRow(this,RF_REPAIR_DELETE,ToBktRow::vRepairHashSet,LR_TOBKTROW)
,       m_xpToNstRow(this,RF_REPAIR_DELETE,ToNstRow::vRepair,       LR_TONSTROW)
,       m_xpToPtnCrw(this,RF_REPAIR_DELETE,ToPtnCrw::vRepairHashSet,LR_TOPTNCRW)
,       m_xpToPtnRow(this,RF_REPAIR_DELETE,ToPtnRow::vRepairHashSet,LR_TOPTNROW)
,          m_xpToRow(this,RF_REPAIR_DELETE,ToRow::vRepairList,      LR_TOROW)
,            m_xpTub(this,RF_REPAIR_DELETE,Tub::vRepair,            LR_TUB)
,             m_xpTx(this,RF_REPAIR_DELETE,ToRow::vRepair,          LR_TX)

{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  m_aTinyFunc = nullptr;
  
  m_pJob  = nullptr;
  m_pOp   = nullptr;

  m_eDdlLocked = LH_HAVE_NO_LOCK;
  m_i_NextRnd  = AM::bDeveloper() ? 0 : C_UNKNOWN;
  m_xpSendNode  = nullptr;
  
  m_xpHandler  = nullptr;

  m_iDebugFlag = 0;

  m_xpNow = new Now;

  m_bCompilerToExit = false;
  m_bIsLftCrwBuffer = false;

  m_eExprKind = EK_NONE;

  m_iCounterA = 0;
  m_iCounter0 = 0;
  m_iCounter1 = 0;
  m_iCounter2 = 0;
  m_iCounter3 = 0;
  m_iCounter4 = 0;
  m_iCounter5 = 0;
  m_iCounter6 = 0;
  m_iCounter7 = 0;
  
  m_afFun0 = nullptr;
  m_afFun1 = nullptr;
  m_afFun2 = nullptr;
  m_afFun3 = nullptr;

  m_sStringA  = ""_Stz;
  m_sString0  = ""_Stz;
  m_sString1  = ""_Stz;
  m_sString2  = ""_Stz;
  m_sString3  = ""_Stz;
  m_sString5  = ""_Stz;
  m_sString6  = ""_Stz;
  m_sString7  = ""_Stz;

  m_xRluCommon.m_iCtr0 = 0;

  for (i8 i_Local = 0; i_Local < C_LOGIC_LOCALS; i_Local++)
    m_xRluCommon.m_iaLocals[i_Local] = C_UNKNOWN;

  for (i8 i_Byte = 0; i_Byte < C_ROWS_PERBUFFER; i_Byte++)
    m_xRluCommon.m_baJoinMap[i_Byte] = 0;
  m_xRluCommon.m_nJoinMapCnt = 0;

  for (i8 i_Cond = 0; i_Cond < C_LOGIC_COND; i_Cond++)
    m_baCondInd[i_Cond] = false;

  m_iNextCond = C_UNKNOWN;
  m_bSuperFrag = false;
  m_aLowerEntry = nullptr;
  m_aUpperEntry = nullptr;
  m_pEntBufferOuter = nullptr;

  m_ePipeInOperand = ZK_NONE;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_afDynFun = nullptr;
  
  m_pTknVec = new TokenVec;
  m_pTkn    = nullptr;
  m_i_Tkn   = C_UNKNOWN;
  
  m_pRluDiary = nullptr;
  
  MemLib::vZero(&m_xRluCommon,sizeof(m_xRluCommon),CN_LOGICUNIT);
  m_xRluCommon.m_iCmnMagic  = 123456789;
  m_xRluCommon.m_iCmnFooter = 987654321;
  
  m_xRluCommon.m_nAltHeadSize = C_CRW_HEADBYTES;
  m_xRluCommon.m_nSrcHeadSize = C_CRW_HEADBYTES;
  m_xRluCommon.m_i_ColSet = 0;

  m_oSince.vStart();
  
  // m_oEntWrk

  m_uSelectColumns = 0;

  m_i_Code = 0;


  m_pDevDstRow = new DevROW;
  m_pDevLftRow = new DevROW;
  m_pDevRhtRow = new DevROW;
  m_pDevAltRow = new DevROW;

  m_pDevLftIxt = new DevIXT;
  m_pDevRhtIxt = new DevIXT;
  m_pDevAltIxt = new DevIXT;
  m_pDevTmpIxt = new DevIXT;

  m_pLogicTimer = nullptr;

  m_xRluCommon.m_pDevDstRow = m_pDevDstRow;
  
  m_xRluCommon.m_pDevLftRow = m_pDevLftRow;
  m_xRluCommon.m_pDevRhtRow = m_pDevRhtRow;
  m_xRluCommon.m_pDevLftIxt = m_pDevLftIxt;
  m_xRluCommon.m_pDevRhtIxt = m_pDevRhtIxt;
  m_xRluCommon.m_pDevAltRow = m_pDevAltRow;
  m_xRluCommon.m_pDevAltIxt = m_pDevAltIxt;
  m_xRluCommon.m_pDevTmpIxt = m_pDevTmpIxt;
  
  m_xRluCommon.m_aWrkEnt = (Ent_int*)m_oEntWrk.aEnt();
  m_xRluCommon.m_pEntWrk = (void*)&m_oEntWrk;
  m_xRluCommon.m_iJoinedRow = 0;

  m_xRluCommon.m_pRowPix = (void*)&m_oRowPix;

  m_xRluCommon.m_i_TablePtn = C_UNKNOWN;
  m_xRluCommon.m_nPtns  = AM::PTNMAP::nPtnsInGrid();
  m_xRluCommon.m_iHash  = 0;

  m_xRluCommon.m_pPot = nullptr;
  m_xRluCommon.m_pIndexDef = nullptr;
  m_xRluCommon.m_pIndexTree = nullptr;

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_eaCode,sizeof(m_eaCode),CN_LOGICUNIT);
#endif
  
  m_nLabels = 0;
  for (i8 i_Label = 0; i_Label < C_LOGIC_LABEL; i_Label++) {

    // forced execution of constructor of s32 needed because s32 is a member of a struct
    // for tinyc and struct doesn't invoke the constructor!!!
    s32* s = new (&m_xaJumpLabel[i_Label].m_sLabelNm) s32;
    IGNORE_RETURN(s);

    m_xaJumpLabel[i_Label].m_bDefined = false;
    m_xaJumpLabel[i_Label].m_bSubroutine = false;
    m_xaJumpLabel[i_Label].m_bDestination = false;
    m_xaJumpLabel[i_Label].m_i_Chain = C_UNKNOWN;
  }

  // m_ixRowsProcessed;
  // m_ixRowsInvalid;
  // m_ixRowsSkipped;
  m_eErrorCode_reg = EC_NONE;
  
  m_eSendKind = XK_NONE;
  
  m_nPotSize = 0;
  
  for (i8 i_UnusedReg = m_nRegs ; i_UnusedReg < C_MAX_REGS; i_UnusedReg++)
    m_paReg[i_UnusedReg] = nullptr;
  
  _checkInitialised_(CN_LOGICUNIT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LogicUnit::~LogicUnit(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  RASSERT(! m_aTinyFunc);

  AM::MASTERLOCK::vDdlLockTestDrop(this);

  RASSERT(LH_HAVE_NO_LOCK == m_eDdlLocked);

  // registers should have been repaired
  RASSERT(m_xpAggTimer.bIsNull());
  RASSERT(m_xpBktMapGet.bIsNull());
  RASSERT(m_xpBucket.bIsNull());
  RASSERT(m_xpBucketGet.bIsNull());

  RASSERT(m_xpCrwBuffer.bIsNull());
  RASSERT(m_xpCrwToRow.bIsNull());
  RASSERT(m_xpCsvBuffer.bIsNull());

  RASSERT(m_xpDataBuffer.bIsNull());
  RASSERT(m_xpDir0.bIsNull());
  RASSERT(m_xpDir1.bIsNull());

  RASSERT(m_xpErrorMarker.bIsNull());
  RASSERT(m_xpExprEval.bIsNull());

  RASSERT(m_xpFile0.bIsNull());
  RASSERT(m_xpFile1.bIsNull());
  RASSERT(m_xpFilter.bIsNull());
  RASSERT(m_xpGrid.bIsNull());

  RASSERT(m_xpIndex.bIsNull());
  RASSERT(m_xpIndexDef.bIsNull());
  RASSERT(m_xpIndexStream.bIsNull());
  RASSERT(m_xpInfoSet.bIsNull());
  RASSERT(m_xpInfoSetRow.bIsNull());
  RASSERT(m_xpInfoSetTable.bIsNull());

  RASSERT(m_xpList.bIsNull());
  RASSERT(m_xpLogicCompiler.bIsNull());

  //RASSERT(m_xpMarker.bIsNull());
  RASSERT(m_xpMemoryHog.bIsNull());
  RASSERT(m_xpMetaGet.bIsNull());

  RASSERT(m_xpNamedBuffer.bIsNull());
  RASSERT(m_xpNode.bIsNull());
  RASSERT(m_xpNode_I.bIsNull());

  RASSERT(m_xpOperandOoO.bIsNull());

  RASSERT(m_xpParcel.bIsNull());
  RASSERT(m_xpPathIdentifier.bIsNull());
  RASSERT(m_xpPipein.bIsNull());
  RASSERT(m_xpPlanOperand.bIsNull());
  RASSERT(m_xpPot.bIsNull());
  RASSERT(m_xpPtnStream.bIsNull());

  RASSERT(m_xpRecvdObject.bIsNull());
  RASSERT(m_xpRowOnce.bIsNull());
  RASSERT(m_xpRowBuffer.bIsNull());

  RASSERT(m_xpSendNode.bIsNull());
  RASSERT(m_xpSends.bIsNull());
  RASSERT(m_xpShardDef.bIsNull());
  RASSERT(m_xpSocket.bIsNull());
  //RASSERT(m_xpStats.bIsNull());
  RASSERT(m_xpStreamDefLft.bIsNull());
  RASSERT(m_xpStreamDefRht.bIsNull());
  RASSERT(m_xpStreamDefOut.bIsNull());
  RASSERT(m_xpStreamSet.bIsNull());
  RASSERT(m_xpStreamSetTable.bIsNull());
  RASSERT(m_xpSubset.bIsNull());

  RASSERT(m_xpTable.bIsNull());
  RASSERT(m_xpTablePtn.bIsNull());
  RASSERT(m_xpTimer.bIsNull());
  RASSERT(m_xpToBktCrw.bIsNull());
  RASSERT(m_xpToBktRow.bIsNull());
  RASSERT(m_xpToNstRow.bIsNull());
  RASSERT(m_xpToPtnCrw.bIsNull());
  RASSERT(m_xpToPtnRow.bIsNull());
  RASSERT(m_xpToRow.bIsNull());
  RASSERT(m_xpTx.bIsNull());
  
  if (! m_xpNow.bIsNull())
    delete *m_xpNow;

  // repair the stats here
  Stats::vRepair(m_xpStats.m_pComponent);

  // PROBLEM
  if (! m_xpPipeline.bIsNull()) {
    
    Pipeline* pPipeline = (* m_xpPipeline);
    RASSERT(pPipeline);
    RASSERT(CN_PIPELINE == pPipeline->eGetClassNo());
    
    m_xpPipeline.vGuardedDeleteComponent(CN_PIPELINE,
                                         CN_LOGICUNIT,this,(Stz*)__func__);
  }
  if (! m_xpSubPipe.bIsNull()) {
    
    Pipeline* pSubPipeline = (* m_xpSubPipe);
    RASSERT(pSubPipeline);
    RASSERT(CN_PIPELINE == pSubPipeline->eGetClassNo());
    
    m_xpSubPipe.vGuardedDeleteComponent(CN_PIPELINE,
                                         CN_LOGICUNIT,this,(Stz*)__func__);
  }

  if (m_pRluDiary) {
    vDiaryReport();
    delete m_pRluDiary;
  }
  
  delete m_pTknVec;
  
  delete m_pDevDstRow;
  delete m_pDevLftRow;
  delete m_pDevRhtRow;
  delete m_pDevAltRow;

  // delete any temporary index tree's
  if (m_pDevTmpIxt->m_pIdxTree)
    delete m_pDevTmpIxt->m_pIdxTree;

  delete m_pDevTmpIxt;
  delete m_pDevAltIxt;
  delete m_pDevLftIxt;
  delete m_pDevRhtIxt;
  
  delete m_pLogicTimer;

//  for (i8 i_Reg = 0 ; i_Reg < m_nRegs; i_Reg++)
//    m_paReg[i_Reg]->vDump();

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* content dump                                                               */
/*----------------------------------------------------------------------------*/
void LogicUnit::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  vTrace("\n"_Stz);
  vTrace("LogicUnit\n"_Stz);
  vTrace("------\n"_Stz);

  vTrace("%p:\n"
         "m_i_NextRnd=%lld\n"
         "m_xpSendNode=%p %s\n"_Stz,

         this,
         
         m_i_NextRnd,
         
         *m_xpSendNode,
         m_xpSendNode.bIsNotNull() ? m_xpSendNode->szNodeNm() : ""_Stz,
         
         C_VARGS_END
         );

}

/*----------------------------------------------------------------------------*/
/* transform compute                                                          */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iTransform(i8 nArgs, i8 iArg0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  i8 iResult = iCompute(nArgs,iArg0);

  return iResult;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add reg                                                                    */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddReg(REG<Object,CN_NONE>* pReg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT);

  m_paReg[m_nRegs++] = pReg;
}

/*----------------------------------------------------------------------------*/
/* conditional code                                                           */
/*----------------------------------------------------------------------------*/
void LogicUnit::vCC(i8 i_Cond)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  if (m_baCondInd[i_Cond])
    m_iNextCond = 1;
  else
    m_iNextCond = 0;
}

/*----------------------------------------------------------------------------*/
/* cleanup                                                                    */
/*----------------------------------------------------------------------------*/
void LogicUnit::vCleanup(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  if (m_aTinyFunc) {
    free(m_aTinyFunc);
    m_aTinyFunc = nullptr;
  }
  
  for (i8 i_Reg = 0 ; i_Reg < m_nRegs; i_Reg++) {
    REG<Object,CN_NONE> *p_Reg = m_paReg[i_Reg];

    if (AM::SYSARGS::pTagSet()->bHasName("<show_op_report>"_Stz)) {

      if (p_Reg->m_eLogicUnitReg == LR_STATS) {

        s32 sBytesAlloc;
        MagLib::vByteMag(m_xpStats->m_nBytesAlloc, sBytesAlloc);

        s32 sBytesFree;
        MagLib::vByteMag(m_xpStats->m_nFrees, sBytesFree);

        s32 sMaxBytes;
        MagLib::vByteMag(m_xpStats->m_nMaxBytes, sMaxBytes);

        s32 sBytesRead;
        MagLib::vByteMag(m_xpStats->m_nBytesRead, sBytesRead);

        s32 sBytesWrite;
        MagLib::vByteMag(m_xpStats->m_nBytesWrite, sBytesWrite);

        vTrace("%10s %16s (P%02lld) : "
               "cpu %8lldus "
               "maxq %6lld "
               "operands %6lld [buffIn %6lld buffOut %6lld] "
               "NewRowBuffers %6lld [DelRowBuffers %6lld] "
               "NewCsvBuffers %6lld [DelCsvBuffers %6lld] "
               "Allocs %6lld [BytesAlloc %6s] "
               "Frees %6lld [BytesFree %6s] "
               "MaxBytes %6s "
               "Reads %6lld [BytesRead %6s] "
               "Writes %6lld [BytesWrite %6s] "
               "ParcelSends %6lld [ObjectsSent %6lld] "
               "ParcelRecvs %6lld [ObjectsRecv %6lld] "
               "\n"_Stz

               ,m_pOp->szStageName()
               ,m_pOp->szOpNm()
               ,m_pOp->tPtnId().iAsI8()
               ,m_xpStats->iMicroseconds()
               ,m_xpStats->m_nMaxQs
               ,m_xpStats->m_nOperands
               ,m_xpStats->m_nBuffIn
               ,m_xpStats->m_nBuffOut
               ,m_xpStats->m_nNewRowBuffer
               ,m_xpStats->m_nDelRowBuffer
               ,m_xpStats->m_nNewCsvBuffer
               ,m_xpStats->m_nDelCsvBuffer
               ,m_xpStats->m_nAllocs
               ,sBytesAlloc.azChar()
               ,m_xpStats->m_nFrees
               ,sBytesFree.azChar()
               ,sMaxBytes.azChar()
               ,m_xpStats->m_nReads
               ,sBytesRead.azChar()
               ,m_xpStats->m_nWrites
               ,sBytesWrite.azChar()
               ,m_xpStats->m_nParcelSends
               ,m_xpStats->m_nObjectsSent
               ,m_xpStats->m_nParcelRecvs
               ,m_xpStats->m_nObjectsRecv
               );
        }
    }
    
    // execute register repair
    if (p_Reg->m_eLogicUnitReg != LR_STATS)
      p_Reg->vRepair();
  }
}
/*----------------------------------------------------------------------------*/
/* get eCode at given i_Code                                                  */
/*----------------------------------------------------------------------------*/
eEO LogicUnit::eCode(i8 i_Code)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  eEO eCode = *(eEO*)&m_eaCode[i_Code];

  return eCode;
}

/*----------------------------------------------------------------------------*/
/* return number of eo codes                                                  */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::nCodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  return m_i_Code;
}

/*----------------------------------------------------------------------------*/
/* get iArg at given i_Code                                                   */
/*----------------------------------------------------------------------------*/
char* LogicUnit::aCodeArg(i8 i_Code)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  char* aArg = &m_eaCode[i_Code];

  return aArg;
}

/*----------------------------------------------------------------------------*/
/* get iArg at given i_Code                                                   */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iCodeArg(i8 i_Code)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  i8 iArg = * (i8*)&m_eaCode[i_Code];

  return iArg;
}

/*----------------------------------------------------------------------------*/
/* get diary                                                                  */
/*----------------------------------------------------------------------------*/
RluDiary* LogicUnit::pDiary(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  return m_pRluDiary;
}

/*----------------------------------------------------------------------------*/
/* set diary name                                                             */
/*----------------------------------------------------------------------------*/
void LogicUnit::vDiaryName(Stz *szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  if (!m_pRluDiary)
    m_pRluDiary = new RluDiary;

  
  m_pRluDiary->vDiaryNm(szName);
}

/*----------------------------------------------------------------------------*/
/* set diary job note                                                         */
/*----------------------------------------------------------------------------*/
void LogicUnit::vDiaryJobNote(Stz *szJobNote)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  if (!m_pRluDiary)
    m_pRluDiary = new RluDiary;

  
  m_pRluDiary->vJobNote(szJobNote);
}

/*----------------------------------------------------------------------------*/
/* set diary dir                                                              */
/*----------------------------------------------------------------------------*/
void LogicUnit::vDiaryDir(Stz *szDir)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  if (!m_pRluDiary)
    m_pRluDiary = new RluDiary;

  
  m_pRluDiary->vDiaryDir(szDir);
}

/*----------------------------------------------------------------------------*/
/* add diary note                                                             */
/*----------------------------------------------------------------------------*/
void LogicUnit::vDiaryNote(i8 i_EnumGroup, i8 i_Enum, i8 iItem, i8 nNanoSecs,
                           i2 iAction, i2 i_ActEnumGroup, i2 i_ActEnum,
                           i8 nQueueLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  if (!m_pRluDiary)
    m_pRluDiary = new RluDiary;

  
  m_pRluDiary->vAddNote(m_xRluCommon.m_iSql,m_xRluCommon.m_i_TablePtn,
                        nQueueLength,
                        
                        i_EnumGroup,i_Enum,iItem,nNanoSecs,
                        iAction,i_ActEnumGroup,i_ActEnum);
}

/*----------------------------------------------------------------------------*/
/* set diary title                                                            */
/*----------------------------------------------------------------------------*/
void LogicUnit::vDiaryTitle(Stz *szTitle)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  if (!m_pRluDiary)
    m_pRluDiary = new RluDiary;

  
  m_pRluDiary->vTitle(szTitle);
}

/*----------------------------------------------------------------------------*/
/* call dynamic function compiled by LogicCompiler                            */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iDynFun(i8 nOuterArgs, i8 iOuterArg0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  i8 iRes = m_afDynFun(this,nOuterArgs,iOuterArg0);

  return iRes;
}

/*----------------------------------------------------------------------------*/
/* dump the logicUnit registers                                               */
/*----------------------------------------------------------------------------*/
void LogicUnit::vDumpRegisters(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  vTrace("\n------------------------------------------------------------\n"_Stz);
  vTrace("LogicUnit RegisterDump\n"_Stz);

  for (i8 i_Reg = 0 ; i_Reg < m_nRegs; i_Reg++)
    m_paReg[i_Reg]->vDump();

  vTrace("------------------------------------------------------------\n\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* optimise with super fragent                                                */
/*----------------------------------------------------------------------------*/
void LogicUnit::vEndOptimise(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  m_bSuperFrag = false;
}

/*----------------------------------------------------------------------------*/
/* diarise dynamic routine entry                                              */
/*----------------------------------------------------------------------------*/
void LogicUnit::vEnter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  if ( AM::bRLUDiary() ) {
    m_oSince.vUpdate();
    i8 nNanoSeconds = m_oSince.iNanoseconds();
  
  //  vTrace("Enter (%lld.%lld) %s %lld %lld\n"_Stz,
  //         m_xRluCommon.m_iSql,
  //         m_xRluCommon.m_i_Ptn,
  //
  //         EnumGroups::szEnumText(EG_ED,ED_ENTER),
  //
  //         m_xRluCommon.m_nEnterCount,
  //         nNanoSeconds);
  
    vDiaryNote(EG_ED,ED_ENTER,
                m_xRluCommon.m_nEnterCount,
                nNanoSeconds,
                0,0,0,-1);
  }
}

/*----------------------------------------------------------------------------*/
/* diarise dynamic routine exit                                               */
/*----------------------------------------------------------------------------*/
void LogicUnit::vExit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  if ( AM::bRLUDiary() ) {
    m_oSince.vUpdate();
    i8 nNanoSeconds = m_oSince.iNanoseconds();
    
  //  vTrace("Exit (%lld.%lld) %s %lld %lld\n"_Stz,
  //         m_xRluCommon.m_iSql,
  //         m_xRluCommon.m_i_Ptn,
  //
  //         EnumGroups::szEnumText(EG_ED,ED_EXIT),
  //
  //         m_xRluCommon.m_nEnterCount,
  //         nNanoSeconds);

    vDiaryNote(EG_ED,ED_EXIT,
               m_xRluCommon.m_nEnterCount,
               nNanoSeconds,
               0,0,0,-1);
  }
}

/*----------------------------------------------------------------------------*/
/* failure processing                                                         */
/*----------------------------------------------------------------------------*/
void LogicUnit::vFailure(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  for (i8 i_Reg = 0 ; i_Reg < m_nRegs; i_Reg++) {
    REG<Object,CN_NONE>* p_Reg = m_paReg[i_Reg];
    
    p_Reg->vRepair();
  }
}

/*----------------------------------------------------------------------------*/
/* find label which is at given i_Code                                        */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iFindLabel(i8 i_Code)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  for (i8 i_Label = 0; i_Label < m_nLabels; i_Label++)
    if (i_Code == m_xaJumpLabel[i_Label].m_i_Code)
      return i_Label;

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* find label by name and return i_Label                                      */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iFindLabel(Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  for (i8 i_Label = 0; i_Label < m_nLabels; i_Label++)
    if (m_xaJumpLabel[i_Label].m_sLabelNm.bSame(szLabelNm))
      return i_Label;

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void LogicUnit::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  m_xRluCommon.m_i_TablePtn = MY::pOp()->tPtnId().iAsI8();
}

/*----------------------------------------------------------------------------*/
/* get job                                                                    */
/*----------------------------------------------------------------------------*/
Job* LogicUnit::pJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  return m_pJob;
}

/*----------------------------------------------------------------------------*/
/* set job                                                                    */
/*----------------------------------------------------------------------------*/
void LogicUnit::vJob(Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
    
  m_pJob = pJob;
}

/*----------------------------------------------------------------------------*/
/* get code point of label                                                    */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iLabelCode(i8 i_Label)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  i8 i_Code = m_xaJumpLabel[i_Label].m_i_Code;
  
  return i_Code;
}

/*----------------------------------------------------------------------------*/
/* is given label a destination?                                              */
/*----------------------------------------------------------------------------*/
bool LogicUnit::bLabelIsDst(i8 i_Label)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  bool bIsDst = m_xaJumpLabel[i_Label].m_bDestination;
  
  return bIsDst;
}

/*----------------------------------------------------------------------------*/
/* is given label a subroutine?                                               */
/*----------------------------------------------------------------------------*/
bool LogicUnit::bLabelIsSbr(i8 i_Label)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  bool bIsSbr = m_xaJumpLabel[i_Label].m_bSubroutine;
  
  return bIsSbr;
}

/*----------------------------------------------------------------------------*/
/* return number of labels                                                    */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::nLabels(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  return m_nLabels;
}

/*----------------------------------------------------------------------------*/
/* return number of labels                                                    */
/*----------------------------------------------------------------------------*/
Stz* LogicUnit::szLabelNm(i8 i_Label)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  Stz* szLblNm = m_xaJumpLabel[i_Label].m_sLabelNm;
  
  return szLblNm;
}

/*----------------------------------------------------------------------------*/
/* get op                                                                     */
/*----------------------------------------------------------------------------*/
Op* LogicUnit::pOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  RASSERT(! m_pJob);

  return m_pOp;
}

/*----------------------------------------------------------------------------*/
/* set op                                                                     */
/*----------------------------------------------------------------------------*/
void LogicUnit::vOp(Op* pOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  RASSERT(! m_pJob);
  
  m_pOp = pOp;
}

/*----------------------------------------------------------------------------*/
/* optimise with super fragent                                                */
/*----------------------------------------------------------------------------*/
void LogicUnit::vOptimise(eEO eOpt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  if (EO_NONE == eOpt) {
    m_bSuperFrag = false;
    return;
  }
  
  vAddCode(eOpt);
  
  m_bSuperFrag = true;
}

/*----------------------------------------------------------------------------*/
/* return query fragment number                                               */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iQueryFragment(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  return m_xRluCommon.m_iSql;
}

/*----------------------------------------------------------------------------*/
/* set conditional compilation state                                          */
/*----------------------------------------------------------------------------*/
void LogicUnit::vSetCC(i8 i_Cond, bool bState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  if (bState)
    m_baCondInd[i_Cond] = 1;
  else
    m_baCondInd[i_Cond] = 0;
}

/*----------------------------------------------------------------------------*/
/* set m_xpPipein and decode into m_ePipeInOperand                            */
/*----------------------------------------------------------------------------*/
void LogicUnit::vSetPipein(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  m_xpPipein = pOperand;
  
  if (pOperand) {
    eZK eOperand = (eZK)AM::iOperandZkKind(pOperand);
    m_ePipeInOperand = eOperand;
  }
  else {
    m_ePipeInOperand = ZK_NONE;
  }
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get entwrk                                                                 */
/*----------------------------------------------------------------------------*/
EntWrk* LogicUnit::pEntWrk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  return & m_oEntWrk;
}

/*----------------------------------------------------------------------------*/
/* -- private ----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add code                                                                   */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCode(eEO eCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  ASSERT(0 == EoArgs[eCode].m_nArgBytes);
  
  if (m_bSuperFrag)
    return;
  
  if (0 == m_iNextCond) {
    m_iNextCond = C_UNKNOWN;
    return;
  }

  *(eEO*)&m_eaCode[m_i_Code] = eCode;
  m_i_Code += 2;

  m_iNextCond = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* add code with argument                                                     */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCodeArg(eEO eCode, i8 iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  ASSERT(8 == EoArgs[eCode].m_nArgBytes);
  
  if (m_bSuperFrag)
    return;
  
  if (0 == m_iNextCond) {
    m_iNextCond = C_UNKNOWN;
    return;
  }
  
  //m_eaCode[m_i_Code++] = eCode;
  *(eEO*)&m_eaCode[m_i_Code] = eCode;
  m_i_Code += 2;

  i8* piArg = (i8*) & m_eaCode[m_i_Code];
  * piArg = iArg;

  m_i_Code += sizeof(i8);
  
  m_iNextCond = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* add code with argument                                                     */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCodeArg(eEO eCode,Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  if (m_bSuperFrag)
    return;
  
  if (0 == m_iNextCond) {
    m_iNextCond = C_UNKNOWN;
    return;
  }
  
  i8 iArg = pTkn->iToI8();
  
  vAddCodeArg(eCode,iArg);

  m_iNextCond = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* add code with date argument                                                */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCodeDte(eEO eCode, Stz* szLiteral)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  ASSERT(EA_DATE == EoArgs[eCode].m_eArgument);
  
  if (m_bSuperFrag)
    return;
  
  if (0 == m_iNextCond) {
    m_iNextCond = C_UNKNOWN;
    return;
  }
  
  char* acCrs = szLiteral->azChar();
  
  SN_Dte snDate;
  i8 iDate = snDate.i(&acCrs,AM::pFmt());
  
  vAddCodeArg(eCode,iDate);

  m_iNextCond = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* add code with named label                                                  */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCodeLabel(eEO eCode, Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  if (m_bSuperFrag)
    return;
  
  if (0 == m_iNextCond) {
    m_iNextCond = C_UNKNOWN;
    return;
  }
  
  i8 i_Label = iFindLabel(szLabelNm);
  
  if (C_UNKNOWN == i_Label) {
    i_Label = iAddLabel(szLabelNm,false);
    m_xaJumpLabel[i_Label].m_i_Chain = m_i_Code + 2;
    vAddCodeArg(eCode,C_UNKNOWN);
  }
  else
  if (m_xaJumpLabel[i_Label].m_bDefined) {
    vAddCodeArg(eCode,m_xaJumpLabel[i_Label].m_i_Code);
  }
  else {
    i8 i_Chain = m_xaJumpLabel[i_Label].m_i_Chain;
    m_xaJumpLabel[i_Label].m_i_Chain = m_i_Code + 2;
    vAddCodeArg(eCode,i_Chain);
  }
  
  if (EO_CALL == eCode)
    m_xaJumpLabel[i_Label].m_bSubroutine = true;
  else if (eCode >= EO_JMP && eCode <= EO_JMP_GREATER)
    m_xaJumpLabel[i_Label].m_bDestination = true;

  //vPrintLabels();
  
  m_iNextCond = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* add code with label name in given token                                    */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCodeLabel(eEO eCode, Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  if (m_bSuperFrag)
    return;
  
  if (0 == m_iNextCond) {
    m_iNextCond = C_UNKNOWN;
    return;
  }
  
  vAddCodeLabel(eCode,pTkn->szChars());
  
  m_iNextCond = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* add code with string literal                                               */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCodeString(eEO eCode, Stz* szString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  ASSERT(-1 == EoArgs[eCode].m_nArgBytes);
  
  if (m_bSuperFrag)
    return;
  
  if (0 == m_iNextCond) {
    m_iNextCond = C_UNKNOWN;
    return;
  }
  
  //m_eaCode[m_i_Code++] = eCode;
  *(eEO*)&m_eaCode[m_i_Code] = eCode;
  m_i_Code += 2;

  for (char* acChr = szString->azChar(); *acChr; acChr++)
    m_eaCode[m_i_Code++] = (eEO)*acChr;
  m_eaCode[m_i_Code++] = (eEO)0;
  
  m_iNextCond = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* add code with time argument                                                */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCodeTme(eEO eCode, Stz* szLiteral)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  ASSERT(EA_TIME == EoArgs[eCode].m_eArgument);

  if (m_bSuperFrag)
    return;

  if (0 == m_iNextCond) {
    m_iNextCond = C_UNKNOWN;
    return;
  }

  char* acCrs = szLiteral->azChar();

  SN_Tme snTme;
  i8 iTme = snTme.i(&acCrs,AM::pFmt());

  vAddCodeArg(eCode,iTme);

  m_iNextCond = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* add code with timestamp argument                                           */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAddCodeTsp(eEO eCode, Stz* szLiteral)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  ASSERT(EA_TIMESTAMP == EoArgs[eCode].m_eArgument);

  if (m_bSuperFrag)
    return;

  if (0 == m_iNextCond) {
    m_iNextCond = C_UNKNOWN;
    return;
  }

  char* acCrs = szLiteral->azChar();

  SN_Tsp snTsp;
  i8 iTsp = snTsp.i(&acCrs,AM::pFmt());

  vAddCodeArg(eCode,iTsp);

  m_iNextCond = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* add label                                                                  */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iAddLabel(Stz* szLabelNm, bool bKnown)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  RASSERT(m_nLabels < C_LOGIC_LABEL - 1);

  i8 i_Label = m_nLabels;

  m_xaJumpLabel[i_Label].m_sLabelNm = szLabelNm;
  m_xaJumpLabel[i_Label].m_bDefined = bKnown;
  m_xaJumpLabel[i_Label].m_i_Chain = m_i_Code;
  m_nLabels++;
  
  return i_Label;
}

/*----------------------------------------------------------------------------*/
/* correct label chain                                                        */
/*----------------------------------------------------------------------------*/
void LogicUnit::vCorrectLabel(i8 i_Label)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  ASSERT(! m_xaJumpLabel[i_Label].m_bDefined);
  
  i8 i_Chain = m_xaJumpLabel[i_Label].m_i_Chain;
  while (C_UNKNOWN != i_Chain) {
    
    i8* piArg = (i8*) & m_eaCode[i_Chain];
 
    i_Chain = * piArg;

    * piArg = m_i_Code;
  }

  m_xaJumpLabel[i_Label].m_bDefined = true;
  m_xaJumpLabel[i_Label].m_i_Code   = m_i_Code;
}

/*----------------------------------------------------------------------------*/
/* decode label into subscript                                                */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iDecodeLabel(Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  i8 i_Label = iFindLabel(szLabelNm);

  if (C_UNKNOWN == i_Label)
    i_Label = iAddLabel(szLabelNm,false);
  
  i8 i_Code = m_xaJumpLabel[i_Label].m_i_Chain;
 
  m_xaJumpLabel[i_Label].m_i_Chain = m_i_Code;
  
  //vPrintLabels();
  
  return i_Code;
}

/*----------------------------------------------------------------------------*/
/* define label                                                               */
/*----------------------------------------------------------------------------*/
void LogicUnit::vDefineLabel(Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  if (m_bSuperFrag)
    return;
  
  // has this label been used yet?
  i8 i_Label = iFindLabel(szLabelNm);
  
  if (C_UNKNOWN == i_Label)
    i_Label = iAddLabel(szLabelNm,true);
  else
    vCorrectLabel(i_Label);
  
  //vPrintLabels();
}

/*----------------------------------------------------------------------------*/
/* delete label                                                               */
/*----------------------------------------------------------------------------*/
void LogicUnit::vDeleteLabel(Stz* szLabelNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // has this label been used yet?
  i8 i_Label = iFindLabel(szLabelNm);
  
  if (C_UNKNOWN == i_Label)
    return;
  
  // for the moment lets just scrub it out.  needs to be fixed
  m_xaJumpLabel[i_Label].m_sLabelNm = " *deleted* "_Stz;
}

/*----------------------------------------------------------------------------*/
/* diary report                                                               */
/*----------------------------------------------------------------------------*/
void LogicUnit::vDiaryReport(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  s256 sDiaryPath;
  
  if (0 != m_xRluCommon.m_iSql)
    sDiaryPath.vSprint("%s/RluDiary_%s_%lld_%lld.csv"_Stz,
                       m_pRluDiary->szDiaryDir(),
                       m_pRluDiary->szDiaryNm(),
                       m_xRluCommon.m_iSql,
                       m_xRluCommon.m_i_TablePtn
                       );
  else
    sDiaryPath.vSprint("%s/RluDiary_%s.csv"_Stz,
                       m_pRluDiary->szDiaryDir(),
                       m_pRluDiary->szDiaryNm()
                       );

  m_pRluDiary->vReportDiary(sDiaryPath);
  
}

/*----------------------------------------------------------------------------*/
/* find code                                                                  */
/*----------------------------------------------------------------------------*/
Stz* LogicUnit::szFindCode(i8 i_Code)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  for (i8 i_Label = 0; i_Label < m_nLabels; i_Label++)
    if (i_Code == m_xaJumpLabel[i_Label].m_i_Code)
      return m_xaJumpLabel[i_Label].m_sLabelNm;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* print labels                                                               */
/*----------------------------------------------------------------------------*/
void LogicUnit::vPrintLabels(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
//#ifdef WITH_TRACE_LOGICUNIT
  for (i8 i_Label = 0; i_Label < m_nLabels; i_Label++)
    vTrace("  [%02lld] %s %lld: %4lld%s%s\n"_Stz,
           i_Label,
           m_xaJumpLabel[i_Label].m_sLabelNm.szStz(),
           (i8)m_xaJumpLabel[i_Label].m_bDefined,
           m_xaJumpLabel[i_Label].m_i_Chain,
           m_xaJumpLabel[i_Label].m_bSubroutine ? " subroutine" : "",
           m_xaJumpLabel[i_Label].m_bDestination ? " destination" : ""
           );
//#endif
  
}



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump code                                                                  */
/*----------------------------------------------------------------------------*/
void LogicUnit::vDumpCode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

//#ifdef WITH_TRACE_LOGICUNIT

  return;
  
  SP_Dte spDate;

  vTrace("EO code for %lld:\n"_Stz,m_xRluCommon.m_iSql);
  
  for (i8 i_Code = 0; i_Code < m_i_Code; ) {
        
    eEO eCode = *(eEO*)&m_eaCode[i_Code];
        
    Stz* szLabel = szFindCode(i_Code);
    vTrace("%12s "_Stz,szLabel ? szLabel : ""_Stz);
      
    if (eCode <= EO_NONE || eCode >= EO_ENUMS) {
      vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
             "Invalid eo code sequence detected"_Stz);
    }
    
    if (-1 == EoArgs[eCode].m_nArgBytes) {
      char* azArg = (char*) & m_eaCode[i_Code+2];
      vTrace("%04lld: %20s (%s)\n"_Stz,i_Code,aEO[eCode],azArg);
      i_Code += 2 + StrLib::nSize(azArg);
    }
    else
    if (
        EA_DATE == EoArgs[eCode].m_eArgument) {
      i8* piArg = (i8*) & m_eaCode[i_Code+2];
      i8 iArg = * piArg;
      vTrace("%04lld: %20s (%s)\n"_Stz,i_Code,aEO[eCode],spDate.sz(iArg));
      i_Code += 2 + sizeof(i8);
    }
    else
    if (EA_LABEL == EoArgs[eCode].m_eArgument) {
      i8* piArg = (i8*) & m_eaCode[i_Code+2];
      i8 iArg = * piArg;
      Stz* szLabel = szFindCode(iArg);
      vTrace("%04lld: %20s => %s @%lld\n"_Stz,
             i_Code,
             aEO[eCode],
             szLabel ? szLabel : "<unknown>"_Stz,
             iArg
             );
      i_Code += 2 + sizeof(i8);
    }
    else
    if (EA_SUBSCR == EoArgs[eCode].m_eArgument) {
      i8* piArg = (i8*) & m_eaCode[i_Code+2];
      i8 iArg = * piArg;
      vTrace("%04lld: %20s [%lld]\n"_Stz,i_Code,aEO[eCode],iArg);
      i_Code += 2 + sizeof(i8);
    }
    else
    if (EoArgs[eCode].m_nArgBytes) {
      i8* piArg = (i8*) & m_eaCode[i_Code+2];
      i8 iArg = * piArg;
      vTrace("%04lld: %20s (%lld)\n"_Stz,i_Code,aEO[eCode],iArg);
      i_Code += 2 + sizeof(i8);
    }
    else {
      vTrace("%04lld: %20s\n"_Stz,i_Code,aEO[eCode]);
      i_Code += 2;
    }
    
    if (EO_RETURN == eCode)
      vTrace("\n"_Stz);
  }
  
  vDebugPoint();
  
  vPrintLabels();
//#endif
  
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* initialise and return address of rlu common                                */
/*----------------------------------------------------------------------------*/
struct RluCommon* LogicUnit::aRlu_INIT(void* aLU)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);

  UNUSED(aLU);
  
  return & m_xRluCommon;
}

/*----------------------------------------------------------------------------*/
/* dump the rlu Dstk                                                          */
/*----------------------------------------------------------------------------*/
void LogicUnit::vRlu_ASSERTSTACK(void* aLU, i8* aiDstk) {
  vDebugPoint();

  //vTrace("vRlu_ASSERTSTACK\n"_Stz);
  LogicUnit *pLogicUnit = _pLogicUnit(aLU);

  i8 nStkUse = nDifference(aiDstk,pLogicUnit->m_xRluCommon.m_iaDataStack);

  if (nStkUse != 0)
    vRlu_DUMP(aLU,aiDstk );

  RASSERT(nStkUse == 0);
}

/*----------------------------------------------------------------------------*/
/* dump the rlu Dstk                                                          */
/*----------------------------------------------------------------------------*/
void LogicUnit::vRlu_DUMP(void* aLU, i8* aiDstk)
{
  vDebugPoint();
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  i8* aiDstkBase = pLogicUnit->m_xRluCommon.m_iaDataStack;
  i8  nUsageBytes = nDifference(pLogicUnit->m_xRluCommon.m_iaDataStack,
                                aiDstk)
                     + 8;
  
  vTrace("\nRlu_Dump: base %llx ; aiDstk %llx ; %lld bytes usage\n"_Stz,
      
         aiDstkBase,
         aiDstk,
         nUsageBytes
         );
  
  MemLib::vHexDump(aiDstkBase,nUsageBytes);
  
  vTrace("\n"_Stz);
}

/*============================================================================*/
/* - - - - - - - - - - - - - RELATIONAL LOGIC UNIT - - - - - - - - - - - - - -*/
/*============================================================================*/

#if 1

  #define dLU3(eCode,iA,iB,iC) \
    if (aRluCommon->m_bTrace) \
      vTrace("%lld : %12s %lld %lld %lld\n"_Stz,\
             nDifference(m_eaCode,aeCode-1),\
             aEO[eCode],iA,iB,iC);

  #define dLU2(eCode,iA,iB) \
    if (aRluCommon->m_bTrace) \
      vTrace("%lld : %12s %lld %lld\n"_Stz,\
             nDifference(m_eaCode,aeCode-1),\
             aEO[eCode],iA,iB);

  #define dLU1(eCode,iA) \
    if (aRluCommon->m_bTrace) \
      vTrace("%lld : %12s %lld\n"_Stz,\
             nDifference(m_eaCode,aeCode-1),\
             aEO[eCode],iA);

  #define dLU0(eCode) \
    if (aRluCommon->m_bTrace) \
      vTrace("%lld : %12s\n"_Stz,\
             nDifference(m_eaCode,aeCode-1),\
             aEO[eCode]);

  #define ArgGet(iArg) \
    ASSERT(8 == EoArgs[eCode].m_nArgBytes); \
    iArg = * (i8*)aeCode; \
    aeCode += 8;

  #define GetStr(iArg) \
    ASSERT(-1 == EoArgs[eCode].m_nArgBytes); \
    iArg = (i8)aeCode; \
    while (*(char*)aeCode) aeCode++; \
    aeCode++;

  #define ArgNil() \
    ASSERT(0 == EoArgs[eCode].m_nArgBytes);

#else

  #define dLU3(eCode,iA,iB,iC)
  #define dLU2(eCode,iA,iB)
  #define dLU1(eCode,iA)
  #define dLU0(eCode)

  #define ArgGet(iArg) \
    iArg = * (i8*)aeCode; \
    aeCode += 8;

  #define GetStr(iArg) \
    iArg = (i8)aeCode; \
    while (*(char*)aeCode) aeCode++; \
    aeCode++;

  #define ArgNil()

#endif


#define EO_ENTER(eCode)
#define EO_EXIT(eCode)

i8 giaVectorSet[] =
{
/* 00 */ L_QUANTITY,
/* 01 */ L_EXTENDEDPRICE,
/* 02 */ L_DISCOUNT,
/* 03 */ L_TAX,
/* 04 */ L_RETURNFLAG,
/* 05 */ L_LINESTATUS,
/* 06 */ L_SHIPDATE,
/* 07 */ C_UNKNOWN,
  
/* 08 */ Q1_LOCAL_RETURNFLAG,
/* 09 */ Q1_LOCAL_LINESTATUS,
/* 10 */ C_UNKNOWN,
  
/* 11 */ Q101_RETURNFLAG,
/* 12 */ Q101_LINESTATUS,
/* 13 */ C_UNKNOWN,
};

/*----------------------------------------------------------------------------*/
/* compute                                                                    */
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iCompute(i8 nOuterArgs, i8 iOuterArg0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  #ifdef WITH_TRACE_LOGICUNIT
    vTrace("\nLogic Unit :: sql %lld\n\n"_Stz,m_xRluCommon.m_iSql);
    vDebugPoint();
  #endif

  i8 iArg;
  i8 iTopA;
  i8 iTopB;
  i8 iTopC;

  char* azTopA;
  char* azTopB;
  Stz* szTopA;
  Stz* szTopB;
  d8 dTopA;
  d8 dTopB;
  i8 iRes;
  d8 dRes;
  Stz* szRes;

  i8 iCmp;

  i8 nSize;
  char* azDst;
  char* azSrc;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // this code equates to aiDynFun() preliminary
  
  LogicUnit* pLU = this;
  
  struct RluCommon* aRluCommon = aRlu_INIT(pLU); // temp... should be nullptr
  ASSERT(123456789 == aRluCommon->m_iCmnMagic);
  ASSERT(987654321 == aRluCommon->m_iCmnFooter);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // temp / incorrect place
  
  i8* aiDstk = aRluCommon->m_iaDataStack;
  *aiDstk = 0xFFFFFFFFFFFFFFFF;

  i8* aiLocals = aRluCommon->m_iaLocals;
  i8* aiPayload;

  if (nOuterArgs) {
    *(++aiDstk) = iOuterArg0;
  }

  aRluCommon->m_i_TablePtn = MY::pOp()->tPtnId().iAsI8();

  aRluCommon->m_nPtns  = AM::PTNMAP::nPtnsInGrid();
  aRluCommon->m_iHash  = 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // required for interpretative inner loop
  
  eEO*  eaReturnStack[C_LOGIC_STACK];
  eEO** aeaRstk = &eaReturnStack[0];
  char*  aeCode  = (char*)& m_eaCode[0];

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//
  //                    interpretative inner loop proper                      //
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//

  for (;;) {
    
#ifdef WITH_TRACE_LOGICUNIT
    // check for stack growth
    if (aiDstk >= &aRluCommon->m_iaDataStack[C_LOGIC_STACK-4])
      vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
             "LogicUnit data stack exhausted\n"_Stz);

    i8 nStkUse = nDifference(aiDstk,aRluCommon->m_iaDataStack);
    i8 iStkPct = (nStkUse * 100) / C_LOGIC_STACK;
    if (iStkPct > 80)
      vTrace("WARNING: LogicUnit stack use %lld%%\n"_Stz,iStkPct);
#endif
    
    eEO eCode = *(eEO*)aeCode;
    aeCode += 2;

#ifdef WITH_TRACE_LOGICUNIT
    if(aRluCommon->m_iaDataStack[0] != -1)
      vRlu_DUMP(pLU, aiDstk);
    vTrace("%s %lld\n"_Stz, aEO[eCode], nStkUse);
#endif
    
    switch (eCode) { //+ExtractBegin
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // arithmetic operators

      // add top two numbers and push result

      case EO_ADD_BCDBCD: // bcd + bcd
        EO_ENTER(eCode);
        ArgNil();
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Add_BCDBCD(iTopA, iTopB);
        dLU3(EO_ADD_BCDBCD,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ADD_BCDFLT: // bcd + flt
        EO_ENTER(eCode);
        ArgNil();
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Add_FLTBCD(iTopB, iTopA);
        dLU3(EO_ADD_FLTFLT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ADD_BCDINT: // bcd + int
        EO_ENTER(eCode);
        ArgNil();
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Add_INTBCD(iTopB, iTopA);
        dLU3(EO_ADD_BCDINT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ADD_DTEIVL:
        EO_ENTER(eCode);
        ArgNil(); // dte + ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Add_DTEIVL(iTopA, iTopB);
        dLU3(EO_ADD_DTEIVL,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ADD_FLTBCD:
        EO_ENTER(eCode);
        ArgNil(); // flt + bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Add_FLTBCD(iTopA, iTopB);
        dLU3(EO_ADD_FLTBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ADD_FLTINT:
        EO_ENTER(eCode);
        ArgNil(); // flt + int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Add_FLTINT(iTopA, iTopB);
        dLU3(EO_ADD_FLTINT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ADD_FLTFLT:
        EO_ENTER(eCode);
        ArgNil(); // flt + flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Add_FLTFLT(iTopA, iTopB);
        dLU3(EO_ADD_FLTFLT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ADD_INTBCD:
        EO_ENTER(eCode);
        ArgNil(); // int + bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Add_INTBCD(iTopA, iTopB);
        dLU3(EO_ADD_BCDINT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ADD_INTFLT:
        EO_ENTER(eCode);
        ArgNil(); // int + flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Add_FLTINT(iTopB, iTopA);
        dLU3(EO_ADD_FLTINT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ADD_INTINT:
        EO_ENTER(eCode);
        ArgNil(); // int + int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Add_INTINT(iTopA, iTopB);
        dLU3(EO_ADD_INTBCD,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ADD_IVLDTE:
        EO_ENTER(eCode);
        ArgNil(); // ivl + dte
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Add_IVLDTE(iTopA, iTopB);
        dLU3(EO_ADD_IVLDTE,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ADD_IVLIVL:
        EO_ENTER(eCode);
        ArgNil(); // ivl + ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Add_IVLIVL(iTopA, iTopB);
        dLU3(EO_ADD_IVLIVL,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ADD_IVLTME:
        EO_ENTER(eCode);
        ArgNil(); // ivl + tme
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Add_IVLTME(iTopA, iTopB);
        dLU3(EO_ADD_IVLTME,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ADD_IVLTSP:
        EO_ENTER(eCode);
        ArgNil(); // ivl + tsp
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Add_IVLTSP(iTopA, iTopB);
        dLU3(EO_ADD_IVLTME,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ADD_TMEIVL:
        EO_ENTER(eCode);
        ArgNil(); // tme + ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Add_IVLTME(iTopB, iTopA);
        dLU3(EO_ADD_TMEIVL,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ADD_TSPIVL:
        EO_ENTER(eCode);
        ArgNil(); // tsp + ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Add_IVLTSP(iTopB, iTopA);
        dLU3(EO_ADD_TSPIVL,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ADD_UNKXYZ:
        EO_ENTER(eCode);
        ArgNil(); // unk + xyz
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopA == C_DATUM_NULL);
        dLU3(EO_ADD_UNKXYZ,iTopA,iTopB,iRes);
        *(++aiDstk) = C_DATUM_NULL;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ADD_XYZUNK:
        EO_ENTER(eCode);
        ArgNil(); // xyz + unk
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopB == C_DATUM_NULL);
        *(++aiDstk) = C_DATUM_NULL;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DIV_BCDBCD:
        EO_ENTER(eCode);
        ArgNil(); // bcd / bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Div_BCDBCD(iTopA, iTopB);
        dLU3(EO_DIV_BCDBCD,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DIV_BCDFLT:
        EO_ENTER(eCode);
        ArgNil(); // bcd / flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Div_BCDFLT(iTopA, iTopB);
        dLU3(EO_DIV_BCDFLT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DIV_BCDINT: // bcd / int
        EO_ENTER(eCode);
        ArgNil();
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Div_BCDINT(iTopA, iTopB);
        dLU3(EO_DIV_BCDINT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DIV_DTEIVL:
        EO_ENTER(eCode);
        ArgNil(); // dte / ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Div_DTEIVL(iTopA, iTopB);
        dLU3(EO_DIV_DTEIVL,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DIV_FLTBCD:
        EO_ENTER(eCode);
        ArgNil(); // flt / bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Div_FLTBCD(iTopA, iTopB);
        dLU3(EO_DIV_FLTBCD,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DIV_FLTINT:
        EO_ENTER(eCode);
        ArgNil(); // flt / int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Div_FLTINT(iTopA, iTopB);
        dLU3(EO_DIV_FLTINT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DIV_FLTFLT:
        EO_ENTER(eCode);
        ArgNil(); // flt / flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Div_FLTFLT(iTopA, iTopB);
        dLU3(EO_DIV_FLTFLT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DIV_INTBCD:
        EO_ENTER(eCode);
        ArgNil(); // int / bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Div_INTBCD(iTopA, iTopB);
        dLU3(EO_DIV_INTBCD,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DIV_INTFLT:
        EO_ENTER(eCode);
        ArgNil(); // int / flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Div_INTFLT(iTopA, iTopB);
        dLU3(EO_DIV_INTFLT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DIV_INTINT:
        EO_ENTER(eCode);
        ArgNil(); // int / int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Div_INTINT(iTopA, iTopB);
        dLU3(EO_DIV_INTINT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DIV_IVLDTE:
        EO_ENTER(eCode);
        ArgNil(); // ivl / dte
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Div_IVLDTE(iTopA, iTopB);
        dLU3(EO_DIV_INTINT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DIV_IVLIVL:
        EO_ENTER(eCode);
        ArgNil(); // ivl / ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Div_IVLIVL(iTopA, iTopB);
        dLU3(EO_DIV_INTINT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DIV_IVLTME:
        EO_ENTER(eCode);
        ArgNil(); // ivl / tme
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Div_IVLTME(iTopA, iTopB);
        dLU3(EO_DIV_INTINT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DIV_IVLTSP:
        EO_ENTER(eCode);
        ArgNil(); // ivl / tsp
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Div_IVLTSP(iTopA, iTopB);
        dLU3(EO_DIV_INTINT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DIV_TMEIVL:
        EO_ENTER(eCode);
        ArgNil(); // tme / ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Div_TMEIVL(iTopA, iTopB);
        dLU3(EO_DIV_INTINT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DIV_TSPIVL:
        EO_ENTER(eCode);
        ArgNil(); // tsp / ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Div_TSPIVL(iTopA, iTopB);
        dLU3(EO_DIV_INTINT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DIV_UNKXYZ:
        EO_ENTER(eCode);
        ArgNil(); // unk / xyz
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopA == C_DATUM_NULL);
        dLU3(EO_DIV_UNKXYZ,iTopA,iTopB,iRes);
        *(++aiDstk) = C_DATUM_NULL;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DIV_XYZUNK:
        EO_ENTER(eCode);
        ArgNil(); // xyz / unk
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopB == C_DATUM_NULL);
        dLU3(EO_DIV_XYZUNK,iTopA,iTopB,iRes);
        *(++aiDstk) = C_DATUM_NULL;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_MOD_INTINT:
        EO_ENTER(eCode);
        ArgNil(); // int % int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Mod_INTINT(iTopA, iTopB);
        dLU3(EO_MOD_INTINT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_MOD_UNKXYZ:
        EO_ENTER(eCode);
        ArgNil(); // unk % xyz
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopA == C_DATUM_NULL);
        dLU3(EO_MOD_UNKXYZ,iTopA,iTopB,iRes);
        *(++aiDstk) = C_DATUM_NULL;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_MOD_XYZUNK:
        EO_ENTER(eCode);
        ArgNil(); // xyz % unk
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopB == C_DATUM_NULL);
        dLU3(EO_MOD_XYZUNK,iTopA,iTopB,iRes);
        *(++aiDstk) = C_DATUM_NULL;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_MUL_BCDBCD:
        EO_ENTER(eCode);
        ArgNil(); // bcd * bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Mul_BCDBCD(iTopB, iTopA);
        dLU3(EO_MUL_BCDBCD,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_MUL_BCDFLT:
        EO_ENTER(eCode);
        ArgNil(); // bcd * flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Mul_BCDFLT(iTopA, iTopB);
        dLU3(EO_MUL_BCDFLT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_MUL_BCDINT:
        EO_ENTER(eCode);
        ArgNil(); // bcd * int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Mul_BCDINT(iTopA, iTopB);
        dLU3(EO_MUL_BCDINT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_MUL_DTEIVL:
        EO_ENTER(eCode);
        ArgNil(); // dte * ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Mul_DTEIVL(iTopA, iTopB);
        dLU3(EO_MUL_DTEIVL,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_MUL_FLTBCD:
        EO_ENTER(eCode);
        ArgNil(); // flt * bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Mul_BCDFLT(iTopB, iTopA);
        dLU3(EO_MUL_FLTBCD,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_MUL_FLTINT:
        EO_ENTER(eCode);
        ArgNil(); // flt * int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Mul_FLTINT(iTopA, iTopB);
        dLU3(EO_MUL_FLTINT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_MUL_FLTFLT:
        EO_ENTER(eCode);
        ArgNil(); // flt * flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Mul_FLTFLT(iTopA, iTopB);
        dLU3(EO_MUL_FLTFLT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_MUL_INTBCD:
        EO_ENTER(eCode);
        ArgNil(); // int * bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Mul_INTBCD(iTopA, iTopB);
        dLU3(EO_MUL_INTBCD,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_MUL_INTFLT:
        EO_ENTER(eCode);
        ArgNil(); // int * flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Mul_FLTINT(iTopB, iTopA);
        dLU3(EO_MUL_INTFLT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_MUL_INTINT:
        EO_ENTER(eCode);
        ArgNil(); // int * int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Mul_INTINT(iTopA, iTopB);
        dLU3(EO_MUL_INTINT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_MUL_IVLDTE:
        EO_ENTER(eCode);
        ArgNil(); // ivl * dte
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Mul_IVLDTE(iTopA, iTopB);
        dLU3(EO_MUL_IVLDTE,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_MUL_IVLIVL:
        EO_ENTER(eCode);
        ArgNil(); // ivl * ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Mul_IVLIVL(iTopA, iTopB);
        dLU3(EO_MUL_IVLIVL,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_MUL_IVLTME:
        EO_ENTER(eCode);
        ArgNil(); // ivl * tme
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Mul_IVLTME(iTopA, iTopB);
        dLU3(EO_MUL_IVLTME,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_MUL_IVLTSP:
        EO_ENTER(eCode);
        ArgNil(); // ivl * tsp
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Mul_IVLTSP(iTopA, iTopB);
        dLU3(EO_MUL_IVLTSP,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_MUL_TMEIVL:
        EO_ENTER(eCode);
        ArgNil(); // tme * ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Mul_IVLTME(iTopB, iTopA);
        dLU3(EO_MUL_TMEIVL,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_MUL_TSPIVL:
        EO_ENTER(eCode);
        ArgNil(); // tsp * ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Mul_IVLTSP(iTopB, iTopA);
        dLU3(EO_MUL_TSPIVL,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_MUL_UNKXYZ:
        EO_ENTER(eCode);
        ArgNil(); // unk * xyz
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopA == C_DATUM_NULL);
        dLU3(EO_MUL_UNKXYZ,iTopA,iTopB,iRes);
        *(++aiDstk) = C_DATUM_NULL;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_MUL_XYZUNK:
        EO_ENTER(eCode);
        ArgNil(); // xyz * unk
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopB == C_DATUM_NULL);
        dLU3(EO_MUL_XYZUNK,iTopA,iTopB,iRes);
        *(++aiDstk) = C_DATUM_NULL;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NEG_BCD:
        EO_ENTER(eCode);
        ArgNil(); // neg bcd
        iTopA = *(aiDstk--);
        iRes  = - iTopA;
        iRes  = iMathLib_Neg_BCD(iTopA);
        dLU2(EO_NEG_BCD,iTopA,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NEG_FLT:
        EO_ENTER(eCode);
        ArgNil(); // neg flt
        iTopA = *(aiDstk--);
        iRes  = - iTopA;
        iRes  = iMathLib_Neg_FLT(iTopA);
        dLU2(EO_NEG_FLT,iTopA,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NEG_INT:
        EO_ENTER(eCode);
        ArgNil(); // neg int
        iTopA = *(aiDstk--);
        iRes  = - iTopA;
        iRes  = iMathLib_Neg_INT(iTopA);
        dLU2(EO_NEG_INT,iTopA,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NEG_IVL:
        EO_ENTER(eCode);
        ArgNil(); // neg ivl
        iTopA = *(aiDstk--);
        iRes  = - iTopA;
        iRes  = iMathLib_Neg_IVL(iTopA);
        dLU2(EO_NEG_IVL,iTopA,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      // subtract second int from top push result
      case EO_SUB_BCDBCD:
        EO_ENTER(eCode);
        ArgNil(); // bcd - bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Sub_BCDBCD(iTopA, iTopB);
        dLU3(EO_SUB_BCDBCD,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_SUB_BCDFLT:
        EO_ENTER(eCode);
        ArgNil(); // bcd - flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Sub_BCDFLT(iTopA, iTopB);
        dLU3(EO_SUB_BCDFLT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_SUB_BCDINT:
        EO_ENTER(eCode);
        ArgNil(); // bcd - int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Sub_BCDINT(iTopA, iTopB);
        dLU3(EO_SUB_BCDINT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_SUB_FLTBCD:
        EO_ENTER(eCode);
        ArgNil(); // flt - bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Sub_FLTBCD(iTopA, iTopB);
        dLU3(EO_SUB_FLTBCD,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_SUB_FLTINT:
        EO_ENTER(eCode);
        ArgNil(); // flt - int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Sub_FLTINT(iTopA, iTopB);
        dLU3(EO_SUB_FLTINT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_SUB_FLTFLT:
        EO_ENTER(eCode);
        ArgNil(); // flt - flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Sub_FLTFLT(iTopA, iTopB);
        dLU3(EO_SUB_FLTFLT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_SUB_INTBCD:
        EO_ENTER(eCode);
        ArgNil(); // int - bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Sub_INTBCD(iTopA, iTopB);
        dLU3(EO_SUB_INTBCD,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_SUB_INTFLT:
        EO_ENTER(eCode);
        ArgNil(); // int - flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Sub_INTFLT(iTopA, iTopB);
        dLU3(EO_SUB_INTFLT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_SUB_INTINT:
        EO_ENTER(eCode);
        ArgNil(); // int - int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Sub_INTINT(iTopA, iTopB);
        dLU3(EO_SUB_INTINT,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_SUB_IVLDTE:
        EO_ENTER(eCode);
        ArgNil(); // ivl - dte
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Sub_IVLDTE(iTopA, iTopB);
        dLU3(EO_SUB_IVLDTE,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_SUB_IVLIVL:
        EO_ENTER(eCode);
        ArgNil(); // ivl - ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Sub_IVLIVL(iTopA, iTopB);
        dLU3(EO_SUB_IVLIVL,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_SUB_IVLTME:
        EO_ENTER(eCode);
        ArgNil(); // ivl - tme
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Sub_IVLTME(iTopA, iTopB);
        dLU3(EO_SUB_IVLTME,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_SUB_IVLTSP:
        EO_ENTER(eCode);
        ArgNil(); // ivl - tsp
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Sub_IVLTSP(iTopA, iTopB);
        dLU3(EO_SUB_IVLTSP,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_SUB_TMEIVL:
        EO_ENTER(eCode);
        ArgNil(); // tme - ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Sub_TMEIVL(iTopA, iTopB);
        dLU3(EO_SUB_TMEIVL,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_SUB_TSPIVL:
        EO_ENTER(eCode);
        ArgNil(); // tsp - ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Sub_TSPIVL(iTopA, iTopB);
        dLU3(EO_SUB_TSPIVL,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_SUB_UNKXYZ:
        EO_ENTER(eCode);
        ArgNil(); // unk - xyz
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopA == C_DATUM_NULL);
        dLU3(EO_SUB_UNKXYZ,iTopA,iTopB,iRes);
        *(++aiDstk) = C_DATUM_NULL;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_SUB_XYZUNK:
        EO_ENTER(eCode);
        ArgNil(); // xyz - unk
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopB == C_DATUM_NULL);
        dLU3(EO_SUB_XYZUNK,iTopA,iTopB,iRes);
        *(++aiDstk) = C_DATUM_NULL;
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // counter
      case EO_CTR_0_DEC:
        EO_ENTER(eCode);
        ArgNil(); // dec counter 0
        aRluCommon->m_iCtr0--;
        dLU1(EO_CTR_0_DEC,aRluCommon->m_iCtr0);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_CTR_0_INC:
        EO_ENTER(eCode);
        ArgNil(); // inc counter 0
        aRluCommon->m_iCtr0++;
        dLU1(EO_CTR_0_INC,aRluCommon->m_iCtr0);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_CTR_0_POP:
        EO_ENTER(eCode);
        ArgNil(); // pop counter 0
        aRluCommon->m_iCtr0 = *(aiDstk--);
        dLU1(EO_CTR_0_POP,aRluCommon->m_iCtr0);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_CTR_0_PUSH:
        EO_ENTER(eCode);
        ArgNil(); // push counter 0
        dLU1(EO_CTR_0_PUSH,aRluCommon->m_iCtr0);
        *(++aiDstk) = aRluCommon->m_iCtr0;
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // jump operators
      case EO_JMP:
        EO_ENTER(eCode);
        ArgGet(iArg); // jump to location
        dLU1(EO_JMP,iArg);
        JumpTo(iArg,EO_JMP); // aeCode = & m_eaCode[iArg];
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_JMP_FALSE:
        EO_ENTER(eCode);
        ArgGet(iArg); // pop top and jump to location if false
        dLU1(EO_JMP_FALSE,iArg);
        if (! *(aiDstk--))
          JumpTo(iArg,EO_JMP_FALSE); // aeCode = & m_eaCode[iArg];
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_JMP_TRUE:
        EO_ENTER(eCode);
        ArgGet(iArg); // pop top and jump to location if true
        dLU1(EO_JMP_TRUE,iArg);
        iRes = *(aiDstk);
        if (*(aiDstk--))
          JumpTo(iArg,EO_JMP_TRUE); // aeCode = & m_eaCode[iArg];
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_JMP_LESS:
        EO_ENTER(eCode);
        ArgGet(iArg); // pop top and jump to location if <
        dLU1(EO_JMP_LESS,iArg);
        if (*(aiDstk--) < 0)
          JumpTo(iArg,EO_JMP_LESS); // aeCode = & m_eaCode[iArg];
        EO_EXIT(eCode);
        break;//+endCase

      case EO_JMP_LESSEQUAL:
        EO_ENTER(eCode);
        ArgGet(iArg); // pop top and jump to location if <=
        dLU1(EO_JMP_LESSEQUAL,iArg);
        if (*(aiDstk--) <= 0)
          JumpTo(iArg,EO_JMP_LESSEQUAL); // aeCode = & m_eaCode[iArg];
        EO_EXIT(eCode);
        break;//+endCase

      case EO_JMP_EQUAL:
        EO_ENTER(eCode);
        ArgGet(iArg); // pop top and jump to location if ==
        dLU1(EO_JMP_EQUAL,iArg);
        if (*(aiDstk--) == 0)
          JumpTo(iArg,EO_JMP_EQUAL); // aeCode = & m_eaCode[iArg];
        EO_EXIT(eCode);
        break;//+endCase

      case EO_JMP_NOTEQUAL:
        EO_ENTER(eCode);
        ArgGet(iArg); // pop top and jump to location if !=
        dLU1(EO_JMP_NOTEQUAL,iArg);
        if (*(aiDstk--) != 0)
          JumpTo(iArg,EO_JMP_NOTEQUAL); // aeCode = & m_eaCode[iArg];
        EO_EXIT(eCode);
        break;//+endCase

      case EO_JMP_GREATEREQUAL:
        EO_ENTER(eCode);
        ArgGet(iArg); // pop top and jump to location if >=
        dLU1(EO_JMP_GREATEREQUAL,iArg);
        if (*(aiDstk--) >= 0)
          JumpTo(iArg,EO_JMP_GREATEREQUAL); // aeCode = & m_eaCode[iArg];
        EO_EXIT(eCode);
        break;//+endCase

      case EO_JMP_GREATER:
        EO_ENTER(eCode);
        ArgGet(iArg); // pop top and jump to location if >
        dLU1(EO_JMP_GREATER,iArg);
        if (*(aiDstk--) > 0)
          JumpTo(iArg,EO_JMP_GREATER); // aeCode = & m_eaCode[iArg];
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // logical operators
      case EO_LGL_AND:
        EO_ENTER(eCode);
        ArgNil(); // logical and top two int's, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes = iMathLib_Lgl_AND(iTopA, iTopB);
        dLU3(EO_LGL_AND,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_LGL_OR:
        EO_ENTER(eCode);
        ArgNil(); // logical or top two int's, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes = iMathLib_Lgl_OR(iTopA, iTopB);
        dLU3(EO_LGL_OR,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_LGL_NOT:
        EO_ENTER(eCode);
        ArgNil(); // logical not top int, push result
        iTopA = *(aiDstk--);
        iRes = iMathLib_Lgl_NOT(iTopA);
        dLU2(EO_LGL_NOT,iTopA,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LGL_XOR:
        EO_ENTER(eCode);
        ArgNil(); // logical xor top two int's, push result
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes = iMathLib_Lgl_XOR(iTopA, iTopB);
        dLU3(EO_LGL_XOR,iTopA,iTopB,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // null test operators
      case EO_ISNULL:
        EO_ENTER(eCode);
        ArgNil(); // test if top is null( NO POPPING) , push result
        iTopA = *(aiDstk);
        iRes  = (C_DATUM_NULL == iTopA) ? 1 : 0;
        dLU2(EO_ISNULL,iTopA,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ISNULL_STR:
        EO_ENTER(eCode);
        ArgNil(); // test if top is null string ( NO POPPING) , push result
        // get topA string
        iRes = ((i8)*aiDstk == C_DATUM_NULL) ? 1 : 0;
        dLU2(EO_ISNULL_STR,iTopA,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ISNOTNULL:
        EO_ENTER(eCode);
        ArgNil(); // test if top is not null, push result
        iTopA = *(aiDstk--);
        iRes  = (C_DATUM_NULL != iTopA) ? 1 : 0;
        dLU2(EO_ISNOTNULL,iTopA,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
       
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // pot
      case EO_POT_USE_STREAMDEFOUT:
        EO_ENTER(eCode);
        ArgGet(iArg); // use streamDefOut in the pot locals
        dLU0(EO_POT_USE_STREAMDEFOUT);
        iTopA = *(aiDstk--);
        vDev_POT_USE_STREAMDEFOUT(pLU,iArg);
        EO_EXIT(eCode);
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // prints
      case EO_PRINT:
        EO_ENTER(eCode);
        ArgNil(); // print top
#ifdef WITH_TRACE_LOGICUNIT
        iTopA = *(aiDstk--);
        dLU1(EO_PRINT,iTopA);

        printf("[Q%lld.%lld] %lld\n",(i8)0,(i8)0,iTopA);
#endif
        EO_EXIT(eCode);
        break;//+endCase
            
      case EO_PRINTF:
        EO_ENTER(eCode);
        GetStr(iArg); // formatted print
#ifdef WITH_TRACE_LOGICUNIT
        dLU0(EO_PRINTF);
        vLib_PRINTF(pLU,(Stz*)iArg,&aiDstk);
#endif
        EO_EXIT(eCode);
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // pushes
      case EO_DATE:
        EO_ENTER(eCode);
        ArgGet(iArg); // push date
        dLU1(EO_DATE,iArg);
        *(++aiDstk) = iArg;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_INTEGER:
        EO_ENTER(eCode);
        ArgGet(iArg); // push integer
        dLU1(EO_INTEGER,iArg);
        *(++aiDstk) = iArg;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_PUSHNULL:
        EO_ENTER(eCode);
        ArgNil(); // push null
        dLU0(EO_PUSHNULL);
        *(++aiDstk) = C_DATUM_NULL;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_PUSH_MINUS1:
        EO_ENTER(eCode);
        ArgNil(); // push integer -1
        dLU0(EO_PUSH_MINUS1);
        *(++aiDstk) = -1;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_PUSH_0:
        EO_ENTER(eCode);
        ArgNil(); // push integer 0
        dLU0(EO_PUSH_0);
        *(++aiDstk) = 0;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_PUSH_1:
        EO_ENTER(eCode);
        ArgNil(); // push integer 1
        dLU0(EO_PUSH_1);
        *(++aiDstk) = 1;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_RANDOM:
        EO_ENTER(eCode);
        ArgGet(iArg); // push random number 0..arg-1
        iRes = iLib_RANDOM(pLU,iArg);
        dLU2(EO_RANDOM,iArg,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_STRING:
        EO_ENTER(eCode);
        GetStr(iArg); // push string
        dLU1(EO_STRING,iArg);
        S_PushStr(iArg);
        //vRlu_DUMP(pLU,aiDstk);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_TIME:
        EO_ENTER(eCode);
        ArgGet(iArg); // push time
        dLU1(EO_TIME,iArg);
        *(++aiDstk) = iArg;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_TIMESTAMP:
        EO_ENTER(eCode);
        ArgGet(iArg); // push timestamp
        dLU1(EO_TIMESTAMP,iArg);
        *(++aiDstk) = iArg;
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // relational operators

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // EQUAL
      case EO_EQ_BCDBCD:
        EO_ENTER(eCode);
        ArgNil(); // bcd == bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Eq_BCDBCD(iTopA, iTopB);
        dLU3(EO_EQ_BCDBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_EQ_BCDFLT:
        EO_ENTER(eCode);
        ArgNil(); // bcd == flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Eq_FLTBCD(iTopB, iTopA);
        dLU3(EO_EQ_BCDFLT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_EQ_BCDINT:
        EO_ENTER(eCode);
        ArgNil(); // bcd == int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Eq_INTBCD(iTopB, iTopA);
        dLU3(EO_EQ_BCDINT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_EQ_BOLBOL:
        EO_ENTER(eCode);
        ArgNil(); // bol == bol
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Eq_BOLBOL(iTopA, iTopB);
        dLU3(EO_EQ_BOLBOL, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_EQ_DTEDTE:
        EO_ENTER(eCode);
        ArgNil(); // dte == dte
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Eq_DTEDTE(iTopA, iTopB);
        dLU3(EO_EQ_DTEDTE, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_EQ_DTETSP:
        EO_ENTER(eCode);
        ArgNil(); // dte == tsp
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Eq_DTETSP(iTopA, iTopB);
        dLU3(EO_EQ_DTETSP, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_EQ_FLTBCD:
        EO_ENTER(eCode);
        ArgNil(); // flt == bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Eq_FLTBCD(iTopA, iTopB);
        dLU3(EO_EQ_FLTBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_EQ_FLTFLT:
        EO_ENTER(eCode);
        ArgNil(); // flt == flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Eq_FLTFLT(iTopA, iTopB);
        dLU3(EO_EQ_FLTFLT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_EQ_FLTINT:
        EO_ENTER(eCode);
        ArgNil(); // flt == int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Eq_FLTINT(iTopA, iTopB);
        dLU3(EO_EQ_BCDBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_EQ_INTBCD:
        EO_ENTER(eCode);
        ArgNil(); // int == bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Eq_INTBCD(iTopA, iTopB);
        dLU3(EO_EQ_INTBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_EQ_INTFLT:
        EO_ENTER(eCode);
        ArgNil(); // int == flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Eq_FLTINT(iTopB, iTopA);
        dLU3(EO_EQ_INTFLT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_EQ_INTINT:
        EO_ENTER(eCode);
        ArgNil(); // int == int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Eq_INTINT(iTopA, iTopB);
        dLU3(EO_EQ_INTINT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_EQ_IVLIVL:
        EO_ENTER(eCode);
        ArgNil(); // ivl == ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Eq_IVLIVL(iTopA, iTopB);
        dLU3(EO_EQ_BCDBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_EQ_TMETME:
        EO_ENTER(eCode);
        ArgNil(); // tme == tme
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Eq_TMETME(iTopA, iTopB);
        dLU3(EO_EQ_TMETME, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_EQ_TSPDTE:
        EO_ENTER(eCode);
        ArgNil(); // tsp == dte
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Eq_TSPDTE(iTopA, iTopB);
        dLU3(EO_EQ_BCDBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_EQ_TSPTSP:
        EO_ENTER(eCode);
        ArgNil(); // tsp == tsp
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Eq_TSPTSP(iTopA, iTopB);
        dLU3(EO_EQ_BCDBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_EQ_VCHVCH:
        EO_ENTER(eCode);
        ArgNil(); // vch == vch
        // get topB string
        iTopB = *(aiDstk--);
        if (iTopB == C_DATUM_NULL) {
          iTopA = *(aiDstk--);
          if (iTopA != C_DATUM_NULL) {
            azTopA = (char *) (aiDstk - (nWordRoundedStrBytes(iTopA) / 8 - 1));
            aiDstk -= nWordRoundedStrBytes(iTopA) / 8;
          }
          *(++aiDstk) = 0;
        }
        else {
          azTopB = (char *) (aiDstk - (nWordRoundedStrBytes(iTopB) / 8 - 1));
          aiDstk -= nWordRoundedStrBytes(iTopB) / 8;

          // get topA string
          iTopA = *(aiDstk--);
          if (iTopA == C_DATUM_NULL) {
            *(++aiDstk) = 0;
          } else {
            azTopA = (char *) (aiDstk - (nWordRoundedStrBytes(iTopA) / 8 - 1));
            aiDstk -= nWordRoundedStrBytes(iTopA) / 8;

            // invoke function
            iRes = iMathLib_Eq_VCHVCH(azTopA, azTopB);
            dLU3(EO_EQ_VCHVCH, iTopA, azTopA, azTopB);
            *(++aiDstk) = iRes;
          }
        }
        EO_EXIT(eCode);
        break;//+endCase

      case EO_EQ_UNKXYZ:
        EO_ENTER(eCode);
        ArgNil(); // unk == xyz
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopA == C_DATUM_NULL);
        dLU3(EO_SUB_UNKXYZ,iTopA,iTopB,iRes);
        *(++aiDstk) = 0;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_EQ_XYZUNK:
        EO_ENTER(eCode);
        ArgNil(); // xyz == unk
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopB == C_DATUM_NULL);
        dLU3(EO_SUB_UNKXYZ,iTopA,iTopB,iRes);
        *(++aiDstk) = 0;
        EO_EXIT(eCode);
        break;//+endCase


      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // GRETAER THAN EQUAL
      case EO_GE_BCDBCD:
        EO_ENTER(eCode);
        ArgNil(); // bcd >= bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ge_BCDBCD(iTopA, iTopB);
        dLU3(EO_GE_BCDBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_GE_BCDFLT:
        EO_ENTER(eCode);
        ArgNil(); // bcd >= bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ge_BCDFLT(iTopA, iTopB);
        dLU3(EO_GE_BCDFLT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_GE_BCDINT:
        EO_ENTER(eCode);
        ArgNil(); // bcd >= int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ge_BCDINT(iTopA, iTopB);
        dLU3(EO_GE_BCDINT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_GE_BOLBOL:
        EO_ENTER(eCode);
        ArgNil(); // bol >= bol
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ge_BOLBOL(iTopA, iTopB);
        dLU3(EO_GE_BOLBOL, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_GE_DTEDTE:
        EO_ENTER(eCode);
        ArgNil(); // dte >= dte
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ge_DTEDTE(iTopA, iTopB);
        dLU3(EO_GE_DTEDTE, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_GE_DTETSP:
        EO_ENTER(eCode);
        ArgNil(); // dte >= tsp
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ge_DTETSP(iTopA, iTopB);
        dLU3(EO_GE_DTETSP, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_GE_FLTBCD:
        EO_ENTER(eCode);
        ArgNil(); // flt >= bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ge_FLTBCD(iTopA, iTopB);
        dLU3(EO_GE_FLTBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_GE_FLTFLT:
        EO_ENTER(eCode);
        ArgNil(); // flt >= flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ge_FLTFLT(iTopA, iTopB);
        dLU3(EO_GE_FLTFLT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_GE_FLTINT:
        EO_ENTER(eCode);
        ArgNil(); // flt >= int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ge_FLTINT(iTopA, iTopB);
        dLU3(EO_GE_FLTINT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_GE_INTBCD:
        EO_ENTER(eCode);
        ArgNil(); // int >= bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ge_INTBCD(iTopA, iTopB);
        dLU3(EO_GE_INTBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_GE_INTFLT:
        EO_ENTER(eCode);
        ArgNil(); // int >= flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ge_INTFLT(iTopA, iTopB);
        dLU3(EO_GE_INTFLT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_GE_INTINT:
        EO_ENTER(eCode);
        ArgNil(); // int >= int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ge_INTINT(iTopA, iTopB);
        dLU3(EO_GE_INTINT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_GE_IVLIVL:
        EO_ENTER(eCode);
        ArgNil(); // ivl >= ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ge_IVLIVL(iTopA, iTopB);
        dLU3(EO_GE_IVLIVL, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_GE_TMETME:
        EO_ENTER(eCode);
        ArgNil(); // tme >= tme
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ge_TMETME(iTopA, iTopB);
        dLU3(EO_GE_TMETME, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_GE_TSPDTE:
        EO_ENTER(eCode);
        ArgNil(); // tsp >= dte
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ge_TSPDTE(iTopA, iTopB);
        dLU3(EO_GE_TSPDTE, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_GE_TSPTSP:
        EO_ENTER(eCode);
        ArgNil(); // tsp >= tsp
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ge_TSPTSP(iTopA, iTopB);
        dLU3(EO_GE_TSPTSP, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_GE_VCHVCH:
        EO_ENTER(eCode);
        ArgNil(); // vch >= vch
        // get topB string
        iTopB = *(aiDstk--);
        azTopB = (char*)(aiDstk - (nWordRoundedStrBytes(iTopB)/8 - 1));
        aiDstk -= nWordRoundedStrBytes(iTopB)/8;

        // get topA string
        iTopA = *(aiDstk--);
        azTopA = (char*)(aiDstk - (nWordRoundedStrBytes(iTopA)/8 - 1));
        aiDstk -= nWordRoundedStrBytes(iTopA)/8;

        // invoke function
        iRes  = iMathLib_Ge_VCHVCH(azTopA, azTopB);
        dLU3(EO_GE_VCHVCH, iTopA, azTopA, azTopB);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_GE_UNKXYZ:
        EO_ENTER(eCode);
        ArgNil(); // unk >= xyz
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopA == C_DATUM_NULL);
        dLU3(EO_GE_UNKXYZ,iTopA,iTopB,iRes);
        *(++aiDstk) = 0;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_GE_XYZUNK:
        EO_ENTER(eCode);
        ArgNil(); // xyz >= unk
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopB == C_DATUM_NULL);
        dLU3(EO_GE_XYZUNK,iTopA,iTopB,iRes);
        *(++aiDstk) = 0;
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // GREATER THAN

      case EO_GT_BCDBCD:
        EO_ENTER(eCode);
        ArgNil(); // bcd > bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Gt_BCDBCD(iTopA, iTopB);
        dLU3(EO_GT_BCDBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_GT_BCDFLT:
        EO_ENTER(eCode);
        ArgNil(); // bcd > bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Gt_BCDFLT(iTopA, iTopB);
        dLU3(EO_GT_BCDFLT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_GT_BCDINT:
        EO_ENTER(eCode);
        ArgNil(); // bcd > int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Gt_BCDINT(iTopA, iTopB);
        dLU3(EO_GT_BCDINT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_GT_BOLBOL:
        EO_ENTER(eCode);
        ArgNil(); // bol > bol
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Gt_BOLBOL(iTopA, iTopB);
        dLU3(EO_GT_BOLBOL, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_GT_DTEDTE:
        EO_ENTER(eCode);
        ArgNil(); // dte > dte
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Gt_DTEDTE(iTopA, iTopB);
        dLU3(EO_GT_DTEDTE, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_GT_DTETSP:
        EO_ENTER(eCode);
        ArgNil(); // dte > tsp
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Gt_DTETSP(iTopA, iTopB);
        dLU3(EO_GT_DTETSP, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_GT_FLTBCD:
        EO_ENTER(eCode);
        ArgNil(); // flt > bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Gt_FLTBCD(iTopA, iTopB);
        dLU3(EO_GT_FLTBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_GT_FLTFLT:
        EO_ENTER(eCode);
        ArgNil(); // flt > flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Gt_FLTFLT(iTopA, iTopB);
        dLU3(EO_GT_FLTFLT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_GT_FLTINT:
        EO_ENTER(eCode);
        ArgNil(); // flt > int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Gt_FLTINT(iTopA, iTopB);
        dLU3(EO_GT_FLTINT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_GT_INTBCD:
        EO_ENTER(eCode);
        ArgNil(); // int > bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Gt_INTBCD(iTopA, iTopB);
        dLU3(EO_GT_INTBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_GT_INTFLT:
        EO_ENTER(eCode);
        ArgNil(); // int > flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Gt_INTFLT(iTopA, iTopB);
        dLU3(EO_GT_INTFLT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_GT_INTINT:
        EO_ENTER(eCode);
        ArgNil(); // int > int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Gt_INTINT(iTopA, iTopB);
        dLU3(EO_GT_INTINT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_GT_IVLIVL:
        EO_ENTER(eCode);
        ArgNil(); // ivl > ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Gt_IVLIVL(iTopA, iTopB);
        dLU3(EO_GT_IVLIVL, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_GT_TMETME:
        EO_ENTER(eCode);
        ArgNil(); // tme > tme
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Gt_TMETME(iTopA, iTopB);
        dLU3(EO_GT_TMETME, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_GT_TSPDTE:
        EO_ENTER(eCode);
        ArgNil(); // tsp > dte
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Gt_TSPDTE(iTopA, iTopB);
        dLU3(EO_GT_TSPDTE, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_GT_TSPTSP:
        EO_ENTER(eCode);
        ArgNil(); // tsp > tsp
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Gt_TSPTSP(iTopA, iTopB);
        dLU3(EO_GT_TSPTSP, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_GT_VCHVCH:
        EO_ENTER(eCode);
        ArgNil(); // vch > vch
        // get topB string
        //vRlu_DUMP(pLU,aiDstk);
        iTopB = *(aiDstk--);
        azTopB = (char*)(aiDstk - (nWordRoundedStrBytes(iTopB)/8 - 1));
        aiDstk -= nWordRoundedStrBytes(iTopB)/8;

        // get topA string
        iTopA = *(aiDstk--);
        azTopA = (char*)(aiDstk - (nWordRoundedStrBytes(iTopA)/8 - 1));
        aiDstk -= nWordRoundedStrBytes(iTopA)/8;

        // invoke function
        iRes  = iMathLib_Gt_VCHVCH(azTopA, azTopB);
        dLU3(EO_EQ_VCHVCH, iTopA, azTopA, azTopB);
        *(++aiDstk) = iRes;
        //vRlu_DUMP(pLU,aiDstk);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_GT_UNKXYZ:
        EO_ENTER(eCode);
        ArgNil(); // unk > xyz
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopA == C_DATUM_NULL);
        dLU3(EO_GT_UNKXYZ,iTopA,iTopB,iRes);
        *(++aiDstk) = 0;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_GT_XYZUNK:
        EO_ENTER(eCode);
        ArgNil(); // xyz > unk
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopB == C_DATUM_NULL);
        dLU3(EO_GT_XYZUNK,iTopA,iTopB,iRes);
        *(++aiDstk) = 0;
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // LESS THAN EQUAL

      case EO_LE_BCDBCD:
        EO_ENTER(eCode);
        ArgNil(); // bcd <= bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Le_BCDBCD(iTopA, iTopB);
        dLU3(EO_LE_BCDBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_LE_BCDFLT:
        EO_ENTER(eCode);
        ArgNil(); // bcd <= bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Le_BCDFLT(iTopA, iTopB);
        dLU3(EO_LE_BCDFLT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LE_BCDINT:
        EO_ENTER(eCode);
        ArgNil(); // bcd <= int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Le_BCDINT(iTopA, iTopB);
        dLU3(EO_LE_BCDINT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LE_BOLBOL:
        EO_ENTER(eCode);
        ArgNil(); // bol <= bol
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Le_BOLBOL(iTopA, iTopB);
        dLU3(EO_LE_BOLBOL, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LE_DTEDTE:
        EO_ENTER(eCode);
        ArgNil(); // dte <= dte
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Le_DTEDTE(iTopA, iTopB);
        dLU3(EO_LE_DTEDTE, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LE_DTETSP:
        EO_ENTER(eCode);
        ArgNil(); // dte <= tsp
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Le_DTETSP(iTopA, iTopB);
        dLU3(EO_LE_DTETSP, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LE_FLTBCD:
        EO_ENTER(eCode);
        ArgNil(); // flt <= bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Le_FLTBCD(iTopA, iTopB);
        dLU3(EO_LE_FLTBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LE_FLTFLT:
        EO_ENTER(eCode);
        ArgNil(); // flt <= flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Le_FLTFLT(iTopA, iTopB);
        dLU3(EO_LE_FLTFLT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LE_FLTINT:
        EO_ENTER(eCode);
        ArgNil(); // flt <= int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Le_FLTINT(iTopA, iTopB);
        dLU3(EO_LE_FLTINT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LE_INTBCD:
        EO_ENTER(eCode);
        ArgNil(); // int <= bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Le_INTBCD(iTopA, iTopB);
        dLU3(EO_LE_INTBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LE_INTFLT:
        EO_ENTER(eCode);
        ArgNil(); // int <= flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Le_INTFLT(iTopA, iTopB);
        dLU3(EO_LE_INTFLT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LE_INTINT:
        EO_ENTER(eCode);
        ArgNil(); // int <= int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Le_INTINT(iTopA, iTopB);
        dLU3(EO_LE_INTINT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LE_IVLIVL:
        EO_ENTER(eCode);
        ArgNil(); // ivl <= ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Le_IVLIVL(iTopA, iTopB);
        dLU3(EO_LE_IVLIVL, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LE_TMETME:
        EO_ENTER(eCode);
        ArgNil(); // tme <= tme
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Le_TMETME(iTopA, iTopB);
        dLU3(EO_LE_TMETME, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LE_TSPDTE:
        EO_ENTER(eCode);
        ArgNil(); // tsp <= dte
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Le_TSPDTE(iTopA, iTopB);
        dLU3(EO_LE_TSPDTE, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LE_TSPTSP:
        EO_ENTER(eCode);
        ArgNil(); // tsp <= tsp
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Le_TSPTSP(iTopA, iTopB);
        dLU3(EO_LE_TSPTSP, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LE_VCHVCH:
        EO_ENTER(eCode);
        ArgNil(); // vch <= vch
        // get topB string
        iTopB = *(aiDstk--);
        azTopB = (char*)(aiDstk - (nWordRoundedStrBytes(iTopB)/8 - 1));
        aiDstk -= nWordRoundedStrBytes(iTopB)/8;

        // get topA string
        iTopA = *(aiDstk--);
        azTopA = (char*)(aiDstk - (nWordRoundedStrBytes(iTopA)/8 - 1));
        aiDstk -= nWordRoundedStrBytes(iTopA)/8;

        // invoke function
        iRes  = iMathLib_Le_VCHVCH(azTopA, azTopB);
        dLU3(EO_LE_VCHVCH, iTopA, azTopA, azTopB);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LE_UNKXYZ:
        EO_ENTER(eCode);
        ArgNil(); // unk <= xyz
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopA == C_DATUM_NULL);
        dLU3(EO_LE_UNKXYZ,iTopA,iTopB,iRes);
        *(++aiDstk) = 0;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LE_XYZUNK:
        EO_ENTER(eCode);
        ArgNil(); // xyz <= unk
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopB == C_DATUM_NULL);
        dLU3(EO_LE_XYZUNK,iTopA,iTopB,iRes);
        *(++aiDstk) = 0;
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // LESS THAN

      case EO_LT_BCDBCD:
        EO_ENTER(eCode);
        ArgNil(); // bcd < bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Lt_BCDBCD(iTopA, iTopB);
        dLU3(EO_LT_BCDBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LT_BCDFLT:
        EO_ENTER(eCode);
        ArgNil(); // bcd < bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Lt_BCDFLT(iTopA, iTopB);
        dLU3(EO_LT_BCDFLT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LT_BCDINT:
        EO_ENTER(eCode);
        ArgNil(); // bcd < int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Lt_BCDINT(iTopA, iTopB);
        dLU3(EO_LT_BCDINT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LT_BOLBOL:
        EO_ENTER(eCode);
        ArgNil(); // bol < bol
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Lt_BOLBOL(iTopA, iTopB);
        dLU3(EO_LT_BOLBOL, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LT_DTEDTE:
        EO_ENTER(eCode);
        ArgNil(); // dte < dte
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Lt_DTEDTE(iTopA, iTopB);
        dLU3(EO_LT_DTEDTE, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LT_DTETSP:
        EO_ENTER(eCode);
        ArgNil(); // dte < tsp
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Lt_DTETSP(iTopA, iTopB);
        dLU3(EO_LT_DTETSP, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LT_FLTBCD:
        EO_ENTER(eCode);
        ArgNil(); // flt < bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Lt_FLTBCD(iTopA, iTopB);
        dLU3(EO_LT_FLTBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LT_FLTFLT:
        EO_ENTER(eCode);
        ArgNil(); // flt < flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Lt_FLTFLT(iTopA, iTopB);
        dLU3(EO_LT_FLTFLT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LT_FLTINT:
        EO_ENTER(eCode);
        ArgNil(); // flt < int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Lt_FLTINT(iTopA, iTopB);
        dLU3(EO_LT_FLTINT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LT_INTBCD:
        EO_ENTER(eCode);
        ArgNil(); // int < bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Lt_INTBCD(iTopA, iTopB);
        dLU3(EO_LT_INTBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LT_INTFLT:
        EO_ENTER(eCode);
        ArgNil(); // int < flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Lt_INTFLT(iTopA, iTopB);
        dLU3(EO_LT_INTFLT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LT_INTINT:
        EO_ENTER(eCode);
        ArgNil(); // int < int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Lt_INTINT(iTopA, iTopB);
        dLU3(EO_LT_INTINT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LT_IVLIVL:
        EO_ENTER(eCode);
        ArgNil(); // ivl < ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Lt_IVLIVL(iTopA, iTopB);
        dLU3(EO_LT_IVLIVL, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LT_TMETME:
        EO_ENTER(eCode);
        ArgNil(); // tme < tme
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Lt_TMETME(iTopA, iTopB);
        dLU3(EO_LT_TMETME, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LT_TSPDTE:
        EO_ENTER(eCode);
        ArgNil(); // tsp < dte
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Lt_TSPDTE(iTopA, iTopB);
        dLU3(EO_LT_TSPDTE, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LT_TSPTSP:
        EO_ENTER(eCode);
        ArgNil(); // tsp < tsp
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Lt_TSPTSP(iTopA, iTopB);
        dLU3(EO_LT_TSPTSP, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LT_VCHVCH:
        EO_ENTER(eCode);
        ArgNil(); // vch < vch
        // get topB string
        iTopB = *(aiDstk--);
        azTopB = (char*)(aiDstk - (nWordRoundedStrBytes(iTopB)/8 - 1));
        aiDstk -= nWordRoundedStrBytes(iTopB)/8;

        // get topA string
        iTopA = *(aiDstk--);
        azTopA = (char*)(aiDstk - (nWordRoundedStrBytes(iTopA)/8 - 1));
        aiDstk -= nWordRoundedStrBytes(iTopA)/8;

        // invoke function
        iRes  = iMathLib_Lt_VCHVCH(azTopA, azTopB);
        dLU3(EO_LT_VCHVCH, iTopA, azTopA, azTopB);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LT_UNKXYZ:
        EO_ENTER(eCode);
        ArgNil(); // unk < xyz
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopA == C_DATUM_NULL);
        dLU3(EO_LT_UNKXYZ,iTopA,iTopB,iRes);
        *(++aiDstk) = 0;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_LT_XYZUNK:
        EO_ENTER(eCode);
        ArgNil(); // xyz < unk
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopB == C_DATUM_NULL);
        dLU3(EO_LT_XYZUNK,iTopA,iTopB,iRes);
        *(++aiDstk) = 0;
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // NOT EQUAL

      case EO_NE_BCDBCD:
        EO_ENTER(eCode);
        ArgNil(); // bcd != bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ne_BCDBCD(iTopA, iTopB);
        dLU3(EO_NE_BCDBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NE_BCDFLT:
        EO_ENTER(eCode);
        ArgNil(); // bcd != bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ne_BCDFLT(iTopA, iTopB);
        dLU3(EO_NE_BCDFLT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NE_BCDINT:
        EO_ENTER(eCode);
        ArgNil(); // bcd != int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ne_BCDINT(iTopA, iTopB);
        dLU3(EO_NE_BCDINT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NE_BOLBOL:
        EO_ENTER(eCode);
        ArgNil(); // bol != bol
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ne_BOLBOL(iTopA, iTopB);
        dLU3(EO_NE_BOLBOL, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NE_DTEDTE:
        EO_ENTER(eCode);
        ArgNil(); // dte != dte
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ne_DTEDTE(iTopA, iTopB);
        dLU3(EO_NE_DTEDTE, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NE_DTETSP:
        EO_ENTER(eCode);
        ArgNil(); // dte != tsp
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ne_DTETSP(iTopA, iTopB);
        dLU3(EO_NE_DTETSP, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NE_FLTBCD:
        EO_ENTER(eCode);
        ArgNil(); // flt != bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ne_FLTBCD(iTopA, iTopB);
        dLU3(EO_NE_FLTBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NE_FLTFLT:
        EO_ENTER(eCode);
        ArgNil(); // flt != flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ne_FLTFLT(iTopA, iTopB);
        dLU3(EO_NE_FLTFLT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NE_FLTINT:
        EO_ENTER(eCode);
        ArgNil(); // flt != int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ne_FLTINT(iTopA, iTopB);
        dLU3(EO_NE_FLTINT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NE_INTBCD:
        EO_ENTER(eCode);
        ArgNil(); // int != bcd
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ne_INTBCD(iTopA, iTopB);
        dLU3(EO_NE_INTBCD, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NE_INTFLT:
        EO_ENTER(eCode);
        ArgNil(); // int != flt
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ne_INTFLT(iTopA, iTopB);
        dLU3(EO_NE_INTFLT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NE_INTINT:
        EO_ENTER(eCode);
        ArgNil(); // int != int
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ne_INTINT(iTopA, iTopB);
        dLU3(EO_NE_INTINT, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NE_IVLIVL:
        EO_ENTER(eCode);
        ArgNil(); // ivl != ivl
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ne_IVLIVL(iTopA, iTopB);
        dLU3(EO_NE_IVLIVL, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NE_TMETME:
        EO_ENTER(eCode);
        ArgNil(); // tme != tme
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ne_TMETME(iTopA, iTopB);
        dLU3(EO_NE_TMETME, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NE_TSPDTE:
        EO_ENTER(eCode);
        ArgNil(); // tsp != dte
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ne_TSPDTE(iTopA, iTopB);
        dLU3(EO_NE_TSPDTE, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NE_TSPTSP:
        EO_ENTER(eCode);
        ArgNil(); // tsp != tsp
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        iRes  = iMathLib_Ne_TSPTSP(iTopA, iTopB);
        dLU3(EO_NE_TSPTSP, iTopA, iTopB, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NE_VCHVCH:
        EO_ENTER(eCode);
        ArgNil(); // vch != vch
        // get topB string
        iTopB = *(aiDstk--);
        azTopB = (char*)(aiDstk - (nWordRoundedStrBytes(iTopB)/8 - 1));
        aiDstk -= nWordRoundedStrBytes(iTopB)/8;

        // get topA string
        iTopA = *(aiDstk--);
        azTopA = (char*)(aiDstk - (nWordRoundedStrBytes(iTopA)/8 - 1));
        aiDstk -= nWordRoundedStrBytes(iTopA)/8;

        // invoke function
        iRes  = iMathLib_Ne_VCHVCH(azTopA, azTopB);
        dLU3(EO_NE_VCHVCH, iTopA, azTopA, azTopB);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NE_UNKXYZ:
        EO_ENTER(eCode);
        ArgNil(); // unk != xyz
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopA == C_DATUM_NULL);
        dLU3(EO_NE_UNKXYZ,iTopA,iTopB,iRes);
        *(++aiDstk) = 0;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_NE_XYZUNK:
        EO_ENTER(eCode);
        ArgNil(); // xyz != unk
        iTopB = *(aiDstk--);
        iTopA = *(aiDstk--);
        //RASSERT(iTopB == C_DATUM_NULL);
        dLU3(EO_NE_XYZUNK,iTopA,iTopB,iRes);
        *(++aiDstk) = 0;
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // column pushes
      case EO_PUSH_COLUMN_LENGTH:
        EO_ENTER(eCode);
        ArgGet(iArg); // push columnn length
        dLU1(EO_PUSH_COLUMN_LENGTH,iArg);
        *(++aiDstk) = iArg;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PUSH_COLUMN_SCALE:
        EO_ENTER(eCode);
        ArgGet(iArg); // push columnn scale
        dLU1(EO_PUSH_COLUMN_SCALE,iArg);
        *(++aiDstk) = iArg;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PUSH_COLUMN_TYPE:
        EO_ENTER(eCode);
        ArgGet(iArg); // push columnn type
        dLU1(EO_PUSH_COLUMN_TYPE,iArg);
        *(++aiDstk) = iArg;
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // ixt device pushes
      case EO_PUSH_DEVIXT_ALT:
        EO_ENTER(eCode);
        ArgNil(); // push alt ixt device
        dLU0(EO_PUSH_DEVIXT_ALT);
        P_PushPtr(aRluCommon->m_pDevAltIxt);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PUSH_DEVIXT_ENT:
        EO_ENTER(eCode);
        ArgNil(); // push entry from ixt device
        dLU0(EO_PUSH_DEVIXT_ENT);
        vDev_IXT_PUSH_ENT(&aiDstk);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PUSH_DEVIXT_LFT:
        EO_ENTER(eCode);
        ArgNil(); // push lft ixt devive
        dLU0(EO_PUSH_DEVIXT_LFT);
        P_PushPtr(aRluCommon->m_pDevLftIxt);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PUSH_DEVIXT_RHT:
        EO_ENTER(eCode);
        ArgNil(); // push rht ixt devive
        dLU0(EO_PUSH_DEVIXT_RHT);
        P_PushPtr(aRluCommon->m_pDevRhtIxt);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PUSH_DEVIXT_TMP:
        EO_ENTER(eCode);
        ArgNil(); // push tmp ixt device
        dLU0(EO_PUSH_DEVIXT_TMP);
        P_PushPtr(aRluCommon->m_pDevTmpIxt);
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // row device pushes
      case EO_PUSH_DEVROW_ALT:
        EO_ENTER(eCode);
        ArgNil(); // push alt row device
        dLU0(EO_PUSH_DEVROW_ALT);
        P_PushPtr(aRluCommon->m_pDevAltRow);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PUSH_DEVROW_DST:
        EO_ENTER(eCode);
        ArgNil(); // push dst row device
        dLU0(EO_PUSH_DEVROW_DST);
        P_PushPtr(aRluCommon->m_pDevDstRow);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PUSH_DEVROW_LFT:
        EO_ENTER(eCode);
        ArgNil(); // push lft row device
        dLU0(EO_PUSH_DEVROW_LFT);
        P_PushPtr(aRluCommon->m_pDevLftRow);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PUSH_DEVROW_RHT:
        EO_ENTER(eCode);
        ArgNil(); // push rht row device
        dLU0(EO_PUSH_DEVROW_RHT);
        P_PushPtr(aRluCommon->m_pDevRhtRow);
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // rlu pushes
      case EO_PUSH_RLU_COLSET:
        EO_ENTER(eCode);
        ArgNil(); // push rlu colset
        dLU0(EO_PUSH_RLU_COLSET);
        P_PushPtr(aRluCommon->m_iaColSet);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PUSH_RLU_ENT:
        EO_ENTER(eCode);
        ArgNil(); // push rlu ent
        dLU0(EO_PUSH_RLU_ENT);
        P_PushPtr(aRluCommon->m_aWrkEnt);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PUSH_RLU_ENTWRK:
        EO_ENTER(eCode);
        ArgNil(); // push rlu entwrk
        dLU0(EO_PUSH_RLU_ENTWRK);
        P_PushPtr(aRluCommon->m_pEntWrk);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PUSH_RLU_INDEXDEF:
        EO_ENTER(eCode);
        ArgNil(); // push rlu indexDef
        dLU0(EO_PUSH_RLU_INDEXDEF);
        P_PushPtr(aRluCommon->m_pIndexDef);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PUSH_RLU_POT:
        EO_ENTER(eCode);
        ArgNil(); // push rlu pot
        dLU0(EO_PUSH_RLU_POT);
        P_PushPtr(aRluCommon->m_pPot);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PUSH_RLU_PTN:
        EO_ENTER(eCode);
        ArgNil(); // push rlu partition
        dLU0(EO_PUSH_RLU_PTN);
        P_PushPtr(aRluCommon->m_i_Ptn);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PUSH_RLU_ROWPIX:
        EO_ENTER(eCode);
        ArgNil(); // push rlu row pix
        dLU0(EO_PUSH_RLU_ROWPIX);
        P_PushPtr(aRluCommon->m_pRowPix);
        EO_EXIT(eCode);
        break;//+endCase

      // locals pushes
      case EO_PUSH_RLU_LOCALS:
        EO_ENTER(eCode);
        ArgNil(); // push rlu locals
        dLU0(EO_PUSH_RLU_LOCALS);
        aiLocals = aRluCommon->m_iaLocals - 1;
        P_PushPtr(&aiLocals);
        EO_EXIT(eCode);
        break;//+endCase

      // indextree pushes
      case EO_PUSH_RLU_INDEXTREE:
        EO_ENTER(eCode);
        ArgNil(); // push rlu indextree
        dLU0(EO_PUSH_RLU_INDEXTREE);
        P_PushPtr(aRluCommon->m_pIndexTree);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PUSH_RLU_JOINEDROW:
        EO_ENTER(eCode);
        ArgNil(); // push rlu indexDef
        dLU0(EO_PUSH_RLU_JOINEDROW);
        P_PushPtr(aRluCommon->m_iJoinedRow);
        EO_EXIT(eCode);
        break;//+endCase

      // joinmap push
      case EO_PUSH_RLU_JOINMAP_BYTE:
        EO_ENTER(eCode);
        ArgNil(); // push rlu joinmap byte
        dLU0(EO_PUSH_RLU_JOINMAP_BYTE);
        iTopA = *(aiDstk--);
        I_PushInt(aRluCommon->m_baJoinMap[iTopA]);
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // table ptn
      case EO_TABLEPTN_DROPREAD:
        EO_ENTER(eCode);
        ArgNil(); // drop the tableptn read lock
        dLU0(EO_TABLEPTN_DROPREAD);
        vDev_TABLEPTN_DROPREAD(pLU);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_TABLEPTN_GRABREAD:
        EO_ENTER(eCode);
        ArgNil(); // grab the tableptn read lock
        dLU0(EO_TABLEPTN_GRABREAD);
        vDev_TABLEPTN_GRABREAD(pLU);
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // stack operators
      case EO_INC:
        EO_ENTER(eCode);
        ArgNil(); // increment top, push result
        iTopA = *aiDstk;
        iRes = iTopA + 1;
        dLU2(EO_INC,iTopA,iRes);
        *aiDstk = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEC:
        EO_ENTER(eCode);
        ArgNil(); // decrement top, push result
        iTopA = *aiDstk;
        iRes = iTopA - 1;
        dLU2(EO_DEC,iTopA,iRes);
        *aiDstk = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DUP:
        EO_ENTER(eCode);
        ArgNil(); // duplicate top
        iTopA = *aiDstk;
        dLU1(EO_DUP,iTopA);
        *(++aiDstk) = iTopA;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DUP_STR:
        EO_ENTER(eCode);
        ArgNil(); // duplicate top string
        //vRlu_DUMP(pLU,aiDstk);
        S_StrDup();
        //vRlu_DUMP(pLU,aiDstk);
        EO_EXIT(eCode);
        break;//+endCase


      case EO_POP:
        EO_ENTER(eCode);
        ArgNil(); // pop / discard top
        aiDstk--;
        dLU0(EO_POP);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_POP_LOCALI:
        EO_ENTER(eCode);
        ArgGet(iArg); // pop into a local reg
        dLU1(EO_POP_LOCALI, iArg);
        //ASSERT(iArg < C_LOGIC_LOCALS);
        iTopA = *(aiDstk--);
        vLib_SET_LOCAL(pLU, iArg, iTopA);
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_POP_LOCALS:
        EO_ENTER(eCode);
        ArgGet(iArg); // pop into a local reg
        dLU1(EO_POP_LOCALS, iArg);
        //if (iArg >= C_LOGIC_LOCALS)
        //  vLib_ASSERT(pLU,0);
        S_PopStr(sLib_GET_LOCALS(pLU, iArg));
        EO_EXIT(eCode);
        break;//+endCase

      case EO_POP_STR:
        EO_ENTER(eCode);
        ArgNil(); // pop / discard top string
        dLU0(EO_POP);
        S_StrDrp();
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PUSH_LOCALI: {
        EO_ENTER(eCode);
        ArgGet(iArg); // push a local reg
        dLU1(EO_PUSH_LOCALI, iArg);
        //ASSERT(iArg < C_LOGIC_LOCALS);
        d8 x = (d8) aRluCommon->m_iaLocals[iArg];
        *(++aiDstk) = aRluCommon->m_iaLocals[iArg]; //iLib_GET_LOCAL(pLU, iArg);
      }
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_PUSH_LOCALS:
        EO_ENTER(eCode);
        ArgGet(iArg); // push a local reg
        dLU1(EO_PUSH_LOCALS, iArg);
        //ASSERT(iArg < C_LOGIC_LOCALS);

        azSrc = sLib_GET_LOCALS(pLU, iArg);

        // check for null string
        if ((i8) azSrc == C_DATUM_NULL) {
          *(++aiDstk) = C_DATUM_NULL;
        } else {
          S_PushStr(azSrc);
        }
        EO_EXIT(eCode);
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // string operators
      case EO_STR_COMPARE:
        EO_ENTER(eCode);
        ArgNil(); // pop top stradr and compare str on stack
        dLU0(EO_STR_COMPARE);
        szTopB = (Stz*) *(aiDstk--);
      
        iTopA = *(aiDstk--);
        szTopA = (Stz*) (aiDstk-(nWordRoundedStrBytes(iTopA)/8 - 1));
        iRes = iLib_STR_COMPARE(pLU,szTopA,szTopB);
        aiDstk -= nWordRoundedStrBytes(nSize)/8;
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
       // string operators
      case EO_STR_COMPARE_a:
        EO_ENTER(eCode);
        ArgNil(); // pop top two azStr and compare
        dLU0(EO_STR_COMPARE_a);
        azTopB = (char*) *(aiDstk--);
        azTopA = (char*) *(aiDstk--);
        iRes = strcmp(azTopA,azTopB);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_STR_PRINTADR:
        EO_ENTER(eCode);
        ArgNil(); // pop top and print iArg as addr of string
#ifdef WITH_TRACE_LOGICUNIT
        azTopA = (char*) *(aiDstk--);
        dLU1(EO_STR_PRINTADR,azTopA);
        printf("[Q%lld.%lld] %s\n",
               aRluCommon->m_iSql,aRluCommon->m_i_TablePtn,azTopA);
#endif
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_STR_PRINTSTK:
        EO_ENTER(eCode);
        ArgNil(); // pop string from top and print
        iTopA = *(aiDstk--);
        szTopA = (Stz*) (aiDstk-(iTopA/8 + 1));
        printf("EO_STR_PRINTSTK : %lld %p\n",iTopA,(char*)szTopA);
        dLU1(EO_STR_PRINTSTK,iTopA);
        printf("[Q%lld,%lld] %s\n",
               aRluCommon->m_iSql,aRluCommon->m_i_TablePtn,(char*)szTopA);
        aiDstk -= (iTopA/8) + 2;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_STR_LITERAL: {
        EO_ENTER(eCode);
        GetStr(iArg); // push address of string literal
        dLU1(EO_STR_LITERAL, iArg);
        Stz *sZ = (Stz *) iArg;
        *(++aiDstk) = (i8) iArg;
      }
        EO_EXIT(eCode);
        break;//+endCase
      
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // subroutine operators
      case EO_CALL:
        EO_ENTER(eCode);
        ArgGet(iArg); // call subroutine at location
        dLU1(EO_CALL,iArg);
#ifdef __TINYC__
#else
        * (aeaRstk++) = (eEO*)aeCode;
        aeCode = & m_eaCode[iArg];
#endif
        EO_EXIT(eCode);
        break;//+endCase

      case EO_RETURN:
        EO_ENTER(eCode);
        ArgNil(); // return from subroutine and
        dLU0(EO_RETURN);
        //vRlu_ASSERTSTACK(pLU, aiDstk);
#ifdef __TINYC__
        return aiDstk;
#else
        if (aeaRstk == &eaReturnStack[0]) {
          iRes = *(aiDstk--);
#ifdef WITH_TRACE_LOGICUNIT
          vTrace("\nEnd Logic Unit :: sql %lld\n\n"_Stz,aRluCommon->m_iSql);
#endif
          return iRes;
        }
        aeCode = (char*)(* (--aeaRstk));
#endif
        EO_EXIT(eCode);
        break;//+endCase

      case EO_RETURN_FALSE:
        EO_ENTER(eCode);
        ArgNil(); // return from subroutine on false
        dLU0(EO_RETURN);
#ifdef __TINYC__
        if (! *(aiDstk--))
          return aiDstk;
#else
        iRes = *(aiDstk--);
        if (! iRes) {
          if (aeaRstk == &eaReturnStack[0]) {

#ifdef WITH_TRACE_LOGICUNIT
            vTrace("\nEnd Logic Unit :: sql %lld\n\n"_Stz,aRluCommon->m_iSql);
#endif
            return iRes;
          }
          aeCode = (char*)(* (--aeaRstk));
        }

#endif
        EO_EXIT(eCode);
        break;//+endCase

      case EO_RETURN_TRUE:
        EO_ENTER(eCode);
        ArgNil(); // return from subroutine on true
        dLU0(EO_RETURN);
#ifdef __TINYC__
        if ( *(aiDstk--))
          return aiDstk;
#else
        iRes = *(aiDstk--);
        if ( iRes) {
          if (aeaRstk == &eaReturnStack[0]) {

#ifdef WITH_TRACE_LOGICUNIT
            vTrace("\nEnd Logic Unit :: sql %lld\n\n"_Stz,aRluCommon->m_iSql);
#endif
            return iRes;
          }
          aeCode = (char*)(* (--aeaRstk));
        }

#endif
        EO_EXIT(eCode);
        break;//+endCase

      case EO_RETURN_VALUE:
        EO_ENTER(eCode);
        ArgNil(); // return from subroutine and
        dLU0(EO_RETURN);
        //vRlu_ASSERTSTACK(pLU, aiDstk);
#ifdef __TINYC__
        return aiDstk;
#else
        if (aeaRstk == &eaReturnStack[0]) {
          iRes = *(aiDstk--);
#ifdef WITH_TRACE_LOGICUNIT
          vTrace("\nEnd Logic Unit :: sql %lld\n\n"_Stz,aRluCommon->m_iSql);
#endif
          return iRes;
        }
        aeCode = (char*)(* (--aeaRstk));
#endif
        EO_EXIT(eCode);
        break;//+endCase

        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // misc
      case EO_ASSERT:
        EO_ENTER(eCode);
        ArgNil(); // pop top and assert
        iTopA = *(aiDstk--);
        dLU1(EO_ASSERT,iTopA);
        vLib_ASSERT(pLU,(void*)iTopA);
        EO_EXIT(eCode);
        break;//+endCase
              
      case EO_BREAKPOINT:
        EO_ENTER(eCode);
        ArgNil(); // breakpoint
        dLU0(EO_BREAKPOINT);
        vLib_BREAKPOINT(pLU);
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_CALLAGAIN:
        EO_ENTER(eCode);
        ArgNil(); // set call again
        dLU0(EO_CALLAGAIN);
        aRluCommon->m_bCallAgain = 1;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_CALLEDAGAIN:
        EO_ENTER(eCode);
        ArgNil(); // push called again
        dLU0(EO_CALLEDAGAIN);
        *(++aiDstk) = aRluCommon->m_bCalledAgain;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_DUMP:
        EO_ENTER(eCode);
        ArgNil(); // stack dump
        dLU0(EO_DUMP);
        //vRlu_DUMP(pLU,aiDstk);
        EO_EXIT(eCode);
        break;//+endCase
              
      case EO_ENTER_LU:
        EO_ENTER(eCode);
        ArgNil(); //  diarise routine enter
        dLU0(EO_ENTER_LU);
        aRluCommon->m_nEnterCount++;
        vRlu_ENTER(pLU);
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_EXIT_LU:
        EO_ENTER(eCode);
        ArgNil(); // diarise routine exit
        dLU0(EO_EXIT_LU);
        vRlu_EXIT(pLU);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_HANDLER:
        EO_ENTER(eCode);
        GetStr(iArg); // set logicUnit handler
        dLU0(EO_HANDLER);
        vRlu_HANDLER(pLU, (Stz*)iArg);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_INIT:
        EO_ENTER(eCode);
        ArgNil(); // initialise the rlu common
        dLU0(EO_INIT);
        aRluCommon = aRlu_INIT(pLU);
#ifdef __TINYC__
        aiDstk = & aRluCommon->m_iaDataStack[0];
        *aiDstk = 0xFFFFFFFFFFFFFFFF;
        if (nOuterArgs) {
          *(++aiDstk) = iOuterArg0;
        }
#endif
        //printf("[Q%lld.%lld] INIT: %lld %lld\n",(i8)0,(i8)0,
        //       aRluCommon->m_iCmnMagic,aRluCommon->m_iCmnFooter);
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_INTEGRITY:
        EO_ENTER(eCode);
        ArgNil(); // integrity  check
        dLU0(EO_INTEGRITY);
        if (aRluCommon->m_iaDataStack != aiDstk) {
          printf("Dstk growth\n");
          vLib_ASSERT(pLU,(void*)0);
        }
        if (0xFFFFFFFFFFFFFFFF != (u8)aRluCommon->m_iaDataStack[0]) {
          printf("Dstk underflow\n");
          vLib_ASSERT(pLU,(void*)0);
        }
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_NEVERREACH:
        EO_ENTER(eCode);
        ArgNil(); // always assert
        dLU0(EO_NEVERREACH);
        vLib_ASSERT(pLU,0);
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_SANITY:
        EO_ENTER(eCode);
        ArgNil(); // sanity check
        dLU0(EO_SANITY);
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_SLEEP:
        EO_ENTER(eCode);
        ArgNil(); // sleep for top seconds
        iTopA = *(aiDstk--);
        dLU1(EO_SLEEP,iTopA);
        vLib_SLEEP(pLU,iTopA);
        EO_EXIT(eCode);
        break;//+endCase
              
      case EO_SQL:
        EO_ENTER(eCode);
        ArgGet(iArg); //  set sql no
        dLU0(EO_SQL);
        aRluCommon->m_iSql = iArg;
        vRlu_QUERYFRAG(pLU,iArg);
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_TABLEPTN:
        EO_ENTER(eCode);
        ArgNil(); // push i_TablePtn
        dLU1(EO_TABLEPTN,aRluCommon->m_i_TablePtn);
        *(++aiDstk) = aRluCommon->m_i_TablePtn;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_TRACEOFF:
        EO_ENTER(eCode);
        ArgNil(); // set trace off
        aRluCommon->m_bTrace = 0;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_TRACEON:
        EO_ENTER(eCode);
        ArgNil(); // set trace on
        aRluCommon->m_bTrace = 1;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_NOOP:
        EO_ENTER(eCode);
        ArgNil(); // no op
        dLU0(EO_NOOP);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ERROR:
        EO_ENTER(eCode);
        ArgNil(); // error marker
        dLU0(EO_ERROR);
        vLib_ERROR(pLU,"Code compiled with error"_Stz);
        EO_EXIT(eCode);
        break;//+endCase
        

      //------------------------------------------------------------------------
      //------------------------------------------------------------------------

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // date operators
      case EO_DTE_NOW:
        EO_ENTER(eCode);
        ArgNil(); // push current date
        iRes = iLib_DTE_NOW(pLU);
        dLU1(EO_DTE_NOW,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_DTE_PRINT:
        EO_ENTER(eCode);
        ArgNil(); // pop top and print as date
        iTopA = *(aiDstk--);
        szRes = szLib_DTE_PRINT(pLU,iTopA);
        printf("[Q%lld.%lld] %s\n",
               aRluCommon->m_iSql,aRluCommon->m_i_TablePtn,(char*)szRes);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DTE_YEAR:
        EO_ENTER(eCode);
        ArgNil(); // pop top, push year of date
        iTopA = *(aiDstk--);
        iRes = iLib_DTE_YEAR(pLU,iTopA);
        dLU1(EO_DTE_NOW,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // hashing
      case EO_HSH_ADD_HASHI:
        EO_ENTER(eCode);
        ArgNil(); // add hash of top int to running hash
        iTopA = *(aiDstk--);
        dLU1(EO_HSH_ADD_HASHI,iTopA);
        vLib_HSH_ADD_HASHI(pLU,iTopA);
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_HSH_RESET:
        EO_ENTER(eCode);
        ArgNil(); // reset hash
        dLU0(EO_HSH_RESET);
        aRluCommon->m_iHash = 0;
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // partitioning
      case EO_PTN_CALC:
        EO_ENTER(eCode);
        ArgNil(); // calculate m_i_Ptn using running m_iHash
        dLU0(EO_PTN_CALC);
        vLib_PTN_CALC(pLU);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PTN_PUSH:
        EO_ENTER(eCode);
        ArgNil(); // push m_i_Ptn
        dLU1(EO_PTN_PUSH,aRluCommon->m_i_Ptn);
        *(++aiDstk) = aRluCommon->m_i_Ptn;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PTN_RESET:
        EO_ENTER(eCode);
        ArgNil(); // reset running m_iHash
        dLU0(EO_PTN_RESET);
        aRluCommon->m_iHash = 0;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PTN_RUNNING:
        EO_ENTER(eCode);
        ArgNil(); // mix top into running m_iHash
        iTopA = *(aiDstk--);
        dLU0(EO_PTN_RUNNING);
        vLib_PTN_RUNNING(pLU,iTopA);
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // timer
      case EO_TMR_SINCE:
        EO_ENTER(eCode);
        ArgNil(); // get nsecs since ::constructor
        iRes = iRlu_TMR_SINCE(pLU);
        dLU1(EO_TMR_SINCE,iRes);
        *(++aiDstk) = iRes;        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_TMR_START:
        EO_ENTER(eCode);
        ArgNil(); // timer start
        dLU0(EO_TMR_START);
        vRlu_TMR_START(pLU);
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_TMR_NSECS:
        EO_ENTER(eCode);
        ArgNil(); // push timer nanoseconds
        iRes = iRlu_TMR_NSECS(pLU);
        dLU1(EO_TMR_NSECS,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_TMR_UPDATE:
        EO_ENTER(eCode);
        ArgNil(); // timer update
        dLU0(EO_TMR_UPDATE);
        vRlu_TMR_UPDATE(pLU);
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_TMR_AGG_ADD:
        EO_ENTER(eCode);
        ArgNil(); // add timer to agg timer
        dLU0(EO_TMR_AGG_ADD);
        vRlu_TMR_AGG_ADD(pLU);
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_TMR_AGG_NSECS:
        EO_ENTER(eCode);
        ArgNil(); // push agg nanoseconds
        iRes = iRlu_TMR_AGG_NSECS(pLU);
        dLU1(EO_TMR_AGG_NSECS,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_TMR_AGG_RESET:
        EO_ENTER(eCode);
        ArgNil(); // reset agg timer
        dLU0(EO_TMR_AGG_RESET);
        vRlu_TMR_AGG_RESET(pLU);
        EO_EXIT(eCode);
        break;//+endCase
          

      //------------------------------------------------------------------------

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // rlu column set
      case EO_RLU_SET_COLUMN:
        EO_ENTER(eCode);
        ArgGet(iArg); // add column to set
        dLU1(EO_RLU_SET_COLUMN,iArg);
        aRluCommon->m_iaColSet[--aRluCommon->m_i_ColSet] = iArg;
        //*(++aiDstk) = iArg;
        EO_EXIT(eCode);
        break;//+endCase

        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
        // columns set
      case EO_RLU_SET_COLUMNS: {
        EO_ENTER(eCode);
        ArgGet(iArg); // add columns to set
        dLU1(EO_RLU_SET_COLUMNS, iArg);
        i8 iCol = 0;

        for (i8 iBit = 0; iBit < (i8)C_BITS_PER_WORD; iBit++) {
          u8 uBitMap = iArg;
          i8 iToLeft  = 63 - iBit;
          i8 iToRight = iBit;

          // strip off upper bits
          uBitMap <<= iToLeft;
          uBitMap >>= iToLeft;

          // strip off lower bits
          uBitMap >>= iToRight;

          // test if given bit is on
          if (uBitMap) {
            aRluCommon->m_iaColSet[iCol++] = iBit;
          }
        }
        aRluCommon->m_iaColSet[iCol] = C_UNKNOWN;
      }
        EO_EXIT(eCode);
        break;//+endCase

      case EO_RLU_SET_ITEMS:
        EO_ENTER(eCode);
        ArgGet(iArg); // set number of column items
        dLU1(EO_RLU_SET_ITEMS,iArg);
        aRluCommon->m_i_ColSet = iArg;
        aRluCommon->m_iaColSet[aRluCommon->m_i_ColSet] = C_UNKNOWN;
        //*(++aiDstk) = -1;
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // rlu joinrow reset
      case EO_RLU_RESET_JOINEDROW: // reset bJoinedRow in the RLU
        EO_ENTER(eCode);
        ArgNil();
        dLU0(EO_RLU_RESET_JOINEDROW);
        aRluCommon->m_iJoinedRow = 0;
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // rlu joinrow set
      case EO_RLU_SET_JOINEDROW: // set bJoinedRow in the RLU
        EO_ENTER(eCode);
        ArgNil();
        dLU0(EO_RLU_SET_JOINEDROW);
        aRluCommon->m_iJoinedRow = 1;
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // rlu joinmap reset
      case EO_RLU_RESET_JOINMAP:
        EO_ENTER(eCode);
        ArgNil(); // set byte in joinmap if not already set
        dLU0(EO_RLU_RESET_JOINMAP);
        for (i8 i_Byte = 0; i_Byte < aRluCommon->m_nJoinMapCnt; i_Byte++)
          m_xRluCommon.m_baJoinMap[i_Byte] = 0;
        aRluCommon->m_nJoinMapCnt = 0;
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // rlu joinmap set
      case EO_RLU_SET_JOINMAP_BYTE:
        EO_ENTER(eCode);
        ArgNil(); // set byte in joinmap
        dLU0(EO_RLU_SET_JOINMAP_BYTE);
        iTopA = *(aiDstk--);
        aRluCommon->m_baJoinMap[iTopA] = 1;
        aRluCommon->m_nJoinMapCnt++;
        //*(++aiDstk) = iArg;
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // rlu operators
      case EO_RLU_INDEXDEF_ADDCOLUMN:
        EO_ENTER(eCode);
        ArgNil(); // add a column to indexDef in rlu
        dLU0(EO_RLU_INDEXDEF_ADDCOLUMN);
        vDev_RLU_INDEXDEF_ADDCOLUMN(&aiDstk);  // dstk: column name / column type / indexdef
        EO_EXIT(eCode);
        break;//+endCase

      case EO_RLU_INDEXDEF_DELETE:
        EO_ENTER(eCode);
        ArgNil();   // delete the indexDef from rlu
        dLU0(EO_RLU_INDEXDEF_DELETE);
        vDev_RLU_INDEXDEF_DELETE(&aiDstk);     // dstk: indexdef
        aRluCommon->m_pIndexDef = nullptr;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_RLU_INDEXDEF_NEW:
        EO_ENTER(eCode);
        ArgNil();      // new an indexDef in the rlu
        dLU0(EO_RLU_INDEXDEF_NEW);
        vDev_RLU_INDEXDEF_NEW(&aiDstk);        // dstk: name
        aRluCommon->m_pIndexDef = P_PopPtr();
        EO_EXIT(eCode);
        break;//+endCase

      case EO_RLU_INDEXTREE_NEW:
        EO_ENTER(eCode);
        ArgNil();     // new an indexTree in the rlu
        dLU0(EO_RLU_INDEXTREE_NEW);
        vDev_RLU_INDEXTREE_NEW(&aiDstk);       // dstk: indexdef
        aRluCommon->m_pIndexTree = P_PopPtr();
        EO_EXIT(eCode);
        break;//+endCase

      case EO_RLU_ROWPIX_PRINT:
        EO_ENTER(eCode);
        ArgNil();      // print row pix from rlu
        dLU0(EO_RLU_ROWPIX_PRINT);
        vDev_RLU_ROWPIX_PRINT(&aiDstk);        // dstk: rowPix
        EO_EXIT(eCode);
        break;//+endCase

      case EO_RLU_POT_COMPLETED:
        EO_ENTER(eCode);
        ArgNil();     // do we have a full pot buffer ?
        dLU0(EO_RLU_POT_COMPLETED);
        *(++aiDstk) = aRluCommon->m_bPotCompleted;
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // ixt device col operators
      case EO_DEVIXT_COLS_TO_LOCALS:
        EO_ENTER(eCode);
        ArgNil(); // inflate dev columns direct into locals
        dLU0(EO_DEVIXT_COLS_TO_LOCALS);
        //vRlu_DUMP(pLU,aiDstk);
        vDev_IXT_COLS_TO_LOCALS(&aiDstk);      // locals / colset / devRow / devIxt
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_COLS_TO_ROW:
        EO_ENTER(eCode);
        ArgNil();    // inflate dev columns to dev row
        dLU0(EO_DEVIXT_COLS_TO_ROW);
        vDev_IXT_COLS_TO_ROW(&aiDstk);         // devRow / devIxt
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_COLS_TO_ROW_OFS:
        EO_ENTER(eCode);
        ArgNil();  // inflate dev columns to dev row at offset column
        dLU0(EO_DEVIXT_COLS_TO_ROW_OFS);
        vDev_IXT_COLS_TO_ROW_OFS(&aiDstk);         // devRow / devIxt
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // row device col operators
      case EO_DEVROW_COLS_PUSHSET_PST:
        EO_ENTER(eCode);
        ArgNil(); // push column value from dev
        vDev_ROW_COLS_PUSHSET_PST(&aiDstk); // colset / devrow / devixt
        dLU0(EO_DEVROW_COLS_PUSHSET_PST);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_COLS_TO_LOCALS:
        EO_ENTER(eCode);
        ArgNil(); // row buffer columns of row dev into locals
        dLU0(EO_DEVROW_COLS_TO_LOCALS);
        vDev_ROW_COLS_TO_LOCALS(&aiDstk);      // locals / colset / devRow
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_COLS_TO_ROW:
        EO_ENTER(eCode);
        ArgNil(); // row buffer columns of row dev into row
        dLU0(EO_DEVROW_COLS_TO_ROW);
        vDev_ROW_COLS_TO_ROW(&aiDstk);      //  src devRow / dst devRow
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_COLS_TO_ROW_OFS:
        EO_ENTER(eCode);
        ArgNil(); // row buffer columns of row dev into row at offset column
        dLU0(EO_DEVROW_COLS_TO_ROW_OFS);
        vDev_ROW_COLS_TO_ROW_OFS(&aiDstk);      //  src devRow / dst devRow
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_NULS_TO_ROW_OFS:
        EO_ENTER(eCode);
        ArgNil(); // row buffer columns of row dev into row at offset column
        dLU0(EO_DEVROW_NULS_TO_ROW_OFS);
        vDev_ROW_NULS_TO_ROW_OFS(&aiDstk);      //  src devRow / dst devRow
        EO_EXIT(eCode);
        break;//+endCase

        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // ixt device crwbuffer operators
      case EO_DEVIXT_CRWBUF_BEGIN:
        EO_ENTER(eCode);
        ArgNil(); // begin row traverse
        dLU0(EO_DEVIXT_CRWBUF_BEGIN);
        vDev_IXT_CRWBUF_BEGIN(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_CRWBUF_FINDCRW:
        EO_ENTER(eCode);
        ArgNil(); // find crw row given rowno
        dLU0(EO_DEVIXT_CRWBUF_FINDCRW);
        vDev_IXT_CRWBUF_FINDCRW(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_CRWBUF_NEXT:
        EO_ENTER(eCode);
        ArgNil(); // move to next row
        dLU0(EO_DEVIXT_CRWBUF_NEXT);
        vDev_IXT_CRWBUF_NEXT(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_CRWBUF_NULL:
        EO_ENTER(eCode);
        ArgNil(); // null the crw
        dLU0(EO_DEVIXT_CRWBUF_NULL);
        vDev_IXT_CRWBUF_NULL(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_CRWBUF_ROWCNT:
        EO_ENTER(eCode);
        ArgNil(); // push row count of crwbuffer
        dLU0(EO_DEVIXT_CRWBUF_ROWCNT);
        vDev_IXT_CRWBUF_ROWCNT(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_CRWBUF_ROWNO:
        EO_ENTER(eCode);
        ArgNil(); // push rowno of crwbuffer
        dLU0(EO_DEVIXT_CRWBUF_ROWNO);
        vDev_IXT_CRWBUF_ROWNO(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_CRWBUF_TEST:
        EO_ENTER(eCode);
        ArgNil(); // test for crw
        dLU0(EO_DEVIXT_CRWBUF_TEST);
        vDev_IXT_CRWBUF_TEST(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // ixt device entry operators
      case EO_DEVIXT_ENT_DEREF:
        EO_ENTER(eCode);
        ArgNil(); // deref Crw using m_aEnt.ref
        dLU0(EO_DEVIXT_ENT_DEREF);
        vDev_IXT_ENT_DEREF(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_ENT_FINDI:
        EO_ENTER(eCode);
        ArgNil(); // find entry with int key
        dLU0(EO_DEVIXT_ENT_FINDI);
        P_PushPtr(aRluCommon->m_aWrkEnt);
        vDev_IXT_ENT_FIND(&aiDstk);       // devixt / entry
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_ENT_NEXT:
        EO_ENTER(eCode);
        ArgNil(); // move to next m_pEnt
        dLU0(EO_DEVIXT_ENT_NEXT);
        vDev_IXT_ENT_NEXT(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_DEVIXT_ENT_POSPRIOR:
        EO_ENTER(eCode);
        ArgNil(); // position m_pEnt prior using m_oEntWrk.key
        dLU0(EO_DEVIXT_ENT_POSPRIOR);
        P_PushPtr(aRluCommon->m_aWrkEnt);
        vDev_IXT_ENT_POSPRIOR(&aiDstk);      // devixt / entry
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_DEVIXT_ENT_PUSH:
        EO_ENTER(eCode);
        ArgNil(); // push entry on the stack
        dLU0(EO_DEVIXT_ENT_PUSH);
        vDev_IXT_ENT_PUSH(&aiDstk);         // devixt
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_DEVIXT_ENT_PUSHELMI0:
        EO_ENTER(eCode);
        ArgNil(); // push entry elm 0 int
        dLU0(EO_DEVIXT_ENT_PUSHELMI0);
        vDev_IXT_ENT_PUSHELMI0(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_DEVIXT_ENT_PUSHELMS0:
        EO_ENTER(eCode);
        ArgNil(); // push entry elm 0 str
        dLU0(EO_DEVIXT_ENT_PUSHELMS0);
        vDev_IXT_ENT_PUSHELMS0(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_DEVIXT_ENT_PUSHUPPER:
        EO_ENTER(eCode);
        ArgNil(); // push entry on the stack
        dLU0(EO_DEVIXT_ENT_PUSHUPPER);
        vDev_IXT_ENT_PUSHUPPER(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // ixt device entry relational operators
      case EO_DEVIXT_ENTKEY_LESS:
        EO_ENTER(eCode);
        ArgNil(); // test if dev m_pEnt.key <  ent.key
        vDev_IXT_ENT_COMPARE(&aiDstk);      // devixt / entry
        iCmp = *(aiDstk--);
        iRes = (KC_BEFORE == iCmp);
        dLU1(EO_DEVIXT_ENTKEY_LESS, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_DEVIXT_ENTKEY_LESSEQUAL:
        EO_ENTER(eCode);
        ArgNil(); // test if dev m_pEnt.key <= m_oEntWrk.key
        vDev_IXT_ENT_COMPARE(&aiDstk);           // devixt / entry
        iCmp = *(aiDstk--);
        iRes = (KC_BEFORE == iCmp || KC_SAME == iCmp);
        dLU1(EO_DEVIXT_ENTKEY_LESSEQUAL, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_DEVIXT_ENTKEY_EQUAL:
        EO_ENTER(eCode);
        ArgNil(); // test if dev m_pEnt.key == m_oEntWrk.key
        vDev_IXT_ENT_COMPARE(&aiDstk);       // devixt / entry
        iCmp = *(aiDstk--);
        iRes = (KC_SAME == iCmp);
        dLU1(EO_DEVIXT_ENTKEY_EQUAL, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_DEVIXT_ENTKEY_NOTEQUAL:
        EO_ENTER(eCode);
        ArgNil(); // test if dev m_pEnt.key != m_oEntWrk.key
        vDev_IXT_ENT_COMPARE(&aiDstk);          // devixt / entry
        iCmp = *(aiDstk--);
        iRes = (KC_SAME != iCmp);
        dLU1(EO_DEVIXT_ENTKEY_NOTEQUAL, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_DEVIXT_ENTKEY_GREATEREQUAL:
        EO_ENTER(eCode);
        ArgNil(); // test if dev m_pEnt.key >= m_oEntWrk.key
        vDev_IXT_ENT_COMPARE(&aiDstk);              // devixt / entry
        iCmp = *(aiDstk--);
        iRes = (KC_AFTER == iCmp || KC_SAME == iCmp);
        dLU1(EO_DEVIXT_ENTKEY_GREATEREQUAL, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_DEVIXT_ENTKEY_GREATER:
        EO_ENTER(eCode);
        ArgNil(); // test if dev m_pEnt.key >  m_oEntWrk.key
        vDev_IXT_ENT_COMPARE(&aiDstk);         // devixt / entry
        iCmp = *(aiDstk--);
        iRes = (KC_AFTER == iCmp);
        dLU1(EO_DEVIXT_ENTKEY_GREATER, iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_ENTKEY_PRINTKEYS:
        EO_ENTER(eCode);
        ArgNil(); // print dev m_pEnt.key and other working key
        vDev_IXT_ENT_PRINTKEYS(&aiDstk);         // devixt / entry
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // ixt device entwrk operators
      case EO_ENTWRK_PRINT:
        EO_ENTER(eCode);
        ArgNil(); // print m_oEntWrk
        dLU0(EO_ENTWRK_PRINT);
        vDev_ENTWRK_PRINT(&aiDstk);   // entry
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ENTWRK_PUSHELMI:
        EO_ENTER(eCode);
        ArgGet(iArg); // push element of entwrk
        dLU0(EO_ENTWRK_PUSHELMI);
        *(++aiDstk) = iArg;
        vDev_ENTWRK_PUSHELMI(&aiDstk);   // entry
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ENTWRK_PUSHELMS:
        EO_ENTER(eCode);
        ArgGet(iArg); // push string element of entwrk
        dLU0(EO_ENTWRK_PUSHELMS);
        *(++aiDstk) = iArg;
        vDev_ENTWRK_PUSHELMS(&aiDstk);   // entry
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ENTWRK_RESET:
        EO_ENTER(eCode);
        ArgNil(); // reset m_oEntWrk
        dLU0(EO_ENTWRK_RESET);
        vDev_ENTWRK_RESET(&aiDstk);   // entry
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ENTWRK_SETENTKND:
        EO_ENTER(eCode);
        ArgNil(); // set the ent kind
        dLU0(EO_ENTWRK_SETENTKND);
        vDev_ENTWRK_SETENTKND(&aiDstk);   // int /entry
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ENTWRK_SETINTELM:
        EO_ENTER(eCode);
        ArgNil(); // append next element value (int)
        dLU0(EO_ENTWRK_SETINTELM);
        vDev_ENTWRK_SETINTELM(&aiDstk);   // int /entry
        EO_EXIT(eCode);
        break;//+endCase

      case EO_ENTWRK_SETSTZELM:
        EO_ENTER(eCode);
        ArgNil(); // append next element value (stz)
        dLU0(EO_ENTWRK_SETSTZELM);
        vDev_ENTWRK_SETSTZELM(&aiDstk);   // stz /entry
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // ixt device index operators
      case EO_DEVIXT_IDX_DEL:
        EO_ENTER(eCode);
        ArgNil(); // delete current m_aEnt from indexTree
        dLU0(EO_DEVIXT_IDX_DEL);
        vDev_IXT_IDX_DEL(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_IDX_DUMP:
        EO_ENTER(eCode);
        ArgNil(); // dump index tree
        dLU0(EO_DEVIXT_IDX_DUMP);
        vDev_IXT_IDX_DUMP(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_IDX_ENTRIES:
        EO_ENTER(eCode);
        ArgNil(); // push number of entries in dev index
        dLU0(EO_DEVIXT_IDX_ENTRIES);
        vDev_IXT_IDX_ENTRIES(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_IDX_INS:
        EO_ENTER(eCode);
        ArgNil(); // insert an entry into the indexTree
        dLU0(EO_DEVIXT_IDX_INS);
        vDev_IXT_IDX_INS(&aiDstk);      // devixt/ entry
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_IDX_BEGIN:
        EO_ENTER(eCode);
        ArgNil(); // begin linear traverse
        dLU0(EO_DEVIXT_IDX_BEGIN);
        vDev_IXT_IDX_BEGIN(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_IDX_NEXT:
        EO_ENTER(eCode);
        ArgNil(); // traverse to next entry
        dLU0(EO_DEVIXT_IDX_NEXT);
        vDev_IXT_IDX_NEXT(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_DEVIXT_LBOUND_GE:
        EO_ENTER(eCode);
        ArgNil(); // pos in buffer to entry before LBOUND
        dLU0(EO_DEVIXT_LBOUND_GE);
        vDev_IXT_POS_LBOUND_GE(&aiDstk);  // entry / devixt
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_LBOUND_GT:
        EO_ENTER(eCode);
        ArgNil(); // pos in buffer to entry before LBOUND
        dLU0(EO_DEVIXT_LBOUND_GT);
        vDev_IXT_POS_LBOUND_GT(&aiDstk);  // entry / devixt
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_UBOUND_LE:
        EO_ENTER(eCode);
        ArgNil(); // pos in buffer to entry for UBOUND
        dLU0(EO_DEVIXT_UBOUND_LE);
        vDev_IXT_POS_UBOUND_LE(&aiDstk);      // entry / devixt
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_DEVIXT_UBOUND_LT:
        EO_ENTER(eCode);
        ArgNil(); // pos in buffer to entry for UBOUND
        dLU0(EO_DEVIXT_UBOUND_LT);
        vDev_IXT_POS_UBOUND_LT(&aiDstk);  // entry / devixt
        EO_EXIT(eCode);
        break;//+endCase
        
      case EO_DEVIXT_CMP_KEYS:
        EO_ENTER(eCode);
        ArgNil(); // compare keys of two devices
        dLU0(EO_DEVIXT_CMP_KEYS);
        vDev_IXT_CMP_KEYS(&aiDstk);     // entry / entry
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_POS_BYKEY:
        EO_ENTER(eCode);
        ArgNil(); // position alt ixt using src.key
        dLU0(EO_DEVIXT_POS_BYKEY);
        vDev_IXT_POS_BYKEY(&aiDstk);      // devixt / entry
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_ENT_PRINT:
        EO_ENTER(eCode);
        ArgNil(); // print ent
        dLU0(EO_DEVIXT_ENT_PRINT);
        vDev_IXT_ENT_PRINT(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // ixt device index set operators
      case EO_DEVIXT_INDEXNM_SET:
        EO_ENTER(eCode);
        GetStr(iArg); // ALKA: set the index name
        dLU0(EO_DEVIXT_INDEXNM_SET);
        S_PushStr(iArg);
        vDev_IXT_INDEXNM_SET(&aiDstk);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_INDEXTREE_SET:
        EO_ENTER(eCode);
        ArgNil(); // set indexTree from indexTree[]
        dLU0(EO_DEVIXT_INDEXTREE_SET);
        vDev_IXT_INDEXTREE_SET(&aiDstk);      // indextree / devixt
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // row device entWrk operators
      case EO_DEVROW_ENTWRK_APPENDELMI:
        EO_ENTER(eCode);
        ArgNil(); // append next element value (int)
        dLU0(EO_DEVROW_ENTWRK_APPENDELMI);
        vDev_ROW_ENTWRK_APPENDELMI(&aiDstk);      // entry / devrow
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_ENTWRK_APPENDELMS:
        EO_ENTER(eCode);
        ArgNil(); // append next element value (str)
        dLU0(EO_DEVROW_ENTWRK_APPENDELMS);
        vDev_ROW_ENTWRK_APPENDELMS(&aiDstk);      // entry / devrow
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_ENTWRK_CREATEPAY:
        EO_ENTER(eCode);
        ArgGet(iArg); // ALKA: create a payload in entwrk
        dLU1(EO_DEVROW_ENTWRK_CREATEPAY,iArg);
        *(++aiDstk) = iArg;
        vDev_ROW_ENTWRK_CREATEPAY(&aiDstk);          // entry / devrow
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_TABLENM_SET:
        EO_ENTER(eCode);
        GetStr(iArg); // ALKA: set tablenm in dev row
        dLU0(EO_DEVROW_TABLENM_SET);
        S_PushStr(iArg);
        vDev_ROW_TABLENM_SET(&aiDstk);
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // row device row operators
      case EO_DEVROW_ROW_NEW:
        EO_ENTER(eCode);
        ArgNil(); // create new row
        dLU0(EO_DEVROW_ROW_NEW);
        //vRlu_DUMP(pLU, aiDstk);
        vDev_ROW_ROW_NEW(&aiDstk);     // devrow / pot /ptn
        aRluCommon->m_bPotCompleted = *(aiDstk--);
        //vRlu_DUMP(pLU, aiDstk);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_ROW_PRINT:
        EO_ENTER(eCode);
        ArgNil(); // print row
        dLU0(EO_DEVROW_ROW_PRINT);
        vDev_ROW_ROW_PRINT(&aiDstk);      // devrow
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_ROW_SET:
        EO_ENTER(eCode);
        ArgNil(); // set dst row
        dLU0(EO_DEVROW_ROW_SET);
        vDev_ROW_ROW_SET(&aiDstk);      // rowNo / devrow
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_ROW_TEST:
        EO_ENTER(eCode);
        ArgNil(); // test for row
        dLU0(EO_DEVROW_ROW_TEST);
        vDev_ROW_ROW_TEST(&aiDstk);      // devrow
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_ROW_ZERO:
        EO_ENTER(eCode);
        ArgNil(); // zero device row
        dLU0(EO_DEVROW_ROW_ZERO);
        vDev_ROW_ROW_ZERO(&aiDstk);      // devrow
        EO_EXIT(eCode);
        break;//+endCase

       // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // row device row buffer operators
      case EO_DEVROW_ROWBUF_BEGIN:
        EO_ENTER(eCode);
        ArgNil(); // begin row traverse
        dLU0(EO_DEVROW_ROWBUF_BEGIN);
        vDev_ROW_ROWBUF_BEGIN(&aiDstk);      // devrow
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_ROWBUF_FINDROW:
        EO_ENTER(eCode);
        ArgNil(); // find crw row given rowno
        dLU0(EO_DEVROW_ROWBUF_FINDROW);
        vDev_ROW_ROWBUF_FINDROW(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_ROWBUF_NEXT:
        EO_ENTER(eCode);
        ArgNil(); // move to next row
        dLU0(EO_DEVROW_ROWBUF_NEXT);
        vDev_ROW_ROWBUF_NEXT(&aiDstk);      // devrow
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_ROWBUF_NULL:
        EO_ENTER(eCode);
        ArgNil(); // null the buffer
        dLU0(EO_DEVROW_ROWBUF_NULL);
        vDev_ROW_ROWBUF_NULL(&aiDstk);      // devrow
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_ROWBUF_RESET:
        EO_ENTER(eCode);
        ArgNil(); // reset rowslab
        dLU0(EO_DEVROW_ROWBUF_RESET);
        vDev_ROW_ROWBUF_RESET(&aiDstk);      // devrow
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_ROWBUF_ROWCNT:
        EO_ENTER(eCode);
        ArgNil(); // push row count of rowbuffer
        dLU0(EO_DEVROW_ROWBUF_ROWCNT);
        vDev_ROW_ROWBUF_ROWCNT(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_ROWBUF_ROWNO:
        EO_ENTER(eCode);
        ArgNil(); // push rowno of crwbuffer
        dLU0(EO_DEVROW_ROWBUF_ROWNO);
        vDev_ROW_ROWBUF_ROWNO(&aiDstk);      // devixt
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_ROWBUF_TEST:
        EO_ENTER(eCode);
        ArgNil(); // test for buffer
        dLU0(EO_DEVROW_ROWBUF_TEST);
        vDev_ROW_ROWBUF_TEST(&aiDstk);      // devrow
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      //  row device column operators
      case EO_DEVROW_POPCOLI:
        EO_ENTER(eCode);
        ArgGet(iArg); // ALKA: pop and deposit top into dev column
        *(++aiDstk) = iArg;
        dLU2(EO_DEVROW_POPCOLI,iArg,iTopA);
        vDev_ROW_POPCOLI(&aiDstk);          // columnNo / devrow
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_POPCOLS:
        EO_ENTER(eCode);
        ArgGet(iArg); // ALKA : pop and deposit top into dev column
        *(++aiDstk) = iArg;
        dLU1(EO_DEVROW_POPCOLS,iArg);
        vDev_ROW_POPCOLS(&aiDstk);         // columnNo / devrow
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_PUSHCOLI:
        EO_ENTER(eCode);
        ArgGet(iArg); // ALKA: push column
        dLU1(EO_DEVROW_PUSHCOLI,iArg);
        *(++aiDstk) = iArg;
        vDev_ROW_PUSHCOLI(&aiDstk);         // columnNo / devrow
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVROW_PUSHCOLS:
        EO_ENTER(eCode);
        ArgGet(iArg); // ALKA: push column
        dLU1(EO_DEVROW_PUSHCOLS,iArg);
        *(++aiDstk) = iArg;
        vDev_ROW_PUSHCOLS(&aiDstk);          // columnNo / devrow
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // pay load operators
      case EO_PAY_ADDRESS:
        EO_ENTER(eCode);
        ArgNil(); // get payload address
        dLU0(EO_PAY_ADDRESS);
        vDev_PAY_ADDRESS(&aiDstk);
        aiPayload = (i8*)*(aiDstk--);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PAY_FETCH_COLI:
        EO_ENTER(eCode);
        ArgGet(iArg); // fetch a column from entry payload
        iRes = aiPayload[iArg];
        dLU2(EO_PAY_FETCH_COLI,iArg,iRes);
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PAY_FETCH_COLS:
        EO_ENTER(eCode);
        ArgGet(iArg); // fetch a column from entry payload
        if (aiPayload[iArg] == C_DATUM_NULL) {
          *(++aiDstk) = C_DATUM_NULL;
        }
        else {
          azSrc = (char *) &aiPayload[iArg];
          nSize = strlen(azSrc) + 1;
          //ASSERT(nSize < 8);
          dLU2(EO_PAY_FETCH_COLS, iArg, (i8) azSrc);
          S_PushStr(azSrc);
        }
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PAY_STORE_COLI:
        EO_ENTER(eCode);
        ArgGet(iArg); // store a column into entry payload
        iTopA = *(aiDstk--);
        dLU2(EO_PAY_STORE_COLI,iArg,iTopA);
        aiPayload[iArg] = iTopA;
        EO_EXIT(eCode);
        break;//+endCase

      case EO_PAY_STORE_COLS:
        EO_ENTER(eCode);
        //vRlu_DUMP(pLU, aiDstk);
        ArgGet(iArg); // store a column into entry payload
        dLU1(EO_PAY_STORE_COLS,iArg);
        azDst = (char*)&aiPayload[iArg];
        S_PopStr(azDst);
        //vRlu_DUMP(pLU, aiDstk);
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // -- TO GO --
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // set int element
      case EO_ENTWRK_SETINTELM_OLD:
        EO_ENTER(eCode);
        ArgNil(); // append next element value (int)
        dLU0(EO_ENTWRK_SETINTELM_OLD);
        vDev_ENTWRK_SETINTELM_OLD(&aiDstk);   // int /entry
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // ixt device col operators
      case EO_DEVIXT_COL_PUSHSET_ADR:
        EO_ENTER(eCode);
        ArgNil(); // push column value from dev crw
        vDev_IXT_COL_PUSHSET_ADR(&aiDstk);
        dLU0(EO_DEVIXT_COL_PUSHSET_ADR);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_COL_PUSHSET_PST:
        EO_ENTER(eCode);
        ArgNil(); // push column value from dev
        vDev_IXT_COL_PUSHSET_PST(&aiDstk); // colset / devrow / devixt
        dLU0(EO_DEVIXT_COL_PUSHSET_PST);
        //vRlu_DUMP(pLU, aiDstk);
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // key
      case EO_KEY_LCL2C_TO_I:
        EO_ENTER(eCode);
        ArgGet(iArg); // convert two lcl varchar(1) to int
        dLU1(EO_KEY_LCL2C_TO_I,iArg);
        aiLocals = aRluCommon->m_iaLocals;
//        azTopA = (char*)aiLocals[giaVectorSet[iArg]];
//        azTopB = (char*)aiLocals[giaVectorSet[iArg+1]];
//        iRes = *azTopA * 256 + *azTopB;
//        *(++aiDstk) = iRes;

        *(++aiDstk) = *(char*)aiLocals[giaVectorSet[iArg]] * 256
                      + *(char*)aiLocals[giaVectorSet[iArg+1]];
        EO_EXIT(eCode);
        break;//+endCase

      case EO_KEY_ROW2C_TO_I:
        EO_ENTER(eCode);
        ArgGet(iArg); // convert two row varchar(1) to int
        dLU1(EO_KEY_ROW2C_TO_I,iArg);
        azTopA = (char*)szDev_ROW_PUSHCOLS(pLU,aRluCommon->m_pDevLftRow,
                                           giaVectorSet[iArg]);
        azTopB = (char*)szDev_ROW_PUSHCOLS(pLU,aRluCommon->m_pDevLftRow,
                                           giaVectorSet[iArg+1]);
        iRes = *azTopA * 256 + *azTopB;
        *(++aiDstk) = iRes;
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // register operators
      case EO_REG_EOSMARKER_NEW:
        EO_ENTER(eCode);
        ArgNil(); // new an EosMarker in OperandA[]
        dLU0(EO_REG_EOSMARKER_NEW);
        vDev_REG_EOSMARKER_NEW(pLU);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_REG_PTNSTREAM_CLOSE:
        EO_ENTER(eCode);
        ArgNil(); // close ptnStream[]
        dLU0(EO_REG_PTNSTREAM_CLOSE);
        vDev_REG_PTNSTREAM_CLOSE(pLU);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_REG_PTNSTREAM_DELETE:
        EO_ENTER(eCode);
        ArgNil(); // delete ptnStream[]
        dLU0(EO_REG_PTNSTREAM_DELETE);
        vDev_REG_PTNSTREAM_DELETE(pLU);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_REG_PTNSTREAM_NEW:
        EO_ENTER(eCode);
        ArgNil(); // new an ptnStream in ptnStream[]
        dLU0(EO_REG_PTNSTREAM_NEW);
        vDev_REG_PTNSTREAM_NEW(pLU);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_REG_PTNSTREAM_OPEN:
        EO_ENTER(eCode);
        ArgNil(); // new an ptnStream in ptnStream[]
        dLU0(EO_REG_PTNSTREAM_OPEN);
        vDev_REG_PTNSTREAM_OPEN(pLU);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_REG_PTNSTREAM_READ:
        EO_ENTER(eCode);
        ArgNil(); // read from ptnStream[]
        dLU0(EO_REG_PTNSTREAM_READ);
        vDev_REG_PTNSTREAM_READ(pLU);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_REG_STREAMDEF_STREAMDEFOUT:
        EO_ENTER(eCode);
        ArgNil(); // new a streamDefOut[] from StreamDefLft[]
        dLU0(EO_REG_STREAMDEF_STREAMDEFOUT);
        vDev_REG_STREAMDEF_STREAMDEFOUT(pLU);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_REG_STREAMDEFOUT_NEW:
        EO_ENTER(eCode);
        ArgNil(); // new a streamDef in streamDefOut[]
        dLU0(EO_REG_STREAMDEFOUT_NEW);
        vDev_REG_STREAMDEFOUT_NEW(pLU);
        EO_EXIT(eCode);
        break;//+endCase

      case EO_REG_STREAMDEFOUT_ADDCOLUMN:
        EO_ENTER(eCode);
        GetStr(iArg); // add a column to streamDefOut[]
        dLU0(EO_REG_STREAMDEFOUT_ADDCOLUMN);
        iTopA = *(aiDstk--); // column type
        iTopB = *(aiDstk--); // column length
        iTopC = *(aiDstk--); // column scale
        vDev_REG_STREAMDEFOUT_ADDCOLUMN(pLU, (Stz*)iArg, iTopA, iTopB, iTopC);
        EO_EXIT(eCode);
        break;//+endCase

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      // super fragments
      case EO_SUPER_Q100: {
        EO_ENTER(eCode);
        ArgNil(); // super frag for Q100
        i8 iWrkA, iWrkB, iWrkC;
        aiLocals = aRluCommon->m_iaLocals;

        iWrkA = aiLocals[Q1_LOCAL_EXTENDEDPRICE];
        iWrkB = aiLocals[Q1_LOCAL_DISCOUNT];
        iWrkC = iWrkA * (100 - iWrkB);

        aiPayload[Q100_SUM_QTY] += aiLocals[Q1_LOCAL_QUANTITY];

        aiPayload[Q100_SUM_BASE_PRICE] += iWrkA;

        aiPayload[Q100_SUM_DISC_PRICE] += ((iWrkC) / 100);

        aiPayload[Q100_SUM_CHARGE] +=
            ((iWrkC) * (100 + aiLocals[Q1_LOCAL_TAX])) / 10000;

        aiPayload[Q100_COUNT_QTY]++;

        aiPayload[Q100_COUNT_PRICE]++;

        aiPayload[Q100_SUM_DISC] += iWrkB;

        aiPayload[Q100_COUNT_DISC]++;

        aiPayload[Q100_COUNT_ORDER]++;
      }
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_LBOUND_GE_OLD:
        EO_ENTER(eCode);
        ArgNil(); // pos in buffer to entry before LBOUND
        dLU0(EO_DEVIXT_LBOUND_GE_OLD);
        vDev_IXT_POS_LBOUND_GE_OLD(&aiDstk);  // entry / devixt
        EO_EXIT(eCode);
        break;//+endCase

      case EO_DEVIXT_UBOUND_LT_OLD:
        EO_ENTER(eCode);
        ArgNil(); // pos in buffer to entry for UBOUND
        dLU0(EO_DEVIXT_UBOUND_LT_OLD);
        vDev_IXT_POS_UBOUND_LT_OLD(&aiDstk);  // entry / devixt
        EO_EXIT(eCode);
        break;//+endCase

        // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      default:
        vTrace("Unknown eo opcode %lld\n"_Stz,(i8)eCode);
        RASSERT(0);
        return C_DATUM_NULL;
    } //+ExtractEnd
    
  }
  
  return -1;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
