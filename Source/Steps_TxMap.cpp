/*============================================================================*/
/* Steps_TxMap.cpp - tx map step group                                        */
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

// txMap: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TxMap(void)
{
  vAddPrimitive("txMap.init"_Stz,      &vTxMapInit,        false);
}

/*----------------------------------------------------------------------------*/
/* txMap.init -- init of txmap on data stack                                  */
/*----------------------------------------------------------------------------*/
void Task::vTxMapInit(MP_Task mpTask)
{
ENTRY("vTxMapInit"_Stz);
  
  Object* pPtrTxMap;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxMap) )
    THROW_INVALID_DSTACK("txMap.init"_Stz);
  
  TxMap* pTxMap = _pTxMap(pPtrTxMap);
  ASSERT(CN_TXMAP == pTxMap->eClass());
  
  new(pTxMap) TxMap;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
