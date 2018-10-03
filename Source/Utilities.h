/*============================================================================*/
/* Utilities.h -- includes for Utilities namespace                            */
/*============================================================================*/
#pragma once

#include "MockEE.h"

//------------------------------------------------------------------------------

namespace Utilities
{
  void vCheckAssumptions(void);
}

using namespace Utilities;

//------------------------------------------------------------------------------

#include "LoadChunk.hpp"
#include "LoadMap.hpp"
#include "SubLoader.hpp"

#include "PerformanceEntry.hpp"
#include "PerformanceBuffer.hpp"
#include "PerformanceTrace.hpp"
#include "Prefetch.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
