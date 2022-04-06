/*============================================================================*/
/* KeyHsh.hpp -- Key Hash                                                     */
/*============================================================================*/
#pragma once

#include "SEtype.hpp"

class KeyHsh : public SEtype
{
  public:
  i8 m_iAsI8;
  
  KeyHsh(void);
  KeyHsh(i8 iAsI8);

  static KeyHsh tToKeyHsh(i8 iKeyHsh);
  
  // auto conversion
  operator i8(void);
  
  void operator =(i8 iAsI8);
  void operator =(KeyHsh tKeyHsh);

  bool operator <(i8 iAsI8);
  
  i8 iAsI8(void);
  void vAsI8(i8 iAsI8);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
