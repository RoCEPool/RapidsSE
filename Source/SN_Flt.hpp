/*============================================================================*/
/* SN_Flt.hpp - Flt scan helper                                               */
/*============================================================================*/
#pragma once

namespace ScanSprint
{
  class SN_Flt : public Scan
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SN_Flt(void);
    ~SN_Flt(void) noexcept(false);
  
    d8 d(char** aacCrs, Column* pCol, Format* pScnFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
