/*============================================================================*/
/* Synch.cpp -- platform independent synchronisation                          */
/*============================================================================*/

#include "Common.h"

//const u8 C_WRITELOCK = 0x8000000000000000;

/*----------------------------------------------------------------------------*/
/* atomically decrement word                                                  */
/*----------------------------------------------------------------------------*/
u8 Synch::uAtomicDec(i8* pWord)
{
#ifdef __APPLE__
  u8 uResult = OSAtomicDecrement64(pWord);
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
#else
  u8 uResult = iTestSet(pWord, uIf, uThen);
#endif
  
  return uResult;
}

/*----------------------------------------------------------------------------*/
/* get ticks                                                                  */
/*----------------------------------------------------------------------------*/
i8 Synch::iTicks(void)
{
#ifdef __APPLE__
  struct timespec tp;
  int iResult = clock_gettime(CLOCK_MONOTONIC_RAW_APPROX,&tp);

  i8 iTicks = tp.tv_sec * C_1Bn;
  iTicks += tp.tv_nsec;
  
#else
  i8 iTicks = iRdtscp();
#endif
  
  return iTicks;
}

/*----------------------------------------------------------------------------*/
/* get tick frequency                                                         */
/*----------------------------------------------------------------------------*/
i8 Synch::iTickFrequency(void)
{
#ifdef __APPLE__
  i8 iFrequency = C_1Bn;
#else
  i8 iFrequency = System::iClockFrequency();
#endif
  
  return iFrequency;
}

/*----------------------------------------------------------------------------*/
/* drop read lock                                                             */
/*----------------------------------------------------------------------------*/
u8 Synch::iLockDropRead(u8* pWord)
{
#ifdef __APPLE__
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
#ifdef __APPLE__
  
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
#ifdef __APPLE__
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
#ifdef __APPLE__
  //u8 uResult = iGrabWrite(pWord);

  u8 uCurrent = *pWord;
  
  if (uCurrent >= C_WRITELOCK)
    return 0;
  
  u8 uAddWriteLock = uCurrent + C_WRITELOCK;
  
  bool b = bTestAndSet(pWord,uCurrent,uAddWriteLock);
  if ( ! b )
    return 0;
  
  while (C_WRITELOCK != *pWord)
    ;
  
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
#ifdef __APPLE__
  u8 uResult = *pWord ? 1 : 0;
#else
  u8 uResult = iIsLocked(pWord);
#endif
  
  return uResult;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
