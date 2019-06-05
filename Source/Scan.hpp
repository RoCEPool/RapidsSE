/*============================================================================*/
/* Scan.hpp - Scan Utility                                                    */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  static const i8 C_SCAN_SIZE = 64;
  
  class Scan : public Object
  {
  public:
  
    // defult buffers
    char m_caFormat[C_SCAN_SIZE];
    char m_caResult[C_SCAN_SIZE];

    // must be last
    Footer m_xFooter;
    
  public:
    Scan(eCN eClass);
    ~Scan(void) noexcept(false);
  
    Stz* sz(void);
  
    Stz* szFormat(void);
    void vFormat(const Stz* szFormat);

    Stz* szResult(void);
    void vResult(const Stz* szResult);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
