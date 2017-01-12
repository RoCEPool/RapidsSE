/*============================================================================*/
/* Task_Loader.cpp - task loader group                                        */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

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

// loader: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Loader(void)
{
  vAddPrimitive("loader.delete",      &vLoaderDelete,      false);
  vAddPrimitive("loader.finalize",    &vLoaderFinalize,    false);
  vAddPrimitive("loader.findChunk",   &vLoaderFindChunk,   false);
  vAddPrimitive("loader.init",        &vLoaderInit,        false);
  vAddPrimitive("loader.new",         &vLoaderNew,         false);
  vAddPrimitive("loader.newSub",      &vLoaderNewSub,      false);
  vAddPrimitive("loader.path!",       &vLoaderPathStore,   false);
  vAddPrimitive("loader.table!",      &vLoaderTableStore,  false);
  vAddPrimitive("loader.txref!",      &vLoaderTxRefStore,  false);
}

/*----------------------------------------------------------------------------*/
/* loader.delete -- delete a table loader                                     */
/*----------------------------------------------------------------------------*/
void Task::vLoaderDelete(NNP_Task nnpTask)
{
  ENTRY("vLoaderDelete");
  
  Object* pPtrLoader;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.delete");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  delete pLoader;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.finalize -- finalize a table loader                                 */
/*----------------------------------------------------------------------------*/
void Task::vLoaderFinalize(NNP_Task nnpTask)
{
  ENTRY("loader.finalize");
  
  Object* pPtrLoader;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.finalize");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  pLoader->vFinalize();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.findChunk -- find next chunk                                        */
/*----------------------------------------------------------------------------*/
void Task::vLoaderFindChunk(NNP_Task nnpTask)
{
  ENTRY("vLoaderFindChunk");
  
  Object* pPtrLoader;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.findChunk");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  bool bFoundChunk = pLoader->bFindChunk();
  
  nnpTask->m_pDataStack->vPush(bFoundChunk);
 
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.init -- init a table loader                                         */
/*----------------------------------------------------------------------------*/
void Task::vLoaderInit(NNP_Task nnpTask)
{
  ENTRY("vLoaderInit");
  
  Object* pPtrLoader;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.init");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  new(pLoader) Loader;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.new -- new a table loader                                           */
/*----------------------------------------------------------------------------*/
void Task::vLoaderNew(NNP_Task nnpTask)
{
  ENTRY("vLoaderNew");
  
  Loader* pLoader = new Loader;
  ASSERT(CN_LOADER == pLoader->eClass());
  
  nnpTask->m_pDataStack->vPush(pLoader);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.newSub -- new a loader sub                                          */
/*----------------------------------------------------------------------------*/
void Task::vLoaderNewSub(NNP_Task nnpTask)
{
  ENTRY("vLoaderNewSub");
  
  Object* pPtrLoader;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrLoader) )
    THROW_INVALID_DSTACK("loader.init");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  LoaderSub* pLoaderSub = new LoaderSub;
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vLoader(pLoader);
  
  nnpTask->m_pDataStack->vPush(pLoaderSub);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.path! -- store file path into loader                                */
/*----------------------------------------------------------------------------*/
void Task::vLoaderPathStore(NNP_Task nnpTask)
{
  ENTRY("vLoaderPathStore");
  
  ValWsp xWspPath;
  Object* pPtrLoader;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrLoader,xWspPath) )
    THROW_INVALID_DSTACK("loader.path!");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  s256 sPath;
  sPath = xWspPath.sStr();
  
  pLoader->vPath(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.table! -- store table into loader                                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderTableStore(NNP_Task nnpTask)
{
  ENTRY("vLoaderTableStore");
  
  Object* pPtrTable;
  Object* pPtrLoader;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrLoader,pPtrTable) )
    THROW_INVALID_DSTACK("loader.tableStore");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  Table* pTable = _pTable(pPtrTable);
  
  ASSERT(CN_TABLE == pTable->eClass());
  
  pLoader->vTable(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.txref! -- store txref into loader                                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderTxRefStore(NNP_Task nnpTask)
{
  ENTRY("vLoaderTxRefStore");
  
  Object* pPtrLoader;
  Object* pPtrTxRef;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrLoader,pPtrTxRef) )
    THROW_INVALID_DSTACK("loader.txref!");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  pLoader->vTxRef(pTxRef);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
