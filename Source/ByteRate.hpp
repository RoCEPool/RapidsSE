/*============================================================================*/
/* ByteRate.hpp -- Byte Rate                                                  */
/*============================================================================*/
#pragma once

namespace Measurement
{
  class ByteRate : public Object
  {
  public:
  
  private:
  s32   m_sReportPrefix;  // report prefix
  i8    m_iReportQuantum; // how often to report at
  
  Timer m_xElapsedTimer;  // timer
  i8    m_nBytesSoFar;    // number of bytes read/written so far
  i8    m_nNextReport;    // next byte quantity to report at

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    ByteRate(void);
    ~ByteRate() noexcept(false);
  
    void vAddBytes(i8 nBytes);
    void vBeginTiming(void);
    void vEndTiming(void);
    void vSetReporting(Stz* sPrefix, i8 iReportQuantum);
  
  private:
    void vReportStatistics(void);

    Footer* aFooter(void);
  };
  
  ByteRate* _pByteRate(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
