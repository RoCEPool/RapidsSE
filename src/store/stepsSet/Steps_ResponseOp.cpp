/*============================================================================*/
/* Steps_RespondToOp.cpp -- Operator to which responses are directed stepGroup*/
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Op.hpp"
#include "Job.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ResponseOp(void)
{
  vAddStep("[ResponseOp]"_Stz,          &Step::vResponseOpGet);
  vAddStep("[ResponseOp].pipeout"_Stz,  &Step::vResponseOpPipeout);
}

/*----------------------------------------------------------------------------*/
/* [ResponseOp].get -- get the p_responseOp of this job                       */
/*----------------------------------------------------------------------------*/
void Step::vResponseOpGet(void)
{
  ENTRY();
  
  Job* pJob = this->pJob();
  
  Op* p_ResponseOp = pJob->p_ResponseOp();

  m_pDataStack->vPushPtr(p_ResponseOp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ResponseOp].pipeout -- pipeout to p_responseOp of this job                */
/*----------------------------------------------------------------------------*/
void Step::vResponseOpPipeout(void)
{
  ENTRY();

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  Job* pJob = this->pJob();
  
  Op* p_ResponseOp = pJob->p_ResponseOp();

  p_ResponseOp->vAppendLeft(pOperand,this);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
