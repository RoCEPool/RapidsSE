/*============================================================================*/
/* Task_Tx.cpp - task tx group                                                */
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

// tx: /////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Tx(void)
{
  vAddPrimitive("tx.dump",         &vTxDump,           false);
}

/*----------------------------------------------------------------------------*/
/* tx.dump -- delete tx                                                       */
/*----------------------------------------------------------------------------*/
void Task::vTxDump(NNP_Task nnpTask)
{
ENTRY("vTxDump");
  
  Object* pPtrTx;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("tx.delete");
  
  Tx* pTx = _pTx(pPtrTx);
  ASSERT(CN_TX == pTx->eClass());

  pTx->vDump();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
