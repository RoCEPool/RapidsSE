/*============================================================================*/
/* SP_AggTimer.hpp - AggTimer sprint helper                     (on c stack)  */
/*============================================================================*/
#pragma once

namespace Resources
{
  class SP_AggTimer : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_AggTimer(void);
   ~SP_AggTimer(void) noexcept(false);
  
    Stz* sz(AggTimer* pAggTimer);

  private:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
