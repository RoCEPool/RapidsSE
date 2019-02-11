/*============================================================================*/
/* Measurement.h -- measurement                                               */
/*============================================================================*/
#pragma once

#include "Chunks.h"

//-----------------------------------------------------------------------------
namespace Measurement
{
  void vCheckAssumptions(void);
}

using namespace Measurement;

//-----------------------------------------------------------------------------

#include "Timer.hpp"

#include "ByteRate.hpp"
#include "EnumFlags.hpp"
#include "LockTracker.hpp"
#include "Now.hpp"
#include "ResourceTracker.hpp"
#include "Statistics.hpp"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
