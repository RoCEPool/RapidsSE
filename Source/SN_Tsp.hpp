/*============================================================================*/
/* SN_Tsp.hpp - Tme scan helper                                               */
/*============================================================================*/
#pragma once

namespace ScanSprint
{
  class SN_Tsp : public Scan
  {
  public:
    
    // must be last
    Footer m_xFooter;
    
  public:
    SN_Tsp(void);
    ~SN_Tsp(void) noexcept(false);
  
    i8 i(char** aacCrs, Column* pCol, Format* pFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
