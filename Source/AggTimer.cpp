/*============================================================================*/
/* AggTimer.cpp -- AggTimer                                                   */
/*============================================================================*/

#include "Gadgets.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
AggTimer* Gadgets::_pAggTimer(void* aAddr)
{
  AggTimer* pAggTimer = reinterpret_cast<AggTimer*>(aAddr);
  
  RASSERT_TYPE(pAggTimer, CN_TIMER);
  
  return pAggTimer;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* AggTimer::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_TIMER);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void AggTimer::operator delete(void* aAddr)
{
  Object* pObject = reinterpret_cast<Object*>(aAddr);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
AggTimer::AggTimer(void)
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
AggTimer::~AggTimer(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void AggTimer::vReset(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  m_iStart  = 0;
  m_iUpdate = 0;
}

/*----------------------------------------------------------------------------*/
/* start                                                                      */
/*----------------------------------------------------------------------------*/
void AggTimer::vStart(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  m_iStart  = Tick::iRdClock();
  m_iUpdate = 0;
}

/*----------------------------------------------------------------------------*/
/* set                                                                        */
/*----------------------------------------------------------------------------*/
void AggTimer::vSet(i8 iStart, i8 iUpdate)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  m_iStart  = iStart;
  m_iUpdate = iUpdate;
}

/*----------------------------------------------------------------------------*/
/* update                                                                     */
/*----------------------------------------------------------------------------*/
void AggTimer::vUpdate(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  ASSERT(m_iStart);
  m_iUpdate = Tick::iRdClock();
}

/*----------------------------------------------------------------------------*/
/* nanoseconds                                                                */
/*----------------------------------------------------------------------------*/
i8 AggTimer::iNanoseconds(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  //ASSERT(m_iStart);
  //ASSERT(m_iUpdate);
  
  i8 iTicks = (m_iUpdate - m_iStart);
  i8 iTicksPerSecond = Tick::iTicksPerSecond();
  
  i8 iNano = (iTicks * C_1G) / iTicksPerSecond;
  
  return iNano;
}

/*----------------------------------------------------------------------------*/
/* microseconds                                                               */
/*----------------------------------------------------------------------------*/
i8 AggTimer::iMicroseconds(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  //ASSERT(m_iStart);
  //ASSERT(m_iUpdate);
  
  i8 iTicks = (m_iUpdate - m_iStart);
  i8 iTicksPerSecond = Tick::iTicksPerSecond();
  
  i8 iMicro = (iTicks * C_1M) / iTicksPerSecond;
  
  return iMicro;
}

/*----------------------------------------------------------------------------*/
/* milliseconds                                                               */
/*----------------------------------------------------------------------------*/
i8 AggTimer::iMilliseconds(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  //ASSERT(m_iStart);
  //ASSERT(m_iUpdate);
  
  i8 iTicks = (m_iUpdate - m_iStart);
  i8 iTicksPerSecond = Tick::iTicksPerSecond();
  
  i8 iMilli = (iTicks * C_1K) / iTicksPerSecond;
  
  return iMilli;
}

/*----------------------------------------------------------------------------*/
/* seconds                                                                    */
/*----------------------------------------------------------------------------*/
i8 AggTimer::iSeconds(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  //ASSERT(m_iStart);
  //ASSERT(m_iUpdate);
  
  i8 iTicks = (m_iUpdate - m_iStart);
  i8 iTicksPerSecond = Tick::iTicksPerSecond();
  
  i8 iSeconds = iTicks / iTicksPerSecond;
  
  return iSeconds;
}

/*----------------------------------------------------------------------------*/
/* number of ticks                                                            */
/*----------------------------------------------------------------------------*/
i8 AggTimer::iTicks(void)
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
i8 AggTimer::iStart(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  return m_iStart;
}

/*----------------------------------------------------------------------------*/
/* get update                                                                 */
/*----------------------------------------------------------------------------*/
i8 AggTimer::iUpdate(void)
{
  _precondition_(_MAGIC_,CN_TIMER,_FOOTER_);
  
  return m_iUpdate;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(AggTimer)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
