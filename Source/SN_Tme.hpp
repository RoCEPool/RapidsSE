/*============================================================================*/
/* SN_Tme.hpp - Tme scan helper                                               */
/*============================================================================*/
#pragma once

namespace ScanSprint
{
  class SN_Tme : public Scan
  {
  public:
  
    Dtm m_xTme;
  
    // must be last
    Footer m_xFooter;
    
  public:
    SN_Tme(void);
    ~SN_Tme(void) noexcept(false);
  
    i8 i(char** aacCrs, Column* pCol, Formatters* pScnFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
