/*============================================================================*/
/* Steps_Lock.cpp - lock step group                                           */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Lock(void)
{
  vAddStep("lock.dropRead"_Stz,   &Step::vLockDropRead);
  vAddStep("lock.dropWrite"_Stz,  &Step::vLockDropWrite);
  vAddStep("lock.grabRead"_Stz,   &Step::vLockGrabRead);
  vAddStep("lock.grabWrite"_Stz,  &Step::vLockGrabWrite);
}

/*----------------------------------------------------------------------------*/
/* lock.dropread -- drop read lock                                            */
/*----------------------------------------------------------------------------*/
void Step::vLockDropRead(void)
{
  ENTRY();
  
  Lock* pLock = _pLock(m_pDataStack->pPopPtr(__func__));
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eGetClassNo());
  
  pLock->vDropRead();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.dropwrite -- drop write lock                                          */
/*----------------------------------------------------------------------------*/
void Step::vLockDropWrite(void)
{
  ENTRY();

  Lock* pLock = _pLock(m_pDataStack->pPopPtr(__func__));
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eGetClassNo());

  pLock->vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.grabread -- grab read lock                                            */
/*----------------------------------------------------------------------------*/
void Step::vLockGrabRead(void)
{
  ENTRY();

  Lock* pLock = _pLock(m_pDataStack->pPopPtr(__func__));
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eGetClassNo());

  pLock->vGrabRead();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.grabwrite -- grab write lock                                          */
/*----------------------------------------------------------------------------*/
void Step::vLockGrabWrite(void)
{
  ENTRY();

  Lock* pLock = _pLock(m_pDataStack->pPopPtr(__func__));
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eGetClassNo());
  
  pLock->vGrabWrite();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
