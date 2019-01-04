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
  
  //m_xElapsedTimer
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
  
  if (m_nBytesSoFar >= m_nNextReport)
    vReportStatistics();
}

/*----------------------------------------------------------------------------*/
/* begin timing                                                               */
/*----------------------------------------------------------------------------*/
void ByteRate::vBeginTiming(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BYTERATE,_FOOTER_);

  m_xElapsedTimer.vStart();
}

/*----------------------------------------------------------------------------*/
/* calculate current byte rate per day                                        */
/*----------------------------------------------------------------------------*/
i8 ByteRate::nBytesPerDay(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BYTERATE,_FOOTER_);
  
  i8 iAvgBytesPerSec = nBytesPerSec();
  
  i8 iAvgBytesPerDay = iAvgBytesPerSec * 60 * 60 * 24;

  return iAvgBytesPerDay;
}

/*----------------------------------------------------------------------------*/
/* calculate current byte rate per sec                                        */
/*----------------------------------------------------------------------------*/
i8 ByteRate::nBytesPerSec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BYTERATE,_FOOTER_);
  
  i8 iAvgBytesPerSec = DIV(m_nBytesSoFar,m_xElapsedTimer.iSeconds());
  
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
  
  s32 sAvgMag;
  s32 sSumMag;
  s32 sDayMag;

  Now oNow;
  
  vUpdateTimer();
  
  i8 iAvgBytesPerSec = nBytesPerSec();
  i8 iAvgBytesPerDay = nBytesPerDay();

  Magnitudes::vByteMag(iAvgBytesPerSec,2,sAvgMag);
  Magnitudes::vByteMag(m_nBytesSoFar,  2,sSumMag);
  Magnitudes::vByteMag(iAvgBytesPerDay,2,sDayMag);

  vTrace("%s %s%s %llds: %s/s (%s/day)\n"_Stz,
         oNow.sFormatted()->aChar(),
         m_sReportPrefix.aStz(),
         sSumMag.aStz(),
         m_xElapsedTimer.iSeconds(),
         
         sAvgMag.aStz(),
         sDayMag.aStz());
  
  m_nNextReport += m_iReportQuantum;
}

/*----------------------------------------------------------------------------*/
/* update timer                                                               */
/*----------------------------------------------------------------------------*/
void ByteRate::vUpdateTimer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BYTERATE,_FOOTER_);
  
  m_xElapsedTimer.vUpdate();
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
