/*============================================================================*/
/* SPmedium.hpp - Sprint buffers (medium m_caResult)                             */
/*============================================================================*/
#pragma once

namespace Esses
{
  static const i8 C_SPMEDIUM_FORMAT = 32;
  static const i8 C_SPMEDIUM_RESULT = C_MAX_STATEMENT;
  
  class SPmedium : public StackObject
  {
  public:
    char m_caFormat[C_SPMEDIUM_FORMAT];
    char m_caResult[C_SPMEDIUM_RESULT];

    // must be last
    Footer m_xFooter;
    
  public:
    SPmedium(eCN eClass);
   ~SPmedium(void) noexcept(false);
  
    Stz* szResult(void);
    void vResult(Stz* szResult);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
