/*============================================================================*/
/* SP_Bits.hpp - Bits sprint helper                             (on c stack)  */
/*============================================================================*/
#pragma once

namespace Resources
{
  class SP_Bits : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Bits(void);
   ~SP_Bits(void) noexcept(false);
  
    Stz* sz(u8 iWord);

  private:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
