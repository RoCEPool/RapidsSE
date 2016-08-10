/*============================================================================*/
/* mn.hpp - magic number                                                      */
/*============================================================================*/
#pragma once

#include "Types.h"

namespace Types
{
  static const u4 MAGIC = 0X4347414d;
  //static const u8 HEADER = 0x5E4845414445525E;  // ^HEADER^

  class mn
  {
  private:
    
    u4 m_uMagicNo;
    
  public:
    mn(void);
    ~mn(void);
    
    //void vAssertMagic(void) const;
    void vSetMagic(void);
    i8   iMagic(void) const;
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/