/*============================================================================*/
/* Now.hpp - Now sprint helper                                                */
/*============================================================================*/
#pragma once

namespace Measurement
{
  class Now : public Object
  {
  private:
    char m_caResult[32];
    Dtm  m_wTsp;

    // must be last
    Footer m_xFooter;
    
  public:
    Now(void);
    ~Now(void) noexcept(false);
  
    void vSetNow(void);
    void vSetNowUTC(void);
  
    Stz* sz(void);
    Stz* sz(Since* pSincePosixEpoch);

    i8   iTsp(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
