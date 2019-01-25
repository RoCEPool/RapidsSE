/*============================================================================*/
/* Synch.cpp -- Threading independent synchronisation                         */
/*============================================================================*/

#include "System.h"

#include "Asm.h"
//const u8 C_WRITELOCK = 0x8000000000000000;

/*----------------------------------------------------------------------------*/
/* drop read lock                                                             */
/*----------------------------------------------------------------------------*/
u8 Synch::iLockDropRead(u8* pWord)
{
#if defined(__APPLE__) || defined(__LINUX__)
  //u8 uResult = iDropRead(pWord);

  bool b;
  
  do {
    u8 uCurrent = *pWord;
    
    // must have a read lock
    assert(uCurrent && uCurrent != C_WRITELOCK);
    
    u8 uRemoveReadLock = uCurrent - 1;
  
    b = Atomic::bTestAndSet(pWord,uCurrent,uRemoveReadLock);
  
  } while ( ! b );
    
  u8 uResult = 1;
  
#else
  u8 uResult = iDropRead(pWord);
#endif
  
  return uResult;
}

/*----------------------------------------------------------------------------*/
/* grab read lock                                                             */
/*----------------------------------------------------------------------------*/
u8 Synch::iLockGrabRead(u8* pWord)
{
#if defined(__APPLE__) || defined(__LINUX__)
  
  //u8 uResult = iGrabRead(pWord);
  
  u8 uCurrent = *pWord;
  if (uCurrent >= C_WRITELOCK)
    return 0;
  
  u8 uAddReadLock = uCurrent + 1;
  
  bool b = Atomic::bTestAndSet(pWord,uCurrent,uAddReadLock);
  
  u8 uResult = b ? 1 : 0;
  
#else
  u8 uResult = iGrabRead(pWord);
#endif
  
  return uResult;
}

/*----------------------------------------------------------------------------*/
/* drop write lock                                                            */
/*----------------------------------------------------------------------------*/
u8 Synch::iLockDropWrite(u8* pWord)
{
#if defined(__APPLE__) || defined(__LINUX__)
  //u8 uResult = iDropWrite(pWord);
  
  u8 uCurrent = *pWord;

  assert (uCurrent == C_WRITELOCK);

  bool b = Atomic::bTestAndSet(pWord,uCurrent,0);
  
  u8 uResult = 1;
#else
  u8 uResult = iDropWrite(pWord);
#endif
  
  return uResult;
}

/*----------------------------------------------------------------------------*/
/* grab write lock                                                            */
/*----------------------------------------------------------------------------*/
u8 Synch::iLockGrabWrite(u8* pWord)
{
#if defined(__APPLE__) || defined(__LINUX__)
  //u8 uResult = iGrabWrite(pWord);

  u8 uCurrent = *pWord;
  
  if (uCurrent >= C_WRITELOCK)
    return 0;
  
  u8 uAddWriteLock = uCurrent + C_WRITELOCK;
  
  bool b = Atomic::bTestAndSet(pWord,uCurrent,uAddWriteLock);
  if ( ! b )
    return 0;
  
  uCurrent = C_WRITELOCK;
  
  while (! Atomic::bTestAndSet(pWord,uCurrent,uCurrent))
    {;}
  
  u8 uResult = 1;
  
#else
  u8 uResult = iGrabWrite(pWord);
#endif
  
  return uResult;
}

/*----------------------------------------------------------------------------*/
/* is lock actually locked                                                    */
/*----------------------------------------------------------------------------*/
u8 Synch::iLockIsLocked(u8* pWord)
{
#if defined(__APPLE__) || defined(__LINUX__)
  u8 uResult = *pWord ? 1 : 0;
#else
  u8 uResult = iIsLocked(pWord);
#endif
  
  return uResult;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
