/*============================================================================*/
/* eXF.cpp - transform                                                        */
/*============================================================================*/
#include "BASE.h"
#include "eXF.h"

Stz* aXF[XF_ENUMS]
{
  /* */ "XF_NONE"_Stz,

  /* */ "XF_ADD_LEVEL"_Stz,
  /* */ "XF_AS_REVIEW"_Stz,
  /* */ "XF_COLNM_TO_STMCOL"_Stz,
  /* */ "XF_COLUMN_MAPPING"_Stz,
  /* */ "XF_CULL_NONUSED_BOUND"_Stz,
  /* */ "XF_CULL_USED_BOUND"_Stz,
  /* */ "XF_CUT_SORT"_Stz,
  /* */ "XF_DEDUPE_AS"_Stz,
  /* */ "XF_DELETE_EMPTY_WHERE"_Stz,
  /* */ "XF_DELETE_FLAT"_Stz,
  /* */ "XF_DELETE_UNNEEDED"_Stz,
  /* */ "XF_DERIVE_BOUNDS"_Stz,
  /* */ "XF_DERIVE_PROJECTION_KEY"_Stz,
  /* */ "XF_EARLY_PROJECTION"_Stz,
  /* */ "XF_FIX_TABLE_POS"_Stz,
  /* */ "XF_FLATTEN"_Stz,
  /* */ "XF_FLATTEN_10_F"_Stz,
  /* */ "XF_FLATTEN_24_W_A_C_RO"_Stz,
  /* */ "XF_FLATTEN_32_W_A_NC_I"_Stz,
  /* */ "XF_FLATTEN_34_W_A_NC_RO"_Stz,
  /* */ "XF_FLATTEN_40_W_L_C_E"_Stz,
  /* */ "XF_FLATTEN_52_W_L_NC_I_EXISTS"_Stz,
  /* */ "XF_FLATTEN_52_W_L_NC_I_JOIN"_Stz,
  /* */ "XF_FLATTEN_53_W_L_NC_NI_EXISTS"_Stz,
  /* */ "XF_FLATTEN_53_W_L_NC_NI_JOIN"_Stz,
  /* */ "XF_FLATTEN_74_H_A_NC_RO"_Stz,
  /* */ "XF_INDEX_ANALYSIS"_Stz,
  /* */ "XF_INDIVIDUATE_FILTER"_Stz,
  /* */ "XF_JOIN_ORDERING"_Stz,
  /* */ "XF_MORPH_SCAN_TO_INDEXSCAN"_Stz,
  /* */ "XF_MORPH_TABLE_TO_EEOP"_Stz,
  /* */ "XF_MOVE_ON"_Stz,
  /* */ "XF_NOT_OPS"_Stz,
  /* */ "XF_OPTIMISE_MAX_TO_TOP"_Stz,
  /* */ "XF_OPTIMISE_ONETIME_CALC"_Stz,
  /* */ "XF_PREDICATE_PULL_UP"_Stz,
  /* */ "XF_PREDICATE_REPOSITION"_Stz,
  /* */ "XF_PROJECTION_ANALYSIS"_Stz,
  /* */ "XF_PROMOTE_GBYAGG_TO_ORDAGG"_Stz,
  /* */ "XF_REDUCE_KEY_GBYAGG"_Stz,
  /* */ "XF_REINSTATE_STAGE"_Stz,
  /* */ "XF_RPNCALC_TO_BOOLCALC"_Stz,
  /* */ "XF_SCAN_TO_TABLE"_Stz,
  /* */ "XF_SHORTEN_QUERYNNM"_Stz,
  /* */ "XF_SMOOSH"_Stz,
  /* */ "XF_SMOOTH"_Stz,
  /* */ "XF_STAGE_CAST"_Stz,
  /* */ "XF_STMCOL_TO_COLNM"_Stz,
  /* */ "XF_STMCOL_TO_TBLCOL"_Stz,
  /* */ "XF_SUBQUERY_ANALYSIS"_Stz,
  /* */ "XF_SYNTHESISE_CODE"_Stz,
  /* */ "XF_TABLE_ANALYSIS"_Stz,
  /* */ "XF_TBLCOL_TO_STMCOL"_Stz,
  /* */ "XF_UPDATE_PROJECTION"_Stz,

//  /* */ "XF_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
