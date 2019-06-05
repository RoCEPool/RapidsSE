/*============================================================================*/
/* snIVL.hpp - Ivl scan helper                                                */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class snIVL : public Scan
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    snIVL(void);
    ~snIVL(void) noexcept(false);
  
    i8 i(Stz* szIn);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
