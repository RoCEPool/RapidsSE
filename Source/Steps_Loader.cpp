/*============================================================================*/
/* Steps_Loader.cpp -- Loader step group                                      */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Loader(void)
{
  vAddStep("Loader[].create"_Stz,         &Step::vLoaderCreate);
  vAddStep("Loader[].delete"_Stz,         &Step::vLoaderDelete);
}

/*----------------------------------------------------------------------------*/
/* Loader[].create -- create the Loader                                       */
/*----------------------------------------------------------------------------*/
void Step::vLoaderCreate(void)
{
  ENTRY();
  
  Job* pJob = m_pTask->pJob();
  
  TagSet* pJobTags = pJob->pJobTags();
  
  if (AM::bVerbose())
    pJobTags->vContentDump();
  
  Loader* pLoader = new Loader(*m_pRegSet->m_xpTable,*m_pRegSet->m_xpCrwHash);
  bool bResult = pLoader->bSetFormat(pJobTags);
  
  if (bResult) {
    m_pDataStack->vPushInt(true);
    m_pRegSet->m_xpLoader = pLoader;
  }
  else {
    m_pDataStack->vPushInt(false);
    delete pLoader;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Loader[].delete -- delete the Loader                                       */
/*----------------------------------------------------------------------------*/
void Step::vLoaderDelete(void)
{
  ENTRY();
  
  Loader* pLoader = *m_pRegSet->m_xpLoader;
  delete pLoader;
  
  m_pRegSet->m_xpLoader = nullptr;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
