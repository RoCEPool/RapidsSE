/*============================================================================*/
/* Magic.cpp -- Magic Functions                                               */
/*============================================================================*/

#include "Magic.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void Magic::vCheckAssumptions(void)
{
  
  ASSERT(4  == sizeof(eCN));
  ASSERT(16 == sizeof(Object));
  ASSERT(8  == sizeof(Footer));
  
  ASSERT(24 == sizeof(s));
  
  ASSERT(sizeof(s) + sizeof(Footer) + 32    == sizeof(s32));
  ASSERT(sizeof(s) + sizeof(Footer) + 256   == sizeof(s256));
  ASSERT(sizeof(s) + sizeof(Footer) + C_48K == sizeof(s48K));
  ASSERT(sizeof(s) + sizeof(Footer) + C_1M  == sizeof(s1M));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
