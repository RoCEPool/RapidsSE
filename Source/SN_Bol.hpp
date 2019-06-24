/*============================================================================*/
/* SN_Bol.hpp - Bol scan helper                                               */
/*============================================================================*/
#pragma once

namespace ScanSprint
{
  class SN_Bol : public Scan
  {
  public:
  
    Dtm m_xBol;
  
   // must be last
    Footer m_xFooter;
    
  public:
    SN_Bol(void);
    ~SN_Bol(void) noexcept(false);
  
    i8 i(char** aacCrs, Column* pCol, Formatters* pScnFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
