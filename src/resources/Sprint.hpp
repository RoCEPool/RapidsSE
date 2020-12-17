/*============================================================================*/
/* Sprint.hpp - Sprint Utility                                                */
/*============================================================================*/
#pragma once

namespace Resources
{
  class Sprint : public SP
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    Sprint(eCN eClass);
    ~Sprint(void) noexcept(false);
  
    Stz* sz(void);
  
    Stz* szFormat(void);
    void vFormat(const Stz* szFormat);

    Stz* szResult(void);
    void vResult(const Stz* szResult);
  
    Stz* szNull(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
