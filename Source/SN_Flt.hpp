/*============================================================================*/
/* SN_Flt.hpp - Flt scan helper                                               */
/*============================================================================*/
#pragma once

namespace Datums
{
  class SN_Flt : public Scan
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SN_Flt(void);
    ~SN_Flt(void) noexcept(false);
  
    d8 d(char** aacCrs, Format* pFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
