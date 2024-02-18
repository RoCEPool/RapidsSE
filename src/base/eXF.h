/*============================================================================*/
/* eXF.h - transform                                                          */
/*============================================================================*/
#pragma once

enum eXF // transform
{
  /* */ XF_NONE = 0,   // none

  /* */ XF_EPILOG_ADD_HASH_CAST,
  /* */ XF_EPILOG_ADD_INFLATE,
  /* */ XF_EPILOG_APPEND_SYNTHESIS,
  /* */ XF_EPILOG_CULL_FACET,
  /* */ XF_EPILOG_DETERMINE_COLUMNS,
  /* */ XF_EPILOG_FUSE_PROJECT,
  /* */ XF_EPILOG_HASH_COLUMNS,
  /* */ XF_EPILOG_ITEMISE_COLUMNS,
  /* */ XF_EPILOG_MAP_COLUMNS,
  /* */ XF_EPILOG_PROMOTE_FILTER,
  /* */ XF_EPILOG_REINSTATE_STAGE,
  /* */ XF_EPILOG_TYPE_CAST,
  /* */ XF_EPILOG_X_STMCOL_TO_COLNM,
  /* */ XF_EPILOG_X_TBLCOL_TO_EXPCOL,
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
  /* */ XF_FLATTEN_PURGE_WHERE,
  /* */ XF_FLATTEN_DELETE_FLAT,
  /* */ XF_FLATTEN_DERIVE_BOUNDS,
  /* */ XF_FLATTEN_ANCHOR_TABLE,
  /* */ XF_FLATTEN_JOIN_ORDERING,
  /* */ XF_FLATTEN_LEFT_SUBQUERY,
  /* */ XF_FLATTEN_PULLUP_RLNCONJ,
  /* */ XF_FLATTEN_PREDICATE_REPOSITION,
  /* */ XF_FLATTEN_SMOOSH,
  /* */ XF_FLATTEN_SMOOTH,
  /* */ XF_FLATTEN_X_EXPCOL_TO_TBLCOL,
  /* */ XF_OPTIMISE_AGGREG_TO_SORT_ORDAGG,
  /* */ XF_OPTIMISE_CULL_NONUSED_BOUND,
  /* */ XF_OPTIMISE_CULL_USED_BOUND,
  /* */ XF_OPTIMISE_CUT_SORT,
  /* */ XF_OPTIMISE_DERIVE_STREAMKEY,
  /* */ XF_OPTIMISE_DUPLICATE_FILTER,
  /* */ XF_OPTIMISE_EARLY_FILTER,
  /* */ XF_OPTIMISE_EARLY_PROJECTION,
  /* */ XF_OPTIMISE_FACTOR_BOOLPRED,
  /* */ XF_OPTIMISE_FROM_TO_OP,
  /* */ XF_OPTIMISE_GBYAGG_TO_ORDAGG,
  /* */ XF_OPTIMISE_LATE_PROJECTION,
  /* */ XF_OPTIMISE_MAX_TO_TOP,
  /* */ XF_OPTIMISE_REDUCE_GBYAGG_KEY,
  /* */ XF_OPTIMISE_SCAN_TO_INDEXSCAN,
  /* */ XF_OPTIMISE_SORTDESC_LIMIT_TO_TOP,
  /* */ XF_OPTIMISE_SORT_LIMIT_ALL_LEVELS,
  /* */ XF_OPTIMISE_STATIC_CALC,
  /* */ XF_PASSTHRU_ADJUST_SORT,
  /* */ XF_PASSTHRU_ADJUST_TOP,
  /* */ XF_PASSTHRU_TWEAK_INDEXJOIN,
  /* */ XF_PASSTHRU_TWEAK_INDEXSCAN,
  /* */ XF_PASSTHRU_TWEAK_KEYEXISTS,
  /* */ XF_PASSTHRU_TWEAK_KEYNOTEXISTS,
  /* */ XF_PROLOG_ANALYSE_EMIT,
  /* */ XF_PROLOG_ANALYSE_INDEX,
  /* */ XF_PROLOG_ANALYSE_INJECTABLE,
  /* */ XF_PROLOG_ANALYSE_SUBQUERY,
  /* */ XF_PROLOG_ANALYSE_TABLE,
   /* */ XF_PROLOG_ENSURE_LEVEL,
  /* */ XF_PROLOG_ERASE_CONSTRUCTION,
  /* */ XF_PROLOG_MOVE_ON,
  /* */ XF_PROLOG_REDUCE_NOT,
  /* */ XF_PROLOG_VALIDATE_BOARD,
  /* */ XF_PROLOG_X_SQYNM_TO_SQYNO,
  /* */ XF_PROLOG_X_COLNM_TO_EXPCOL,
  /* */ XF_PROLOG_X_RPNCALC_TO_BOOLPRED,
  /* */ XF_PROLOG_X_SCAN_TO_FROM,
  /* */ XF_SYNTHESISE_CODE,

  /* */ XF_ENUMS
};

extern Stz* aXF[XF_ENUMS];
extern Stz* aXF_desc[XF_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
