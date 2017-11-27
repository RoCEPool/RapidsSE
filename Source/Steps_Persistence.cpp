/*============================================================================*/
/* Steps_Persistence.cpp - persistence step group                             */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap*      gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Persistence(void)
{
  vAddPrimitive("persistence::checkpoint"_Stz,    &vPersistenceCheckpoint,     false);
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
  
  vTrace("Taking checkpoint into file : %s\n"_Stz, sFilePath.aChar());

  BPA* pBPA = new BPA;
  
  pBPA->vWritePersistentHeaps(sFilePath);
  
  delete pBPA;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
