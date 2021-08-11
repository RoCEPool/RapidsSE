/*============================================================================*/
/* SP_Pointer.hpp - Pointer sprint helper                                */
/*============================================================================*/
#pragma once

namespace BASE
{
  class SP_Pointer : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Pointer(void);
   ~SP_Pointer(void) noexcept(false);
  
    Stz* sz(Object* pObject);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
