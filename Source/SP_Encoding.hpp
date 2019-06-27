/*============================================================================*/
/* SP_Encoding.hpp - Row Encoding sprint helper                               */
/*============================================================================*/
#pragma once

namespace Rows
{
  class SP_Encoding : public Sprint
  {
  public:
  
    char m_caEncoding[C_16KB];
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Encoding(void);
    ~SP_Encoding(void) noexcept(false);

    void vEncoding(Encoding* pEncoding);
  
    Stz* sz(void);
    Stz* sz(Encoding* pEncoding);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
