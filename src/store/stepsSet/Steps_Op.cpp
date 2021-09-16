/*============================================================================*/
/* Steps_Op.cpp -- Opeartor step group                                        */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "Op.hpp"
#include "Pipeline.hpp"
#include "StageSpec.hpp"

#include "Job.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Op(void)
{
  vAddStep("Op::pipeBackin"_Stz,          &Step::vOpPipeBackIn);
  vAddStep("Op::pipeout"_Stz,         &Step::vOpPipeOut);

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
  vAddStep("Op.sendKind!"_Stz,        &Step::vOpSendKindStore);
  
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
/* Op::pipeBackIn -- append operand back onto same operator in pipeline       */
/*----------------------------------------------------------------------------*/
void Step::vOpPipeBackIn(void)
{
  ENTRY();

  // revised job/op model
    Op* pOp = *m_pOp;

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  pOp->vPipeToSelf(pOperand,this);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op::pipeout -- append operand onto next operator in pipeline               */
/*----------------------------------------------------------------------------*/
void Step::vOpPipeOut(void)
{
  ENTRY();
  
  // revised job/op model
    Op* pOp = *m_pOp;

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  if (pOperand->bIsBuffer()) {
    LogicUnit* pLogicUnit = pOp->pOpLogicUnit();
    pLogicUnit->m_xpStats->m_nBuffOut++;
  }
  pOp->vPipeToNextOp(pOperand,this);

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
/* Op.SendKind! -- store SendKind in the operator                             */
/*----------------------------------------------------------------------------*/
void Step::vOpSendKindStore(void)
{
  ENTRY();
  
  // revised job/op model
    Op* pOp = *m_pOp;

  pOp->vSendKind((eXK)m_pDataStack->iPopInt(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
