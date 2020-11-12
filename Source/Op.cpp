/*============================================================================*/
/* Op.cpp -- Operator of Strand of Stage of Pipeline                          */
/*============================================================================*/

#include "Pipelines.h"

Atomic gixOp;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Pipelines,Op,CN_OP)
__aNewFunction(Op,CN_OP)
__vDeleteFunction(Op,CN_OP)
__aFooterFunction(Op);

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Op::Op(OpSpec* pOpSpec, Strand* pStrand)
: Focal(CN_OP,AM::iNextOpId()
, AM::iJobUniqueId(pStrand->pPipeline()->pPipelineJob()))
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  RASSERT(pOpSpec);
  
  m_sPipelineNm = pStrand->pPipeline()->szPipelineNm();
  m_sStageNm    = pStrand->pStage()->szStageNm();
  m_sStrandNm   = pStrand->szStrandNm();
  m_sOpNm       = pOpSpec->szOpNm();
  m_i_Op        = pOpSpec->i_OpSpec();
  
  m_pOpSpec   = pOpSpec;
  m_pStrand   = pStrand;
  m_pStage    = pStrand->pStage();
  m_pPipeline = pStrand->pPipeline();

  if (m_pStage->pStageSpec()->bNodeTopo()) {
    m_tPtnNo = C_UNKNOWN;
    RASSERT(pStrand->tPtnNo() == m_tPtnNo);
    m_i_Node = pStrand->i_Node();
  }
  else {
    m_tPtnNo = pStrand->tPtnNo();
    m_i_Node = AM::pPtnMap()->i_WhichNode(m_tPtnNo);
    RASSERT(pStrand->i_Node() == m_i_Node);
  }
  
  bool m_bDataBuffer  = false;
  bool m_bDataPulse   = false;
  bool m_bEosMarker   = false;
  bool m_bStartMarker = false;
  bool m_bStreamDef   = false;
  
  // -- -- -- -- -- --

  m_pPipelineJob  = m_pPipeline->pPipelineJob();
  m_pOpJob        = nullptr;
  
  m_oOpTagSet.vCopy(pOpSpec->pOpSpecTagSet());
  m_oTopologyTagSet.vCopy(pOpSpec->pTopologyTagSet());
  
  m_bPulsedLimit  = false;
  m_iWorkQuanta   = 1;
  m_iWorkDone     = 0;

  m_bAppendRight  = pOpSpec->bAppendRight();
  m_pQueueLeft    = new OperandQueue;
  m_pQueueRight   = new OperandQueue;

  m_eCrossTopo = PX_NONE;
  m_iNextStage = C_UNKNOWN;
  m_iNextOp    = C_UNKNOWN;
  m_iXmitNode  = C_UNKNOWN;
  m_iRobin     = 0;
  m_iLocalRobin = 0;
  
  m_pDownstreamOp = nullptr;
  
  vInitOperandHandlers();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Task* pTask = AM::pThisTLSTask();
  AM::vTaskBeginOp(pTask);
  
  ++ gixOp;
  AM::vLogOpr(SL_INF,this,OA_OP_CONSTRUCT,
              "Op construct : %p (%lld ops exist)"_Stz, //!format-LogOpr
              this,* gixOp);
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Op::~Op(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  delete m_pQueueLeft;
  delete m_pQueueRight;
  
  -- gixOp;
  AM::vLogOpr(SL_INF,this,OA_OP_DESTRUCT,
              "Op destruct : %p (%lld ops exist)"_Stz, //!format-LogOpr
              this,* gixOp);
}

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump contents                                                              */
/*----------------------------------------------------------------------------*/
void Op::vDump(i8 i_Op)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  s256 sArgs;
  
  TagSet* pTopologyTagSet = this->pTopologyTagSet();
  //pTopologyTagSet->vDumpSet();
  pTopologyTagSet->vSprint(sArgs);
  
  // get the next stage
  i8 iNextStage = pTopologyTagSet->iValue(C_TAG_NEXTSTAGE);
  Stage* pNextStage = m_pPipeline->pGet(iNextStage);
  
  // get the next op
  i8 iNextOp = pTopologyTagSet->iValue(C_TAG_NEXTOP);
  Strand* pNextStrand = pNextStage->pGet(0);
  Op* pNextOp = pNextStrand->pGet(iNextOp);

  // get the cross topology
  ePX eTopology = (ePX)(pTopologyTagSet->iValue(C_TAG_TOPOLOGY));
  
  // get the left EOS markers for consume op
  //! ALKA: right EOS markers ???
  TagSet* pOpTags = this->pOpTagSet();
  i8 iXmits =  pOpTags->iValue(C_TAG_XMITS);
  i8 iLeftMarkers = iXmits ? iXmits : 1;
  
  vTrace("    [%03lld] (N%lld,P%02lld) Op %12s%-20s { Markers(L%lld R0) :%-10s %s %s }\n"_Stz,
         i_Op,
         this->iNode(),
         this->tPtnNo(),
         this->szOpNm(),
         sArgs.szStz(),
         iLeftMarkers,
         pNextOp->szOpNm(),
         this->bAppendRight() ? "appendRight"_Stz : "appendLeft "_Stz,
         aPX[eTopology]
        );
}

/*----------------------------------------------------------------------------*/
/* set ack job on end marker                                                  */
/*----------------------------------------------------------------------------*/
void Op::vAckJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Task* pTask = AM::pExecutingTask(m_pPipeline->pPipelineJob());
  
  AM::vLogOpr(SL_INF,this,OA_OP_ACK,
              "Acking job task"_Stz); //!format-LogOpr
  
  if (AM::bVerbose())
    vTrace("Op %s (%s (N%lld) (P%02lld)): acking pipeline job\n"_Stz,
           AM::szJobName(m_pOpJob),
           m_pStage->szStageNm(),
           m_i_Node,
           m_tPtnNo);
  
  AM::vAckWaitingTask(pTask,AM::pExecutingTask(m_pOpJob));
}

/*----------------------------------------------------------------------------*/
/* get ack job on end marker                                                  */
/*----------------------------------------------------------------------------*/
void Op::vAddWorkDone(i8 iWorkDone)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_iWorkDone += iWorkDone;
}

/*----------------------------------------------------------------------------*/
/* append operand to this op's operand queue                                  */
/*----------------------------------------------------------------------------*/
void Op::vAppendLeft(Operand* pOperand, Task* pTriggeringTask, bool bSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
//  vTrace("%s::vAppendLeft : %p %s\n"_Stz,
//         m_sOpNm.szStz(),
//         pOperand,
//         aCN[pOperand->eClass()]);
  
  vGrabTaskLock();
  if (bSet) {
    while (pOperand) {
      pQueueLeft()->vAppendOperand(pOperand);
      DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
      pOperand = pDataBuffer->pNextBuffer();
    }
  }
  else
    pQueueLeft()->vAppendOperand(pOperand);

  AM::vOperandsInc();

  Task* pBidingTask = AM::pExecutingTask(m_pOpJob);
  
  if (TS_BIDING != (eTS)AM::iTaskState(pBidingTask)) {
    vDropTaskLock();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // trigger this biding op
  AM::vTriggerBidingTask(pBidingTask,pTriggeringTask);
}

/*----------------------------------------------------------------------------*/
/* append operand to this op's operand queue                                  */
/*----------------------------------------------------------------------------*/
void Op::vAppendRight(Operand* pOperand, Task* pTriggeringTask, bool bSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
    
  //  vTrace("%s::vAppendOperand : %p %s\n"_Stz,
  //         m_sOpNm(),
  //         pOperand,
  //         aCN[pOperand->eClass()]);
  
  vGrabTaskLock();
  
  if (bSet) {
    while (pOperand) {
      pQueueRight()->vAppendOperand(pOperand);
      DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
      pOperand = pDataBuffer->pNextBuffer();
    }
  }
  pQueueRight()->vAppendOperand(pOperand);
  
  AM::vOperandsInc();

  Task* pBidingTask = AM::pExecutingTask(m_pOpJob);
  
  if (TS_BIDING != (eTS)AM::iTaskState(pBidingTask)) {
    vDropTaskLock();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // trigger this biding op
  AM::vTriggerBidingTask(pBidingTask,pTriggeringTask);
}

/*----------------------------------------------------------------------------*/
/* append right ?                                                             */
/*----------------------------------------------------------------------------*/
bool Op::bAppendRight(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_bAppendRight;
}

/*----------------------------------------------------------------------------*/
/* get cross topology                                                         */
/*----------------------------------------------------------------------------*/
ePX Op::eCrossTopo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_eCrossTopo;
}

/*----------------------------------------------------------------------------*/
/* set cross topology                                                         */
/*----------------------------------------------------------------------------*/
void Op::vCrossTopo(ePX eCrossTopo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_eCrossTopo = eCrossTopo;
}

/*----------------------------------------------------------------------------*/
/* set downstream op                                                          */
/*----------------------------------------------------------------------------*/
Op* Op::pDownstreamOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pDownstreamOp;
}

/*----------------------------------------------------------------------------*/
/* set downstream op                                                          */
/*----------------------------------------------------------------------------*/
void Op::vDownstreamOp(Op* pDownstreamOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_pDownstreamOp = pDownstreamOp;
}

/*----------------------------------------------------------------------------*/
/* drop lock                                                                  */
/*----------------------------------------------------------------------------*/
void Op::vDropTaskLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  AM::vDropTaskLock(AM::pExecutingTask(m_pOpJob));
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Op::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  vTrace("Op\n"_Stz);
  vTrace("--\n"_Stz);
  vTrace("%p:\n"_Stz,this);
  vTrace("m_sOpNm=%s\n"_Stz,m_sOpNm.azChar());
  vTrace("m_i_Node=%lld\n"_Stz,m_i_Node);
  vTrace("m_tPtnNo=%lld\n"_Stz,m_tPtnNo);
}

/*----------------------------------------------------------------------------*/
/* focal outline                                                              */
/*----------------------------------------------------------------------------*/
void Op::vFocalOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  sOutline.vSprint("<%s focal>"_Stz,aCN[eClass()]);
}

/*----------------------------------------------------------------------------*/
/* grab lock                                                                  */
/*----------------------------------------------------------------------------*/
void Op::vGrabTaskLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  AM::vGrabTaskLock(AM::pExecutingTask(m_pOpJob));
}

/*----------------------------------------------------------------------------*/
/* inc work done                                                              */
/*----------------------------------------------------------------------------*/
void Op::vIncWorkDone(void)
{
 _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_iWorkDone++;
}

/*----------------------------------------------------------------------------*/
/* init operand handlers                                                      */
/*----------------------------------------------------------------------------*/
void Op::vInitOperandHandlers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  Dic* pRootDic = AM::pRootDic();
  
  s32 sHandlerNm;
  
  for (i8 i_Kind = 1; i_Kind < ZK_ENUMS; i_Kind++) {
    
    sHandlerNm = aZK[i_Kind];
    sHandlerNm.vConcat("_handler"_Stz);
    
    //vTrace("Handler: %s\n"_Stz,sHandlerNm.szStz());
    
    Word* pHandler = pRootDic->pFindWord(sHandlerNm);
    if (! pHandler)
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "Unable to find operand handler for: %s"_Stz,
             sHandlerNm.szStz());
    
    m_iOperandCount[i_Kind] = 0;
    m_paOperandHandler[i_Kind] = pHandler;
  }
  
  Correctness::vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* is locked?                                                                 */
/*----------------------------------------------------------------------------*/
bool Op::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  bool bLocked = AM::bTaskIsLocked(AM::pExecutingTask(m_pOpJob));

  return bLocked;
}

/*----------------------------------------------------------------------------*/
/* is operand received valid?                                                 */
/*----------------------------------------------------------------------------*/
bool Op::bIsOperandValid(i8 i_OperandKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  switch(i_OperandKind) {

    case ZK_CANCELMARKER:
      // should not have received a ZK_CANCELMARKER already
      if (m_iOperandCount[i_OperandKind]) {
        vTrace("multiple ZK_CANCELMARKER arrived\n"_Stz);
        return false;
      }
      
      // should not have received a ZK_CANCELLEDMARKER
      if ( m_iOperandCount[ZK_CANCELLEDMARKER]) {
        vTrace("ZK_CANCELMARKER arrived after ZK_CANCELLEDMARKER\n"_Stz);
        return false;
      }
      break;
      
    case ZK_CANCELLEDMARKER:
      // should not have received a ZK_CANCELLEDMARKER already
      if (m_iOperandCount[i_OperandKind]) {
        vTrace("multiple ZK_CANCELLEDMARKER arrived\n"_Stz);
        return false;
      }
      
      // should have received a ZK_CANCELMARKER
      if ( ! m_iOperandCount[ZK_CANCELMARKER]) {
        vTrace("ZK_CANCELLEDMARKER arrived without a prior ZK_CANCELMARKER\n"_Stz);
        return false;
      }
      break;

    case   ZK_CSVBUFFER:
      // should have received a ZK_STARTMARKER
      if ( ! m_iOperandCount[ZK_STARTMARKER]) {
        vTrace("%s arrived without a prior ZK_STARTMARKER\n"_Stz, aZK[i_OperandKind]);
        return false;
      }
      break;
      
    case   ZK_CRWBUFFER:
    case   ZK_ENTBUFFER:
    case   ZK_RSBBUFFER:
    case   ZK_SRTBUFFER:
      // should have received a ZK_STARTMARKER
      if ( ! m_iOperandCount[ZK_STARTMARKER]) {
        vTrace("%s arrived without a prior ZK_STARTMARKER\n"_Stz, aZK[i_OperandKind]);
        return false;
      }
      
      // should have received a ZK_STREAMDEF
      if ( ! m_iOperandCount[ZK_STREAMDEF]) {
        vTrace("%s: %s arrived without a prior ZK_STREAMDEF\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      
      // should have received a ZK_DATAPULSE
      if ( ! m_iOperandCount[ZK_DATAPULSE]) {
        vTrace("%s:%s arrived without a prior ZK_DATAPULSE\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      break;

    case   ZK_DATAPULSE:
      // should have received a ZK_STARTMARKER
      if ( ! m_iOperandCount[ZK_STARTMARKER]) {
        vTrace("%s arrived without a prior ZK_STARTMARKER\n"_Stz, aZK[i_OperandKind]);
        return false;
      }
     break;
      
      /*
    case   ZK_EOSMARKER:
      // should not have received a ZK_EOSMARKER
      if ( m_iOperandCount[ZK_EOSMARKER]) {
        vTrace("multiple ZK_EOSMARKER arrived\n"_Stz);
        return false;
      }
      
      // should have received a ZK_STARTMARKER
      if ( ! m_iOperandCount[ZK_STARTMARKER]) {
        vTrace("%s arrived without a prior ZK_STARTMARKER\n"_Stz, aZK[i_OperandKind]);
        return false;
      }
 
      // should have received a ZK_STARTEDMARKER
      if ( ! m_iOperandCount[ZK_STARTEDMARKER]) {
        vTrace("%s arrived without a prior ZK_STARTEDMARKER\n"_Stz, aZK[i_OperandKind]);
        return false;
      }
 
      // should have received a ZK_STREAMDEF
      if ( ! m_iOperandCount[ZK_STREAMDEF]) {
        vTrace("%s arrived without a prior ZK_STREAMDEF\n"_Stz, aZK[i_OperandKind]);
        return false;
      }
      
      // should have received a ZK_DATAPULSE
      if ( ! m_iOperandCount[ZK_DATAPULSE]) {
        vTrace("%s arrived without a prior ZK_DATAPULSE\n"_Stz, aZK[i_OperandKind]);
        return false;
      }
      break;
  
    case   ZK_ERRORMARKER:
      // should not have received a ZK_ERRORMARKER already
      if (m_iOperandCount[i_OperandKind]) {
        vTrace("multiple ZK_ERROREDMARKER arrived\n"_Stz);
        return false;
      }
      
      // should not have received a ZK_ERRORREDMARKER
      if ( m_iOperandCount[ZK_ERROREDMARKER]) {
        vTrace("ZK_ERRORMARKER arrived after ZK_ERROREDMARKER\n"_Stz);
        return false;
      }
      break;
      
    case   ZK_ERROREDMARKER:
      // should have received a ZK_ERRORMARKER
      if (m_iOperandCount[ZK_ERRORMARKER]) {
        vTrace("ZK_ERROREDMARKER arrived without a prior ZK_ERRORMARKER\n"_Stz);
        vTrace("%s arrived without a prior ZK_ERRORMARKER\n"_Stz, aZK[i_OperandKind]);
        return false;
      }
      
      // should ≤not have received an ZK_ERROREDMARKER already
      if (m_iOperandCount[i_OperandKind]) {
        vTrace("multiple ZK_ERROREDMARKER arrived\n"_Stz);
        return false;
      }
      break;
  */
    case   ZK_EXITMARKER:
      // should not have received a ZK_EXITMARKER already
      if (m_iOperandCount[i_OperandKind]) {
        vTrace("multiple ZK_EXITMARKER arrived\n"_Stz);
        return false;
      }
      
      // should not have received a ZK_EXITEDMARKER
      if ( m_iOperandCount[ZK_EXITEDMARKER]) {
        vTrace("ZK_EXITMARKER arrived after ZK_EXITEDMARKER\n"_Stz);
        return false;
      }
      break;
      
    case   ZK_EXITEDMARKER:
      // should not have received a ZK_EXITEDMARKER already
      if (m_iOperandCount[i_OperandKind]) {
        vTrace("multiple ZK_EXITEDMARKER arrived\n"_Stz);
        return false;
      }
      break;
 
    case ZK_PULSEDMARKER:
      // should have received a ZK_STARTMARKER
      if ( ! m_iOperandCount[ZK_STARTMARKER]) {
        vTrace("%s arrived without a prior ZK_STARTMARKER\n"_Stz, aZK[i_OperandKind]);
        return false;
      }
      break;
  
    case   ZK_STARTMARKER:
      // should not have received a ZK_STARTMARKER already
      if (m_iOperandCount[i_OperandKind]) {
        vTrace("multiple ZK_STARTMARKER's arrived\n"_Stz);
        return false;
      }
      
      // No other markers should have arrived before STARTMARKER
      if ( m_iOperandCount[ZK_CANCELMARKER] ||
           m_iOperandCount[ZK_CANCELLEDMARKER] ||
           m_iOperandCount[ZK_DATAPULSE] ||
           m_iOperandCount[ZK_EOSMARKER] ||
           m_iOperandCount[ZK_ERRORMARKER] ||
           m_iOperandCount[ZK_ERROREDMARKER] ||
           m_iOperandCount[ZK_EXITMARKER] ||
           m_iOperandCount[ZK_EXITEDMARKER] ||
           m_iOperandCount[ZK_PULSEDMARKER] ||
           m_iOperandCount[ZK_STARTEDMARKER] ||
           m_iOperandCount[ZK_STREAMDEF]) {
        vTrace("%s arrived before ZK_STARTMARKER\n"_Stz, aZK[i_OperandKind]);
        return false;
      }
      
      break;
      
    case   ZK_STARTEDMARKER:
      // should not have received a ZK_STARTEDMARKER already
      if (m_iOperandCount[i_OperandKind]) {
        vTrace("multiple ZK_STARTEDMARKER's arrived\n"_Stz);
        return false;
      }
      
      // should have received a ZK_STARTMARKER
      if ( ! m_iOperandCount[ZK_STARTMARKER]) {
        vTrace("ZK_STARTEDMARKER arrived without a prior ZK_STARTMARKER\n"_Stz);
        return false;
      }
      
      // No other markers should have arrived before STARTMARKER
      if ( m_iOperandCount[ZK_CANCELMARKER] ||
          m_iOperandCount[ZK_CANCELLEDMARKER] ||
          m_iOperandCount[ZK_DATAPULSE] ||
          m_iOperandCount[ZK_EOSMARKER] ||
          m_iOperandCount[ZK_ERRORMARKER] ||
          m_iOperandCount[ZK_ERROREDMARKER] ||
          m_iOperandCount[ZK_EXITMARKER] ||
          m_iOperandCount[ZK_EXITEDMARKER] ||
          m_iOperandCount[ZK_PULSEDMARKER] ||
          m_iOperandCount[ZK_STARTEDMARKER] ||
          m_iOperandCount[ZK_STREAMDEF]) {
        vTrace("%s arrived before ZK_STARTMARKER\n"_Stz, aZK[i_OperandKind]);
        return false;
      }
    
      break;
/*
    case   ZK_STREAMDEF:
      // should not have received a ZK_STARTEDMARKER already
      if (m_iOperandCount[i_OperandKind]) {
        vTrace("multiple ZK_STARTEDMARKER's arrived\n"_Stz);
        return false;
      }
      
      // should have received a ZK_STARTMARKER
      if ( ! m_iOperandCount[ZK_STARTMARKER]) {
        vTrace("ZK_STARTEDMARKER arrived without a prior ZK_STARTMARKER\n"_Stz);
        return false;
      }
      
      // No data buffers should have arrived before StreamDef
      if ( m_iOperandCount[ZK_CRWBUFFER] ||
          m_iOperandCount[ZK_CRWBUFFER] ||
          m_iOperandCount[ZK_CSVBUFFER] ||
          m_iOperandCount[ZK_DATAPULSE] ||
          m_iOperandCount[ZK_ENTBUFFER] ||
          m_iOperandCount[ZK_RSBBUFFER] ||
          m_iOperandCount[ZK_SRTBUFFER] ||
          m_iOperandCount[ZK_STREAMDEF]) {
        vTrace("%s arrived after DATA BUFFERS\n"_Stz, aZK[i_OperandKind]);
        return false;
      }
      break;
 */
    default:
      if (i_OperandKind <= ZK_NONE || i_OperandKind >= ZK_ENUMS) {
        vTrace("Invalid Operand received: %lld\n"_Stz, i_OperandKind);
        return false;
      }
  }
  
  m_iOperandCount[i_OperandKind]++;
  return true;
}

/*----------------------------------------------------------------------------*/
/* set the job                                                                */
/*----------------------------------------------------------------------------*/
void Op::vJobPipeline(Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_pPipelineJob = pJob;
}

/*----------------------------------------------------------------------------*/
/* get next op                                                                */
/*----------------------------------------------------------------------------*/
i8 Op::iNextOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_iNextOp;
}

/*----------------------------------------------------------------------------*/
/* get next op (in stage)                                                     */
/*----------------------------------------------------------------------------*/
Op* Op::pNextOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  if (m_iNextStage != m_pStage->pStageSpec()->i_StageSpec())
    return nullptr;
  
  Stage* pStage = m_pPipeline->pGet(m_iNextStage);

  Op* pNextOp = pStage->pGet(m_iNextStage)->pGet(m_iNextOp);
  
  return pNextOp;
}

/*----------------------------------------------------------------------------*/
/* set next stage                                                             */
/*----------------------------------------------------------------------------*/
void Op::vNextOp(i8 iNextOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_iNextOp = iNextOp;
}

/*----------------------------------------------------------------------------*/
/* get next stage                                                             */
/*----------------------------------------------------------------------------*/
i8 Op::iNextStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_iNextStage;
}

/*----------------------------------------------------------------------------*/
/* set next stage                                                             */
/*----------------------------------------------------------------------------*/
void Op::vNextStage(i8 iNextStage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_iNextStage = iNextStage;
}

/*----------------------------------------------------------------------------*/
/* get node                                                                   */
/*----------------------------------------------------------------------------*/
i8 Op::iNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_i_Node;
}

/*----------------------------------------------------------------------------*/
/* set node                                                                   */
/*----------------------------------------------------------------------------*/
void Op::vNode(i8 iNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_i_Node = iNode;
}

/*----------------------------------------------------------------------------*/
/* get operand handler for given operand kind                                 */
/*----------------------------------------------------------------------------*/
Word* Op::pOperandHandler(i8 i_OperandKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Word* pHandler = m_paOperandHandler[i_OperandKind];
  
  return pHandler;
}

/*----------------------------------------------------------------------------*/
/* set operand handler for given operand kind                                 */
/*----------------------------------------------------------------------------*/
void Op::vOperandHandler(i8 i_OperandKind, Word* pHandler)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_paOperandHandler[i_OperandKind] = pHandler;
}

/*----------------------------------------------------------------------------*/
/* inc operand count for given operand kind                                   */
/*----------------------------------------------------------------------------*/
void Op::vIncOperandCount(i8 i_OperandKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_iOperandCount[i_OperandKind]++;
}

/*----------------------------------------------------------------------------*/
/* get out operator                                                           */
/*----------------------------------------------------------------------------*/
Job* Op::pOpJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pOpJob;
}

/*----------------------------------------------------------------------------*/
/* get op name                                                                */
/*----------------------------------------------------------------------------*/
Stz* Op::szOpNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_sOpNm;
}

/*----------------------------------------------------------------------------*/
/* get the TagSet                                                             */
/*----------------------------------------------------------------------------*/
TagSet* Op::pOpTagSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return &m_oOpTagSet;
}

/*----------------------------------------------------------------------------*/
/* set partition in op                                                        */
/*----------------------------------------------------------------------------*/
void Op::vPtnNo(tPN tPtnNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_tPtnNo = tPtnNo;

  // set the node tis partition belongs to 
  m_i_Node = AM::pPtnMap()->i_WhichNode(m_tPtnNo);
}

/*----------------------------------------------------------------------------*/
/* get partition from op                                                      */
/*----------------------------------------------------------------------------*/
tPN Op::tPtnNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_tPtnNo;
}

/*----------------------------------------------------------------------------*/
/* operator is to pause?                                                      */
/*----------------------------------------------------------------------------*/
bool Op::bPause(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  if (! m_bPulsedLimit)
    return false;
  
  if (0 == m_iWorkDone % m_iWorkQuanta)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* peek for operand from operand queue                                        */
/*----------------------------------------------------------------------------*/
bool Op::bPeekQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  bool bOperand;
  
  vGrabTaskLock();
  
  bOperand = pQueueLeft()->bPeekQueue();
  
  vDropTaskLock();
  
  return bOperand;
}

/*----------------------------------------------------------------------------*/
/* peek for OoO operand from operand queue                                    */
/*----------------------------------------------------------------------------*/
bool Op::bPeekQueueOoO(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  bool bOperandOoO;
  
  vGrabTaskLock();
  
  bOperandOoO = pQueueLeft()->bPeekQueueOoO();
  
  vDropTaskLock();
  
  return bOperandOoO;
}

/*----------------------------------------------------------------------------*/
/* get pipeline from op                                                       */
/*----------------------------------------------------------------------------*/
Pipeline* Op::pPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pPipeline;
}

/*----------------------------------------------------------------------------*/
/* append operand to next op's operand queue                                  */
/*----------------------------------------------------------------------------*/
void Op::vPipeOut_nextOp(Operand* pOperand, Task* pTriggeringTask, bool bSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  if (AM::bVerbose())
    vTrace("%12s (%10s) (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
           m_sOpNm.szStz(),
           m_pStage->szStageNm(),
           m_i_Node,
           m_tPtnNo,
           m_pPipelineJob,
           "vPipeOut :"_Stz,
           AM::pOperandName(pOperand),
           pOperand);
  
  if (AM::szJobName(m_pOpJob)->bSame("xmit"_Stz)) {
    vSendXmitOperand(pOperand, pTriggeringTask, bSet);
  }
  else {
    
    // get the next stage the xmit is supposed to xmit to
    Stage* pStage = m_pPipeline->pGet(m_iNextStage);
  
    // get the op to forward to depending on the cross topology
    Op* pOp = pStage->pPipeOutOp(m_eCrossTopo,
                                 m_i_Node,
                                 m_tPtnNo,
                                 m_iNextOp);
    if (m_bAppendRight)
      pOp->vAppendRight(pOperand,pTriggeringTask,bSet);
    else
      pOp->vAppendLeft(pOperand,pTriggeringTask,bSet);
   
    // Can Operand be deleted already ????
//    AM::vLogOpr(SL_INF,this,OA_OP_APPEND,
//                "PipeOut operand %s to %s queue of %s"_Stz, //!format-LogOpr
//                AM::pOperandName(pOperand),
//                m_bAppendRight ? "right"_Stz : "left"_Stz,
//                aCN[pOp->eClass()]);
  }
  
}

/*----------------------------------------------------------------------------*/
/* append operand to specific operand                                         */
/*----------------------------------------------------------------------------*/
void Op::vPipeOut_thisOp(Operand* pOperand, Task* pTriggeringTask, bool bSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  if (AM::bVerbose())
    vTrace("%12s (%10s) (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
           m_sOpNm.szStz(),
           m_pStage->szStageNm(),
           m_i_Node,
           m_tPtnNo,
           m_pPipelineJob,
           "vPipeOut :"_Stz,
           AM::pOperandName(pOperand),
           pOperand);
  
  vAppendLeft(pOperand,pTriggeringTask,bSet);
  
      // Can Operand be deleted already ????
//  AM::vLogOpr(SL_INF,this,OA_OP_APPEND,
//              "PipeOut operand %s to %s queue of %s"_Stz, //!format-LogOpr
//              pOperand->eClass() == CN_MARKER ?
//              aOM[(_pMarker(pOperand))->eOpMarker()] :
//              aCN[pOperand->eClass()],
//              m_bAppendRight ? "right"_Stz : "left"_Stz,
//              aCN[eClass()]);
}

/*----------------------------------------------------------------------------*/
/* append operand to the pipeline job's buffer queue                          */
/*----------------------------------------------------------------------------*/
void Op::vPipeToJob(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Queue* pBufferQueue = AM::pBufferQueue(m_pPipelineJob);
  
  RASSERT(pBufferQueue);
  
  if (AM::bVerbose())
    vTrace("%12s (%10s) (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
           m_sOpNm.szStz(),
           m_pStage->szStageNm(),
           m_i_Node,
           m_tPtnNo,
           m_pPipelineJob,
           "vPipeToJob :"_Stz,
           AM::pOperandName(pOperand),
           pOperand);
  
  AM::vLogOpr(SL_INF,this,OA_OP_APPEND,
              "vPipeToJob operand %s to queue of pipeline job"_Stz, //!format-LogOpr
              aCN[pOperand->eClass()]);
  
  pBufferQueue->vAppend(pOperand);
  
}

/*----------------------------------------------------------------------------*/
/* pop operand queue                                                          */
/*----------------------------------------------------------------------------*/
Operand* Op::pPopQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Operand* pOperand = nullptr;
  
  vGrabTaskLock();
  
  pOperand = pQueueLeft()->pPopQueue();
  
      // Can Operand be deleted already ????
//  if (pOperand)
//    AM::vLogOpr(SL_INF,this,OA_OP_POP,
//                "PopQueue operand %s from left queue"_Stz, //!format-LogOpr
//                AM::pOperandName(pOperand));
//  else
//    AM::vLogOpr(SL_INF,this,OA_OP_POP,
//                "PopQueue no operand on left queue"_Stz); //!format-LogOpr

  if (! pOperand)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDropTaskLock();
    
  AM::vOperandsDec();
  
  if (AM::bVerbose()) {
    vTrace("%12s (%10s) (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
           m_sOpNm.szStz(),
           m_pStage->szStageNm(),
           m_i_Node,
           m_tPtnNo,
           m_pPipelineJob,
           "pPopQueue :"_Stz,
           AM::pOperandName(pOperand),
           pOperand);
  }
  
  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* pop operand OoO off the queue                                              */
/*----------------------------------------------------------------------------*/
Operand* Op::pPopQueueOoO(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Operand* pOperand = nullptr;
  
  vGrabTaskLock();
  
  pOperand = pQueueLeft()->pPopQueueOoO();
  
      // Can Operand be deleted already ????
//  if (pOperand)
//    AM::vLogOpr(SL_INF,this,OA_OP_POP,
//                "PopQueue operand %s from left queue"_Stz, //!format-LogOpr
//                AM::pOperandName(pOperand));
//  else
//    AM::vLogOpr(SL_INF,this,OA_OP_POP,
//                "PopQueue no operand on left queue"_Stz); //!format-LogOpr
  
  vDropTaskLock();
  
  if (pOperand)
    AM::vOperandsDec();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (AM::bVerbose() && pOperand) {
    vTrace("%12s (%10s) (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
           m_sOpNm.szStz(),
           m_pStage->szStageNm(),
           m_i_Node,
           m_tPtnNo,
           m_pPipelineJob,
           "pPopQueue :"_Stz,
           AM::pOperandName(pOperand),
           pOperand);
  }
  
  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* pop operand queue A                                                        */
/*----------------------------------------------------------------------------*/
Operand* Op::pPopQueueLeft(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Operand* pOperand = nullptr;
  
  vGrabTaskLock();
  
  pOperand = pQueueLeft()->pPopQueue();
  
  if (pOperand) {
    vDropTaskLock();
    
    AM::vOperandsDec();
  }
  
      // Can Operand be deleted already ????
//  if (pOperand) {
//    if (AM::bVerbose())
//      AM::vLogOpr(SL_INF,this,OA_OP_POP,
//                  "PopQueueLeft operand %s from left queue"_Stz, //!format-LogOpr
//                  AM::pOperandName(pOperand));
//  }
//  else {
//    if (AM::bVerbose())
//      AM::vLogOpr(SL_INF,this,OA_OP_POP,
//                  "PopQueueLeft no operand on left queue"_Stz); //!format-LogOpr
//  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pOperand) {
    if (AM::bVerbose())
      vTrace("%s::pPopQueueLeft : %s %p\n"_Stz,
             m_sOpNm.szStz(),
             AM::pOperandName(pOperand),
             pOperand);
  }
  
  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* pop operand queue B                                                        */
/*----------------------------------------------------------------------------*/
Operand* Op::pPopQueueRight(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Operand* pOperand = nullptr;
  
  vGrabTaskLock();
  
  pOperand = pQueueRight()->pPopQueue();
  
  if (pOperand) {
    vDropTaskLock();
    
    AM::vOperandsDec();
  }
      // Can Operand be deleted already ????
//  if (pOperand)
//    AM::vLogOpr(SL_INF,this,OA_OP_POP,
//                "PopQueueRight operand %s from right queue"_Stz, //!format-LogOpr
//                AM::pOperandName(pOperand));
//  else
//    AM::vLogOpr(SL_INF,this,OA_OP_POP,
//                "PopQueueRight no operand on right queue"_Stz); //!format-LogOpr
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pOperand) {
    if (AM::bVerbose())
      vTrace("%s::pPopQueueRight : %s %p\n"_Stz,
             m_sOpNm.szStz(),
             AM::pOperandName(pOperand),
             pOperand);
  }
  
  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* is pulsed operator?                                                        */
/*----------------------------------------------------------------------------*/
bool Op::bPulsedLimit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_bPulsedLimit;
}

/*----------------------------------------------------------------------------*/
/* set pulsed operator                                                        */
/*----------------------------------------------------------------------------*/
void Op::vPulsedLimit(bool bPulsedLimit)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_bPulsedLimit = bPulsedLimit;
}

/*----------------------------------------------------------------------------*/
/* get left operand queue                                                     */
/*----------------------------------------------------------------------------*/
OperandQueue* Op::pQueueLeft(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pQueueLeft;
}

/*----------------------------------------------------------------------------*/
/* get operand queue length                                                   */
/*----------------------------------------------------------------------------*/
i8 Op::nQueueLength(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  i8 nOperands = m_pQueueLeft->nQueuedOperands();
  return nOperands;
}

/*----------------------------------------------------------------------------*/
/* get right operand queue                                                    */
/*----------------------------------------------------------------------------*/
OperandQueue* Op::pQueueRight(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pQueueRight;
}

/*----------------------------------------------------------------------------*/
/* set xmit operand                                                           */
/*----------------------------------------------------------------------------*/
void Op::vSendXmitOperand(Operand* pOperand, Task* pTriggeringTask, bool bSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  // there shold be a next Stage
  ASSERT (m_iNextStage != C_UNKNOWN);
  
  // there should be a next Op
  ASSERT(m_iNextOp != C_UNKNOWN);
  
  // get the next stage
  Stage* pNextStage = m_pPipeline->pGet(m_iNextStage);
  RASSERT(pNextStage);
  
  // figure out the xmit kind
  eXK eXmitKind = m_eXmitKind;
  
  // is the operand is a marker or a streamDef ?
  if (pOperand->bIsMarker() || pOperand->bIsStreamDef())
    eXmitKind = XK_NORMAL;
  
  // is the operand an InfoSet ?
  if (pOperand->bIsInfoSet() || pOperand->bIsProfile())
    eXmitKind = XK_ROBIN;
  
  switch(m_eCrossTopo) {
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ALL_NODES_TO_ORIGIN:
    case PX_ALL_PARTITIONS_TO_ORIGIN:
    case PX_ORIGIN_TO_ORIGIN:
    case PX_SPECIFIC_PARTITION_TO_ORIGIN: {
      vSendToOrigin(pOperand, pTriggeringTask, bSet, eXmitKind);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ORIGIN_TO_ALL_PARTITIONS:
    case PX_ALL_NODES_TO_ALL_PARTITIONS:
    case PX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS: {
      vSendToAllPartitions(pOperand, pTriggeringTask, bSet, eXmitKind);
      break;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN: {
      vSendToAllOriginPartitions(pOperand, pTriggeringTask, bSet, eXmitKind);
      break;
    }
     
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ORIGIN_TO_SPECIFIC_PARTITION: {
      Strand* pStrand = pNextStage->pGetPartitionStrand(m_tPtnNo);
      tPN tPtnNo = pStrand->tPtnNo();
      vSendToPartition(pOperand, pTriggeringTask,bSet, eXmitKind, tPtnNo);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ALL_PARTITIONS_TO_ALL_NODES: {
      i8 i_Node = AM::pPtnMap()->i_WhichNode(m_tPtnNo);
      vSendToNode(pOperand, pTriggeringTask, bSet, eXmitKind,i_Node);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS: {
      if (pOperand->bIsBuffer()) {
        vSendToPartition(pOperand, pTriggeringTask, bSet, eXmitKind, m_tPtnNo);
      }
      else {
        vSendToAllPartitions(pOperand, pTriggeringTask, bSet, eXmitKind);
      }
      break;
    }
     
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ORIGIN_TO_ALL_NODES: {
      vSendToAllNodes(pOperand, pTriggeringTask, bSet, eXmitKind);
      break;
    }
      
    default:
      RASSERT(0);
  }
    
  // delete the operand
  //AM::vDeleteOperand(pOperand);
}

/*----------------------------------------------------------------------------*/
/* spawn a job for this operator                                              */
/*----------------------------------------------------------------------------*/
void Op::vSpawnOperatorJob(i8 iSessionId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  //vTrace("Spawning job for %s\n"_Stz, szCmdStr.szStz());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a new Job for the Op
  ReqTuple xReqTuple(m_sOpNm,iSessionId, false);
  m_pOpJob = AM::pNewJobOp(this,&xReqTuple);
  
  // get the operator job tag set
  TagSet* pOpJobTagSet = AM::pJobTags(m_pOpJob);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy the pipeline job tag set into the operator job tags
  TagSet* pPipeLineJobTags = AM::pJobTags(m_pPipelineJob);
  pOpJobTagSet->vCopy(pPipeLineJobTags);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now copy the hidden topology ename pairs to the job ename set
  pOpJobTagSet->vCopy(&m_oTopologyTagSet);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add the OpJob JobNM to be the same as the Opertor name
  pOpJobTagSet->vReplace(C_TAG_JOBNM, m_sOpNm);
  
  //pOpJobTags->vDumpSet();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the command string using the parameter ename pairs
  sSTMT szCmdStr;
  vToCmdStr(szCmdStr);

  TokenVec* pTknVec = new TokenVec;
  pTknVec->vTokenise(szCmdStr);

  bool b = AM::bParseCmd(pTknVec,pOpJobTagSet);
  RASSERT(b);
//  if (! b) {
//    //vTrace("Unable to match job syntax\n"_Stz);
//    vParticipantHasError(true);
//    return EC_INVALID_COMMAND;
//  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // populate the job
  
  eEC eError = AM::ePopulateJob(m_pOpJob, pOpJobTagSet);
    
  //RASSERT(EC_NONE != eError);
  
  // delete the tokenVec
  pTknVec->vSetAllTokens();
  delete pTknVec;
  
  if (StrLib::bSame(m_sOpNm.azChar(), "receiver"))
    AM::vSpawnJob_dedicated(m_pOpJob);
  else
    AM::vSpawnJob_op(m_pOpJob);
}

/*----------------------------------------------------------------------------*/
/* get operator stage name                                                    */
/*----------------------------------------------------------------------------*/
Stz* Op::szStageName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pStage->szStageNm();
}

/*----------------------------------------------------------------------------*/
/* get stage                                                                  */
/*----------------------------------------------------------------------------*/
Stage* Op::pStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pStage;
}

/*----------------------------------------------------------------------------*/
/* get task                                                                   */
/*----------------------------------------------------------------------------*/
Task* Op::pTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return AM::pExecutingTask(m_pOpJob);
}

/*----------------------------------------------------------------------------*/
/* set task                                                                   */
/*----------------------------------------------------------------------------*/
//void Op::vTask(Task* pTask)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
//
//  m_pTask = pTask;
//}

/*----------------------------------------------------------------------------*/
/* print stage spec                                                           */
/*----------------------------------------------------------------------------*/
void Op::vToCmdStr(Stz* szCmdStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
    
  szCmdStr->vCopy(m_sOpNm);
  m_oOpTagSet.vConcat(szCmdStr);
  //vTrace("%s\n"_Stz, szCmdStr);
}

/*----------------------------------------------------------------------------*/
/* get the topology TagSet                                                    */
/*----------------------------------------------------------------------------*/
TagSet* Op::pTopologyTagSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return &m_oTopologyTagSet;
}

/*----------------------------------------------------------------------------*/
/* set work quanta                                                            */
/*----------------------------------------------------------------------------*/
void Op::vWorkQuanta(i8 iWorkQuanta)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_iWorkQuanta = iWorkQuanta;
}

/*----------------------------------------------------------------------------*/
/* get xmit kind                                                              */
/*----------------------------------------------------------------------------*/
eXK Op::eXmitKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_eXmitKind;
}

/*----------------------------------------------------------------------------*/
/* set xmit kind                                                              */
/*----------------------------------------------------------------------------*/
void Op::vXmitKind(eXK eXmitKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_eXmitKind = eXmitKind;
}

/*----------------------------------------------------------------------------*/
/* get xmit node                                                              */
/*----------------------------------------------------------------------------*/
i8 Op::iXmitNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_iXmitNode;
}

/*----------------------------------------------------------------------------*/
/* set xmit node                                                              */
/*----------------------------------------------------------------------------*/
void Op::vXmitNode(i8 iXmitNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_iXmitNode = iXmitNode;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* send the operand                                                           */
/*----------------------------------------------------------------------------*/
void Op::vSend(Operand* pOperand, bool bSet, i8 i_Node, tPN tPtnNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  XmitOperand* pXmitOperand;
  
  // new an xmit operand
  pXmitOperand = AM::pNewXmitOperand(m_pPipelineJob, pOperand, this, i_Node, tPtnNo);
  
  // get the send node in the task
  Node* pSendNode = AM::pGrid()->pGetNode(i_Node);
  
  if (bSet) {
    
    while (pOperand) {
      
      // set the operand in the job
      AM::vSetOperand(pXmitOperand, pOperand);
      
      // send the operand job across the sendNode
      AM::vSend(AM::pExecutingTask(m_pOpJob), pSendNode, pXmitOperand);
      DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
      pOperand = pDataBuffer->pNextBuffer();
    }
  }
  else {
    // send the operand job across the sendNode
    AM::vSend(AM::pExecutingTask(m_pOpJob), pSendNode, pXmitOperand);
  }

  // delete the xmitOperand
  AM::vDeleteXmitOperand(pXmitOperand);
}

/*----------------------------------------------------------------------------*/
/* send the operand                                                           */
/*----------------------------------------------------------------------------*/
void Op::vSendToAllNodes(Operand* pOperand, Task* pTriggeringTask, bool bSet, eXK eXmitKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  ASSERT(eXmitKind == XK_NORMAL || eXmitKind == XK_ROBIN);
  
  // get the next stage
  Stage* pNextStage = m_pPipeline->pGet(m_iNextStage);
  RASSERT(pNextStage);
  
  ASSERT(pNextStage->nItems() == AM::pGrid()->nConnectedNodes());
  
  for (i8 i_Node = 0; i_Node < pNextStage->nItems(); i_Node++) {
    vSend(pOperand, bSet, i_Node, C_UNKNOWN);
  }
  
  // delete the operand
  AM::vDeleteOperand(pOperand);
}

/*----------------------------------------------------------------------------*/
/* send the operand                                                           */
/*----------------------------------------------------------------------------*/
void Op::vSendToAllOriginPartitions(Operand* pOperand, Task* pTriggeringTask, bool bSet, eXK eXmitKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  tPN tPtnNo;
  i8 i_TablePtn;
  
  // get the next stage
  Stage* pNextStage = m_pPipeline->pGet(m_iNextStage);
  RASSERT(pNextStage);
  
#ifdef WITH_NO_XMIT_OPT
  // get the current node
  i8 i_ThisNode = AM::pGrid()->iThisNode();
#endif
  
  // check the XmitKind
  switch (eXmitKind) {
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_ROBIN : {
      m_iRobin++;
      i_TablePtn = m_iRobin % AM::pGrid()->nTablePtns();
      tPtnNo = PtnMap::tStrandToLglPtnNo(i_TablePtn);
      
      // xmitNode is the partition node
      m_iXmitNode = AM::pPtnMap()->i_WhichNode(tPtnNo);
      ASSERT(m_iXmitNode == m_pPipeline->i_Origin());
   
      if ( m_iXmitNode == m_pPipeline->i_Origin()) {
        ASSERT( ! bSet );
        
#ifdef WITH_NO_XMIT_OPT
        if ( m_iXmitNode == i_ThisNode) {

          // get the next stage the xmit is supposed to xmit to
          Stage* pStage = m_pPipeline->pGet(m_iNextStage);

          // get the op to forward to depending on the cross topology
          Op* pOp = pStage->pPipeOutOp(m_eCrossTopo,
                                       m_iXmitNode,
                                       i_Ptn,
                                       m_iNextOp);
          if (m_bAppendRight)
            pOp->vAppendRight(pOperand,pTriggeringTask,bSet);
          else
            pOp->vAppendLeft(pOperand,pTriggeringTask,bSet);

              // Can Operand be deleted already ????
//          AM::vLogOpr(SL_INF,this,OA_OP_APPEND,
//                      "PipeOut operand %s to %s queue of %s"_Stz, //!format-LogOpr
//                      AM::pOperandName(pOperand),
//                      m_bAppendRight ? "right"_Stz : "left"_Stz,
//                      aCN[pOp->eClass()]);

        }
        else {
          vSend(pOperand, bSet, m_iXmitNode, i_Ptn);
          AM::vDeleteOperand(pOperand);
        }
#else
        vSend(pOperand, bSet, m_iXmitNode, tPtnNo);
        AM::vDeleteOperand(pOperand);
#endif
        
      }
      
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_NORMAL: {
      
      ASSERT(pNextStage->nItems() == AM::pGrid()->nTablePtns());
      for (i8 i_Strand = 0; i_Strand < pNextStage->nItems(); i_Strand++) {
        
        Strand* pStrand = pNextStage->pGet(i_Strand);
        tPtnNo = pStrand->tPtnNo();
        
        // xmitNode is the partition node
    
        m_iXmitNode = AM::pPtnMap()->i_WhichNode(tPtnNo);
        if ( m_iXmitNode == m_pPipeline->i_Origin()) {
 #ifdef WITH_NO_XMIT_OPT
          if ( (pOperand->bIsMarker()  || pOperand->bIsStreamDef() ) &&
                m_iXmitNode == i_ThisNode) {
            
            // get the next stage the xmit is supposed to xmit to
            Stage* pStage = m_pPipeline->pGet(m_iNextStage);
            
            // get the op to forward to depending on the cross topology
            Op* pOp = pStage->pPipeOutOp(m_eCrossTopo,
                                         m_iXmitNode,
                                         i_Ptn,
                                         m_iNextOp);
            
            // duplicate the marker / streamDef
            Operand* pNewOperand = AM::pDuplicateOperand(pOperand);
            
            if (m_bAppendRight)
              pOp->vAppendRight(pNewOperand,pTriggeringTask,bSet);
            else
              pOp->vAppendLeft(pNewOperand,pTriggeringTask,bSet);
   
                // Can Operand be deleted already ????
//            AM::vLogOpr(SL_INF,this,OA_OP_APPEND,
//                        "PipeOut operand %s to %s queue of %s"_Stz, //!format-LogOpr
//                        AM::pOperandName(pNewOperand),
//                        m_bAppendRight ? "right"_Stz : "left"_Stz,
//                        aCN[pOp->eClass()]);
            
          }
     
          else {
            vSend(pOperand, bSet, m_iXmitNode, i_Ptn);
          }
#else
          vSend(pOperand, bSet, m_iXmitNode, tPtnNo);
#endif
        }
        
      }
      AM::vDeleteOperand(pOperand);
    }
    break;
      
    default:
      ASSERT(0);
  }
  
}

/*----------------------------------------------------------------------------*/
/* send the operand                                                           */
/*----------------------------------------------------------------------------*/
void Op::vSendToAllPartitions(Operand* pOperand, Task* pTriggeringTask, bool bSet, eXK eXmitKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  tPN tPtnNo;
  i8 i_TablePtn;
  
  // get the next stage
  Stage* pNextStage = m_pPipeline->pGet(m_iNextStage);
  RASSERT(pNextStage);
  
#ifdef WITH_NO_XMIT_OPT
  // get the current node
  i8 i_ThisNode = AM::pGrid()->iThisNode();
#endif
  
  switch (eXmitKind) {
      
      //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_HASHED : {
      DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
      tPtnNo = pDataBuffer->tPtnNo();
      
      // xmitNode is the partition node
      m_iXmitNode = AM::pPtnMap()->i_WhichNode(tPtnNo);
      
#ifdef WITH_NO_XMIT_OPT
      if ( m_iXmitNode == i_ThisNode) {
        
        // get the next stage the xmit is supposed to xmit to
        Stage* pStage = m_pPipeline->pGet(m_iNextStage);
        
        // get the op to forward to depending on the cross topology
        Op* pOp = pStage->pPipeOutOp(m_eCrossTopo,
                                     m_iXmitNode,
                                     tPtnNo,
                                     m_iNextOp);
        if (m_bAppendRight)
          pOp->vAppendRight(pOperand,pTriggeringTask,bSet);
        else
          pOp->vAppendLeft(pOperand,pTriggeringTask,bSet);
        
            // Can Operand be deleted already ????
//        AM::vLogOpr(SL_INF,this,OA_OP_APPEND,
//                    "PipeOut operand %s to %s queue of %s"_Stz, //!format-LogOpr
//                    AM::pOperandName(pOperand),
//                    m_bAppendRight ? "right"_Stz : "left"_Stz,
//                    aCN[pOp->eClass()]);
        
      }
      else {
        vSend(pOperand, bSet, m_iXmitNode, tPtnNo);
        AM::vDeleteOperand(pOperand);
      }
#else
      vSend(pOperand, bSet, m_iXmitNode, tPtnNo);
      AM::vDeleteOperand(pOperand);
#endif
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_LOCALROBIN : {
      m_iLocalRobin++;
      i_TablePtn = m_iLocalRobin % AM::pGrid()->nTablePtns();
      tPtnNo = PtnMap::tStrandToLglPtnNo(i_TablePtn);
      
      // xmitNode is this node
      m_iXmitNode = AM::pPtnMap()->i_WhichNode(tPtnNo);
      RASSERT(m_iXmitNode == AM::pGrid()->iThisNode());
      
      ASSERT( ! bSet );
      vSend(pOperand, bSet, m_iXmitNode, tPtnNo);
      AM::vDeleteOperand(pOperand);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_NORMAL: {
      
      ASSERT(pNextStage->nItems() == AM::pGrid()->nPartitionsInGrid());
      for (tPtnNo = 0; tPtnNo < pNextStage->nItems(); tPtnNo++) {
        
        // xmitNode is the partition node
        m_iXmitNode = AM::pPtnMap()->i_WhichNode(tPtnNo);
        ASSERT( ! bSet );
#ifdef WITH_NO_XMIT_OPT
        if ( (pOperand->bIsMarker()  || pOperand->bIsStreamDef() ) &&
            m_iXmitNode == i_ThisNode) {
          
          // get the next stage the xmit is supposed to xmit to
          Stage* pStage = m_pPipeline->pGet(m_iNextStage);
          
          // get the op to forward to depending on the cross topology
          Op* pOp = pStage->pPipeOutOp(m_eCrossTopo,
                                       m_iXmitNode,
                                       tPtnNo,
                                       m_iNextOp);
          
          // duplicate the marker / streamDef
          Operand* pNewOperand = AM::pDuplicateOperand(pOperand);
          
          if (m_bAppendRight)
            pOp->vAppendRight(pNewOperand,pTriggeringTask,bSet);
          else
            pOp->vAppendLeft(pNewOperand,pTriggeringTask,bSet);
         
              // Can Operand be deleted already ????
//          AM::vLogOpr(SL_INF,this,OA_OP_APPEND,
//                      "PipeOut operand %s to %s queue of %s"_Stz, //!format-LogOpr
//                      AM::pOperandName(pNewOperand),
//                      m_bAppendRight ? "right"_Stz : "left"_Stz,
//                      aCN[pOp->eClass()]);
          
        }
        else {
          vSend(pOperand, bSet, m_iXmitNode, tPtnNo);
        }
#else
        vSend(pOperand, bSet, m_iXmitNode, tPtnNo);
#endif
      }
      AM::vDeleteOperand(pOperand);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_ROBIN : {
      m_iRobin++;
      tPtnNo = m_iRobin % AM::pGrid()->nPartitionsInGrid();
      
      // xmitNode is the partition node
      m_iXmitNode = AM::pPtnMap()->i_WhichNode(tPtnNo);
      
      ASSERT( ! bSet );
      vSend(pOperand, bSet, m_iXmitNode, tPtnNo);
      AM::vDeleteOperand(pOperand);
      break;
    }
      
    default:
      ASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* send the operand to a specific node                                        */
/*----------------------------------------------------------------------------*/
void Op::vSendToNode(Operand* pOperand, Task* pTriggeringTask, bool bSet, eXK eXmitKind,i8 i_Node)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  ASSERT(eXmitKind == XK_NORMAL || eXmitKind == XK_ROBIN);
  
  // get the next stage
  Stage* pNextStage = m_pPipeline->pGet(m_iNextStage);
  RASSERT(pNextStage);
  
  ASSERT(pNextStage->nItems() == AM::pGrid()->nConnectedNodes());
  
  vSend(pOperand, bSet, i_Node, C_UNKNOWN);
  
  // delete the operand
  AM::vDeleteOperand(pOperand);
}

/*----------------------------------------------------------------------------*/
/* send the operand to the origin                                             */
/*----------------------------------------------------------------------------*/
void Op::vSendToOrigin(Operand* pOperand, Task* pTriggeringTask, bool bSet, eXK eXmitKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  ASSERT(eXmitKind == XK_NORMAL || eXmitKind == XK_ROBIN);
  m_iXmitNode = m_pPipeline->i_Origin();
  vSend(pOperand, bSet, m_iXmitNode, C_UNKNOWN);
  
  // delete the operand
  AM::vDeleteOperand(pOperand);
}

/*----------------------------------------------------------------------------*/
/* send the operand to a partition                                            */
/*----------------------------------------------------------------------------*/
void Op::vSendToPartition(Operand* pOperand, Task* pTriggeringTask, bool bSet,
                          eXK eXmitKind, tPN tPtnNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  tPN tSendPtn = C_UNKNOWN;
  
  ASSERT(eXmitKind == XK_NORMAL ||
         eXmitKind == XK_ROBIN  ||
         eXmitKind == XK_HASHED);   // ray added XK_HASHED 26 jul 2020 re q301
  
  switch (eXmitKind) {
      
    case XK_HASHED : {
      DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
      tSendPtn = pDataBuffer->tPtnNo();
      break;
    }
      
    case XK_NORMAL:
      tSendPtn = tPtnNo;
      break;
      
    case XK_ROBIN:
      m_iRobin++;
      tSendPtn = m_iRobin % AM::pGrid()->nPartitionsInGrid();
      break;
      
    default:
      ASSERT(0);
  }
  
  m_iXmitNode = AM::pPtnMap()->i_WhichNode(tSendPtn);
  vSend(pOperand, bSet, m_iXmitNode, tSendPtn);
  
  // delete the operand
  AM::vDeleteOperand(pOperand);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
