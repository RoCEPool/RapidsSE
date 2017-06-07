/*============================================================================*/
/* Steps_AccessPath.cpp - access path step group                              */
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

// accessPath: /////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_AccessPath(void)
{
  vAddPrimitive("accessPath.delete"_Str, &vAccessPathDelete, false);
  vAddPrimitive("accessPath::new"_Str,   &vAccessPathNew,    false);
  vAddPrimitive("accessPath.set"_Str,    &vAccessPathSet, false);
}

/*----------------------------------------------------------------------------*/
/* accessPath.delete -- delete an access path ( accessPath -- )               */
/*----------------------------------------------------------------------------*/
void Task::vAccessPathDelete(NNP_Task nnpTask)
{
ENTRY("vAccessPathDelete"_Str);
  
  Object* pPtrAccessPath;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrAccessPath) )
    THROW_INVALID_DSTACK("accessPath.delete"_Str);
  
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);

  delete pAccessPath;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath::New -- new a file ( -- file )                                  */
/*----------------------------------------------------------------------------*/
void Task::vAccessPathNew(NNP_Task nnpTask)
{
ENTRY("vAccessPathNew"_Str);
  
  AccessPath* pAccessPath = new AccessPath;
  
  Object* pPtrAccessPath;
  pPtrAccessPath = (Object*)pAccessPath;
  
  nnpTask->m_pDataStack->vPushObject(pPtrAccessPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath.set -- set access path ( seitf accessPath -- )                  */
/*----------------------------------------------------------------------------*/
void Task::vAccessPathSet(NNP_Task nnpTask)
{
  ENTRY("vAccessPathSet"_Str);
  
  Object* pPtrAccessPath;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrAccessPath) )
    THROW_INVALID_DSTACK("accessPath.set"_Str);
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("accessPath.set"_Str);
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  
  pAccessPath->vInit(pSEitf);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
