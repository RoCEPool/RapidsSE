/*============================================================================*/
/* SN_Ivl.hpp - Ivl scan helper                                               */
/*============================================================================*/
#pragma once

namespace ScanSprint
{
  class SN_Ivl : public Scan
  {
  public:
  
    Dtm m_xIvl;
  
    // must be last
    Footer m_xFooter;
    
  public:
    SN_Ivl(void);
    ~SN_Ivl(void) noexcept(false);
  
    i8 i(char** aacCrs, Column* pCol, Formatters* pScnFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
