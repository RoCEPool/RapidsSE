/*============================================================================*/
/* Steps_Lock.cpp - lock step group                                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Lock(void)
{
  vAddStep("lock.dropRead"_Stz,   &Step::vLockDropRead);
  vAddStep("lock.dropWrite"_Stz,  &Step::vLockDropWrite);
  vAddStep("lock.grabRead"_Stz,   &Step::vLockGrabRead);
  vAddStep("lock.grabWrite"_Stz,  &Step::vLockGrabWrite);
  vAddStep("lock.init"_Stz,       &Step::vLockInit);
}

/*----------------------------------------------------------------------------*/
/* lock.dropread -- drop read lock                                            */
/*----------------------------------------------------------------------------*/
void Step::vLockDropRead(MP_Task mpTask, Forth* pForth)
{
ENTRY("vLockDropRead"_Stz);
  
  Object* pPtrLock;;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrLock) )
    THROW_INVALID_DSTACK("lock.dropread"_Stz);
  
  Lock* pLock = _pLock(pPtrLock);
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());
  
  pLock->vDropRead();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.dropwrite -- drop write lock                                          */
/*----------------------------------------------------------------------------*/
void Step::vLockDropWrite(MP_Task mpTask, Forth* pForth)
{
ENTRY("vLockDropWrite"_Stz);

  Object* pPtrLock;;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrLock) )
    THROW_INVALID_DSTACK("lock.dropwrite"_Stz);
  
  Lock* pLock = _pLock(pPtrLock);
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());

  pLock->vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.grabread -- grab read lock                                            */
/*----------------------------------------------------------------------------*/
void Step::vLockGrabRead(MP_Task mpTask, Forth* pForth)
{
ENTRY("vLockGrabRead"_Stz);

  Object* pPtrLock;;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrLock) )
    THROW_INVALID_DSTACK("lock.grabread"_Stz);
  
  Lock* pLock = _pLock(pPtrLock);
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());

  pLock->vGrabRead();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.grabwrite -- grab write lock                                          */
/*----------------------------------------------------------------------------*/
void Step::vLockGrabWrite(MP_Task mpTask, Forth* pForth)
{
ENTRY("vLockGrabWrite"_Stz);

  Object* pPtrLock;;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrLock) )
    THROW_INVALID_DSTACK("lock.grabwrite"_Stz);
  
  Lock* pLock = _pLock(pPtrLock);
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());
  
  pLock->vGrabWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.init -- initialise lock                                               */
/*----------------------------------------------------------------------------*/
void Step::vLockInit(MP_Task mpTask, Forth* pForth)
{
ENTRY("vLockInit"_Stz);
  
  Object* pPtrLock;;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrLock) )
    THROW_INVALID_DSTACK("lock.init"_Stz);
  
  Lock* pLock = _pLock(pPtrLock);
  
  new(pLock) Lock(LK_UNKNOWN,nullptr,nullptr);
  
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());
  
  Word* pLockWord = Word::pToWord(pLock);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
