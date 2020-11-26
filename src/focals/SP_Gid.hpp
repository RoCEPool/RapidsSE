/*============================================================================*/
/* SP_Gid.hpp - Gid sprint helper                                             */
/*============================================================================*/
#pragma once

namespace Focals
{
  class SP_Gid : public SPsmall
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Gid(void);
   ~SP_Gid(void) noexcept(false);
  
    Stz* sz(i8 iGid);
    Stz* sz(eIK eKind, i8 iGid);
    Stz* sz(eLG eKind, i8 iGid);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
