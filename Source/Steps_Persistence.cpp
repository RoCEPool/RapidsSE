/*============================================================================*/
/* Steps_Persistence.cpp - persistence step group                             */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap*      gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddSteps_Persistence(void)
{
  vAddStep("persistence::checkpoint"_Stz, &Task::vPersistenceCheckpoint,  false);
  vAddStep("persistence::reload"_Stz,     &Task::vPersistenceReload,      false);
}

/*----------------------------------------------------------------------------*/
/* persistence::checkpoint -- take a checkpoint                               */
/*----------------------------------------------------------------------------*/
void Task::vPersistenceCheckpoint(MP_Task mpTask)
{
  ENTRY("vPersistenceCheckpoint"_Stz);
  
  s256 sFilePath;
  if ( ! mpTask->m_pDataStack->bPop(sFilePath) )
    THROW_INVALID_DSTACK("persistence::checkpoint"_Stz);
  
  vTrace("\nTaking checkpoint into file : '%s'\n"_Stz, sFilePath.aChar());

  BPA* pBPA = new BPA;
  
  pBPA->vWritePersistentHeaps(sFilePath);
  
  delete pBPA;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* persistence::reload -- reload                                              */
/*----------------------------------------------------------------------------*/
void Task::vPersistenceReload(MP_Task mpTask)
{
  ENTRY("vPersistenceReload"_Stz);
  
  s256 sFilePath;
  if ( ! mpTask->m_pDataStack->bPop(sFilePath) )
    THROW_INVALID_DSTACK("persistence::reload"_Stz);
  
  vTrace("\nReloading from file : '%s'\n"_Stz, sFilePath.aChar());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
