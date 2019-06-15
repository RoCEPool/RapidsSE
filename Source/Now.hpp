/*============================================================================*/
/* Now.hpp - Now sprint helper                                                */
/*============================================================================*/
#pragma once

namespace Measurement
{
  class Now : public Object
  {
  public:
    char m_caResult[32];
    Dtm  m_oTsp;

    // must be last
    Footer m_xFooter;
    
  public:
    Now(void);
    ~Now(void) noexcept(false);
  
    Stz* sz(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
