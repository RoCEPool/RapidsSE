/*============================================================================*/
/* Format.hpp -- set of formatting characters                                 */
/*============================================================================*/
#pragma once

namespace Chunks
{
  class Format : public Object
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
    
    Format(void);
   ~Format(void) noexcept(false);
  
    bool bIsOrdCsv(void);
    void vReset(void);
    void vSet(Format* pFmt);
    bool bSetFormat(TagSet* pFmtTagSet);
    void vShow(void);
    
  private:
    bool bExtractDateFormat(Stz* dateFormat);
    Footer* aFooter(void);
  };
  
  Format* _pFormatters(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
