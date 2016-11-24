/*============================================================================*/
/* Synch.cpp -- platform independent synchronisation                          */
/*============================================================================*/

#include "Common.h"

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
  i8 iTicks = iRdtscp();
  
  return iTicks;
}

/*----------------------------------------------------------------------------*/
/* get tick frequency                                                         */
/*----------------------------------------------------------------------------*/
i8 Synch::iTickFrequency(void)
{
  i8 iFrequency = System::iClockFrequency();
  
  return iFrequency;
}

/*----------------------------------------------------------------------------*/
/* drop read lock                                                             */
/*----------------------------------------------------------------------------*/
u8 Synch::iLockDropRead(u8* pWord)
{
  u8 uResult = iDropRead(pWord);
  
  return uResult;
}

/*----------------------------------------------------------------------------*/
/* grab read lock                                                             */
/*----------------------------------------------------------------------------*/
u8 Synch::iLockGrabRead(u8* pWord)
{
  u8 uResult = iGrabRead(pWord);
  
  return uResult;
}

/*----------------------------------------------------------------------------*/
/* drop write lock                                                            */
/*----------------------------------------------------------------------------*/
u8 Synch::iLockDropWrite(u8* pWord)
{
  u8 uResult = iDropWrite(pWord);
  
  return uResult;
}

/*----------------------------------------------------------------------------*/
/* grab write lock                                                            */
/*----------------------------------------------------------------------------*/
u8 Synch::iLockGrabWrite(u8* pWord)
{
  u8 uResult = iGrabWrite(pWord);
  
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
