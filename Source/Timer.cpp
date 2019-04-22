/*============================================================================*/
/* Timer.cpp -- timer                                                         */
/*============================================================================*/

#include "Measurement.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Timer* Measurement::_pTimer(void* aAddr)
{
  Timer* pTimer = reinterpret_cast<Timer*>(aAddr);
  
  RASSERT_TYPE(pTimer,CN_TIMER);
  
  return pTimer;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Timer::operator new(size_t, Timer* pTimer)
{
  return pTimer;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Timer::operator delete(void*)
{
  // do nothing
  // must be explicitly free'd by caller
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
  
  m_iStart  = Tsc::iTicks();
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
  m_iUpdate = Tsc::iTicks();
}

/*----------------------------------------------------------------------------*/
/* nanoseconds                                                                */
/*----------------------------------------------------------------------------*/
i8 Timer::iNanoseconds(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  //ASSERT(m_iStart);
  //ASSERT(m_iUpdate);
  
  i8 iTicks = (m_iUpdate - m_iStart);
  i8 iTicksPerSecond = Tsc::iTicksPerSecond();
  
  i8 iNano = (iTicks * C_1G) / iTicksPerSecond;
  
  return iNano;
}

/*----------------------------------------------------------------------------*/
/* microseconds                                                               */
/*----------------------------------------------------------------------------*/
i8 Timer::iMicroseconds(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  //ASSERT(m_iStart);
  //ASSERT(m_iUpdate);
  
  i8 iTicks = (m_iUpdate - m_iStart);
  i8 iTicksPerSecond = Tsc::iTicksPerSecond();
  
  i8 iMicro = (iTicks * C_1M) / iTicksPerSecond;
  
  return iMicro;
}

/*----------------------------------------------------------------------------*/
/* milliseconds                                                               */
/*----------------------------------------------------------------------------*/
i8 Timer::iMilliseconds(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  //ASSERT(m_iStart);
  //ASSERT(m_iUpdate);
  
  i8 iTicks = (m_iUpdate - m_iStart);
  i8 iTicksPerSecond = Tsc::iTicksPerSecond();
  
  i8 iMilli = (iTicks * C_1K) / iTicksPerSecond;
  
  return iMilli;
}

/*----------------------------------------------------------------------------*/
/* seconds                                                                    */
/*----------------------------------------------------------------------------*/
i8 Timer::iSeconds(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  //ASSERT(m_iStart);
  //ASSERT(m_iUpdate);
  
  i8 iTicks = (m_iUpdate - m_iStart);
  i8 iTicksPerSecond = Tsc::iTicksPerSecond();
  
  i8 iSeconds = iTicks / iTicksPerSecond;
  
  return iSeconds;
}

/*----------------------------------------------------------------------------*/
/* number of ticks                                                            */
/*----------------------------------------------------------------------------*/
i8 Timer::iTicks(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  //ASSERT(m_iStart);
  //ASSERT(m_iUpdate);

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
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Timer::aFooter(void)
{
  _precondition_(_MAGIC_,CN_TIMER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
