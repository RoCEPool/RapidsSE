/*============================================================================*/
/* eSF.h - symbol form                                                        */
/*============================================================================*/
#pragma once

enum eSF // symbol form
{
  /* */ SF_NONE = 0,    // none

  /* */ SF_AGG_FN,      // Aggregate Function  : avg, count, ... sum
  /* */ SF_AS_NM,       // As Name             : bIsName() but not bIsAggFn() etc
  /* */ SF_AS_NM_syn,   // As Name (synthetic) : __COLn
  /* */ SF_BOL_FN,      // Boolean Function    : between, case ... matchTrailing
  /* */ SF_BOL_OP,      // Boolean Operator    : and, not, or
  /* */ SF_BOUND,       // Index Bound         : BOUND, LBOUND, UBOUND

  /* */ SF_COL_NM,      // Column Name         : bIsName() but not bIsAggFn() etc
  /* */ SF_COL_NM_dis,  // " (disambiguated)   : Cn_<col_nm>
  /* */ SF_COL_NO,      // Column Number       : $n
  /* */ SF_COL_TC,      // (table) Column No   : $c.n

  /* */ SF_DIRECT,      // Direction           : asc, desc
  /* */ SF_IDX_COL,     // Index Column no     : $In
  /* */ SF_KEY_COL,     // Key Column no       : $Kn
  /* */ SF_NUMBER,      // Numeric Literal     : StrLib::bIsDecimal/Float/Integer()

  /* */ SF_OTHER,       // other

  /* */ SF_PRM_COL,     // Parameter Column    : %n
  /* */ SF_RLN_OP,      // Relational Op       : <, <=, ... >
  /* */ SF_SQY_KEY,     // SubQuery Correl Key :
  /* */ SF_SQY_NO,      // SubQuery No         :
  /* */ SF_SQY_RSL,     // SubQuery Result     :
  /* */ SF_TBL_SYN,     // Table Synonym       : c
  /* */ SF_XRF_NO,      // CrossRef No         : #n

  /* */ SF_ENUMS
};

static Stz* aSF[]
{
  /* */ "SF_NONE"_Stz,

  /* */ "SF_AGG_FN"_Stz,
  /* */ "SF_AS_NM"_Stz,
  /* */ "SF_AS_NM_syn"_Stz,
  /* */ "SF_BOL_FN"_Stz,
  /* */ "SF_BOL_OP"_Stz,
  /* */ "SF_BOUND"_Stz,

  /* */ "SF_COL_NM"_Stz,
  /* */ "SF_COL_NM_dis"_Stz,
  /* */ "SF_COL_NO"_Stz,
  /* */ "SF_COL_TC"_Stz,

  /* */ "SF_DIRECT"_Stz,
  /* */ "SF_IDX_COL"_Stz,
  /* */ "SF_KEY_COL"_Stz,
  /* */ "SF_NUMBER"_Stz,

  /* */ "SF_OTHER"_Stz,

  /* */ "SF_PRM_COL"_Stz,
  /* */ "SF_RLN_OP"_Stz,
  /* */ "SF_SQY_KEY"_Stz,
  /* */ "SF_SQY_NO"_Stz,
  /* */ "SF_SQY_RSL"_Stz,
  /* */ "SF_TBL_SYN"_Stz,
  /* */ "SF_XRF_NO"_Stz,

  /* */ "SF_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
