/*============================================================================*/
/* SP_String.hpp - String sprint helper                                       */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class SP_String : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_String(void);
    ~SP_String(void) noexcept(false);
  
    Stz* szRight(Stz* szString, i8 iLength);

  private:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
