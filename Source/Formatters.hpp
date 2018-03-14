/*============================================================================*/
/* Formatters.hpp -- set of formatting characters                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Formatters : public Object
  {
  public:
  
    char m_cEnclosedBy_Leading;  // enclosed by
    char m_cEnclosedBy_Trailing; // enclosed by
    char m_cEscapeChar;          // escape char
    char m_cFieldSeparator;      // field separator
    char m_cRecordTerminator;    // record terminator
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
