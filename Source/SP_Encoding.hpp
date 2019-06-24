/*============================================================================*/
/* SP_Encoding.hpp - Row Encoding sprint helper                             */
/*============================================================================*/
#pragma once

namespace Data
{
  class SP_Encoding : public Sprint
  {
  public:
  
    char m_caRow[C_16KB];
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Encoding(void);
    ~SP_Encoding(void) noexcept(false);

    void vRowEncoding(Encoding* pRowEncoding);
  
    Stz* sz(void);
    Stz* sz(Encoding* pRowEncoding);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
