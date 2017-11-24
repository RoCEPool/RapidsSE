/*============================================================================*/
/* Steps_LoaderSub.cpp - loader sub step group                                */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// loaderSub: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_LoaderSub(void)
{
  vAddPrimitive("loaderSub.begin"_Stz,        &vLoaderSubBegin,         false);
  vAddPrimitive("loaderSub.delete"_Stz,       &vLoaderSubDelete,        false);
  vAddPrimitive("loaderSub.end"_Stz,          &vLoaderSubEnd,           false);
  vAddPrimitive("loaderSub.fetchRecord"_Stz,  &vLoaderSubFetchRecord,   false);
  vAddPrimitive("loaderSub.hashPartition"_Stz,&vLoaderSubHashPartition, false);
  vAddPrimitive("loaderSub.hashRecord"_Stz,   &vLoaderSubHashRecord,    false);
  vAddPrimitive("loaderSub.init"_Stz,         &vLoaderSubInit,          false);
  vAddPrimitive("loaderSub.loader@"_Stz,      &vLoaderSubLoaderFetch,   false);
  vAddPrimitive("loaderSub.loader!"_Stz,      &vLoaderSubLoaderStore,   false);
  vAddPrimitive("loaderSub.loadData"_Stz,     &vLoaderSubLoadData,      false);
  vAddPrimitive("loaderSub::new"_Stz,         &vLoaderSubNew,           false);
  vAddPrimitive("loaderSub.newRowBuffer"_Stz, &vLoaderSubNewRowBuffer,  false);
  vAddPrimitive("loaderSub.partition@"_Stz,   &vLoaderSubPartitionFetch,false);
  vAddPrimitive("loaderSub.partition!"_Stz,   &vLoaderSubPartitionStore,false);
  vAddPrimitive("loaderSub.recordsFetched"_Stz, &vLoaderSubRecordsFetched, false);
  vAddPrimitive("loaderSub.rowBuffer@"_Stz,   &vLoaderSubRowBufferFetch,false);
  vAddPrimitive("loaderSub.rowBuffer!"_Stz,   &vLoaderSubRowBufferStore,false);
  vAddPrimitive("loaderSub.table"_Stz,        &vLoaderSubTable,         false);
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
  
  i8 iNode = Partition::iPartitionToNode(iPartition);
  
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
  
  new(pLoaderSub) LoaderSub;
  
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
  
  LoaderSub* pLoaderSub = new LoaderSub;
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  mpTask->m_pDataStack->vPushObject(pLoaderSub);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.newRowBuffer -- create a new row buffer for given partition      */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubNewRowBuffer(MP_Task mpTask)
{
  ENTRY("vLoaderSubNewRowBuffer"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.newRowBuffer"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  ASSERT( ! pLoaderSub->pRowBuffer() );
  pLoaderSub->vNewRowBuffer();
  
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
/* loaderSub.recordsFetched -- return no of records fetched by loader sub     */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubRecordsFetched(MP_Task mpTask)
{
  ENTRY("vLoaderSubRecordsFetched"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.recordsFetched"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  mpTask->m_pDataStack->vPushInt(pLoaderSub->iFetchedRecords());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.rowBuffer@ -- get row buffer of given partition                  */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubRowBufferFetch(MP_Task mpTask)
{
  ENTRY("vLoaderSubRowBufferFetch"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.rowBuffer@"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  EntryBuffer* pRowBuffer = pLoaderSub->pRowBuffer();
  
  mpTask->m_pDataStack->vPushObject(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.rowBuffer! -- store row buffer                                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubRowBufferStore(MP_Task mpTask)
{
  ENTRY("vLoaderSubRowBufferStore"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.rowBuffer@"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  Object* pPtrRowBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("loaderSub.rowBuffer!"_Stz);
  
  EntryBuffer* pRowBuffer = (EntryBuffer*)_pEntryBuffer(pPtrRowBuffer);
  ASSERT( !pRowBuffer || CN_ENTRYBUFFER == pRowBuffer->eClass() );
  
  pLoaderSub->vRowBuffer(pRowBuffer);
  
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
