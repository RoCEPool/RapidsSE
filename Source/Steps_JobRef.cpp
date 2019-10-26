/*============================================================================*/
/* Steps_JobRef.cpp -- job reference                                          */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobRef(void)
{
  vAddStep("JobRef[]"_Stz,         &Step::vJobRefPointer);
  vAddStep("JobRef[].return"_Stz,  &Step::vJobRefReturn);
}

/*----------------------------------------------------------------------------*/
/* JobRef[] -- return pointer to the referenced job                           */
/*----------------------------------------------------------------------------*/
void Step::vJobRefPointer(void)
{
  ENTRY("vJobRefPointer"_Stz);
  
  Job* pJob = m_pTask->m_pRegSet->m_xpJobRef.pReferencedObject();

  m_pDataStack->vPushPointer(pJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* JobRef[].return -- return the job reference                                */
/*----------------------------------------------------------------------------*/
void Step::vJobRefReturn(void)
{
  ENTRY("vJobRefReturn"_Stz);
  
  if (m_pTask->m_pRegSet->m_xpJobRef.bHasReference())
    m_pTask->m_pRegSet->m_xpJobRef.vReturnRef();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
