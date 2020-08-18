/*============================================================================*/
/* SP_Encoding.hpp - Row Encoding_v1 sprint helper                               */
/*============================================================================*/
#pragma once

namespace CompactBuffers
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

    void vEncoding(Encoding_v1* pEncoding);
  
    Stz* sz(void);
    Stz* sz(Encoding_v1* pEncoding);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
