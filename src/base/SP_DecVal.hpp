/*============================================================================*/
/* SP_DecVal.hpp - Decimal Value sprintf helper                               */
/*============================================================================*/
#pragma once

namespace BASE
{
  class SP_DecVal : public SPmedium
  {
  public:
    
    // must be last
    Footer m_xFooter;
    
  public:
    SP_DecVal(void);
    SP_DecVal(DecVal xDecVal);
   ~SP_DecVal(void) noexcept(false);

    void vDecVal(DecVal xDecVal);
  
    Stz* sz(void);
    Stz* sz(DecVal xDecVal);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
