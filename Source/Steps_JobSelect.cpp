/*============================================================================*/
/* Steps_JobSelect.cpp - JobSelect step group                                 */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobSelect(void)
{
  vAddStep("Job_Select.begin"_Stz,    &Step::vJob_SelectBegin);
  vAddStep("Job_Select.end"_Stz,      &Step::vJob_SelectEnd);
  vAddStep("Job_Select.execute"_Stz,  &Step::vJob_SelectExecute);
}

/*----------------------------------------------------------------------------*/
/* Job_Select.begin -- begin                                                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_SelectBegin(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_SelectBegin"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_Select.begin"_Stz);
  Job_Select* pJob_Select = _pJob_Select(pJobPtr);
  
  pJob_Select->vBegin();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Select.end -- end                                                      */
/*----------------------------------------------------------------------------*/
void Step::vJob_SelectEnd(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_SelectEnd"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_Select.end"_Stz);
  Job_Select* pJob_Select = _pJob_Select(pJobPtr);

  pJob_Select->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Select.execute -- execute                                              */
/*----------------------------------------------------------------------------*/
void Step::vJob_SelectExecute(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_SelectExecute"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_Select.execute"_Stz);
  Job_Select* pJob_Select = _pJob_Select(pJobPtr);
  
  pJob_Select->vExecute();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
