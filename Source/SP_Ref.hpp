/*============================================================================*/
/* SP_Ref.hpp - Ref sprint helper                                             */
/*============================================================================*/
#pragma once

namespace Rows
{
  class SP_Ref : public SPsml
  {
  public:
    
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Ref(void);
   ~SP_Ref(void) noexcept(false);

    void vRef(Ref wRef);
  
    Stz* sz(void);
    Stz* sz(Ref wRef);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
