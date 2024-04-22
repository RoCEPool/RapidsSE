/*============================================================================*/
/* eSF.h - symbol facet                                                       */
/*============================================================================*/
#pragma once

enum eSF // symbol facet
{
  /* */ SF_NONE = 0,    // none

  /* */ SF_AGG_FN,      // Aggregate Function  : AVG, COUNT, ... SUM
  /* */ SF_AS,          // As                  : AS

  /* */ SF_BOL_FN,      // Boolean Function    : BETWEEN, CASE ... MATCHTRAILING
  /* */ SF_BOL_LIT,     // Boolean Literal     : FALSE, TRUE
  /* */ SF_BOL_OP,      // Boolean Operator    : AND, NOT, OR
  /* */ SF_BOUND,       // Index Bound         : BOUND, LBOUND, UBOUND
  /* */ SF_BRACE,       // Brace               : {

  /* */ SF_CASE,        // Case                : CASE
  /* */ SF_CAST,        // Cast                : CAST
  /* */ SF_CASTTYPE,    // CastType            : CASTTYPE
  /* */ SF_COL_TYPE,    // Column Type         : BOOLEAN, ... VARCHAR
  /* */ SF_COMMA,       // comma               : ,
  /* */ SF_CROSS,       // Cross               : CROSS

  /* */ SF_DCM_LIT,     // Decimal Literal     : StrLib::bIsDecimal()
  /* */ SF_DIRECTION,   // Direction           : ASC, DESC

  /* */ SF_ELSE,        // Else                : ELSE
  /* */ SF_ENDIF,       // EndIf               : ENDIF
  /* */ SF_ENUM,        // Enum                : enum
  /* */ SF_EXP_COL,     // Expr Column No      : $n

  /* */ SF_FLD_NM,      // XF_Fold Name           :
  /* */ SF_FLT_LIT,     // Float Literal       : StrLib::bIsFloat()

  /* */ SF_GROUPBY,     // GroupBy             : GROUPBY

  /* */ SF_IDX_COL,     // Index Column no     : $In
  /* */ SF_IDX_NM,      // Index Name          : bIsName
  /* */ SF_INT_LIT,     // Integer Literal     : StrLib::bIsInteger()
  /* */ SF_INTERVAL_DT, // INTERVAL_DT         : INTERVAL_DT
  /* */ SF_INTERVAL_YM, // INTERVAL_YM         : INTERVAL_YM
  /* */ SF_ITM_COL,     // Itemised Col Name   : Cn_<col_nm>
  /* */ SF_IVL_FN,      // Interval Function   : extract_year etc

  /* */ SF_KEY_COL,     // Key Column no       : $Kn

  /* */ SF_LIST_CNT,    // List item count     : #n

  /* */ SF_MTH_OP,      // Math Op             : +, -, *, /

  /* */ SF_NEGATE,      // Negate              : NEGATE
  /* */ SF_NMD_COL,     // Named Column        : bIsName() but not bIs_Agg_Fn() etc
  /* */ SF_NOTE,        // Note Literal        : ...any...
  /* */ SF_NULL,        // Null                : NULL

  /* */ SF_PARENTHESIS, // Parenthesis         : (, )

  /* */ SF_RLN_OP,      // Relational Op       : <, <=, ... >

  /* */ SF_SQY_NM,      // Subquery name       : bIsName
  /* */ SF_SQY_NO,      // SubQuery No         : $Qn
  /* */ SF_SQY_OP,      // SubQuery Op         :
  /* */ SF_SQY_PRM,     // Subquery Parameter  : %n
  /* */ SF_SQY_RSL,     // SubQuery Result     : $SRn
  /* */ SF_STG_CAST,    // Stage Cast          : BROADCAST, HASHCAST
  /* */ SF_STG_NM,      // Stage No            : Sn
  /* */ SF_STR_FN,      // String Function     : SUBSTRING2 ... SUBSTRING3
  /* */ SF_STR_LIT,     // String              : 'abc'

  /* */ SF_TBL_COL,     // Table Column No     : $t.c
  /* */ SF_TBL_NM,      // Table Name          : bIsName
  /* */ SF_TBL_SYN,     // Table Synonym       : c
  /* */ SF_TBL_SYN_rep, // " (replicated)      : C
  /* */ SF_THEN,        // Then                : THEN
  /* */ SF_TYPE,        // Type                : TYPE

  /* */ SF_WHEN,        // When                : WHEN

  /* */ SF_XK_CAST,     // XK_                 : XK_BROADCAST, XK_HASHCAST, XK_UNICAST

  /* */ SF_RW_AGGREGATES, // Aggregates        : AGGREGATES

  /* */ SF_ENUMS
};

extern Stz* aSF[SF_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
