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
#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Op(void)
{
  vAddStep("op.enqueOperand"_Stz,       &Step::vOpEnqueOperand);
  vAddStep("op.name@"_Stz,              &Step::vOpNameFetch);

  vAddStep("Op::findOp"_Stz,            &Step::vOpFindOp);
  
  vAddStep("[op]"_Stz,                  &Step::vOpGet);
  vAddStep("[op].ackJobTask"_Stz,       &Step::vOpAckJobTask);
  vAddStep("[op].disableAllocate"_Stz,  &Step::vOpDisableAllocate);
  vAddStep("[op].enableAllocate"_Stz,   &Step::vOpEnableAllocate);
  vAddStep("[op].eventCountInc"_Stz,    &Step::vOpEventCountInc);
  vAddStep("[op].handlerEntry"_Stz,     &Step::vOpHandlerEntry);
  vAddStep("[op].handlerExit"_Stz,      &Step::vOpHandlerExit);
  vAddStep("[op].handler@"_Stz,         &Step::vOpHandlerFetch);
  vAddStep("[op].handler!"_Stz,         &Step::vOpHandlerStore);
  vAddStep("[op].handlerAll!"_Stz,      &Step::vOpHandlerAllStore);
  vAddStep("[op].isEventValid"_Stz,     &Step::vOpIsEventValid);
  vAddStep("[op].isRecv"_Stz,           &Step::vOpIsRecv);
  vAddStep("[op].isSend"_Stz,           &Step::vOpIsSend);
  vAddStep("[op].job@"_Stz,             &Step::vOpJobFetch);
  vAddStep("[op].opName@"_Stz,          &Step::vOpOpNameFetch);
  vAddStep("[op].partition@"_Stz,       &Step::vOpPartitionFetch);
  vAddStep("[op].pipeBackin"_Stz,       &Step::vOpPipeBackIn);
  vAddStep("[op].pipeout"_Stz,          &Step::vOpPipeOut);
	vAddStep("[op].pipeoutPtnTasks"_Stz,  &Step::vOpPipeOutPtnTasks);
  vAddStep("[op].ptnId@"_Stz,           &Step::vOpPtnIdFetch);
  vAddStep("[op].queueLength@"_Stz,     &Step::vOpQueueLength);
  vAddStep("[op].stageName@"_Stz,       &Step::vOpStageNameFetch);
  vAddStep("[op].stageNm@"_Stz,         &Step::vOpStageNmFetch);
  vAddStep("[op].tagSet@"_Stz,          &Step::vOpTagSetFetch);
}

/*----------------------------------------------------------------------------*/
/* [op].ackJobTask -- ack the job task                                        */
/*----------------------------------------------------------------------------*/
void Step::vOpAckJobTask(void)
{
  ENTRY();

	if (bIsSecondary())
		m_pOp->vAckJobTask(this);
	else
    m_pOp->vAckJobTask();
}

/*----------------------------------------------------------------------------*/
/* [op].disableAllocate -- disable memory allocation by this op               */
/*----------------------------------------------------------------------------*/
void Step::vOpDisableAllocate(void)
{
  ENTRY();

  m_pOp->vSetAllocate(false);
}

/*----------------------------------------------------------------------------*/
/* [op].enableAllocate -- enable memory allocation by this op               */
/*----------------------------------------------------------------------------*/
void Step::vOpEnableAllocate(void)
{
  ENTRY();

  m_pOp->vSetAllocate(true);
}

/*----------------------------------------------------------------------------*/
/* op.enqueOperand -- enqueue operand                                         */
/*----------------------------------------------------------------------------*/
void Step::vOpEnqueOperand(void)
{
  ENTRY();
  
  Op* pOp = _pOp(m_pDataStack->pPopPtr(__func__));

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

	if (AM::TRACE::bVerbose(C_VB_OP))
		pOp->vOpTrace(this, nullptr,(Stz*)__func__,false,
                 "// queue StartMarker into pipeline"_Stz);

  pOp->vAppendLeft(pOperand,this);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].eventCountInc -- inc operand count                                    */
/*----------------------------------------------------------------------------*/
void Step::vOpEventCountInc(void)
{
  ENTRY();

  eEV eEvent = (eEV) m_pDataStack->iPopInt(__func__);

	if ( ! bIsSecondary() )
    m_pOp->vEventCountInc(eEvent);
  
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

			if (AM::TRACE::bVerbose(C_VB_OP))
        m_pOp->vOpTrace(this, pOperand,(Stz*)__func__,true);
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

  for (i8 i_OpKind = 0; i_OpKind < EV_ENUMS; i_OpKind++)
    m_pOp->vEventHandler(i_OpKind,pHandler);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].isEventValid -- check if operand is valid                             */
/*----------------------------------------------------------------------------*/
void Step::vOpIsEventValid(void)
{
  ENTRY();
  
  bool bValid = true;
  
  i8 i_OperandKind = m_pDataStack->iPopInt(__func__);

	i8 i_BufferType = m_pDataStack->iPopInt(__func__);

  //vTrace("vOpIsEventValid:   %s\n"_Stz, m_pOp->szOpNm());
  // skip consumeOp operand validation
  if ( ! m_pOp->szOpNm()->bSame("consumeOP"_Stz) &&
	     ! m_pOp->szOpNm()->bSame("ptnConsume"_Stz) &&
			 ! bIsSecondary() )
    bValid = m_pOp->bIsEventValid(i_OperandKind, i_BufferType);

  if ( ! bValid ) {
    vTrace("Steps_Op  vOpIsEventValid:   %s\n"_Stz, m_pOp->szOpNm());
    RASSERT(0);
  }

	m_pDataStack->vPushInt(bValid);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].isRecv -- check if operand is the recv OP                             */
/*----------------------------------------------------------------------------*/
void Step::vOpIsRecv(void)
{
  ENTRY();

  if ( m_pOp->szOpNm()->bSame("recvSE"_Stz))
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].isSend -- check if operand is the send OP                             */
/*----------------------------------------------------------------------------*/
void Step::vOpIsSend(void)
{
  ENTRY();

  if ( m_pOp->szOpNm()->bSame("sendSE"_Stz))
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);

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
/* op.name@ -- name fetch                                                     */
/*----------------------------------------------------------------------------*/
void Step::vOpNameFetch(void)
{
  ENTRY();
  
  Op* pOp = _pOp(m_pDataStack->pPopPtr(__func__));

  Stz* szOpNm = pOp->szOpNm();

  m_pDataStack->vPushStz(szOpNm);

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
  
  m_pDataStack->vPushInt(m_pOp->tLglPtn().iAsI8());

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

  if(! BufferId::bIsBufferId(pOperand)) {
    if(DataBuffer::bIsDataBuffer(pOperand->eGetClassNo())) {
      DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
      AM::pHashMap()->vDropKeyLock(pDataBuffer->oGetBufferId().m_pvAsAddress);
      pOperand = _pOperand((Object*)pDataBuffer->oGetBufferId().m_pvAsAddress);
    }
  }
//  if (pOperand->bIsBuffer()) {
//    LogicUnit* pLogicUnit = pOp->pOpLogicUnit();
//    pLogicUnit->m_xpStats->m_nBuffOut++;
//  }
//  pOp->vPipeToNextOp(pOperand,this);

  AM::OP::vPipeToNextOp(m_pOp,pOperand,this);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].pipeOutPtnTasks -- append operand onto ptn tasks of next operator     */
/*----------------------------------------------------------------------------*/
void Step::vOpPipeOutPtnTasks(void)
{
	ENTRY();

	Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

	//vTrace("[op].pipeoutPtnTasks %p\n"_Stz, pOperand);

	if(! BufferId::bIsBufferId(pOperand)) {
		if(DataBuffer::bIsDataBuffer(pOperand->eGetClassNo())) {
			DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
			AM::pHashMap()->vDropKeyLock(pDataBuffer->oGetBufferId().m_pvAsAddress);
			pOperand = _pOperand((Object*)pDataBuffer->oGetBufferId().m_pvAsAddress);
		}
	}
//  if (pOperand->bIsBuffer()) {
//    LogicUnit* pLogicUnit = pOp->pOpLogicUnit();
//    pLogicUnit->m_xpStats->m_nBuffOut++;
//  }
//  pOp->vPipeToNextOp(pOperand,this);

	AM::OP::vPipeToNextOpTasks(m_pOp,pOperand,this);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* [op].ptnId@ -- get the partition id of this op                             */
/*----------------------------------------------------------------------------*/
void Step::vOpPtnIdFetch(void)
{
  ENTRY();

  LglPtn tPtn = m_pOp->tLglPtn();

  m_pDataStack->vPushInt(tPtn.iAsI8());

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
