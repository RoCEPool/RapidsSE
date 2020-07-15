/*============================================================================*/
/* SPmed.hpp - Sprint buffers (medium m_caResult)                             */
/*============================================================================*/
#pragma once

namespace Esses
{
  static const i8 C_SPLRG_FORMAT = 32;
  static const i8 C_SPLRG_RESULT = C_MAX_STATEMENT;
  
  class SPmed : public StackObject
  {
  public:
    char m_caFormat[C_SPLRG_FORMAT];
    char m_caResult[C_SPLRG_RESULT];

    // must be last
    Footer m_xFooter;
    
  public:
    SPmed(eCN eClass);
   ~SPmed(void) noexcept(false);
  
    Stz* szResult(void);
    void vResult(Stz* szResult);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
