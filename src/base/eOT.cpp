/*============================================================================*/
/* eOT.cpp -- Optimiser Transformations                                       */
/*============================================================================*/
#include "BASE.h"
#include "eOT.h"

Stz* aOT[OT_ENUMS]
{
  /*   */ "OT_NONE"_Stz,

  /*  */ "OT_CUT_SORT"_Stz,
  /*  */ "OT_DUPLICATE_FILTERS"_Stz,
  /*  */ "OT_EARLY_FILTERS"_Stz,
  /*  */ "OT_EARLY_PROJECTION"_Stz,
  /*  */ "OT_JOIN_ORDERINT"_Stz,
  /*  */ "OT_ONETIME_CALCULATIONS"_Stz,
  /*  */ "OT_POSTPONE_PROJECTIONS"_Stz,
  /*  */ "OT_REDUCE_GBYAGG_KEY"_Stz,
  /*  */ "OT_REPLACE_AGGREG_BY_SORT_ORDAGG"_Stz,
  /*  */ "OT_REPLACE_GBYAGG_BY_ORDAGG"_Stz,
  /*  */ "OT_REPLACE_MAX_BY_TOP"_Stz,
  /*  */ "OT_REPLACE_SCAN_BY_INDEXSCAN"_Stz,
  /*  */ "OT_REPLACE_SORTDESC_LIMIT_BY_TOP"_Stz,
  /*  */ "OT_REPLACE_SORT_LIMIT_ONORIGIN_BY_ALLLEVELS"_Stz,

//  /*  */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
