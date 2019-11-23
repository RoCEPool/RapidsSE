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
  vAddStep("JobRef[].none"_Stz,    &Step::vJobRefNone);
  vAddStep("JobRef[].return"_Stz,  &Step::vJobRefReturn);
}

/*----------------------------------------------------------------------------*/
/* JobRef[] -- return pointer to the referenced job                           */
/*----------------------------------------------------------------------------*/
void Step::vJobRefPointer(void)
{
  ENTRY(); // ENTRY("vJobRefPointer"_Stz);
  
  Job* pJob = m_pRegSet->m_xpJobRef.pReferencedObject();

  m_pDataStack->vPushPointer(pJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* JobRef[].none -- check there is no job reference                           */
/*----------------------------------------------------------------------------*/
void Step::vJobRefNone(void)
{
  ENTRY(); // ENTRY("vJobRefNone"_Stz);
  
  if (m_pRegSet->m_xpJobRef.pReferencedObject())
    THROW("JobRef[].none : register still has reference to a job"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* JobRef[].return -- return the job reference                                */
/*----------------------------------------------------------------------------*/
void Step::vJobRefReturn(void)
{
  ENTRY(); // ENTRY("vJobRefReturn"_Stz);
  
  if (m_pRegSet->m_xpJobRef.bHasReference())
    m_pRegSet->m_xpJobRef.vReturnRef();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
