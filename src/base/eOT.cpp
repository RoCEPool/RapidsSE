/*============================================================================*/
/* eOT.cpp -- Optimiser Transformations                                       */
/*============================================================================*/
#include "BASE.h"
#include "eOT.h"

Stz* aOT[OT_ENUMS]
{
  /*   */ "OT_NONE"_Stz,

  // CUT
  /*  */ "OT_CUT_SORT"_Stz,

  // DUPLICATE
  /*  */ "OT_DUP_FILTER"_Stz,

  // EARLY
  /*  */ "OT_ELY_FILTERS"_Stz,
  /*  */ "OT_ELY_PROJECTIONS"_Stz,

  // MINIMIZE
  /*  */ "OT_MIN_GROUPBY"_Stz,

  // ONE-TIME CALCULATIONS
  /*  */ "OT_OTM_CALCULATIONS"_Stz,

  // Postpone
  /*  */ "OT_PPT_PROJECTIONS"_Stz,

  // REPLACE
  /*  */ "OT_RPL_AGGREG_BY_ORDAGG"_Stz,
  /*  */ "OT_RPL_AGGREG_BY_SORT_ORDAGG"_Stz,
  /*  */ "OT_RPL_MAX_BY_TOP"_Stz,
  /*  */ "OT_RPL_SCAN_BY_INDEXSCAN"_Stz,
  /*  */ "OT_RPL_SORTDESC_LIMIT_BY_TOP"_Stz,
  /*  */ "OT_RPL_SORT_LIMIT_ONORIGIN_BY_ALLLEVELS"_Stz,

  // SEQUENCING
  /*  */ "OT_JOI_ORD"_Stz,

//  /*  */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
