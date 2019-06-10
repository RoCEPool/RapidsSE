/*============================================================================*/
/* snINT.hpp - Int scan helper                                                */
/*============================================================================*/
#pragma once

namespace Transform
{
  class snINT : public Scan
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    snINT(void);
    ~snINT(void) noexcept(false);
  
    i8 i(Stz* szIn);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
