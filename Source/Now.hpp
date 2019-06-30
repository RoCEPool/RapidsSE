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
    i8   m_iTsp;

    // must be last
    Footer m_xFooter;
    
  public:
    Now(void);
    ~Now(void) noexcept(false);
  
    void vSetNow(void);
  
    Stz* sz(void);
    Stz* sz(Since* pSincePosixEpoch);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
