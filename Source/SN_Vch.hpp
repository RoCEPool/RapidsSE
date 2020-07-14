/*============================================================================*/
/* SN_Vch.hpp - Vch scan helper                                               */
/*============================================================================*/
#pragma once

namespace Datums
{
  class SN_Vch : public Scan
  {
  public:
    
    char m_caVch[C_MAX_VCH];
  
    // must be last
    Footer m_xFooter;
    
  public:
    SN_Vch(void);
    ~SN_Vch(void) noexcept(false);
    
    Stz* sz(char** aacCrs,
            i8 iLength,
            Format* pFmt,
            bool bSawLeadingEnclosed,
            i8& nChars_);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
