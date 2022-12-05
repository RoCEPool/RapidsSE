/*============================================================================*/
/* eOT.h -- Optimiser Transformations                                         */
/*============================================================================*/
#pragma once

enum eOT // Optimiser Transformations
{
  /*   */ OT_NONE = 0,

  // ADD
  /*  */ OT_A_SORT_AT_ALLLEVELS,      // Add : add sort at all levels (aggreg->sort by ordAgg->sort)

  // CUT
  /*  */ OT_C_AGGREG,                 // Cut : AGGREG's on NODE/ORIGIN after OT_R_AGGREG_ORDAGG
  /*  */ OT_C_SORT,                   // Cut : SORT (AGGREG and SORT on SAME COLUMNS)

  // DUPLICATE
  /*  */ OT_D_FILTER,                // Duplicate: Join-dependent predicate duplicates

  // EARLY
  /*  */ OT_E_FILTERS,                // Early: Filters
  /*  */ OT_E_PROJECTIONS,            // Early: Projections
  /*  */ OT_E_SORT_LIMIT,             // Early: Sort and Limit

  // MINIMIZE
  /*  */ OT_M_GROUPBY,                // Minimize: group by keys based on unique keys

  // one-time CALCULATIONS
  /*  */ OT_O_CALCULATIONS,           // One-Time: CALCULATIONS

  // Postpone
  /*  */ OT_P_PROJECTIONS,           // Postone: VARCHAR Projections after re-join

  // PULLUP
  /*  */ OT_P_FILTERS,                // Pullup: Filters
  /*  */ OP_P_SORT_LIMIT,             // Pullup: Sort

  // REPLACE
  /*  */ OT_R_AGGREG_BY_ORDAGG,       // Replace: AGGREG with ORDAGG
  /*  */ OT_R_AGGREG_BY_SORT_ORDAGG,  // Replace: AGGREG with SORT AND ORDAGG
  /*  */ OT_R_MAX_BY_TOP,             // Replace: MAX with TOP
  /*  */ OT_R_SCAN_BY_INDEXSCAN,      // Replace: SCAN with INDEXSCAN
  /*  */ OT_R_SORTDESC_LIMIT_BY_TOP,  // Replace: SORTDESC +LIMIT with TOP
  /*  */ OT_R_SORT_LIMIT_ONORIGIN_BY_ALLLEVELS,  // Replace: SORT+LIMIT ON ORIGIN with SORT ON all LEVELS

  // SEQUENCING
  /*  */ OT_S_JOIN_ORDER,             // Sequence: Join Order
};

static Stz* aOT[]
{
  /*  0 */ "NONE"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
