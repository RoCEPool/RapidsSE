/*============================================================================*/
/* SN_Ivl.hpp - Ivl scan helper                                               */
/*============================================================================*/
#pragma once

namespace ScanSprint
{
  class SN_Ivl : public Scan
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SN_Ivl(void);
    ~SN_Ivl(void) noexcept(false);
  
    i8 i(char** aacCrs, Column* pCol, Format* pFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
