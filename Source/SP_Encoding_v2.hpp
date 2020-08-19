/*============================================================================*/
/* SP_Encoding_v2.hpp - Row Encoding_v2 sprint helper                         */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  class SP_Encoding_v2 : public Sprint
  {
  public:
  
    char m_caEncoding[C_16KB];
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Encoding_v2(void);
    ~SP_Encoding_v2(void) noexcept(false);

    void vEncoding(Encoding_v2* pEncoding);
  
    Stz* sz(void);
    Stz* sz(Encoding_v2* pEncoding);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
