/*============================================================================*/
/* Format.hpp -- set of formatting characters and rules                       */
/*============================================================================*/
#pragma once

#include "eLE.h"

namespace BASE
{
  class Format : public Object
  {
  private:
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

    // Have the following fields been explicitly set?
    bool m_bIsSetDateComponent;
    bool m_bIsSetDateSeparator;
    bool m_bIsSetEnclosedBy_Leading;
    bool m_bIsSetEnclosedBy_Trailing;
    bool m_bIsSetEscapeChar;
    bool m_bIsSetFieldDelimiter;
    bool m_bIsSetLineEnd;
    bool m_bIsSetRecordTerminator;

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
    void vShow(void);

    bool bExtractDateFormat(Stz* dateFormat);
    void vEnclosedBy_Leading(char c);
    void vEnclosedBy_Trailing(char c);
    void vEscapeChar(char c);
    void vFieldDelimiter(char c);
    void vLineEnd(eLE eEnding);
    void vRecordTerminator(char c);
    void vAllowNonPrintable(bool bAllow);
    void vMaxErrors(i8 iVal);

    eDC* pDateComponents();
    eDC  eDateComponent(i8 iIdx);
    char cDateSeparator();
    char cEnclosedBy_Leading();
    char cEnclosedBy_Trailing();
    char cEscapeChar();
    char cFieldDelimiter();
    eLE  eLineEnd();
    char cRecordTerminator();
    bool bAllowNonPrintable();
    i8   nMaxErrors();

    bool bIsSetDateSeparator();
    bool bIsSetEnclosedBy_Leading();
    bool bIsSetEnclosedBy_Trailing();
    bool bIsSetEscapeChar();
    bool bIsSetFieldDelimiter();
    bool bIsSetLineEnd();
    bool bIsSetRecordTerminator();

  private:
    Footer* aFooter(void);
  };
  
  Format* _pFormat(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
