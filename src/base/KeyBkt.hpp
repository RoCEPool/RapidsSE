/*============================================================================*/
/* KeyBkt.hpp -- Key Bucket                                                   */
/*============================================================================*/
#pragma once

#include "SEtype.hpp"

class KeyBkt : public SEtype
{
  public:
  i8 m_iAsI8;
  
  KeyBkt(void);
  KeyBkt(i8 iAsI8);

  static KeyBkt tToKeyBkt(i8 iKeyHsh);
  
  // auto conversion
  operator i8(void);
  
  void operator =(i8 iAsI8);
  void operator =(KeyBkt tKeyBkt);

  i8 operator ++(void); // increment (prefix form)
  i8 operator ++(int);  // increment (postfix)

  bool operator <(i8 iAsI8);
  
  i8 iAsI8(void);
  void vAsI8(i8 iAsI8);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
