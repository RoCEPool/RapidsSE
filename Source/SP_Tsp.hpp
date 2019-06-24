/*============================================================================*/
/* SP_Tsp.hpp - Tsp sprint helper                                             */
/*============================================================================*/
#pragma once

namespace ScanSprint
{
  class SP_Tsp : public Sprint
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
