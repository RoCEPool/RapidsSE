/*============================================================================*/
/* SP.hpp - SP Utility                                                        */
/*============================================================================*/
#pragma once

namespace Resources
{
  static const i8 C_SP_FORMAT = 32;
  static const i8 C_SP_RESULT = C_MAX_STATEMENT;
  
  class SP : public StackObject
  {
  public:
  
    // defult buffers
    char m_caFormat[C_SP_FORMAT];
    char m_caResult[C_SP_RESULT];

    // must be last
    Footer m_xFooter;
    
  public:
    SP(eCN eClass);
   ~SP(void) noexcept(false);
  
    Stz* szResult(void);
    void vResult(Stz* szResult);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
