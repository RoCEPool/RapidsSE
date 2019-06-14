/*============================================================================*/
/* Sprint.hpp - Sprint Utility                                                */
/*============================================================================*/
#pragma once

namespace Transform
{
  static const i8 C_SPRINT_SIZE = 64;
  
  class Sprint : public Object
  {
  public:
  
    // defult buffers
    char m_caFormat[C_SPRINT_SIZE];
    char m_caResult[C_SPRINT_SIZE];

    // must be last
    Footer m_xFooter;
    
  public:
    Sprint(eCN eClass);
    ~Sprint(void) noexcept(false);
  
    static void vInto(Stz* szTgt, void* avSrc, Column* pCol);

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
