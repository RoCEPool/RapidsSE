/*============================================================================*/
/* SP_Char.hpp - Char sprint helper                                           */
/*============================================================================*/
#pragma once

namespace Resources
{
  class SP_Char : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Char(void);
   ~SP_Char(void) noexcept(false);
  
    Stz* sz(char cChar);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
