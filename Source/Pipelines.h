/*============================================================================*/
/* Pipelines.h -- Pipelines namespace                                         */
/*============================================================================*/
#pragma once

#include "Registers.h"

//------------------------------------------------------------------------------

namespace Pipelines
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

// tags
static Stz* C_TAG_CMD       = "<cmd>"_Stz;
static Stz* C_TAG_DISTID    = "<distId>"_Stz;
static Stz* C_TAG_JOBCLASS  = "<jobClass>"_Stz;
static Stz* C_TAG_JOBKIND   = "<jobKind>"_Stz;
static Stz* C_TAG_JOBNM     = "<jobNm>"_Stz;
static Stz* C_TAG_NEXTOP    = "<nextOp>"_Stz;
static Stz* C_TAG_NEXTSTAGE = "<nextStage>"_Stz;
static Stz* C_TAG_NODENO    = "<nodeNo>"_Stz;
static Stz* C_TAG_PATH      = "<path>"_Stz;
static Stz* C_TAG_STMT      = "<stmt>"_Stz;
static Stz* C_TAG_TABLENM   = "<tableNm>"_Stz;
static Stz* C_TAG_TOPOLOGY  = "<topology>"_Stz;

#include "Op.hpp"
#include "Strand.hpp"
#include "Stage.hpp"
#include "Pipeline.hpp"


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
