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

#include "SP_Margin.hpp"
#include "SP_Op.hpp"

#include "CrwBuffer.hpp"
#include "CsvBuffer.hpp"
#include "RowBuffer.hpp"

#include "LogicUnit.hpp"

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
  
  m_pOpJob       = nullptr;
  
  m_pOpLogicUnit = nullptr;

  m_sPipelineNm = pStream->pPipeline()->szPipelineNm();
  m_sStageNm    = pStream->pStage()->szStageNm();
  m_sStreamNm   = pStream->szStreamNm();
  m_sOpNm       = pOpSpec->szOpNm();
  m_i_Op        = pOpSpec->iOpSpec();
  
  m_bTraceOp    = false;
  
  m_pOpSpec   = pOpSpec;
  m_pStream   = pStream;
  m_pStage    = pStream->pStage();
  m_p_Pipeline_op = pStream->pPipeline();

  // -- -- -- -- -- --

  // m_oOpTags
  m_pTask = nullptr;
  
  m_eSeed = SS_SEED_REQ;
  
  // -- -- -- -- -- --

  m_bPulsedLimit  = false;
  m_iWorkQuanta   = 1;
  m_iWorkDone     = 0;

  m_pQueueLeft    = new OperandQueue;
  m_pQueueRight   = new OperandQueue;
  m_pQueueOoO     = new OperandQueue;

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
  
  if (AM::TRACE::bVerbose(C_VB_BOOT,C_VB_OP) ||
      pJobTags->bHasName(C_TAG_TRACEOP)) {
    m_bTraceOp = true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ++ gixOp;
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Op::Op(Stz* szOpNm)
: Operand(CN_OP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_pOpJob       = nullptr;
  
  m_pOpLogicUnit = nullptr;

  m_sPipelineNm = C_NO_NAME;
  m_sStageNm    = C_NO_NAME;
  m_sStreamNm   = C_NO_NAME;
  m_sOpNm       = szOpNm;
  m_i_Op        = C_UNKNOWN;
  
  m_bTraceOp    = false;
  
  m_pOpSpec   = nullptr;
  m_pStream   = nullptr;
  m_pStage    = nullptr;
  m_p_Pipeline_op = nullptr;
  
  // -- -- -- -- -- --

  // m_oOpTags
  m_pTask = nullptr;

  // -- -- -- -- -- --

  m_pOpJob        = nullptr;

  m_bPulsedLimit  = false;
  m_iWorkQuanta   = 1;
  m_iWorkDone     = 0;

  m_pQueueLeft    = new OperandQueue;
  m_pQueueRight   = new OperandQueue;
  m_pQueueOoO     = new OperandQueue;

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
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Op::~Op(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  if (m_pOpLogicUnit) {
    RASSERT(this == m_pOpLogicUnit->pOp());
    RASSERT(! m_pOpLogicUnit->pJob());

    delete m_pOpLogicUnit;
    m_pOpLogicUnit = nullptr;
  }
  
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
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set ack job on end marker                                                  */
/*----------------------------------------------------------------------------*/
void Op::vAckPipelineJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Task* pPipelineTask = AM::pExecutingTask(m_p_Pipeline_op->pPipelineJob());
    

  if (m_bTraceOp) {
    
    if (AM::TRACE::bVBlong()) {
      SP_Op spOp;

      vTraceM("%s "
              "%-18s pipelineJob=%p\n"_Stz
             
              ,spOp.szLeading(this)
             
              ,__func__
              ,pPipelineJob()
             );
    }
    else {
      vTrace("Op %s (%s (N%lld) (P%02lld)): acking pipeline job\n"_Stz,
             szOpNm(),
             m_pStage->szStageNm(),
             iNode(),
             tPtnNo());
    }
  }
  
  i8 nAcksWereRemaining;
  
  SP_Op spOp;

  if (m_bTraceOp && AM::TRACE::bVBlong())
    spOp.szLeading(this);
  
  AM::vAckWaitingTask(pPipelineTask,m_pTask,nAcksWereRemaining);
  
  if (m_bTraceOp && AM::TRACE::bVBlong()) {
      SP_Op spOp;

      vTraceM("%s "
              "%-18s nAcksWereRemaining=%lld\n"_Stz
             
              ,spOp.sz()
             
              ,__func__
              ,nAcksWereRemaining
             );
   }
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
  
  if (AM::OPERAND::bIsOoO(pOperand)) {
    vAppendOoO(pOperand,pTriggeringTask);
    return;
  }
      
  vGrabTaskLock();
  RASSERT(bIsLocked());

  m_pQueueLeft->vAppendOperand(pOperand);

  AM::vOperandsInc();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for biding op/task
  
  eTS eTaskState = (eTS)AM::iTaskState(m_pTask);
  if (TS_BIDING == eTaskState) {
    AM::TASK::vTriggerBiding(m_pTask,pTriggeringTask);
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
void Op::vAppendOoO(Operand* pOperand, Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  vGrabTaskLock();
  RASSERT(bIsLocked());

  m_pQueueOoO->vAppendOperand(pOperand);

  AM::vOperandsInc();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for biding op/task
  
  eTS eTaskState = (eTS)AM::iTaskState(m_pTask);
  if (TS_BIDING == eTaskState) {
    AM::TASK::vTriggerBiding(m_pTask,pTriggeringTask);
    return;
  }
  else {
    vDropTaskLock();
    return;
  }
}

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

  eTS eTaskState = (eTS)AM::iTaskState(m_pTask);
  if (TS_BIDING != eTaskState) {
    vDropTaskLock();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // trigger this biding op
  AM::TASK::vTriggerBiding(m_pTask,pTriggeringTask);
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
/* get current handler                                                        */
/*----------------------------------------------------------------------------*/
i8 Op::iCurHandler(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_i_CurHandler;
}

/*----------------------------------------------------------------------------*/
/* get current executing word                                                 */
/*----------------------------------------------------------------------------*/
Word* Op::pCurWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Task* pTask = MY::pTask();
  
  Word* pCurWord = pTask->pCurWord();
  
  return pCurWord;
}

/*----------------------------------------------------------------------------*/
/* sen . print                                                                */
/*----------------------------------------------------------------------------*/
void Op::vDotPrint(Stz* szText)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  vTrace("[%s: op %s] "
         "%s"
         "\n"_Stz
         
         ,AM::pGrid()->pThisNode()->szNodeNm()
         ,szOpNm()
         
         ,szText
         
         ,C_VARGS_END
         );
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
    
  AM::vDropTaskLock(m_pTask);
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
  vTrace("m_i_Node=%lld\n"_Stz,iNode());
  vTrace("m_tPtnNo=%lld\n"_Stz,tPtnNo());
}

/*----------------------------------------------------------------------------*/
/* get event hander                                                           */
/*----------------------------------------------------------------------------*/
Word* Op::pEventHandler(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Task* pTask = MY::pTask();
  
  Word* pEventHandler = pTask->pEventHandler();
  
  return pEventHandler;
}

/*----------------------------------------------------------------------------*/
/* grab lock                                                                  */
/*----------------------------------------------------------------------------*/
void Op::vGrabTaskLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  AM::vGrabTaskLock(m_pTask);
}

/*----------------------------------------------------------------------------*/
/* get handler count for given handler                                        */
/*----------------------------------------------------------------------------*/
i8 Op::iHandlerCount(i8 i_Handler)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  i8 iCount = m_iHandlerCount[i_Handler];
  
  return iCount;
}

/*----------------------------------------------------------------------------*/
/* inc handler count for given handler                                        */
/*----------------------------------------------------------------------------*/
void Op::vIncHandlerCount(i8 i_Handler)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_i_CurHandler = i_Handler;
  
  m_iHandlerCount[m_i_CurHandler]++;
}

/*----------------------------------------------------------------------------*/
/* inc operand count for given operand kind                                   */
/*----------------------------------------------------------------------------*/
void Op::vIncOperandCount(i8 i_OperandKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  m_i_CurOperand = i_OperandKind;

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
      vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
             "Unable to find operand handler for: %s"_Stz,
             sHandlerNm.szStz());
    
    m_iOperandCount[i_Kind] = 0;
    m_paOperandHandler[i_Kind] = pHandler;
    m_iHandlerCount[i_Kind] = 0;

  }
  
  m_i_CurHandler = 0;
  m_i_CurOperand = 0;
}

/*----------------------------------------------------------------------------*/
/* is locked?                                                                 */
/*----------------------------------------------------------------------------*/
bool Op::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  bool bLocked = AM::bTaskIsLocked(m_pTask);

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

    case ZK_END_CRWRHTMARKER:
      // should not have received a ZK_END_ROWRHTMARKER already
      if (m_iOperandCount[ZK_END_ROWRHTMARKER]) {
        vTrace("%s: ZK_END_ROWRHTMARKER already arrived\n"_Stz, szOpNm());
        return false;
      }

      // should have received a ~ ZK_START_CRWRHTMARKER already
      if (m_iOperandCount[i_OperandKind] == m_iOperandCount[ZK_START_CRWRHTMARKER] ) {
        vTrace("%s: ZK_START_CRWRHTMARKER not yet arrived\n"_Stz, szOpNm());
        return false;
      }
      break;

    case ZK_END_ROWRHTMARKER:
      // should not have received a ZK_END_CRWRHTMARKER already
      if (m_iOperandCount[ZK_END_CRWRHTMARKER]) {
        vTrace("%s: ZK_END_CRWRHTMARKER already arrived\n"_Stz, szOpNm());
        return false;
      }

      // should have received a ~ ZK_START_ROWRHTMARKER already
      if (m_iOperandCount[i_OperandKind] == m_iOperandCount[ZK_START_ROWRHTMARKER] ) {
        vTrace("%s: ZK_START_ROWRHTMARKER not yet arrived\n"_Stz, szOpNm());
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
//      if ( ! m_iOperandCount[ZK_STARTMARKER]) {
//        vTrace("%s: %s arrived without a prior ZK_STARTMARKER\n"_Stz,
//               szOpNm(), aZK[i_OperandKind]);
//        return false;
//      }
      
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

    case ZK_LEAVEMARKER:
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

    case ZK_LEFTMARKER:
      // Only StartOP receives an ZK_LEFTMARKER
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

    case ZK_START_CRWRHTMARKER:
      // should not have received a ZK_START_ROWRHTMARKER already
      if (m_iOperandCount[ZK_START_ROWRHTMARKER]) {
        vTrace("%s: ZK_START_ROWRHTMARKER already arrived\n"_Stz, szOpNm());
        return false;
      }

      // should not have received a ~ ZK_END_CRWRHTMARKER already
      if (m_iOperandCount[i_OperandKind] == m_iOperandCount[ZK_END_CRWRHTMARKER] + 1 ) {
        vTrace("%s: ZK_END_CRWRHTMARKER already arrived\n"_Stz, szOpNm());
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

    case ZK_START_ROWRHTMARKER:
      // should not have received a ZK_START_CRWRHTMARKER already
      if (m_iOperandCount[ZK_START_CRWRHTMARKER]) {
        vTrace("%s: ZK_START_CRWRHTMARKER already arrived\n"_Stz, szOpNm());
        return false;
      }

      // should not have received a ~ ZK_START_ROWRHTMARKER already
      if (m_iOperandCount[i_OperandKind] == m_iOperandCount[ZK_END_ROWRHTMARKER] + 1 ) {
        vTrace("%s: ZK_END_ROWRHTMARKER already arrived\n"_Stz, szOpNm());
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
  
  //m_iOperandCount[i_OperandKind]++;
  return true;
}

/*----------------------------------------------------------------------------*/
/* get job                                                                    */
/*----------------------------------------------------------------------------*/
Job* Op::pOpJob_(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pOpJob;
}

/*----------------------------------------------------------------------------*/
/* set job                                                                    */
/*----------------------------------------------------------------------------*/
void Op::vOpJob_(Job* pOpJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_pOpJob = pOpJob;
}

/*----------------------------------------------------------------------------*/
/* generate next local round robin                                            */
/*----------------------------------------------------------------------------*/
i8 Op::iLocalRobinNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_iLocalRobin++;
}

/*----------------------------------------------------------------------------*/
/* get next op                                                                */
/*----------------------------------------------------------------------------*/
i8 Op::iNextOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pOpSpec->iNextOp();
}

/*----------------------------------------------------------------------------*/
/* get next op (in stage)                                                     */
/*----------------------------------------------------------------------------*/
Op* Op::pNextOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
    
  i8 i_NextStage = iNextStage();
  i8 i_NextOp    = iNextOp();

  Stage* p_NextStage = m_p_Pipeline_op->pGet(i_NextStage);

  Stream* p_NextStream = p_NextStage->pGet(0);
  Op* p_NextOp = p_NextStream->pGet(i_NextOp);
  
  return p_NextOp;
}

/*----------------------------------------------------------------------------*/
/* get next stage                                                             */
/*----------------------------------------------------------------------------*/
i8 Op::iNextStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pOpSpec->iNextStage();
}

/*----------------------------------------------------------------------------*/
/* get next stage                                                             */
/*----------------------------------------------------------------------------*/
Stage* Op::pNextStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Stage* pNextStage = m_p_Pipeline_op->pGet(iNextStage());
  
  return pNextStage;
}

/*----------------------------------------------------------------------------*/
/* get node                                                                   */
/*----------------------------------------------------------------------------*/
i8 Op::iNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  i8 i_Node;
  
  if (m_pStage->pStageSpec()->bNodeTopo())
    i_Node = m_pStream->i_Node();
  else
    i_Node = AM::pPtnMap()->iWhichNodeNo(tPtnNo());

  return i_Node;
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
/* get operand count   for given operand kind                                 */
/*----------------------------------------------------------------------------*/
i8 Op::iOperandCount(i8 i_OperandKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  i8 iCount = m_iOperandCount[i_OperandKind];
  
  return iCount;
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
/* get this op's logic unit                                                   */
/*----------------------------------------------------------------------------*/
LogicUnit* Op::pOpLogicUnit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pOpLogicUnit;
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
/* get op tag set                                                             */
/*----------------------------------------------------------------------------*/
TagSet* Op::pOpTags(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return & m_oOpTags;
}

/*----------------------------------------------------------------------------*/
/* trace operator action                                                      */
/*----------------------------------------------------------------------------*/
void Op::vOpTrace(Operand* pOperand
                  ,Stz* szFunNm
                  ,bool bHandler
                  ,Stz* szSupplemental
                  )
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  if (AM::TRACE::bVBlong())
    goto L_Long_Form;

  
  if (! AM::TRACE::bVerbose(C_VB_OP))
    return;

  vTrace("%12s (%10s) (N%lld) (P%02lld) (job:%p) "
         "%18s %-15s %p %s\n"_Stz,

         m_sOpNm.szStz(),
         m_pStage->szStageNm(),
         iNode(),
         tPtnNo(),
         pPipelineJob(),

         szFunNm,
         pOperand ? AM::pOperandName(pOperand) : "null"_Stz,
         pOperand,
         szSupplemental);
  
  return;
  
  L_Long_Form:
  
  //if (m_bTraceOp) {
  if (! AM::TRACE::bVerbose(C_VB_OP))
    return;
  
  SP_Op spOp;
  
  if (pOperand) {
    if (bHandler) {
      vTraceM("%s "
            "%-18s %s #%0004ld @%p %s\n"_Stz
                    
            ,spOp.szLeading(this)
           
            ,szFunNm
            ,AM::pOperandName(pOperand)
            ,m_iOperandCount[m_i_CurOperand]
            ,pOperand
            ,szSupplemental
           );
    }
    else {
      vTraceM("%s "
              "%-18s %s @%p %s\n"_Stz
                      
              ,spOp.szLeading(this)
             
              ,szFunNm
              ,AM::pOperandName(pOperand)
              ,pOperand
              ,szSupplemental
             );
    }
  }
  else {
    vTraceM("%s "
            "%-18s %s\n"_Stz
                  
           ,spOp.szLeading(this)
         
           ,szFunNm
           ,szSupplemental
         );

  }
}

/*----------------------------------------------------------------------------*/
/* get next op                                                                */
/*----------------------------------------------------------------------------*/
i8 Op::iOtherOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  i8 i_OtherOp = m_pOpSpec->iOtherOp();
  
  return i_OtherOp;
}

/*----------------------------------------------------------------------------*/
/* get other stage                                                            */
/*----------------------------------------------------------------------------*/
i8 Op::iOtherStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  i8 i_OtherStage = m_pOpSpec->iOtherStage();
  
  return i_OtherStage;
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
Pipeline* Op::pPipeline_op(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_p_Pipeline_op;
}

/*----------------------------------------------------------------------------*/
/* get pipeline job from op                                                   */
/*----------------------------------------------------------------------------*/
Job* Op::pPipelineJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Job* pPipelineJob = m_p_Pipeline_op->pPipelineJob();

  
  return pPipelineJob;
}

/*----------------------------------------------------------------------------*/
/* append operand to next op's operand queue                                  */
/*----------------------------------------------------------------------------*/
void Op::vPipeToNextOp(Operand* pOperand, Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  vOpTrace(pOperand,(Stz*)__func__);
    
  RASSERT(! szOpNm()->bSame("send"_Stz));
  
    
  // get the next stage the send is supposed to send to
  Stage* pDstStage = m_p_Pipeline_op->pGet(iNextStage());

  // get the op to forward to depending on the cross topology
  Op* pDstOp = pDstStage->pPipeOutOp(eCrossTopo(),tPtnNo(),iNextOp());
  
  eCN eClassNo = pOperand->eGetClassNo();
  if (CN_CRWBUFFER == eClassNo && AM::TRACE::bVerbose(C_VB_PIPEOUT_CRWBUF)) {
    CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
    vTrace("[%6s %10s]-Pipeout : CrwBuffer (%lld rows) to [%s %s]\n"_Stz
           ,pStage()->szStageNm()
           ,szOpNm()
           ,pCrwBuffer->nRows()
           ,pDstOp->pStage()->szStageNm()
           ,pDstOp->szOpNm()
           );
  }
  else
  if (CN_CSVBUFFER == eClassNo && AM::TRACE::bVerbose(C_VB_PIPEOUT_CSVBUF)) {
    CsvBuffer* pCsvBuffer = _pCsvBuffer(pOperand);
    vTrace("[%6s %10s]-Pipeout : CsvBuffer to [%s %s]\n"_Stz
           ,pStage()->szStageNm()
           ,szOpNm()
           ,pDstOp->pStage()->szStageNm()
           ,pDstOp->szOpNm()
           );
  }
  else
  if (CN_ROWBUFFER == eClassNo && AM::TRACE::bVerbose(C_VB_PIPEOUT_ROWBUF)) {
    RowBuffer* pRowBuffer = _pRowBuffer(pOperand);
    vTrace("[%6s %10s]-Pipeout : RowBuffer (%lld rows) to [%s %s]\n"_Stz
           ,pStage()->szStageNm()
           ,szOpNm()
           ,pRowBuffer->nRows()
           ,pDstOp->pStage()->szStageNm()
           ,pDstOp->szOpNm()
           );
  }

  if (bAppendRight())
    pDstOp->vAppendRight(pOperand,pTriggeringTask);
  else
    pDstOp->vAppendLeft(pOperand,pTriggeringTask);
}

/*----------------------------------------------------------------------------*/
/* append operand to op's operand queue                                       */
/*----------------------------------------------------------------------------*/
void Op::vPipeToSelf(Operand* pOperand, Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  vOpTrace(pOperand,(Stz*)__func__);

  RASSERT(! szOpNm()->bSame("send"_Stz));

  if (bAppendRight())
    vAppendRight(pOperand,pTriggeringTask);
  else
    vAppendLeft(pOperand,pTriggeringTask);
}

/*----------------------------------------------------------------------------*/
/* append operand to the pipeline job's buffer queue                          */
/*----------------------------------------------------------------------------*/
void Op::vPipeToOutput(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Session* pSession = MY::pSession();
  
  Queue* pOutputQueue = AM::pOutputQueue(pSession);

  RASSERT(pOutputQueue);
  
  vOpTrace(pOperand,(Stz*)__func__);
  
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
    vOpTrace(pOperand,(Stz*)__func__,false);
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

  Operand* pOperand = m_pQueueOoO->pPopOoO();
  
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
  
  bool bLeft = false;
  
  Operand* pOperand = m_pQueueOoO->pPopOoO();
  if (! pOperand) {
    pOperand = m_pQueueLeft->pPopQueue();
    bLeft = true;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (! pOperand) {
    RASSERT(bIsLocked());
    return nullptr;
  }
  
  vDropTaskLock();
  AM::vOperandsDec();

  vOpTrace(pOperand
           ,(Stz*)__func__
           ,false
           ,bLeft ? "Left"_Stz : "OoO"_Stz);

  eCN eClassNo = pOperand->eGetClassNo();
  if (CN_CRWBUFFER == eClassNo && AM::TRACE::bVerbose(C_VB_PIPEIN_CRWBUF)) {
    CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
    vTrace("[%6s %10s]-Pipein  : CrwBuffer (%lld rows)\n"_Stz
           ,pStage()->szStageNm()
           ,szOpNm()
           ,pCrwBuffer->nRows()
           );
  }
  else
  if (CN_CSVBUFFER == eClassNo && AM::TRACE::bVerbose(C_VB_PIPEIN_CSVBUF)) {
    CsvBuffer* pCsvBuffer = _pCsvBuffer(pOperand);
    vTrace("[%6s %10s]-Pipein  : CsvBuffer\n"_Stz
           ,pStage()->szStageNm()
           ,szOpNm()
           );
  }
  else
  if (CN_ROWBUFFER == eClassNo && AM::TRACE::bVerbose(C_VB_PIPEIN_ROWBUF)) {
    RowBuffer* pRowBuffer = _pRowBuffer(pOperand);
    vTrace("[%6s %10s]-Pipein  : RowBuffer (%lld rows)\n"_Stz
           ,pStage()->szStageNm()
           ,szOpNm()
           ,pRowBuffer->nRows()
           );
  }

  return pOperand;

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
/* get partition from op                                                      */
/*----------------------------------------------------------------------------*/
tPN Op::tPtnNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  i8 tPtnNo;

  if (m_pStage->pStageSpec()->bNodeTopo())
    tPtnNo = C_UNKNOWN;
  else
    tPtnNo = m_pStream->tPtnNo();
  
  return tPtnNo;
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
/* generate next round robin                                                  */
/*----------------------------------------------------------------------------*/
i8 Op::iRobinNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_iRobin++;
}

/*----------------------------------------------------------------------------*/
/* get the seed to be spawned                                                 */
/*----------------------------------------------------------------------------*/
Stz* Op::szSeed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  Stz* szSeed = AM::SEED::szDecodeSeed(m_eSeed,&m_oOpTags);
    
  return szSeed;
}

/*----------------------------------------------------------------------------*/
/* get send kind                                                              */
/*----------------------------------------------------------------------------*/
eXK Op::eSendKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  eXK eSendKind = m_pOpSpec->eSendKind();
  return eSendKind;
}

/*----------------------------------------------------------------------------*/
/* set send kind                                                              */
/*----------------------------------------------------------------------------*/
void Op::vSendKind(eXK eSendKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  RASSERT(eSendKind == m_pOpSpec->eSendKind());
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
/* spawn this operator                                                        */
/*----------------------------------------------------------------------------*/
  // revised job/op model

void Op::vSpawnTaskForOp(i8 iSessionId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
    
  TagSet* pPipeLineJobTags = AM::pJobTags(pPipelineJob());
  m_oOpTags.vCopy(pPipeLineJobTags);
  m_oOpTags.vReplace(C_TAG_JOBNM, m_sOpNm);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form and parse op command string
  
  sSTMT sOpCmd;
  vToCmdStr(sOpCmd);
  
  //vTrace("<><><> %s\n"_Stz,sOpCmd.azChar());

  TokenVec* pTknVec = new TokenVec;
  pTknVec->vTokenise(sOpCmd);

  bool b = AM::bParseCmd(pTknVec,&m_oOpTags);
  RASSERT(b);
  
  pTknVec->vSetAllTokens();
  delete pTknVec;
  
  //pOpJobTagSet->vDumpSet();
  
//  if (! b) {
//    //vTrace("Unable to match job syntax\n"_Stz);
//    vParticipantHasError(true);
//    return EC_INVALID_COMMAND;
//  }
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pOpLogicUnit = new LogicUnit;
  m_pOpLogicUnit->vOp(this);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // create a new Job for the Op
//  m_pOpJob = AM::pNewJobOp(this,
//                         AM::GRID::iThisNodeNo(),
//                         iSessionId,
//                         1
//                         );
//
//  // get the operator job tag set
//  TagSet* pOpJobTagSet = AM::pJobTags(m_pOpJob);
//  pOpJobTagSet->vCopy(&m_oOpTags);

  if (StrLib::bSame(m_sOpNm.azChar(), "recv")) {
    AM::vSpawnTaskDedicated_op(this);
//    m_pTask = AM::pExecutingTask(m_pOpJob);
//    RASSERT(m_pTask->pJob() == m_pOpJob);
//    m_pTask->vOp(this);
  }
  else {
    AM::vSpawnTask_op(this);
//    m_pTask = AM::pExecutingTask(m_pOpJob);
//    RASSERT(m_pTask->pJob() == m_pOpJob);
//    m_pTask->vOp(this);
  }
  
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
  
  return m_pTask;
}

/*----------------------------------------------------------------------------*/
/* set task                                                                   */
/*----------------------------------------------------------------------------*/
void Op::vTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_pTask = pTask;
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
/* verify op is composed correctly and therefore capable of being executed    */
/*----------------------------------------------------------------------------*/
void Op::vVerifyOpComposition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  RASSERT(EC_NONE == eValidHeader());
  RASSERT(CN_OP == eGetClassNo());
  aFooter()->vAssertFooter();
  
  RASSERT(m_pOpLogicUnit);
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
