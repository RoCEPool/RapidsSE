/*============================================================================*/
/* SeeMe.hpp - see me memory marker                                           */
/*============================================================================*/
#pragma once

namespace Correctness
{
  static const u8 C_SEEME = 0x23654D5F65655323; // #See_Me#
                          //   # e M _ e e S #
  class SeeMe
  {
  private:
  union {
    u8   m_uSeeMe;
    char m_caSeeMe[8];
  };
  
  public:
    SeeMe(void);
    ~SeeMe(void) noexcept(false);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
