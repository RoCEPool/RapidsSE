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
  SEsubscript(i8 iLimit);
  SEsubscript(i8 iLimit, i8 iSubscript);

  // auto conversion
  operator i8(void);

  void operator =(i8 iSubscript); // assignment
  
  i8 operator ++(int); // increment (postfix form)
  bool operator <(i8 iSubscript);

  void vIncSubscript(void);

  i8   iLimit(void);
  void vLimit(i8 iLimit);

  i8   iSubscript(void);
  void vSubscript(i8 iSubscript);
  
  bool bValid(void);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
