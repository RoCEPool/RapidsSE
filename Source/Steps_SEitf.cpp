/*============================================================================*/
/* Steps_SEitf.cpp - SEitf step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// seitf: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_SEitf(void)
{
  vAddPrimitive("seitf.accessPath@"_Stz,      &vSEitfAccessPathFetch,  false);
  vAddPrimitive("seitf.accessPath!"_Stz,      &vSEitfAccessPathStore,  false);
  vAddPrimitive("seitf.acksRemainingDec"_Stz, &vSEitfAcksRemainingDec, false);
  vAddPrimitive("seitf.acksRemaining@"_Stz,   &vSEitfAcksRemainingFetch, false);
  vAddPrimitive("seitf.acksRemaining!"_Stz,   &vSEitfAcksRemainingStore, false);
  vAddPrimitive("seitf.bufferQueue@"_Stz,     &vSEitfBufferQueueFetch, false);
  vAddPrimitive("seitf.bufferQueue!"_Stz,     &vSEitfBufferQueueStore, false);
  vAddPrimitive("seitf.commandProcessor"_Stz, &vSEitfCommandProcessor, false);
  vAddPrimitive("seitf.dicStack"_Stz,         &vSEitfDicStack,         false);
  vAddPrimitive("seitf.display@"_Stz,         &vSEitfDisplayFetch,     false);
  vAddPrimitive("seitf.display!"_Stz,         &vSEitfDisplayStore,     false);
  vAddPrimitive("seitf.dump"_Stz,             &vSEitfDump,             false);
  vAddPrimitive("seitf.fieldDelimiter@"_Stz,  &vSEitfFieldDelimiterFetch,   false);
  vAddPrimitive("seitf.entry@"_Stz,           &vSEitfEntryFetch,       false);
  vAddPrimitive("seitf.entry!"_Stz,           &vSEitfEntryStore,       false);
  vAddPrimitive("seitf.fetchTaskEnded"_Stz,   &vSEitfFetchTaskEnded,   false);
  vAddPrimitive("seitf.groupBy@"_Stz,         &vSEitfGroupByFetch,     false);
  vAddPrimitive("seitf.groupBy!"_Stz,         &vSEitfGroupByStore,     false);
  vAddPrimitive("seitf.indexBuffer@"_Stz,     &vSEitfIndexBufferFetch, false);
  vAddPrimitive("seitf.indexBuffer!"_Stz,     &vSEitfIndexBufferStore, false);
  vAddPrimitive("seitf.index!"_Stz,           &vSEitfIndexStore,       false);
  vAddPrimitive("seitf.index@"_Stz,           &vSEitfIndexFetch,       false);
  vAddPrimitive("seitf.isExiting"_Stz,        &vSEitfIsExiting,        false);
  vAddPrimitive("seitf.lastBuffers@"_Stz,     &vSEitfLastBuffersFetch, false);
  vAddPrimitive("seitf.lastBuffers!"_Stz,     &vSEitfLastBuffersStore, false);
  vAddPrimitive("seitf.partition@"_Stz,       &vSEitfPartitionFetch,   false);
  vAddPrimitive("seitf.partition!"_Stz,       &vSEitfPartitionStore,   false);
  vAddPrimitive("seitf.pid"_Stz,              &vSEitfPid,              false);
  vAddPrimitive("seitf.planner@"_Stz,         &vSEitfPlannerFetch,     false);
  vAddPrimitive("seitf.planner!"_Stz,         &vSEitfPlannerStore,     false);
  vAddPrimitive("seitf.rowsInvalid!"_Stz,     &vSEitfRowsInvalidStore, false);
  vAddPrimitive("seitf.rowsProcessed"_Stz,    &vSEitfRowsProcessed,    false);
  vAddPrimitive("seitf.rowsValid!"_Stz,       &vSEitfRowsValidStore,   false);
  vAddPrimitive("seitf.slot@"_Stz,            &vSEitfSlotFetch,        false);
  vAddPrimitive("seitf.table@"_Stz,           &vSEitfTableFetch,       false);
  vAddPrimitive("seitf.tableRef"_Stz,         &vSEitfTableRef,         false);
  vAddPrimitive("seitf.tableDeref"_Stz,       &vSEitfTableDeref,       false);
  vAddPrimitive("seitf.tx@"_Stz,              &vSEitfTxFetch,          false);
  vAddPrimitive("seitf.tx!"_Stz,              &vSEitfTxStore,          false);
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
/* seitf.acksRemainingDec -- decrement acks remaining                         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfAcksRemainingDec(MP_Task mpTask)
{
  ENTRY("vSEitfAcksRemainingDec"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.acksRemainingDec"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  pSEitf->vDecAcksRemaining();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.acksRemainingFetch -- fetch acks remaining                           */
/*----------------------------------------------------------------------------*/
void Task::vSEitfAcksRemainingFetch(MP_Task mpTask)
{
  ENTRY("vSEitfacksRemainingFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.acksRemaining@"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  i8 iAcksRemaining = pSEitf->iGetAcksRemaining();
  mpTask->m_pDataStack->vPushInt(iAcksRemaining);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.acksRemaining! -- store acks remaining                               */
/*----------------------------------------------------------------------------*/
void Task::vSEitfAcksRemainingStore(MP_Task mpTask)
{
  ENTRY("vSEitfAcksRemainingStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.acksRemaining!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  i8 iAcksRemaining;
  if ( ! mpTask->m_pDataStack->bPop(iAcksRemaining) )
    THROW_INVALID_DSTACK("seitf.AcksRemaining!"_Stz);
  
  pSEitf->vSetAcksRemaining(iAcksRemaining);
  
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
/* seitf.commandProcessor -- get SEitf command processor                      */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCommandProcessor(MP_Task mpTask)
{
  ENTRY("vSEitfDicStack"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.dicStack"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushObject(pSEitf->m_pCommandProcessor);
  
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
/* seitf.fetchTaskEnded -- fetch task has eneded                              */
/*----------------------------------------------------------------------------*/
void Task::vSEitfFetchTaskEnded(MP_Task mpTask)
{
  ENTRY("vSEitfFetchTaskEnded"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.fetchTaskEnded"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  --pSEitf->m_ziFetchTasks;
  //vTrace("Fetch task ended %s %lld\n"_Stz,
  //       pSEitf->m_pCurrentTable->aTableName(), *pSEitf->m_ziFetchTasks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.fieldDelimiter@ -- get field delimiter                               */
/*----------------------------------------------------------------------------*/
void Task::vSEitfFieldDelimiterFetch(MP_Task mpTask)
{
  ENTRY("vSEitfFieldDelimiterFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.fieldDelimiter@"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  s32 sFieldDelimiter;
  Stz* s = (Stz*)(&pSEitf->m_xFormatters.m_cFieldDelimiter);
  sFieldDelimiter = s;
  
  mpTask->m_pDataStack->vPushS32(sFieldDelimiter);
  
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
  
  mpTask->m_pDataStack->vPushObject(pSEitf->m_pEntryBuffer);
  
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
  
  Object* pPtrEntryBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntryBuffer) )
    THROW_INVALID_DSTACK("seitf.indexBuffer!"_Stz);
  
  EntryBuffer* pEntryBuffer = _pEntryBuffer(pPtrEntryBuffer);
  
  pSEitf->m_pEntryBuffer = pEntryBuffer;
  
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
/* seitf.isExiting -- is seitf exiting                                        */
/*----------------------------------------------------------------------------*/
void Task::vSEitfIsExiting(MP_Task mpTask)
{
  ENTRY("vSEitfIsExiting"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.isExiting"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushInt(pSEitf->m_bSeitfExiting);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.lastBuffers@ -- fetch the number of last buffers for a fetch         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfLastBuffersFetch(MP_Task mpTask)
{
  ENTRY("vSEitfLastBuffersFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
  THROW_INVALID_DSTACK("seitf.indexBuffer@"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  i8 nLastBuffers = pSEitf->nLastBuffers();
  
  mpTask->m_pDataStack->vPushInt(nLastBuffers);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.lastBuffers! -- store the number of last buffers for a fetch         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfLastBuffersStore(MP_Task mpTask)
{
  ENTRY("vSEitfLastBuffersStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.lastBuffers!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  i8 nLastBuffers;
  if ( ! mpTask->m_pDataStack->bPop(nLastBuffers) )
    THROW_INVALID_DSTACK("seitf.lastBuffers!"_Stz);
  
  pSEitf->vLastBuffers(nLastBuffers);
  
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
/* seitf.planner@ -- fetch planner from the SE interface                     */
/*----------------------------------------------------------------------------*/
void Task::vSEitfPlannerFetch(MP_Task mpTask)
{
  ENTRY("vSEitfPlannerFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.planner@"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushObject(pSEitf->m_pPlanner);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.planner! -- store partition in the SE interface                      */
/*----------------------------------------------------------------------------*/
void Task::vSEitfPlannerStore(MP_Task mpTask)
{
  ENTRY("vSEitfPlannerStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.planner!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrPlanner;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("seitf.partition!"_Stz);
  
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  ASSERT(CN_PLANNER == pPlanner->eClass());
  
  pSEitf->m_pPlanner = pPlanner;
  
  // if we have or phrases we'll need a RowOnce object
  if (pPlanner->nPhrases() > 1)
    pSEitf->vCreateRowOnce();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.rowsInvalid! -- store rows invalid in SE interface                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfRowsInvalidStore(MP_Task mpTask)
{
  ENTRY("vSEitfRowsInvalid"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.rowsInvalid!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  i8 iRowsInvalid;
  if ( ! mpTask->m_pDataStack->bPop(iRowsInvalid) )
    THROW_INVALID_DSTACK("seitf.rowsInvalid!"_Stz);
  
  pSEitf->m_iRowsInvalid = iRowsInvalid;
  
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
/* seitf.rowsValid! -- store rows valid in SE interface                       */
/*----------------------------------------------------------------------------*/
void Task::vSEitfRowsValidStore(MP_Task mpTask)
{
  ENTRY("vSEitfRowsVvalid"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.rowsValid!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  i8 iRowsValid;
  if ( ! mpTask->m_pDataStack->bPop(iRowsValid) )
    THROW_INVALID_DSTACK("seitf.rowsInvalid!"_Stz);
  
  pSEitf->m_iRowsValid = iRowsValid;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.slot@ -- fetch slot of SE interface                                  */
/*----------------------------------------------------------------------------*/
void Task::vSEitfSlotFetch(MP_Task mpTask)
{
  ENTRY("vSEitfSlotFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.slot@"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushInt(pSEitf->iSlot());
  
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

/*----------------------------------------------------------------------------*/
/* seitf.tx@ -- fetch tx of SE interface                                      */
/*----------------------------------------------------------------------------*/
void Task::vSEitfTxFetch(MP_Task mpTask)
{
  ENTRY("vSEitfTxFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.tx@"_Stz);
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  
  Tx* pTx = pSEitf->pTx();
  
  mpTask->m_pDataStack->vPushObject(pTx);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.tx! -- store tx of SE interface                                      */
/*----------------------------------------------------------------------------*/
void Task::vSEitfTxStore(MP_Task mpTask)
{
  ENTRY("vSEitfTxStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.tx!"_Stz);
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("seitf.tx!"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  pSEitf->vTx(pTx);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
