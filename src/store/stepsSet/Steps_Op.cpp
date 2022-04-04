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
#include "Marker.hpp"

#include "Job.hpp"
#include "LogicUnit.hpp"
#include "Stream.hpp"
#include "Stage.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Op(void)
{
  vAddStep("Op.enqueOperand"_Stz,       &Step::vOpEnqueOperand);
  vAddStep("Op::findOp"_Stz,            &Step::vOpFindOp);
  
  vAddStep("[op]"_Stz,                  &Step::vOpGet);
  vAddStep("[op].ackInvokingTask"_Stz,  &Step::vOpAckInvokingTask);
  vAddStep("[op].handlerEntry"_Stz,     &Step::vOpHandlerEntry);
  vAddStep("[op].handlerExit"_Stz,      &Step::vOpHandlerExit);
  vAddStep("[op].handler@"_Stz,         &Step::vOpHandlerFetch);
  vAddStep("[op].handler!"_Stz,         &Step::vOpHandlerStore);
  vAddStep("[op].handlerAll!"_Stz,      &Step::vOpHandlerAllStore);
  vAddStep("[op].isOperandValid"_Stz,   &Step::vOpIsOperandValid);
  vAddStep("[op].incHandlerCount"_Stz,  &Step::vOpIncHandlerCount);
  vAddStep("[op].incOperandCount"_Stz,  &Step::vOpIncOperandCount);
  vAddStep("[op].job@"_Stz,             &Step::vOpJobFetch);
  vAddStep("[op].opName@"_Stz,          &Step::vOpOpNameFetch);
  vAddStep("[op].partition@"_Stz,       &Step::vOpPartitionFetch);
  vAddStep("[op].pipeBackin"_Stz,       &Step::vOpPipeBackIn);
  vAddStep("[op].pipeout"_Stz,          &Step::vOpPipeOut);
  vAddStep("[op].ptnId@"_Stz,           &Step::vOpPtnIdFetch);
  vAddStep("[op].queueLength@"_Stz,     &Step::vOpQueueLength);
  vAddStep("[op].stageName@"_Stz,       &Step::vOpStageNameFetch);
  vAddStep("[op].stageNm@"_Stz,         &Step::vOpStageNmFetch);
  vAddStep("[op].tagSet@"_Stz,          &Step::vOpTagSetFetch);
}

/*----------------------------------------------------------------------------*/
/* [op].ackInvokingTask -- ack the invoking task                              */
/*----------------------------------------------------------------------------*/
void Step::vOpAckInvokingTask(void)
{
  ENTRY();
  
  m_pOp->vAckInvokingTask();
}

/*----------------------------------------------------------------------------*/
/* Op.enqueOperand -- enqueue operand                                         */
/*----------------------------------------------------------------------------*/
void Step::vOpEnqueOperand(void)
{
  ENTRY();
  
  Op* pOp = _pOp(m_pDataStack->pPopPtr(__func__));

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  pOp->vOpTrace(nullptr,(Stz*)__func__,false,
                 "// queue StartMarker into pipeline"_Stz);

  pOp->vAppendLeft(pOperand,this);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op::findOp -- find downstream op by name                                   */
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
/* [op].get -- get op                                                         */
/*----------------------------------------------------------------------------*/
void Step::vOpGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(m_pOp);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].handlerEntry -- handler entry                                         */
/*----------------------------------------------------------------------------*/
void Step::vOpHandlerEntry(void)
{
  ENTRY();
  
  if (AM::TRACE::bVBlong()) {
    
    if (m_pLogicUnit->m_xpPipein.bIsNotNull()) {
      Operand* pOperand = *m_pLogicUnit->m_xpPipein;
    
      m_pOp->vOpTrace(pOperand,(Stz*)__func__,true);
    }
    
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].handlerExit -- handler exit                                           */
/*----------------------------------------------------------------------------*/
void Step::vOpHandlerExit(void)
{
  ENTRY();
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].handler@ -- get this op's handler for given operand                   */
/*----------------------------------------------------------------------------*/
void Step::vOpHandlerFetch(void)
{
  ENTRY();

  i8 i_OperandKind = m_pDataStack->iPopInt(__func__);

  Word* pHandler = m_pOp->pEventHandler(i_OperandKind);
    
  m_pDataStack->vPushPtr(pHandler);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].handler! -- set this op's handler for given operand                   */
/*----------------------------------------------------------------------------*/
void Step::vOpHandlerStore(void)
{
  ENTRY();

  i8 i_OpKind = m_pDataStack->iPopInt(__func__);
  
  Word* pHandler = _pWord(m_pDataStack->pPopPtr(__func__));

  m_pOp->vEventHandler(i_OpKind,pHandler);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].handlerAll! -- set this op's handler for given operand                */
/*----------------------------------------------------------------------------*/
void Step::vOpHandlerAllStore(void)
{
  ENTRY();

  Word* pHandler = _pWord(m_pDataStack->pPopPtr(__func__));

  for (i8 i_OpKind = 0; i_OpKind < ZK_ENUMS; i_OpKind++)
    m_pOp->vEventHandler(i_OpKind,pHandler);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].isOperandValid -- check if operand is valid                           */
/*----------------------------------------------------------------------------*/
void Step::vOpIsOperandValid(void)
{
  ENTRY();
  
  bool bValid = true;
  
  i8 i_OperandKind = m_pDataStack->iPopInt(__func__);
  
  // skip consumeOp operand validation
  if ( ! m_pOp->szOpNm()->bSame("consumeOP"_Stz))
    bValid = m_pOp->bIsOperandValid(i_OperandKind);
  
  if ( ! bValid )
    RASSERT(0);
  
  m_pDataStack->vPushInt(bValid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].incHandlerCount -- inc handler count                                  */
/*----------------------------------------------------------------------------*/
void Step::vOpIncHandlerCount(void)
{
  ENTRY();
  
  eZK eEvent = (eZK)m_pDataStack->iPopInt(__func__);
  
  m_pOp->vIncHandlerCount(eEvent);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].incOperandCount -- inc operand count                                  */
/*----------------------------------------------------------------------------*/
void Step::vOpIncOperandCount(void)
{
  ENTRY();
  
  eZK eEvent = (eZK) m_pDataStack->iPopInt(__func__);
  
  m_pOp->vIncOperandCount(eEvent);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].job@ -- fetch pipeline job from op                                    */
/*----------------------------------------------------------------------------*/
void Step::vOpJobFetch(void)
{
  ENTRY();
    
  Pipeline* pPipeline = m_pOp->pPipeline();
  
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
/* [op].opName@ -- fetch operator name                                        */
/*----------------------------------------------------------------------------*/
void Step::vOpOpNameFetch(void)
{
  ENTRY();
    
  s256 sOpName = m_pOp->szOpNm();
  
  m_pDataStack->vPushStr(sOpName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].partition@ -- fetch partition from op                                 */
/*----------------------------------------------------------------------------*/
void Step::vOpPartitionFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pOp->tPtnId().iAsI8());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].pipeBackin -- append operand back onto same operator in pipeline      */
/*----------------------------------------------------------------------------*/
void Step::vOpPipeBackIn(void)
{
  ENTRY();

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  m_pOp->vPipeToThis(pOperand,this);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].pipeout -- append operand onto next operator in pipeline              */
/*----------------------------------------------------------------------------*/
void Step::vOpPipeOut(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

//  if (pOperand->bIsBuffer()) {
//    LogicUnit* pLogicUnit = pOp->pOpLogicUnit();
//    pLogicUnit->m_xpStats->m_nBuffOut++;
//  }
//  pOp->vPipeToNextOp(pOperand,this);

  AM::OP::vPipeToNextOp(m_pOp,pOperand,this);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].ptnId@ -- get the partition id of this op                             */
/*----------------------------------------------------------------------------*/
void Step::vOpPtnIdFetch(void)
{
  ENTRY();

  PtnId tPtn = m_pOp->tPtnId();

  m_pDataStack->vPushInt(tPtn.uPtnNo());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].queueLength@ -- get queue length                                      */
/*----------------------------------------------------------------------------*/
void Step::vOpQueueLength(void)
{
  ENTRY();
    
  i8 nLength = m_pOp->nQueuedOperands();

  m_pDataStack->vPushInt(nLength);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].stageName@ -- fetch operator stage name                               */
/*----------------------------------------------------------------------------*/
void Step::vOpStageNameFetch(void)
{
  ENTRY();
  
  s256 sStageName = m_pOp->szStageName();
  
  m_pDataStack->vPushStr(sStageName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].stageNm@ --fetch stageNm of op                                        */
/*----------------------------------------------------------------------------*/
void Step::vOpStageNmFetch(void)
{
  ENTRY();

  s256 sStageNm;

  sStageNm = m_pOp->pStream()->pStage()->szStageNm();
  m_pDataStack->vPushStr(sStageNm);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].tagSet@ -- push ptr to op TagSet                                       */
/*----------------------------------------------------------------------------*/
void Step::vOpTagSetFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(m_pOp->pOpTags());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
