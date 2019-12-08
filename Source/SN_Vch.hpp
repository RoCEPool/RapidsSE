/*============================================================================*/
/* SN_Vch.hpp - Vch scan helper                                               */
/*============================================================================*/
#pragma once

namespace ScanSprint
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
            Column* pCol,
            Format* pFmt,
            bool bSawLeadingEnclosed,
            i8& nChars_);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
