/*============================================================================*/
/* snVCH.hpp - Vch scan helper                                                */
/*============================================================================*/
#pragma once

namespace Transform
{
  class snVCH : public Scan
  {
  public:
  
    char m_caVch[C_MAX_VCH];
  
    // must be last
    Footer m_xFooter;
    
  public:
    snVCH(void);
    ~snVCH(void) noexcept(false);
    
    Stz* sz(char** aacCrs,
            Column* pCol,
            Formatters* pScnFmt,
            bool bSawLeadingEnclosed,
            i8& nChars_);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
