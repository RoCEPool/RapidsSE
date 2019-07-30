/*============================================================================*/
/* Library.h -- Library namespace                                            */
/*============================================================================*/
#pragma once

#include "Correctness.h"

namespace Library
{
  void vCheckAssumptions(void);
}

using namespace Library;

//------------------------------------------------------------------------------

#include "AM.hpp"

#include "Timer.hpp"

#include "IntLib.hpp"
#include "Lock.hpp"
#include "LockTracker.hpp"
#include "Wsp_t.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
