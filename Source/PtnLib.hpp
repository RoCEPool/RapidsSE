/*============================================================================*/
/* PtnLib.hpp -- partitioning utility                                         */
/*============================================================================*/
#pragma once

namespace Library
{
  class PtnLib
  {
  public:
    static tPN  tPtnNo(i8 iRunningHash, i8 nPartitionsInGrid);
    static void vRunning(i8* aiRunningHash, i8 iVal);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
