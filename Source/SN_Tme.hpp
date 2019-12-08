/*============================================================================*/
/* SN_Tme.hpp - Tme scan helper                                               */
/*============================================================================*/
#pragma once

namespace ScanSprint
{
  class SN_Tme : public Scan
  {
  public:
    
    // must be last
    Footer m_xFooter;
    
  public:
    SN_Tme(void);
    ~SN_Tme(void) noexcept(false);
  
    i8 i(char** aacCrs, Column* pCol, Format* pFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
