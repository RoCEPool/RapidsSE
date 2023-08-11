/*============================================================================*/
/* eSF.h - symbol form                                                        */
/*============================================================================*/
#pragma once

enum eSF // symbol form
{
  /* */ SF_NONE = 0,    // none

  /* */ SF_AGG_FN,      // Aggregate Function  : AVG, COUNT, ... SUM
  /* */ SF_AS,          // As                  : AS
  /* */ SF_AS_NM,       // As Name             : bIsName() but not bIs_Agg_Fn() etc
  /* */ SF_AS_NM_syn,   // As Name (synthetic) : __COLn
  /* */ SF_BOL_FN,      // Boolean Function    : BETWEEN, CASE ... MATCHTRAILING
  /* */ SF_BOL_LIT,     // Boolean Literal     : FALSE, TRUE
  /* */ SF_BOL_OP,      // Boolean Operator    : AND, NOT, OR
  /* */ SF_BOUND,       // Index Bound         : BOUND, LBOUND, UBOUND
  /* */ SF_BRACE,       // Brace               : {

  /* */ SF_COL_NM,      // Column Name         : bIsName() but not bIs_Agg_Fn() etc
  /* */ SF_COL_NM_dis,  // " (disambiguated)   : Cn_<col_nm>
  /* */ SF_COL_NM_syn,  // " (synthetic)       : COLn
  /* */ SF_COL_NO,      // Column Number       : $n
  /* */ SF_COL_TC,      // (table) Column No   : $c.n
  /* */ SF_COL_TYPE,    // Column Type         : BOOLEAN, ... VARCHAR
  /* */ SF_COMMA,       // comma               : ,
  /* */ SF_CROSS,       // Cross               : CROSS

  /* */ SF_DCM_LIT,     // Decimal Literal     : StrLib::bIsDecimal()
  /* */ SF_DIRECTION,   // Direction           : ASC, DESC
  /* */ SF_FLD_NM,      // Fold Name           :
  /* */ SF_FLT_LIT,     // Float Literal       : StrLib::bIsFloat()
  /* */ SF_GROUPBY,     // GroupBy             : GROUPBY
  /* */ SF_IDX_COL,     // Index Column no     : $In
  /* */ SF_IDX_NM,      // Index Name          : bIsName
  /* */ SF_INT_LIT,     // Integer Literal     : StrLib::bIsInteger()
  /* */ SF_KEY_COL,     // Key Column no       : $Kn
  /* */ SF_MTH_OP,      // Math Op             : +, -, *, /
  /* */ SF_NOTE,        // Note Literal        : ...any...

  /* */ SF_PARENTHESIS, // Parenthesis         : (, )
  /* */ SF_PPE_NM,      // PipeName            : bIsName
  /* */ SF_PRM_COL,     // Parameter Column    : %n
  /* */ SF_RLN_OP,      // Relational Op       : <, <=, ... >
  /* */ SF_SQY_NO,      // SubQuery No         :
  /* */ SF_SQY_OP,      // SubQuery Op         :
  /* */ SF_SQY_RSL,     // SubQuery Result     :
  /* */ SF_STG_CAST,    // Stage Cast          : BROADCAST, HASHCAST
  /* */ SF_STG_NO,      // Stage No            : Sn
  /* */ SF_STRING,      // String              : 'abc'

  /* */ SF_TBL_NM,      // Table Name          : bIsName
  /* */ SF_TBL_SYN,     // Table Synonym       : c
  /* */ SF_TBL_SYN_rep, // " (replicated)      : C
  /* */ SF_TYPE,        // Type                : TYPE
  /* */ SF_WHEN,        // When                : WHEN
  /* */ SF_XK_CAST,     // CrossRef No         : XK_BROADCAST, XK_HASHCAST, XK_UNICAST
  /* */ SF_XRF_NO,      // CrossRef No         : #n

  /* */ SF_ENUMS
};

static Stz* aSF[]
{
  /* */ "SF_NONE"_Stz,

  /* */ "SF_AGG_FN"_Stz,
  /* */ "SF_AS"_Stz,
  /* */ "SF_AS_NM"_Stz,
  /* */ "SF_AS_NM_syn"_Stz,
  /* */ "SF_BOL_FN"_Stz,
  /* */ "SF_BOL_LIT"_Stz,
  /* */ "SF_BOL_OP"_Stz,
  /* */ "SF_BOUND"_Stz,
  /* */ "SF_BRACE"_Stz,

  /* */ "SF_COL_NM"_Stz,
  /* */ "SF_COL_NM_dis"_Stz,
  /* */ "SF_COL_NM_syn"_Stz,
  /* */ "SF_COL_NO"_Stz,
  /* */ "SF_COL_TC"_Stz,
  /* */ "SF_COL_TYPE"_Stz,
  /* */ "SF_COMMA"_Stz,
  /* */ "SF_CROSS"_Stz,

  /* */ "SF_DCM"_Stz,
  /* */ "SF_DIRECTION"_Stz,
  /* */ "SF_FLD_NM"_Stz,
  /* */ "SF_FLT_LIT"_Stz,
  /* */ "SF_GROUPBY"_Stz,
  /* */ "SF_IDX_COL"_Stz,
  /* */ "SF_IDX_NM"_Stz,
  /* */ "SF_FLT_LIT"_Stz,
  /* */ "SF_KEY_COL"_Stz,
  /* */ "SF_MTH_OP"_Stz,
  /* */ "SF_NOTE"_Stz,

  /* */ "SF_PARENTHESIS"_Stz,
  /* */ "SF_PPE_NM"_Stz,
  /* */ "SF_PRM_COL"_Stz,
  /* */ "SF_RLN_OP"_Stz,
  /* */ "SF_SQY_NO"_Stz,
  /* */ "SF_SQY_OP"_Stz,
  /* */ "SF_SQY_RSL"_Stz,
  /* */ "SF_STG_CAST"_Stz,
  /* */ "SF_STG_NO"_Stz,
  /* */ "SF_STRING"_Stz,

  /* */ "SF_TBL_NM"_Stz,
  /* */ "SF_TBL_SYN"_Stz,
  /* */ "SF_TYPE"_Stz,
  /* */ "SF_WHEN"_Stz,
  /* */ "SF_XK_CAST"_Stz,
  /* */ "SF_XRF_NO"_Stz,

  /* */ "SF_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
