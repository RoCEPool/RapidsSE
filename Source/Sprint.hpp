/*============================================================================*/
/* Sprint.hpp - Sprint Utility                                                */
/*============================================================================*/
#pragma once

namespace Sprinting
{
  static const i8 C_SPRINT_SIZE = 64;
  
  class Sprint : public Object
  {
  public:
  
    char m_acFormat[C_SPRINT_SIZE];
    char m_acResult[C_SPRINT_SIZE];

    // must be last
    Footer m_xFooter;
    
  public:
    Sprint(eCN eClass);
    ~Sprint() noexcept(false);
  
    Stz* sz(void);
  
    Stz* szFormat(void);
    void vFormat(const Stz* szFormat);

    Stz* szResult(void);
    void vResult(const Stz* szResult);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
