/*============================================================================*/
/* SN_Bcd.hpp - Bcd scan helper                                               */
/*============================================================================*/
#pragma once

namespace ScanSprint
{
  class SN_Bcd : public Scan
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SN_Bcd(void);
    ~SN_Bcd(void) noexcept(false);
  
    i8 i(char** aacCrs, Column* pCol, Formatters* pScnFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
