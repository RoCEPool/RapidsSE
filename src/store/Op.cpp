/*============================================================================*/
/* Op.cpp -- Operator of Stream of Stage of Pipeline                          */
/*============================================================================*/

#include "BASE.h"

#include "eYX.h"

#include "LglPtnMap.hpp"
#include "Node.hpp"
#include "Node_I.hpp"
#include "Grid.hpp"

#include "Op.hpp"
#include "OpSpec.hpp"
#include "Stream.hpp"
#include "Stage.hpp"
#include "Deck.hpp"
#include "Vertex.hpp"
#include "Pipeline.hpp"
#include "Job.hpp"
#include "StageSpec.hpp"
#include "MY.hpp"
#include "Parcel.hpp"
#include "SP_Word.hpp"

#include "eTS.h"
#include "TagConstants.h"

#include "SP_Margin.hpp"
#include "SP_Op.hpp"

#include "CrwBuffer.hpp"
#include "CsvBuffer.hpp"
#include "RowBuffer.hpp"

#include "LogicUnit.hpp"
#include "EventHandlers.hpp"

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
Op::Op(Stream* p_Stream, OpSpec* p_OpSpec)
: Operand(CN_OP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  RASSERT(p_Stream);
  RASSERT(p_OpSpec);

  m_p_Stream = p_Stream;
  m_p_OpSpec = p_OpSpec;

  m_pTask   = nullptr;

  //m_xOpId

  SP_Op spOp;
  m_sPath = spOp.szPath(this);

  m_pOpTags   = nullptr;
  m_bTraceOp  = false;
  m_bTraceAck = false;
  
  m_pEvMgr    = new EvMgr;
  
  m_pPipeIn   = new PipeIn;

  // -- - -- - -- - -- - -- - -- - --

  m_iSendNode   = C_UNKNOWN;
  m_eSendCast   = XK_NONE;
  m_iRobin      = 0;
  m_iLocalRobin = 0;

  m_bPulsedLimit = false;
  m_iWorkQuanta = 0;
  m_iWorkDone = 0;
  
  m_iMetricsQueued = 0;

  _checkInitialised_(CN_OP);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  SMA<Job,CN_JOB> pPipelineJob = p_Stream->pPipeline()->pPipelineJob();
  
  TagSet* pJobTags = AM::pJobTags(pPipelineJob);
  
  if (AM::TRACE::bVerbose(C_VB_BOOT,C_VB_OP) ||
      pJobTags->bHasName(C_TAG_TRACEOP)) {
    m_bTraceOp = true;
  }

  if (AM::TRACE::bVerbose(C_VB_ACK)) {
    m_bTraceAck = true;
  }

  ++ gixOp;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Op::~Op(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  if (m_pPipeIn->pQueueLeft()->nItems() != 0) {
    vTrace("Operand Queue not empty\n"_Stz);
    Operand *pOperand = m_pPipeIn->pQueueLeft()->pPopQueue();
    while (pOperand) {
      vTrace("Operand Queue for OP %s has %s\n"_Stz, szOpNm(), AM::pOperandName(pOperand));
      pOperand = m_pPipeIn->pQueueLeft()->pPopQueue();
    }
    RASSERT(0);
  }
  if (m_pPipeIn->pQueueRight()->nItems() != 0) {
    vTrace("Operand Queue not empty\n"_Stz);
    Operand *pOperand = m_pPipeIn->pQueueRight()->pPopQueue();
    while (pOperand) {
      vTrace("Operand Queue for OP %s has %s\n"_Stz, szOpNm(), AM::pOperandName(pOperand));
      pOperand = m_pPipeIn->pQueueRight()->pPopQueue();
    }
    RASSERT(0);
  }

  delete m_pEvMgr;

  //delete m_pOperandWatch;

  delete m_pPipeIn;
    
  if (m_pTask.bIsNotNull()) {
    Task* pTask = *m_pTask;
    
    while (TS_RETIRED != m_pTask->eState() ) {
      vTrace("Napping\n"_Stz);
      AM::vNapMsec(10, eGetClassNo());
    }

    
    pTask->vGrabTaskLock((Stz*)__func__);
    RASSERT(TS_RETIRED == pTask->eState());
    pTask->vState(TS_DELETING);
    pTask->vDropTaskLock((Stz*)__func__);
    
    m_pTask = nullptr;
    
    delete pTask;

  }

  delete m_pOpTags;

  -- gixOp;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* ack the invoking task                                                      */
/*----------------------------------------------------------------------------*/
void Op::vAckInvokingTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  //Task* pInvokingTask = m_p_PipelineOfOp->pPipelineJob()->pTask();
  //RASSERT(m_pTask->pInvoker() == pInvokingTask);

  SP_Op spOp;
  i8 nAcksWereRemaining;
  Task* pInvokingTask = m_pTask->pInvokingTask();

  if (m_bTraceOp) {

    if (AM::TRACE::bVBlong()) {
      SP_Op spOp;

      vTraceM("%s "
              "%-18s pipelineJob=%p\n"_Stz, spOp.szLeading(this), __func__, pJobOfOp()
      );
    } else {
      vTrace("Op %s (%s (N%lld) (P%02lld)): acking invoking task\n"_Stz,
             szOpNm(),
             m_p_Stream->pStage()->szStageNm(),
             iNodeId(),
             tLglPtn().iAsI8());
    }
  }

  if (m_bTraceOp && AM::TRACE::bVBlong())
    spOp.szLeading(this);

  AM::vAckWaitingTask(pInvokingTask, m_pTask, nAcksWereRemaining);

  if (m_bTraceOp && AM::TRACE::bVBlong()) {
    vTraceM("%s "
            "%-18s nAcksWereRemaining=%lld\n"_Stz
            , spOp.szAck(this), __func__, nAcksWereRemaining
    );
  }

  if (m_bTraceAck)
    vTrace("Op %s (%s (N%lld) (P%02lld)): acking invoking task (acksRemaining : %lld)\n"_Stz,
           szOpNm(),
           m_p_Stream->pStage()->szStageNm(),
           iNodeId(),
           tLglPtn().iAsI8(),
           nAcksWereRemaining);

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
  
  AM::METRICS::vDerefMetrics((i8)pOperand,pOperand->eGetClassNo(),DR_ENQUEUE);
    
  vGrabTaskLock();
  RASSERT(bIsLocked());

  i8 nOperands = m_pPipeIn->pQueueLeft()->iAppendOperand(pOperand);

  if (pOpLogicUnit()->m_xpStats.bIsNotNull()) {
    if (nOperands > pOpLogicUnit()->m_xpStats->m_nMaxQs)
      pOpLogicUnit()->m_xpStats->m_nMaxQs = nOperands;
  }


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
  
  AM::METRICS::vDerefMetrics((i8)pOperand,pOperand->eGetClassNo(),DR_ENQUEUE);

  vGrabTaskLock();
  RASSERT(bIsLocked());

  IGNORE_RETURN(m_pPipeIn->pQueueOoO()->iAppendOperand(pOperand));

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
/* get cross topology                                                         */
/*----------------------------------------------------------------------------*/
eYX Op::eCrossTopology(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_p_OpSpec->eCrossTopology();
}

/*----------------------------------------------------------------------------*/
/* get current operand                                                        */
/*----------------------------------------------------------------------------*/
eZK Op::eCurEvent(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pEvMgr->m_eCurEvent;
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
/* get deck  of this operator                                                 */
/*----------------------------------------------------------------------------*/
Deck* Op::pDeck(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Deck* pDeck = m_p_Stream->pDeck();
  
  return pDeck;
}

  
/*----------------------------------------------------------------------------*/
/* sen . print                                                                */
/*----------------------------------------------------------------------------*/
void Op::vDotPrint(Stz* szText)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  vTrace("[%s: op %10s] "
         "%s"
         "\n"_Stz
         
         ,AM::pGrid()->pThisNode()->szNodeNm()
         ,szOpNm()
         
         ,szText
         
         ,C_VARGS_END
         );
}

/*----------------------------------------------------------------------------*/
/* drop lock                                                                  */
/*----------------------------------------------------------------------------*/
void Op::vDropTaskLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
    
  AM::vDropTaskLock(m_pTask,(Stz*)__func__);
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
  vTrace("OpNm=%s\n"_Stz,szOpNm());
  vTrace("m_i_Node=%lld\n"_Stz,iNodeId());
  vTrace("m_tLglPtn=%lld\n"_Stz,tLglPtn().iAsI8());
}

/*----------------------------------------------------------------------------*/
/* find the entry word for this op                                            */
/*----------------------------------------------------------------------------*/
Word* Op::pEntryWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Dic* pDicRoot = AM::DIC::pRoot();
     
  s32 sEntryWord(szOpNm());
  sEntryWord.vConcat("_op"_Stz);
  
  Word* pEntryWord = pDicRoot->pFindWord(sEntryWord);
  
  if (! pEntryWord) {
    RASSERT(pEntryWord);
  }
  
  return pEntryWord;
}

/*----------------------------------------------------------------------------*/
/* get count for given event                                                  */
/*----------------------------------------------------------------------------*/
i8 Op::nEventCount(eZK eEvent)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  i8 nCount = m_pEvMgr->m_naEventCount[eEvent];
  
  return nCount;
}

/*----------------------------------------------------------------------------*/
/* inc event count for given event                                            */
/*----------------------------------------------------------------------------*/
void Op::vEventCountInc(eZK eEvent)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  m_pEvMgr->m_eCurEvent = eEvent;

  m_pEvMgr->m_naEventCount[m_pEvMgr->m_eCurEvent]++;
  
  i8 iTscp = BASE::iRdtscp();
  m_pEvMgr->m_pOperandWatch->vSaw(m_pEvMgr->m_eCurEvent,iTscp);
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
/* get operand handler for given operand kind                                 */
/*----------------------------------------------------------------------------*/
Word* Op::pEventHandler(i8 i_OperandKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Word* pHandler = m_pEvMgr->m_pEventHandlers->pGet((eZK) i_OperandKind);
  
  return pHandler;
}

/*----------------------------------------------------------------------------*/
/* set operand handler for given operand kind                                 */
/*----------------------------------------------------------------------------*/
void Op::vEventHandler(i8 i_OperandKind, Word* pHandler)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_pEvMgr->m_pEventHandlers->vSet((eZK) i_OperandKind, pHandler);
}

/*----------------------------------------------------------------------------*/
/* grab lock                                                                  */
/*----------------------------------------------------------------------------*/
void Op::vGrabTaskLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  AM::vGrabTaskLock(m_pTask,(Stz*)__func__);
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
bool Op::bIsEventValid(i8 i_OperandKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  switch(i_OperandKind) {

    case ZK_M_CANCELLING:
      
      // should not have received a ZK_M_CANCELLING already
      if (m_pEvMgr->m_naEventCount[i_OperandKind]) {
        vTrace("%s: multiple ZK_M_CANCELLING arrived\n"_Stz, szOpNm());
        return false;
      }
      
      // should not have received a ZK_M_CANCELLED
      if ( m_pEvMgr->m_naEventCount[ZK_M_CANCELLED]) {
        vTrace("%s: %s  arrived after ZK_M_CANCELLED\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      break;
      
    case ZK_M_CANCELLED:
      // Only StartOP receives an ZK_M_CANCELLED
      if ( ! szOpNm()->bSame("start"_Stz) ) {
        vTrace("%s: incorrectly received a %s\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      
      // should have received a ZK_M_CANCEL
      if ( ! m_pEvMgr->m_naEventCount[ZK_M_CANCEL]) {
        vTrace("%s: %s arrived without a prior ZK_M_CANCEL\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      
      // should not have received a ZK_M_CANCELLED already
      if (m_pEvMgr->m_naEventCount[i_OperandKind]) {
        vTrace("multiple ZK_M_CANCELLED arrived\n"_Stz);
        return false;
      }
      
      break;

    case ZK_D_CSVBUFFER:
      // should have received a ZK_M_START
      if ( ! m_pEvMgr->m_naEventCount[ZK_M_START]) {
        vTrace("%s: %s arrived without a prior ZK_M_START\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      break;

    case ZK_D_CRWBUFFER:
    case ZK_D_EACHROWBUFFER:
    case ZK_D_ENTBUFFER:
    case ZK_D_ROWBUFFER:
    case ZK_D_SRTBUFFER:
      // should have received a ZK_M_DATAPULSE
      if ( ! m_pEvMgr->m_naEventCount[ZK_M_DATAPULSE]) {
        vTrace("%s: %s arrived without a prior ZK_M_DATAPULSE\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      
      // should have received a ZK_D_STREAMDEF
      if ( ! m_pEvMgr->m_naEventCount[ZK_D_STREAMDEF]) {
        vTrace("%s: %s arrived without a prior ZK_D_STREAMDEF\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      
      // should not have received ZK_M_EOS's > ZK_M_DATAPULSE's
      if ( m_pEvMgr->m_naEventCount[ZK_M_EOS] > m_pEvMgr->m_naEventCount[ZK_M_DATAPULSE]) {
        vTrace("%s: %s arrived after ZK_M_EOS and no new ZK_M_DATAPULSE\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      
      break;

    case ZK_M_DATAPULSE:
      // should have received a ZK_M_START
      if ( ! m_pEvMgr->m_naEventCount[ZK_M_START]) {
        vTrace("%s: %s arrived without a prior ZK_M_START\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      
      // should not have received a ZK_M_END
      if ( m_pEvMgr->m_naEventCount[ZK_M_END]) {
        vTrace("%s: %s arrived after ZK_M_END\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      break;

    case ZK_M_ENDCRWRHT:
      // should not have received a ZK_M_ENDROWRHT already
      if (m_pEvMgr->m_naEventCount[ZK_M_ENDROWRHT]) {
        vTrace("%s: ZK_M_ENDROWRHT already arrived\n"_Stz, szOpNm());
        return false;
      }

      // should have received a ~ ZK_M_STARTCRWRHT already
      if (m_pEvMgr->m_naEventCount[i_OperandKind] == m_pEvMgr->m_naEventCount[ZK_M_STARTCRWRHT] ) {
        vTrace("%s: ZK_M_STARTCRWRHT not yet arrived\n"_Stz, szOpNm());
        return false;
      }
      break;

    case ZK_M_ENDROWRHT:
      // should not have received a ZK_M_ENDCRWRHT already
      if (m_pEvMgr->m_naEventCount[ZK_M_ENDCRWRHT]) {
        vTrace("%s: ZK_M_ENDCRWRHT already arrived\n"_Stz, szOpNm());
        return false;
      }

      // should have received a ~ ZK_M_STARTROWRHT already
      if (m_pEvMgr->m_naEventCount[i_OperandKind] == m_pEvMgr->m_naEventCount[ZK_M_STARTROWRHT] ) {
        vTrace("%s: ZK_M_STARTROWRHT not yet arrived\n"_Stz, szOpNm());
        return false;
      }
      break;

    case ZK_M_EOS:
      // should have received a ZK_M_START
      if ( ! m_pEvMgr->m_naEventCount[ZK_M_START]) {
        vTrace("%s: %s arrived without a prior ZK_M_START\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      
      // should not have received a ZK_M_EOS
      if ( m_pEvMgr->m_naEventCount[ZK_M_EOS]) {
        vTrace("%s: multiple ZK_M_EOS arrived\n"_Stz, szOpNm());
        return false;
      }
      
      // should not have received a ZK_M_DATAPULSE
      if ( ! m_pEvMgr->m_naEventCount[ZK_M_DATAPULSE]) {
        vTrace("%s: %s arrived without a prior ZK_M_DATAPULSE\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      break;

    case ZK_M_ERROR:
      // should not have received a ZK_ERRORREDMARKER
      if ( m_pEvMgr->m_naEventCount[ZK_M_ERRORED]) {
        vTrace("%s: %s arrived after ZK_M_ERROR\n"_Stz, szOpNm());
        return false;
      }
      break;
      
    case ZK_M_ERRORED:
      // Only StartOP receives an ZK_M_ERRORED
      if ( ! szOpNm()->bSame("start"_Stz) ) {
        vTrace("%s: incorrectly received a %s\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      
      // should have received an ZK_M_ERROR
      if (m_pEvMgr->m_naEventCount[ZK_M_ERROR]) {
        vTrace("%s: %s arrived without a prior ZK_M_ERROR\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      break;

    case ZK_M_EXITING:
      // should have received a ZK_M_START
//      if ( ! m_naOperandCount[ZK_M_START]) {
//        vTrace("%s: %s arrived without a prior ZK_M_START\n"_Stz,
//               szOpNm(), aZK[i_OperandKind]);
//        return false;
//      }
      
      // should not have received a ZK_M_EXITING already
      if (m_pEvMgr->m_naEventCount[i_OperandKind]) {
        vTrace("%s: multiple ZK_M_EXITING arrived\n"_Stz, szOpNm());
        return false;
      }
      
      // should not have received a ZK_M_EXITED
      if ( m_pEvMgr->m_naEventCount[ZK_M_EXITED]) {
        vTrace("%s: %s arrived after ZK_M_EXITED\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      break;

    case ZK_M_LEAVE:
      // should have received a ZK_M_START
      if ( ! m_pEvMgr->m_naEventCount[ZK_M_START]) {
        vTrace("%s: %s arrived without a prior ZK_M_START\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }

      // should not have received a ZK_M_EXITING already
      if (m_pEvMgr->m_naEventCount[i_OperandKind]) {
        vTrace("%s: multiple ZK_M_EXITING arrived\n"_Stz, szOpNm());
        return false;
      }
      break;

    case ZK_M_EXITED:
      // Only StartOP receives an ZK_M_EXITED
      if ( ! szOpNm()->bSame("start"_Stz) ) {
        vTrace("%s: incorrectly received a %s\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      // should not have received an ZK_M_EXITED already
      if (m_pEvMgr->m_naEventCount[i_OperandKind]) {
        vTrace("%s: multiple ZK_M_EXITED arrived\n"_Stz, szOpNm());
        return false;
      }
      break;

    case ZK_M_LEFT:
      // Only StartOP receives an ZK_M_LEFT
      if ( ! szOpNm()->bSame("start"_Stz) ) {
        vTrace("%s: incorrectly received a %s\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      // should not have received an ZK_M_EXITED already
      if (m_pEvMgr->m_naEventCount[i_OperandKind]) {
        vTrace("%s: multiple ZK_M_EXITED arrived\n"_Stz, szOpNm());
        return false;
      }
      break;

    case ZK_M_END:
      // should have received a ZK_M_START
      if ( ! m_pEvMgr->m_naEventCount[ZK_M_START]) {
        vTrace("%s: %s arrived without a prior ZK_M_START\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      break;

    case ZK_M_INVOKE:
      // should not have received a ZK_M_INVOKE already
      if (m_pEvMgr->m_naEventCount[ZK_M_INVOKE]) {
        vTrace("%s: multiple ZK_M_INVOKES's arrived\n"_Stz, szOpNm());
        return false;
      }
      break;
      
    case ZK_M_STARTCRWRHT:
      // should not have received a ZK_M_STARTROWRHT already
      if (m_pEvMgr->m_naEventCount[ZK_M_STARTROWRHT]) {
        vTrace("%s: ZK_M_STARTROWRHT already arrived\n"_Stz, szOpNm());
        return false;
      }

      // should not have received a ~ ZK_M_ENDCRWRHT already
      if (m_pEvMgr->m_naEventCount[i_OperandKind] == m_pEvMgr->m_naEventCount[ZK_M_ENDCRWRHT] + 1 ) {
        vTrace("%s: ZK_M_ENDCRWRHT already arrived\n"_Stz, szOpNm());
        return false;
      }
      break;

    case ZK_M_START:
      // should not have received a ZK_M_START already
      if (m_pEvMgr->m_naEventCount[i_OperandKind]) {
        vTrace("%s: multiple ZK_M_START's arrived\n"_Stz, szOpNm());
        return false;
      }
      break;

    case ZK_M_STARTROWRHT:
      // should not have received a ZK_M_STARTCRWRHT already
      if (m_pEvMgr->m_naEventCount[ZK_M_STARTCRWRHT]) {
        vTrace("%s: ZK_M_STARTCRWRHT already arrived\n"_Stz, szOpNm());
        return false;
      }

      // should not have received a ~ ZK_M_STARTROWRHT already
      if (m_pEvMgr->m_naEventCount[i_OperandKind] == m_pEvMgr->m_naEventCount[ZK_M_ENDROWRHT] + 1 ) {
        vTrace("%s: ZK_M_ENDROWRHT already arrived\n"_Stz, szOpNm());
        return false;
      }

      break;

    case ZK_M_STARTED:
      // should have received a ZK_M_START
      if (szOpNm()->bSame("start"_Stz) ) {
        if (! m_pEvMgr->m_naEventCount[ZK_M_INVOKE]) {
          vTrace("%s: %s arrived without a prior ZK_M_INVOKE\n"_Stz,
                 szOpNm(), aZK[i_OperandKind]);
          return false;
        }
      }
      else {
        if (! m_pEvMgr->m_naEventCount[ZK_M_START]) {
          vTrace("%s: %s arrived without a prior ZK_M_START\n"_Stz,
                 szOpNm(), aZK[i_OperandKind]);
          return false;
        }
      }
      
      // should not have received a ZK_M_STARTED already
      if (m_pEvMgr->m_naEventCount[i_OperandKind]) {
        vTrace("%s: multiple ZK_M_STARTED's arrived\n"_Stz, szOpNm());
        return false;
      }
    
      // Only StartOP/SinkOP receives an ZK_M_STARTED
      if ( ! szOpNm()->bSame("start"_Stz) ) {
        vTrace("%s: incorrectly received a %s\n"_Stz,
               szOpNm(), aZK[i_OperandKind]);
        return false;
      }
      break;

    case ZK_D_STREAMDEF:
      // should have received a ZK_M_START
      if ( ! m_pEvMgr->m_naEventCount[ZK_M_START]) {
        vTrace("ZK_M_STARTED arrived without a prior ZK_M_START\n"_Stz);
        return false;
      }
      break;

    default:
      if (i_OperandKind <= ZK_NONE || i_OperandKind >= ZK_ENUMS) {
        vTrace("Invalid Operand received: %lld\n"_Stz, i_OperandKind);
        return false;
      }
  }
  
  //m_naOperandCount[i_OperandKind]++;
  return true;
}

/*----------------------------------------------------------------------------*/
/* get job of this op                                                         */
/*----------------------------------------------------------------------------*/
Job* Op::pJobOfOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Stream*   pStream   = this->pStream();
  Stage*    pStage    = pStream->pStage();
  Pipeline* pPipeline = pStage->pVertex()->pPipeline();
  Job*      pJob      = pPipeline->pPipelineJob();

  return pJob;
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
/* get metrics queued ops                                                     */
/*----------------------------------------------------------------------------*/
i8 Op::iMetricsQueued(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_iMetricsQueued;
}

/*----------------------------------------------------------------------------*/
/* set metrics queued ops                                                     */
/*----------------------------------------------------------------------------*/
void Op::vMetricsQueued(i8 iMetricsQueued)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_iMetricsQueued = iMetricsQueued;
}

/*----------------------------------------------------------------------------*/
/* get next op                                                                */
/*----------------------------------------------------------------------------*/
i8 Op::iNextOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_p_OpSpec->iNextOp();
}

/*----------------------------------------------------------------------------*/
/* get next op (in current stream)                                            */
/*----------------------------------------------------------------------------*/
Op* Op::pNextOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
    
  i8 i_NextOp = iNextOp();

  Op* p_NextOp = m_p_Stream->pGet(i_NextOp);
  
  return p_NextOp;
}

/*----------------------------------------------------------------------------*/
/* get next stage                                                             */
/*----------------------------------------------------------------------------*/
i8 Op::iNextStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_p_OpSpec->iNextStage();
}

/*----------------------------------------------------------------------------*/
/* get next stage                                                             */
/*----------------------------------------------------------------------------*/
Stage* Op::pNextStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Deck* pDeck    = this->pDeck();
  i8 i_NextStage = this->iNextStage();
  
  Stage* pNextStage = pDeck->pGet(i_NextStage);
  
  return pNextStage;
}

/*----------------------------------------------------------------------------*/
/* get node                                                                   */
/*----------------------------------------------------------------------------*/
i8 Op::iNodeId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  i8 iNodeId;
  
  if (m_p_Stream->pStage()->pStageSpec()->bNodeTopo())
    iNodeId = m_p_Stream->iNodeId();
  else
    iNodeId = AM::pLglPtnMap()->iNodeId(tLglPtn());

  return iNodeId;
}

/*----------------------------------------------------------------------------*/
/* get i_Op                                                                   */
/*----------------------------------------------------------------------------*/
i8 Op::i_Op(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_p_OpSpec->i_OpSpec();;
}

/*----------------------------------------------------------------------------*/
/* get operand watch                                                          */
/*----------------------------------------------------------------------------*/
OperandWatch* Op::pOperandWatch(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pEvMgr->m_pOperandWatch;
}

/*----------------------------------------------------------------------------*/
/* get this op's logic unit                                                   */
/*----------------------------------------------------------------------------*/
LogicUnit* Op::pOpLogicUnit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pTask->pLogicUnit();
}

/*----------------------------------------------------------------------------*/
/* get op name                                                                */
/*----------------------------------------------------------------------------*/
Stz* Op::szOpNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_p_OpSpec->szOpNm();;
}

/*----------------------------------------------------------------------------*/
/* get op spec                                                                */
/*----------------------------------------------------------------------------*/
OpSpec* Op::pOpSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_p_OpSpec;
}

/*----------------------------------------------------------------------------*/
/* get op tag set                                                             */
/*----------------------------------------------------------------------------*/
TagSet* Op::pOpTags(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pOpTags;
}

/*----------------------------------------------------------------------------*/
/* set op tag set                                                             */
/*----------------------------------------------------------------------------*/
void Op::vOpTags(TagSet* pOpTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_pOpTags = pOpTags;
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

         szOpNm(),
         m_p_Stream->pStage()->szStageNm(),
         iNodeId(),
         tLglPtn().iAsI8(),
         pJobOfOp(),

         szFunNm,
         pOperand ? AM::pOperandName(pOperand) : "null"_Stz,
         pOperand,
         szSupplemental);

//  if (pOperand
//        && m_pStream->pStage()->szStageNm()->bSame("StageC"_Stz)
//        && AM::pOperandName(pOperand)->bSame("LOADRESULT"_Stz)
//        && szOpNm()->bSame("consumeOp"_Stz))
//    vDebugPoint();

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
            ,m_pEvMgr->m_naEventCount[m_pEvMgr->m_eCurEvent]
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

  i8 i_OtherOp = m_p_OpSpec->iOtherOp();
  
  return i_OtherOp;
}

/*----------------------------------------------------------------------------*/
/* get other stage                                                            */
/*----------------------------------------------------------------------------*/
i8 Op::iOtherStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  i8 i_OtherStage = m_p_OpSpec->iOtherStage();
  
  return i_OtherStage;
}

/*----------------------------------------------------------------------------*/
/* get other stage                                                            */
/*----------------------------------------------------------------------------*/
Stage* Op::pOtherStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Deck* pDeck     = this->pDeck();
  i8 i_OtherStage = this->iOtherStage();
    
  Stage* pOtherStage = pDeck->pGet(i_OtherStage);
  
  return pOtherStage;
}

/*----------------------------------------------------------------------------*/
/* get operator path                                                          */
/*----------------------------------------------------------------------------*/
Stz* Op::szPath(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_sPath;
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
  
  bOperand = m_pPipeIn->pQueueLeft()->bPeekQueue();
  
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
  
  bOperandOoO = m_pPipeIn->pQueueLeft()->bPeekOoO();
  
  vDropTaskLock();
  
  return bOperandOoO;
}

/*----------------------------------------------------------------------------*/
/* get this op's destination op                                               */
/*----------------------------------------------------------------------------*/
Op* Op::pPipeDstOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
   // get the next stage the send is supposed to send to
  Stage* pDstStage = pNextStage();

  // get the op to forward to depending on the cross topology
  LglPtn tThisPtnId = tLglPtn();
  i8 iNxtOp        = iNextOp();
  Op* pDstOp = pDstStage->pPipeDstOp(tThisPtnId,iNxtOp);
  
  return pDstOp;
}

/*----------------------------------------------------------------------------*/
/* get pipeline of op                                                         */
/*----------------------------------------------------------------------------*/
Pipeline* Op::pPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_p_Stream->pPipeline();
}

/*----------------------------------------------------------------------------*/
/* append operand to next op's operand queue                                  */
/*----------------------------------------------------------------------------*/
void Op::vPipeToNextOp(Operand* pOperand, Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
    
  vOpTrace(pOperand,(Stz*)__func__);
  
  if (pOperand->bIsBuffer()) {
    LogicUnit* pLogicUnit = this->pOpLogicUnit();
    pLogicUnit->m_xpStats->m_nBuffOut++;
  }

  Op* pDstOp = pPipeDstOp();

  pDstOp->vPipeToThis(pOperand,pTriggeringTask);
}

/*----------------------------------------------------------------------------*/
/* append operand to op's operand queue                                       */
/*----------------------------------------------------------------------------*/
void Op::vPipeToThis(Operand* pOperand, Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  vOpTrace(pOperand,(Stz*)__func__);

  eCN eClassNo = pOperand->eGetClassNo();
  if (CN_CRWBUFFER == eClassNo && AM::TRACE::bVerbose(C_VB_PIPEOUT_CRWBUF)) {
    CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
    vTrace("[%6s %10s]-Pipeout : CrwBuffer (%lld rows) to [%s %s]\n"_Stz
           ,pStage()->szStageNm()
           ,szOpNm()
           ,pCrwBuffer->nRows()
           ,this->pStage()->szStageNm()
           ,this->szOpNm()
           );
  }
  else
  if (CN_CSVBUFFER == eClassNo && AM::TRACE::bVerbose(C_VB_PIPEOUT_CSVBUF)) {
    CsvBuffer* pCsvBuffer = _pCsvBuffer(pOperand);
    UNUSED(pCsvBuffer);
    vTrace("[%6s %10s]-Pipeout : CsvBuffer to [%s %s]\n"_Stz
           ,pStage()->szStageNm()
           ,szOpNm()
           ,this->pStage()->szStageNm()
           ,this->szOpNm()
           );
  }
  else
  if (CN_ROWBUFFER == eClassNo && AM::TRACE::bVerbose(C_VB_PIPEOUT_ROWBUF)) {
    RowBuffer* pRowBuffer = _pRowBuffer(pOperand);
    vTrace("[%6s %10s]-Pipeout : RowBuffer (%lld rows) to [%s %s]\n"_Stz
           ,pStage()->szStageNm()
           ,szOpNm()
           ,pRowBuffer->nRows()
           ,this->pStage()->szStageNm()
           ,this->szOpNm()
           );
  }

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
  
  IGNORE_RETURN(pOutputQueue->iAppend(pOperand));
}

/*----------------------------------------------------------------------------*/
/* get pot size                                                               */
/*----------------------------------------------------------------------------*/
i8 Op::nPotSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
    
  i8 nBuffers = pOpLogicUnit()->m_nPotSize;

  return nBuffers;
}

/*----------------------------------------------------------------------------*/
/* pop left operand queue                                                     */
/*----------------------------------------------------------------------------*/
Operand* Op::pPopLeft(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  vGrabTaskLock();
  RASSERT(bIsLocked());
  
  Operand* pOperand = m_pPipeIn->pQueueLeft()->pPopQueue();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pOperand) {
    vDropTaskLock();
    vOpTrace(pOperand,(Stz*)__func__,false);
    AM::vOperandsDec();
    
    AM::METRICS::vDerefMetrics((i8)pOperand,pOperand->eGetClassNo(),DR_DEQUEUE);
   
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

  Operand* pOperand = m_pPipeIn->pQueueOoO()->pPopOoO();
  
  vDropTaskLock();
  
  if (pOperand) {
    AM::vOperandsDec();
    AM::METRICS::vDerefMetrics((i8)pOperand,pOperand->eGetClassNo(),DR_DEQUEUE);
  }
  
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
  
  Operand* pOperand = m_pPipeIn->pQueueOoO()->pPopOoO();
  if (! pOperand) {
    pOperand = m_pPipeIn->pQueueLeft()->pPopQueue();
    bLeft = true;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (! pOperand) {
    RASSERT(bIsLocked());
    return nullptr;
  }
  
  vDropTaskLock();
  AM::vOperandsDec();

  AM::METRICS::vDerefMetrics((i8)pOperand,pOperand->eGetClassNo(),DR_DEQUEUE);

  vOpTrace(pOperand
           ,(Stz*)__func__
           ,false
           ,bLeft ? "Left"_Stz : "OoO"_Stz);

  eCN eClassNo = pOperand->eGetClassNo();
  if (CN_CRWBUFFER == eClassNo && AM::TRACE::bVerbose(C_VB_PIPEIN_CRWBUF)) {
    CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
    UNUSED(pCrwBuffer);
    vTrace("[%6s %10s]-Pipein  : CrwBuffer (%lld rows)\n"_Stz
           ,pStage()->szStageNm()
           ,szOpNm()
           ,pCrwBuffer->nRows()
           );
  }
  else
  if (CN_CSVBUFFER == eClassNo && AM::TRACE::bVerbose(C_VB_PIPEIN_CSVBUF)) {
    CsvBuffer* pCsvBuffer = _pCsvBuffer(pOperand);
    UNUSED(pCsvBuffer);
    vTrace("[%6s %10s]-Pipein  : CsvBuffer\n"_Stz
           ,pStage()->szStageNm()
           ,szOpNm()
           );
  }
  else
  if (CN_ROWBUFFER == eClassNo && AM::TRACE::bVerbose(C_VB_PIPEIN_ROWBUF)) {
    RowBuffer* pRowBuffer = _pRowBuffer(pOperand);
    UNUSED(pRowBuffer);
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
  
  pOperand = m_pPipeIn->pQueueRight()->pPopQueue();
  
  if (pOperand) {
    vDropTaskLock();
    
    AM::vOperandsDec();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pOperand) {
    if (m_bTraceOp)
      vTrace("%s::pPopQueueRight : %s %p\n"_Stz,
             szOpNm(),
             AM::pOperandName(pOperand),
             pOperand);
  }
  
  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* get partition from op                                                      */
/*----------------------------------------------------------------------------*/
LglPtn Op::tLglPtn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  LglPtn tLglPtn;

  if (m_p_Stream->pStage()->pStageSpec()->bNodeTopo())
    tLglPtn = C_UNKNOWN;
  else
    tLglPtn = m_p_Stream->tLglPtn();

  return tLglPtn;
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
  
  i8 nLeft = m_pPipeIn->pQueueLeft()->nQueuedOperands();
  i8 nOoO  = m_pPipeIn->pQueueOoO()->nQueuedOperands();

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
/* get send cast                                                              */
/*----------------------------------------------------------------------------*/
eXK Op::eSendCast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  eXK eSendCast = m_p_OpSpec->eSendCast();
  return eSendCast;
}

/*----------------------------------------------------------------------------*/
/* set send cast                                                              */
/*----------------------------------------------------------------------------*/
void Op::vSendCast(eXK eSendCast)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  RASSERT(eSendCast == m_p_OpSpec->eSendCast());
  m_eSendCast = eSendCast;
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
/* set tags for this op                                                       */
/*----------------------------------------------------------------------------*/
void Op::vSetInvoker(Task* pInvoker)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  if (m_pTask)
    m_pTask->vInvokingTask(pInvoker);
}

/*----------------------------------------------------------------------------*/
/* set tags for this op                                                       */
/*----------------------------------------------------------------------------*/
void Op::vSetTags(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  TagSet* pPipeLineJobTags = AM::pJobTags(pJobOfOp());
  m_pOpTags = _pTagSet(pPipeLineJobTags->pMemoryCopy());
  m_pOpTags->vReplace(C_TAG_JOBNM, szOpNm());
}

/*----------------------------------------------------------------------------*/
/* spawn task for this operator                                               */
/*----------------------------------------------------------------------------*/
void Op::vSpawnTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
      
    
  Job* pJob = this->pJobOfOp();
  pJob->vActiveOpsInc();
  
  if (m_pOpTags->bHasName(C_TAG_DEDICATEDTHD)) {
    
    //m_oOpTags.vDumpSet(true);
    
    AM::vSpawnTaskDedicated_op(this);
  }
//  else
//  if (StrLib::bSame(szOpNm(), "recv")) {
//    
//    m_oOpTags.vDumpSet(true);
//    
//    AM::vSpawnTaskDedicated_op(this);
//  }
  else {
    
    AM::vSpawnTask_op(this);
  }
  
}

/*----------------------------------------------------------------------------*/
/* get operator stage name                                                    */
/*----------------------------------------------------------------------------*/
Stz* Op::szStageName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_p_Stream->pStage()->szStageNm();
}

/*----------------------------------------------------------------------------*/
/* get stage                                                                  */
/*----------------------------------------------------------------------------*/
Stage* Op::pStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_p_Stream->pStage();
}

/*----------------------------------------------------------------------------*/
/* get stream                                                                 */
/*----------------------------------------------------------------------------*/
Stream* Op::pStream(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_p_Stream;
}

/*----------------------------------------------------------------------------*/
/* get stream topology                                                        */
/*----------------------------------------------------------------------------*/
eYS Op::eStreamTopology_op(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_p_OpSpec->eStreamTopology_op();
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
/* get run size                                                               */
/*----------------------------------------------------------------------------*/
i8 Op::nTmpIndexSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
    
  if (pOpLogicUnit()->m_xpCombined.bIsNull())
    return -8;

  Combined* pCombined = * pOpLogicUnit()->m_xpCombined;
  Combined_T* aCombinedT = pCombined->aCombinedT();
  if (! aCombinedT)
    return -7;

  IndexTree* pIndexTree = _pIndexTree(aCombinedT->m_aIndexTreePL);
  if (! pIndexTree)
    return -6;
  
  i8 nEntBuffers = pIndexTree->nEntBuffers();
  
  return nEntBuffers;
}
/*----------------------------------------------------------------------------*/
/* print stage spec                                                           */
/*----------------------------------------------------------------------------*/
void Op::vToCmdStr(Stz* szCmdStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
    
  szCmdStr->vCopy(szOpNm());
  m_p_OpSpec->pArgTags()->vConcat(szCmdStr);
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
  
  RASSERT(pOpLogicUnit());
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
