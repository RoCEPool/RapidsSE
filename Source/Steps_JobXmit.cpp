/*============================================================================*/
/* Steps_JobXmit.cpp - job xmit                                               */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobXmit(void)
{
  vAddStep("Job_Xmit.parentJobId@"_Stz,  &Step::vJob_XmitParentJobIdFetch);
  vAddStep("Job_Xmit.node@"_Stz,         &Step::vJob_XmitNodeFetch);
}

/*----------------------------------------------------------------------------*/
/* Job_Xmit.parentJobId@ -- fetch parent jobid from job                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_XmitParentJobIdFetch(MP_Task mpTask)
{
  ENTRY("vJob_XmitParentJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_Xmit.parentJobId@"_Stz);
  Job_Xmit* pJob_Xmit = _pJob_Xmit(pJobPtr);
  
  i8 iParentJobId = pJob_Xmit->m_iParentJobId;
  RASSERT(C_UNKNOWN != iParentJobId);
  
  mpTask->m_pDataStack->vPushI8(iParentJobId);
  
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
