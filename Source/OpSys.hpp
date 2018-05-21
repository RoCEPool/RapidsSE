/*============================================================================*/
/* OpSys.hpp -- operating system utility                                      */
/*============================================================================*/
#pragma once

namespace System
{
  class OpSys
  {
  public:
    static void vGetInfo(void);
  
    char* aMachine(void);
    char* aNodeName(void);
    char* aRelease(void);
    char* aSysName(void);
    char* aVersion(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
