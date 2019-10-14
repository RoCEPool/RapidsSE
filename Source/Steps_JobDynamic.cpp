/*============================================================================*/
/* Steps_JobDynamic.cpp - JobDynamic step group                               */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobDynamic(void)
{
  vAddStep("Job_Dynamic.execute"_Stz,  &Step::vJob_DynamicExecute);
}

/*----------------------------------------------------------------------------*/
/* Job_Dynamic.execute -- execute                                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_DynamicExecute(MP_Task mpTask)
{
  ENTRY("vJob_DynamicExecute"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_Dynamic.execute"_Stz);
  Job_Dynamic* pJob_Dynamic = _pJob_Dynamic(pJobPtr);
  
  pJob_Dynamic->vExecute();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
