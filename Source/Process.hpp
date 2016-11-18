/*============================================================================*/
/* Process.hpp -- process utility                                             */
/*============================================================================*/
#pragma once

#include "Common.h"

namespace Common
{
  class Process : public Static
  {
  public:
    static i8   iGetPid(void);
    static bool bKillPid(i8 iPid);
    static bool bTestPid(i8 iPid);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
