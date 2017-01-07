/*============================================================================*/
/* Task_TxMap.cpp - task tx map group                                         */
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

// txMap: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TxMap(void)
{
  vAddPrimitive("txMap.init",      &vTxMapInit,        false);
}

/*----------------------------------------------------------------------------*/
/* txMap.init -- init of txmap on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vTxMapInit(NNP_Task nnpTask)
{
ENTRY("vTxMapInit");
  
  Object* pPtrTxMap;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxMap) )
    THROW_INVALID_DSTACK("txMap.init");
  
  TxMap* pTxMap = _pTxMap(pPtrTxMap);
  ASSERT(CN_TXMAP == pTxMap->eClass());
  
  new(pTxMap) TxMap;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
