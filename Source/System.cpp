/*============================================================================*/
/* System.cpp -- System Functions                                             */
/*============================================================================*/

#include "System.h"

#include "Pool.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void System::vCheckAssumptions(void)
{
  ASSERT(Process::bTestPid(Process::iGetPid()));
  ASSERT(4  == sizeof(eCN));
  ASSERT(16 == sizeof(Object));
  ASSERT(8  == sizeof(Footer));
  
  ASSERT(24 == sizeof(StrVbl));
  
  ASSERT(sizeof(StrVbl) + sizeof(Footer) + 32    == sizeof(s32));
  ASSERT(sizeof(StrVbl) + sizeof(Footer) + 256   == sizeof(s256));
  ASSERT(sizeof(StrVbl) + sizeof(Footer) + C_48K == sizeof(s48K));
  ASSERT(sizeof(StrVbl) + sizeof(Footer) + C_1M  == sizeof(s1M));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
