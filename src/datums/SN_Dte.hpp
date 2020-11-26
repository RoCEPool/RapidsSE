/*============================================================================*/
/* SN_Dte.hpp - Dte scan helper                                               */
/*============================================================================*/
#pragma once

namespace Datums
{
  class SN_Dte : public Scan
  {
  public:
    
    // must be last
    Footer m_xFooter;
    
  public:
    SN_Dte(void);
    ~SN_Dte(void) noexcept(false);
  
    i8 i(char** aacCrs, Format* pFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
