/*============================================================================*/
/* Steps_JobQuery.cpp - jobQuery step group                                   */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobQuery(void)
{
  vAddStep("Job_Query.begin"_Stz,    &Step::vJob_QueryBegin);
  vAddStep("Job_Query.end"_Stz,      &Step::vJob_QueryEnd);
  vAddStep("Job_Query.execute"_Stz,  &Step::vJob_QueryExecute);
}

/*----------------------------------------------------------------------------*/
/* Job_Query.begin -- begin query                                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_QueryBegin(MP_Task mpTask)
{
  ENTRY("vJob_QueryBegin"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_Query.begin"_Stz);
  Job_Query* pJob_Query = _pJob_Query(pJobPtr);
  
  pJob_Query->vBegin();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Query.end -- end query                                                 */
/*----------------------------------------------------------------------------*/
void Step::vJob_QueryEnd(MP_Task mpTask)
{
  ENTRY("vJob_QueryEnd"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_Query.end"_Stz);
  Job_Query* pJob_Query = _pJob_Query(pJobPtr);

  pJob_Query->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Query.execute -- execute query                                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_QueryExecute(MP_Task mpTask)
{
  ENTRY("vJob_QueryExecute"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_Query.execute"_Stz);
  Job_Query* pJob_Query = _pJob_Query(pJobPtr);
  
  pJob_Query->vExecute();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
