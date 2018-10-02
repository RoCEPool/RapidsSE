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
  vAddStep("loaderSub.begin"_Stz,            &Task::vSubLoaderBegin,             false);
  vAddStep("loaderSub.danglingBuffer"_Stz,   &Task::vSubLoaderDanglingBuffer,    false);
  vAddStep("loaderSub.delete"_Stz,           &Task::vSubLoaderDelete,            false);
  vAddStep("loaderSub.end"_Stz,              &Task::vSubLoaderEnd,               false);
  vAddStep("loaderSub.fetchRecord"_Stz,      &Task::vSubLoaderFetchRecord,       false);
  vAddStep("loaderSub.fetchRecordDist"_Stz,  &Task::vSubLoaderFetchRecordDist,   false);
  vAddStep("loaderSub.fieldDelimiter!"_Stz,  &Task::vSubLoaderFieldDelimiterStore, false);
  vAddStep("loaderSub.hashPartition"_Stz,    &Task::vSubLoaderHashPartition,     false);
  vAddStep("loaderSub.hashRecord"_Stz,       &Task::vSubLoaderHashRecord,        false);
  vAddStep("loaderSub.init"_Stz,             &Task::vSubLoaderInit,              false);
  vAddStep("loaderSub.loader@"_Stz,          &Task::vSubLoaderLoaderFetch,       false);
  vAddStep("loaderSub.loader!"_Stz,          &Task::vSubLoaderLoaderStore,       false);
  vAddStep("loaderSub.loadData"_Stz,         &Task::vSubLoaderLoadData,          false);
  vAddStep("loaderSub.loadNodeData"_Stz,     &Task::vSubLoaderLoadNodeData,      false);
  vAddStep("loaderSub::new"_Stz,             &Task::vSubLoaderNew,               false);
  vAddStep("loaderSub.newWorkingBuffer"_Stz, &Task::vSubLoaderNewWorkingBuffer,  false);
  vAddStep("loaderSub.partition@"_Stz,       &Task::vSubLoaderPartitionFetch,    false);
  vAddStep("loaderSub.partition!"_Stz,       &Task::vSubLoaderPartitionStore,    false);
  vAddStep("loaderSub.processRecords"_Stz,   &Task::vSubLoaderProcessRecords,    false);
  vAddStep("loaderSub.recordsInvalid"_Stz,   &Task::vSubLoaderRecordsInvalid,    false);
  vAddStep("loaderSub.recordsValid"_Stz,     &Task::vSubLoaderRecordsValid,      false);
  vAddStep("loaderSub.spawnLoadBuffer"_Stz,  &Task::vSubLoaderSpawnLoadBuffer,   false);
  vAddStep("loaderSub.workingBuffer@"_Stz,   &Task::vSubLoaderWorkingBufferFetch,false);
  vAddStep("loaderSub.workingBuffer!"_Stz,   &Task::vSubLoaderWorkingBufferStore,false);
  vAddStep("loaderSub.table"_Stz,            &Task::vSubLoaderTable,             false);
}

/*----------------------------------------------------------------------------*/
/* loaderSub.begin -- begin loading                                           */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderBegin(MP_Task mpTask)
{
  ENTRY("vSubLoaderBegin"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.begin"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  ASSERT(CN_SUBLOADER == pSubLoader->eClass());
  
  pSubLoader->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.dangingBuffer -- ensure there's a dangling buffer for partition  */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderDanglingBuffer(MP_Task mpTask)
{
  ENTRY("vSubLoaderDanglingBuffer"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.delete"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  if ( ! pSubLoader->pWorkingBuffer() )
    pSubLoader->vNewWorkingBuffer();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.delete -- delete a table loader                                  */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderDelete(MP_Task mpTask)
{
  ENTRY("vSubLoaderDelete"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.delete"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  ASSERT(CN_SUBLOADER == pSubLoader->eClass());
  
  delete pSubLoader;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.end -- end loading                                               */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderEnd(MP_Task mpTask)
{
  ENTRY("vSubLoaderEnd"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.end"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  ASSERT(CN_SUBLOADER == pSubLoader->eClass());
  
  pSubLoader->vEnd();
  
  delete pSubLoader;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.fetchRecord -- fetch next record from file being loaded          */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderFetchRecord(MP_Task mpTask)
{
  ENTRY("vSubLoaderFetchRecord"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.fetchRecord"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  bool bFetched = pSubLoader->bFetchRecord();
  
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  
  //pSubLoader->vTruncateRecord(13);
  
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  
  mpTask->m_pDataStack->vPushInt(bFetched);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.fetchRecordDist -- fetch next record from file being loaded      */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderFetchRecordDist(MP_Task mpTask)
{
  ENTRY("vSubLoaderFetchRecord"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.fetchRecord"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  bool bFetched = pSubLoader->bFetchRecordDist();
  
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  
  //pSubLoader->vTruncateRecord(13);
  
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  
  mpTask->m_pDataStack->vPushInt(bFetched);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.fieldDelimiter! -- store field delimiter in sub loader           */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderFieldDelimiterStore(MP_Task mpTask)
{
  ENTRY("vSubLoaderFieldDelimiterStore"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.fieldDelimiter!"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  s32 sFieldDelimiter;
  if ( ! mpTask->m_pDataStack->bPop(sFieldDelimiter) )
    THROW_INVALID_DSTACK("loaderSub.fieldDelimiter!"_Stz);
  
  pSubLoader->vSetDistFormatter(*(sFieldDelimiter.aChar()));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.hashRecord -- hash record being loaded                           */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderHashRecord(MP_Task mpTask)
{
  ENTRY("vSubLoaderHashRecord"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.hashRecord"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  pSubLoader->vHashRecord();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.hashPartition -- hash partition & get node where partition lives */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderHashPartition(MP_Task mpTask)
{
  ENTRY("vSubLoaderHashRecord"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.hashPartition"_Stz);
  
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  i8 iPartition = pSubLoader->iPartition();
  
  i8 iNode = TablePtn::iPartitionToNode(iPartition);
  
  mpTask->m_pDataStack->vPushInt(iNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.init -- init a table loader                                      */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderInit(MP_Task mpTask)
{
  ENTRY("vLoaderInit"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.init"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  new(pSubLoader) SubLoader(nullptr,nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.loadData -- load data                                            */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderLoadData(MP_Task mpTask)
{
  ENTRY("vSubLoaderLoadData"_Stz);
  
  Object* pPtrLoaderSub;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoaderSub) )
    THROW_INVALID_DSTACK("loaderSub.loadData"_Stz);
  
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoaderSub);
  
  do  {
    
    // hash the record
    pSubLoader->vHashRecord();
    
    // attempt to insert the record
    bool bInserted = pSubLoader->bInsertRow();
    
    // check if buffer was full
    if ( ! bInserted) {
      
      // buffer was full
      // return to loaderSub to append buffer to the table/partition
      mpTask->m_pDataStack->vPushInt(true);
      return;
    }
    
  } while (pSubLoader->bFetchRecord());
  
  mpTask->m_pDataStack->vPushInt(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.loadNodeData -- load node data                                   */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderLoadNodeData(MP_Task mpTask)
{
  ENTRY("vSubLoaderLoadNodeData"_Stz);
  
  Object* pPtrLoaderSub;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoaderSub) )
    THROW_INVALID_DSTACK("loaderSub.loadNodeData"_Stz);
  
  // get this node
  i8 iNode = gpAttachMap->m_pGrid->iThisNode();
  
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoaderSub);
  
  do  {
    
    // hash the record
    pSubLoader->vHashRecord();
    
    i8 iPartition = pSubLoader->iPartition();
    
    // skip records not for this node
    if (TablePtn::iPartitionToNode(iPartition) != iNode) {
      //vTrace("Node: %lld skipping row %s for partition %lld\n"_Stz, iNode, pSubLoader->aRecord(), iPartition);
      // we should come across data only for this node
      RASSERT(TablePtn::iPartitionToNode(iPartition) == iNode);
    }
    
    // attempt to insert the record
    bool bInserted = pSubLoader->bInsertRowDist();
    
    // check if buffer was full
    if ( ! bInserted) {
      
      // buffer was full
      // return to loaderSub to append buffer to the table/partition
      mpTask->m_pDataStack->vPushInt(true);
      return;
    }
    
  } while (pSubLoader->bFetchRecordDist());
  
  mpTask->m_pDataStack->vPushInt(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.loader@ -- fetch loader                                          */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderLoaderFetch(MP_Task mpTask)
{
  ENTRY("vSubLoaderLoaderFetch"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.partition@"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  LoadTable* pLoadTable = pSubLoader->pLoadTable();
  
  mpTask->m_pDataStack->vPushObject(pLoadTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.loader! -- store loader                                          */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderLoaderStore(MP_Task mpTask)
{
  ENTRY("vSubLoaderLoaderStore"_Stz);
  
  Object* pPtrLoaderSub;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoaderSub) )
    THROW_INVALID_DSTACK("loaderSub.loader!"_Stz);
  
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoaderSub);
  ASSERT(CN_SUBLOADER == pSubLoader->eClass());
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loadTable.openfile"_Stz);
  
  LoadTable* pLoadTable = _pLoader(pPtrLoader);
  ASSERT(CN_LOADTABLE == pLoadTable->eClass());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub::new -- new a table loader subordinate                            */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderNew(MP_Task mpTask)
{
  ENTRY("vSubLoaderNew"_Stz);
  
  SubLoader* pSubLoader = new SubLoader(nullptr,nullptr);
  ASSERT(CN_SUBLOADER == pSubLoader->eClass());
  
  mpTask->m_pDataStack->vPushObject(pSubLoader);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.newWorkingBuffer --create new working buffer for given partition */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderNewWorkingBuffer(MP_Task mpTask)
{
  ENTRY("vSubLoaderNewWorkingBuffer"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.newWorkingBuffer"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  ASSERT( ! pSubLoader->pWorkingBuffer() );
  pSubLoader->vNewWorkingBuffer();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.partition@ -- get partition number derived from hash of record   */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderPartitionFetch(MP_Task mpTask)
{
  ENTRY("vSubLoaderPartitionFetch"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.partition@"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  i8 iPartition = pSubLoader->iPartition();
  
  mpTask->m_pDataStack->vPushInt(iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.partition! -- store partition number                             */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderPartitionStore(MP_Task mpTask)
{
  ENTRY("vSubLoaderPartitionStore"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.partition!"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("loaderSub.partition!"_Stz);

  pSubLoader->vPartition(iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.processRecords -- read & process records till buffer full or eof  */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderProcessRecords(MP_Task mpTask)
{
  ENTRY("vSubLoaderProcessRecords"_Stz);
  
  Object* pPtrLoaderSub;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoaderSub) )
    THROW_INVALID_DSTACK("loaderSub.processRecords"_Stz);
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
/* loaderSub.recordsInvalid -- return no of invalid records fetched by sub    */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderRecordsInvalid(MP_Task mpTask)
{
  ENTRY("vSubLoaderRecordsInvalid"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.recordsInvalid"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  mpTask->m_pDataStack->vPushInt(pSubLoader->iInvalidRecords());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.recordsValid -- return no of valid records fetched by sub        */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderRecordsValid(MP_Task mpTask)
{
  ENTRY("vSubLoaderRecordsValid"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.recordsValid"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  mpTask->m_pDataStack->vPushInt(pSubLoader->iValidRecords());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.table -- get table being loaded                                  */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderTable(MP_Task mpTask)
{
  ENTRY("vSubLoaderTable"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.table"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  Table* pTable = pSubLoader->pTable();
  
  mpTask->m_pDataStack->vPushObject(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.spawnLoadBuffer -- spawn Job_LoadBuffer                          */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderSpawnLoadBuffer(MP_Task mpTask)
{
  ENTRY("vSubLoaderSpawnLoadBuffer"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.spawnLoadBuffer"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  i8 iFinal;
  if ( ! mpTask->m_pDataStack->bPop(iFinal) )
    THROW_INVALID_DSTACK("loaderSub.spawnLoadBuffer"_Stz);
  
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

/*----------------------------------------------------------------------------*/
/* loaderSub.workingBuffer@ -- get working buffer of given partition          */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderWorkingBufferFetch(MP_Task mpTask)
{
  ENTRY("vSubLoaderWorkingBufferFetch"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.workingBuffer@"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  WorkingBuffer* pWorkingBuffer = pSubLoader->pWorkingBuffer();
  
  mpTask->m_pDataStack->vPushObject(pWorkingBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.workingBuffer! -- store working buffer                           */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderWorkingBufferStore(MP_Task mpTask)
{
  ENTRY("vSubLoaderWorkingBufferStore"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.workingBuffer@"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrLoader);
  
  Object* pPtrWorkingBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrWorkingBuffer) )
    THROW_INVALID_DSTACK("loaderSub.workingBuffer!"_Stz);
  
  WorkingBuffer* pWorkingBuffer = (WorkingBuffer*)_pWorkingBuffer(pPtrWorkingBuffer);
  
  pSubLoader->vWorkingBuffer(pWorkingBuffer);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
