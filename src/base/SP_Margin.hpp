/*============================================================================*/
/* SP_Margin.hpp - Log Margin sprintf helper                                  */
/*============================================================================*/
#pragma once

namespace BASE
{
  class SP_Margin : public SPmedium
  {
  public:
    
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Margin(void);
   ~SP_Margin(void) noexcept(false);

    void vMargin(void);
  
    Stz* sz(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
