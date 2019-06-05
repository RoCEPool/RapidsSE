/*============================================================================*/
/* snBOL.hpp - Bol scan helper                                                */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  class snBOL : public Scan
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    snBOL(void);
    ~snBOL(void) noexcept(false);
  
    i8 i(Stz* szIn);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
