/*============================================================================*/
/* eXF.cpp - transform                                                        */
/*============================================================================*/
#include "BASE.h"
#include "eXF.h"

Stz* aXF[XF_ENUMS]
{
  /* */ "XF_NONE"_Stz,

  /* */ "XF_EPILOG_ADD_SYNTHESIS"_Stz,
  /* */ "XF_EPILOG_FUSE_PROJECT"_Stz,
  /* */ "XF_EPILOG_HASH_COLUMNS"_Stz,
  /* */ "XF_EPILOG_MAO_COLUMNS"_Stz,
  /* */ "XF_EPILOG_COLUMNS_REQUIRED"_Stz,
  /* */ "XF_EPILOG_ITEMISE_COLUMNS"_Stz,
  /* */ "XF_EPILOG_ITEMSTRIP_COLNM"_Stz,
  /* */ "XF_EPILOG_PROMOTE_FILTER"_Stz,
  /* */ "XF_EPILOG_REINSTATE_STAGE"_Stz,
  /* */ "XF_EPILOG_ADD_HASH_CAST"_Stz,
  /* */ "XF_EPILOG_STMCOL_TO_COLNM"_Stz,
  /* */ "XF_EPILOG_STMCOL_TO_TBLCOL"_Stz,
  /* */ "XF_EPILOG_TBLCOL_TO_STMCOL"_Stz,

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
  /* */ "XF_FLATTEN_DELETE_EMPTY_WHERE"_Stz,
  /* */ "XF_FLATTEN_DELETE_FLAT"_Stz,
  /* */ "XF_FLATTEN_DERIVE_BOUNDS"_Stz,
  /* */ "XF_FLATTEN_FIX_TABLE_POS"_Stz,
  /* */ "XF_FLATTEN_PREDICATE_PULLUP"_Stz,
  /* */ "XF_FLATTEN_PREDICATE_REPOSITION"_Stz,
  /* */ "XF_FLATTEN_SMOOSH"_Stz,
  /* */ "XF_FLATTEN_SMOOTH"_Stz,
  /* */ "XF_FLATTEN_UPDATE_PROJECTION"_Stz,

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

  /* */ "XF_PASSTHRU_ADJUST_SORT"_Stz,
  /* */ "XF_PASSTHRU_ADJUST_TOP"_Stz,
  /* */ "XF_PASSTHRU_TWEAK_INDEXJOIN"_Stz,
  /* */ "XF_PASSTHRU_TWEAK_INDEXSCAN"_Stz,
  /* */ "XF_PASSTHRU_TWEAK_KEYEXISTS"_Stz,
  /* */ "XF_PASSTHRU_TWEAK_KEYNOTEXITS"_Stz,

  /* */ "XF_PROLOG_ENSURE_LEVEL"_Stz,
  /* */ "XF_PROLOG_AS_REVIEW"_Stz,
  /* */ "XF_PROLOG_COLNM_TO_STMCOL"_Stz,
  /* */ "XF_PROLOG_ERASE_CONSTRUCTION"_Stz,
  /* */ "XF_PROLOG_INDEX_ANALYSIS"_Stz,
  /* */ "XF_PROLOG_MOVE_ON"_Stz,
  /* */ "XF_PROLOG_NOT_OPS"_Stz,
  /* */ "XF_PROLOG_PROJECTION_ANALYSIS"_Stz,
  /* */ "XF_PROLOG_RPNCALC_TO_BOOLCALC"_Stz,
  /* */ "XF_PROLOG_SCAN_TO_FROM"_Stz,
  /* */ "XF_PROLOG_SHORTEN_QUERYNNM"_Stz,
  /* */ "XF_PROLOG_SUBQUERY_ANALYSIS"_Stz,
  /* */ "XF_PROLOG_TABLE_ANALYSIS"_Stz,

  /* */ "XF_SYNTHESISE_CODE"_Stz,

//  /* */ "XF_ENUMS"_Stz
};

Stz* aXF_desc[XF_ENUMS]
{
  /* */ "none"_Stz, // XF_NONE

  /* */ "<to be supplied>"_Stz, // XF_EPILOG_ADD_SYNTHESIS
  /* */ "<to be supplied>"_Stz, // XF_EPILOG_FUSE_PROJECT
  /* */ "<to be supplied>"_Stz, // XF_EPILOG_MAO_COLUMNS
  /* */ "<to be supplied>"_Stz, // XF_EPILOG_COLUMNS_REQUIRED
  /* */ "<to be supplied>"_Stz, // XF_EPILOG_ITEMISE_COLUMNS
  /* */ "<to be supplied>"_Stz, // XF_EPILOG_ITEMSTRIP_COLNM
  /* */ "<to be supplied>"_Stz, // XF_EPILOG_PROMOTE_FILTER
  /* */ "<to be supplied>"_Stz, // XF_EPILOG_REINSTATE_STAGE
  /* */ "<to be supplied>"_Stz, // XF_EPILOG_ADD_HASH_CAST
  /* */ "<to be supplied>"_Stz, // XF_EPILOG_STMCOL_TO_COLNM
  /* */ "<to be supplied>"_Stz, // XF_EPILOG_STMCOL_TO_TBLCOL
  /* */ "<to be supplied>"_Stz, // XF_EPILOG_TBLCOL_TO_STMCOL

  /* */ "<to be supplied>"_Stz, // XF_FLATTEN
  /* */ "<to be supplied>"_Stz, // XF_FLATTEN_10_F
  /* */ "<to be supplied>"_Stz, // XF_FLATTEN_24_W_A_C_RO
  /* */ "<to be supplied>"_Stz, // XF_FLATTEN_32_W_A_NC_I
  /* */ "<to be supplied>"_Stz, // XF_FLATTEN_34_W_A_NC_RO
  /* */ "<to be supplied>"_Stz, // XF_FLATTEN_40_W_L_C_E
  /* */ "<to be supplied>"_Stz, // XF_FLATTEN_52_W_L_NC_I_EXISTS
  /* */ "<to be supplied>"_Stz, // XF_FLATTEN_52_W_L_NC_I_JOIN
  /* */ "<to be supplied>"_Stz, // XF_FLATTEN_53_W_L_NC_NI_EXISTS
  /* */ "<to be supplied>"_Stz, // XF_FLATTEN_53_W_L_NC_NI_JOIN
  /* */ "<to be supplied>"_Stz, // XF_FLATTEN_74_H_A_NC_RO
  /* */ "<to be supplied>"_Stz, // XF_FLATTEN_DELETE_EMPTY_WHERE
  /* */ "<to be supplied>"_Stz, // XF_FLATTEN_DELETE_FLAT
  /* */ "<to be supplied>"_Stz, // XF_FLATTEN_DERIVE_BOUNDS
  /* */ "<to be supplied>"_Stz, // XF_FLATTEN_FIX_TABLE_POS
  /* */ "<to be supplied>"_Stz, // XF_FLATTEN_PREDICATE_PULLUP
  /* */ "<to be supplied>"_Stz, // XF_FLATTEN_PREDICATE_REPOSITION
  /* */ "<to be supplied>"_Stz, // XF_FLATTEN_SMOOSH
  /* */ "<to be supplied>"_Stz, // XF_FLATTEN_SMOOTH
  /* */ "<to be supplied>"_Stz, // XF_FLATTEN_UPDATE_PROJECTION

  /* */ "<to be supplied>"_Stz, // XF_JOIN_ORDERING

  /* */ "<to be supplied>"_Stz, // XF_OPTIMISE_AGGREG_TO_SORT_ORDAGG
  /* */ "<to be supplied>"_Stz, // XF_OPTIMISE_CULL_NONUSED_BOUND
  /* */ "<to be supplied>"_Stz, // XF_OPTIMISE_CULL_USED_BOUND
  /* */ "<to be supplied>"_Stz, // XF_OPTIMISE_CUT_SORT
  /* */ "<to be supplied>"_Stz, // XF_OPTIMISE_DERIVE_STREAMKEY
  /* */ "<to be supplied>"_Stz, // XF_OPTIMISE_DUPLICATE_FILTER
  /* */ "<to be supplied>"_Stz, // XF_OPTIMISE_EARLY_FILTER
  /* */ "<to be supplied>"_Stz, // XF_OPTIMISE_EARLY_PROJECTION
  /* */ "<to be supplied>"_Stz, // XF_OPTIMISE_FROM_TO_OP
  /* */ "<to be supplied>"_Stz, // XF_OPTIMISE_GBYAGG_TO_ORDAGG
  /* */ "<to be supplied>"_Stz, // XF_OPTIMISE_LATE_PROJECTION
  /* */ "<to be supplied>"_Stz, // XF_OPTIMISE_MAX_TO_TOP
  /* */ "<to be supplied>"_Stz, // XF_OPTIMISE_REDUCE_KEY_GBYAGG
  /* */ "<to be supplied>"_Stz, // XF_OPTIMISE_SCAN_TO_INDEXSCAN
  /* */ "<to be supplied>"_Stz, // XF_OPTIMISE_SORTDESC_LIMIT_TO_TOP
  /* */ "<to be supplied>"_Stz, // XF_OPTIMISE_SORT_LIMIT_ALL_LEVELS
  /* */ "<to be supplied>"_Stz, // XF_OPTIMISE_STATIC_CALC

  /* */ "<to be supplied>"_Stz, // XF_PASSTHRU_ADJUST_SORT
  /* */ "<to be supplied>"_Stz, // XF_PASSTHRU_ADJUST_TOP
  /* */ "<to be supplied>"_Stz, // XF_PASSTHRU_TWEAK_INDEXJOIN
  /* */ "<to be supplied>"_Stz, // XF_PASSTHRU_TWEAK_INDEXSCAN
  /* */ "<to be supplied>"_Stz, // XF_PASSTHRU_TWEAK_KEYEXISTS
  /* */ "<to be supplied>"_Stz, // XF_PASSTHRU_TWEAK_KEYNOTEXITS

  /* */ "<to be supplied>"_Stz, // XF_PROLOG_ENSURE_LEVEL
  /* */ "<to be supplied>"_Stz, // XF_PROLOG_AS_REVIEW
  /* */ "<to be supplied>"_Stz, // XF_PROLOG_COLNM_TO_STMCOL
  /* */ "<to be supplied>"_Stz, // XF_PROLOG_ERASE_CONSTRUCTION
  /* */ "<to be supplied>"_Stz, // XF_PROLOG_INDEX_ANALYSIS
  /* */ "<to be supplied>"_Stz, // XF_PROLOG_MOVE_ON
  /* */ "<to be supplied>"_Stz, // XF_PROLOG_NOT_OPS
  /* */ "<to be supplied>"_Stz, // XF_PROLOG_PROJECTION_ANALYSIS
  /* */ "<to be supplied>"_Stz, // XF_PROLOG_RPNCALC_TO_BOOLCALC
  /* */ "<to be supplied>"_Stz, // XF_PROLOG_SCAN_TO_FROM
  /* */ "<to be supplied>"_Stz, // XF_PROLOG_SHORTEN_QUERYNNM
  /* */ "<to be supplied>"_Stz, // XF_PROLOG_SUBQUERY_ANALYSIS
  /* */ "<to be supplied>"_Stz, // XF_PROLOG_TABLE_ANALYSIS

  /* */ "<to be supplied>"_Stz, // XF_SYNTHESISE_CODE

// /* */        // XF_ENUMS"_Stz
};



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
