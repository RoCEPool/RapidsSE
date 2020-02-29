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
  vAddStep("Loader[].get"_Stz,            &Step::vLoaderGet);
  vAddStep("Loader[].handleDangling"_Stz, &Step::vLoaderHandleDangling);
  vAddStep("Loader[].set"_Stz,            &Step::vLoaderset);
}

/*----------------------------------------------------------------------------*/
/* Loader[].create -- create the Loader                                       */
/*----------------------------------------------------------------------------*/
void Step::vLoaderCreate(void)
{
  ENTRY(); // ENTRY("vLoaderGet"_Stz);
  
  Loader* pLoader = new Loader(*m_pRegSet->m_xpTable);
  
  m_pRegSet->m_xpLoader = pLoader;
}

/*----------------------------------------------------------------------------*/
/* Loader[].crwBuffer@ -- get crwbuffer for given partition from the loader   */
/*----------------------------------------------------------------------------*/
void Step::vLoaderCrwBufferFetch(void)
{
  ENTRY(); // ENTRY("vLoaderCrwBufferFetch"_Stz);
  
  i8 iPartition = m_pDataStack->iPopInt(__func__);
  Loader* pLoader = *m_pRegSet->m_xpLoader;
  
  m_pDataStack->vPushPtr(pLoader->pCrwBuffer(iPartition));
}

/*----------------------------------------------------------------------------*/
/* Loader[].crwBuffer! -- set crwbuffer for given partition from the loader   */
/*----------------------------------------------------------------------------*/
void Step::vLoaderCrwBufferStore(void)
{
  ENTRY(); // ENTRY("vLoaderCrwBufferStore"_Stz);
  
  i8 iPartition = m_pDataStack->iPopInt(__func__);
  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));
  Loader* pLoader = *m_pRegSet->m_xpLoader;
  
  pLoader->vCrwBuffer(iPartition, pCrwBuffer);
}

/*----------------------------------------------------------------------------*/
/* Loader[].delete -- delete the Loader                                       */
/*----------------------------------------------------------------------------*/
void Step::vLoaderDelete(void)
{
  ENTRY(); // ENTRY("vLoaderGet"_Stz);
  
  Loader* pLoader = *m_pRegSet->m_xpLoader;
  delete pLoader;
  
  m_pRegSet->m_xpLoader = nullptr;
}

/*----------------------------------------------------------------------------*/
/* Loader[].get -- get the Loader                                             */
/*----------------------------------------------------------------------------*/
void Step::vLoaderGet(void)
{
  ENTRY(); // ENTRY("vLoaderGet"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpLoader);
}

/*----------------------------------------------------------------------------*/
/* Loader[].handleDangling -- pipeout the Loader dangling operands            */
/*----------------------------------------------------------------------------*/
void Step::vLoaderHandleDangling(void)
{
  ENTRY(); // ENTRY("vLoaderHandleDangling"_Stz);
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  Loader* pLoader = *m_pRegSet->m_xpLoader;
  
  for (i8 i_Ptn = 0; i_Ptn < AM::nPartitions(); i_Ptn++) {
    
    CrwBuffer* pCrwBuffer = pLoader->pCrwBuffer(i_Ptn);
    
    if (pCrwBuffer) {
      // close the append
      pCrwBuffer->vAppendClose();
      pOp->vPipeOut(pCrwBuffer);
    }
    pLoader->vCrwBuffer(i_Ptn, nullptr);
    
  }
}

/*----------------------------------------------------------------------------*/
/* Loader[].set -- set the Loader                                             */
/*----------------------------------------------------------------------------*/
void Step::vLoaderset(void)
{
  ENTRY(); // ENTRY("vLoaderset"_Stz);
  
  m_pRegSet->m_xpLoader = _pLoader(m_pDataStack->pPopPtr(__func__));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
