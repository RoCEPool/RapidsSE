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

  m_nTimes  = 0;

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
/* add                                                                        */
/*----------------------------------------------------------------------------*/
void AggTimer::vAdd(i8 iTicks)
{
  _precondition_(_MAGIC_,CN_AGGTIMER,_FOOTER_);
  
  m_iTicks += iTicks;
  
  m_nTimes++;
}

/*----------------------------------------------------------------------------*/
/* add                                                                        */
/*----------------------------------------------------------------------------*/
void AggTimer::vAdd(Timer* pTimer)
{
  _precondition_(_MAGIC_,CN_AGGTIMER,_FOOTER_);
  
  m_iTicks += pTimer->iTickDelta();

  m_nTimes++;
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void AggTimer::vReset(void)
{
  _precondition_(_MAGIC_,CN_AGGTIMER,_FOOTER_);
  
  m_iTicks = 0;
  
  m_nTimes = 0;
}

/*----------------------------------------------------------------------------*/
/* set                                                                        */
/*----------------------------------------------------------------------------*/
void AggTimer::vSet(i8 iTicks)
{
  _precondition_(_MAGIC_,CN_AGGTIMER,_FOOTER_);
  
  m_iTicks = iTicks;
}

/*----------------------------------------------------------------------------*/
/* nanoseconds                                                                */
/*----------------------------------------------------------------------------*/
i8 AggTimer::iNanoseconds(void)
{
  _precondition_(_MAGIC_,CN_AGGTIMER,_FOOTER_);
  
  i8 iNano;
  
  i8 iTicks = this->iTicks();
  
  i8 iTicksPerSec = TickingDevice::iTicksPerSec();
  
  if (iTicks > iTicksPerSec) {
    i8 iSecs = iTicks / iTicksPerSec;
    i8 iRemTicks  = iTicks % iTicksPerSec;
    
    iNano = iSecs * C_1G +
            (iRemTicks * C_1G) / iTicksPerSec;
  }
  else
    iNano = (iTicks * C_1G) / iTicksPerSec;
  
  return iNano;
}

/*----------------------------------------------------------------------------*/
/* microseconds                                                               */
/*----------------------------------------------------------------------------*/
i8 AggTimer::iMicroseconds(void)
{
  _precondition_(_MAGIC_,CN_AGGTIMER,_FOOTER_);
  
  i8 iTicks = this->iTicks();
  
  i8 iTicksPerSec = TickingDevice::iTicksPerSec();
  
  i8 iMicro = (iTicks * C_1M) / iTicksPerSec;
  
  return iMicro;
}

/*----------------------------------------------------------------------------*/
/* milliseconds                                                               */
/*----------------------------------------------------------------------------*/
i8 AggTimer::iMilliseconds(void)
{
  _precondition_(_MAGIC_,CN_AGGTIMER,_FOOTER_);
  
  i8 iTicks = this->iTicks();
  
  i8 iTicksPerSec = TickingDevice::iTicksPerSec();
  
  i8 iMilli = (iTicks * C_1K) / iTicksPerSec;
  
  return iMilli;
}

/*----------------------------------------------------------------------------*/
/* seconds                                                                    */
/*----------------------------------------------------------------------------*/
i8 AggTimer::iSeconds(void)
{
  _precondition_(_MAGIC_,CN_AGGTIMER,_FOOTER_);
  
  i8 iTicks = this->iTicks();
  
  i8 iTicksPerSec = TickingDevice::iTicksPerSec();
  
  i8 iSeconds = iTicks / iTicksPerSec;
  
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
/* number of times                                                            */
/*----------------------------------------------------------------------------*/
i8 AggTimer::nTimes(void)
{
  _precondition_(_MAGIC_,CN_AGGTIMER,_FOOTER_);
    
  return m_nTimes;
}

/*----------------------------------------------------------------------------*/
/* get rate per second                                                        */
/*----------------------------------------------------------------------------*/
d8 AggTimer::dRatePerSec(i8 nItems)
{
  _precondition_(_MAGIC_,CN_AGGTIMER,_FOOTER_);
  
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
__aFooterFunction(AggTimer)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
