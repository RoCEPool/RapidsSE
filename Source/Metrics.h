/*============================================================================*/
/* Metrics.h -- Metrics namespace                                             */
/*============================================================================*/
#pragma once

#include "Utilities.h"

//------------------------------------------------------------------------------

namespace Metrics
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

static Stz* C_TDEF_GRIDSANITY    = "__gridSanity"_Stz;
static Stz* C_TDEF_LISTCOLUMNS   = "__listColumns"_Stz;
static Stz* C_TDEF_LOCKSTATS     = "__lockStats"_Stz;
static Stz* C_TDEF_NODEINFO      = "__nodeInfo"_Stz;
static Stz* C_TDEF_PROGRESSUNITS = "__progressUnits"_Stz;
static Stz* C_TDEF_SEEKOFFSETS   = "__seekOffsets"_Stz;
static Stz* C_TDEF_SPACE         = "__space"_Stz;
static Stz* C_TDEF_TOTALUNITS    = "__totalUnits"_Stz;

#include "LockMetrics.hpp"
#include "MetricsHelper.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
