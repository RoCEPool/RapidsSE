/*============================================================================*/
/* eXF.h - transform                                                          */
/*============================================================================*/
#pragma once

enum eXF // transform
{
  /* */ XF_NONE = 0,   // none

  /* */ XF_ADD_LEVEL,
  /* */ XF_ADD_STAGE_CAST,
  /* */ XF_ADJUST_SORT,
  /* */ XF_ADJUST_TOP,
  /* */ XF_AS_REVIEW,
  /* */ XF_COLNM_TO_STMCOL,
  /* */ XF_CULL_USED_BOUND,
  /* */ XF_DELETE_EMPTY_WHERE,
  /* */ XF_DELETE_FLAT,
  /* */ XF_DERIVE_BOUNDS,
  /* */ XF_DERIVE_PROJECTION_KEY,
  /* */ XF_EPILOG_COLUMN_MAPPING,
  /* */ XF_EPILOG_DEDUPE_AS,
  /* */ XF_EPILOG_PROMOTE_FILTER,
  /* */ XF_EPILOG_REINSTATE_STAGE,
  /* */ XF_EPILOG_STAGE_CAST,
  /* */ XF_EPILOG_STMCOL_TO_COLNM,
  /* */ XF_EPILOG_STMCOL_TO_TBLCOL,
  /* */ XF_EPILOG_TBLCOL_TO_STMCOL,
  /* */ XF_ERASE_CONSTRUCTION,
  /* */ XF_FIX_TABLE_POS,
  /* */ XF_FLATTEN,
  /* */ XF_FLATTEN_10_F,
  /* */ XF_FLATTEN_24_W_A_C_RO,
  /* */ XF_FLATTEN_32_W_A_NC_I,
  /* */ XF_FLATTEN_34_W_A_NC_RO,
  /* */ XF_FLATTEN_40_W_L_C_E,
  /* */ XF_FLATTEN_52_W_L_NC_I_EXISTS,
  /* */ XF_FLATTEN_52_W_L_NC_I_JOIN,
  /* */ XF_FLATTEN_53_W_L_NC_NI_EXISTS,
  /* */ XF_FLATTEN_53_W_L_NC_NI_JOIN,
  /* */ XF_FLATTEN_74_H_A_NC_RO,
  /* */ XF_INDEX_ANALYSIS,
  /* */ XF_JOIN_ORDERING,
  /* */ XF_MOVE_ON,
  /* */ XF_NOT_OPS,
  /* */ XF_OPTIMISE_AGGREG_TO_SORT_ORDAGG,
  /* */ XF_OPTIMISE_CULL_BOUND,
  /* */ XF_OPTIMISE_CUT_SORT,
  /* */ XF_OPTIMISE_DERIVE_STREAMKEY,
  /* */ XF_OPTIMISE_DUPLICATE_FILTER,
  /* */ XF_OPTIMISE_EARLY_FILTER,
  /* */ XF_OPTIMISE_EARLY_PROJECTION,
  /* */ XF_OPTIMISE_FROM_TO_OP,
  /* */ XF_OPTIMISE_GBYAGG_TO_ORDAGG,
  /* */ XF_OPTIMISE_LATE_PROJECTION,
  /* */ XF_OPTIMISE_MAX_TO_TOP,
  /* */ XF_OPTIMISE_REDUCE_GBYAGG_KEY,
  /* */ XF_OPTIMISE_SCAN_TO_INDEXSCAN,
  /* */ XF_OPTIMISE_SORTDESC_LIMIT_TO_TOP,
  /* */ XF_OPTIMISE_SORT_LIMIT_ALL_LEVELS,
  /* */ XF_OPTIMISE_STATIC_CALC,
  /* */ XF_PREDICATE_PULL_UP,
  /* */ XF_PREDICATE_REPOSITION,
  /* */ XF_PROJECTION_ANALYSIS,
  /* */ XF_RPNCALC_TO_BOOLCALC,
  /* */ XF_SCAN_TO_TABLE,
  /* */ XF_SHORTEN_QUERYNM,
  /* */ XF_SMOOSH,
  /* */ XF_SMOOTH,
  /* */ XF_SUBQUERY_ANALYSIS,
  /* */ XF_SYNTHESISE_CODE,
  /* */ XF_TABLE_ANALYSIS,
  /* */ XF_TWEAK_INDEXJOIN,
  /* */ XF_TWEAK_INDEXSCAN,
  /* */ XF_TWEAK_KEYEXISTS,
  /* */ XF_TWEAK_KEYNOTEXISTS,
  /* */ XF_UPDATE_PROJECTION,

  /* */ XF_ENUMS
};

extern Stz* aXF[XF_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
