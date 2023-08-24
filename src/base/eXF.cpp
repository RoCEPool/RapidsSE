/*============================================================================*/
/* eXF.cpp - transform                                                        */
/*============================================================================*/
#include "BASE.h"
#include "eXF.h"

Stz* aXF[XF_ENUMS]
{
  /* */ "XF_NONE"_Stz,

  /* */ "XF_ADJUST_SORT"_Stz,
  /* */ "XF_ADJUST_TOP"_Stz,
  /* */ "XF_DELETE_EMPTY_WHERE"_Stz,
  /* */ "XF_DELETE_FLAT"_Stz,
  /* */ "XF_DERIVE_PROJECTION_KEY"_Stz,
  /* */ "XF_EPILOG_COLUMN_MAPPING"_Stz,
  /* */ "XF_EPILOG_DEDUPE_AS"_Stz,
  /* */ "XF_EPILOG_PROMOTE_FILTER"_Stz,
  /* */ "XF_EPILOG_REINSTATE_STAGE"_Stz,
  /* */ "XF_EPILOG_STAGE_CAST"_Stz,
  /* */ "XF_EPILOG_STMCOL_TO_COLNM"_Stz,
  /* */ "XF_EPILOG_STMCOL_TO_TBLCOL"_Stz,
  /* */ "XF_EPILOG_TBLCOL_TO_STMCOL"_Stz,
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
  /* */ "XF_FLATTEN_DERIVE_BOUNDS"_Stz,
  /* */ "XF_JOIN_ORDERING"_Stz,
  /* */ "XF_OPTIMISE_AGGREG_TO_SORT_ORDAGG"_Stz,
  /* */ "XF_OPTIMISE_CULL_NONUSED_BOUND"_Stz,
  /* */ "XF_OPTIMISE_CULL_USED_BOUND"_Stz,
  /* */ "XF_OPTIMISE_CUT_SORT"_Stz,
  /* */ "XF_OPTIMISE_DERIVE_STREAMKEY"_Stz,
  /* */ "XF_OPTIMISE_DUPLICATE_FILTER"_Stz,
  /* */ "XF_OPTIMISE_EARLY_FILTER"_Stz,
  /* */ "XF_OPTIMISE_EARLY_PROJECTION"_Stz,
  /* */ "XF_OPTIMISE_FROM_TO_OP"_Stz,
  /* */ "XF_OPTIMISE_GBYAGG_TO_ORDAGG"_Stz,
  /* */ "XF_OPTIMISE_LATE_PROJECTION"_Stz,
  /* */ "XF_OPTIMISE_MAX_TO_TOP"_Stz,
  /* */ "XF_OPTIMISE_REDUCE_KEY_GBYAGG"_Stz,
  /* */ "XF_OPTIMISE_SCAN_TO_INDEXSCAN"_Stz,
  /* */ "XF_OPTIMISE_SORTDESC_LIMIT_TO_TOP"_Stz,
  /* */ "XF_OPTIMISE_SORT_LIMIT_ALL_LEVELS"_Stz,
  /* */ "XF_OPTIMISE_STATIC_CALC"_Stz,
  /* */ "XF_PREDICATE_PULLUP"_Stz,
  /* */ "XF_PREDICATE_REPOSITION"_Stz,
  /* */ "XF_PROLOG_ADD_LEVEL"_Stz,
  /* */ "XF_PROLOG_AS_REVIEW"_Stz,
  /* */ "XF_PROLOG_COLNM_TO_STMCOL"_Stz,
  /* */ "XF_PROLOG_ERASE_CONSTRUCTION"_Stz,
  /* */ "XF_PROLOG_INDEX_ANALYSIS"_Stz,
  /* */ "XF_PROLOG_MOVE_ON"_Stz,
  /* */ "XF_PROLOG_NOT_OPS"_Stz,
  /* */ "XF_PROLOG_PROJECTION_ANALYSIS"_Stz,
  /* */ "XF_PROLOG_RPNCALC_TO_BOOLCALC"_Stz,
  /* */ "XF_PROLOG_SCAN_TO_FROM"_Stz,
  /* */ "XF_PROLOG_SUBQUERY_ANALYSIS"_Stz,
  /* */ "XF_PROLOG_TABLE_ANALYSIS"_Stz,
  /* */ "XF_SHORTEN_QUERYNNM"_Stz,
  /* */ "XF_SMOOSH"_Stz,
  /* */ "XF_SMOOTH"_Stz,
  /* */ "XF_SYNTHESISE_CODE"_Stz,
  /* */ "XF_TWEAK_INDEXJOIN"_Stz,
  /* */ "XF_TWEAK_INDEXSCAN"_Stz,
  /* */ "XF_TWEAK_KEYEXISTS"_Stz,
  /* */ "XF_TWEAK_KEYNOTEXITS"_Stz,
  /* */ "XF_UPDATE_PROJECTION"_Stz,

//  /* */ "XF_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
