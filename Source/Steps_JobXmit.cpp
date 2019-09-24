/*============================================================================*/
/* Steps_JobXmit.cpp - job xmit                                               */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobXmit(void)
{
  vAddStep("Job_Xmit.pipeOut"_Stz,       &Step::vJob_XmitPipeOut);
  vAddStep("Job_Xmit.node@"_Stz,         &Step::vJob_XmitNodeFetch);
}

/*----------------------------------------------------------------------------*/
/* Job_Xmit.pipeOut -- pipe out the operand                                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_XmitPipeOut(MP_Task mpTask)
{
  ENTRY("vJob_XmitPipeOut"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_Xmit.pipeOut@"_Stz);
  Job_Xmit* pJob_Xmit = _pJob_Xmit(pJobPtr);
  
//  Object* pParentJobPtr;
//  if ( ! mpTask->m_pDataStack->bPop(pParentJobPtr) )
//    THROW_INVALID_DSTACK("Job_Xmit.pipeOut@"_Stz);
  
//  Job_Dynamic* pParentJob = _pJob_Dynamic(pParentJobPtr);
  pJob_Xmit->m_pOp->vPipeOut(pJob_Xmit->m_pOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Xmit.node@ -- fetch node no. from Job_Xmit                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_XmitNodeFetch(MP_Task mpTask)
{
  ENTRY("vJob_XmitNodeFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_Xmit.node@"_Stz);
  Job_Xmit* pJob_Xmit = _pJob_Xmit(pJobPtr);
  
  i8 i_Node = pJob_Xmit->m_iNode;
  mpTask->m_pDataStack->vPushI8(i_Node);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
