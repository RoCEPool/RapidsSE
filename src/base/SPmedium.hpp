/*============================================================================*/
/* SPmedium.hpp - Sprint buffers (medium m_caResult)                             */
/*============================================================================*/
#pragma once

namespace BASE
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
    SPmedium(void);
   ~SPmedium(void) noexcept(false);
  
    void vCatf(Stz* szFormat, ...);
    void vCatf(i8 iDepth, Stz* szFormat, ...);
    void vCats(Stz* szSubstring);
    void vCats(i8 iDepth, Stz* szSubstring);
    void vInit(void);

    Stz* szResult(void);
    void vResult(Stz* szResult);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
