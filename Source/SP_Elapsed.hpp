/*============================================================================*/
/* SP_Elapsed.hpp - Elapsed sprint helper                                     */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class SP_Elapsed : public Object
  {
  public:
    char m_caResult[32];

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Elapsed(void);
   ~SP_Elapsed(void) noexcept(false);
  
    Stz* szNsec(i8 iNSec);

  private:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
