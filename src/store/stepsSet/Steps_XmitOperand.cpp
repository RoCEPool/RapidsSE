/*============================================================================*/
/* Steps_JobXmit.cpp - job xmit                                               */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "XmitOperand.hpp"

#include "Op.hpp"
#include "Stage.hpp"
#include "Pipeline.hpp"
#include "Job.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_XmitOperand(void)
{
  vAddStep("XmitOperand.operand!"_Stz,   &Step::vXmitOperandOperandStore);
  vAddStep("XmitOperand.pipeOut"_Stz,    &Step::vXmitOperandPipeOut);
}

/*----------------------------------------------------------------------------*/
/* XmitOperand.operand! -- store operand in the xmitOperand                   */
/*----------------------------------------------------------------------------*/
void Step::vXmitOperandOperandStore(void)
{
  ENTRY();
  
  XmitOperand* pXmitOperand = _pXmitOperand(m_pDataStack->pPopPtr(__func__));
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  pXmitOperand->m_pOperand = pOperand;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* XmitOperand.pipeOut -- pipe out the operand                                */
/*----------------------------------------------------------------------------*/
void Step::vXmitOperandPipeOut(void)
{
  ENTRY();
  
  XmitOperand* pXmitOperand = _pXmitOperand(m_pDataStack->pPopPtr(__func__));
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  // find the pipeline participant job
  i8 iPipelineJob = pXmitOperand->m_iPipelineJobId;
  Job* pPipelineJob = AM::pJob_FindParticipating(iPipelineJob);

  // get the next stage the xmit is supposed to xmit to
  Stage* pStage = pPipelineJob->m_pPipeline->pGet(pXmitOperand->m_iNextStage);
  
  // get the op to forward to depending on the cross topology
  Op* pOp = pStage->pPipeOutOp(pXmitOperand->m_eCrossTopo,
                               pXmitOperand->m_i_Node,
                               pXmitOperand->m_tPtnNo,
                               pXmitOperand->m_iNextOp);
  
  //! ALKA come back to this. May need a m_bAppendLeft in pXmitOperand
  //vTrace("Step::vXmitOperandPipeOut pXmitOperand->m_i_Node:%lld   pXmitOperand->m_tPtnNo:%lld\n"_Stz, pXmitOperand->m_i_Node, pXmitOperand->m_tPtnNo);
  // append to left queue
  if (pXmitOperand->m_pOperand)
    pOp->vAppendLeft(pOperand,this);
  
  // now delete the pXmitOperand
  delete pXmitOperand;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
