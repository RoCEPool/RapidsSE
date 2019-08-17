/*============================================================================*/
/* SP_Id.hpp - Id sprint helper                                               */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class SP_Id : public Object
  {
  public:
    char m_caResult[32];

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Id(void);
    ~SP_Id(void) noexcept(false);
  
    Stz* sz(Stz* szPrefix, i8 iId);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
