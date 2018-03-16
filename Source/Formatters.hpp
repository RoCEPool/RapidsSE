/*============================================================================*/
/* Formatters.hpp -- set of formatting characters                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  enum eRE // record ends
  {
  /* 0 */ RE_NONE=0,
  /* 1 */ RE_NL,     // record finally ends with \n
  /* 2 */ RE_CR,     // record finally ends with \r
  /* 3 */ RE_CRNL    // record finally ends with \r\n
  };
  
  static Stz* aRE[]
  {
  /* 0 */ "none"_Stz,
  /* 1 */ "NL"_Stz,
  /* 2 */ "CR"_Stz,
  /* 3 */ "CRNL"_Stz
  };
  class Formatters : public Object
  {
  public:
  
    char m_cEnclosedBy_Leading;  // enclosed by
    char m_cEnclosedBy_Trailing; // enclosed by
    char m_cEscapeChar;          // escape char
    char m_cFieldSeparator;      // field separator
    char m_cRecordTerminator;    // record terminator
    eRE  m_eRecordEnds;          // record end sequence
    eDC  m_eDateComponent[3];    // date component order
    char m_cDateSeparator;       // date separater
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Formatters(void);
    ~Formatters(void);
  
    void vReset(Formatters* pDefaults);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
