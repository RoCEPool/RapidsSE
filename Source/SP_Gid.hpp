/*============================================================================*/
/* SP_Gid.hpp - Gid sprint helper                                             */
/*============================================================================*/
#pragma once

namespace Focals
{
  class SP_Gid : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Gid(void);
   ~SP_Gid(void) noexcept(false);
  
    Stz* sz(i8 iId);
    Stz* sz(eIK eKind, i8 iId);
    Stz* sz(eEK eKind, i8 iId);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
