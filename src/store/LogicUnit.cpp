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

,          m_xpFile0(this,RF_REPAIR_DELETE,File::vRepair,           LR_FILE0)
,          m_xpFile1(this,RF_REPAIR_DELETE,File::vRepair,           LR_FILE1)

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

  RASSERT(m_xpFile0.bIsNull());
  RASSERT(m_xpFile1.bIsNull());
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
