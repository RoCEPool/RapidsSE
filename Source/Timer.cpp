/*============================================================================*/
/* Timer.cpp -- timer                                                         */
/*============================================================================*/

#include "Timer.hpp"

#include "Pool.h"

extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Timer* Common::_pTimer(void* pAddr)
{
  Timer* pTimer = reinterpret_cast<Timer*>(pAddr);
  
  return pTimer;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Timer::operator new(size_t iSize, Timer* pTimer)
{
  RASSERT(iSize);
  return pTimer;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Timer::operator delete(void* pTimerAddress)
{
  ASSERT(pTimerAddress);
  // do nothing
  // must be explicitly free'd by caller
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Timer::Timer(void)
: Header(CN_TIMER)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  vInit();
  
  _checkInitialised_(CN_TIMER);

  _postcondition_(_MAGIC_,CN_TIMER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Timer::~Timer(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void Timer::vInit(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);

  m_iStart  = 0;
  m_iUpdate = 0;
}

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
  
  m_iStart  = Synch::iTicks();
  m_iUpdate = 0;
}

/*----------------------------------------------------------------------------*/
/* update                                                                     */
/*----------------------------------------------------------------------------*/
void Timer::vUpdate(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  ASSERT(m_iStart);
  m_iUpdate = Synch::iTicks();
}

/*----------------------------------------------------------------------------*/
/* nanoseconds                                                                */
/*----------------------------------------------------------------------------*/
i8 Timer::iNanoseconds(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  ASSERT(m_iStart);
  ASSERT(m_iUpdate);
  
  i8 iTicks = (m_iUpdate - m_iStart);
  i8 iTicksPerSecond = Synch::iTickFrequency();
  
  i8 iNano = iTicks * C_1Bn / iTicksPerSecond;
  
  return iNano;
}

/*----------------------------------------------------------------------------*/
/* microseconds                                                               */
/*----------------------------------------------------------------------------*/
i8 Timer::iMicroseconds(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  ASSERT(m_iStart);
  ASSERT(m_iUpdate);
  
  i8 iTicks = (m_iUpdate - m_iStart);
  i8 iTicksPerSecond = Synch::iTickFrequency();
  
  i8 iMicro = iTicks * C_1M / iTicksPerSecond;
  
  return iMicro;
}

/*----------------------------------------------------------------------------*/
/* milliseconds                                                               */
/*----------------------------------------------------------------------------*/
i8 Timer::iMilliseconds(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  ASSERT(m_iStart);
  ASSERT(m_iUpdate);
  
  i8 iTicks = (m_iUpdate - m_iStart);
  i8 iTicksPerSecond = Synch::iTickFrequency();
  
  i8 iMilli = iTicks * C_1K / iTicksPerSecond;
  
  return iMilli;
}

/*----------------------------------------------------------------------------*/
/* seconds                                                                    */
/*----------------------------------------------------------------------------*/
i8 Timer::iSeconds(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  ASSERT(m_iStart);
  ASSERT(m_iUpdate);
  
  i8 iTicks = (m_iUpdate - m_iStart);
  i8 iTicksPerSecond = Synch::iTickFrequency();
  
  i8 iSeconds = iTicks / iTicksPerSecond;
  
  return iSeconds;
}

/*----------------------------------------------------------------------------*/
/* number of ticks                                                            */
/*----------------------------------------------------------------------------*/
i8 Timer::iTicks(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  ASSERT(m_iStart);
  ASSERT(m_iUpdate);

  i8 iTicks = (m_iUpdate - m_iStart);
  
  return iTicks;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Timer::pFooter(void)
{
  _precondition_(_MAGIC_,CN_TIMER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
