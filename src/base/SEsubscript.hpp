/*============================================================================*/
/* SEsubscript.hpp -- subscript with limit                                    */
/*============================================================================*/
#pragma once

#include "SEtype.hpp"

class SEsubscript : public SEtype
{
  protected:

  union {
    struct {
      u8 m_uLimit     : 32 ; // 0..4Bn
      u8 m_uSubscript : 32 ; // 0..4Bn
      //               ---
      //                64
      //               ===
    };
    i8 m_iAsI8;
  };
  
  public:

  SEsubscript(void);
  SEsubscript(u8 uLimit);
  SEsubscript(u8 uLimit, u8 uSubscript);

  // auto conversion
  operator u8(void);

  void operator =(u8 uSubscript); // assignment
  
  u8 operator ++(int); // increment (postfix form)
  bool operator <(u8 uSubscript);

  void vIncSubscript(void);

  u8   uLimit(void);
  void vLimit(u8 uLimit);

  u8   uSubscript(void);
  void vSubscript(u8 uSubscript);
  
  bool bValid(void);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
