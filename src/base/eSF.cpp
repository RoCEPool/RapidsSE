/*============================================================================*/
/* eSF.cpp - symbol facet                                                     */
/*============================================================================*/

#include "BASE.h"
#include "eSF.h"

Stz* aSF[SF_ENUMS]
{
  /* */ "SF_NONE"_Stz,

  /* */ "SF_AGG_FN"_Stz,
  /* */ "SF_AS"_Stz,

  /* */ "SF_BOL_FN"_Stz,
  /* */ "SF_BOL_LIT"_Stz,
  /* */ "SF_BOL_OP"_Stz,
  /* */ "SF_BOUND"_Stz,
  /* */ "SF_BRACE"_Stz,

  /* */ "SF_CASE"_Stz,
  /* */ "SF_CAST"_Stz,
  /* */ "SF_CASTTYPE"_Stz,
  /* */ "SF_COL_TYPE"_Stz,
  /* */ "SF_COMMA"_Stz,
  /* */ "SF_CRL_KEY"_Stz,
  /* */ "SF_CROSS"_Stz,

  /* */ "SF_DCM_LIT"_Stz,
  /* */ "SF_DIRECTION"_Stz,

  /* */ "SF_ELSE"_Stz,
  /* */ "SF_ENDIF"_Stz,
  /* */ "SF_EXP_COL"_Stz,

  /* */ "SF_FLD_NM"_Stz,
  /* */ "SF_FLT_LIT"_Stz,

  /* */ "SF_GROUPBY"_Stz,

  /* */ "SF_IDX_COL"_Stz,
  /* */ "SF_IDX_NM"_Stz,
  /* */ "SF_INT_LIT"_Stz,
  /* */ "SF_INTERVAL_DT"_Stz,
  /* */ "SF_INTERVAL_YM"_Stz,
  /* */ "SF_ITM_COL"_Stz,
  /* */ "SF_IVL_FN"_Stz,

  /* */ "SF_KEY_COL"_Stz,

  /* */ "SF_LIST_CNT"_Stz,

  /* */ "SF_MTH_OP"_Stz,

  /* */ "SF_NEGATE"_Stz,
  /* */ "SF_NMD_COL"_Stz,
  /* */ "SF_NOTE"_Stz,
  /* */ "SF_NULL"_Stz,

  /* */ "SF_PARENTHESIS"_Stz,

  /* */ "SF_RLN_OP"_Stz,

  /* */ "SF_SQY_NM"_Stz,
  /* */ "SF_SQY_NO"_Stz,
  /* */ "SF_SQY_OP"_Stz,
  /* */ "SF_SQY_RSL"_Stz,
  /* */ "SF_STG_CAST"_Stz,
  /* */ "SF_STG_NM"_Stz,
  /* */ "SF_STR_FN"_Stz,
  /* */ "SF_STR_LIT"_Stz,

  /* */ "SF_TBL_COL"_Stz,
  /* */ "SF_TBL_NM"_Stz,
  /* */ "SF_TBL_SYN"_Stz,
  /* */ "SF_TBL_SYN_rep"_Stz,
  /* */ "SF_THEN"_Stz,
  /* */ "SF_TYPE"_Stz,

  /* */ "SF_WHEN"_Stz,

  /* */ "SF_XK_CAST"_Stz,

  /* */ "SF_RW_AGGREGATES"_Stz,

//  /* */ "SF_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
