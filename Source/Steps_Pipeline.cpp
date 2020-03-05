/*============================================================================*/
/* Steps_Pipeline.cpp -- Pipeline step group                                  */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Pipeline(void)
{
  vAddStep("pipeline.pipeout"_Stz,  &Step::vPipelinePipeout);
}

/*----------------------------------------------------------------------------*/
/* Op::pipeout -- append operand onto next operator in pipeline               */
/*----------------------------------------------------------------------------*/
void Step::vPipelinePipeout(void)
{
  ENTRY(); // ENTRY("vOpPipeOut"_Stz);
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  pOp->vPipeOut(pOperand);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
