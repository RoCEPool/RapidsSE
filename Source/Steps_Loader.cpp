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
  vAddStep("Loader[].crwBuffer@"_Stz,     &Step::vLoaderCrwBufferFetch);
  vAddStep("Loader[].crwBuffer!"_Stz,     &Step::vLoaderCrwBufferStore);
  vAddStep("Loader[].delete"_Stz,         &Step::vLoaderDelete);
  vAddStep("Loader[].fullCrwBuffer@"_Stz, &Step::vLoaderFullCrwBufferFetch);
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
  
  Loader* pLoader = new Loader(*m_pRegSet->m_xpTable);
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
/* Loader[].crwBuffer@ -- get crwbuffer for given partition from the loader   */
/*----------------------------------------------------------------------------*/
void Step::vLoaderCrwBufferFetch(void)
{
  ENTRY();
  
  i8 iPartition = m_pDataStack->iPopInt(__func__);
  Loader* pLoader = *m_pRegSet->m_xpLoader;
  
  WriteHash* pWriteHash = pLoader->pWriteHash();
  
  m_pDataStack->vPushPtr(pWriteHash->pCrwBuffer(iPartition));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Loader[].crwBuffer! -- set crwbuffer for partition from the loader         */
/*----------------------------------------------------------------------------*/
void Step::vLoaderCrwBufferStore(void)
{
  ENTRY();
  
  i8 iPartition = m_pDataStack->iPopInt(__func__);
  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));
  Loader* pLoader = *m_pRegSet->m_xpLoader;
  
  WriteHash* pWriteHash = pLoader->pWriteHash();

  pWriteHash->vCrwBuffer(iPartition, pCrwBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Loader[].FullCrwBuffer@ -- get full crwbuffer for partition from loader    */
/*----------------------------------------------------------------------------*/
void Step::vLoaderFullCrwBufferFetch(void)
{
  ENTRY();
  
  i8 iPartition = m_pDataStack->iPopInt(__func__);
  Loader* pLoader = *m_pRegSet->m_xpLoader;

  WriteHash* pWriteHash = pLoader->pWriteHash();

  CrwBuffer* pCrwBuffer = pWriteHash->pFullCrwBuffer(iPartition);
  
  m_pDataStack->vPushPtr(pCrwBuffer);
  
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
