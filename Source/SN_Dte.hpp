/*============================================================================*/
/* SN_Dte.hpp - Dte scan helper                                               */
/*============================================================================*/
#pragma once

namespace ScanSprint
{
  class SN_Dte : public Scan
  {
  public:
    
    // must be last
    Footer m_xFooter;
    
  public:
    SN_Dte(void);
    ~SN_Dte(void) noexcept(false);
  
    i8 i(char** aacCrs, Column* pCol, Formatters* pScnFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
