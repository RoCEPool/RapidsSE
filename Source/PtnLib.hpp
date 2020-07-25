/*============================================================================*/
/* PtnLib.hpp -- partitioning utility                                         */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class PtnLib
  {
  public:
    static i8   iPartition(i8 iRunningHash, i8 nPtns);
    static void vRunning(i8* aiRunningHash, i8 iVal);
    static i8   iResetRunning(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
