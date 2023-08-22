/*============================================================================*/
/* eOT.h -- Optimiser Transformations                                         */
/*============================================================================*/
#pragma once

enum eOT // Optimiser Transformations
{
  /* 00 */ OT_NONE = 0,

  /* */ OT_AGGREG_TO_SORT_ORDAGG,      // Replace: AGGREG with SORT AND ORDAGG
  /* */ OT_CUT_SORT,                   // Cut : SORT (AGGREG and SORT on SAME COLUMNS)
  /* */ OT_DUPLICATE_FILTER,           // Duplicate: Join-dependent predicate duplicates
  /* */ OT_EARLY_FILTER,               // Early: Filters
  /* */ OT_EARLY_PROJECTION,           // Early: Projections
  /* */ OT_GBYAGG_TO_ORDAGG,           // Replace: GBYAGG with ORDAGG
  /* */ OT_JOIN_ORDERING,              // Sequence: Join Ordering
  /* */ OT_LATE_PROJECTION,            // Postpone: VARCHAR Projections after re-join
  /* */ OT_MAX_TO_TOP,                 // Replace: MAX with TOP
  /* */ OT_ONETIME_CALCULATIONS,       // One-Time: CALCULATIONS
  /* */ OT_REDUCE_GBYAGG_KEY,          // Minimize: group by keys based on unique keys
  /* */ OT_SCAN_TO_INDEXSCAN,          // Replace: SCAN with INDEXSCAN
  /* */ OT_SORTDESC_LIMIT_TO_TOP,      // Replace: SORTDESC +LIMIT with TOP
  /* */ OT_SORT_LIMIT_ALL_LEVELS,      // Replace: SORT+LIMIT ON ORIGIN with SORT ON all LEVELS

  /* 15 */ OT_ENUMS,
};

extern Stz* aOT[OT_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
