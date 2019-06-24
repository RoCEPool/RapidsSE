/*============================================================================*/
/* SP_Int.hpp - Int sprint helper                                             */
/*============================================================================*/
#pragma once

namespace ScanSprint
{
  class SP_Int : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Int(void);
    ~SP_Int(void) noexcept(false);
  
    Stz* sz(i8 iInt);

    Stz* szMax(void);
    Stz* szMid(void);
    Stz* szMin(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
