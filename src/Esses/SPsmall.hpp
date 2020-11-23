/*============================================================================*/
/* SPsmall.hpp - Sprint buffers (small m_caResult)                              */
/*============================================================================*/
#pragma once

namespace Esses
{
  static const i8 C_SPSMALL_FORMAT = 32;
  static const i8 C_SPSMALL_RESULT = C_4KB;
  
  class SPsmall : public StackObject
  {
  public:
    char m_caFormat[C_SPSMALL_FORMAT];
    char m_caResult[C_SPSMALL_RESULT];

    // must be last
    Footer m_xFooter;
    
  public:
    SPsmall(void);
   ~SPsmall(void) noexcept(false);
  
    Stz* sz(void);
    
    Stz* szFormat(void);
    void vFormat(const Stz* szFormat);

    void vReset(void);
  
    Stz* szResult(void);
    void vResult(Stz* szResult);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
