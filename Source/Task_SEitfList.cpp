/*============================================================================*/
/* Task_SEitfList.cpp - task SEitf list group                                 */
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

// seitflist: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_SEitfList(void)
{
  vAddPrimitive("seitfList.dump",  &vSEitfListDump,    false);
  vAddPrimitive("seitfList.init",  &vSEitfListInit,    false);
}

/*----------------------------------------------------------------------------*/
/* seitflist.dump -- dump of seitfList on param stack                         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfListDump(NNP_Task nnpTask)
{
ENTRY("vSEitfListDump");
  
  Object* pPtrSEitfList;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitfList) )
    THROW_INVALID_DSTACK("seitflist.dump");
  
  SEitfList* pSEitfList = _pSEitfList(pPtrSEitfList);
  ASSERT(CN_SEITFLIST == pSEitfList->eClass());
  
  pSEitfList->vDump("SEitfList:", pSingleTab());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitflist.init -- init of seitfList on param stack                         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfListInit(NNP_Task nnpTask)
{
ENTRY("vSEitfListInit");
  
  Object* pPtrSEitfList;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitfList) )
    THROW_INVALID_DSTACK("seitflist.init");
  
  SEitfList* pSEitfList = _pSEitfList(pPtrSEitfList);
  ASSERT(CN_SEITFLIST == pSEitfList->eClass());
  
  new(pSEitfList) SEitfList;
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
