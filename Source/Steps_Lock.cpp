/*============================================================================*/
/* Steps_Lock.cpp - lock step group                                           */
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

// lock: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Lock(void)
{
  vAddPrimitive("lock.dropRead"_Str,   &vLockDropRead,     false);
  vAddPrimitive("lock.dropWrite"_Str,  &vLockDropWrite,    false);
  vAddPrimitive("lock.grabRead"_Str,   &vLockGrabRead,     false);
  vAddPrimitive("lock.grabWrite"_Str,  &vLockGrabWrite,    false);
  vAddPrimitive("lock.init"_Str,       &vLockInit,         false);
}

/*----------------------------------------------------------------------------*/
/* lock.dropread -- drop read lock                                            */
/*----------------------------------------------------------------------------*/
void Task::vLockDropRead(NNP_Task nnpTask)
{
ENTRY("vLockDropRead"_Str);
  
  Object* pPtrLock;;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrLock) )
    THROW_INVALID_DSTACK("lock.dropread"_Str);
  
  Lock* pLock = _pLock(pPtrLock);
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());
  
  pLock->vDropRead();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.dropwrite -- drop write lock                                          */
/*----------------------------------------------------------------------------*/
void Task::vLockDropWrite(NNP_Task nnpTask)
{
ENTRY("vLockDropWrite"_Str);

  Object* pPtrLock;;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrLock) )
    THROW_INVALID_DSTACK("lock.dropwrite"_Str);
  
  Lock* pLock = _pLock(pPtrLock);
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());

  pLock->vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.grabread -- grab read lock                                            */
/*----------------------------------------------------------------------------*/
void Task::vLockGrabRead(NNP_Task nnpTask)
{
ENTRY("vLockGrabRead"_Str);

  Object* pPtrLock;;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrLock) )
    THROW_INVALID_DSTACK("lock.grabread"_Str);
  
  Lock* pLock = _pLock(pPtrLock);
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());

  pLock->vGrabRead();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.grabwrite -- grab write lock                                          */
/*----------------------------------------------------------------------------*/
void Task::vLockGrabWrite(NNP_Task nnpTask)
{
ENTRY("vLockGrabWrite"_Str);

  Object* pPtrLock;;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrLock) )
    THROW_INVALID_DSTACK("lock.grabwrite"_Str);
  
  Lock* pLock = _pLock(pPtrLock);
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());
  
  pLock->vGrabWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.init -- initialise lock                                               */
/*----------------------------------------------------------------------------*/
void Task::vLockInit(NNP_Task nnpTask)
{
ENTRY("vLockInit"_Str);
  
  Object* pPtrLock;;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrLock) )
    THROW_INVALID_DSTACK("lock.init"_Str);
  
  Lock* pLock = _pLock(pPtrLock);
  
  new(pLock) Lock;
  
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());
  
  Word* pLockWord = Word::pToWord(pLock);
  
  pLock->vInit((Str*)pLockWord->sName().s());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
