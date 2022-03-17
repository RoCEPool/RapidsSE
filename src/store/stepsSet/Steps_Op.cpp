/*============================================================================*/
/* Steps_Op.cpp -- Opeartor step group                                        */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "Node_I.hpp"
#include "Op.hpp"
#include "Pipeline.hpp"
#include "StageSpec.hpp"

#include "Job.hpp"
#include "LogicUnit.hpp"
#include "Stream.hpp"
#include "Stage.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Op(void)
{
  vAddStep("op.pipeBackin"_Stz,       &Step::vOpPipeBackIn);
  vAddStep("op.pipeout"_Stz,         &Step::vOpPipeOut);

  vAddStep("Op.ackPipelineJob"_Stz,   &Step::vOpAckPipelineJob);
  
  vAddStep("Op.downstreamOp!"_Stz,    &Step::vOpDownstreamOpStore);
  
  vAddStep("Op.findOp"_Stz,           &Step::vOpFindOp);
  
  vAddStep("Op.opName@"_Stz,          &Step::vOpOpNameFetch);
  vAddStep("Op.outboundOperand@"_Stz, &Step::vOpOutboundOperandFetch);
  vAddStep("Op.outboundOperand!"_Stz, &Step::vOpOutboundOperandStore);
  
  vAddStep("Op.partition@"_Stz,       &Step::vOpPartitionFetch);
  vAddStep("Op.pipelineJob@"_Stz,     &Step::vOpPiplineJobFetch);
  
  vAddStep("Op.queueLength@"_Stz,     &Step::vOpQueueLength);
  
  vAddStep("Op.stageName@"_Stz,       &Step::vOpStageNameFetch);
  
  
  vAddStep("Op[].enqueOperand"_Stz,    &Step::vOpEnqueOperand);
  vAddStep("Op[]"_Stz,                 &Step::vOpGet);
  vAddStep("Op[].handlerEntry"_Stz,    &Step::vOpHandlerEntry);
  vAddStep("Op[].handlerExit"_Stz,     &Step::vOpHandlerExit);
  vAddStep("Op[].handler@"_Stz,        &Step::vOpHandlerFetch);
  vAddStep("Op[].handler!"_Stz,        &Step::vOpHandlerStore);
  vAddStep("Op[].handlerAll!"_Stz,     &Step::vOpHandlerAllStore);
  vAddStep("Op[].init"_Stz,            &Step::vOpInit);
  vAddStep("Op[].isOperandValid"_Stz,  &Step::vOpIsOperandValid);
  vAddStep("Op[].incHandlerCount"_Stz, &Step::vOpIncHandlerCount);
  vAddStep("Op[].incOperandCount"_Stz, &Step::vOpIncOperandCount);
  vAddStep("Op[].ptnId@"_Stz,          &Step::vOpPtnIdFetch);
  vAddStep("Op[].stageNm@"_Stz,        &Step::vOpStageNmFetch);
  vAddStep("Op[].tagSet@"_Stz,         &Step::vOpTagSetFetch);

}

/*----------------------------------------------------------------------------*/
/* Op.ackPipelineJob -- ack the pipeline job                                  */
/*----------------------------------------------------------------------------*/
void Step::vOpAckPipelineJob(void)
{
  ENTRY();
  
  // revised job/op model
    Op* pOp = *m_pOp;
  
  
  pOp->vAckPipelineJob();
}

/*----------------------------------------------------------------------------*/
/* Op.downstreamOp! -- store downstream op                                    */
/*----------------------------------------------------------------------------*/
void Step::vOpDownstreamOpStore(void)
{
  ENTRY();
    
  // revised job/op model
    Op* pOp = *m_pOp;

  Op* pDownstreamOp = _pOp(m_pDataStack->pPopPtr(__func__));
  
  pOp->vDownstreamOp(pDownstreamOp);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op.findOp -- find downstream op by name                                    */
/*----------------------------------------------------------------------------*/
void Step::vOpFindOp(void)
{
  ENTRY();

  RASSERT(0);
//  s32 sOpName;
//  m_pDataStack->vPopStr(sOpName,__func__);
//
//  Op* pDownstreamOp = nullptr;
//
//  Op* pOp = m_pJob->m_pOp;
//
//  for (Op* pScanOp = pOp->pNextOp(); pScanOp; pScanOp = pScanOp->pNextOp()) {
//    if (pScanOp->szOpNm()->bSame(sOpName)) {
//      pDownstreamOp = pScanOp;
//      break;
//    }
//  }
//
//  m_pDataStack->vPushPtr(pDownstreamOp);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op.opName@ -- fetch operator name                                          */
/*----------------------------------------------------------------------------*/
void Step::vOpOpNameFetch(void)
{
  ENTRY();
  
  // revised job/op model
    Op* pOp = *m_pOp;
  
  s256 sOpName = pOp->szOpNm();
  
  m_pDataStack->vPushStr(sOpName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op.partition@ -- fetch partition from op                                   */
/*----------------------------------------------------------------------------*/
void Step::vOpPartitionFetch(void)
{
  ENTRY();
  
  // revised job/op model
    Op* pOp = *m_pOp;

    m_pDataStack->vPushInt(pOp->tPtnId().iAsI8());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op.pipelineJob@ -- fetch pipeline job from op                              */
/*----------------------------------------------------------------------------*/
void Step::vOpPiplineJobFetch(void)
{
  ENTRY();
  
  // revised job/op model
    Op* pOp = *m_pOp;
  
  Pipeline* pPipeline = pOp->pPipeline_op();
  
  if (! pPipeline) {
    m_pDataStack->vPushPtr(nullptr);
  }
  else {
    Job* pPipelineJob = pPipeline->pPipelineJob();
    m_pDataStack->vPushPtr(pPipelineJob);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* op.pipeBackin -- append operand back onto same operator in pipeline       */
/*----------------------------------------------------------------------------*/
void Step::vOpPipeBackIn(void)
{
  ENTRY();

  // revised job/op model
    Op* pOp = *m_pOp;

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  pOp->vPipeToThis(pOperand,this);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* op.pipeout -- append operand onto next operator in pipeline               */
/*----------------------------------------------------------------------------*/
void Step::vOpPipeOut(void)
{
  ENTRY();
  
  // revised job/op model
  Op* pCurrOp = *m_pOp;

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

//  if (pOperand->bIsBuffer()) {
//    LogicUnit* pLogicUnit = pOp->pOpLogicUnit();
//    pLogicUnit->m_xpStats->m_nBuffOut++;
//  }
//  pOp->vPipeToNextOp(pOperand,this);

  AM::OP::vPipeToNextOp(pCurrOp,pOperand,this);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op.outboundOperand@ -- fetch outbound operand from op                      */
/*----------------------------------------------------------------------------*/
void Step::vOpOutboundOperandFetch(void)
{
  ENTRY();
  
  // revised job/op model
    Op* pOp = *m_pOp;
  
  m_pDataStack->vPushPtr(pOp->pOutboundOperand());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op.outboundOperand! -- store outbound operand into op                      */
/*----------------------------------------------------------------------------*/
void Step::vOpOutboundOperandStore(void)
{
  ENTRY();
  
  // revised job/op model
    Op* pOp = *m_pOp;
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  pOp->vOutboundOperand(pOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op.queueLength@ -- get queue length                                        */
/*----------------------------------------------------------------------------*/
void Step::vOpQueueLength(void)
{
  ENTRY();
  
  // revised job/op model
    Op* pOp = *m_pOp;
  
  i8 nLength = pOp->nQueuedOperands();

  m_pDataStack->vPushInt(nLength);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op.stageName@ -- fetch operator stage name                                 */
/*----------------------------------------------------------------------------*/
void Step::vOpStageNameFetch(void)
{
  ENTRY();
  
  // revised job/op model
    Op* pOp = *m_pOp;
  
  s256 sStageName = pOp->szStageName();
  
  m_pDataStack->vPushStr(sStageName);
  
  EXIT();
}






/*----------------------------------------------------------------------------*/
/* Op[].enqueOperand -- enqueue operand                                       */
/*----------------------------------------------------------------------------*/
void Step::vOpEnqueOperand(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  Op* pOp = (* m_p_TaskLogicUnit->m_xpOp);

  pOp->vAppendLeft(pOperand, this);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].get -- get op                                                         */
/*----------------------------------------------------------------------------*/
void Step::vOpGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpOp);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].handlerEntry -- handler entry                                         */
/*----------------------------------------------------------------------------*/
void Step::vOpHandlerEntry(void)
{
  ENTRY();
  
  if (AM::TRACE::bVBlong()) {

  // revised job/op model
    Op* pOp = *m_pOp;
    
    if (m_p_TaskLogicUnit->m_xpPipein.bIsNotNull()) {
      Operand* pOperand = *m_p_TaskLogicUnit->m_xpPipein;
    
      pOp->vOpTrace(pOperand,(Stz*)__func__,true);
    }
    
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].handlerExit -- handler exit                                           */
/*----------------------------------------------------------------------------*/
void Step::vOpHandlerExit(void)
{
  ENTRY();
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].handler@ -- get this op's handler for given operand                   */
/*----------------------------------------------------------------------------*/
void Step::vOpHandlerFetch(void)
{
  ENTRY();

  i8 i_OperandKind = m_pDataStack->iPopInt(__func__);

  Word* pHandler = m_p_TaskLogicUnit->m_xpOp->pEventHandler(i_OperandKind);
    
  m_pDataStack->vPushPtr(pHandler);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].handler! -- set this op's handler for given operand                   */
/*----------------------------------------------------------------------------*/
void Step::vOpHandlerStore(void)
{
  ENTRY();

  i8 i_OpKind = m_pDataStack->iPopInt(__func__);
  
  Word* pHandler = _pWord(m_pDataStack->pPopPtr(__func__));

  m_p_TaskLogicUnit->m_xpOp->vEventHandler(i_OpKind,pHandler);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].handlerAll! -- set this op's handler for given operand                */
/*----------------------------------------------------------------------------*/
void Step::vOpHandlerAllStore(void)
{
  ENTRY();

  Word* pHandler = _pWord(m_pDataStack->pPopPtr(__func__));

  for (i8 i_OpKind = 0; i_OpKind < ZK_ENUMS; i_OpKind++)
    m_p_TaskLogicUnit->m_xpOp->vEventHandler(i_OpKind,pHandler);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].init -- init op                                                       */
/*----------------------------------------------------------------------------*/
void Step::vOpInit(void)
{
  ENTRY();

  // revised job/op model
  Op* pOp = *m_pOp;

  m_p_TaskLogicUnit->m_xpOp = pOp;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].isOperandValid -- check if operand is valid                           */
/*----------------------------------------------------------------------------*/
void Step::vOpIsOperandValid(void)
{
  ENTRY();
  
  bool bValid = true;
  
  i8 i_OperandKind = m_pDataStack->iPopInt(__func__);
  
  // skip consumeOp operand validation
  if ( ! m_p_TaskLogicUnit->m_xpOp->szOpNm()->bSame("consumeOP"_Stz))
    bValid = m_p_TaskLogicUnit->m_xpOp->bIsOperandValid(i_OperandKind);
  
  if ( ! bValid )
    RASSERT(0);
  
  m_pDataStack->vPushInt(bValid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].incHandlerCount -- inc handler count                                  */
/*----------------------------------------------------------------------------*/
void Step::vOpIncHandlerCount(void)
{
  ENTRY();
  
  eZK eEvent = (eZK)m_pDataStack->iPopInt(__func__);
  
  m_p_TaskLogicUnit->m_xpOp->vIncHandlerCount(eEvent);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].incOperandCount -- inc operand count                                  */
/*----------------------------------------------------------------------------*/
void Step::vOpIncOperandCount(void)
{
  ENTRY();
  
  eZK eEvent = (eZK) m_pDataStack->iPopInt(__func__);
  
  m_p_TaskLogicUnit->m_xpOp->vIncOperandCount(eEvent);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].ptnId@ -- get the partition id of this op                             */
/*----------------------------------------------------------------------------*/
void Step::vOpPtnIdFetch(void)
{
  ENTRY();

  Op* pOp = _pOp(*m_p_TaskLogicUnit->m_xpOp);
  PtnId tPtn = pOp->tPtnId();

  m_pDataStack->vPushInt(tPtn.uPtnNo());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].stageNm@ --fetch stageNm of op                                        */
/*----------------------------------------------------------------------------*/
void Step::vOpStageNmFetch(void)
{
  ENTRY();

  Op* pOp = *m_pOp;

  s256 sStageNm;

  sStageNm = pOp->pStream()->pStage()->szStageNm();
  m_pDataStack->vPushStr(sStageNm);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].tagSet@ -- push ptr to op TagSet                                       */
/*----------------------------------------------------------------------------*/
void Step::vOpTagSetFetch(void)
{
  ENTRY();
  
  Op* pOp = *m_pOp;

  m_pDataStack->vPushPtr(pOp->pOpTags());
  
  EXIT();
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
