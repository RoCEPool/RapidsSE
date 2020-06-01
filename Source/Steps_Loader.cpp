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
  vAddStep("Loader[].fullCrwBuffer@"_Stz, &Step::vLoaderFullCrwBufferFetch);
  vAddStep("Loader[].delete"_Stz,         &Step::vLoaderDelete);
  vAddStep("Loader[].get"_Stz,            &Step::vLoaderGet);
  vAddStep("Loader[].handleDangling"_Stz, &Step::vLoaderHandleDangling);
  vAddStep("Loader[].set"_Stz,            &Step::vLoaderset);
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
  
  m_pDataStack->vPushPtr(pLoader->pCrwBuffer(iPartition));
  
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
  
  pLoader->vCrwBuffer(iPartition, pCrwBuffer);
  
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
  
  CrwBuffer* pCrwBuffer = pLoader->pFullCrwBuffer(iPartition);
  
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

/*----------------------------------------------------------------------------*/
/* Loader[].get -- get the Loader                                             */
/*----------------------------------------------------------------------------*/
void Step::vLoaderGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpLoader);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Loader[].handleDangling -- pipeout the Loader dangling operands            */
/*----------------------------------------------------------------------------*/
void Step::vLoaderHandleDangling(void)
{
  ENTRY();
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  Loader* pLoader = *m_pRegSet->m_xpLoader;
  
  for (i8 i_Ptn = 0; i_Ptn < AM::nPartitions(); i_Ptn++) {
    
    CrwBuffer* pCrwBuffer = pLoader->pCrwBuffer(i_Ptn);
    
    if (pCrwBuffer) {
      // close the append
      pCrwBuffer->vAppendClose();
      pOp->vPipeOut_nextOp(pCrwBuffer,m_pTask);
    }
    pLoader->vCrwBuffer(i_Ptn, nullptr);
    
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Loader[].set -- set the Loader                                             */
/*----------------------------------------------------------------------------*/
void Step::vLoaderset(void)
{
  ENTRY();
  
  m_pRegSet->m_xpLoader = _pLoader(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
