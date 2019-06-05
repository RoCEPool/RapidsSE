/*============================================================================*/
/* snFLT.hpp - Flt scan helper                                                */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class snFLT : public Scan
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    snFLT(void);
    ~snFLT(void) noexcept(false);
  
    d8 d(Stz* szIn);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
