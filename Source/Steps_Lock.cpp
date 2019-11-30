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
void Step::vLockDropRead(void)
{
  ENTRY(); // ENTRY("vLockDropRead"_Stz);
  
  Object* pPtrLock;;
  if ( ! m_pDataStack->bPopPtr(pPtrLock) ) THROW_DSTACK();
  
  Lock* pLock = _pLock(pPtrLock);
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());
  
  pLock->vDropRead();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.dropwrite -- drop write lock                                          */
/*----------------------------------------------------------------------------*/
void Step::vLockDropWrite(void)
{
  ENTRY(); // ENTRY("vLockDropWrite"_Stz);

  Object* pPtrLock;;
  if ( ! m_pDataStack->bPopPtr(pPtrLock) ) THROW_DSTACK();
  
  Lock* pLock = _pLock(pPtrLock);
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());

  pLock->vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.grabread -- grab read lock                                            */
/*----------------------------------------------------------------------------*/
void Step::vLockGrabRead(void)
{
  ENTRY(); // ENTRY("vLockGrabRead"_Stz);

  Object* pPtrLock;;
  if ( ! m_pDataStack->bPopPtr(pPtrLock) ) THROW_DSTACK();
  
  Lock* pLock = _pLock(pPtrLock);
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());

  pLock->vGrabRead();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.grabwrite -- grab write lock                                          */
/*----------------------------------------------------------------------------*/
void Step::vLockGrabWrite(void)
{
  ENTRY(); // ENTRY("vLockGrabWrite"_Stz);

  Object* pPtrLock;;
  if ( ! m_pDataStack->bPopPtr(pPtrLock) ) THROW_DSTACK();
  
  Lock* pLock = _pLock(pPtrLock);
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());
  
  pLock->vGrabWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.init -- initialise lock                                               */
/*----------------------------------------------------------------------------*/
void Step::vLockInit(void)
{
  ENTRY(); // ENTRY("vLockInit"_Stz);
  
  Object* pPtrLock;;
  if ( ! m_pDataStack->bPopPtr(pPtrLock) ) THROW_DSTACK();
  
  Lock* pLock = _pLock(pPtrLock);
  
  new(pLock) Lock(LK_UNKNOWN,nullptr);
  
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());
  
  Word* pLockWord = Word::pToWord(pLock);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
