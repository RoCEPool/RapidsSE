/*============================================================================*/
/* Formatters.hpp -- set of formatting characters                             */
/*============================================================================*/
#pragma once

namespace Chunks
{
  class Formatters : public Object
  {
  public:
    bool m_bDefault;             // default formatters
    bool m_bAllowNonPrintable;   // allow non-printable
  
    char m_cEnclosedBy_Leading;  // enclosed by
    char m_cEnclosedBy_Trailing; // enclosed by
    char m_cEscapeChar;          // escape char
    char m_cFieldDelimiter;      // field delimiter
    char m_cRecordTerminator;    // record terminator
    eRE  m_eRecordEnd;           // record end sequence
    eDC  m_eDateComponent[3];    // date component order
    char m_cDateSeparator;       // date separater
  
    i8   m_nMaxErrors;           // maximum number of allowable errors
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Formatters(void);
    ~Formatters(void) noexcept(false);
  
    void vReset(void);
    void vSet(Formatters* pFormatters);
    void vShow(void);
    
    Footer* aFooter(void);
  };
  
  Formatters* _pFormatters(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
