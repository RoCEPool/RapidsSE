/*============================================================================*/
/* LogicUnit.cpp -- Logic Unit                                                */
/*============================================================================*/

#include "BASE.h"

#include "eED.h"
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

,      m_xpAdaptorWL(this,RF_REPAIR_DELETE, AdaptorWL::vRepair,          LR_ADAPTORWL)
,       m_xpAggTimer(this,RF_REPAIR_DELETE,  AggTimer::vRepair,           LR_AGGTIMER)
,   m_xpAggTypeStack(this,RF_REPAIR_DELETE,  IntStack::vRepair,       LR_AGGTYPESTACK)
,       m_xpAnswerWL(this,RF_REPAIR_DELETE,  AnswerWL::vRepair,           LR_ANSWERWL)
,    m_xpPreExpStack(this,RF_REPAIR_DELETE,  IntStack::vRepair,        LR_PREEXPSTACK)

,      m_xpBktMapGet(this,RF_REPAIR_DELETE, BktMapGet::vRepair,             LR_BKTMAP)
,         m_xpBucket(this,RF_REPAIR_DELETE,    Bucket::vRepair,             LR_BUCKET)
,      m_xpBucketGet(this,RF_REPAIR_DELETE, BucketGet::vRepair,          LR_BUCKETGET)
,     m_xpBucketInfo(this,RF_REPAIR_DELETE,BucketInfo::vRepair,         LR_BUCKETINFO)

,       m_xpCombined(this,RF_REPAIR_DELETE,  Combined::vRepair,           LR_COMBINED)
,      m_xpCrwBuffer(this,RF_REPAIR_DELETE, CrwBuffer::vRepairList,      LR_CRWBUFFER)
,       m_xpCrwToRow(this,RF_REPAIR_DELETE,  CrwToRow::vRepair,           LR_CRWTOROW)
,      m_xpCsvBuffer(this,RF_REPAIR_DELETE, CsvBuffer::vRepairList,      LR_CSVBUFFER)

,     m_xpDataBuffer(this,RF_REPAIR_DELETE,DataBuffer::vRepair,         LR_DATABUFFER)
,           m_xpDeck(this,RF_REPAIR_DELETE,      Deck::vRepair,         LR_DATABUFFER)
,           m_xpDir0(this,RF_REPAIR_DELETE,       Dir::vRepair,               LR_DIR0)
,           m_xpDir1(this,RF_REPAIR_DELETE,       Dir::vRepair,               LR_DIR1)
,         m_xpDynamo(this,RF_REPAIR_DELETE,    Dynamo::vRepair,             LR_DYNAMO)

,    m_xpErrorMarker(this,RF_REPAIR_DELETE,   Operand::vRepair,        LR_ERRORMARKER)

,          m_xpFile0(this,RF_REPAIR_DELETE,      File::vRepair,              LR_FILE0)
,          m_xpFile1(this,RF_REPAIR_DELETE,      File::vRepair,              LR_FILE1)

,           m_xpGrid(this,RF_REPAIR_DELETE,     Parcel::vRepair,              LR_GRID)

,        m_xpHandler(this,RF_REPAIR_NONE,  nullptr,                        LR_HANDLER)

,          m_xpIndex(this,RF_SET_NULL,     nullptr,                          LR_INDEX)
,       m_xpIndexDef(this,RF_SET_NULL,     nullptr,                       LR_INDEXDEF)
,    m_xpIndexStream(this,RF_REPAIR_DELETE,IndexStream::vRepair,       LR_INDEXSTREAM)
,        m_xpInfoSet(this,RF_REPAIR_DELETE,    InfoSet::vRepair,           LR_INFOSET)
,     m_xpInfoSetRow(this,RF_SET_NULL,     nullptr,                     LR_INFOSETROW)
,   m_xpInfoSetTable(this,RF_SET_NULL,     nullptr,                   LR_INFOSETTABLE)
,       m_xpIntStack(this,RF_REPAIR_DELETE,   IntStack::vRepair,          LR_INTSTACK)

,     m_xpJobMetrics(this,RF_REPAIR_DELETE, JobMetrics::vRepair,        LR_JOBMETRICS)

,           m_xpList(this,RF_REPAIR_DELETE,  RowSingle::vRepairList,          LR_LIST)

,         m_xpMarker(this,RF_SET_NULL,     nullptr,                         LR_MARKER)
,      m_xpMemoryHog(this,RF_REPAIR_DELETE,  RowBuffer::vRepairList,     LR_MEMORYHOG)
,        m_xpMetaGet(this,RF_REPAIR_DELETE,    MetaGet::vRepair,              LR_META)

,    m_xpNamedBuffer(this,RF_REPAIR_DELETE,NamedBuffer::vRepairList,   LR_NAMEDBUFFER)
,           m_xpNode(this,RF_REPAIR_NEVER, nullptr,                           LR_NODE)
,         m_xpNode_I(this,RF_SET_NULL,     nullptr,                         LR_NODE_I)
,            m_xpNow(this,RF_REPAIR_DELETE,        Now::vRepair,               LR_NOW)

,             m_xpOp(this,RF_NONE,         nullptr,                             LR_OP)
,     m_xpOperandOoO(this,RF_REPAIR_NONE,  nullptr,                     LR_OPERANDOOO)
,      m_xpOptimiser(this,RF_REPAIR_DELETE,nullptr,                      LR_OPTIMISER)

, m_xpPathIdentifier(this,RF_REPAIR_DELETE,PathIdentifier::vRepair, LR_PATHIDENTIFIER)
,         m_xpParcel(this,RF_REPAIR_DELETE,        Parcel::vRepair,         LR_PARCEL)
,         m_xpPipein(this,RF_REPAIR_NONE,  nullptr,                         LR_PIPEIN)
,       m_xpPipeline(this,RF_NONE,         nullptr,                       LR_PIPELINE)
,    m_xpPlanOperand(this,RF_REPAIR_DELETE,   PlanOperand::vRepair,    LR_PLANOPERAND)
,            m_xpPot(this,RF_REPAIR_DELETE,           Pot::vRepair,            LR_POT)
,      m_xpPtnStream(this,RF_REPAIR_DELETE,     PtnStream::vRepair,      LR_PTNSTREAM)

,    m_xpRecvdObject(this,RF_REPAIR_NONE,  nullptr,                    LR_RECVDOBJECT)
,         m_xpRecvWL(this,RF_REPAIR_NONE,  nullptr,                         LR_RECVWL)
,      m_xpRowBuffer(this,RF_REPAIR_DELETE,     RowBuffer::vRepairList,  LR_ROWBUFFER)
,        m_xpRowOnce(this,RF_REPAIR_DELETE,       RowOnce::vRepair,        LR_ROWONCE)

,       m_xpSendNode(this,RF_SET_NULL,     nullptr,                       LR_SENDNODE)
,          m_xpSends(this,RF_REPAIR_NONE,  nullptr,                          LR_SENDS)
,         m_xpSendWL(this,RF_REPAIR_NONE,  nullptr,                         LR_SENDWL)
,       m_xpShardDef(this,RF_REPAIR_DELETE,      ShardDef::vRepair,       LR_SHARDDEF)
,         m_xpSocket(this,RF_REPAIR_NEVER, nullptr,                         LR_SOCKET)
,          m_xpStage(this,RF_SET_NULL,     nullptr,                          LR_STAGE)
,          m_xpStats(this,RF_REPAIR_DELETE,nullptr,                          LR_STATS)
,        m_xpStencil(this,RF_REPAIR_DELETE,       Stencil::vRepair,        LR_STENCIL)
,         m_xpStream(this,RF_SET_NULL,     nullptr,                          LR_STAGE)
,   m_xpStreamDefLft(this,RF_REPAIR_DELETE,     StreamDef::vRepair,   LR_STREAMDEFLFT)
,   m_xpStreamDefOut(this,RF_REPAIR_DELETE,     StreamDef::vRepair,   LR_STREAMDEFOUT)
,   m_xpStreamDefRht(this,RF_REPAIR_DELETE,  StreamDefRht::vRepair,   LR_STREAMDEFRHT)
,      m_xpStreamSet(this,RF_REPAIR_DELETE,     StreamDef::vRepair,      LR_STREAMSET)
, m_xpStreamSetTable(this,RF_SET_NULL,     nullptr,                 LR_STREAMSETTABLE)
,        m_xpSubCall(this,RF_REPAIR_DELETE,       SubCall::vRepair,        LR_SUBCALL)
,        m_xpSubPipe(this,RF_NONE,         nullptr,                        LR_SUBPIPE)
,      m_xpSubReturn(this,RF_REPAIR_DELETE,     SubReturn::vRepair,      LR_SUBRETURN)
,         m_xpSubset(this,RF_REPAIR_DELETE,       Operand::vRepairList,     LR_SUBSET)

,          m_xpTable(this,RF_SET_NULL,     nullptr,                          LR_TABLE)
,       m_xpTablePtn(this,RF_SET_NULL,     nullptr,                       LR_TABLEPTN)
,         m_xpTagSet(this,RF_REPAIR_DELETE,        TagSet::vRepair,         LR_TAGSET)
,          m_xpTimer(this,RF_REPAIR_DELETE,         Timer::vRepair,          LR_TIMER)
,          m_xpTinyC(this,RF_REPAIR_DELETE,         TinyC::vRepair,          LR_TINYC)
,    m_xpToLglPtnCrw(this,RF_REPAIR_DELETE,   ToLglPtnCrw::vRepairHashSet,LR_TOLGLPTNCRW)
,    m_xpToLglPtnRow(this,RF_REPAIR_DELETE,   ToLglPtnRow::vRepairHashSet,LR_TOLGLPTNROW)
,       m_xpToNstRow(this,RF_REPAIR_DELETE,      ToLglPtnNst::vRepair,       LR_TONSTROW)
,          m_xpToRow(this,RF_REPAIR_DELETE,         ToRow::vRepairList,      LR_TOROW)
,            m_xpTub(this,RF_REPAIR_DELETE,           Tub::vRepair,            LR_TUB)
,             m_xpTx(this,RF_REPAIR_DELETE,            Tx::vRepair,             LR_TX)

,         m_xpVertex(this,RF_REPAIR_DELETE,        Vertex::vRepair,         LR_VERTEX)
,     m_xpVertexSpec(this,RF_REPAIR_DELETE,    VertexSpec::vRepair,     LR_VERTEXSPEC)

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

  m_oSince.vStart();
  
  // m_oEntWrk

  m_uSelectColumns = 0;

  m_xRluCommon.m_pPot = nullptr;

  m_eErrorCode_reg = EC_NONE;
  
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
  RASSERT(m_xpToLglPtnCrw.bIsNull());
  RASSERT(m_xpToLglPtnRow.bIsNull());
  RASSERT(m_xpToNstRow.bIsNull());
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

//  if (!m_pRluDiary)
//    m_pRluDiary = new RluDiary;
//
//
//  m_pRluDiary->vAddNote(m_xRluCommon.m_iSql,m_xRluCommon.m_i_TablePtn,
//                        nQueueLength,
//
//                        i_EnumGroup,i_Enum,iItem,nNanoSecs,
//                        iAction,i_ActEnumGroup,i_ActEnum);
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
  
//    vDiaryNote(EG_ED,ED_ENTER,
//                m_xRluCommon.m_nEnterCount,
//                nNanoSeconds,
//                0,0,0,-1);
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

//    vDiaryNote(EG_ED,ED_EXIT,
//               m_xRluCommon.m_nEnterCount,
//               nNanoSeconds,
//               0,0,0,-1);
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
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void LogicUnit::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  //m_xRluCommon.m_i_TablePtn = MY::pOp()->tPtnId().iAsI8();
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
/* diary report                                                               */
/*----------------------------------------------------------------------------*/
void LogicUnit::vDiaryReport(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
//  s256 sDiaryPath;
//
//  if (0 != m_xRluCommon.m_iSql)
//    sDiaryPath.vSprint("%s/RluDiary_%s_%lld_%lld.csv"_Stz,
//                       m_pRluDiary->szDiaryDir(),
//                       m_pRluDiary->szDiaryNm(),
//                       m_xRluCommon.m_iSql,
//                       m_xRluCommon.m_i_TablePtn
//                       );
//  else
//    sDiaryPath.vSprint("%s/RluDiary_%s.csv"_Stz,
//                       m_pRluDiary->szDiaryDir(),
//                       m_pRluDiary->szDiaryNm()
//                       );
//
//  m_pRluDiary->vReportDiary(sDiaryPath);
  
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
