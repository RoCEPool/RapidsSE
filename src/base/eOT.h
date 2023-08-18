/*============================================================================*/
/* eOT.h -- Optimiser Transformations                                         */
/*============================================================================*/
#pragma once

enum eOT // Optimiser Transformations
{
  /* 00 */ OT_NONE = 0,

  /* 01 */ OT_CUT_SORT,                   // Cut : SORT (AGGREG and SORT on SAME COLUMNS)
  /* 02 */ OT_DUP_FILTER,                 // Duplicate: Join-dependent predicate duplicates
  /* 03 */ OT_ELY_FILTERS,                // Early: Filters
  /* 04 */ OT_EARLY_PROJECTION,           // Early: Projections
  /* 05 */ OT_REDUCE_GBYAGG_KEY,          // Minimize: group by keys based on unique keys
  /* 06 */ OT_OTM_CALCULATIONS,           // One-Time: CALCULATIONS
  /* 07 */ OT_PPT_PROJECTIONS,            // Postpone: VARCHAR Projections after re-join
  /* 08 */ OT_RPL_AGGREG_BY_SORT_ORDAGG,  // Replace: AGGREG with SORT AND ORDAGG
  /* 09 */ OT_REPLACE_GBYAGG_BY_ORDAGG,   // Replace: GBYAGG with ORDAGG
  /* 10 */ OT_RPL_MAX_BY_TOP,             // Replace: MAX with TOP
  /* 11 */ OT_REPLACE_SCAN_BY_INDEXSCAN,  // Replace: SCAN with INDEXSCAN
  /* 12 */ OT_RPL_SORTDESC_LIMIT_BY_TOP,  // Replace: SORTDESC +LIMIT with TOP
  /* 13 */ OT_RPL_SORT_LIMIT_ONORIGIN_BY_ALLLEVELS,  // Replace: SORT+LIMIT ON ORIGIN with SORT ON all LEVELS
  /* 14 */ OT_JOIN_ORDERING,              // Sequence: Join Ordering

  /* 15 */ OT_ENUMS,
};

extern Stz* aOT[OT_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
