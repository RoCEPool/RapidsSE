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
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// loaderSub: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_LoaderSub(void)
{
  vAddPrimitive("loaderSub.appendBuffer"_Str, &vLoaderSubAppendBuffer,  false);
  vAddPrimitive("loaderSub.begin"_Str,        &vLoaderSubBegin,         false);
  vAddPrimitive("loaderSub.delete"_Str,       &vLoaderSubDelete,        false);
  vAddPrimitive("loaderSub.end"_Str,          &vLoaderSubEnd,           false);
  vAddPrimitive("loaderSub.fetchRecord"_Str,  &vLoaderSubFetchRecord,   false);
  vAddPrimitive("loaderSub.hashPartition"_Str,&vLoaderSubHashPartition, false);
  vAddPrimitive("loaderSub.hashRecord"_Str,   &vLoaderSubHashRecord,    false);
  vAddPrimitive("loaderSub.init"_Str,         &vLoaderSubInit,          false);
  vAddPrimitive("loaderSub.insertRow"_Str,    &vLoaderSubInsertRow,     false);
  vAddPrimitive("loaderSub.insertRow?"_Str,   &vLoaderSubInsertRow2,    false);
  vAddPrimitive("loaderSub.loader!"_Str,      &vLoaderSubLoaderStore,   false);
  vAddPrimitive("loaderSub::new"_Str,          &vLoaderSubNew,           false);
  vAddPrimitive("loaderSub.partition"_Str,    &vLoaderSubPartition,     false);
  vAddPrimitive("loaderSub.partition!"_Str,   &vLoaderSubPartitionStore,false);
  vAddPrimitive("loaderSub.rowBuffer"_Str,    &vLoaderSubRowBuffer,     false);
  vAddPrimitive("loaderSub.rowBuffer!"_Str,   &vLoaderSubRowBufferStore,false);
  vAddPrimitive("loaderSub.table"_Str,        &vLoaderSubTable,         false);
  vAddPrimitive("loaderSub.validateRow"_Str,  &vLoaderSubValidateRow,   false);
}

/*----------------------------------------------------------------------------*/
/* loaderSub.appendBuffer -- append buffer into table                         */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubAppendBuffer(MP_Task mpTask)
{
  ENTRY("vLoaderSubAppendBuffer"_Str);
  
  Object* pObject = _pPtrVal(mpTask->m_pRegFrame->pValReg(0))->pPtr();
  
  LoaderSub* pLoaderSub = _pLoaderSub(pObject);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vAppendBuffer();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.begin -- begin loading                                           */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubBegin(MP_Task mpTask)
{
  ENTRY("vLoaderSubBegin"_Str);
  
  Object* pObject = _pPtrVal(mpTask->m_pRegFrame->pValReg(0))->pPtr();
  
  LoaderSub* pLoaderSub = _pLoaderSub(pObject);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.delete -- delete a table loader                                  */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubDelete(MP_Task mpTask)
{
  ENTRY("vLoade3SubDelete"_Str);
  
  Object* pObject = _pPtrVal(mpTask->m_pRegFrame->pValReg(0))->pPtr();
  
  LoaderSub* pLoaderSub = _pLoaderSub(pObject);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  delete pLoaderSub;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.end -- end loading                                               */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubEnd(MP_Task mpTask)
{
  ENTRY("vLoaderSubEnd"_Str);
  
  Object* pObject = _pPtrVal(mpTask->m_pRegFrame->pValReg(0))->pPtr();
  
  LoaderSub* pLoaderSub = _pLoaderSub(pObject);
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
  ENTRY("vLoaderSubFetchRecord"_Str);
  
  Object* pObject = _pPtrVal(mpTask->m_pRegFrame->pValReg(0))->pPtr();
  
  LoaderSub* pLoaderSub = _pLoaderSub(pObject);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  bool bFetched = pLoaderSub->bFetchRecord();
  
  mpTask->m_pDataStack->vPushInt(bFetched);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.hashPartition -- hash partition & get node where partition lives */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubHashPartition(MP_Task mpTask)
{
  ENTRY("vLoaderSubHashRecord"_Str);
  
  Object* pObject = _pPtrVal(mpTask->m_pRegFrame->pValReg(0))->pPtr();
  
  LoaderSub* pLoaderSub = _pLoaderSub(pObject);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());

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
  ENTRY("vLoaderSubHashRecord"_Str);
  
  Object* pObject = _pPtrVal(mpTask->m_pRegFrame->pValReg(0))->pPtr();
  
  LoaderSub* pLoaderSub = _pLoaderSub(pObject);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vHashRecord();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.init -- init a table loader                                      */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubInit(MP_Task mpTask)
{
  ENTRY("vLoaderInit"_Str);
  
  Object* pPtrLoaderSub;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoaderSub) )
    THROW_INVALID_DSTACK("loaderSub.init"_Str);
  
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoaderSub);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  new(pLoaderSub) LoaderSub;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.insertRow -- insert row into buffer (buffer into table when full)*/
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubInsertRow(MP_Task mpTask)
{
  ENTRY("vLoaderSubInsertRow"_Str);
  
  Object* pObject = _pPtrVal(mpTask->m_pRegFrame->pValReg(0))->pPtr();
  
  LoaderSub* pLoaderSub = _pLoaderSub(pObject);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  bool bInserted = pLoaderSub->bInsertRow();
  
  mpTask->m_pDataStack->vPushInt(bInserted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.insertRow2 -- insert row into buffer                             */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubInsertRow2(MP_Task mpTask)
{
  ENTRY("vLoaderSubInsertRow?"_Str);
  
  Object* pObject = _pPtrVal(mpTask->m_pRegFrame->pValReg(0))->pPtr();
  
  LoaderSub* pLoaderSub = _pLoaderSub(pObject);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  bool bInserted = pLoaderSub->bInsertRow2();
  
  mpTask->m_pDataStack->vPushInt(bInserted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.loader! -- store loader                                          */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubLoaderStore(MP_Task mpTask)
{
  ENTRY("vLoaderSubLoaderStore"_Str);
  
  Object* pPtrLoaderSub;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoaderSub) )
    THROW_INVALID_DSTACK("loaderSub.loader!"_Str);
  
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoaderSub);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  Object* pPtrLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.openfile"_Str);
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub::new -- new a table loader subordinate                            */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubNew(MP_Task mpTask)
{
  ENTRY("vLoaderSubNew"_Str);
  
  LoaderSub* pLoaderSub = new LoaderSub;
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  mpTask->m_pDataStack->vPushObject(pLoaderSub);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.partition -- get partition number derived from hash of record    */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubPartition(MP_Task mpTask)
{
  ENTRY("vLoaderSubPartition"_Str);
  
  Object* pObject = _pPtrVal(mpTask->m_pRegFrame->pValReg(0))->pPtr();
  
  LoaderSub* pLoaderSub = _pLoaderSub(pObject);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  i8 iPartition = pLoaderSub->iPartition();
  
  mpTask->m_pDataStack->vPushInt(iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.partition! -- store partition number                             */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubPartitionStore(MP_Task mpTask)
{
  ENTRY("vLoaderSubPartitionStore"_Str);
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("loaderSub.partition!"_Str);
 
  Object* pObject = _pPtrVal(mpTask->m_pRegFrame->pValReg(0))->pPtr();
  
  LoaderSub* pLoaderSub = _pLoaderSub(pObject);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());

  pLoaderSub->vPartition(iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.rowBuffer -- get row buffer of given partition                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubRowBuffer(MP_Task mpTask)
{
  ENTRY("vLoaderSubRowBuffer"_Str);
  
  Object* pObject = _pPtrVal(mpTask->m_pRegFrame->pValReg(0))->pPtr();
  
  LoaderSub* pLoaderSub = _pLoaderSub(pObject);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  EntryBuffer* pRowBuffer = pLoaderSub->pRowBuffer();
  
  mpTask->m_pDataStack->vPushObject(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.rowBuffer! -- store row buffer                                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubRowBufferStore(MP_Task mpTask)
{
  ENTRY("vLoaderSubRowBufferStore"_Str);
  
  Object* pPtrRowBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("loaderSub.rowBuffer!"_Str);
  
  EntryBuffer* pRowBuffer = (EntryBuffer*)_pEntryBuffer(pPtrRowBuffer);
  ASSERT( !pRowBuffer || CN_ENTRYBUFFER == pRowBuffer->eClass() );
  
  Object* pObject = _pPtrVal(mpTask->m_pRegFrame->pValReg(0))->pPtr();
  
  LoaderSub* pLoaderSub = _pLoaderSub(pObject);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vRowBuffer(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.table -- get table being loaded                                  */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubTable(MP_Task mpTask)
{
  ENTRY("vLoaderSubTable"_Str);
  
  Object* pObject = _pPtrVal(mpTask->m_pRegFrame->pValReg(0))->pPtr();
  
  LoaderSub* pLoaderSub = _pLoaderSub(pObject);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  Table* pTable = pLoaderSub->pTable();
  
  mpTask->m_pDataStack->vPushObject(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.validateRow -- get table being loaded                            */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubValidateRow(MP_Task mpTask)
{
  ENTRY("vLoaderSubValidateRow"_Str);
  
  Object* pObject = _pPtrVal(mpTask->m_pRegFrame->pValReg(0))->pPtr();
  
  LoaderSub* pLoaderSub = _pLoaderSub(pObject);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  i8 i_Column;
  bool bValid = pLoaderSub->bValidateRow(i_Column);
  //ASSERT(bValid);
  
//  vTrace("vLoaderSubValidateRow: %s %lld\n"_Str,
//         bValid ? "Valid"_Str : "NotValid"_Str,
//         i_Column);
  
  mpTask->m_pDataStack->vPushInt(bValid);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
