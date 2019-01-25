/*============================================================================*/
/* Synch.cpp -- Threading independent synchronisation                         */
/*============================================================================*/

#include "System.h"

#include "Asm.h"
//const u8 C_WRITELOCK = 0x8000000000000000;

/*----------------------------------------------------------------------------*/
/* atomically add word                                                        */
/*----------------------------------------------------------------------------*/
u8 Synch::uAtomicAdd(i8* pWord, i8 iValue)
{
#ifdef __APPLE__
  u8 uResult = OSAtomicAdd64(iValue, pWord);
  
#elif __LINUX__
  u8 uResult = __sync_add_and_fetch((u8*)pWord, iValue);
  
#else
#error "Unknown Operating System"
#endif
  
  return uResult;
}

/*----------------------------------------------------------------------------*/
/* atomically decrement word                                                  */
/*----------------------------------------------------------------------------*/
u8 Synch::uAtomicDec(i8* pWord)
{
#ifdef __APPLE__
  u8 uResult = OSAtomicDecrement64(pWord);
  
#elif __LINUX__
  u8 uResult = __sync_sub_and_fetch((u8*)pWord, 1);
  
#else
  u8 uResult = iAtomicDec(pWord);
  
#endif
  
  return uResult;
}

/*----------------------------------------------------------------------------*/
/* atomically increment word                                                  */
/*----------------------------------------------------------------------------*/
u8 Synch::uAtomicInc(i8* pWord)
{
#ifdef __APPLE__
  u8 uResult = OSAtomicIncrement64(pWord);
  
#elif __LINUX__
  u8 uResult = __sync_add_and_fetch((u8*)pWord, 1);
  
#else
  u8 uResult = iAtomicInc(pWord);
  
#endif
  
  return uResult;
}

/*----------------------------------------------------------------------------*/
/* atomic test and set                                                        */
/*----------------------------------------------------------------------------*/
bool Synch::bTestAndSet(u8* pWord, u8 uIf, u8 uThen)
{
  
#ifdef __APPLE__
  u8 uResult = OSAtomicCompareAndSwap64((i8)uIf,(i8)uThen,(i8*)pWord);
  
#elif __LINUX__
  bool uResult = __sync_bool_compare_and_swap(pWord, uIf, uThen);
  
#else
  u8 uResult = iTestSet(pWord, uIf, uThen);
  
#endif
  
  return uResult;
}

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
  
    b = bTestAndSet(pWord,uCurrent,uRemoveReadLock);
  
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
  
  bool b = bTestAndSet(pWord,uCurrent,uAddReadLock);
  
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

  bool b = bTestAndSet(pWord,uCurrent,0);
  
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
  
  bool b = bTestAndSet(pWord,uCurrent,uAddWriteLock);
  if ( ! b )
    return 0;
  
  uCurrent = C_WRITELOCK;
  
  while (!bTestAndSet(pWord,uCurrent,uCurrent))
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
