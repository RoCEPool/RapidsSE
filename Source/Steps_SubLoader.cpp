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
void Step::vAddSteps_LoaderSub(void)
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
  vAddStep("subLoader.spawnLoadBuffer"_Stz,  &Task::vSubLoaderSpawnLoadBuffer,   false);
}

/*----------------------------------------------------------------------------*/
/* subLoader.begin -- begin loading                                           */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderBegin(MP_Task mpTask)
{
  ENTRY("vSubLoaderBegin"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("subLoader.begin"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
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
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("subLoader.delete"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
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
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("subLoader.delete"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
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
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("subLoader.end"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
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
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("subLoader.partition@"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
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
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("subLoader.partition!"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("subLoader.partition!"_Stz);

  pSubLoader->vPartition(iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subLoader.processRecords -- read & process records till buffer full or eof  */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderProcessRecords(MP_Task mpTask)
{
  ENTRY("vSubLoaderProcessRecords"_Stz);
  
  Object* pPtrLoaderSub;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoaderSub) )
    THROW_INVALID_DSTACK("subLoader.processRecords"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoaderSub);
  
  while (pSubLoader->bFetchRecord()) {
    
    // hash the record to get m_iPartition
    pSubLoader->vHashRecord();
  
    // attempt to add record into a buffer
    bool bInserted = pSubLoader->bInsertRow();
  
    // check if buffer was full
    if ( ! bInserted) {
      
      // buffer was full
      // return to loaderSub to append buffer to the table/partition
      mpTask->m_pDataStack->vPushInt(true);
      return;
    }
  
  }

  mpTask->m_pDataStack->vPushInt(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subLoader.recordsInvalid -- return no of invalid records fetched by sub    */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderRecordsInvalid(MP_Task mpTask)
{
  ENTRY("vSubLoaderRecordsInvalid"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("subLoader.recordsInvalid"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  mpTask->m_pDataStack->vPushInt(pSubLoader->iInvalidRecords());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subLoader.recordsValid -- return no of valid records fetched by sub        */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderRecordsValid(MP_Task mpTask)
{
  ENTRY("vSubLoaderRecordsValid"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("subLoader.recordsValid"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  mpTask->m_pDataStack->vPushInt(pSubLoader->iValidRecords());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subLoader.spawnLoadBuffer -- spawn Job_LoadBuffer                          */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderSpawnLoadBuffer(MP_Task mpTask)
{
  ENTRY("vSubLoaderSpawnLoadBuffer"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("subLoader.spawnLoadBuffer"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  i8 iFinal;
  if ( ! mpTask->m_pDataStack->bPop(iFinal) )
    THROW_INVALID_DSTACK("subLoader.spawnLoadBuffer"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get info
  
  Job_SubLoader* pJob_SubLoader = pSubLoader->pJob_SubLoader();
  Job_LoadTable* pJob_LoadTable = pJob_SubLoader->pJob_LoadTable();
  
  i8 iJobLoadTableId = pJob_LoadTable->iJobId();
  i8 iPartition = pSubLoader->iPartition();
  WorkingBuffer* pWorkingBuffer = pSubLoader->pWorkingBuffer();
  pSubLoader->vWorkingBuffer(nullptr);
  SEitf* pSEitf = pJob_SubLoader->m_pSEitf;
  
  //ASSERT( (  pWorkingBuffer->bIsLast() && 0 == pWorkingBuffer->nWorkingRows()) ||
  //        ( !pWorkingBuffer->bIsLast() && pWorkingBuffer->nWorkingRows()));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a load buffer job
  
  //! this needs to be optimised
  JobClass* pJobClass_LoadBuffer = JobClass::pFind(CN_JOB_LOADBUFFER);
  
  Job_LoadBuffer* pJob_LoadBuffer =
    static_cast<Job_LoadBuffer*>(pJobClass_LoadBuffer->pPreJob(pSEitf));

  pJob_LoadBuffer->vSeed(SS_SEED_RUN);
  
  pJob_LoadBuffer->vJobLoadTableId(iJobLoadTableId);
  pJob_LoadBuffer->m_sObjectName = pJob_SubLoader->m_sObjectName;
  pJob_LoadBuffer->vPartition(iPartition);
  pJob_LoadBuffer->m_pWorkingBuffer = pWorkingBuffer;
  
  if (iFinal) {
    i8 nBuffersLoadable = pSubLoader->nBuffersLoadable();
    ASSERT(C_UNKNOWN != nBuffersLoadable);
    pJob_LoadBuffer->vBuffersFromSubLoader(nBuffersLoadable);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn the load buffer job
  
  RapidsSE_bSpawnJob(pJob_LoadBuffer,SS_SEED_RUN,pSEitf);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
