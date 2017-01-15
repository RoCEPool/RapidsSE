/*============================================================================*/
/* Task_TxCollection.cpp - task tx collection group                           */
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

// txCollection: ///////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TxCollection(void)
{
  vAddPrimitive("txCollection.dump", &vTxCollectionDump, false);
  vAddPrimitive("txCollection.init", &vTxCollectionInit, false);
}

/*----------------------------------------------------------------------------*/
/* txCollection.dump -- dump of txCollection on data stack                    */
/*----------------------------------------------------------------------------*/
void Task::vTxCollectionDump(NNP_Task nnpTask)
{
ENTRY("vTxCollectionDump");
  
  Object* pPtrTxCollection;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTxCollection) )
    THROW_INVALID_DSTACK("txCollection.dump");
  
  TxCollection* pTxCollection = _pTxCollection(pPtrTxCollection);
  ASSERT(CN_TXCOLLECTION == pTxCollection->eClass());
  
  pTxCollection->vDump("gpTxCollection:");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCollection.init -- init of txCollection on data stack                    */
/*----------------------------------------------------------------------------*/
void Task::vTxCollectionInit(NNP_Task nnpTask)
{
ENTRY("vTxCollectionInit");
  
  Object* pPtrTxCollection;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTxCollection) )
    THROW_INVALID_DSTACK("txCollection.init");
  
  TxCollection* pTxCollection = _pTxCollection(pPtrTxCollection);
  ASSERT(CN_TXCOLLECTION == pTxCollection->eClass());
  
  new(pTxCollection) TxCollection;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
