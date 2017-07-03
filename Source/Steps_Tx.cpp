/*============================================================================*/
/* Steps_Tx.cpp - tx step grou                                                */
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

// tx: /////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Tx(void)
{
  vAddPrimitive("tx.dump"_Str,         &vTxDump,           false);
}

/*----------------------------------------------------------------------------*/
/* tx.dump -- delete tx                                                       */
/*----------------------------------------------------------------------------*/
void Task::vTxDump(MP_Task mpTask)
{
ENTRY("vTxDump"_Str);
  
  Object* pPtrTx;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("tx.delete"_Str);
  
  Tx* pTx = _pTx(pPtrTx);
  ASSERT(CN_TX == pTx->eClass());

  pTx->vContentDump();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
