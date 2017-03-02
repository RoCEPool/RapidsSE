/*============================================================================*/
/* Steps_IndexDefn.cpp - index definition step group                          */
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

// indexDefn: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_IndexDefn(void)
{
  vAddPrimitive("indexDefn.init",        &vIndexDefnInit,       false);
}

/*----------------------------------------------------------------------------*/
/* indexDefn.init -- initialize the indexDefn                                */
/*----------------------------------------------------------------------------*/
void Task::vIndexDefnInit(NNP_Task nnpTask)
{
  ENTRY("vIndexDefnInit");
  Object* pPtrIndexDefn = nullptr;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndexDefn) )
    THROW_INVALID_DSTACK("indexDefn::init");
  
  IndexDefn* pIndexDefn = _pIndexDefn(pPtrIndexDefn);
  ASSERT(CN_INDEXDEFN == pPtrIndexDefn->eClass());
  
  new(pIndexDefn) IndexDefn();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
