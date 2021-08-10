/*============================================================================*/
/* SP_Verify.hpp - Verify Object sprint helper                                */
/*============================================================================*/
#pragma once

namespace BASE
{
  class SP_Verify : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Verify(void);
   ~SP_Verify(void) noexcept(false);
  
    Stz* sz(Object* pObject);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
