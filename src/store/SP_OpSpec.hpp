/*============================================================================*/
/* SP_OpSpec.hpp - OpSpec sprint helper                                       */
/*============================================================================*/
#pragma once

namespace STORE
{
  class SP_OpSpec : public SPmedium
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_OpSpec(void);
   ~SP_OpSpec(void) noexcept(false);

    void vOpSpec(OpSpec* pOpSpec);
  
    Stz* sz(void);
    Stz* sz(OpSpec* pOpSpec);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
