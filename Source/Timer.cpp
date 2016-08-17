/*============================================================================*/
/* Timer.cpp -- timer                                                         */
/*============================================================================*/

#include "Timer.hpp"

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Timer::operator new(size_t, void* pTimerAddress)
{
  return pTimerAddress;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Timer::operator delete(void* pTimerAddress)
{
  // do nothing
  // must be explicitly free'd by caller
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Timer::Timer(void)
: cn(CN_TIMER)
{
  vInit();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Timer::~Timer(void)
{
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void Timer::vInit(void)
{
  m_iStart  = 0;
  m_iUpdate = 0;
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void Timer::vReset(void)
{
  m_iStart  = 0;
  m_iUpdate = 0;
}

/*----------------------------------------------------------------------------*/
/* begin                                                                      */
/*----------------------------------------------------------------------------*/
void Timer::vBegin(void)
{
  m_iStart  = iRdtscp();
  m_iUpdate = 0;
}

/*----------------------------------------------------------------------------*/
/* update                                                                     */
/*----------------------------------------------------------------------------*/
void Timer::vUpdate(void)
{
  ASSERT(m_iStart);
  m_iUpdate = iRdtscp();
}

/*----------------------------------------------------------------------------*/
/* nanoseconds                                                                */
/*----------------------------------------------------------------------------*/
i8 Timer::iNanoseconds(void)
{
  ASSERT(m_iStart);
  ASSERT(m_iUpdate);
  
  i8 iTicks = (m_iUpdate - m_iStart);
  i8 iTicksPerSecond = System::iClockFrequency();
  i8 iTicksPerNano = iTicksPerSecond / C_1Bn;
  
  i8 iNano = iTicks / iTicksPerNano;
  
  return iNano;
}

/*----------------------------------------------------------------------------*/
/* microseconds                                                               */
/*----------------------------------------------------------------------------*/
i8 Timer::iMicroseconds(void)
{
  ASSERT(m_iStart);
  ASSERT(m_iUpdate);
  
  i8 iTicks = (m_iUpdate - m_iStart);
  i8 iTicksPerSecond = System::iClockFrequency();
  i8 iTicksPerMicro = iTicksPerSecond / C_1M;
  
  i8 iMicro = iTicks / iTicksPerMicro;
  
  return iMicro;
}

/*----------------------------------------------------------------------------*/
/* milliseconds                                                               */
/*----------------------------------------------------------------------------*/
i8 Timer::iMilliseconds(void)
{
  ASSERT(m_iStart);
  ASSERT(m_iUpdate);
  
  i8 iTicks = (m_iUpdate - m_iStart);
  i8 iTicksPerSecond = System::iClockFrequency();
  i8 iTicksPerMilli = iTicksPerSecond / C_1K;
  
  i8 iMilli = iTicks / iTicksPerMilli;
  
  return iMilli;
}

/*----------------------------------------------------------------------------*/
/* number of ticks                                                            */
/*----------------------------------------------------------------------------*/
i8 Timer::iTicks(void)
{
  ASSERT(m_iStart);
  ASSERT(m_iUpdate);

  i8 iTicks = (m_iUpdate - m_iStart);
  
  return iTicks;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/