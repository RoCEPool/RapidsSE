/*============================================================================*/
/* eOT.cpp -- Optimiser Transformations                                       */
/*============================================================================*/
#include "BASE.h"
#include "eOT.h"

Stz* aOT[OT_ENUMS]
{
  /*   */ "OT_NONE"_Stz,

  /*  */ "OT_AGGREG_TO_SORT_ORDAGG"_Stz,
  /*  */ "OT_CULL_BOUND"_Stz,
  /*  */ "OT_CUT_SORT"_Stz,
  /*  */ "OT_DERIVE_STREAMKEY"_Stz,
  /*  */ "OT_DUPLICATE_FILTER"_Stz,
  /*  */ "OT_EARLY_FILTER"_Stz,
  /*  */ "OT_EARLY_PROJECTION"_Stz,
  /*  */ "OT_FROM_TO_OP"_Stz,
  /*  */ "OT_GBYAGG_TO_ORDAGG"_Stz,
  /*  */ "OT_JOIN_ORDERING"_Stz,
  /*  */ "OT_LATE_PROJECTION"_Stz,
  /*  */ "OT_MAX_TO_TOP"_Stz,
  /*  */ "OT_REDUCE_GBYAGG_KEY"_Stz,
  /*  */ "OT_SCAN_TO_INDEXSCAN"_Stz,
  /*  */ "OT_SORTDESC_LIMIT_TO_TOP"_Stz,
  /*  */ "OT_SORT_LIMIT_ALL_LEVELS"_Stz,
  /*  */ "OT_STATIC_CALC"_Stz,

//  /*  */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
