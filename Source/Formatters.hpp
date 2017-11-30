/*============================================================================*/
/* Formatters.hpp -- set of formatting characters                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Formatters : public Object
  {
  public:
  
    char m_cEnclosedBy;       // enclosed by
    char m_cEscapeChar;       // escape char
    char m_cFieldSeparator;   // field separator
    char m_cRecordTerminator; // record terminator
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Formatters(void);
    ~Formatters(void);
        
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
