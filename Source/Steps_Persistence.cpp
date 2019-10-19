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
void Step::vPersistenceCheckpoint(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vPersistenceCheckpoint"_Stz);
  
  s256 sFilePath;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sFilePath) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  vTrace("\nTaking checkpoint into file : '%s'\n"_Stz, sFilePath.azChar());

  BPA* pBPA = new BPA;
  
  pBPA->vWritePersistentHeapMem(sFilePath);
  
  delete pBPA;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* persistence::reload -- reload                                              */
/*----------------------------------------------------------------------------*/
void Step::vPersistenceReload(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vPersistenceReload"_Stz);
  
  s256 sFilePath;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sFilePath) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  vTrace("\nReloading from file : '%s'\n"_Stz, sFilePath.azChar());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
