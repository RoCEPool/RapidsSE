/*============================================================================*/
/* Steps_SubLoader.cpp - sub-loader step group                                */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*  gpAttachMap;

// loaderSub: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SubLoader(void)
{
  vAddStep("subLoader.begin"_Stz,            &Task::vSubLoaderBegin,             false);
  vAddStep("subLoader.danglingBuffer"_Stz,   &Task::vSubLoaderDanglingBuffer,    false);
  vAddStep("subLoader.delete"_Stz,           &Task::vSubLoaderDelete,            false);
  vAddStep("subLoader.end"_Stz,              &Task::vSubLoaderEnd,               false);
  vAddStep("subLoader.partition@"_Stz,       &Task::vSubLoaderPartitionFetch,    false);
  vAddStep("subLoader.partition!"_Stz,       &Task::vSubLoaderPartitionStore,    false);
  vAddStep("subLoader.processRecords"_Stz,   &Task::vSubLoaderProcessRecords,    false);
  vAddStep("subLoader.recordsInvalid"_Stz,   &Task::vSubLoaderRecordsInvalid,    false);
  vAddStep("subLoader.recordsValid"_Stz,     &Task::vSubLoaderRecordsValid,      false);
}

/*----------------------------------------------------------------------------*/
/* subLoader.begin -- begin loading                                           */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderBegin(MP_Task mpTask)
{
  ENTRY("vSubLoaderBegin"_Stz);
  
  Object* pPtrSubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSubLoader) )
    THROW_INVALID_DSTACK("subLoader.begin"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrSubLoader);
  
  ASSERT(CN_SUBLOADER == pSubLoader->eClass());
  
  pSubLoader->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subLoader.dangingBuffer -- ensure there's a dangling buffer for partition  */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderDanglingBuffer(MP_Task mpTask)
{
  ENTRY("vSubLoaderDanglingBuffer"_Stz);
  
  Object* pPtrSubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSubLoader) )
    THROW_INVALID_DSTACK("subLoader.delete"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrSubLoader);
  
  if ( ! pSubLoader->pWorkingBuffer() )
    pSubLoader->vNewWorkingBuffer();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subLoader.delete -- delete a table loader                                  */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderDelete(MP_Task mpTask)
{
  ENTRY("vSubLoaderDelete"_Stz);
  
  Object* pPtrSubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSubLoader) )
    THROW_INVALID_DSTACK("subLoader.delete"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrSubLoader);
  
  ASSERT(CN_SUBLOADER == pSubLoader->eClass());
  
  delete pSubLoader;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subLoader.end -- end loading                                               */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderEnd(MP_Task mpTask)
{
  ENTRY("vSubLoaderEnd"_Stz);
  
  Object* pPtrSubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSubLoader) )
    THROW_INVALID_DSTACK("subLoader.end"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrSubLoader);
  
  ASSERT(CN_SUBLOADER == pSubLoader->eClass());
  
  pSubLoader->vEnd();
  
  delete pSubLoader;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subLoader.partition@ -- get partition number derived from hash of record   */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderPartitionFetch(MP_Task mpTask)
{
  ENTRY("vSubLoaderPartitionFetch"_Stz);
  
  Object* pPtrSubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSubLoader) )
    THROW_INVALID_DSTACK("subLoader.partition@"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrSubLoader);
  
  i8 iPartition = pSubLoader->iPartition();
  
  mpTask->m_pDataStack->vPushInt(iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subLoader.partition! -- store partition number                             */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderPartitionStore(MP_Task mpTask)
{
  ENTRY("vSubLoaderPartitionStore"_Stz);
  
  Object* pPtrSubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSubLoader) )
    THROW_INVALID_DSTACK("subLoader.partition!"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrSubLoader);
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("subLoader.partition!"_Stz);

  pSubLoader->vPartition(iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subLoader.processRecords -- read & process records till buffer full or eof */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderProcessRecords(MP_Task mpTask)
{
  ENTRY("vSubLoaderProcessRecords"_Stz);
  
  Object* pPtrSubLoaderSub;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSubLoaderSub) )
    THROW_INVALID_DSTACK("subLoader.processRecords"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrSubLoaderSub);
  
  bool bTargetBufferFull = pSubLoader->bProcessRecords();
  
  mpTask->m_pDataStack->vPushInt(bTargetBufferFull);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subLoader.recordsInvalid -- return no of invalid records fetched by sub    */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderRecordsInvalid(MP_Task mpTask)
{
  ENTRY("vSubLoaderRecordsInvalid"_Stz);
  
  Object* pPtrSubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSubLoader) )
    THROW_INVALID_DSTACK("subLoader.recordsInvalid"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrSubLoader);
  
  mpTask->m_pDataStack->vPushInt(pSubLoader->iInvalidRecords());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subLoader.recordsValid -- return no of valid records fetched by sub        */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderRecordsValid(MP_Task mpTask)
{
  ENTRY("vSubLoaderRecordsValid"_Stz);
  
  Object* pPtrSubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSubLoader) )
    THROW_INVALID_DSTACK("subLoader.recordsValid"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrSubLoader);
  
  mpTask->m_pDataStack->vPushInt(pSubLoader->iValidRecords());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
