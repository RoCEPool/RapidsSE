/*============================================================================*/
/* Steps_Semaphore.cpp - semaphore step group                                 */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Dic*            gpMainDic;

extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// Semaphore: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Semaphore(void)
{
  vAddPrimitive("Semaphore.init"_Stz,    &vSemaphoreInit,        false);
}

/*----------------------------------------------------------------------------*/
/* Semaphore.init -- init of Sempahore on data stack                          */
/*----------------------------------------------------------------------------*/
void Task::vSemaphoreInit(MP_Task mpTask)
{
ENTRY("vSemaphoreInit"_Stz);
  
  Object* pPtrSemaphore;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSemaphore) )
    THROW_INVALID_DSTACK("Semaphore.init"_Stz);
  
  Semaphore* pSemaphore = _pSemaphore(pPtrSemaphore);
  
  new(pSemaphore) Semaphore;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
