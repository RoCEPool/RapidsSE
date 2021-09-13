/*============================================================================*/
/* Stats.cpp -- Stats                                                         */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Stats* BASE::_pStats(void* aAddr)
{
  Stats* pStats = reinterpret_cast<Stats*>(aAddr);
  
  RASSERT_TYPE(pStats, CN_STATS);
  
  return pStats;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Stats::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_STATS);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Stats::operator delete(void* aAddr)
{
  Object* pObject = reinterpret_cast<Object*>(aAddr);
  
  Heaps::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stats::Stats(void)
: Object(CN_STATS)
{
  _precondition_(_MAGIC_,CN_STATS,_FOOTER_);

  // cpu
  m_iStart  = 0;
  m_iUpdate = 0;

  // work
  m_iTicks = 0;
  m_nTimes = 0;

  m_nOperands = 0;
  m_nMaxQs    = 0;

  m_nBuffIn   = 0;
  m_nBuffOut  = 0;

  // i/o
  m_nSends     = 0;
  m_nBytesSent = 0;
  m_nRecvs     = 0;
  m_nBytesRecv = 0;

  m_nRead       = 0;
  m_nBytesRead  = 0;
  m_nWrite      = 0;
  m_nBytesWrite = 0;

  // memory
  m_nNewRowBuffer = 0;
  m_nDelRowBuffer = 0;
  m_nMaxRowBuffer = 0;
  m_nAvgRowBuffer = 0;

  m_nNewCsvBuffer = 0;
  m_nDelCsvBuffer = 0;
  m_nMaxCsvBuffer = 0;
  m_nAvgCsvBuffer = 0;

  m_nAllocs     = 0;
  m_nBytesAlloc = 0;
  m_nFrees      = 0;
  m_nBytesFree  = 0;
  m_nMaxBytes   = 0;
  m_nAvgBytes   = 0;

  _checkInitialised_(CN_STATS);

  _postcondition_(_MAGIC_,CN_STATS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stats::~Stats(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_STATS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the stats                                                           */
/*----------------------------------------------------------------------------*/
void Stats::vRepair(Object* pObject)
{
  _static_

  Stats* pStats = _pStats(pObject);

  // delete the stats
  delete pStats;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* nanoseconds                                                                */
/*----------------------------------------------------------------------------*/
i8 Stats::iNanoseconds(void)
{
  _precondition_(_MAGIC_,CN_STATS,_FOOTER_);
  
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
i8 Stats::iMicroseconds(void)
{
  _precondition_(_MAGIC_,CN_STATS,_FOOTER_);
  
  i8 iTicks = this->iTicks();
  
  i8 iTicksPerSec = TickingDevice::iTicksPerSec();
  
  i8 iMicro = (iTicks * C_1M) / iTicksPerSec;
  
  return iMicro;
}

/*----------------------------------------------------------------------------*/
/* milliseconds                                                               */
/*----------------------------------------------------------------------------*/
i8 Stats::iMilliseconds(void)
{
  _precondition_(_MAGIC_,CN_STATS,_FOOTER_);
  
  i8 iTicks = this->iTicks();
  
  i8 iTicksPerSec = TickingDevice::iTicksPerSec();
  
  i8 iMilli = (iTicks * C_1K) / iTicksPerSec;
  
  return iMilli;
}

/*----------------------------------------------------------------------------*/
/* seconds                                                                    */
/*----------------------------------------------------------------------------*/
i8 Stats::iSeconds(void)
{
  _precondition_(_MAGIC_,CN_STATS,_FOOTER_);
  
  i8 iTicks = this->iTicks();
  
  i8 iTicksPerSec = TickingDevice::iTicksPerSec();
  
  i8 iSeconds = iTicks / iTicksPerSec;
  
  return iSeconds;
}

/*----------------------------------------------------------------------------*/
/* number of ticks                                                            */
/*----------------------------------------------------------------------------*/
i8 Stats::iTicks(void)
{
  _precondition_(_MAGIC_,CN_STATS,_FOOTER_);
    
  return m_iTicks;
}

/*----------------------------------------------------------------------------*/
/* number of times                                                            */
/*----------------------------------------------------------------------------*/
i8 Stats::nTimes(void)
{
  _precondition_(_MAGIC_,CN_STATS,_FOOTER_);
    
  return m_nTimes;
}

/*----------------------------------------------------------------------------*/
/* start                                                                      */
/*----------------------------------------------------------------------------*/
void Stats::vStartTimer(void)
{
  _precondition_(_MAGIC_,CN_STATS,_FOOTER_);

  m_iStart  = TickingDevice::iGetTick();
  m_iUpdate = 0;
}

/*----------------------------------------------------------------------------*/
/* get rate per second                                                        */
/*----------------------------------------------------------------------------*/
d8 Stats::dRatePerSec(i8 nItems)
{
  _precondition_(_MAGIC_,CN_STATS,_FOOTER_);
  
  i8 nNanosPerItem = iNanoseconds() / nItems;
  
  RASSERT(nNanosPerItem < C_1G);
  
  if (! nNanosPerItem)
    return 0.0;
  
  d8 dRate = (d8)C_1G / (d8)nNanosPerItem;
  
  return dRate;
}

/*----------------------------------------------------------------------------*/
/* update                                                                     */
/*----------------------------------------------------------------------------*/
void Stats::vUpdateTimer(void)
{
  _precondition_(_MAGIC_,CN_STATS,_FOOTER_);

  ASSERT(m_iStart);

  m_iUpdate = TickingDevice::iGetTick();

  m_iTicks += (m_iUpdate - m_iStart);
  m_nTimes++;

}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Stats)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
