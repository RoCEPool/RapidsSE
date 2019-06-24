/*============================================================================*/
/* SP_RowEncoding.hpp - Row Encoding sprint helper                             */
/*============================================================================*/
#pragma once

namespace Data
{
  class SP_RowEncoding : public Sprint
  {
  public:
  
    char m_caRow[C_16KB];
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_RowEncoding(void);
    ~SP_RowEncoding(void) noexcept(false);

    void vRowEncoding(RowEncoding* pRowEncoding);
  
    Stz* sz(void);
    Stz* sz(RowEncoding* pRowEncoding);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
