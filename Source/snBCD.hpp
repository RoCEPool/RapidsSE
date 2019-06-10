/*============================================================================*/
/* snBCD.hpp - Bcd scan helper                                                */
/*============================================================================*/
#pragma once

namespace Transform
{
  class snBCD : public Scan
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    snBCD(void);
    ~snBCD(void) noexcept(false);
  
    i8 i(Stz* szIn);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
