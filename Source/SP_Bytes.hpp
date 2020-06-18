/*============================================================================*/
/* SP_Bytes.hpp - Bytes sprint helper                           (on c stack)  */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class SP_Bytes : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Bytes(void);
   ~SP_Bytes(void) noexcept(false);
  
    Stz* sz(i8 nBytes);

  private:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
