/*============================================================================*/
/* Steps_Lock.cpp - lock step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern NP_Task         gnpBootTask;

// lock: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Lock(void)
{
  vAddPrimitive("lock.dropRead"_Stz,   &vLockDropRead,     false);
  vAddPrimitive("lock.dropWrite"_Stz,  &vLockDropWrite,    false);
  vAddPrimitive("lock.grabRead"_Stz,   &vLockGrabRead,     false);
  vAddPrimitive("lock.grabWrite"_Stz,  &vLockGrabWrite,    false);
  vAddPrimitive("lock.init"_Stz,       &vLockInit,         false);
}

/*----------------------------------------------------------------------------*/
/* lock.dropread -- drop read lock                                            */
/*----------------------------------------------------------------------------*/
void Task::vLockDropRead(MP_Task mpTask)
{
ENTRY("vLockDropRead"_Stz);
  
  Object* pPtrLock;;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLock) )
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
void Task::vLockDropWrite(MP_Task mpTask)
{
ENTRY("vLockDropWrite"_Stz);

  Object* pPtrLock;;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLock) )
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
void Task::vLockGrabRead(MP_Task mpTask)
{
ENTRY("vLockGrabRead"_Stz);

  Object* pPtrLock;;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLock) )
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
void Task::vLockGrabWrite(MP_Task mpTask)
{
ENTRY("vLockGrabWrite"_Stz);

  Object* pPtrLock;;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLock) )
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
void Task::vLockInit(MP_Task mpTask)
{
ENTRY("vLockInit"_Stz);
  
  Object* pPtrLock;;
  if ( ! mpTask->m_pDataStack->bPop(pPtrLock) )
    THROW_INVALID_DSTACK("lock.init"_Stz);
  
  Lock* pLock = _pLock(pPtrLock);
  
  new(pLock) Lock;
  
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());
  
  Word* pLockWord = Word::pToWord(pLock);
  
  pLock->vInitLock(LK_UNKNOWN);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
