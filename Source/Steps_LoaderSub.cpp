/*============================================================================*/
/* Steps_LoaderSub.cpp - loader sub step group                                */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*  gpAttachMap;

// loaderSub: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddSteps_LoaderSub(void)
{
  vAddStep("loaderSub.begin"_Stz,         &vLoaderSubBegin,         false);
  vAddStep("loaderSub.danglingBuffer"_Stz,&vLoaderSubDanglingBuffer,false);
  vAddStep("loaderSub.delete"_Stz,       &vLoaderSubDelete,        false);
  vAddStep("loaderSub.end"_Stz,          &vLoaderSubEnd,           false);
  vAddStep("loaderSub.fetchRecord"_Stz,  &vLoaderSubFetchRecord,   false);
  vAddStep("loaderSub.fetchRecordDist"_Stz,  &vLoaderSubFetchRecordDist, false);
  vAddStep("loaderSub.fieldDelimiter!"_Stz,  &vLoaderSubFieldDelimiterStore, false);
  vAddStep("loaderSub.hashPartition"_Stz,&vLoaderSubHashPartition, false);
  vAddStep("loaderSub.hashRecord"_Stz,   &vLoaderSubHashRecord,    false);
  vAddStep("loaderSub.init"_Stz,         &vLoaderSubInit,          false);
  vAddStep("loaderSub.loader@"_Stz,      &vLoaderSubLoaderFetch,   false);
  vAddStep("loaderSub.loader!"_Stz,      &vLoaderSubLoaderStore,   false);
  vAddStep("loaderSub.loadData"_Stz,     &vLoaderSubLoadData,      false);
  vAddStep("loaderSub.loadNodeData"_Stz, &vLoaderSubLoadNodeData,  false);
  vAddStep("loaderSub::new"_Stz,         &vLoaderSubNew,           false);
  vAddStep("loaderSub.newWorkingBuffer"_Stz, &vLoaderSubNewWorkingBuffer,false);
  vAddStep("loaderSub.partition@"_Stz,   &vLoaderSubPartitionFetch,false);
  vAddStep("loaderSub.partition!"_Stz,   &vLoaderSubPartitionStore,false);
  vAddStep("loaderSub.processRecords"_Stz,  &vLoaderSubProcessRecords, false);
  vAddStep("loaderSub.recordsInvalid"_Stz, &vLoaderSubRecordsInvalid, false);
  vAddStep("loaderSub.recordsValid"_Stz, &vLoaderSubRecordsValid,  false);
  vAddStep("loaderSub.spawnLoadBuffer"_Stz, &vLoaderSubSpawnLoadBuffer, false);
  vAddStep("loaderSub.workingBuffer@"_Stz, &vLoaderSubWorkingBufferFetch,false);
  vAddStep("loaderSub.workingBuffer!"_Stz, &vLoaderSubWorkingBufferStore,false);
  vAddStep("loaderSub.table"_Stz,        &vLoaderSubTable,         false);
}

/*----------------------------------------------------------------------------*/
/* loaderSub.begin -- begin loading                                           */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubBegin(MP_Task mpTask)
{
  ENTRY("vLoaderSubBegin"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.begin"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.dangingBuffer -- ensure there's a dangling buffer for partition  */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubDanglingBuffer(MP_Task mpTask)
{
  ENTRY("vLoaderSubDanglingBuffer"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.delete"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  if ( ! pLoaderSub->pWorkingBuffer() )
    pLoaderSub->vNewWorkingBuffer();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.delete -- delete a table loader                                  */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubDelete(MP_Task mpTask)
{
  ENTRY("vLoaderSubDelete"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.delete"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  delete pLoaderSub;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.end -- end loading                                               */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubEnd(MP_Task mpTask)
{
  ENTRY("vLoaderSubEnd"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.end"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vEnd();
  
  delete pLoaderSub;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.fetchRecord -- fetch next record from file being loaded          */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubFetchRecord(MP_Task mpTask)
{
  ENTRY("vLoaderSubFetchRecord"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.fetchRecord"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  bool bFetched = pLoaderSub->bFetchRecord();
  
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  
  //pLoaderSub->vTruncateRecord(13);
  
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  
  mpTask->m_pDataStack->vPushInt(bFetched);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.fetchRecordDist -- fetch next record from file being loaded      */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubFetchRecordDist(MP_Task mpTask)
{
  ENTRY("vLoaderSubFetchRecord"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.fetchRecord"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  bool bFetched = pLoaderSub->bFetchRecordDist();
  
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  
  //pLoaderSub->vTruncateRecord(13);
  
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  
  mpTask->m_pDataStack->vPushInt(bFetched);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.fieldDelimiter! -- store field delimiter in sub loader           */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubFieldDelimiterStore(MP_Task mpTask)
{
  ENTRY("vLoaderSubFieldDelimiterStore"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.fieldDelimiter!"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  s32 sFieldDelimiter;
  if ( ! mpTask->m_pDataStack->bPop(sFieldDelimiter) )
    THROW_INVALID_DSTACK("loaderSub.fieldDelimiter!"_Stz);
  
  pLoaderSub->vSetDistFormatter(*(sFieldDelimiter.aChar()));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.hashRecord -- hash record being loaded                           */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubHashRecord(MP_Task mpTask)
{
  ENTRY("vLoaderSubHashRecord"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.hashRecord"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  pLoaderSub->vHashRecord();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.hashPartition -- hash partition & get node where partition lives */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubHashPartition(MP_Task mpTask)
{
  ENTRY("vLoaderSubHashRecord"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.hashPartition"_Stz);
  
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  i8 iPartition = pLoaderSub->iPartition();
  
  i8 iNode = TablePtn::iPartitionToNode(iPartition);
  
  mpTask->m_pDataStack->vPushInt(iNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.init -- init a table loader                                      */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubInit(MP_Task mpTask)
{
  ENTRY("vLoaderInit"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.init"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  new(pLoaderSub) LoaderSub(nullptr,nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.loadData -- load data                                            */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubLoadData(MP_Task mpTask)
{
  ENTRY("vLoaderSubLoadData"_Stz);
  
  Object* pPtrLoaderSub;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoaderSub) )
    THROW_INVALID_DSTACK("loaderSub.loadData"_Stz);
  
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoaderSub);
  
  do  {
    
    // hash the record
    pLoaderSub->vHashRecord();
    
    // attempt to insert the record
    bool bInserted = pLoaderSub->bInsertRow();
    
    // check if buffer was full
    if ( ! bInserted) {
      
      // buffer was full
      // return to loaderSub to append buffer to the table/partition
      mpTask->m_pDataStack->vPushInt(true);
      return;
    }
    
  } while (pLoaderSub->bFetchRecord());
  
  mpTask->m_pDataStack->vPushInt(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.loadNodeData -- load node data                                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubLoadNodeData(MP_Task mpTask)
{
  ENTRY("vLoaderSubLoadNodeData"_Stz);
  
  Object* pPtrLoaderSub;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoaderSub) )
    THROW_INVALID_DSTACK("loaderSub.loadNodeData"_Stz);
  
  // get this node
  i8 iNode = gpAttachMap->m_pGrid->iThisNode();
  
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoaderSub);
  
  do  {
    
    // hash the record
    pLoaderSub->vHashRecord();
    
    i8 iPartition = pLoaderSub->iPartition();
    
    // skip records not for this node
    if (TablePtn::iPartitionToNode(iPartition) != iNode) {
      //vTrace("Node: %lld skipping row %s for partition %lld\n"_Stz, iNode, pLoaderSub->aRecord(), iPartition);
      // we should come across data only for this node
      RASSERT(TablePtn::iPartitionToNode(iPartition) == iNode);
    }
    
    // attempt to insert the record
    bool bInserted = pLoaderSub->bInsertRowDist();
    
    // check if buffer was full
    if ( ! bInserted) {
      
      // buffer was full
      // return to loaderSub to append buffer to the table/partition
      mpTask->m_pDataStack->vPushInt(true);
      return;
    }
    
  } while (pLoaderSub->bFetchRecordDist());
  
  mpTask->m_pDataStack->vPushInt(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.loader@ -- fetch loader                                          */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubLoaderFetch(MP_Task mpTask)
{
  ENTRY("vLoaderSubLoaderFetch"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.partition@"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  Loader* pLoader = pLoaderSub->pLoader();
  
  mpTask->m_pDataStack->vPushObject(pLoader);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.loader! -- store loader                                          */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubLoaderStore(MP_Task mpTask)
{
  ENTRY("vLoaderSubLoaderStore"_Stz);
  
  Object* pPtrLoaderSub;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoaderSub) )
    THROW_INVALID_DSTACK("loaderSub.loader!"_Stz);
  
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoaderSub);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.openfile"_Stz);
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub::new -- new a table loader subordinate                            */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubNew(MP_Task mpTask)
{
  ENTRY("vLoaderSubNew"_Stz);
  
  LoaderSub* pLoaderSub = new LoaderSub(nullptr,nullptr);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  mpTask->m_pDataStack->vPushObject(pLoaderSub);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.newWorkingBuffer --create new working buffer for given partition */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubNewWorkingBuffer(MP_Task mpTask)
{
  ENTRY("vLoaderSubNewWorkingBuffer"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.newWorkingBuffer"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  ASSERT( ! pLoaderSub->pWorkingBuffer() );
  pLoaderSub->vNewWorkingBuffer();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.partition@ -- get partition number derived from hash of record   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubPartitionFetch(MP_Task mpTask)
{
  ENTRY("vLoaderSubPartitionFetch"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.partition@"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  i8 iPartition = pLoaderSub->iPartition();
  
  mpTask->m_pDataStack->vPushInt(iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.partition! -- store partition number                             */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubPartitionStore(MP_Task mpTask)
{
  ENTRY("vLoaderSubPartitionStore"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.partition!"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("loaderSub.partition!"_Stz);

  pLoaderSub->vPartition(iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.processRecords -- read & process records till buffer full or eof  */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubProcessRecords(MP_Task mpTask)
{
  ENTRY("vLoaderSubProcessRecords"_Stz);
  
  Object* pPtrLoaderSub;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoaderSub) )
    THROW_INVALID_DSTACK("loaderSub.processRecords"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoaderSub);
  
  while (pLoaderSub->bFetchRecord()) {
    
    // hash the record to get m_iPartition
    pLoaderSub->vHashRecord();
  
    // attempt to add record into a buffer
    bool bInserted = pLoaderSub->bInsertRow();
  
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
void Task::vLoaderSubRecordsInvalid(MP_Task mpTask)
{
  ENTRY("vLoaderSubRecordsInvalid"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.recordsInvalid"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  mpTask->m_pDataStack->vPushInt(pLoaderSub->iInvalidRecords());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.recordsValid -- return no of valid records fetched by sub        */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubRecordsValid(MP_Task mpTask)
{
  ENTRY("vLoaderSubRecordsValid"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.recordsValid"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  mpTask->m_pDataStack->vPushInt(pLoaderSub->iValidRecords());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.table -- get table being loaded                                  */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubTable(MP_Task mpTask)
{
  ENTRY("vLoaderSubTable"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.table"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  Table* pTable = pLoaderSub->pTable();
  
  mpTask->m_pDataStack->vPushObject(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.spawnLoadBuffer -- spawn Job_LoadBuffer                          */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubSpawnLoadBuffer(MP_Task mpTask)
{
  ENTRY("vLoaderSubSpawnLoadBuffer"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.spawnLoadBuffer"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  i8 iFinal;
  if ( ! mpTask->m_pDataStack->bPop(iFinal) )
    THROW_INVALID_DSTACK("loaderSub.spawnLoadBuffer"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get info
  
  Job_SubLoader* pJob_SubLoader = pLoaderSub->pJob_SubLoader();
  Job_LoadTable* pJob_LoadTable = pJob_SubLoader->pJob_LoadTable();
  
  i8 iJobLoadTableId = pJob_LoadTable->iJobId();
  i8 iPartition = pLoaderSub->iPartition();
  WorkingBuffer* pWorkingBuffer = pLoaderSub->pWorkingBuffer();
  pLoaderSub->vWorkingBuffer(nullptr);
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
    i8 nBuffersLoadable = pLoaderSub->nBuffersLoadable();
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
void Task::vLoaderSubWorkingBufferFetch(MP_Task mpTask)
{
  ENTRY("vLoaderSubWorkingBufferFetch"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.workingBuffer@"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  WorkingBuffer* pWorkingBuffer = pLoaderSub->pWorkingBuffer();
  
  mpTask->m_pDataStack->vPushObject(pWorkingBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.workingBuffer! -- store working buffer                           */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubWorkingBufferStore(MP_Task mpTask)
{
  ENTRY("vLoaderSubWorkingBufferStore"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.workingBuffer@"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  Object* pPtrWorkingBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrWorkingBuffer) )
    THROW_INVALID_DSTACK("loaderSub.workingBuffer!"_Stz);
  
  WorkingBuffer* pWorkingBuffer = (WorkingBuffer*)_pWorkingBuffer(pPtrWorkingBuffer);
  
  pLoaderSub->vWorkingBuffer(pWorkingBuffer);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
