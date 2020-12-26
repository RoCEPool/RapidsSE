/*============================================================================*/
/* SP_Header.hpp - Header sprint helper                                       */
/*============================================================================*/
#pragma once

namespace Resources
{
  class SP_Header : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Header(void);
   ~SP_Header(void) noexcept(false);
  
    Stz* sz(Header* aHeader);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
