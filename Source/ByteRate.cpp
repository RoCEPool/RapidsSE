/*============================================================================*/
/* ByteRate.cpp -- Byte Rate                                                  */
/*============================================================================*/

#include "Measurement.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ByteRate* Measurement::_pByteRate(void* aAddr)
{
  ByteRate* pByteRate = reinterpret_cast<ByteRate*>(aAddr);
  
  RASSERT_TYPE(pByteRate,CN_BYTERATE);
  
  return pByteRate;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* ByteRate::operator new(size_t)
{
  ByteRate* pByteRate = (ByteRate*)HeapMem::pAllocate(CN_BYTERATE);
  
  return pByteRate;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void ByteRate::operator delete(void*)
{
  // do nothing
  // must be explicitly free'd by caller
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ByteRate::ByteRate(void)
: Object(CN_BYTERATE)
{
  _precondition_(_MAGIC_,CN_BYTERATE,_FOOTER_);
  
  m_sReportPrefix  = "<none>"_Stz;
  m_iReportQuantum = 0;
  
  //m_oElapsedTimer
  m_nBytesSoFar = 0;
  m_nNextReport = 0;
  
  _checkInitialised_(CN_BYTERATE);

  _postcondition_(_MAGIC_,CN_BYTERATE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ByteRate::~ByteRate(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_BYTERATE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* add bytes                                                                  */
/*----------------------------------------------------------------------------*/
void ByteRate::vAddBytes(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BYTERATE,_FOOTER_);

  m_nBytesSoFar += nBytes;
  
  if (m_iReportQuantum && m_nBytesSoFar >= m_nNextReport)
    vReportStatistics();
}

/*----------------------------------------------------------------------------*/
/* begin timing                                                               */
/*----------------------------------------------------------------------------*/
void ByteRate::vBeginTiming(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BYTERATE,_FOOTER_);

  m_oElapsedTimer.vStart();
}

/*----------------------------------------------------------------------------*/
/* calculate current byte rate per day                                        */
/*----------------------------------------------------------------------------*/
i8 ByteRate::nBytesPerDay(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BYTERATE,_FOOTER_);
  
  i8 iAvgBytesPerMilliSec = nBytesPerMilliSecond();
  
  i8 iAvgBytesPerDay = iAvgBytesPerMilliSec * 1000 * 60 * 60 * 24;

  return iAvgBytesPerDay;
}

/*----------------------------------------------------------------------------*/
/* calculate current byte rate per millisec                                   */
/*----------------------------------------------------------------------------*/
i8 ByteRate::nBytesPerMilliSecond(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BYTERATE,_FOOTER_);
  
  i8 nMilliSeconds = m_oElapsedTimer.iMilliseconds();
  
  i8 iAvgBytesPerMilliSec = DIV(m_nBytesSoFar,nMilliSeconds);
  
  return iAvgBytesPerMilliSec;
}

/*----------------------------------------------------------------------------*/
/* calculate current byte rate per sec                                        */
/*----------------------------------------------------------------------------*/
i8 ByteRate::nBytesPerSecond(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BYTERATE,_FOOTER_);
  
  i8 nSeconds = m_oElapsedTimer.iSeconds();
  
  i8 iAvgBytesPerSec = DIV(m_nBytesSoFar,nSeconds);
  
  return iAvgBytesPerSec;
}


/*----------------------------------------------------------------------------*/
/* end timing                                                                 */
/*----------------------------------------------------------------------------*/
void ByteRate::vEndTiming(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BYTERATE,_FOOTER_);
  
  vReportStatistics();
}

/*----------------------------------------------------------------------------*/
/* set reporting                                                              */
/*----------------------------------------------------------------------------*/
void ByteRate::vSetReporting(Stz* sPrefix, i8 iReportQuantum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BYTERATE,_FOOTER_);
  
  m_sReportPrefix  = sPrefix;
  m_iReportQuantum = iReportQuantum;
  m_nNextReport    = iReportQuantum;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* report statistics                                                          */
/*----------------------------------------------------------------------------*/
void ByteRate::vReportStatistics(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BYTERATE,_FOOTER_);
  
  s32 sSumMag;
  s32 sMilMag;
  s32 sSecMag;
  s32 sDayMag;

  Now oNow;
  
  vUpdateTimer();
  
  i8 iElapsedSeconds = m_oElapsedTimer.iSeconds();
  i8 iAvgBytesPerMil = nBytesPerMilliSecond();
  i8 iAvgBytesPerSec = nBytesPerSecond();
  i8 iAvgBytesPerDay = nBytesPerDay();

  Magnitudes::vByteMag(m_nBytesSoFar,  2,sSumMag);
  Magnitudes::vByteMag(iAvgBytesPerMil,2,sMilMag);
  Magnitudes::vByteMag(iAvgBytesPerSec,2,sSecMag);
  Magnitudes::vByteMag(iAvgBytesPerDay,2,sDayMag);

  m_nNextReport += m_iReportQuantum;
  vTrace("%s %s %llds: sum %s %s/ms %s/s %s/day\n"_Stz,
         
         oNow.sFormatted()->aChar(),
         m_sReportPrefix.aStz(),
         iElapsedSeconds,
         
         sSumMag.aStz(),
         sMilMag.aStz(),
         sSecMag.aStz(),
         sDayMag.aStz());
  
}

/*----------------------------------------------------------------------------*/
/* update timer                                                               */
/*----------------------------------------------------------------------------*/
void ByteRate::vUpdateTimer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BYTERATE,_FOOTER_);
  
  m_oElapsedTimer.vUpdate();
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ByteRate::aFooter(void)
{
  _precondition_(_MAGIC_,CN_BYTERATE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
