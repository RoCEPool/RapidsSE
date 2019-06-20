/*============================================================================*/
/* hpRowEncoding.hpp - Row Encoding sprint helper                             */
/*============================================================================*/
#pragma once

namespace Data
{
  class hpRowEncoding : public Sprint
  {
  public:
  
    char m_caRow[C_16KB];
  
    // must be last
    Footer m_xFooter;
    
  public:
    hpRowEncoding(void);
    ~hpRowEncoding(void) noexcept(false);

    void vRowEncoding(RowEncoding* pRowEncoding);
  
    Stz* sz(void);
    Stz* sz(RowEncoding* pRowEncoding);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
