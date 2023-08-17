/*============================================================================*/
/* eOT.h -- Optimiser Transformations                                         */
/*============================================================================*/
#pragma once

enum eOT // Optimiser Transformations
{
  /* 00 */ OT_NONE = 0,

  // CUT
  /* 01 */ OT_CUT_SORT,                   // Cut : SORT (AGGREG and SORT on SAME COLUMNS)

  // DUPLICATE
  /* 02 */ OT_DUP_FILTER,                 // Duplicate: Join-dependent predicate duplicates

  // EARLY
  /* 03 */ OT_ELY_FILTERS,                // Early: Filters
  /* 04 */ OT_ELY_PROJECTIONS,            // Early: Projections

  // MINIMIZE
  /* 05 */ OT_MIN_GROUPBY,                // Minimize: group by keys based on unique keys

  // one-time CALCULATIONS
  /* 06 */ OT_OTM_CALCULATIONS,           // One-Time: CALCULATIONS

  // Postpone
  /* 07 */ OT_PPT_PROJECTIONS,            // Postpone: VARCHAR Projections after re-join

  // REPLACE
  /* 08 */ OT_RPL_AGGREG_BY_ORDAGG,       // Replace: AGGREG with ORDAGG
  /* 09 */ OT_RPL_AGGREG_BY_SORT_ORDAGG,  // Replace: AGGREG with SORT AND ORDAGG
  /* 10 */ OT_RPL_MAX_BY_TOP,             // Replace: MAX with TOP
  /* 11 */ OT_RPL_SCAN_BY_INDEXSCAN,      // Replace: SCAN with INDEXSCAN
  /* 12 */ OT_RPL_SORTDESC_LIMIT_BY_TOP,  // Replace: SORTDESC +LIMIT with TOP
  /* 13 */ OT_RPL_SORT_LIMIT_ONORIGIN_BY_ALLLEVELS,  // Replace: SORT+LIMIT ON ORIGIN with SORT ON all LEVELS

  // SEQUENCING
  /* 14 */ OT_SEQ_JOIN_ORDER,             // Sequence: Join Order

  /* 15 */ OT_ENUMS,
};

extern Stz* aOT[OT_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
