/*============================================================================*/
/* SP.hpp - SP Utility                                                        */
/*============================================================================*/
#pragma once

namespace Esses
{
  static const i8 C_SP_FORMAT = 32;
  static const i8 C_SP_RESULT = C_4K;
  
  class SP : public Object
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
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
