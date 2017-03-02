/*============================================================================*/
/* Steps_SEitf.cpp - SEitf step group                                         */
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

// seitf: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_SEitf(void)
{
  vAddPrimitive("seitf.count",         &vSEitfCount,         false);
  vAddPrimitive("seitf.delete",        &vSEitfDelete,        false);
  vAddPrimitive("seitf.dicStack",      &vSEitfDicStack,      false);
  vAddPrimitive("seitf.dump",          &vSEitfDump,          false);
  vAddPrimitive("seitf.indexRef",      &vSEitfIndexRef,      false);
  vAddPrimitive("seitf.pid",           &vSEitfPid,           false);
  vAddPrimitive("seitf.req@",          &vSEitfReqFetch,      false);
  vAddPrimitive("seitf.req!",          &vSEitfReqStore,      false);
  vAddPrimitive("seitf.rsp@",          &vSEitfRspFetch,      false);
  vAddPrimitive("seitf.rsp!",          &vSEitfRspStore,      false);
  vAddPrimitive("seitf.responseQueue", &vSEitfResponseQueue, false);
  vAddPrimitive("seitf.table@",        &vSEitfTableFetch,    false);
  vAddPrimitive("seitf.tableRef",      &vSEitfTableRef,      false);
  vAddPrimitive("seitf.tableDeref",    &vSEitfTableDeref,    false);
}

/*----------------------------------------------------------------------------*/
/* seitf.count -- get count of SE interface's                                 */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCount(NNP_Task nnpTask)
{
  ENTRY("vSEitfCount");
  
  Object* pPtrSEitf;
  
  nnpTask->m_pDataStack->vPush(gpSEitfList->iCount());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.delete -- delete SE interface                                        */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDelete(NNP_Task nnpTask)
{
ENTRY("vSEitfDelete");
  
  Object* pPtrSEitf;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.delete");

  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  delete pSEitf;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.dicStack -- get SEitf dic stack                                      */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDicStack(NNP_Task nnpTask)
{
  ENTRY("vSEitfDicStack");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.dicStack");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  nnpTask->m_pDataStack->vPush(pSEitf->pPathStack());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.dump -- dump of SE interface                                         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDump(NNP_Task nnpTask)
{
ENTRY("vSEitfDump");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.dump");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  pSEitf->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.indexRef -- store index in SEitf and inc reference                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfIndexRef(NNP_Task nnpTask)
{
  ENTRY("vSEitfIndexRef");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.indexRef");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrIndex;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("seitf.indexRef");
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_TABLE == pIndex->eClass());
  
  //pIndex->vRefInc();
  
  //pSEitf->vCurrentIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.pid -- get pid of SE interface                                       */
/*----------------------------------------------------------------------------*/
void Task::vSEitfPid(NNP_Task nnpTask)
{
  ENTRY("vSEitfPid");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.pid");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  nnpTask->m_pDataStack->vPush(pSEitf->m_iPid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.req@ -- fetch SEitf request queue                                    */
/*----------------------------------------------------------------------------*/
void Task::vSEitfReqFetch(NNP_Task nnpTask)
{
ENTRY("vSEitfReqFetch");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.req@");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrQueue = pSEitf->pRequestQueue();
  
  nnpTask->m_pDataStack->vPush(pPtrQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.req! -- store SEitf request queue                                    */
/*----------------------------------------------------------------------------*/
void Task::vSEitfReqStore(NNP_Task nnpTask)
{
ENTRY("vSEitfReqStore");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.req!");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrQueue;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrQueue) )
    THROW_INVALID_DSTACK("seitf.req!");
  
  Queue* pQueue = _pQueue(pPtrQueue);
  ASSERT(CN_QUEUE == pQueue->eClass());
  
  pSEitf->vRequestQueue(pQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.rsp@ -- fetch SEitf response queue                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfRspFetch(NNP_Task nnpTask)
{
ENTRY("vSEitfRspFetch");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.rsp@");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrQueue = pSEitf->pResponseQueue();
  
  nnpTask->m_pDataStack->vPush(pPtrQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.rsp! -- store SEitf response queue                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfRspStore(NNP_Task nnpTask)
{
ENTRY("vSEitfRspStore");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.rsp!");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrQueue;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrQueue) )
    THROW_INVALID_DSTACK("seitf.rsp!");
  
  Queue* pQueue = _pQueue(pPtrQueue);
  ASSERT(CN_QUEUE == pQueue->eClass());
  
  pSEitf->vResponseQueue(pQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.responseQueue -- get SEitf response queue                            */
/*----------------------------------------------------------------------------*/
void Task::vSEitfResponseQueue(NNP_Task nnpTask)
{
  ENTRY("vSEitfeResponseQueue");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.responseQueue");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  nnpTask->m_pDataStack->vPush(pSEitf->pResponseQueue());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.table@ -- fetch table from SEitf                                     */
/*----------------------------------------------------------------------------*/
void Task::vSEitfTableFetch(NNP_Task nnpTask)
{
  ENTRY("vSEitfTableFetch");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.rsp!");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  nnpTask->m_pDataStack->vPush(pSEitf->pCurrentTable());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.tableDeref -- store table in SEitf and inc reference                 */
/*----------------------------------------------------------------------------*/
void Task::vSEitfTableDeref(NNP_Task nnpTask)
{
  ENTRY("vSEitfRspStore");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.tableDeref");
  
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
void Task::vSEitfTableRef(NNP_Task nnpTask)
{
  ENTRY("vSEitfTableRef");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.tableRef");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrTable;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("seitf.tableRef");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vRefInc();
  
  pSEitf->vCurrentTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
