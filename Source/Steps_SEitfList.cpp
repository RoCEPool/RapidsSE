/*============================================================================*/
/* Steps_SEitfList.cpp - SEitf list step group                                */
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

// seitflist: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_SEitfList(void)
{
  vAddPrimitive("seitfList.dump"_Str,  &vSEitfListDump,    false);
  vAddPrimitive("seitfList.init"_Str,  &vSEitfListInit,    false);
}

/*----------------------------------------------------------------------------*/
/* seitflist.dump -- dump of seitfList on data stack                          */
/*----------------------------------------------------------------------------*/
void Task::vSEitfListDump(NNP_Task nnpTask)
{
ENTRY("vSEitfListDump"_Str);
  
  Object* pPtrSEitfList;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitfList) )
    THROW_INVALID_DSTACK("seitflist.dump"_Str);
  
  SEitfList* pSEitfList = _pSEitfList(pPtrSEitfList);
  ASSERT(CN_SEITFLIST == pSEitfList->eClass());
  
  pSEitfList->vContentDump("SEitfList:"_Str, aSingleTab());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitflist.init -- init of seitfList on data stack                          */
/*----------------------------------------------------------------------------*/
void Task::vSEitfListInit(NNP_Task nnpTask)
{
ENTRY("vSEitfListInit"_Str);
  
  Object* pPtrSEitfList;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitfList) )
    THROW_INVALID_DSTACK("seitflist.init"_Str);
  
  SEitfList* pSEitfList = _pSEitfList(pPtrSEitfList);
  ASSERT(CN_SEITFLIST == pSEitfList->eClass());
  
  new(pSEitfList) SEitfList;
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
