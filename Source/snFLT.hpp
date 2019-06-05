/*============================================================================*/
/* snFLT.hpp - Flt sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class snFLT : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    snFLT(void);
    ~snFLT(void) noexcept(false);
  
    Stz* sz(d8 dFlt);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
