/*============================================================================*/
/* Steps_Semaphore.cpp - semaphore step group                                 */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Semaphore(void)
{
  vAddStep("Semaphore.init"_Stz,    &Step::vSemaphoreInit);
}

/*----------------------------------------------------------------------------*/
/* Semaphore.init -- init of Sempahore on data stack                          */
/*----------------------------------------------------------------------------*/
void Step::vSemaphoreInit(MP_Task mpTask)
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
