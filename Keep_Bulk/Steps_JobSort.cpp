/*============================================================================*/
/* Steps_JobSort.cpp - jobSort step group                                   */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobSortExp(void)
{
  vAddStep("Job_SortExp.begin"_Stz,    &Step::vJob_SortBegin);
  vAddStep("Job_SortExp.end"_Stz,      &Step::vJob_SortEnd);
  vAddStep("Job_SortExp.execute"_Stz,  &Step::vJob_SortExecute);
}

/*----------------------------------------------------------------------------*/
/* job_SortExp.begin -- begin sort                                            */
/*----------------------------------------------------------------------------*/
void Step::vJob_SortBegin(MP_Task mpTask)
{
  ENTRY("vJob_SortBegin"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_SortExp.begin!"_Stz);
  Job_SortExp* pJob_SortExp = _pJob_SortExp(pJobPtr);
  
  pJob_SortExp->vBegin();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_SortExp.end -- end sort                                                */
/*----------------------------------------------------------------------------*/
void Step::vJob_SortEnd(MP_Task mpTask)
{
  ENTRY("vJob_SortEnd"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_SortExp.end!"_Stz);
  Job_SortExp* pJob_SortExp = _pJob_SortExp(pJobPtr);

  pJob_SortExp->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_SortExp.execute -- execute sort                                        */
/*----------------------------------------------------------------------------*/
void Step::vJob_SortExecute(MP_Task mpTask)
{
  ENTRY("vJob_SortExecute"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_SortExp.execute!"_Stz);
  Job_SortExp* pJob_SortExp = _pJob_SortExp(pJobPtr);
  
  pJob_SortExp->vExecute();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
