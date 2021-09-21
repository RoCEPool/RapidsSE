/*============================================================================*/
/* Timer.cpp -- timer                                                         */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Timer* BASE::_pTimer(void* aAddr)
{
  Timer* pTimer = reinterpret_cast<Timer*>(aAddr);
  
  RASSERT_TYPE(pTimer, CN_TIMER);
  
  return pTimer;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Timer::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_TIMER);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Timer::operator delete(void* aAddr)
{
  Object* pObject = reinterpret_cast<Object*>(aAddr);
  
  Heaps::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Timer::Timer(void)
: Object(CN_TIMER)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  m_iStart  = 0;
  m_iUpdate = 0;

  _checkInitialised_(CN_TIMER);

  _postcondition_(_MAGIC_,CN_TIMER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Timer::~Timer(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the timer                                                           */
/*----------------------------------------------------------------------------*/
void Timer::vRepair(Object* pObject)
{
  _static_

  Timer* pTimer = _pTimer(pObject);

  // delete the timer
  delete pTimer;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void Timer::vReset(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  m_iStart  = 0;
  m_iUpdate = 0;
}

/*----------------------------------------------------------------------------*/
/* start                                                                      */
/*----------------------------------------------------------------------------*/
void Timer::vStart(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  m_iStart  = TickingDevice::iGetTick();
  m_iUpdate = 0;
}

/*----------------------------------------------------------------------------*/
/* set                                                                        */
/*----------------------------------------------------------------------------*/
void Timer::vSet(i8 iStart, i8 iUpdate)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  m_iStart  = iStart;
  m_iUpdate = iUpdate;
}

/*----------------------------------------------------------------------------*/
/* update                                                                     */
/*----------------------------------------------------------------------------*/
void Timer::vUpdate(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  ASSERT(m_iStart);
  m_iUpdate = TickingDevice::iGetTick();
}

/*----------------------------------------------------------------------------*/
/* magnitude                                                                  */
/*----------------------------------------------------------------------------*/
i8 Timer::iMagnitude(i8 iBase)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  i8 iNs = iNanoseconds();
  
  i8 iVal = iNs / iBase;
  
  i8 iMag = 0;
  
       if (iVal < 1) iMag = 0;
  else if (iVal < 10) iMag = 1;
  else if (iVal < 100) iMag = 2;
  else if (iVal < 1000) iMag = 3;
  else if (iVal < 10000) iMag = 4;
  else if (iVal < 100000) iMag = 5;
  else if (iVal < 1000000) iMag = 6;
  else if (iVal < 10000000) iMag = 7;
  else if (iVal < 100000000) iMag = 8;
  else                         iMag = 9;
  
  return iMag;
}

/*----------------------------------------------------------------------------*/
/* microseconds                                                               */
/*----------------------------------------------------------------------------*/
i8 Timer::iMicroseconds(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  i8 iTicks = this->iTickDelta();

  i8 iTicksPerSec = TickingDevice::iTicksPerSec();
  
  i8 iMicro = (iTicks * C_1M) / iTicksPerSec;
  
  return iMicro;
}

/*----------------------------------------------------------------------------*/
/* milliseconds                                                               */
/*----------------------------------------------------------------------------*/
i8 Timer::iMilliseconds(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  i8 iTicks = this->iTickDelta();

  i8 iTicksPerSec = TickingDevice::iTicksPerSec();
  
  i8 iMilli = (iTicks * C_1K) / iTicksPerSec;
  
  return iMilli;
}

/*----------------------------------------------------------------------------*/
/* nanoseconds                                                                */
/*----------------------------------------------------------------------------*/
i8 Timer::iNanoseconds(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  i8 iTicks = this->iTickDelta();

  i8 iTicksPerSec = TickingDevice::iTicksPerSec();
  
  //i8 iNano = (iTicks * C_1G) / iTicksPerSec;
  
  i16 iNano = ((i16)iTicks * (i16)C_1G) / (i16)iTicksPerSec;
  
  return (i8)iNano;
}

/*----------------------------------------------------------------------------*/
/* seconds                                                                    */
/*----------------------------------------------------------------------------*/
i8 Timer::iSeconds(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  i8 iTicks = this->iTickDelta();

  i8 iTicksPerSec = TickingDevice::iTicksPerSec();
  
  i8 iSeconds = iTicks / iTicksPerSec;
  
  return iSeconds;
}

/*----------------------------------------------------------------------------*/
/* number of ticks                                                            */
/*----------------------------------------------------------------------------*/
i8 Timer::iTickDelta(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  i8 iTicks = (m_iUpdate - m_iStart);
  
  return iTicks;
}

/*----------------------------------------------------------------------------*/
/* get start                                                                  */
/*----------------------------------------------------------------------------*/
i8 Timer::iStart(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  return m_iStart;
}

/*----------------------------------------------------------------------------*/
/* get update                                                                  */
/*----------------------------------------------------------------------------*/
i8 Timer::iUpdate(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  return m_iUpdate;
}

/*----------------------------------------------------------------------------*/
/* get rate per second                                                        */
/*----------------------------------------------------------------------------*/
d8 Timer::dRatePerSec(i8 nItems)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  i8 nNanosPerItem = iNanoseconds() / nItems;
  
  RASSERT(nNanosPerItem < C_1G);
  
  if (! nNanosPerItem)
    return 0.0;
  
  d8 dRate = (d8)C_1G / (d8)nNanosPerItem;
  
  return dRate;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Timer)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
