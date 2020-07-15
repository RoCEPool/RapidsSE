/*============================================================================*/
/* SPsml.hpp - Sprint buffers (small m_caResult)                              */
/*============================================================================*/
#pragma once

namespace Esses
{
  static const i8 C_SPSML_FORMAT = 32;
  static const i8 C_SPSML_RESULT = C_4KB;
  
  class SPsml : public StackObject
  {
  public:
    char m_caFormat[C_SPSML_FORMAT];
    char m_caResult[C_SPSML_RESULT];

    // must be last
    Footer m_xFooter;
    
  public:
    SPsml(eCN eClass);
   ~SPsml(void) noexcept(false);
  
    Stz* szResult(void);
    void vResult(Stz* szResult);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
