/*============================================================================*/
/* Steps_JobDynamic.cpp - JobDynamic step group                               */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobDynamic(void)
{
  vAddStep("Job_Dynamic.begin"_Stz,    &Step::vJob_DynamicBegin);
  vAddStep("Job_Dynamic.end"_Stz,      &Step::vJob_DynamicEnd);
  vAddStep("Job_Dynamic.execute"_Stz,  &Step::vJob_DynamicExecute);
}

/*----------------------------------------------------------------------------*/
/* Job_Dynamic.begin -- begin the dynamic job                                 */
/*----------------------------------------------------------------------------*/
void Step::vJob_DynamicBegin(Forth* pForth)
{
  ENTRY("vJob_DynamicBegin"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job_Dynamic* pJob_Dynamic = _pJob_Dynamic(pJobPtr);
  
  pJob_Dynamic->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Dynamic.end -- end the dynamic job                                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_DynamicEnd(Forth* pForth)
{
  ENTRY("vJob_DynamicEnd"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job_Dynamic* pJob_Dynamic = _pJob_Dynamic(pJobPtr);
  
  pJob_Dynamic->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Dynamic.execute -- execute                                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_DynamicExecute(Forth* pForth)
{
  ENTRY("vJob_DynamicExecute"_Stz);
  
  Object* pJobPtr;
  if ( ! pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_DSTACK();
  Job_Dynamic* pJob_Dynamic = _pJob_Dynamic(pJobPtr);
  
  pJob_Dynamic->vExecute();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
