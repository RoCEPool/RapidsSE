/*============================================================================*/
/* Formatters.hpp -- set of formatting characters                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  enum eDC // date components
  {
  /* 0 */ DC_NONE=0,
  /* 1 */ DC_YYYY,
  /* 2 */ DC_MM,
  /* 3 */ DC_DD
  };
  
  static Stz* aDC[]
  {
  /* 0 */ "none"_Stz,
  /* 1 */ "YYYY"_Stz,
  /* 2 */ "MM"_Stz,
  /* 3 */ "DD"_Stz
  };

  class Formatters : public Object
  {
  public:
  
    char m_cEnclosedBy;       // enclosed by
    char m_cEscapeChar;       // escape char
    char m_cFieldSeparator;   // field separator
    char m_cRecordTerminator; // record terminator
    eDC  m_eDateComponent[3]; // date component order
    char m_cDateSeparator;    // date separacter
  
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
