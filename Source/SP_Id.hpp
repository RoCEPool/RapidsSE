/*============================================================================*/
/* SP_Id.hpp - Id sprint helper                                               */
/*============================================================================*/
#pragma once

namespace Focals
{
  class SP_Id : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Id(void);
    ~SP_Id(void) noexcept(false);
  
    Stz* sz(i8 iId);
    Stz* sz(eIK eKind, i8 iId);
    Stz* sz(eEK eKind, i8 iId);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
