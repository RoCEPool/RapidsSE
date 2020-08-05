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
  vAddStep("JobRef[].references"_Stz,  &Step::vJobRefReferences);
  vAddStep("JobRef[].hasTask"_Stz, &Step::vJobRefHasTask);
}

/*----------------------------------------------------------------------------*/
/* jobRef[].hasTask -- does the jobRef have a task ?                          */
/*----------------------------------------------------------------------------*/
void Step::vJobRefHasTask(void)
{
  ENTRY();
  
  Job* pJob = m_pLogicUnit->m_xpJobRef.pReferencedObject();
  
  m_pDataStack->vPushInt(pJob->pTask() ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* JobRef[] -- return pointer to the referenced job                           */
/*----------------------------------------------------------------------------*/
void Step::vJobRefPointer(void)
{
  ENTRY();
  
  Job* pJob = m_pLogicUnit->m_xpJobRef.pReferencedObject();

  m_pDataStack->vPushPtr(pJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* JobRef[].none -- check there is no job reference                           */
/*----------------------------------------------------------------------------*/
void Step::vJobRefNone(void)
{
  ENTRY();
  
  if (m_pLogicUnit->m_xpJobRef.pReferencedObject())
    THROW("JobRef[].none : register still has reference to a job"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* JobRef[].references -- return the job references                           */
/*----------------------------------------------------------------------------*/
void Step::vJobRefReferences(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpJobRef.nReferences());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* JobRef[].return -- return the job reference                                */
/*----------------------------------------------------------------------------*/
void Step::vJobRefReturn(void)
{
  ENTRY();
  
  if (m_pLogicUnit->m_xpJobRef.bHasReference())
    m_pLogicUnit->m_xpJobRef.vReturnRef();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
