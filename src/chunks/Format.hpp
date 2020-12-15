/*============================================================================*/
/* Format.hpp -- set of formatting characters and rules                       */
/*============================================================================*/
#pragma once

#include "eLE.h"

namespace Chunks
{
  class Format : public Object
  {
  public:
    eDC  m_eDateComponent[3];    // date component order
    char m_cDateSeparator;       // date separater
    char m_cEnclosedBy_Leading;  // enclosed by
    char m_cEnclosedBy_Trailing; // enclosed by
    char m_cEscapeChar;          // escape char
    char m_cFieldDelimiter;      // field delimiter
    eLE  m_eLineEnd;             // line end sequence
    char m_cRecordTerminator;    // record terminator
  
    bool m_bAllowNonPrintable;   // allow non-printable
    i8   m_nMaxErrors;           // maximum number of allowable errors
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Format(void);
   ~Format(void) noexcept(false);
  
    bool bIsStdCsv(void);
    void vReset(void);
    void vSet(Format* pFmt);
    bool bSetFormat(TagSet* pFmtTagSet);
    void vShow(void);
    
  private:
    bool bExtractDateFormat(Stz* dateFormat);
    Footer* aFooter(void);
  };
  
  Format* _pFormat(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
