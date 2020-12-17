/*============================================================================*/
/* SP_Elapsed.hpp - Elapsed sprint helper                       (on c stack)  */
/*============================================================================*/
#pragma once

namespace Resources
{
  class SP_Elapsed : public SP
  {
  public:

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
