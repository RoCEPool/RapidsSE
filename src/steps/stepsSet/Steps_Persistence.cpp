/*============================================================================*/
/* Steps_Persistence.cpp - persistence step group                             */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Persistence(void)
{
  vAddStep("persistence::checkpoint"_Stz, &Step::vPersistenceCheckpoint);
  vAddStep("persistence::reload"_Stz,     &Step::vPersistenceReload);
}

/*----------------------------------------------------------------------------*/
/* persistence::checkpoint -- take a checkpoint                               */
/*----------------------------------------------------------------------------*/
void Step::vPersistenceCheckpoint(void)
{
  ENTRY();
  
  s256 sFilePath;
  m_pDataStack->vPopStr(sFilePath,__func__);
  
  vTrace("\nTaking checkpoint into file : '%s'\n"_Stz, sFilePath.azChar());

  BPA* pBPA = new BPA;
  
  PoolHelper::vWritePersistentHeapMem(sFilePath);
  
  delete pBPA;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* persistence::reload -- reload                                              */
/*----------------------------------------------------------------------------*/
void Step::vPersistenceReload(void)
{
  ENTRY();
  
  s256 sFilePath;
  m_pDataStack->vPopStr(sFilePath,__func__);
  
  vTrace("\nReloading from file : '%s'\n"_Stz, sFilePath.azChar());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
