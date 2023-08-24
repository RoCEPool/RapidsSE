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
  /* */ XF_COLUMN_MAPPING,
  /* */ XF_CULL_NONUSED_BOUND,
  /* */ XF_CULL_USED_BOUND,
  /* */ XF_CUT_SORT,
  /* */ XF_DEDUPE_AS,
  /* */ XF_DELETE_EMPTY_WHERE,
  /* */ XF_DELETE_FLAT,
  /* */ XF_DERIVE_BOUNDS,
  /* */ XF_DERIVE_PROJECTION_KEY,
  /* */ XF_DERIVE_STREAM_KEY,
  /* */ XF_EARLY_PROJECTION,
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
  /* */ XF_FROM_TO_OP,
  /* */ XF_INDEX_ANALYSIS,
  /* */ XF_INDIVIDUATE_FILTER,
  /* */ XF_JOIN_ORDERING,
  /* */ XF_MOVE_ON,
  /* */ XF_NOT_OPS,
  /* */ XF_OPTIMISE_AGGREG_TO_SORT_ORDAGG,
  /* */ XF_OPTIMISE_MAX_TO_TOP,
  /* */ XF_OPTIMISE_ONETIME_CALC,
  /* */ XF_OPTIMISE_SCAN_TO_INDEXSCAN,
  /* */ XF_PREDICATE_PULL_UP,
  /* */ XF_PREDICATE_REPOSITION,
  /* */ XF_PROJECTION_ANALYSIS,
  /* */ XF_PROMOTE_GBYAGG_TO_ORDAGG,
  /* */ XF_REDUCE_KEY_GBYAGG,
  /* */ XF_REINSTATE_STAGE,
  /* */ XF_RPNCALC_TO_BOOLCALC,
  /* */ XF_SCAN_TO_TABLE,
  /* */ XF_SHORTEN_QUERYNM,
  /* */ XF_SMOOSH,
  /* */ XF_SMOOTH,
  /* */ XF_STAGE_CAST,
  /* */ XF_STMCOL_TO_COLNM,
  /* */ XF_STMCOL_TO_TBLCOL,
  /* */ XF_SUBQUERY_ANALYSIS,
  /* */ XF_SYNTHESISE_CODE,
  /* */ XF_TABLE_ANALYSIS,
  /* */ XF_TBLCOL_TO_STMCOL,
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
