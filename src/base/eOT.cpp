/*============================================================================*/
/* eOT.cpp -- Optimiser Transformations                                       */
/*============================================================================*/
#include "BASE.h"
#include "eOT.h"

Stz* aOT[OT_ENUMS]
{
  /*   */ "NONE"_Stz,

  // CUT
  /*  */ "CUT_SORT"_Stz,

  // DUPLICATE
  /*  */ "DUP_FILTER"_Stz,

  // EARLY
  /*  */ "ELY_FILTERS"_Stz,
  /*  */ "ELY_PROJECTIONS"_Stz,

  // MINIMIZE
  /*  */ "MIN_GROUPBY"_Stz,

  // ONE-TIME CALCULATIONS
  /*  */ "OTM_CALCULATIONS"_Stz,

  // Postpone
  /*  */ "PPT_PROJECTIONS"_Stz,

  // REPLACE
  /*  */ "RPL_AGGREG_BY_ORDAGG"_Stz,
  /*  */ "RPL_AGGREG_BY_SORT_ORDAGG"_Stz,
  /*  */ "RPL_MAX_BY_TOP"_Stz,
  /*  */ "RPL_SCAN_BY_INDEXSCAN"_Stz,
  /*  */ "RPL_SORTDESC_LIMIT_BY_TOP"_Stz,
  /*  */ "RPL_SORT_LIMIT_ONORIGIN_BY_ALLLEVELS"_Stz,

  // SEQUENCING
  /*  */ "JOI_ORD"_Stz,

//  /*  */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
