/*============================================================================*/
/* Op.cpp -- Operator of Stream of Stage of Pipeline                          */
/*============================================================================*/

#include "BASE.h"

#include "ePX.h"

#include "PtnMap.hpp"
#include "Node.hpp"
#include "Node_I.hpp"
#include "Grid.hpp"

#include "Op.hpp"
#include "OpSpec.hpp"
#include "Stream.hpp"
#include "Stage.hpp"
#include "Pipeline.hpp"
#include "StageSpec.hpp"
#include "MY.hpp"
#include "Parcel.hpp"

#include "eTS.h"
#include "TagConstants.h"

Atomic gixOp;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Op,CN_OP)
__aNewFunction(Op,CN_OP)
__vDeleteFunction(Op,CN_OP)
__aFooterFunction(Op);

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Op::Op(OpSpec* pOpSpec, Stream* pStream)
: Object(CN_OP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  RASSERT(pOpSpec);
  
  m_sPipelineNm = pStream->pPipeline()->szPipelineNm();
  m_sStageNm    = pStream->pStage()->szStageNm();
  m_sStreamNm   = pStream->szStreamNm();
  m_sOpNm       = pOpSpec->szOpNm();
  m_i_Op        = pOpSpec->i_OpSpec();
  
  m_bTraceOp    = false;
  
  m_pOpSpec   = pOpSpec;
  m_pStream   = pStream;
  m_pStage    = pStream->pStage();
  m_pPipeline = pStream->pPipeline();

  if (m_pStage->pStageSpec()->bNodeTopo()) {
    m_tPtnNo = C_UNKNOWN;
    RASSERT(pStream->tPtnNo() == m_tPtnNo);
    m_i_Node = pStream->i_Node();
  }
  else {
    m_tPtnNo = pStream->tPtnNo();
    m_i_Node = AM::pPtnMap()->i_WhichNode(m_tPtnNo);
    RASSERT(pStream->i_Node() == m_i_Node);
  }
    
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

  m_eCrossTopo  = PX_NONE;
  m_iNextStage  = C_UNKNOWN;
  m_iNextOp     = C_UNKNOWN;
  m_iOtherStage = C_UNKNOWN;
  m_iOtherOp    = C_UNKNOWN;
  m_iSendNode   = C_UNKNOWN;
  m_iRobin      = 0;
  m_iLocalRobin = 0;
  
  m_pDownstreamOp = nullptr;
  m_pOutboundOperand = nullptr;

  vInitOperandHandlers();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Job* pPipelineJob = pStream->pPipeline()->pPipelineJob();
  TagSet* pJobTags = AM::pJobTags(pPipelineJob);
  
  if (AM::bVerbose() || pJobTags->bHasName(C_TAG_TRACEOP)) {
    m_bTraceOp = true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ++ gixOp;
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  AM::LOG::vEvent(EV_CONSTRUCT,"Op constructor"_Stz);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Op::~Op(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);


  if (m_pQueueLeft->nItems() != 0) {
    vTrace("Operand Queue not empty\n"_Stz);
    Operand *pOperand = m_pQueueLeft->pPop();
    while (pOperand) {
      vTrace("Operand Queue for OP %s has %s\n"_Stz, m_sOpNm.azChar(), AM::pOperandName(pOperand));
      pOperand = m_pQueueLeft->pPop();
    }
    RASSERT(0);
  }
  if (m_pQueueRight->nItems() != 0) {
    vTrace("Operand Queue not empty\n"_Stz);
    Operand *pOperand = m_pQueueRight->pPop();
    while (pOperand) {
      vTrace("Operand Queue for OP %s has %s\n"_Stz, m_sOpNm.azChar(), AM::pOperandName(pOperand));
      pOperand = m_pQueueRight->pPop();
    }
    RASSERT(0);
  }

  delete m_pQueueLeft;
  delete m_pQueueRight;
  
  -- gixOp;
  
  AM::LOG::vEvent(EV_DESTRUCT,"Op destructor"_Stz);
}

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
  Stream* pNextStream = pNextStage->pGet(0);
  Op* pNextOp = pNextStream->pGet(iNextOp);

  // get the cross topology
  ePX eTopology = (ePX)(pTopologyTagSet->iValue(C_TAG_TOPOLOGY));
  
  // get the left EOS markers for consume op
  //! ALKA: right EOS markers ???
  TagSet* pOpTags = this->pOpTagSet();
  i8 iSends =  pOpTags->iValue(C_TAG_Sends);
  i8 iLeftMarkers = iSends ? iSends : 1;
  
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
    
  if (m_bTraceOp)
    vTrace("Op %s (%s (N%lld) (P%02lld)): acking pipeline job\n"_Stz,
           AM::JOB::szJobNm(m_pOpJob),
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
/* append operand to specific operand                                         */
/*----------------------------------------------------------------------------*/
void Op::vAppend(Operand* pOperand, Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  if (m_bTraceOp)
    vTrace("%12s (%10s) (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
           m_sOpNm.szStz(),
           m_pStage->szStageNm(),
           m_i_Node,
           m_tPtnNo,
           m_pPipelineJob,
           "vPipeOut :"_Stz,
           AM::pOperandName(pOperand),
           pOperand);
  
  vAppendLeft(pOperand,pTriggeringTask);
}

/*----------------------------------------------------------------------------*/
/* append operand to this op's operand queue                                  */
/*----------------------------------------------------------------------------*/
void Op::vAppendLeft(Operand* pOperand, Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
//  vTrace("%s::vAppendLeft : %p %s\n"_Stz,
//         m_sOpNm.szStz(),
//         pOperand,
//         aCN[pOperand->eGetClassNo()]);
    
  vGrabTaskLock();
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
void Op::vAppendRight(Operand* pOperand, Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
    
  //  vTrace("%s::vAppendOperand : %p %s\n"_Stz,
  //         m_sOpNm(),
  //         pOperand,
  //         aCN[pOperand->eGetClassNo()]);
    
  vGrabTaskLock();
  
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
/* outline                                                                    */
/*----------------------------------------------------------------------------*/
void Op::vOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  sOutline.vSprint("<%s focal>"_Stz,aCN[eGetClassNo()]);
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

  Dic* pDicRoot = AM::pDicRoot();
  
  s32 sHandlerNm;
  
  for (i8 i_Kind = 1; i_Kind < ZK_ENUMS; i_Kind++) {
    
    sHandlerNm = aZK[i_Kind];
    sHandlerNm.vConcat("_handler"_Stz);
    
    //vTrace("Handler: %s\n"_Stz,sHandlerNm.szStz());
    
    Word* pHandler = pDicRoot->pFindWord(sHandlerNm);
    if (! pHandler)
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "Unable to find operand handler for: %s"_Stz,
             sHandlerNm.szStz());
    
    m_iOperandCount[i_Kind] = 0;
    m_paOperandHandler[i_Kind] = pHandler;
  }
  
  BASE::vDebugPoint();
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
      // should have received an ZK_ERROREDMARKER
      if (m_iOperandCount[ZK_ERROREDMARKER]) {
        vTrace("%s: %s arrived without a prior ZK_ERROREDMARKER\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      
      // should not have received a ZK_CANCELMARKER already
      if (m_iOperandCount[i_OperandKind]) {
        vTrace("%s: multiple ZK_CANCELMARKER arrived\n"_Stz, szOpNm());
        return false;
      }
      
      // should not have received a ZK_CANCELLEDMARKER
      if ( m_iOperandCount[ZK_CANCELLEDMARKER]) {
        vTrace("%s: %s  arrived after ZK_CANCELLEDMARKER\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      break;
      
    case ZK_CANCELLEDMARKER:
      // Only StartOP receives an ZK_CANCELLEDMARKER
      if ( ! m_sOpNm.bSame("start"_Stz) ) {
        vTrace("%s: incorrectly received a %s\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      
      // should have received a ZK_CANCELMARKER
      if ( ! m_iOperandCount[ZK_CANCELMARKER]) {
        vTrace("%s: %s arrived without a prior ZK_CANCELMARKER\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      
      // should not have received a ZK_CANCELLEDMARKER already
      if (m_iOperandCount[i_OperandKind]) {
        vTrace("multiple ZK_CANCELLEDMARKER arrived\n"_Stz);
        return false;
      }
      
      break;

    case   ZK_CSVBUFFER:
      // should have received a ZK_STARTMARKER
      if ( ! m_iOperandCount[ZK_STARTMARKER]) {
        vTrace("%s: %s arrived without a prior ZK_STARTMARKER\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      break;
      
    case   ZK_CRWBUFFER:
    case   ZK_ENTBUFFER:
    case   ZK_ROWBUFFER:
    case   ZK_SRTBUFFER:
      // should have received a ZK_DATAPULSE
      if ( ! m_iOperandCount[ZK_DATAPULSE]) {
        vTrace("%s: %s arrived without a prior ZK_DATAPULSE\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      
      // should have received a ZK_STREAMDEF
      if ( ! m_iOperandCount[ZK_STREAMDEF]) {
        vTrace("%s: %s arrived without a prior ZK_STREAMDEF\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      
      // should not have received ZK_EOSMARKER's > ZK_DATAPULSE's
      if ( m_iOperandCount[ZK_EOSMARKER] > m_iOperandCount[ZK_DATAPULSE]) {
        vTrace("%s: %s arrived after ZK_EOSMARKER and no new ZK_DATAPULSE\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      
      break;

    case   ZK_DATAPULSE:
      // should have received a ZK_STARTMARKER
      if ( ! m_iOperandCount[ZK_STARTMARKER]) {
        vTrace("%s: %s arrived without a prior ZK_STARTMARKER\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      
      // should not have received a ZK_PULSEDMARKER
      if ( m_iOperandCount[ZK_PULSEDMARKER]) {
        vTrace("%s: %s arrived after ZK_PULSEDMARKER\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      break;
      
    case   ZK_EOSMARKER:
      // should have received a ZK_STARTMARKER
      if ( ! m_iOperandCount[ZK_STARTMARKER]) {
        vTrace("%s: %s arrived without a prior ZK_STARTMARKER\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      
      // should not have received a ZK_EOSMARKER
      if ( m_iOperandCount[ZK_EOSMARKER]) {
        vTrace("%s: multiple ZK_EOSMARKER arrived\n"_Stz, szOpNm());
        return false;
      }
      
      // should not have received a ZK_DATAPULSE
      if ( ! m_iOperandCount[ZK_DATAPULSE]) {
        vTrace("%s: %s arrived without a prior ZK_DATAPULSE\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      break;

    case   ZK_ERRORMARKER:
      // should not have received a ZK_ERRORREDMARKER
      if ( m_iOperandCount[ZK_ERROREDMARKER]) {
        vTrace("%s: %s arrived after ZK_ERRORMARKER\n"_Stz, szOpNm());
        return false;
      }
      break;
      
    case   ZK_ERROREDMARKER:
      // Only StartOP receives an ZK_ERROREDMARKER
      if ( ! m_sOpNm.bSame("start"_Stz) ) {
        vTrace("%s: incorrectly received a %s\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      
      // should have received an ZK_ERRORMARKER
      if (m_iOperandCount[ZK_ERRORMARKER]) {
        vTrace("%s: %s arrived without a prior ZK_ERRORMARKER\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      break;

    case ZK_EXITMARKER:
      // should have received a ZK_STARTMARKER
      if ( ! m_iOperandCount[ZK_STARTMARKER]) {
        vTrace("%s: %s arrived without a prior ZK_STARTMARKER\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      
      // should not have received a ZK_EXITMARKER already
      if (m_iOperandCount[i_OperandKind]) {
        vTrace("%s: multiple ZK_EXITMARKER arrived\n"_Stz, szOpNm());
        return false;
      }
      
      // should not have received a ZK_EXITEDMARKER
      if ( m_iOperandCount[ZK_EXITEDMARKER]) {
        vTrace("%s: %s arrived after ZK_EXITEDMARKER\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      break;

    case ZK_EXITEARLYMARKER:
      // should have received a ZK_STARTMARKER
      if ( ! m_iOperandCount[ZK_STARTMARKER]) {
        vTrace("%s: %s arrived without a prior ZK_STARTMARKER\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }

      // should not have received a ZK_EXITMARKER already
      if (m_iOperandCount[i_OperandKind]) {
        vTrace("%s: multiple ZK_EXITMARKER arrived\n"_Stz, szOpNm());
        return false;
      }
      break;

    case   ZK_EXITEDMARKER:
      // Only StartOP receives an ZK_EXITEDMARKER
      if ( ! m_sOpNm.bSame("start"_Stz) ) {
        vTrace("%s: incorrectly received a %s\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      // should not have received an ZK_EXITEDMARKER already
      if (m_iOperandCount[i_OperandKind]) {
        vTrace("%s: multiple ZK_EXITEDMARKER arrived\n"_Stz, szOpNm());
        return false;
      }
      break;

    case   ZK_EXITEDEARLYMARKER:
      // Only StartOP receives an ZK_EXITEDEARLYMARKER
      if ( ! m_sOpNm.bSame("start"_Stz) ) {
        vTrace("%s: incorrectly received a %s\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      // should not have received an ZK_EXITEDMARKER already
      if (m_iOperandCount[i_OperandKind]) {
        vTrace("%s: multiple ZK_EXITEDMARKER arrived\n"_Stz, szOpNm());
        return false;
      }
      break;

    case ZK_PULSEDMARKER:
      // should have received a ZK_STARTMARKER
      if ( ! m_iOperandCount[ZK_STARTMARKER]) {
        vTrace("%s: %s arrived without a prior ZK_STARTMARKER\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      break;
  
    case   ZK_STARTMARKER:
      // should not have received a ZK_STARTMARKER already
      if (m_iOperandCount[i_OperandKind]) {
        vTrace("%s: multiple ZK_STARTMARKER's arrived\n"_Stz, szOpNm());
        return false;
      }
      break;
      
    case   ZK_STARTEDMARKER:
      // should have received a ZK_STARTMARKER
      if ( ! m_iOperandCount[ZK_STARTMARKER]) {
        vTrace("%s: %s arrived without a prior ZK_STARTMARKER\n"_Stz, szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      
      // should not have received a ZK_STARTEDMARKER already
      if (m_iOperandCount[i_OperandKind]) {
        vTrace("%s: multiple ZK_STARTEDMARKER's arrived\n"_Stz, szOpNm());
        return false;
      }
    
      // Only StartOP/SinkOP receives an ZK_STARTEDMARKER
      if ( ! m_sOpNm.bSame("start"_Stz) ) {
        vTrace("%s: incorrectly received a %s\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      break;

    case   ZK_STREAMDEF:
      // should have received a ZK_STARTMARKER
      if ( ! m_iOperandCount[ZK_STARTMARKER]) {
        vTrace("ZK_STARTEDMARKER arrived without a prior ZK_STARTMARKER\n"_Stz);
        return false;
      }
      break;

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
/* get local robin                                                            */
/*----------------------------------------------------------------------------*/
i8 Op::iLocalRobin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_iLocalRobin;
}

/*----------------------------------------------------------------------------*/
/* set local robin                                                            */
/*----------------------------------------------------------------------------*/
void Op::vLocalRobin(i8 iLocalRobin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_iLocalRobin = iLocalRobin;
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
/* get next op                                                                */
/*----------------------------------------------------------------------------*/
i8 Op::iOtherOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  return m_iOtherOp;
}

/*----------------------------------------------------------------------------*/
/* get other op (in stage)                                                    */
/*----------------------------------------------------------------------------*/
Op* Op::pOtherOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  if (m_iOtherStage != m_pStage->pStageSpec()->i_StageSpec())
    return nullptr;

  Stage* pStage = m_pPipeline->pGet(m_iOtherStage);

  Op* pNextOp = pStage->pGet(m_iOtherStage)->pGet(m_iOtherOp);

  return pNextOp;
}

/*----------------------------------------------------------------------------*/
/* set other op                                                               */
/*----------------------------------------------------------------------------*/
void Op::vOtherOp(i8 iOtherOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  m_iOtherOp = iOtherOp;
}

/*----------------------------------------------------------------------------*/
/* get other stage                                                            */
/*----------------------------------------------------------------------------*/
i8 Op::iOtherStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  return m_iOtherStage;
}

/*----------------------------------------------------------------------------*/
/* set other stage                                                            */
/*----------------------------------------------------------------------------*/
void Op::vOtherStage(i8 iOtherStage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  m_iOtherStage = iOtherStage;
}

/*----------------------------------------------------------------------------*/
/* get outbound operand                                                       */
/*----------------------------------------------------------------------------*/
Operand* Op::pOutboundOperand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pOutboundOperand;
}

/*----------------------------------------------------------------------------*/
/* set outbound operand                                                       */
/*----------------------------------------------------------------------------*/
void Op::vOutboundOperand(Operand* pOutboundOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_pOutboundOperand = pOutboundOperand;
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
/* get pipeline job from op                                                   */
/*----------------------------------------------------------------------------*/
Job* Op::pPipelineJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pPipelineJob;
}

/*----------------------------------------------------------------------------*/
/* append operand to next op's operand queue                                  */
/*----------------------------------------------------------------------------*/
void Op::vPipeToNextOp(Operand* pOperand, Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  if (m_bTraceOp)
    vTrace("%12s (%10s) (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
           m_sOpNm.szStz(),
           m_pStage->szStageNm(),
           m_i_Node,
           m_tPtnNo,
           m_pPipelineJob,
           "vPipeOut :"_Stz,
           AM::pOperandName(pOperand),
           pOperand);
  
  RASSERT((AM::JOB::szJobNm(m_pOpJob)->bSame(szOpNm())));
  
  RASSERT(! szOpNm()->bSame("send"_Stz));
  
    
  // get the next stage the send is supposed to send to
  Stage* pStage = m_pPipeline->pGet(m_iNextStage);

  // get the op to forward to depending on the cross topology
  Op* pOp = pStage->pPipeOutOp(m_eCrossTopo,
                               m_tPtnNo,
                               m_iNextOp);
  if (m_bAppendRight)
    pOp->vAppendRight(pOperand,pTriggeringTask);
  else
    pOp->vAppendLeft(pOperand,pTriggeringTask);
}

/*----------------------------------------------------------------------------*/
/* append operand to the pipeline job's buffer queue                          */
/*----------------------------------------------------------------------------*/
void Op::vPipeToOutput(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Queue* pOutputQueue = AM::pOutputQueue(MY::pSession());

  RASSERT(pOutputQueue);
  
  if (m_bTraceOp)
    vTrace("%12s (%10s) (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
           m_sOpNm.szStz(),
           m_pStage->szStageNm(),
           m_i_Node,
           m_tPtnNo,
           m_pPipelineJob,
           "vPipeToOutput :"_Stz,
           AM::pOperandName(pOperand),
           pOperand);
    
  pOutputQueue->vAppend(pOperand);
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
  
  if (! pOperand)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vDropTaskLock();
    
  AM::vOperandsDec();
  
  if (m_bTraceOp) {
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
    
  vDropTaskLock();
  
  if (pOperand)
    AM::vOperandsDec();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_bTraceOp && pOperand) {
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
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pOperand) {
    if (m_bTraceOp)
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
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pOperand) {
    if (m_bTraceOp)
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
  
  //pPipeLineJobTags->vDumpSet(true);
  
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
  UNUSED(eError);
  //RASSERT(EC_NONE != eError);
  
  // delete the tokenVec
  pTknVec->vSetAllTokens();
  delete pTknVec;
  
  if (StrLib::bSame(m_sOpNm.azChar(), "recv"))
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
/* get stream                                                                 */
/*----------------------------------------------------------------------------*/
Stream* Op::pStream(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pStream;
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
/* get robin                                                                  */
/*----------------------------------------------------------------------------*/
i8 Op::iRobin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  return m_iRobin;
}

/*----------------------------------------------------------------------------*/
/* set robin                                                                  */
/*----------------------------------------------------------------------------*/
void Op::vRobin(i8 iRobin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  m_iRobin = iRobin;
}

/*----------------------------------------------------------------------------*/
/* get send kind                                                              */
/*----------------------------------------------------------------------------*/
eXK Op::eSendKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_eSendKind;
}

/*----------------------------------------------------------------------------*/
/* set send kind                                                              */
/*----------------------------------------------------------------------------*/
void Op::vSendKind(eXK eSendKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_eSendKind = eSendKind;
}

/*----------------------------------------------------------------------------*/
/* get send node                                                              */
/*----------------------------------------------------------------------------*/
i8 Op::iSendNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  return m_iSendNode;
}

/*----------------------------------------------------------------------------*/
/* set send node                                                              */
/*----------------------------------------------------------------------------*/
void Op::vSendNode(i8 iSendNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  m_iSendNode = iSendNode;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
