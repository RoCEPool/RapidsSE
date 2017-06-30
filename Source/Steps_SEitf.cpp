/*============================================================================*/
/* Steps_SEitf.cpp - SEitf step group                                         */
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

// seitf: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_SEitf(void)
{
  vAddPrimitive("seitf.accessPath@"_Str,      &vSEitfAccessPathFetch,  false);
  vAddPrimitive("seitf.accessPath!"_Str,      &vSEitfAccessPathStore,  false);
  vAddPrimitive("seitf.bufferQueue@"_Str,     &vSEitfBufferQueueFetch, false);
  vAddPrimitive("seitf.bufferQueue!"_Str,     &vSEitfBufferQueueStore, false);
  vAddPrimitive("seitf.count"_Str,            &vSEitfCount,            false);
  vAddPrimitive("seitf.delete"_Str,           &vSEitfDelete,           false);
  vAddPrimitive("seitf.dicStack"_Str,         &vSEitfDicStack,         false);
  vAddPrimitive("seitf.display@"_Str,         &vSEitfDisplayFetch,     false);
  vAddPrimitive("seitf.display!"_Str,         &vSEitfDisplayStore,     false);
  vAddPrimitive("seitf.dump"_Str,             &vSEitfDump,             false);
  vAddPrimitive("seitf.entry@"_Str,           &vSEitfEntryFetch,       false);
  vAddPrimitive("seitf.entry!"_Str,           &vSEitfEntryStore,       false);
  vAddPrimitive("seitf.indexBuffer@"_Str,     &vSEitfIndexBufferFetch, false);
  vAddPrimitive("seitf.indexBuffer!"_Str,     &vSEitfIndexBufferStore, false);
  vAddPrimitive("seitf.indexGet"_Str,         &vSEitfIndexGet,         false);
  vAddPrimitive("seitf.indexReturn"_Str,      &vSEitfIndexReturn,      false);
  vAddPrimitive("seitf.metadata@"_Str,        &vSEitfMetadataFetch,    false);
  vAddPrimitive("seitf.metadata!"_Str,        &vSEitfMetadataStore,    false);
  vAddPrimitive("seitf.partition@"_Str,       &vSEitfPartitionFetch,   false);
  vAddPrimitive("seitf.partition!"_Str,       &vSEitfPartitionStore,   false);
  vAddPrimitive("seitf.pid"_Str,              &vSEitfPid,              false);
  vAddPrimitive("seitf.req@"_Str,             &vSEitfReqFetch,         false);
  vAddPrimitive("seitf.req!"_Str,             &vSEitfReqStore,         false);
  vAddPrimitive("seitf.rowsProcessed"_Str,    &vSEitfRowsProcessed,    false);
  vAddPrimitive("seitf.rsp@"_Str,             &vSEitfRspFetch,         false);
  vAddPrimitive("seitf.rsp!"_Str,             &vSEitfRspStore,         false);
  vAddPrimitive("seitf.responseQueue"_Str,    &vSEitfResponseQueue,    false);
  vAddPrimitive("seitf.table@"_Str,           &vSEitfTableFetch,       false);
  vAddPrimitive("seitf.tableRef"_Str,         &vSEitfTableRef,         false);
  vAddPrimitive("seitf.tableDeref"_Str,       &vSEitfTableDeref,       false);
}

/*----------------------------------------------------------------------------*/
/* seitf.accessPath@ -- fetch access path from SE interface                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfAccessPathFetch(MP_Task mpTask)
{
  ENTRY("vSEitfAccessPathFetch"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.accessPath@"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushObject(pSEitf->m_pAccessPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.accessPath! -- store access path in SE interface                     */
/*----------------------------------------------------------------------------*/
void Task::vSEitfAccessPathStore(MP_Task mpTask)
{
  ENTRY("vSEitfAccessPathStore"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.accessPath!"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrAccessPath;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccessPath) )
    THROW_INVALID_DSTACK("seitf.accessPath!"_Str);
  
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);
  
  pSEitf->m_pAccessPath = pAccessPath;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.bufferQueue@ -- get SEitf buffer queue                               */
/*----------------------------------------------------------------------------*/
void Task::vSEitfBufferQueueFetch(MP_Task mpTask)
{
  ENTRY("vSEitfBufferQueueFetch"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.bufferQueue@"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushObject(pSEitf->pBufferQueue());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.bufferQueue! -- store SEitf buffer queue                             */
/*----------------------------------------------------------------------------*/
void Task::vSEitfBufferQueueStore(MP_Task mpTask)
{
  ENTRY("vSEitfBufferQueueStore"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.bufferQueue!"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrQueue;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrQueue) )
    THROW_INVALID_DSTACK("seitf.bufferQueue!"_Str);
  
  Queue* pQueue = _pQueue(pPtrQueue);
  ASSERT(nullptr == pQueue || CN_QUEUE == pQueue->eClass());
  
  pSEitf->vBufferQueue(pQueue);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* seitf.count -- get count of SE interface's                                 */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCount(MP_Task mpTask)
{
  ENTRY("vSEitfCount"_Str);
  
  Object* pPtrSEitf;
  
  mpTask->m_pDataStack->vPushInt(gpSEitfList->iCount());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.delete -- delete SE interface                                        */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDelete(MP_Task mpTask)
{
ENTRY("vSEitfDelete"_Str);
  
  Object* pPtrSEitf;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.delete"_Str);

  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  delete pSEitf;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.dicStack -- get SEitf dic stack                                      */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDicStack(MP_Task mpTask)
{
  ENTRY("vSEitfDicStack"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.dicStack"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushObject(pSEitf->pPathStack());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.display@ -- get display from the SE interface                        */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDisplayFetch(MP_Task mpTask)
{
  ENTRY("vSEitfDisplayFetch"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.display@"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushInt(pSEitf->m_bDisplay ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.display! -- set display in the SE interface                          */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDisplayStore(MP_Task mpTask)
{
  ENTRY("vSEitfDisplayStore"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.display!"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  i8 iDisplay;
  if ( ! mpTask->m_pDataStack->bPop(iDisplay) )
    THROW_INVALID_DSTACK("seitf.displayOff"_Str);
  
  pSEitf->m_bDisplay = iDisplay == 1 ? true : false;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.dump -- dump of SE interface                                         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDump(MP_Task mpTask)
{
ENTRY("vSEitfDump"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.dump"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  pSEitf->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.entry@ -- fetch entry from SE interface                              */
/*----------------------------------------------------------------------------*/
void Task::vSEitfEntryFetch(MP_Task mpTask)
{
  ENTRY("vSEitfEntryFetch"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.entry@"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushObject(pSEitf->m_pEntry);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.entry! -- store entry in SE interface                                */
/*----------------------------------------------------------------------------*/
void Task::vSEitfEntryStore(MP_Task mpTask)
{
  ENTRY("vSEitfEntryStore"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.entry!"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrEntry;
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("seitf.entry!"_Str);
  
  Entry* pEntry = _pEntry(pPtrEntry);
  ASSERT(nullptr == pEntry || CN_ENTRY == pEntry->eClass());
  
  pSEitf->m_pEntry = pEntry;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.indexBuffer@ -- fetch index buffer from SE interface                 */
/*----------------------------------------------------------------------------*/
void Task::vSEitfIndexBufferFetch(MP_Task mpTask)
{
  ENTRY("vSEitfIndexBufferFetch"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.indexBuffer@"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushObject(pSEitf->m_pIndexBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.indexBuffer! -- store index buffer in SE interface                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfIndexBufferStore(MP_Task mpTask)
{
  ENTRY("vSEitfIndexBufferStore"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.indexBuffer!"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrIndexBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("seitf.indexBuffer!"_Str);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  pSEitf->m_pIndexBuffer = pIndexBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.indexReturn -- clear index in SEitf                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfIndexReturn(MP_Task mpTask)
{
  ENTRY("vSEitfIndexReturn"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.indexReturn"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  pSEitf->vCurrentIndex(nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.indexGet -- store index in SEitf                                     */
/*----------------------------------------------------------------------------*/
void Task::vSEitfIndexGet(MP_Task mpTask)
{
  ENTRY("vSEitfIndexGet"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.indexGet"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrIndex;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("seitf.indexGet"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  pSEitf->vCurrentIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.metadata@ -- fetch metadata in SE interface                          */
/*----------------------------------------------------------------------------*/
void Task::vSEitfMetadataFetch(MP_Task mpTask)
{
  ENTRY("vSEitfMetadataFetch"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.metadata@"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  s256 sMetadata;
  
  sMetadata = pSEitf->m_sMetadata;
  mpTask->m_pDataStack->vPushS256(sMetadata);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.metadata! -- store metadata in SE interface                          */
/*----------------------------------------------------------------------------*/
void Task::vSEitfMetadataStore(MP_Task mpTask)
{
  ENTRY("vSEitfMetadataStore"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.metadata!"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  s256 sMetadata;
  if ( ! mpTask->m_pDataStack->bPop(sMetadata) )
    THROW_INVALID_DSTACK("seitf.metadata@"_Str);
  
  pSEitf->m_sMetadata = sMetadata;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.partition@ -- fetch partition of SE interface                        */
/*----------------------------------------------------------------------------*/
void Task::vSEitfPartitionFetch(MP_Task mpTask)
{
  ENTRY("vSEitfPartitionFetch"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.partition@"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushInt(pSEitf->m_iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.partition! -- store partition of SE interface                        */
/*----------------------------------------------------------------------------*/
void Task::vSEitfPartitionStore(MP_Task mpTask)
{
  ENTRY("vSEitfPartitionStore"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.partition!"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("seitf.partition!"_Str);
  
  pSEitf->m_iPartition = iPartition;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.pid -- get pid of SE interface                                       */
/*----------------------------------------------------------------------------*/
void Task::vSEitfPid(MP_Task mpTask)
{
  ENTRY("vSEitfPid"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.pid"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushInt(pSEitf->m_iPid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.req@ -- fetch SEitf request queue                                    */
/*----------------------------------------------------------------------------*/
void Task::vSEitfReqFetch(MP_Task mpTask)
{
ENTRY("vSEitfReqFetch"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.req@"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrQueue = pSEitf->pRequestQueue();
  
  mpTask->m_pDataStack->vPushObject(pPtrQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.req! -- store SEitf request queue                                    */
/*----------------------------------------------------------------------------*/
void Task::vSEitfReqStore(MP_Task mpTask)
{
ENTRY("vSEitfReqStore"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.req!"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrQueue;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrQueue) )
    THROW_INVALID_DSTACK("seitf.req!"_Str);
  
  Queue* pQueue = _pQueue(pPtrQueue);
  ASSERT(CN_QUEUE == pQueue->eClass());
  
  pSEitf->vRequestQueue(pQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.rowsProcessed -- store rows processed in SE interface                */
/*----------------------------------------------------------------------------*/
void Task::vSEitfRowsProcessed(MP_Task mpTask)
{
  ENTRY("vSEitfRowsProcessed"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.rowsProcessed"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  i8 iRowsProcessed;
  if ( ! mpTask->m_pDataStack->bPop(iRowsProcessed) )
    THROW_INVALID_DSTACK("seitf.rowsProcessed"_Str);
  
  pSEitf->m_iRowsProcessed = iRowsProcessed;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.rsp@ -- fetch SEitf response queue                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfRspFetch(MP_Task mpTask)
{
ENTRY("vSEitfRspFetch"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.rsp@"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrQueue = pSEitf->pResponseQueue();
  
  mpTask->m_pDataStack->vPushObject(pPtrQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.rsp! -- store SEitf response queue                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfRspStore(MP_Task mpTask)
{
ENTRY("vSEitfRspStore"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.rsp!"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrQueue;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrQueue) )
    THROW_INVALID_DSTACK("seitf.rsp!"_Str);
  
  Queue* pQueue = _pQueue(pPtrQueue);
  ASSERT(CN_QUEUE == pQueue->eClass());
  
  pSEitf->vResponseQueue(pQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.responseQueue -- get SEitf response queue                            */
/*----------------------------------------------------------------------------*/
void Task::vSEitfResponseQueue(MP_Task mpTask)
{
  ENTRY("vSEitfeResponseQueue"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.responseQueue"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushObject(pSEitf->pResponseQueue());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.table@ -- fetch table from SEitf                                     */
/*----------------------------------------------------------------------------*/
void Task::vSEitfTableFetch(MP_Task mpTask)
{
  ENTRY("vSEitfTableFetch"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.rsp!"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushObject(pSEitf->pCurrentTable());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.tableDeref -- store table in SEitf and inc reference                 */
/*----------------------------------------------------------------------------*/
void Task::vSEitfTableDeref(MP_Task mpTask)
{
  ENTRY("vSEitfRspStore"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.tableDeref"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Table* pTable = pSEitf->pCurrentTable();
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vRefDec();

  pSEitf->vCurrentTable(nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.tableRef -- store table in SEitf and inc reference                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfTableRef(MP_Task mpTask)
{
  ENTRY("vSEitfTableRef"_Str);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.tableRef"_Str);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrTable;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("seitf.tableRef"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vRefInc();
  
  pSEitf->vCurrentTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
