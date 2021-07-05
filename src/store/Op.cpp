/*============================================================================*/
/* Op.cpp -- Operator of Stream of Stage of Pipeline                          */
/*============================================================================*/

#include "BASE.h"

#include "ePX.h"

#include "PtnMap.hpp"

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

#include "SP_Op.hpp"

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
: Operand(CN_OP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  RASSERT(pOpSpec);
  
  m_pJob        = nullptr;
  
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

  m_bPulsedLimit  = false;
  m_iWorkQuanta   = 1;
  m_iWorkDone     = 0;

  m_pQueueLeft    = new OperandQueue;
  m_pQueueRight   = new OperandQueue;
  m_pQueueOoO     = new OperandQueue;

  m_iOtherStage = C_UNKNOWN;
  m_iOtherOp    = C_UNKNOWN;
  m_iSendNode   = C_UNKNOWN;
  m_eSendKind   = XK_NONE;
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
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Op::Op(Stz* szOpNm)
: Operand(CN_OP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  
  m_sPipelineNm = C_NO_NAME;
  m_sStageNm    = C_NO_NAME;
  m_sStreamNm   = C_NO_NAME;
  m_sOpNm       = szOpNm;
  m_i_Op        = C_UNKNOWN;
  
  m_bTraceOp    = false;
  
  m_pOpSpec   = nullptr;
  m_pStream   = nullptr;
  m_pStage    = nullptr;
  m_pPipeline = nullptr;
  
  m_tPtnNo = C_UNKNOWN;
  m_i_Node = C_UNKNOWN;

    
  // -- -- -- -- -- --

  m_pJob        = nullptr;

  m_bPulsedLimit  = false;
  m_iWorkQuanta   = 1;
  m_iWorkDone     = 0;

  m_pQueueLeft    = new OperandQueue;
  m_pQueueRight   = new OperandQueue;
  m_pQueueOoO     = new OperandQueue;

  m_iOtherStage = C_UNKNOWN;
  m_iOtherOp    = C_UNKNOWN;
  m_iSendNode   = C_UNKNOWN;
  m_eSendKind   = XK_NONE;
  m_iRobin      = 0;
  m_iLocalRobin = 0;
  
  m_pDownstreamOp = nullptr;
  m_pOutboundOperand = nullptr;

  vInitOperandHandlers();

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
    Operand *pOperand = m_pQueueLeft->pPopQueue();
    while (pOperand) {
      vTrace("Operand Queue for OP %s has %s\n"_Stz, m_sOpNm.azChar(), AM::pOperandName(pOperand));
      pOperand = m_pQueueLeft->pPopQueue();
    }
    RASSERT(0);
  }
  if (m_pQueueRight->nItems() != 0) {
    vTrace("Operand Queue not empty\n"_Stz);
    Operand *pOperand = m_pQueueRight->pPopQueue();
    while (pOperand) {
      vTrace("Operand Queue for OP %s has %s\n"_Stz, m_sOpNm.azChar(), AM::pOperandName(pOperand));
      pOperand = m_pQueueRight->pPopQueue();
    }
    RASSERT(0);
  }

  delete m_pQueueLeft;
  delete m_pQueueRight;
  delete m_pQueueOoO;

  -- gixOp;
  
  AM::LOG::vEvent(EV_DESTRUCT,"Op destructor"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set ack job on end marker                                                  */
/*----------------------------------------------------------------------------*/
void Op::vAckJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Task* pTask = AM::pExecutingTask(m_pPipeline->pPipelineJob());
    
  if (m_bTraceOp)
    vTrace("Op %s (%s (N%lld) (P%02lld)): acking pipeline job\n"_Stz,
           AM::JOB::szJobNm(m_pJob),
           m_pStage->szStageNm(),
           m_i_Node,
           m_tPtnNo);
  
  AM::vAckWaitingTask(pTask,AM::pExecutingTask(m_pJob));
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
/* append operand to this op's left operand queue                             */
/*----------------------------------------------------------------------------*/
void Op::vAppendLeft(Operand* pOperand, Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
#ifdef WITH_SEPARATE_OOO_QUEUE
  if (AM::OPERAND::bIsOoO(pOperand)) {
    vAppendOoO(pOperand,pTriggeringTask);
    return;
  }
#endif
      
  vGrabTaskLock();
  RASSERT(bIsLocked());

  m_pQueueLeft->vAppendOperand(pOperand);

  AM::vOperandsInc();

  Task* pDstTask = AM::pExecutingTask(m_pJob);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for biding op/task
  
  eTS eTaskState = (eTS)AM::iTaskState(pDstTask);
  if (TS_BIDING == eTaskState) {
    AM::TASK::vTriggerBiding(pDstTask,pTriggeringTask);
    return;
  }
  else {
    vDropTaskLock();
    return;
  }
}

/*----------------------------------------------------------------------------*/
/* append operand to this op's operand out-of-order queue                     */
/*----------------------------------------------------------------------------*/
#ifdef WITH_SEPARATE_OOO_QUEUE
void Op::vAppendOoO(Operand* pOperand, Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
      
  vGrabTaskLock();
  RASSERT(bIsLocked());

  m_pQueueOoO->vAppendOperand(pOperand);

  AM::vOperandsInc();

  Task* pDstTask = AM::pExecutingTask(m_pJob);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for biding op/task
  
  eTS eTaskState = (eTS)AM::iTaskState(pDstTask);
  if (TS_BIDING == eTaskState) {
    AM::TASK::vTriggerBiding(pDstTask,pTriggeringTask);
    return;
  }
  else {
    vDropTaskLock();
    return;
  }
}
#endif

/*----------------------------------------------------------------------------*/
/* append operand to this op's right operand queue                            */
/*----------------------------------------------------------------------------*/
void Op::vAppendRight(Operand* pOperand, Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
    
  //  vTrace("%s::vAppendOperand : %p %s\n"_Stz,
  //         m_sOpNm(),
  //         pOperand,
  //         aCN[pOperand->eGetClassNo()]);
    
  vGrabTaskLock();
  
  m_pQueueRight->vAppendOperand(pOperand);
  
  AM::vOperandsInc();

  Task* pDstTask = AM::pExecutingTask(m_pJob);
  
  eTS eTaskState = (eTS)AM::iTaskState(pDstTask);
  if (TS_BIDING != eTaskState) {
    vDropTaskLock();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // trigger this biding op
  AM::TASK::vTriggerBiding(pDstTask,pTriggeringTask);
}

/*----------------------------------------------------------------------------*/
/* append right ?                                                             */
/*----------------------------------------------------------------------------*/
bool Op::bAppendRight(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pOpSpec->bAppendRight();
}

/*----------------------------------------------------------------------------*/
/* get cross topology                                                         */
/*----------------------------------------------------------------------------*/
ePX Op::eCrossTopo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pOpSpec->eCrossTopo();
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
  
  AM::vDropTaskLock(AM::pExecutingTask(m_pJob));
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
/* grab lock                                                                  */
/*----------------------------------------------------------------------------*/
void Op::vGrabTaskLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  AM::vGrabTaskLock(AM::pExecutingTask(m_pJob));
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

  Dic* pDicRoot = AM::DIC::pRoot();
  
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
  
  bool bLocked = AM::bTaskIsLocked(AM::pExecutingTask(m_pJob));

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

    case ZK_CSVBUFFER:
      // should have received a ZK_STARTMARKER
      if ( ! m_iOperandCount[ZK_STARTMARKER]) {
        vTrace("%s: %s arrived without a prior ZK_STARTMARKER\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      break;

    case ZK_CRWBUFFER:
    case ZK_EACHROWBUFFER:
    case ZK_ENTBUFFER:
    case ZK_ROWBUFFER:
    case ZK_SRTBUFFER:
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

    case ZK_DATAPULSE:
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
      
    case ZK_EOSMARKER:
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

    case ZK_ERRORMARKER:
      // should not have received a ZK_ERRORREDMARKER
      if ( m_iOperandCount[ZK_ERROREDMARKER]) {
        vTrace("%s: %s arrived after ZK_ERRORMARKER\n"_Stz, szOpNm());
        return false;
      }
      break;
      
    case ZK_ERROREDMARKER:
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

    case ZK_EXITEDMARKER:
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

    case ZK_EXITEDEARLYMARKER:
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
  
    case ZK_STARTMARKER:
      // should not have received a ZK_STARTMARKER already
      if (m_iOperandCount[i_OperandKind]) {
        vTrace("%s: multiple ZK_STARTMARKER's arrived\n"_Stz, szOpNm());
        return false;
      }
      break;
      
    case ZK_STARTEDMARKER:
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

    case ZK_STREAMDEF:
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
/* get job                                                                    */
/*----------------------------------------------------------------------------*/
Job* Op::pJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pJob;
}

/*----------------------------------------------------------------------------*/
/* set job                                                                    */
/*----------------------------------------------------------------------------*/
void Op::vJob(Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_pJob = pJob;
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
  
  return m_pOpSpec->i_NextOp();
}

/*----------------------------------------------------------------------------*/
/* get next op (in stage)                                                     */
/*----------------------------------------------------------------------------*/
Op* Op::pNextOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  if (iNextStage() != m_pStage->pStageSpec()->i_StageSpec())
    return nullptr;
  
  Stage* pStage = m_pPipeline->pGet(iNextStage());

  Op* pNextOp = pStage->pGet(iNextStage())->pGet(iNextOp());
  
  return pNextOp;
}

/*----------------------------------------------------------------------------*/
/* get next stage                                                             */
/*----------------------------------------------------------------------------*/
i8 Op::iNextStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pOpSpec->i_NextStage();
}

/*----------------------------------------------------------------------------*/
/* get next stage                                                             */
/*----------------------------------------------------------------------------*/
Stage* Op::pNextStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Stage* pNextStage = m_pPipeline->pGet(iNextStage());
  
  return pNextStage;
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
/* get m_i_Op                                                                 */
/*----------------------------------------------------------------------------*/
i8 Op::iOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_i_Op;
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
/* get op name                                                                */
/*----------------------------------------------------------------------------*/
Stz* Op::szOpNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_sOpNm;
}

/*----------------------------------------------------------------------------*/
/* get op spec                                                                */
/*----------------------------------------------------------------------------*/
OpSpec* Op::pOpSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pOpSpec;
}

/*----------------------------------------------------------------------------*/
/* get the TagSet                                                             */
/*----------------------------------------------------------------------------*/
TagSet* Op::pOpTagSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pOpSpec->pArgTags();
}

/*----------------------------------------------------------------------------*/
/* trace operator action                                                      */
/*----------------------------------------------------------------------------*/
void Op::vOpTrace(Operand* pOperand, Stz* szFunActNm, Stz* szSupplemental)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  if (! pOperand)
    return;
  
  if (m_bTraceOp) {
    vTrace("%12s (%10s) (N%lld) (P%02lld) (job:%p) %18s %-15s %p %s\n"_Stz,
           m_sOpNm.szStz(),
           m_pStage->szStageNm(),
           m_i_Node,
           m_tPtnNo,
           pPipelineJob(),
           szFunActNm,
           AM::pOperandName(pOperand),
           pOperand,
           szSupplemental);
  }
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
bool Op::bPeekLeft(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  bool bOperand;
  
  vGrabTaskLock();
  
  bOperand = m_pQueueLeft->bPeekQueue();
  
  vDropTaskLock();
  
  return bOperand;
}

/*----------------------------------------------------------------------------*/
/* peek for OoO operand from operand queue                                    */
/*----------------------------------------------------------------------------*/
bool Op::bPeekOoO(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  bool bOperandOoO;
  
  vGrabTaskLock();
  
  bOperandOoO = m_pQueueLeft->bPeekOoO();
  
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
  
  Job* pPipelineJob = m_pPipeline->pPipelineJob();

  
  return pPipelineJob;
}

/*----------------------------------------------------------------------------*/
/* append operand to next op's operand queue                                  */
/*----------------------------------------------------------------------------*/
void Op::vPipeToNextOp(Operand* pOperand, Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  if (m_bTraceOp) {
//    vTrace("%12s (%10s) (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
//           m_sOpNm.szStz(),
//           m_pStage->szStageNm(),
//           m_i_Node,
//           m_tPtnNo,
//           m_pPipelineJob,
//           "vPipeOut :"_Stz,
//           AM::pOperandName(pOperand),
//           pOperand);
    vOpTrace(pOperand,"vPipeToNextOp :"_Stz);
  }
  
  RASSERT((AM::JOB::szJobNm(m_pJob)->bSame(szOpNm())));
  
  RASSERT(! szOpNm()->bSame("send"_Stz));
  
    
  // get the next stage the send is supposed to send to
  Stage* pStage = m_pPipeline->pGet(iNextStage());

  // get the op to forward to depending on the cross topology
  Op* pOp = pStage->pPipeOutOp(eCrossTopo(),
                               m_tPtnNo,
                               iNextOp());
  if (bAppendRight())
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
  
  if (m_bTraceOp) {
//    vTrace("%12s (%10s) (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
//           m_sOpNm.szStz(),
//           m_pStage->szStageNm(),
//           m_i_Node,
//           m_tPtnNo,
//           m_pPipelineJob,
//           "vPipeToOutput :"_Stz,
//           AM::pOperandName(pOperand),
//           pOperand);
    vOpTrace(pOperand,"vPipeToOutput :"_Stz);
  }
  
  pOutputQueue->vAppend(pOperand);
}

/*----------------------------------------------------------------------------*/
/* pop left operand queue                                                     */
/*----------------------------------------------------------------------------*/
Operand* Op::pPopLeft(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  vGrabTaskLock();
  RASSERT(bIsLocked());
  
  Operand* pOperand = m_pQueueLeft->pPopQueue();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pOperand) {
    vDropTaskLock();
    AM::vOperandsDec();
    return pOperand;
  }
  else {
    RASSERT(bIsLocked());
    return nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/* pop operand OoO off the queue                                              */
/*----------------------------------------------------------------------------*/
Operand* Op::pPopOoO(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  vGrabTaskLock();
  RASSERT(bIsLocked());

#ifdef WITH_SEPARATE_OOO_QUEUE
  Operand* pOperand = m_pQueueOoO->pPopOoO();
#else
  Operand* pOperand = m_pQueueLeft->pPopOoO();
#endif
  
  vDropTaskLock();
  
  if (pOperand)
    AM::vOperandsDec();
  
  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* pop operand first off OoO queue, if null then Left queue                   */
/*----------------------------------------------------------------------------*/
Operand* Op::pPopOoOLeft(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  vGrabTaskLock();
  RASSERT(bIsLocked());
  
  //Operand* pOperand = m_pQueueLeft->pPopQueue();
  
#ifdef WITH_SEPARATE_OOO_QUEUE
  Operand* pOperand = m_pQueueOoO->pPopOoO();
  if (! pOperand)
    pOperand = m_pQueueLeft->pPopQueue();
#else
  Operand* pOperand = m_pQueueLeft->pPopOoO();
  if (! pOperand)
    pOperand = m_pQueueLeft->pPopQueue();
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pOperand) {
    vDropTaskLock();
    AM::vOperandsDec();
    return pOperand;
  }
  else {
    RASSERT(bIsLocked());
    return nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/* pop operand queue B                                                        */
/*----------------------------------------------------------------------------*/
Operand* Op::pPopRight(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Operand* pOperand = nullptr;
  
  vGrabTaskLock();
  
  pOperand = m_pQueueRight->pPopQueue();
  
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
/* get number of operands currently queued                                    */
/*----------------------------------------------------------------------------*/
i8 Op::nQueuedOperands(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  i8 nLeft = m_pQueueLeft->nQueuedOperands();
  i8 nOoO  = m_pQueueOoO->nQueuedOperands();

  i8 nOperands = nLeft + nOoO;
  
  return nOperands;
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
  m_pJob = AM::pNewJobOp(this,&xReqTuple);
  
  // get the operator job tag set
  TagSet* pOpJobTagSet = AM::pJobTags(m_pJob);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy the pipeline job tag set into the operator job tags
  TagSet* pPipeLineJobTags = AM::pJobTags(pPipelineJob());
  
  //pPipeLineJobTags->vDumpSet(true);
  
  pOpJobTagSet->vCopy(pPipeLineJobTags);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now copy the hidden topology ename pairs to the job ename set
  pOpJobTagSet->vCopy(m_pOpSpec->pTopoTags());
  
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
  
  eEC eError = AM::ePopulateJob(m_pJob, pOpJobTagSet);
  UNUSED(eError);
  //RASSERT(EC_NONE != eError);
  
  // delete the tokenVec
  pTknVec->vSetAllTokens();
  delete pTknVec;
  
  if (StrLib::bSame(m_sOpNm.azChar(), "recv"))
    AM::vSpawnJob_dedicated(m_pJob);
  else
    AM::vSpawnJob_op(m_pJob);
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
  
  return AM::pExecutingTask(m_pJob);
}

/*----------------------------------------------------------------------------*/
/* print stage spec                                                           */
/*----------------------------------------------------------------------------*/
void Op::vToCmdStr(Stz* szCmdStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
    
  szCmdStr->vCopy(m_sOpNm);
  m_pOpSpec->pArgTags()->vConcat(szCmdStr);
  //vTrace("%s\n"_Stz, szCmdStr);
}

/*----------------------------------------------------------------------------*/
/* get the topology TagSet                                                    */
/*----------------------------------------------------------------------------*/
TagSet* Op::pTopologyTagSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pOpSpec->pTopoTags();
}

/*----------------------------------------------------------------------------*/
/* set work quanta                                                            */
/*----------------------------------------------------------------------------*/
void Op::vWorkQuanta(i8 iWorkQuanta)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_iWorkQuanta = iWorkQuanta;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
