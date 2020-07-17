/*============================================================================*/
/* SP_Tsp.hpp - Tsp sprint helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class SP_Tsp : public SPsmall
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Tsp(void);
    ~SP_Tsp(void) noexcept(false);
  
    Stz* sz(i8 iTme);
  
    Stz* szMax(void);
    Stz* szMid(void);
    Stz* szMin(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
