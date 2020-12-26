/*============================================================================*/
/* SP_Timer.hpp - Timer sprint helper                           (on c stack)  */
/*============================================================================*/
#pragma once

namespace Resources
{
  class SP_Timer : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Timer(void);
   ~SP_Timer(void) noexcept(false);
  
    Stz* sz(i8 iNanoseconds);
    Stz* sz(Timer* pTimer);

  private:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
