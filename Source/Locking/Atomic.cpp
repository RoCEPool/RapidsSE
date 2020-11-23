/*============================================================================*/
/* Atomic.cpp -- Atomic Integer                                               */
/*============================================================================*/

#include "Locking.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Atomic::Atomic(void)
: Object(CN_ATOMIC)
{
  m_iInt = 0;
  
  _checkInitialised_(CN_ATOMIC);
  
  _postcondition_(_MAGIC_,CN_ATOMIC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Atomic::~Atomic(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ATOMIC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* atomically add word                                                        */
/*----------------------------------------------------------------------------*/
u8 Atomic::uAtomicAdd(i8* pWord, i8 iValue)
{
#ifdef WITH_APPLE
  u8 uResult = OSAtomicAdd64(iValue, pWord);
  
#elif defined(WITH_LINUX)
  u8 uResult = __sync_add_and_fetch((u8*)pWord, iValue);
  
#else
#error "Unknown Operating Library"
#endif
  
  return uResult;
}

/*----------------------------------------------------------------------------*/
/* atomically decrement word                                                  */
/*----------------------------------------------------------------------------*/
u8 Atomic::uAtomicDec(i8* pWord)
{
#ifdef WITH_APPLE
  u8 uResult = OSAtomicDecrement64(pWord);
  
#elif defined(WITH_LINUX)
  u8 uResult = __sync_sub_and_fetch((u8*)pWord, 1);
  
#else
  u8 uResult = iAtomicDec(pWord);
  
#endif
  
  return uResult;
}

/*----------------------------------------------------------------------------*/
/* atomically increment word                                                  */
/*----------------------------------------------------------------------------*/
u8 Atomic::uAtomicInc(i8* pWord)
{
#ifdef WITH_APPLE
  u8 uResult = OSAtomicIncrement64(pWord);
  
#elif defined(WITH_LINUX)
  u8 uResult = __sync_add_and_fetch((u8*)pWord, 1);
  
#else
  u8 uResult = iAtomicInc(pWord);
  
#endif
  
  return uResult;
}

/*----------------------------------------------------------------------------*/
/* atomic test and set                                                        */
/*----------------------------------------------------------------------------*/
bool Atomic::bTestAndSet(u8* pWord, u8 uIf, u8 uThen)
{
#ifdef WITH_APPLE
  u8 uResult = OSAtomicCompareAndSwap64((i8)uIf,(i8)uThen,(i8*)pWord);
  
#elif defined(WITH_LINUX)
  bool uResult = __sync_bool_compare_and_swap(pWord, uIf, uThen);
  
#else
  u8 uResult = iTestSet(pWord, uIf, uThen);
  
#endif
  
  return uResult;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* check lock initialization                                                  */
/*----------------------------------------------------------------------------*/
void Atomic::vCheckInit(void)
{
  _precondition_(_MAGIC_,CN_ATOMIC,_FOOTER_);
  
  ASSERT(0 == m_iInt); // logic check
}

/*----------------------------------------------------------------------------*/
/* atomically add                                                             */
/*----------------------------------------------------------------------------*/
i8 Atomic::iAdd(i8 iValue)
{
  _precondition_(_MAGIC_,CN_ATOMIC,_FOOTER_);
  
  i8 i = Atomic::uAtomicAdd(&m_iInt, iValue);
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* atomically dec                                                             */
/*----------------------------------------------------------------------------*/
i8 Atomic::iDec(void)
{
  _precondition_(_MAGIC_,CN_ATOMIC,_FOOTER_);

  i8 i = Atomic::uAtomicDec(&m_iInt);
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* atomically inc                                                             */
/*----------------------------------------------------------------------------*/
i8 Atomic::iInc(void)
{
  _precondition_(_MAGIC_,CN_ATOMIC,_FOOTER_);
  
  i8 i = Atomic::uAtomicInc(&m_iInt);
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
i8 Atomic::iVal(void)
{
  _precondition_(_MAGIC_,CN_ATOMIC,_FOOTER_);
  
  return m_iInt;
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void Atomic::operator =(i8 iInt)
{
  _precondition_(_MAGIC_,CN_ATOMIC,_FOOTER_);
  
  m_iInt = iInt;
}

/*----------------------------------------------------------------------------*/
/* dereference / get value operator                                           */
/*----------------------------------------------------------------------------*/
i8 Atomic::operator *(void)
{
  _precondition_(_MAGIC_,CN_ATOMIC,_FOOTER_);
  
  return m_iInt;
}

/*----------------------------------------------------------------------------*/
/* atomically inc operator (prefix form)                                      */
/*----------------------------------------------------------------------------*/
i8 Atomic::operator ++(void)
{
  _precondition_(_MAGIC_,CN_ATOMIC,_FOOTER_);
  
  i8 i = Atomic::uAtomicInc(&m_iInt);
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* atomically dec operator (prefix form)                                      */
/*----------------------------------------------------------------------------*/
i8 Atomic::operator --(void)
{
  _precondition_(_MAGIC_,CN_ATOMIC,_FOOTER_);
  
  i8 i = Atomic::uAtomicDec(&m_iInt);

  return i;
}

/*----------------------------------------------------------------------------*/
/* atomically add operator                                                    */
/*----------------------------------------------------------------------------*/
i8 Atomic::operator +=(i8 iValue)
{
  _precondition_(_MAGIC_,CN_ATOMIC,_FOOTER_);
  
  i8 i = Atomic::uAtomicAdd(&m_iInt, iValue);

  return i;
}

/*----------------------------------------------------------------------------*/
/* atomically subtract operator                                               */
/*----------------------------------------------------------------------------*/
i8 Atomic::operator -=(i8 iValue)
{
  _precondition_(_MAGIC_,CN_ATOMIC,_FOOTER_);
  
  i8 i = Atomic::uAtomicAdd(&m_iInt, ( - iValue) );  // nb the minus sign
  
  return i;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Atomic)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
