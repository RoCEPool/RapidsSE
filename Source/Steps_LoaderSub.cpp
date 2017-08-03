/*============================================================================*/
/* Steps_LoaderSub.cpp - loader sub step group                                */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
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
  vAddPrimitive("loaderSub.insertRow"_Stz,    &vLoaderSubInsertRow,     false);
  vAddPrimitive("loaderSub.loader!"_Stz,      &vLoaderSubLoaderStore,   false);
  vAddPrimitive("loaderSub::new"_Stz,         &vLoaderSubNew,           false);
  vAddPrimitive("loaderSub.partition"_Stz,    &vLoaderSubPartition,     false);
  vAddPrimitive("loaderSub.partition!"_Stz,   &vLoaderSubPartitionStore,false);
  vAddPrimitive("loaderSub.rowBuffer"_Stz,    &vLoaderSubRowBuffer,     false);
  vAddPrimitive("loaderSub.rowBuffer!"_Stz,   &vLoaderSubRowBufferStore,false);
  vAddPrimitive("loaderSub.table"_Stz,        &vLoaderSubTable,         false);
  vAddPrimitive("loaderSub.validateRow"_Stz,  &vLoaderSubValidateRow,   false);
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
/* loaderSub.insertRow -- insert row into buffer                              */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubInsertRow(MP_Task mpTask)
{
  ENTRY("vLoaderSubInsertRow"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.insertRow"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  bool bInserted = pLoaderSub->bInsertRow();
  
  mpTask->m_pDataStack->vPushInt(bInserted);
  
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
/* loaderSub.partition@ -- get partition number derived from hash of record   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubPartition(MP_Task mpTask)
{
  ENTRY("vLoaderSubPartition"_Stz);
  
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
/* loaderSub.rowBuffer@ -- get row buffer of given partition                  */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubRowBuffer(MP_Task mpTask)
{
  ENTRY("vLoaderSubRowBuffer"_Stz);
  
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

/*----------------------------------------------------------------------------*/
/* loaderSub.validateRow -- get table being loaded                            */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubValidateRow(MP_Task mpTask)
{
  ENTRY("vLoaderSubValidateRow"_Stz);
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loaderSub.validateRow"_Stz);
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoader);
  
  i8 i_Column;
  bool bValid = pLoaderSub->bValidateRow(i_Column);
  //ASSERT(bValid);
  
//  vTrace("vLoaderSubValidateRow: %s %lld\n"_Stz,
//         bValid ? "Valid"_Stz : "NotValid"_Stz,
//         i_Column);
  
  mpTask->m_pDataStack->vPushInt(bValid);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
