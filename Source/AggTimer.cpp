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
  
  RASSERT_TYPE(pAggTimer, CN_AGGTIMER);
  
  return pAggTimer;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* AggTimer::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_AGGTIMER);
  
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
: Object(CN_AGGTIMER)
{
  _precondition_(_MAGIC_,CN_AGGTIMER,_FOOTER_);
  
  m_iTicks  = 0;

  _checkInitialised_(CN_AGGTIMER);

  _postcondition_(_MAGIC_,CN_AGGTIMER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
AggTimer::~AggTimer(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_AGGTIMER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void AggTimer::vReset(void)
{
  _precondition_(_MAGIC_,CN_AGGTIMER,_FOOTER_);
  
  m_iTicks  = 0;
}

/*----------------------------------------------------------------------------*/
/* set                                                                        */
/*----------------------------------------------------------------------------*/
void AggTimer::vSet(i8 iTicks)
{
  _precondition_(_MAGIC_,CN_AGGTIMER,_FOOTER_);
  
  m_iTicks  = iTicks;
}

/*----------------------------------------------------------------------------*/
/* nanoseconds                                                                */
/*----------------------------------------------------------------------------*/
i8 AggTimer::iNanoseconds(void)
{
  _precondition_(_MAGIC_,CN_AGGTIMER,_FOOTER_);
    
  i8 iTicks = this->iTicks();
  
  i8 iTicksPerSecond = TickingDevice::iTicksPerSecond();
  
  i8 iNano = (iTicks * C_1G) / iTicksPerSecond;
  
  return iNano;
}

/*----------------------------------------------------------------------------*/
/* microseconds                                                               */
/*----------------------------------------------------------------------------*/
i8 AggTimer::iMicroseconds(void)
{
  _precondition_(_MAGIC_,CN_AGGTIMER,_FOOTER_);
  
  i8 iTicks = this->iTicks();
  
  i8 iTicksPerSecond = TickingDevice::iTicksPerSecond();
  
  i8 iMicro = (iTicks * C_1M) / iTicksPerSecond;
  
  return iMicro;
}

/*----------------------------------------------------------------------------*/
/* milliseconds                                                               */
/*----------------------------------------------------------------------------*/
i8 AggTimer::iMilliseconds(void)
{
  _precondition_(_MAGIC_,CN_AGGTIMER,_FOOTER_);
  
  i8 iTicks = this->iTicks();
  
  i8 iTicksPerSecond = TickingDevice::iTicksPerSecond();
  
  i8 iMilli = (iTicks * C_1K) / iTicksPerSecond;
  
  return iMilli;
}

/*----------------------------------------------------------------------------*/
/* seconds                                                                    */
/*----------------------------------------------------------------------------*/
i8 AggTimer::iSeconds(void)
{
  _precondition_(_MAGIC_,CN_AGGTIMER,_FOOTER_);
  
  i8 iTicks = this->iTicks();
  
  i8 iTicksPerSecond = TickingDevice::iTicksPerSecond();
  
  i8 iSeconds = iTicks / iTicksPerSecond;
  
  return iSeconds;
}

/*----------------------------------------------------------------------------*/
/* number of ticks                                                            */
/*----------------------------------------------------------------------------*/
i8 AggTimer::iTicks(void)
{
  _precondition_(_MAGIC_,CN_AGGTIMER,_FOOTER_);
    
  return m_iTicks;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(AggTimer)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
