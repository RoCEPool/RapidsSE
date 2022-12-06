/*============================================================================*/
/* eOT.h -- Optimiser Transformations                                         */
/*============================================================================*/
#pragma once

enum eOT // Optimiser Transformations
{
  /*  */ OT_NONE = 0,

  // CUT
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
  /*   */ "NONE"_Stz,

  // CUT
  /*  */ "CUT_SRT"_Stz,   // CUT_GBY_SRT

  // DUPLICATE
  /*  */ "DUP_FTR"_Stz,  // DUP_FTR_PRD

  // EARLY
  /*  */ "ELY_FTR"_Stz,  // ELY_ROW_FTR
  /*  */ "ELY_PRJ"_Stz,  // ELY_COL_PRJ
  /*  */ "ELY_SLT"_Stz,  // ELY_SRT_LMT

  // MINIMIZE
  /*  */ "MIN_GBY"_Stz,  // MIN_GBY_COL

  // ONE-TIME CALCULATIONS
  /*  */ "OTM_CAL"_Stz,  // OTM_LIT_CAL

  // Postpone
  /*  */ "PST_PRJ"_Stz,

  // REPLACE
  /*  */ "AGG_OAG"_Stz, // AGGREGG -> ORDAGG                     // AGG-ORD_AGG
  /*  */ "AGG_SOA"_Stz, // AGGREG -> SORT + ORDAGG               // AGG-SRT_AGG
  /*  */ "MAX_TOP"_Stz, // MAX    -> TOP                         // MAX-TOP_ONE
  /*  */ "SCN_ISN"_Stz, // SCAN    -> INDEXSCAN                  // SCN-IDX_SCN
  /*  */ "SDL_TOP"_Stz, // SORT-DESC-LIMIT -> TOP                // SDL-TOP_NUM
  /*  */ "SLO_SLA"_Stz, // SORT-LIMIT-ORIGIN -> SORT-LIMIT-ALL   // SLT-SLT-ALL

  // SEQUENCING
  /*  */ "JOI_ORD"_Stz,                                         // SEQ_JOI_ORD
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
