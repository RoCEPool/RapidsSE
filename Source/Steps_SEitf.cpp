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

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// seitf: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_SEitf(void)
{
  vAddPrimitive("seitf.accessPath@"_Stz,      &vSEitfAccessPathFetch,  false);
  vAddPrimitive("seitf.accessPath!"_Stz,      &vSEitfAccessPathStore,  false);
  vAddPrimitive("seitf.bufferQueue@"_Stz,     &vSEitfBufferQueueFetch, false);
  vAddPrimitive("seitf.bufferQueue!"_Stz,     &vSEitfBufferQueueStore, false);
  vAddPrimitive("seitf.count"_Stz,            &vSEitfCount,            false);
  vAddPrimitive("seitf.delete"_Stz,           &vSEitfDelete,           false);
  vAddPrimitive("seitf.dicStack"_Stz,         &vSEitfDicStack,         false);
  vAddPrimitive("seitf.display@"_Stz,         &vSEitfDisplayFetch,     false);
  vAddPrimitive("seitf.display!"_Stz,         &vSEitfDisplayStore,     false);
  vAddPrimitive("seitf.dump"_Stz,             &vSEitfDump,             false);
  vAddPrimitive("seitf.entry@"_Stz,           &vSEitfEntryFetch,       false);
  vAddPrimitive("seitf.entry!"_Stz,           &vSEitfEntryStore,       false);
  vAddPrimitive("seitf.groupBy@"_Stz,         &vSEitfGroupByFetch,     false);
  vAddPrimitive("seitf.groupBy!"_Stz,         &vSEitfGroupByStore,     false);
  vAddPrimitive("seitf.indexBuffer@"_Stz,     &vSEitfIndexBufferFetch, false);
  vAddPrimitive("seitf.indexBuffer!"_Stz,     &vSEitfIndexBufferStore, false);
  vAddPrimitive("seitf.index!"_Stz,           &vSEitfIndexStore,       false);
  vAddPrimitive("seitf.index@"_Stz,           &vSEitfIndexFetch,       false);
  vAddPrimitive("seitf.metadata@"_Stz,        &vSEitfMetadataFetch,    false);
  vAddPrimitive("seitf.metadata!"_Stz,        &vSEitfMetadataStore,    false);
  vAddPrimitive("seitf.partition@"_Stz,       &vSEitfPartitionFetch,   false);
  vAddPrimitive("seitf.partition!"_Stz,       &vSEitfPartitionStore,   false);
  vAddPrimitive("seitf.pid"_Stz,              &vSEitfPid,              false);
  vAddPrimitive("seitf.req@"_Stz,             &vSEitfReqFetch,         false);
  vAddPrimitive("seitf.req!"_Stz,             &vSEitfReqStore,         false);
  vAddPrimitive("seitf.rowsProcessed"_Stz,    &vSEitfRowsProcessed,    false);
  vAddPrimitive("seitf.rsp@"_Stz,             &vSEitfRspFetch,         false);
  vAddPrimitive("seitf.rsp!"_Stz,             &vSEitfRspStore,         false);
  vAddPrimitive("seitf.responseQueue"_Stz,    &vSEitfResponseQueue,    false);
  vAddPrimitive("seitf.table@"_Stz,           &vSEitfTableFetch,       false);
  vAddPrimitive("seitf.tableRef"_Stz,         &vSEitfTableRef,         false);
  vAddPrimitive("seitf.tableDeref"_Stz,       &vSEitfTableDeref,       false);
}

/*----------------------------------------------------------------------------*/
/* seitf.accessPath@ -- fetch access path from SE interface                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfAccessPathFetch(MP_Task mpTask)
{
  ENTRY("vSEitfAccessPathFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.accessPath@"_Stz);
  
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
  ENTRY("vSEitfAccessPathStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.accessPath!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrAccessPath;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccessPath) )
    THROW_INVALID_DSTACK("seitf.accessPath!"_Stz);
  
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);
  
  pSEitf->m_pAccessPath = pAccessPath;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.bufferQueue@ -- get SEitf buffer queue                               */
/*----------------------------------------------------------------------------*/
void Task::vSEitfBufferQueueFetch(MP_Task mpTask)
{
  ENTRY("vSEitfBufferQueueFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.bufferQueue@"_Stz);
  
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
  ENTRY("vSEitfBufferQueueStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.bufferQueue!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrQueue;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrQueue) )
    THROW_INVALID_DSTACK("seitf.bufferQueue!"_Stz);
  
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
  ENTRY("vSEitfCount"_Stz);
  
  Object* pPtrSEitf;
  
  mpTask->m_pDataStack->vPushInt(gpSEitfList->iCount());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.delete -- delete SE interface                                        */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDelete(MP_Task mpTask)
{
ENTRY("vSEitfDelete"_Stz);
  
  Object* pPtrSEitf;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.delete"_Stz);

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
  ENTRY("vSEitfDicStack"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.dicStack"_Stz);
  
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
  ENTRY("vSEitfDisplayFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.display@"_Stz);
  
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
  ENTRY("vSEitfDisplayStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.display!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  i8 iDisplay;
  if ( ! mpTask->m_pDataStack->bPop(iDisplay) )
    THROW_INVALID_DSTACK("seitf.displayOff"_Stz);
  
  pSEitf->m_bDisplay = iDisplay == 1 ? true : false;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.dump -- dump of SE interface                                         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDump(MP_Task mpTask)
{
ENTRY("vSEitfDump"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.dump"_Stz);
  
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
  ENTRY("vSEitfEntryFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.entry@"_Stz);
  
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
  ENTRY("vSEitfEntryStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.entry!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrEntry;
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("seitf.entry!"_Stz);
  
  Entry* pEntry = _pEntry(pPtrEntry);
  ASSERT(nullptr == pEntry || CN_ENTRY == pEntry->eClass());
  
  pSEitf->m_pEntry = pEntry;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.groupBy! -- store groupBy for slot                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfGroupByStore(MP_Task mpTask)
{
  ENTRY("vSEitfGroupByStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.groupBy!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrGroupBy;
  if ( ! mpTask->m_pDataStack->bPop(pPtrGroupBy) )
    THROW_INVALID_DSTACK("seitf.groupBy!"_Stz);
  
  GroupBy* pGroupBy = _pGroupBy(pPtrGroupBy);
  ASSERT(CN_GROUPBY == pGroupBy->eClass());
  
  i8 iSlot;
  if ( ! mpTask->m_pDataStack->bPop(iSlot) )
    THROW_INVALID_DSTACK("seitf.groupBy!"_Stz);
  
  pSEitf->m_pGroupBy[iSlot] = pGroupBy;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.groupBy@ -- fetch groupBy for slot                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfGroupByFetch(MP_Task mpTask)
{
  ENTRY("vSEitfGroupByFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.groupBy@"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  i8 iSlot;
  if ( ! mpTask->m_pDataStack->bPop(iSlot) )
    THROW_INVALID_DSTACK("seitf.partition!"_Stz);
  
  mpTask->m_pDataStack->vPushObject(pSEitf->m_pGroupBy[iSlot]);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* seitf.indexBuffer@ -- fetch index buffer from SE interface                 */
/*----------------------------------------------------------------------------*/
void Task::vSEitfIndexBufferFetch(MP_Task mpTask)
{
  ENTRY("vSEitfIndexBufferFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.indexBuffer@"_Stz);
  
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
  ENTRY("vSEitfIndexBufferStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.indexBuffer!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrIndexBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("seitf.indexBuffer!"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(nullptr == pIndexBuffer || CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  pSEitf->m_pIndexBuffer = pIndexBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.index@ -- fetch index from SEitf                                     */
/*----------------------------------------------------------------------------*/
void Task::vSEitfIndexFetch(MP_Task mpTask)
{
  ENTRY("vSEitfIndexReturn"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.index@"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushObject(pSEitf->pCurrentIndex());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.index! -- store index in SEitf                                       */
/*----------------------------------------------------------------------------*/
void Task::vSEitfIndexStore(MP_Task mpTask)
{
  ENTRY("vSEitfIndexStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.index!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrIndex;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("seitf.index!"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(pIndex == nullptr || CN_INDEX == pIndex->eClass());
  
  pSEitf->vCurrentIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.metadata@ -- fetch metadata in SE interface                          */
/*----------------------------------------------------------------------------*/
void Task::vSEitfMetadataFetch(MP_Task mpTask)
{
  ENTRY("vSEitfMetadataFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.metadata@"_Stz);
  
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
  ENTRY("vSEitfMetadataStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.metadata!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  s256 sMetadata;
  if ( ! mpTask->m_pDataStack->bPop(sMetadata) )
    THROW_INVALID_DSTACK("seitf.metadata@"_Stz);
  
  pSEitf->m_sMetadata = sMetadata;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.partition@ -- fetch partition of SE interface                        */
/*----------------------------------------------------------------------------*/
void Task::vSEitfPartitionFetch(MP_Task mpTask)
{
  ENTRY("vSEitfPartitionFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.partition@"_Stz);
  
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
  ENTRY("vSEitfPartitionStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.partition!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("seitf.partition!"_Stz);
  
  pSEitf->m_iPartition = iPartition;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.pid -- get pid of SE interface                                       */
/*----------------------------------------------------------------------------*/
void Task::vSEitfPid(MP_Task mpTask)
{
  ENTRY("vSEitfPid"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.pid"_Stz);
  
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
ENTRY("vSEitfReqFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.req@"_Stz);
  
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
ENTRY("vSEitfReqStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.req!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrQueue;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrQueue) )
    THROW_INVALID_DSTACK("seitf.req!"_Stz);
  
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
  ENTRY("vSEitfRowsProcessed"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.rowsProcessed"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  i8 iRowsProcessed;
  if ( ! mpTask->m_pDataStack->bPop(iRowsProcessed) )
    THROW_INVALID_DSTACK("seitf.rowsProcessed"_Stz);
  
  pSEitf->m_iRowsProcessed = iRowsProcessed;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.rsp@ -- fetch SEitf response queue                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfRspFetch(MP_Task mpTask)
{
ENTRY("vSEitfRspFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.rsp@"_Stz);
  
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
ENTRY("vSEitfRspStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.rsp!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrQueue;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrQueue) )
    THROW_INVALID_DSTACK("seitf.rsp!"_Stz);
  
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
  ENTRY("vSEitfeResponseQueue"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.responseQueue"_Stz);
  
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
  ENTRY("vSEitfTableFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.rsp!"_Stz);
  
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
  ENTRY("vSEitfRspStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.tableDeref"_Stz);
  
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
  ENTRY("vSEitfTableRef"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.tableRef"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrTable;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("seitf.tableRef"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vRefInc();
  
  pSEitf->vCurrentTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
