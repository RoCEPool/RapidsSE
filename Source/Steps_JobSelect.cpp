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
void Step::vJob_SelectBegin(void)
{
  ENTRY(); // ENTRY("vJob_SelectBegin"_Stz);
  
  Job_Select* pJob_Select = _pJob_Select(m_pDataStack->pPopPtr(__func__));
  
  pJob_Select->vBegin();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Select.end -- end                                                      */
/*----------------------------------------------------------------------------*/
void Step::vJob_SelectEnd(void)
{
  ENTRY(); // ENTRY("vJob_SelectEnd"_Stz);
  
  Job_Select* pJob_Select = _pJob_Select(m_pDataStack->pPopPtr(__func__));

  pJob_Select->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Select.execute -- execute                                              */
/*----------------------------------------------------------------------------*/
void Step::vJob_SelectExecute(void)
{
  ENTRY(); // ENTRY("vJob_SelectExecute"_Stz);
  
  Job_Select* pJob_Select = _pJob_Select(m_pDataStack->pPopPtr(__func__));
  
  pJob_Select->vExecute();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
