/*============================================================================*/
/* SN_Bol.hpp - Bol scan helper                                               */
/*============================================================================*/
#pragma once

namespace Datums

{
  class SN_Bol : public Scan
  {
  public:
    
   // must be last
    Footer m_xFooter;
    
  public:
    SN_Bol(void);
    ~SN_Bol(void) noexcept(false);
  
    i8 i(char** aacCrs, Format* pFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
