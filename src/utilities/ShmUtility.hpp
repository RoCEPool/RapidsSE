/*============================================================================*/
/* ShmUtility.hpp -- shared memory utility                                    */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class ShmUtility
  {
  public:
  
  public:
    static void vRun(int argc, Stz* argv[]);
  
    static void vAttach(void);
    static void vCreate(void);
    static void vDelete(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
