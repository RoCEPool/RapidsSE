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

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// timer: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Timer(void)
{
  vAddPrimitive("timer.delete"_Stz,    &vTimerDelete,      false);
  vAddPrimitive("timer.init"_Stz,      &vTimerInit,        false);
  vAddPrimitive("timer.microsecs"_Stz, &vTimerMicrosecs,   false);
  vAddPrimitive("timer.millisecs"_Stz, &vTimerMillisecs,   false);
  vAddPrimitive("timer.nanosecs"_Stz,  &vTimerNanosecs,    false);
  vAddPrimitive("timer::new"_Stz,       &vTimerNew,         false);
  vAddPrimitive("timer.reset"_Stz,     &vTimerReset,       false);
  vAddPrimitive("timer.secs"_Stz,      &vTimerSecs,        false);
  vAddPrimitive("timer.start"_Stz,     &vTimerStart,       false);
  vAddPrimitive("timer.ticks"_Stz,     &vTimerTicks,       false);
  vAddPrimitive("timer.update"_Stz,    &vTimerUpdate,      false);
}

/*----------------------------------------------------------------------------*/
/* timer.delete -- delete timer                                               */
/*----------------------------------------------------------------------------*/
void Task::vTimerDelete(MP_Task mpTask)
{
ENTRY("vTimeDelete"_Stz);
  
  Object* pPtrTimer;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.delete"_Stz);
  
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
void Task::vTimerInit(MP_Task mpTask)
{
ENTRY("vTimerInit"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.init"_Stz);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  new(pTimer) Timer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.microsecs -- get elapsed duration in microseconds                    */
/*----------------------------------------------------------------------------*/
void Task::vTimerMicrosecs(MP_Task mpTask)
{
ENTRY("vTimerMicrosecs"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.microsecs"_Stz);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iMicroseconds = pTimer->iMicroseconds();
  
  mpTask->m_pDataStack->vPushInt(iMicroseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.millisecs -- get elapsed duration in milliseconds                    */
/*----------------------------------------------------------------------------*/
void Task::vTimerMillisecs(MP_Task mpTask)
{
ENTRY("vTimerMillisecs"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.millisecs"_Stz);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iMilliseconds = pTimer->iMilliseconds();
  
  mpTask->m_pDataStack->vPushInt(iMilliseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.nanosecs -- get elapsed duration in nanoseconds                      */
/*----------------------------------------------------------------------------*/
void Task::vTimerNanosecs(MP_Task mpTask)
{
ENTRY("vTimerNanosecs"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.nanosecs"_Stz);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iNanoseconds = pTimer->iNanoseconds();
  
  mpTask->m_pDataStack->vPushInt(iNanoseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer::new -- new timer                                                     */
/*----------------------------------------------------------------------------*/
void Task::vTimerNew(MP_Task mpTask)
{
ENTRY("vTimerNew"_Stz);

  Timer* pTimer = _pTimer(Heap::pAllocate(CN_TIMER));

  new(pTimer) Timer;
 
  ASSERT(CN_TIMER == pTimer->eClass());
  
  Object* pPtrTimer = pTimer;
  
  mpTask->m_pDataStack->vPushObject(pPtrTimer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.reset -- reset timer                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTimerReset(MP_Task mpTask)
{
ENTRY("vTimerReset"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.reset"_Stz);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vReset();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.secs -- get elapsed duration in seconds                              */
/*----------------------------------------------------------------------------*/
void Task::vTimerSecs(MP_Task mpTask)
{
ENTRY("vTimerSecs"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.secs"_Stz);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iSeconds = pTimer->iSeconds();
  
  mpTask->m_pDataStack->vPushInt(iSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.start -- start timer                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTimerStart(MP_Task mpTask)
{
ENTRY("vTimerStart"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.Start"_Stz);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vStart();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.ticks -- get elapsed ticks                                           */
/*----------------------------------------------------------------------------*/
void Task::vTimerTicks(MP_Task mpTask)
{
ENTRY("vTimerTicks"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.ticks"_Stz);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iTicks = pTimer->iTicks();
  
  mpTask->m_pDataStack->vPushInt(iTicks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.update -- update timer                                               */
/*----------------------------------------------------------------------------*/
void Task::vTimerUpdate(MP_Task mpTask)
{
ENTRY("vTimerUpdate"_Stz);
  
  Object* pPtrTimer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTimer) )
    THROW_INVALID_DSTACK("timer.update"_Stz);
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vUpdate();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
