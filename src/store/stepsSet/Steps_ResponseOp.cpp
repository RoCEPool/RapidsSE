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
  vAddStep("[ResponseOp]"_Stz,           &Step::vResponseOpGet);
  vAddStep("[ResponseOp].isNull"_Stz,    &Step::vResponseOpIsNull);
  vAddStep("[ResponseOp].isNotNull"_Stz, &Step::vResponseOpIsNotNull);
  vAddStep("[ResponseOp].pipeout"_Stz,   &Step::vResponseOpPipeout);
}

/*----------------------------------------------------------------------------*/
/* [ResponseOp].get -- get the p_responseOp of this job                       */
/*----------------------------------------------------------------------------*/
void Step::vResponseOpGet(void)
{
  ENTRY();
  
  Job* pJob = this->pJob();
  
  Op* p_ResponseOp = pJob->p_ResponseOp();

  m_oDataStack.vPushPtr(p_ResponseOp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ResponseOp].isNotNull -- return boolean                                      */
/*----------------------------------------------------------------------------*/
void Step::vResponseOpIsNotNull(void)
{
  ENTRY();

  Job* pJob = this->pJob();

  Op* p_ResponseOp = pJob->p_ResponseOp();

  m_oDataStack.vPushBol(p_ResponseOp != nullptr);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ResponseOp].isNull -- return boolean                                      */
/*----------------------------------------------------------------------------*/
void Step::vResponseOpIsNull(void)
{
  ENTRY();

  Job* pJob = this->pJob();

  Op* p_ResponseOp = pJob->p_ResponseOp();

  m_oDataStack.vPushBol(p_ResponseOp == nullptr);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ResponseOp].pipeout -- pipeout to p_responseOp of this job                */
/*----------------------------------------------------------------------------*/
void Step::vResponseOpPipeout(void)
{
  ENTRY();

  Operand* pOperand = _pOperand(m_oDataStack.pPopPtr(__func__));

  Job* pJob = this->pJob();
  
  Op* p_ResponseOp = pJob->p_ResponseOp();

  p_ResponseOp->vAppendLeft(pOperand,this);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
