/*============================================================================*/
/* SN_Bcd.hpp - Bcd scan helper                                               */
/*============================================================================*/
#pragma once

namespace Datums
{
  class SN_Bcd : public Scan
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SN_Bcd(void);
    ~SN_Bcd(void) noexcept(false);
  
    i8 i(char** aacCrs, i8 iLength, i8 iScale, Format* pFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
