/*============================================================================*/
/* SP_Footer.hpp - Footer sprint helper                                       */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class SP_Footer : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Footer(void);
   ~SP_Footer(void) noexcept(false);
  
    Stz* sz(Footer* aFoot);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
