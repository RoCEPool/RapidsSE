/*============================================================================*/
/* Steps_Timer.cpp - timer step group                                         */
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

// timer: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Timer(void)
{
  vAddPrimitive("timer.delete"_Str,    &vTimerDelete,      false);
  vAddPrimitive("timer.init"_Str,      &vTimerInit,        false);
  vAddPrimitive("timer.microsecs"_Str, &vTimerMicrosecs,   false);
  vAddPrimitive("timer.millisecs"_Str, &vTimerMillisecs,   false);
  vAddPrimitive("timer.nanosecs"_Str,  &vTimerNanosecs,    false);
  vAddPrimitive("timer.new"_Str,       &vTimerNew,         false);
  vAddPrimitive("timer.reset"_Str,     &vTimerReset,       false);
  vAddPrimitive("timer.secs"_Str,      &vTimerSecs,        false);
  vAddPrimitive("timer.start"_Str,     &vTimerStart,       false);
  vAddPrimitive("timer.ticks"_Str,     &vTimerTicks,       false);
  vAddPrimitive("timer.update"_Str,    &vTimerUpdate,      false);
}

/*----------------------------------------------------------------------------*/
/* timer.delete -- delete timer                                               */
/*----------------------------------------------------------------------------*/
void Task::vTimerDelete(NNP_Task nnpTask)
{
ENTRY("vTimeDelete"_Str);
  
  Object* pPtrTimer;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.delete"_Str);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  Object* pTimerObject = reinterpret_cast<Object*>(pTimer);
  
  delete pTimer;
  
  Heap::vFree(pTimerObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.init -- init timer                                                   */
/*----------------------------------------------------------------------------*/
void Task::vTimerInit(NNP_Task nnpTask)
{
ENTRY("vTimerInit"_Str);
  
  Object* pPtrTimer;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.init"_Str);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  new(pTimer) Timer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.microsecs -- get elapsed duration in microseconds                    */
/*----------------------------------------------------------------------------*/
void Task::vTimerMicrosecs(NNP_Task nnpTask)
{
ENTRY("vTimerMicrosecs"_Str);
  
  Object* pPtrTimer;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.microsecs"_Str);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iMicroseconds = pTimer->iMicroseconds();
  
  nnpTask->m_pDataStack->vPush(iMicroseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.millisecs -- get elapsed duration in milliseconds                    */
/*----------------------------------------------------------------------------*/
void Task::vTimerMillisecs(NNP_Task nnpTask)
{
ENTRY("vTimerMillisecs"_Str);
  
  Object* pPtrTimer;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.millisecs"_Str);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iMilliseconds = pTimer->iMilliseconds();
  
  nnpTask->m_pDataStack->vPush(iMilliseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.nanosecs -- get elapsed duration in nanoseconds                      */
/*----------------------------------------------------------------------------*/
void Task::vTimerNanosecs(NNP_Task nnpTask)
{
ENTRY("vTimerNanosecs"_Str);
  
  Object* pPtrTimer;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.nanosecs"_Str);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iNanoseconds = pTimer->iNanoseconds();
  
  nnpTask->m_pDataStack->vPush(iNanoseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.new -- new timer                                                     */
/*----------------------------------------------------------------------------*/
void Task::vTimerNew(NNP_Task nnpTask)
{
ENTRY("vTimerNew"_Str);
  
  void* pObject = Heap::pAllocate(CN_ROWSTRAND);

  Timer* pTimer = _pTimer(Heap::pAllocate(CN_TIMER));

  new(pTimer) Timer;
 
  ASSERT(CN_TIMER == pTimer->eClass());
  
  Object* pPtrTimer = pTimer;
  
  nnpTask->m_pDataStack->vPushObject(pPtrTimer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.reset -- reset timer                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTimerReset(NNP_Task nnpTask)
{
ENTRY("vTimerReset"_Str);
  
  Object* pPtrTimer;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.reset"_Str);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vReset();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.secs -- get elapsed duration in seconds                              */
/*----------------------------------------------------------------------------*/
void Task::vTimerSecs(NNP_Task nnpTask)
{
ENTRY("vTimerSecs"_Str);
  
  Object* pPtrTimer;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.secs"_Str);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iSeconds = pTimer->iSeconds();
  
  nnpTask->m_pDataStack->vPush(iSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.start -- start timer                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTimerStart(NNP_Task nnpTask)
{
ENTRY("vTimerStart"_Str);
  
  Object* pPtrTimer;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.Start"_Str);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vStart();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.ticks -- get elapsed ticks                                           */
/*----------------------------------------------------------------------------*/
void Task::vTimerTicks(NNP_Task nnpTask)
{
ENTRY("vTimerTicks"_Str);
  
  Object* pPtrTimer;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.ticks"_Str);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iTicks = pTimer->iTicks();
  
  nnpTask->m_pDataStack->vPush(iTicks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.update -- update timer                                               */
/*----------------------------------------------------------------------------*/
void Task::vTimerUpdate(NNP_Task nnpTask)
{
ENTRY("vTimerUpdate"_Str);
  
  Object* pPtrTimer;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.update"_Str);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vUpdate();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
