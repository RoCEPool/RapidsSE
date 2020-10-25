/*============================================================================*/
/* SP_Ref.hpp - Rrf sprint helper                                             */
/*============================================================================*/
#pragma once

namespace Entries
{
  class SP_Ref : public SPsmall
  {
  public:
    
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Ref(void);
   ~SP_Ref(void) noexcept(false);

    void vRef(Rrf wRef);
  
    Stz* sz(void);
    Stz* sz(Rrf wRef);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
