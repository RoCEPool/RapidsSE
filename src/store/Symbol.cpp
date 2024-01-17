/*============================================================================*/
/* Symbol.cpp -- Symbolic Value                                               */
/*============================================================================*/

#include "Card.hpp"

#include "SketchTkn.hpp"
#include "TypeSystem.hpp"

/*----------------------------------------------------------------------------*/
/*-- Lifecycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Symbol)


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Symbol::Symbol(void)
: Object(CN_SYMBOL)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vFacetReset();
  vCastReset();

  m_pProperties = nullptr;

  _checkInitialised_(CN_SYMBOL);

  RASSERT(!bHasFacets());

  _postcondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Symbol::Symbol(eSF eFacet, i8 iInt)
    : Object(CN_SYMBOL)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vFacetReset();
	//m_xTypeId;
  m_pProperties = nullptr;

  _checkInitialised_(CN_SYMBOL);

  vFacet(eFacet, iInt);

  RASSERT(bHasFacets());

  _postcondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Symbol::Symbol(eSF eFacet, Stz* szChrSeq)
    : Object(CN_SYMBOL)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vFacetReset();
	//m_xTypeId;
  m_pProperties = nullptr;

  _checkInitialised_(CN_SYMBOL);

  vFacet(eFacet, szChrSeq);

  RASSERT(bHasFacets());

  _postcondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Symbol::~Symbol(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  delete m_pProperties;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* evaluate sub-query op                                                      */
/*----------------------------------------------------------------------------*/
eQO Symbol::eEvalSubQryOp(Stz* szArg)
{
  _static_

       if (szArg->bSame("exists"_Stz))    return QO_EXISTS_OP;
  else if (szArg->bSame("notExists"_Stz)) return QO_NOT_EXISTS_OP;
  else if (szArg->bSame("in"_Stz))        return QO_IN_OP;
  else if (szArg->bSame("notIn"_Stz))     return QO_NOT_IN_OP;

  else return QO_NONE;
}

/*----------------------------------------------------------------------------*/
/* evaluate relational op                                                     */
/*----------------------------------------------------------------------------*/
eRO Symbol::eEvalRelnOp(Stz* szArg)
{
  _static_

       if (szArg->bSame("<"_Stz))   return RO_LESS_THAN;
  else if (szArg->bSame("<="_Stz))  return RO_LESS_EQUAL;
  else if (szArg->bSame("="_Stz))   return RO_EQUAL_TO;
  else if (szArg->bSame("=="_Stz))  return RO_EQUAL_TO;
  else if (szArg->bSame("!="_Stz))  return RO_NOT_EQUAL;
  else if (szArg->bSame(">="_Stz))  return RO_GREATER_EQUAL;
  else if (szArg->bSame(">"_Stz))   return RO_GREATER_THAN;

  else return RO_NONE;
}

/*----------------------------------------------------------------------------*/
/* evaluate symbol form of given arg                                          */
/*----------------------------------------------------------------------------*/
eSF Symbol::eEvalSymbol(Stz* szArg)
{
  _static_

  if (bIs_Agg_Fn(szArg))       return SF_AGG_FN;
  if (bIs_As(szArg))           return SF_AS;
  if (bIs_Bol_Fn(szArg))       return SF_BOL_FN;
  if (bIs_Bol_Lit(szArg))      return SF_BOL_LIT;
  if (bIs_Bol_Op(szArg))       return SF_BOL_OP;
  if (bIs_Bound(szArg))        return SF_BOUND;
  if (bIs_Brace(szArg))        return SF_BRACE;
  if (bIs_Case(szArg))         return SF_CASE;
  if (bIs_Cast(szArg))         return SF_CAST;
  if (bIs_CastType(szArg))     return SF_CASTTYPE;
  if (bIs_Col_Type(szArg))     return SF_COL_TYPE;
  if (bIs_Comma(szArg))        return SF_COMMA;
  if (bIs_Cross(szArg))        return SF_CROSS;
  if (bIs_Dcm_Lit(szArg))      return SF_DCM_LIT;
  if (bIs_Direction(szArg))    return SF_DIRECTION;
  if (bIs_Else(szArg))         return SF_ELSE;
  if (bIs_EndIf(szArg))        return SF_ENDIF;
  if (bIs_Flt_Lit(szArg))      return SF_FLT_LIT;
  if (bIs_GroupBy(szArg))      return SF_GROUPBY;
  if (bIs_Idx_Col(szArg))      return SF_IDX_COL;
  if (bIs_Int_Lit(szArg))      return SF_INT_LIT;
  if (bIs_Interval_DT(szArg))  return SF_INTERVAL_DT;
  if (bIs_Interval_YM(szArg))  return SF_INTERVAL_YM;
  if (bIs_Ivl_Fn(szArg))       return SF_IVL_FN;
  if (bIs_Key_Col(szArg))      return SF_KEY_COL;
  if (bIs_Mth_Op(szArg))       return SF_MTH_OP;
  if (bIs_Negate(szArg))       return SF_NEGATE;
  if (bIs_Null(szArg))         return SF_NULL;
  if (bIs_Parenthesis(szArg))  return SF_PARENTHESIS;
  if (bIs_Pay_Col(szArg))      return SF_PAY_COL;
  if (bIs_Rln_Op(szArg))       return SF_RLN_OP;
  if (bIs_Sqy_Nm(szArg))       return SF_SQY_NM;
  if (bIs_Sqy_Prm(szArg))      return SF_SQY_PRM;
  if (bIs_Sqy_Op(szArg))       return SF_SQY_OP;
  if (bIs_Sqy_Rsl(szArg))      return SF_SQY_RSL;
  if (bIs_Stg_Cast(szArg))     return SF_STG_CAST;
  if (bIs_Exp_Col(szArg))      return SF_EXP_COL;
  if (bIs_Str_Fn(szArg))       return SF_STR_FN;
  if (bIs_String(szArg))       return SF_STR_LIT;
  if (bIs_Tbl_Col(szArg))      return SF_TBL_COL;
  if (bIs_Then(szArg))         return SF_THEN;
  if (bIs_Type(szArg))         return SF_TYPE;
  if (bIs_When(szArg))         return SF_WHEN;
  if (bIs_List_cnt(szArg))       return SF_LIST_CNT;

  return SF_NONE;
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-synonym                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vInit_Tbl_Syn(char cTblSyn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vFacetSprint(SF_TBL_SYN,"%c"_Stz, cTblSyn);

  RASSERT(bIs_Tbl_Syn());
}

/*----------------------------------------------------------------------------*/
/* is this symbol an aggregate function?                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Agg_Fn(Stz* szArg)
{
  _static_

       if (szArg->bSame("AVG"_Stz))      return true;
  else if (szArg->bSame("COUNT"_Stz))    return true;
  else if (szArg->bSame("COUNTALL"_Stz)) return true;
  else if (szArg->bSame("MAX"_Stz))      return true;
  else if (szArg->bSame("MIN"_Stz))      return true;
//else if (szArg->bSame("STDDEV"_Stz))   return true;
  else if (szArg->bSame("SUM"_Stz))      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol AS?                                                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_As(Stz* szArg)
{
  _static_

  if (szArg->bSame("AS"_Stz))      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a boolean function?                                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Bol_Fn(Stz* szArg)
{
  _static_

       if (szArg->bSame("BETWEEN"_Stz))         return true;
  else if (szArg->bSame("INLIST"_Stz))          return true;
  else if (szArg->bSame("ISNULL"_Stz))          return true;
  else if (szArg->bSame("LIKE2"_Stz))           return true;
  else if (szArg->bSame("LIKE3"_Stz))           return true;
  else if (szArg->bSame("MATCHLEADING"_Stz))    return true;
  else if (szArg->bSame("MATCHMIDDLE"_Stz))     return true;
  else if (szArg->bSame("MATCHMIDDLE1"_Stz))    return true;
  else if (szArg->bSame("MATCHTRAILING"_Stz))   return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a boolean literal?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Bol_Lit(Stz* szArg)
{
  _static_

       if (szArg->bSame("false"_Stz))  return true;
  else if (szArg->bSame("true"_Stz))   return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a boolean operator?                                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Bol_Op(Stz* szArg)
{
  _static_

       if (szArg->bSame("and"_Stz))  return true;
  else if (szArg->bSame("not"_Stz))  return true;
  else if (szArg->bSame("or"_Stz))   return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a bound?                                                    */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Bound(Stz* szArg)
{
  _static_

       if (szArg->bSame("BOUND"_Stz))   return true;
  else if (szArg->bSame("LBOUND"_Stz))  return true;
  else if (szArg->bSame("UBOUND"_Stz))  return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a comma separator?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Brace(Stz* szArg)
{
  _static_

  if (szArg->bSame("{"_Stz))       return true;
  if (szArg->bSame("}"_Stz))       return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol CASE?                                                       */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Case(Stz* szArg)
{
  _static_

  if (szArg->bSame("CASE"_Stz))      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol CAST?                                                       */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Cast(Stz* szArg)
{
  _static_

  if (szArg->bSame("CAST"_Stz))      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol CASTTYPE?                                                   */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_CastType(Stz* szArg)
{
  _static_

  if (szArg->bSame("CASTTYPE"_Stz))      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a sort direction?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Col_Type(Stz* szArg)
{
  _static_

       if (szArg->bSame("BOOLEAN"_Stz))      return true;
  else if (szArg->bSame("DATE"_Stz))         return true;
  else if (szArg->bSame("DECIMAL"_Stz))      return true;
  else if (szArg->bSame("INTEGER"_Stz))      return true;
  else if (szArg->bSame("INTERVAL"_Stz))     return true;
  else if (szArg->bSame("FLOAT"_Stz))        return true;
  else if (szArg->bSame("TIME"_Stz))         return true;
  else if (szArg->bSame("TIMESTAMP"_Stz))    return true;
  else if (szArg->bSame("VARCHAR"_Stz))      return true;

  return false;
}


/*----------------------------------------------------------------------------*/
/* is this symbol a comma separator?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Comma(Stz* szArg)
{
  _static_

  if (szArg->bSame(","_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a comma separator?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Cross(Stz* szArg)
{
  _static_

  if (szArg->bSame("CROSS"_Stz))       return true;
  if (szArg->bSame("LEFT_OUTER"_Stz))  return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a decimal literal?                                                 */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Dcm_Lit(Stz* szArg)
{
  _static_

  if (StrLib::bIsDecimal(szArg->azChar()))  return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a sort direction?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Direction(Stz* szArg)
{
  _static_

  if (szArg->bSame("ASC"_Stz))   return true;
  else if (szArg->bSame("DESC"_Stz))  return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol ELSE?                                                       */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Else(Stz* szArg)
{
  _static_

  if (szArg->bSame("ELSE"_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol ENDIF                                                       */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_EndIf(Stz* szArg)
{
  _static_

  if (szArg->bSame("ENDIF"_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a stream-column?  (i.e. $n)                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Exp_Col(Stz* szArg)
{
  _static_

  char acFirst  = szArg->azChar()[0];
  char acSecond = szArg->azChar()[1];

  if ('$' == acFirst &&
      StrLib::bIsDigit(acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a sort direction?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Fld_Nm(Stz* szArg)
{
  _static_
  UNUSED(szArg);

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this a float literal?                                                   */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Flt_Lit(Stz* szArg)
{
  _static_

  if (StrLib::bIsFloat(szArg->azChar()))  return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol GROUPBY ?                                                   */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_GroupBy(Stz* szArg)
{
  _static_

  if (szArg->bSame("GROUPBY"_Stz))   return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for an index-column?  (i.e. $In)                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Idx_Col(Stz* szArg)
{
  _static_

  char acFirst  = szArg->azChar()[0];
  char acSecond = szArg->azChar()[1];
  char acThird  = szArg->azChar()[2];
  char acFourth = szArg->azChar()[3];

  if ('$' == acFirst
      && ('I' == acSecond || 'i' == acSecond)
      && StrLib::bIsDigit(acThird)
      && ! acFourth)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a index name?                                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Idx_Nm(Stz* szArg)
{
  _static_

  if (! bPossibleNm(szArg))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this an integer literal?                                                */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Int_Lit(Stz* szArg)
{
  _static_

  if (StrLib::bIsInteger(szArg->azChar()))  return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol INTERVAL_DT ?                                               */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Interval_DT(Stz* szArg)
{
  _static_

  if (szArg->bSame("INTERVAL_DT"_Stz))   return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol INTERVAL_YM ?                                               */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Interval_YM(Stz* szArg)
{
  _static_

  if (szArg->bSame("INTERVAL_YM"_Stz))   return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a column name?                                                     */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Itm_Col(Stz* szArg)
{
  _static_

  if (! bPossibleNm(szArg))
    return false;

  if (bIs_As(szArg))          return false;
  if (bIs_Bol_Fn(szArg))      return false;
  if (bIs_Bol_Lit(szArg))     return false;
  if (bIs_Bol_Op(szArg))      return false;
  if (bIs_Bound(szArg))       return false;
  if (bIs_Case(szArg))        return false;
  if (bIs_Cast(szArg))        return false;
  if (bIs_CastType(szArg))    return false;
  if (bIs_Col_Type(szArg))    return false;
  if (bIs_Cross(szArg))       return false;
  if (bIs_Direction(szArg))   return false;
  if (bIs_Else(szArg))        return false;
  if (bIs_EndIf(szArg))       return false;
  if (bIs_GroupBy(szArg))     return false;
  if (bIs_Interval_DT(szArg)) return false;
  if (bIs_Interval_YM(szArg)) return false;
  if (bIs_Ivl_Fn(szArg))      return false;
  if (bIs_Negate(szArg))      return false;
  if (bIs_Null(szArg))        return false;
  if (bIs_Sqy_Nm(szArg))      return false;
  if (bIs_Sqy_Op(szArg))      return false;
  if (bIs_Stg_Cast(szArg))    return false;
  if (bIs_Str_Fn(szArg))      return false;
  if (bIs_Then(szArg))        return false;
  if (bIs_Type(szArg))        return false;
  if (bIs_When(szArg))        return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this symbol an interval function?                                       */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Ivl_Fn(Stz* szArg)
{
  _static_

  if ( szArg->bSame("extract_year"_Stz)   ||
			 szArg->bSame("extract_month"_Stz)  ||
			 szArg->bSame("extract_day"_Stz)    ||
			 szArg->bSame("extract_hour"_Stz)   ||
			 szArg->bSame("extract_minute"_Stz) ||
			 szArg->bSame("extract_second"_Stz)
		)
	return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a list count  (i.e. #n)                               */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_List_cnt(Stz* szChrSeq)
{
  _static_

  char acFirst  = szChrSeq->azChar()[0];
  char acSecond = szChrSeq->azChar()[1];

  if ('#' == acFirst &&
      StrLib::bIsDigit(acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a key symbol? (i.e. $Kn)                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Key_Col(Stz* szArg)
{
  _static_

  char acFirst  = szArg->azChar()[0];
  char acSecond = szArg->azChar()[1];
  char acThird  = szArg->azChar()[2];
  char acFourth = szArg->azChar()[3];

  if ('$' == acFirst
      && ('K' == acSecond || 'k' == acSecond)
      && StrLib::bIsDigit(acThird)
      && ! acFourth)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a math operator ?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Mth_Op(Stz* szArg)
{
  _static_

  if (szArg->bSame("+"_Stz))     return true;
  if (szArg->bSame("-"_Stz))     return true;
  if (szArg->bSame("*"_Stz))     return true;
  if (szArg->bSame("/"_Stz))     return true;
  if (szArg->bSame("%"_Stz))     return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol NEGATE ?                                                    */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Negate(Stz* szArg)
{
  _static_

  if (szArg->bSame("NEGATE"_Stz))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a column name?                                                     */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Nmd_Col(Stz* szChrSeq)
{
  _static_

  if (! bPossibleNm(szChrSeq))
    return false;

  if (bIs_As(szChrSeq))          return false;
  if (bIs_Bol_Fn(szChrSeq))      return false;
  if (bIs_Bol_Lit(szChrSeq))     return false;
  if (bIs_Bol_Op(szChrSeq))      return false;
  if (bIs_Bound(szChrSeq))       return false;
  if (bIs_Case(szChrSeq))        return false;
  if (bIs_Cast(szChrSeq))        return false;
  if (bIs_CastType(szChrSeq))    return false;
  if (bIs_Col_Type(szChrSeq))    return false;
  if (bIs_Cross(szChrSeq))       return false;
  if (bIs_Direction(szChrSeq))   return false;
  if (bIs_Else(szChrSeq))        return false;
  if (bIs_EndIf(szChrSeq))       return false;
  if (bIs_GroupBy(szChrSeq))     return false;
  if (bIs_Interval_DT(szChrSeq)) return false;
  if (bIs_Interval_YM(szChrSeq)) return false;
  if (bIs_Ivl_Fn(szChrSeq))      return false;
  if (bIs_Negate(szChrSeq))      return false;
  if (bIs_Null(szChrSeq))        return false;
  if (bIs_Sqy_Nm(szChrSeq))      return false;
  if (bIs_Sqy_Op(szChrSeq))      return false;
  if (bIs_Stg_Cast(szChrSeq))    return false;
  if (bIs_Str_Fn(szChrSeq))      return false;
  if (bIs_Then(szChrSeq))        return false;
  if (bIs_Type(szChrSeq))        return false;
  if (bIs_When(szChrSeq))        return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a math operator ?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_None(Stz* szArg)
{
  _static_

  char acFirst  = szArg->azChar()[0];

  if (acFirst)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a note ?                                                    */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Note(Stz* szArg)
{
  _static_
  UNUSED(szArg);

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this symbol NULL ?                                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Null(Stz* szArg)
{
  _static_

  if (szArg->bSame("NULL"_Stz))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a parenthesis?                                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Parenthesis(Stz* szArg)
{
  _static_

  if (szArg->bSame("("_Stz))     return true;
  if (szArg->bSame(")"_Stz))     return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a payload symbol? (i.e. $Pn)                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Pay_Col(Stz* szArg)
{
  _static_

  char acFirst  = szArg->azChar()[0];
  char acSecond = szArg->azChar()[1];
  char acThird  = szArg->azChar()[2];
  char acFourth = szArg->azChar()[3];

  if ('$' == acFirst
      && ('P' == acSecond || 'P' == acSecond)
      && StrLib::bIsDigit(acThird)
      && ! acFourth)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a relational-operator?                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Ppe_Nm(Stz* szArg)
{
  _static_

  if (! bPossibleNm(szArg))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a relational-operator?                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Rln_Op(Stz* szArg)
{
  _static_

  if (RO_NONE != eEvalRelnOp(szArg))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is value a __subquery or $Qn like name?                                    */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Nm(BASE::Stz *szArg)
{
  _static_

  if (! szArg)
    return false;

  if (StrLib::bBeginsWith(szArg->azChar(), "__subquery"))
    return true;

  char acFirst  = szArg->azChar()[0];
  char acSecond = szArg->azChar()[1];
  char acThird  = szArg->azChar()[2];

  if ('$' == acFirst
      && 'Q' == acSecond
      && StrLib::bIsDigit(acThird))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a subQuery-operator?                                  */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Op(Stz* szArg)
{
  _static_

       if (szArg->bSame("exists"_Stz))    return true;
  else if (szArg->bSame("in"_Stz))        return true;
  else if (szArg->bSame("notExists"_Stz)) return true;
  else if (szArg->bSame("notIn"_Stz))     return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a correlation-parameter?  (i.e. %n)                   */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Prm(Stz* szArg)
{
  _static_

  char acFirst  = szArg->azChar()[0];
  char acSecond = szArg->azChar()[1];
  char acThird  = szArg->azChar()[2];

  if ('%' == acFirst
      && StrLib::bIsDigit(acSecond)
      && ! acThird)
    return true;
  else
    return false;

}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a subQuery-column?  (i.e. $Rn)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Rsl(Stz* szArg)
{
  _static_

  char acFirst  = szArg->azChar()[0];
  char acSecond = szArg->azChar()[1];
  char acThird  = szArg->azChar()[2];
  char acFourth = szArg->azChar()[3];

  if (   '$' == acFirst
      && 'R' == acSecond
      && StrLib::bIsDigit(acThird)
      && ! acFourth)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a sort direction?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Stg_Cast(Stz* szArg)
{
  _static_

       if (szArg->bSame("BROADCAST"_Stz))   return true;
  else if (szArg->bSame("HASHCAST"_Stz))    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is value a __subQuery like name?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Stg_Nm(BASE::Stz *szArg)
{
  _static_

  char acFirst  = szArg->azChar()[0];
  char acSecond = szArg->azChar()[1];

  if ('S' == acFirst
      && StrLib::bIsDigit(acSecond))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a string function?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Str_Fn(Stz* szArg)
{
  _static_

       if (szArg->bSame("SUBSTRING2"_Stz))      return true;
  else if (szArg->bSame("SUBSTRING3"_Stz))      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a table-column (i.e. $c.n)                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_String(Stz* szArg)
{
  _static_

  char acFirst  = szArg->azChar()[0];

  if ('\'' == acFirst)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a table-column (i.e. $c.n)                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Tbl_Col(Stz* szArg)
{
  _static_

  char acFirst  = szArg->azChar()[0];
  char acSecond = szArg->azChar()[1];
  char acThird  = szArg->azChar()[2];
  char acFourth = szArg->azChar()[3];

  if ('$' == acFirst
      && StrLib::bIsAlpha(acSecond)
      && '.' == acThird
      && StrLib::bIsDigit(acFourth))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a column name?                                                     */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Tbl_Nm(Stz* szArg)
{
  _static_

  if (! bPossibleNm(szArg))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol a table synonym?  (i.e. c)                                */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Tbl_Syn(Stz* szArg)
{
  _static_

  char acFirst  = szArg->azChar()[0];
  char acSecond = szArg->azChar()[1];

  if (   StrLib::bIsAlpha(acFirst)
         && '\0' == acSecond)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol a table synonym?  (i.e. C)                               */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Tbl_Syn_rep(Stz* szArg)
{
  _static_

  char acFirst  = szArg->azChar()[0];
  char acSecond = szArg->azChar()[1];

  if (   StrLib::bIsUpperCase(acFirst)
         && '\0' == acSecond)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol THEN?                                                       */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Then(Stz* szArg)
{
  _static_

  if (szArg->bSame("THEN"_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol TYPE?                                                       */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Type(Stz* szArg)
{
  _static_

  if (szArg->bSame("TYPE"_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a comma separator?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_When(Stz* szArg)
{
  _static_

  if (szArg->bSame("WHEN"_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a comma separator?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Xk_Cast(Stz* szArg)
{
  _static_

  if (szArg->bSame("XK_BROADCAST"_Stz))   return true;
  if (szArg->bSame("XK_HASHCAST"_Stz))    return true;
  if (szArg->bSame("XK_UNICAST"_Stz))     return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* strip disambiguation                                                       */
/*----------------------------------------------------------------------------*/
bool Symbol::bPossibleNm(Stz* szChrSeq)
{
  _static_

  if (! StrLib::bIsSqlName(szChrSeq->azChar()))
    return false;

  if (ST_NONE != SketchTkn::eEvalSketch(szChrSeq))
    return false;

  if (bIs_Exp_Col(szChrSeq))  return false;
  if (bIs_Tbl_Col(szChrSeq))  return false;
  if (bIs_Sqy_Prm(szChrSeq))  return false;
  if (bIs_Sqy_Rsl(szChrSeq))  return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* are the two given facets the same?                                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bSame(Facet* aFacet_a, Facet* aFacet_b)
{
  _static_

  if (aFacet_a->m_eFacet != aFacet_b->m_eFacet)
    return false;

  if (! aFacet_a->m_sChrSeq.bSame(aFacet_b->m_sChrSeq))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* convert type name to typeId                                                */
/*----------------------------------------------------------------------------*/
TypeId Symbol::xTo_TypeId(Stz* szTypeName)
{
  _static_

	return TypeId::xIdOf("rapids"_Stz, szTypeName);
}

/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append facet                                                               */
/*----------------------------------------------------------------------------*/
void Symbol::vAppendFacet(Facet* aFacet_arg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vAppendFacet(aFacet_arg->m_eFacet, aFacet_arg->m_sChrSeq);
}

/*----------------------------------------------------------------------------*/
/* append facet                                                               */
/*----------------------------------------------------------------------------*/
void Symbol::vAppendFacet(eSF eFacet_arg, Stz* szChrSeq_arg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacet(eFacet_arg));
  vFacetAppend(eFacet_arg, szChrSeq_arg);
}

/*----------------------------------------------------------------------------*/
/* append facets                                                              */
/*----------------------------------------------------------------------------*/
void Symbol::vAppendFacets(Symbol* pSymbol_arg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  i8 nFacets_arg = pSymbol_arg->m_nFacets;
  for (i8 i_Facet_arg = 0; i_Facet_arg < nFacets_arg; i_Facet_arg++) {
    Facet* aFacet_arg = pSymbol_arg->aFacet(i_Facet_arg);

    if (bHasFacet(aFacet_arg->m_eFacet))
      continue;

    vFacetAppend(aFacet_arg->m_eFacet, aFacet_arg->m_sChrSeq);
  }
}

/*----------------------------------------------------------------------------*/
/* get cast                                                                   */
/*----------------------------------------------------------------------------*/
Cast Symbol::xCast(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  return m_xCast;
}

/*----------------------------------------------------------------------------*/
/* set cast                                                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vCast(Cast xCast)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_xCast = xCast;
}

/*----------------------------------------------------------------------------*/
/* reset cast                                                                 */
/*----------------------------------------------------------------------------*/
void Symbol::vCastReset(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_xCast.m_xTypeId = TypeId();

  m_xCast.m_nTypeLength   = C_UNKNOWN;
  m_xCast.m_nTypeScale    = C_UNKNOWN;
  m_xCast.m_bTypeNullable = true;
}

/*----------------------------------------------------------------------------*/
/* set cast & facets from arg                                                 */
/*----------------------------------------------------------------------------*/
void Symbol::vCastFacets(Symbol* pSymbol_arg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_xCast = pSymbol_arg->m_xCast;
  vFacetConcat(pSymbol_arg);
}

/*----------------------------------------------------------------------------*/
/* set cast, facets and properties from arg                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vCastFacetsPpy(Symbol* pSymbol_arg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vCastFacets(pSymbol_arg);
  Y_vAll(pSymbol_arg);
}

/*----------------------------------------------------------------------------*/
/* append facet                                                               */
/*----------------------------------------------------------------------------*/
void Symbol::vConcatFacet(eSF eFacet_arg, Stz* szChrSeq_arg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vFacetRotateSet(eFacet_arg,szChrSeq_arg);
}

/*----------------------------------------------------------------------------*/
/* set cast & facets from arg                                                 */
/*----------------------------------------------------------------------------*/
void Symbol::vConcatFacets(Symbol* pSymbol_arg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vFacetConcat(pSymbol_arg);
}

/*----------------------------------------------------------------------------*/
/* delete facet                                                               */
/*----------------------------------------------------------------------------*/
void Symbol::vDeleteFacet(eSF eFacet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  i8 i_Facet_cut = i_Facet(eFacet);
  if (C_UNKNOWN == i_Facet_cut)
    return;

  vFacetCut(i_Facet_cut);
}

/*----------------------------------------------------------------------------*/
/* delete facet                                                               */
/*----------------------------------------------------------------------------*/
void Symbol::vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  i8 i_Facet_also = i_Facet(eAlsoFacet,cTblSyn);
  if (C_UNKNOWN == i_Facet_also)
    return;

  i8 i_Facet_cut = i_Facet(eFacet);
  if (C_UNKNOWN == i_Facet_cut)
    return;

  vFacetCut(i_Facet_cut);
}

/*----------------------------------------------------------------------------*/
/* get column type                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vDump(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  s4K sProperties;
  Y_vTo_String(sProperties);

  vTrace("Symbol : %s %p\n"_Stz
         ,szWhatIs()
         ,this
         );

  for (i8 i_Facet = 0; i_Facet < m_nFacets; i_Facet++)
    vTrace("%s%-12s:%lld %-18s~%lld\n"_Stz
           ,Stz::szTab(1)
           ,aSF[xaFacet[i_Facet].m_eFacet]
           ,(i8)xaFacet[i_Facet].m_eFacet
           ,*szSF_chrSeq(xaFacet[i_Facet].m_eFacet)->azChar() ? szSF_chrSeq(xaFacet[i_Facet].m_eFacet) : "??Symbol"_Stz
           ,i_Facet
           );

  vTrace("%s{%s}\n"_Stz
         ,Stz::szTab(1)
         ,sProperties.szStz()
         );
}

/*----------------------------------------------------------------------------*/
/* evaluate relational op                                                     */
/*----------------------------------------------------------------------------*/
eRO Symbol::eEvalRlnOp(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  eRO eRelnOp = eEvalRelnOp(xaFacet[0].m_sChrSeq);

  return eRelnOp;
}

/*----------------------------------------------------------------------------*/
/* evaluate sub-query op                                                      */
/*----------------------------------------------------------------------------*/
eQO Symbol::eEvalSqyOp(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  eQO eSubQryOp = eEvalSubQryOp(xaFacet[0].m_sChrSeq);

  return eSubQryOp;
}

/*----------------------------------------------------------------------------*/
/* return facet                                                               */
/*----------------------------------------------------------------------------*/
Facet* Symbol::aFacet(eSF eFacet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  i8 i_Facet = this->i_Facet(eFacet);
  if (C_UNKNOWN == i_Facet)
    return nullptr;

  return &xaFacet[i_Facet];
}

/*----------------------------------------------------------------------------*/
/* return facet                                                               */
/*----------------------------------------------------------------------------*/
Facet* Symbol::aFacet(i8 i_Facet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(i_Facet < m_nFacets);
  return &xaFacet[i_Facet];
}

/*----------------------------------------------------------------------------*/
/* return symbol facet                                                        */
/*----------------------------------------------------------------------------*/
eSF Symbol::eFacet(i8 i_Facet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  //RASSERT(i_Facet < m_nFacets);
  if (! m_nFacets)
    return SF_NONE;

  return xaFacet[i_Facet].m_eFacet;
}

/*----------------------------------------------------------------------------*/
/* scan for symbol facet                                                      */
/*----------------------------------------------------------------------------*/
i8 Symbol::i_Facet(eSF eFacet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  for (i8 i_Facet = 0; i_Facet < m_nFacets; i_Facet++) {
    if (eFacet == xaFacet[i_Facet].m_eFacet)
      return i_Facet;
  }
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* scan for symbol facet                                                      */
/*----------------------------------------------------------------------------*/
Stz* Symbol::szFacet(eSF eFacet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  for (i8 i_Facet = 0; i_Facet < m_nFacets; i_Facet++) {
    if (eFacet == xaFacet[i_Facet].m_eFacet)
      return xaFacet[i_Facet].m_sChrSeq;
  }

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* scan for symbol facet                                                      */
/*----------------------------------------------------------------------------*/
i8 Symbol::i_Facet(eSF eFacet, char cTblSyn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  for (i8 i_Facet = 0; i_Facet < m_nFacets; i_Facet++) {
    if (eFacet  == xaFacet[i_Facet].m_eFacet &&
        '$'     == xaFacet[i_Facet].m_sChrSeq.azChar()[0] &&
        cTblSyn == xaFacet[i_Facet].m_sChrSeq.azChar()[1])
      return i_Facet;
  }
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* get facet                                                                 */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet(eSF eFacet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  switch (eFacet) {

    case SF_TBL_SYN_rep: vFacet_Tbl_Syn_rep(); break;

    default: NEVER_REACH;
  }

  RASSERT(bHasFacets());

}

/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet(eSF eFacet, char cChr)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  switch (eFacet) {

    case SF_TBL_SYN: vFacet_Tbl_Syn(cChr); break;

    default: NEVER_REACH;
  }

  RASSERT(bHasFacets());

}

/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet(eSF eFacet, char cChr, i8 iInt)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  switch (eFacet) {

    case SF_TBL_COL:
      vFacet_Tbl_Col(cChr, iInt);    break;

    default: NEVER_REACH;
  }

  RASSERT(bHasFacets());

}

/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet(eSF eFacet, i8 iInt)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  switch (eFacet) {

    case SF_EXP_COL:    vFacet_Stm_Col(iInt);    break;
    case SF_IDX_COL:    vFacet_Idx_Col(iInt);    break;
    case SF_INT_LIT:    vFacet_Int_Lit(iInt);    break;
    case SF_ITM_COL:    vFacet_Itm_Col(iInt);    break;
    case SF_KEY_COL:    vFacet_Key_Col(iInt);    break;
    case SF_NMD_COL:    vFacet_Nmd_Col(iInt);    break;
    case SF_PAY_COL:    vFacet_Pay_Col(iInt);    break;
    case SF_SQY_PRM:    vFacet_Sqy_Prm(iInt);    break;
    case SF_SQY_RSL:    vFacet_Sqy_Rsl(iInt);    break;
    case SF_SQY_NM:     vFacet_Sqy_Nm(iInt);     break;
    case SF_STG_NO:     vFacet_Stg_Nm(iInt);     break;
    case SF_LIST_CNT:   vFacet_List_Cnt(iInt);   break;

    default: NEVER_REACH;
  }

  RASSERT(bHasFacets());

}

/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet(eSF eFacet, i8 iInt, Symbol* pSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  switch (eFacet) {

    case SF_ITM_COL:
      vFacet_Itm_Col(iInt, pSymbol); break;

    default: NEVER_REACH;
  }

  RASSERT(bHasFacets());

}

/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet(eSF eFacet, Stz* szChrSeq)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  switch (eFacet) {

    case SF_AGG_FN:      vFacet_Agg_Fn(szChrSeq);      break;
    case SF_AS:          vFacet_As(szChrSeq);          break;
    case SF_BOL_FN:      vFacet_Bol_Fn(szChrSeq);      break;
    case SF_BOL_LIT:     vFacet_Bol_Lit(szChrSeq);     break;
    case SF_BOL_OP:      vFacet_Bol_Op(szChrSeq);      break;
    case SF_BOUND:       vFacet_Bound(szChrSeq);       break;
    case SF_BRACE:       vFacet_Brace(szChrSeq);       break;
    case SF_CASE:        vFacet_Case(szChrSeq);        break;
    case SF_CAST:        vFacet_Cast(szChrSeq);        break;
    case SF_CASTTYPE:    vFacet_CastType(szChrSeq);    break;
    case SF_COL_TYPE:    vFacet_Col_Type(szChrSeq);    break;
    case SF_COMMA:       vFacet_Comma(szChrSeq);       break;
    case SF_CROSS:       vFacet_Cross(szChrSeq);       break;
    case SF_DCM_LIT:     vFacet_Dcm_Lit(szChrSeq);     break;
    case SF_DIRECTION:   vFacet_Direction(szChrSeq);   break;
    case SF_ELSE:        vFacet_Else(szChrSeq);        break;
    case SF_ENDIF:       vFacet_EndIf(szChrSeq);       break;
    case SF_EXP_COL:     vFacet_Exp_Col(szChrSeq);     break;
    case SF_FLD_NM:      vFacet_Fld_Nm(szChrSeq);      break;
    case SF_FLT_LIT:     vFacet_Flt_Lit(szChrSeq);     break;
    case SF_GROUPBY:     vFacet_GroupBy(szChrSeq);     break;
    case SF_IDX_COL:     vFacet_Idx_Col(szChrSeq);     break;
    case SF_IDX_NM:      vFacet_Idx_Nm(szChrSeq);      break;
    case SF_INT_LIT:     vFacet_Int_Lit(szChrSeq);     break;
    case SF_IVL_FN:      vFacet_Ivl_Fn(szChrSeq);      break;
    case SF_INTERVAL_DT: vFacet_Interval_DT(szChrSeq); break;
    case SF_INTERVAL_YM: vFacet_Interval_YM(szChrSeq); break;
    case SF_KEY_COL:     vFacet_Key_Col(szChrSeq);     break;
    case SF_LIST_CNT:    vFacet_List_Cnt(szChrSeq);    break;
    case SF_MTH_OP:      vFacet_Mth_Op(szChrSeq);      break;
    case SF_NEGATE:      vFacet_Negate(szChrSeq);      break;
    case SF_NMD_COL:     vFacet_Nmd_Col(szChrSeq);     break;
    case SF_NOTE:        vFacet_Note(szChrSeq);        break;
    case SF_NULL:        vFacet_Null(szChrSeq);        break;
    case SF_PARENTHESIS: vFacet_Parenthesis(szChrSeq); break;
    case SF_PAY_COL:     vFacet_Pay_Col(szChrSeq);     break;
    case SF_PPE_NM:      vFacet_Ppe_Nm(szChrSeq);      break;
    case SF_SQY_NM:      vFacet_Sqy_Nm(szChrSeq);      break;
    case SF_SQY_PRM:     vFacet_Sqy_Prm(szChrSeq);     break;
    case SF_SQY_RSL:     vFacet_Sqy_Rsl(szChrSeq);     break;
    case SF_RLN_OP:      vFacet_Rln_Op(szChrSeq);      break;
    case SF_SQY_OP:      vFacet_Sqy_Op(szChrSeq);      break;
    case SF_STG_CAST:    vFacet_Stg_Cast(szChrSeq);    break;
    case SF_STR_FN:      vFacet_Str_Fn(szChrSeq);      break;
    case SF_STR_LIT:     vFacet_String(szChrSeq);      break;
    case SF_TBL_COL:     vFacet_Tbl_Col(szChrSeq);     break;
    case SF_TBL_NM:      vFacet_Tbl_Nm(szChrSeq);      break;
    case SF_TBL_SYN:     vFacet_Tbl_Syn(szChrSeq);     break;
    case SF_THEN:        vFacet_Then(szChrSeq);        break;
    case SF_TYPE:        vFacet_Type(szChrSeq);        break;
    case SF_WHEN:        vFacet_When(szChrSeq);        break;
    case SF_XK_CAST:     vFacet_Xk_Cast(szChrSeq);     break;

    default: NEVER_REACH;
  }

  RASSERT(bHasFacets());

}

/*----------------------------------------------------------------------------*/
/* get number of facets                                                       */
/*----------------------------------------------------------------------------*/
i8 Symbol::nFacets(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  return m_nFacets;
}

/*----------------------------------------------------------------------------*/
/* get facets as single string                                                */
/*----------------------------------------------------------------------------*/
void Symbol::vFacets_To_String(String* pString)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  pString->vReset();

  for (i8 i_Facet = 0; i_Facet < m_nFacets; i_Facet++) {
    if (i_Facet)
      pString->vConcat(" "_Stz);
    pString->vCatSprint("%s %s~%lld"_Stz,
                        aSF[xaFacet[i_Facet].m_eFacet],
                        xaFacet[i_Facet].m_sChrSeq.szStz(),
                        i_Facet);
  }

}

/*----------------------------------------------------------------------------*/
/* true if symbol is given form                                               */
/*----------------------------------------------------------------------------*/
bool Symbol::bFacetIs(eSF eFacet_arg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (eFacet_arg == xaFacet[0].m_eFacet)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* does symbol have given facet?                                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bHasFacet(eSF eFacet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (C_UNKNOWN == i_Facet(eFacet))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* does symbol have given facet?                                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bHasFacet(Stz* szChrSeq)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  for (i8 i_Facet = 0; i_Facet < m_nFacets; i_Facet++) {
    if (xaFacet[i_Facet].m_sChrSeq.bSame(szChrSeq))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* does symbol a symbolic value?                                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bHasFacets(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (* xaFacet[0].m_sChrSeq.azChar()) {
    RASSERT(m_nFacets);
    return true;
  }

  RASSERT(! m_nFacets);
  return false;
}

/*----------------------------------------------------------------------------*/
/* does symbol have properties?                                               */
/*----------------------------------------------------------------------------*/
bool Symbol::bHasProperties(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (m_pProperties)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* does symbol have a type?                                                   */
/*----------------------------------------------------------------------------*/
bool Symbol::bHasType(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  return m_xCast.m_xTypeId.bIsValid();
}

/*----------------------------------------------------------------------------*/
/* increment table synonym                                                    */
/*----------------------------------------------------------------------------*/
void Symbol::vInc_Tbl_Syn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(bHasFacets());

  RASSERT(SF_TBL_SYN == xaFacet[0].m_eFacet);

  char cTblSyn = cSF_char();

  vFacetSprint(SF_TBL_SYN,"%c"_Stz, cTblSyn + 1);

  RASSERT(bIs_Tbl_Syn());
}

/*----------------------------------------------------------------------------*/
/* is this symbol an aggregate op?                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Agg_Fn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_AGG_FN != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Agg_Fn(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a synthetic as?  (i.e. __COLn)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_As(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_AS != xaFacet[0].m_eFacet)
    return false;

  if (bIs_As(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a boolean function?                                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Bol_Fn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_BOL_FN != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Bol_Fn(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a boolean literal?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Bol_Lit(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_BOL_LIT != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Bol_Lit(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a boolean operator?                                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Bol_Op(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_BOL_OP != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Bol_Op(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a bound?                                                    */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Bound(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_BOUND != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Bound(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a bound?                                                    */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Brace(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_BRACE != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Brace(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for CASE?                                                 */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Case(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_CASE != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Case(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for CAST?                                                 */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Cast(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_CAST != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Cast(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for CASTTYPE?                                             */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_CastType(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_CASTTYPE != xaFacet[0].m_eFacet)
    return false;

  if (bIs_CastType(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a table-column (i.e. $c.n)                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Col_Type(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_COL_TYPE != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Col_Type(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a comma separator?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Comma(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_COMMA != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Comma(xaFacet[0].m_sChrSeq))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a comma separator?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Cross(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_CROSS != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Cross(xaFacet[0].m_sChrSeq))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a decimal literal?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Dcm_Lit(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_DCM_LIT != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Dcm_Lit(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a sort direction?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Direction(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_DIRECTION != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Direction(xaFacet[0].m_sChrSeq))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol ELSE?                                                       */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Else(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_ELSE != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Else(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol ENDIF?                                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_EndIf(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_ENDIF != xaFacet[0].m_eFacet)
    return false;

  if (bIs_EndIf(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a expr-column?  (i.e. $n)                             */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Exp_Col(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_EXP_COL != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Exp_Col(xaFacet[0].m_sChrSeq)) {
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a sort direction?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Fld_Nm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_FLD_NM != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Fld_Nm(xaFacet[0].m_sChrSeq))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a float literal?                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Flt_Lit(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_FLT_LIT != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Flt_Lit(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol GROUPBY ?                                                   */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_GroupBy(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_GROUPBY != xaFacet[0].m_eFacet)
    return false;

  if (bIs_GroupBy(xaFacet[0].m_sChrSeq))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for an index-column?  (i.e. $In)                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Idx_Col(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_IDX_COL != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Idx_Col(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a index name?                                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Idx_Nm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_IDX_NM != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Idx_Nm(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a integer literal?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Int_Lit(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_INT_LIT != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Int_Lit(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol INTERVAL_DT ?                                               */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Interval_DT(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_INTERVAL_DT != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Interval_DT(xaFacet[0].m_sChrSeq))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol INTERVAL_YM ?                                               */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Interval_YM(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_INTERVAL_YM != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Interval_YM(xaFacet[0].m_sChrSeq))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a comma separator?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Itm_Col(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_ITM_COL != xaFacet[0].m_eFacet)
    return false;

  if (! bIs_Itm_Col(xaFacet[0].m_sChrSeq))
    return false;

  if (xaFacet[0].m_sChrSeq.nLength() < 4)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this symbol an interval function?                                       */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Ivl_Fn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_IVL_FN != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Ivl_Fn(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a list count?  (i.e. #n)                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_List_Cnt(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(bHasFacets());

  if (SF_LIST_CNT != xaFacet[0].m_eFacet)
    return false;

  if (bIs_List_cnt(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a key symbol? (i.e. $Kn)                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Key_Col(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_KEY_COL != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Key_Col(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a math operator ?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Mth_Op(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_MTH_OP != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Mth_Op(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol NEGATE ?                                                    */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Negate(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_NEGATE != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Negate(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a column name?                                                     */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Nmd_Col(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_NMD_COL != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Nmd_Col(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a math operator ?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_None(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_NONE != xaFacet[0].m_eFacet)
    return false;

  if (bIs_None(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a note                                                             */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Note(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_NOTE != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Note(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol NULL ?                                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Null(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_NULL != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Null(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a parenthesis?                                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Parenthesis(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_PARENTHESIS != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Parenthesis(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a pay symbol? (i.e. $Pn)                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Pay_Col(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_PAY_COL != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Pay_Col(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a index name?                                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Ppe_Nm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_PPE_NM != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Ppe_Nm(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a relational-operator?                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Rln_Op(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_RLN_OP != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Rln_Op(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a correlation-parameter?  (i.e. %n)                   */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Prm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Sqy_Prm(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a subQuery-operator?                                  */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Nm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_SQY_NM != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Sqy_Nm(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a subQuery-operator?                                  */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Op(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_SQY_OP != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Sqy_Op(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a subQuery-column?  (i.e. __n)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Rsl(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_SQY_RSL != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Sqy_Rsl(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a bound?                                                    */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Stg_Cast(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_STG_CAST != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Stg_Cast(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a subQuery-column?  (i.e. __n)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Stg_Nm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_STG_NO != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Stg_Nm(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a string function?                                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Str_Fn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_STR_FN != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Str_Fn(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}
/*----------------------------------------------------------------------------*/
/* is this a symbol for a subQuery-column?  (i.e. __n)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_String(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_STR_LIT != xaFacet[0].m_eFacet)
    return false;

  if (bIs_String(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a table-column (i.e. $c.n)                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Tbl_Col(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_TBL_COL != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Tbl_Col(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a table name?                                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Tbl_Nm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_TBL_NM != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Tbl_Nm(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol a table synonym?  (i.e. c)                                */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Tbl_Syn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_TBL_SYN != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Tbl_Syn(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol a table synonym?  (i.e. c)                                */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Tbl_Syn_rep(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_TBL_SYN_rep != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Tbl_Syn_rep(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol THEN?                                                       */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Then(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_THEN != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Then(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol TYPE?                                                       */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Type(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_TYPE != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Type(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a parenthesis?                                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_When(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_WHEN != xaFacet[0].m_eFacet)
    return false;

  if (bIs_When(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a cross-refernece?  (i.e. #n)                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Xk_Cast(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_XK_CAST != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Xk_Cast(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* does this symbol have a matching facet with given arg?                     */
/*----------------------------------------------------------------------------*/
bool Symbol::bMatches(eSF eFacet_arg, Stz* szChrSeq_arg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  Facet* aFacet_this = this->aFacet(eFacet_arg);
  if (!aFacet_this)
    return false;

  if (aFacet_this->m_sChrSeq.bSame(szChrSeq_arg))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* does this symbol have a matching facet with given arg?                     */
/*----------------------------------------------------------------------------*/
bool Symbol::bMatches(eSF eFacet, Symbol* pSymbol_arg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  Facet* aFacet_this = this->aFacet(eFacet);
  if (!aFacet_this)
    return false;

  Facet* aFacet_arg = pSymbol_arg->aFacet(eFacet);
  if (! aFacet_arg)
    return false;

  Stz* szChrSeq_this = aFacet_this->m_sChrSeq;
  Stz* szChrSeq_arg  = aFacet_arg->m_sChrSeq;

  if (szChrSeq_arg->bSame(szChrSeq_this))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* does this symbol have a matching facet with given arg?                     */
/*----------------------------------------------------------------------------*/
bool Symbol::bMatches(Symbol* pSymbol_arg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  for (i8 i_Facet = 0; i_Facet < m_nFacets; i_Facet++) {
    eSF eFacet_this = xaFacet[i_Facet].m_eFacet;

    if (bMatches(eFacet_this, pSymbol_arg)) {
      return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* does this symbol have a matching facet with given arg?                     */
/*----------------------------------------------------------------------------*/
bool Symbol::bMatches_ignoring(Symbol* pSymbol_arg, eSF eFacet_ignore)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  for (i8 i_Facet = 0; i_Facet < m_nFacets; i_Facet++) {
    eSF eFacet_this = xaFacet[i_Facet].m_eFacet;

    if (SF_NONE != eFacet_ignore && eFacet_this == eFacet_ignore)
      continue;

    if (bMatches(eFacet_this, pSymbol_arg)) {
      return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bNeedsBrace(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (!bHasFacets())
    return false;

  if (xaFacet[0].m_sChrSeq.bSame("between"_Stz))        return true;
  else if (xaFacet[0].m_sChrSeq.bSame("case"_Stz))           return true;
  else if (xaFacet[0].m_sChrSeq.bSame("in"_Stz))             return true;
  else if (xaFacet[0].m_sChrSeq.bSame("matchLeading"_Stz))   return true;
  else if (xaFacet[0].m_sChrSeq.bSame("matchMiddle"_Stz))    return true;
  else if (xaFacet[0].m_sChrSeq.bSame("matchMiddle1"_Stz))   return true;
  else if (xaFacet[0].m_sChrSeq.bSame("matchTrailing"_Stz))  return true;
//  else if (xaFacet[0].m_sChrSeq.bSame("SUBSTRING2"_Stz))     return true;
//  else if (xaFacet[0].m_sChrSeq.bSame("SUBSTRING3"_Stz))     return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* append facet                                                               */
/*----------------------------------------------------------------------------*/
void Symbol::vPushFacet(eSF eFacet_arg, Stz* szChrSeq_arg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacet(eFacet_arg));
  vFacetPush(eFacet_arg, szChrSeq_arg);
}

/*----------------------------------------------------------------------------*/
/* does this card have same symbol as given argument?                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bSame(char cArg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(1 == nFacets());

  if (*xaFacet[0].m_sChrSeq.azChar() == cArg) {
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bSame(Stz* szArg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(1 == nFacets());

  if (xaFacet[0].m_sChrSeq.bSame(szArg)) {
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* set facet                                                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vSetFacet(eSF eFacet, Stz* szFacet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  i8 i_Facet = this->i_Facet(eFacet);

  if (C_UNKNOWN == i_Facet)
    vFacetSet(m_nFacets++,eFacet,szFacet);
  else
    vFacetSet(i_Facet,eFacet,szFacet);
}

/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
char Symbol::cSF_char(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(bHasFacets());

  return * xaFacet[0].m_sChrSeq.azChar();
}

/*----------------------------------------------------------------------------*/
/* get symbol stz                                                             */
/*----------------------------------------------------------------------------*/
Stz* Symbol::szSF_chrSeq(eSF eSymbol_xchk)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(bHasFacets());

  if (SF_NONE == eSymbol_xchk)
    return xaFacet[0].m_sChrSeq.szStz();

  for (i8 i_Facet = 0; i_Facet < m_nFacets; i_Facet++) {
    if (eSymbol_xchk == xaFacet[i_Facet].m_eFacet)
      return xaFacet[i_Facet].m_sChrSeq.szStz();
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* get symbol int                                                             */
/*----------------------------------------------------------------------------*/
i8 Symbol::iSF_number(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(bHasFacets());

  i8 i = xaFacet[0].m_sChrSeq.iToI8();

  return i;
}

/*----------------------------------------------------------------------------*/
/* get ordinal n  (i.e. $n or $In or $Kn or $c.n)                             */
/*----------------------------------------------------------------------------*/
i8 Symbol::iSF_ordinal(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(bHasFacets());

  i8 i_Col = StrLib::iGleanInt(xaFacet[0].m_sChrSeq.azChar());

  return i_Col;
}

/*----------------------------------------------------------------------------*/
/* get table synonym from $c.n                                                */
/*----------------------------------------------------------------------------*/
char Symbol::cSF_tblSyn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(bHasFacets());

  char acFirst  = xaFacet[0].m_sChrSeq.azChar()[0];
  char acSecond = xaFacet[0].m_sChrSeq.azChar()[1];
//  char acThird  = xaFacet[0].m_sChrSeq.azChar()[2];
//  char acFourth = xaFacet[0].m_sChrSeq.azChar()[3];

  if ('$' == acFirst && StrLib::bIsAlpha(acSecond))
    return acSecond;
  else
    return acFirst;
}

/*----------------------------------------------------------------------------*/
/* get typeId                                                                 */
/*----------------------------------------------------------------------------*/
TypeId Symbol::xTypeId(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  return m_xCast.m_xTypeId;
}

/*----------------------------------------------------------------------------*/
/* get typeLength                                                             */
/*----------------------------------------------------------------------------*/
i8 Symbol::nTypeLength(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  return m_xCast.m_nTypeLength;
}

/*----------------------------------------------------------------------------*/
/* get typeNullable                                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bTypeNullable(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  return m_xCast.m_bTypeNullable;
}

/*----------------------------------------------------------------------------*/
/* get typeScale                                                              */
/*----------------------------------------------------------------------------*/
i8 Symbol::nTypeScale(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  return m_xCast.m_nTypeScale;
}

/*----------------------------------------------------------------------------*/
/* validate symbol                                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bValidate(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  bool b = false;

  switch (xaFacet[0].m_eFacet) {
    case SF_NONE:        b = bIs_None();        break;
    case SF_AGG_FN:      b = bIs_Agg_Fn();      break;
    case SF_AS:          b = bIs_As();          break;
    case SF_BOL_FN:      b = bIs_Bol_Fn();      break;
    case SF_BOL_LIT:     b = bIs_Bol_Lit();     break;
    case SF_BOL_OP:      b = bIs_Bol_Op();      break;
    case SF_BOUND:       b = bIs_Bound();       break;
    case SF_BRACE:       b = bIs_Brace();       break;
    case SF_CAST:        b = bIs_Cast();        break;
    case SF_CASTTYPE:    b = bIs_CastType();    break;
    case SF_COL_TYPE:    b = bIs_Col_Type();    break;
    case SF_COMMA:       b = bIs_Comma();       break;
    case SF_CROSS:       b = bIs_Cross();       break;
    case SF_DCM_LIT:     b = bIs_Dcm_Lit();     break;
    case SF_DIRECTION:   b = bIs_Direction();   break;
    case SF_EXP_COL:     b = bIs_Exp_Col();     break;
    case SF_FLD_NM:      b = bIs_Fld_Nm();      break;
    case SF_FLT_LIT:     b = bIs_Flt_Lit();     break;
    case SF_GROUPBY:     b = bIs_GroupBy();     break;
    case SF_IDX_COL:     b = bIs_Idx_Col();     break;
    case SF_IDX_NM:      b = bIs_Idx_Nm();      break;
    case SF_INT_LIT:     b = bIs_Int_Lit();     break;
    case SF_INTERVAL_DT: b = bIs_Interval_DT(); break;
    case SF_INTERVAL_YM: b = bIs_Interval_YM(); break;
    case SF_IVL_FN:      b = bIs_Ivl_Fn();      break;
    case SF_KEY_COL:     b = bIs_Key_Col();     break;
    case SF_LIST_CNT:    b = bIs_List_Cnt();    break;
    case SF_MTH_OP:      b = bIs_Mth_Op();      break;
    case SF_NMD_COL:     b = bIs_Nmd_Col();     break;
    case SF_NOTE:        b = bIs_Note();        break;
    case SF_NULL:        b = bIs_Null();        break;
    case SF_PARENTHESIS: b = bIs_Parenthesis(); break;
    case SF_PAY_COL:     b = bIs_Pay_Col();     break;
    case SF_PPE_NM:      b = bIs_Ppe_Nm();      break;
    case SF_RLN_OP:      b = bIs_Rln_Op();      break;
    case SF_SQY_NM:      b = bIs_Sqy_Nm();      break;
    case SF_SQY_OP:      b = bIs_Sqy_Op();      break;
    case SF_SQY_PRM:     b = bIs_Sqy_Prm();     break;
    case SF_SQY_RSL:     b = bIs_Sqy_Rsl();     break;
    case SF_STG_NO:      b = bIs_Stg_Nm();      break;
    case SF_STG_CAST:    b = bIs_Stg_Cast();    break;
    case SF_STR_FN:      b = bIs_Str_Fn();      break;
    case SF_STR_LIT:     b = bIs_String();      break;
    case SF_TBL_COL:     b = bIs_Tbl_Col();     break;
    case SF_TBL_NM:      b = bIs_Tbl_Nm();      break;
    case SF_TBL_SYN:     b = bIs_Tbl_Syn();     break;
    case SF_TBL_SYN_rep: b = bIs_Tbl_Syn_rep(); break;
    case SF_TYPE:        b = bIs_Type();        break;
    case SF_WHEN:        b = bIs_When();        break;
    case SF_XK_CAST:     b = bIs_Xk_Cast();     break;

    default:
      NEVER_REACH
  }

  return b;
}

/*----------------------------------------------------------------------------*/
/* validate symbol                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vValidate(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  bool b = bValidate();
  if (! b) {
    vDump();
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* what is this symbol?                                                       */
/*----------------------------------------------------------------------------*/
Stz* Symbol::szWhatIs(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Agg_Fn())       return "AggOp"_Stz;
  if (bIs_Bol_Fn())       return "BoolFn"_Stz;
  if (bIs_Bound())        return "Bound"_Stz;
  if (bIs_Exp_Col())      return "StreamOrdinal"_Stz;
  if (bIs_Tbl_Col())      return "ColumnOrdinal"_Stz;
  if (bIs_Comma())        return "Comma"_Stz;
  if (bIs_Idx_Col())      return "IndexColumn"_Stz;
  if (bIs_Key_Col())      return "KeyOrdinal"_Stz;
  if (bIs_Mth_Op())       return "NumbOp"_Stz;
  if (bIs_Parenthesis())  return "Parenthesis"_Stz;
  if (bIs_Rln_Op())       return "RelnOp"_Stz;
  if (bIs_Sqy_Op())       return "SubQueryOp"_Stz;
  if (bIs_Sqy_Prm())      return "ParameterOrdinal"_Stz;
  if (bIs_Sqy_Rsl())      return "SubQueryColumn"_Stz;

  return "??WhatIs"_Stz;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
void Symbol::Y_vAll(Symbol* pSymbol_arg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! pSymbol_arg->m_pProperties) {
    if (m_pProperties) {
      delete m_pProperties;
      m_pProperties = nullptr;
    }
    return;
  }

  if (! m_pProperties)
    m_pProperties = new TagSet;

  m_pProperties->vCopy(pSymbol_arg->m_pProperties);
}

/*----------------------------------------------------------------------------*/
/* remove given property                                                      */
/*----------------------------------------------------------------------------*/
void Symbol::Y_vCut(Stz* szPpyNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! m_pProperties)
    return;

  m_pProperties->vDelete(szPpyNm);
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
i8 Symbol::Y_iGet(Stz* szPpyNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! m_pProperties)
    return C_UNKNOWN;

  i8 iPpy = m_pProperties->iValue(szPpyNm);

  return iPpy;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
i8 Symbol::Y_iGet(i8 i_Ppy)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! m_pProperties)
    return C_UNKNOWN;

  i8 iPpy = m_pProperties->iValue(i_Ppy);

  return iPpy;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
Object* Symbol::Y_pGet(i8 i_Ppy)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! m_pProperties)
    return nullptr;

  Object* pPpy = m_pProperties->pValue(i_Ppy);

  return pPpy;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
Object* Symbol::Y_pGet(Stz* szPpyNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! m_pProperties)
    return nullptr;

  Object* pPpy = m_pProperties->pValue(szPpyNm);

  return pPpy;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
Stz* Symbol::Y_szGet(i8 i_Ppy)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! m_pProperties)
    return nullptr;

  Stz* szPpy = m_pProperties->szValue(i_Ppy);

  return szPpy;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
Stz* Symbol::Y_szGet(Stz* szPpyNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! m_pProperties)
    return nullptr;

  Stz* szPpy = m_pProperties->szValue(szPpyNm);

  return szPpy;
}

/*----------------------------------------------------------------------------*/
/* has given property ?                                                       */
/*----------------------------------------------------------------------------*/
bool Symbol::Y_bHas(Stz* szPpyNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! m_pProperties) {
    return false;
  }

  bool bHas = m_pProperties->bHasName(szPpyNm);
  return bHas;
}

/*----------------------------------------------------------------------------*/
/* get number of properties                                                   */
/*----------------------------------------------------------------------------*/
i8 Symbol::Y_nPpy(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! m_pProperties)
    return 0;

  i8 nProperties = m_pProperties->nItems();

  return nProperties;
}

/*----------------------------------------------------------------------------*/
/* get given property name                                                    */
/*----------------------------------------------------------------------------*/
Stz* Symbol::Y_szPpyNm(i8 i_Ppy)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! m_pProperties)
    return nullptr;

  Stz* szName = m_pProperties->szName(i_Ppy);

  return szName;
}

/*----------------------------------------------------------------------------*/
/* get type of property                                                       */
/*----------------------------------------------------------------------------*/
eTN Symbol::Y_ePpyTkn(i8 i_Ppy)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! m_pProperties)
    return TN_NONE;

  eTN eToken = m_pProperties->eToken(i_Ppy);

  return eToken;
}

/*----------------------------------------------------------------------------*/
/* get type of property                                                       */
/*----------------------------------------------------------------------------*/
eTN Symbol::Y_ePpyTkn(Stz* szPpyNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! m_pProperties)
    return TN_NONE;

  i8 i_Ppy = Y_iGet(szPpyNm);
  if (C_UNKNOWN == i_Ppy)
    return TN_NONE;

  eTN eToken = m_pProperties->eToken(i_Ppy);

  return eToken;
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Symbol::Y_vSet(Stz* szPpyNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;

  m_pProperties->vReplace(szPpyNm, "y"_Stz);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Symbol::Y_vSet(Stz* szPpyNm, i8 iPpy)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;

  m_pProperties->vReplace(szPpyNm, iPpy);
}

/*----------------------------------------------------------------------------*/
/* set given property                                                         */
/*----------------------------------------------------------------------------*/
void Symbol::Y_vSet(Stz* szPpyNm, Object* pObject)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;

  m_pProperties->vReplace(szPpyNm, pObject);

}
/*----------------------------------------------------------------------------*/
/* set given property                                                         */
/*----------------------------------------------------------------------------*/
void Symbol::Y_vSet(Stz* szPpyNm, Stz* szPpy)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(szPpy);
  RASSERT(*szPpy->azChar());

  if (! m_pProperties)
    m_pProperties = new TagSet;

  m_pProperties->vReplace(szPpyNm, szPpy);
}

/*----------------------------------------------------------------------------*/
/* set property from given symbol                                             */
/*----------------------------------------------------------------------------*/
void Symbol::Y_vSet(Stz* szPpyNm, Symbol* pSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! pSymbol->Y_bHas(szPpyNm)) {
    Y_vCut(szPpyNm);
    return;
  }

  if (TN_INTEGER == pSymbol->Y_ePpyTkn(szPpyNm))
    Y_vSet(szPpyNm, pSymbol->Y_iGet(szPpyNm));
  else
  if (TN_STRING == pSymbol->Y_ePpyTkn(szPpyNm))
    Y_vSet(szPpyNm, pSymbol->Y_szGet(szPpyNm));
  else
    NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* convert properties to string                                               */
/*----------------------------------------------------------------------------*/
void Symbol::Y_vTo_String(String* pString)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (m_pProperties)
    m_pProperties->vToString(pString);
  else
    pString->vCopy("nil"_Stz);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set this symbol to given aggregate fn                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Agg_Fn(Stz* szAggFn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_AGG_FN,"%s"_Stz,szAggFn);

  RASSERT(bIs_Agg_Fn());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given as                                                */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_As(Stz* szAs)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_AS,"%s"_Stz,szAs);

  RASSERT(bIs_As());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given boolean fn                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Bol_Fn(Stz* szBolFn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_BOL_FN,"%s"_Stz,szBolFn);

  RASSERT(bIs_Bol_Fn());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given boolean literal                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Bol_Lit(Stz* szBolLit)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_BOL_LIT,"%s"_Stz,szBolLit);

  RASSERT(bIs_Bol_Lit());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given boolean op                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Bol_Op(Stz* szBolOp)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_BOL_OP,"%s"_Stz,szBolOp);

  RASSERT(bIs_Bol_Op());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given bound                                             */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Bound(Stz* szBound)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_BOUND,"%s"_Stz,szBound);

  RASSERT(bIs_Bound());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given brace                                             */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Brace(Stz* szBrace)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_BRACE,"%s"_Stz,szBrace);

  RASSERT(bIs_Brace());
}

/*----------------------------------------------------------------------------*/
/* set this symbol CASE                                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Case(Stz* szCase)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_CASE,"%s"_Stz,szCase);

  RASSERT(bIs_Case());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given as                                                */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Cast(Stz* szCast)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_CAST,"%s"_Stz,szCast);

  RASSERT(bIs_Cast());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given as                                                */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_CastType(Stz* szCastType)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_CASTTYPE,"%s"_Stz,szCastType);

  RASSERT(bIs_CastType());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given comma                                             */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Comma(Stz* szComma)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_COMMA,"%s"_Stz,szComma);

  RASSERT(bIs_Comma());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given cross                                             */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Cross(Stz* szCross)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_CROSS,"%s"_Stz,szCross);

  RASSERT(bIs_Cross());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given type                                              */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Col_Type(Stz* szColType)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_COL_TYPE,"%s"_Stz,szColType);

  RASSERT(bIs_Col_Type());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given decimal literal                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Dcm_Lit(Stz* szDcmLit)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_DCM_LIT,"%s"_Stz,szDcmLit);

  RASSERT(bIs_Dcm_Lit());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given direction                                         */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Direction(Stz* szDirection)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_DIRECTION,"%s"_Stz,szDirection);

  RASSERT(bIs_Direction());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to ELSE                                                    */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Else(Stz* szElse)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_ELSE,"%s"_Stz,szElse);

  RASSERT(bIs_Else());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to ENDIF                                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_EndIf(Stz* szEndIf)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_ENDIF,"%s"_Stz,szEndIf);

  RASSERT(bIs_EndIf());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given expr column number                                */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Exp_Col(Stz* szExpCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet ||
          SF_NMD_COL == xaFacet[0].m_eFacet ||
          SF_EXP_COL == xaFacet[0].m_eFacet ||
          SF_TBL_COL == xaFacet[0].m_eFacet ||

          SF_KEY_COL == xaFacet[0].m_eFacet ||  // todo: these need to be investigated
          SF_PARENTHESIS == xaFacet[0].m_eFacet ||
          SF_SQY_PRM == xaFacet[0].m_eFacet ||
          SF_LIST_CNT == xaFacet[0].m_eFacet
  );

  vFacetSprint(SF_EXP_COL, "%s"_Stz, szExpCol);

  RASSERT(bIs_Exp_Col());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given fold name                                         */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Fld_Nm(Stz* szFldNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_FLD_NM,"%s"_Stz,szFldNm);

  RASSERT(bIs_Fld_Nm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to be a float literal                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Flt_Lit(Stz* szFltLit)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_FLT_LIT,"%s"_Stz,szFltLit);

  RASSERT(bIs_Flt_Lit());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given group-by                                          */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_GroupBy(Stz* szGroupBy)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_GROUPBY,"%s"_Stz,szGroupBy);

  RASSERT(bIs_GroupBy());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given index-column                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Idx_Col(i8 iIdxCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_IDX_COL,"$I%lld"_Stz, iIdxCol);

  RASSERT(bIs_Idx_Col());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given index-column                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Idx_Col(Stz* szIdxCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_IDX_COL,"%s"_Stz,szIdxCol);

  RASSERT(bIs_Idx_Col());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to a index name                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Idx_Nm(Stz* szIdxNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_IDX_NM,"%s"_Stz,szIdxNm);

  RASSERT(bIs_Idx_Nm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given integer literal                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Int_Lit(i8 iIntLit)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_INT_LIT, "%lld"_Stz, iIntLit);

  RASSERT(bIs_Int_Lit());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to be a integer literal                                    */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Int_Lit(Stz* szIntLit)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_INT_LIT,"%s"_Stz,szIntLit);

  RASSERT(bIs_Int_Lit());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given INTERVAL_DT                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Interval_DT(Stz* szInterval_DT)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_INTERVAL_DT,"%s"_Stz,szInterval_DT);

  RASSERT(bIs_Interval_DT());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given INTERVAL_YM                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Interval_YM(Stz* szInterval_YM)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_INTERVAL_YM,"%s"_Stz,szInterval_YM);

  RASSERT(bIs_Interval_YM());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given itemised-column-name                              */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Itm_Col(i8 i_Child)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet ||
              SF_NMD_COL == xaFacet[0].m_eFacet ||
              SF_NMD_COL == xaFacet[0].m_eFacet);

  vFacetSprint(SF_ITM_COL,"C%lld_%s"_Stz, i_Child, xaFacet[0].m_sChrSeq.szStz());

  RASSERT(bIs_Itm_Col());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given itemised-column-name                              */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Itm_Col(i8 i_Child, Symbol* pSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_ITM_COL,"C%lld_%s"_Stz,
               i_Child,
               pSymbol->szSF_chrSeq(SF_NMD_COL));

  RASSERT(bIs_Itm_Col());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given interval fn                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Ivl_Fn(Stz* szIvlFn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_IVL_FN,"%s"_Stz,szIvlFn);

  RASSERT(bIs_Ivl_Fn());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given list count                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_List_Cnt(i8 nListCnt)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_LIST_CNT, "#%lld"_Stz, nListCnt);

  RASSERT(bIs_List_Cnt());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given list count                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_List_Cnt(Stz* szListCnt)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_LIST_CNT, "%s"_Stz, szListCnt);

  RASSERT(bIs_List_Cnt());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given key-column                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Key_Col(i8 iKeyCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet || SF_KEY_COL == xaFacet[0].m_eFacet);

  vFacetSprint(SF_KEY_COL, "$K%lld"_Stz, iKeyCol);

  RASSERT(bIs_Key_Col());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given key-column                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Key_Col(Stz* szKeyCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_KEY_COL,"%s"_Stz,szKeyCol);

  RASSERT(bIs_Key_Col());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given math op                                           */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Mth_Op(Stz* szMthOp)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_MTH_OP,"%s"_Stz,szMthOp);

  RASSERT(bIs_Mth_Op());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to NEGATE                                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Negate(Stz* szNegate)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_NEGATE,"%s"_Stz,szNegate);

  RASSERT(bIs_Negate());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given synthetic-as no                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Nmd_Col(i8 iColNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_NMD_COL, "COL%lld"_Stz, iColNo);

  RASSERT(bIs_Nmd_Col());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given column-name                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Nmd_Col(Stz* szColNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  //RASSERT(SF_NONE == xaFacet[0].m_eFacet || SF_EXP_COL == xaFacet[0].m_eFacet);

  vFacetSprint(SF_NMD_COL, "%s"_Stz, szColNm);

  RASSERT(bIs_Nmd_Col());
  RASSERT(!bIs_Itm_Col());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given note                                              */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Note(Stz* szNote)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_NOTE,"%s"_Stz,szNote);

  RASSERT(bIs_Note());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to NULL                                                    */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Null(Stz* szNull)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_NULL,"%s"_Stz,szNull);

  RASSERT(bIs_Null());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given parenthesis                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Parenthesis(Stz* szParenthesis)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_PARENTHESIS,"%s"_Stz,szParenthesis);

  RASSERT(bIs_Parenthesis());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given key-column                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Pay_Col(i8 iPayCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet || SF_PAY_COL == xaFacet[0].m_eFacet);

  vFacetSprint(SF_PAY_COL, "$P%lld"_Stz, iPayCol);

  RASSERT(bIs_Pay_Col());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given key-column                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Pay_Col(Stz* szPayCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_PAY_COL,"%s"_Stz,szPayCol);

  RASSERT(bIs_Pay_Col());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given pipeline name                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Ppe_Nm(Stz* szPpeNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_PPE_NM,"%s"_Stz,szPpeNm);

  RASSERT(bIs_Ppe_Nm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given relational op                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Rln_Op(Stz* szRlnOp)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet || SF_SQY_OP == xaFacet[0].m_eFacet);

  vFacetSprint(SF_RLN_OP,"%s"_Stz,szRlnOp);

  RASSERT(bIs_Rln_Op());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given subquery-number                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Sqy_Nm(i8 iSqyNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_SQY_NM,"$Q%lld"_Stz, iSqyNo);

  RASSERT(bIs_Sqy_Nm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given subquery name                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Sqy_Nm(Stz* szSqyNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_SQY_NM, "%s"_Stz, szSqyNm);

  RASSERT(bIs_Sqy_Nm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given subquery op                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Sqy_Op(Stz* szSqyOp)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet || SF_SQY_OP == xaFacet[0].m_eFacet);

  vFacetSprint(SF_SQY_OP,"%s"_Stz,szSqyOp);

  RASSERT(bIs_Sqy_Op());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given parameter column                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Sqy_Prm(i8 iOrdinal)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_SQY_PRM, "%%%lld"_Stz, iOrdinal);

  RASSERT(bIs_Sqy_Prm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given parameter column                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Sqy_Prm(Stz* szSqyPrm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_SQY_PRM, "%s"_Stz, szSqyPrm);

  RASSERT(bIs_Sqy_Prm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given parameter column                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Sqy_Rsl(i8 iOrdinal)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  //RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_SQY_RSL, "$R%lld"_Stz, iOrdinal);

  RASSERT(bIs_Sqy_Rsl());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given parameter column                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Sqy_Rsl(Stz* szSqyRsl)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  //RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_SQY_RSL, "%s"_Stz, szSqyRsl);

  RASSERT(bIs_Sqy_Rsl());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given stage cast                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Stg_Cast(Stz* szStgCast)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet || SF_STG_CAST == xaFacet[0].m_eFacet);

  vFacetSprint(SF_STG_CAST,"%s"_Stz,szStgCast);

  RASSERT(bIs_Stg_Cast());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to iven stage name                                         */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Stg_Nm(i8 iStgNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_STG_NO,"S%02lld"_Stz, iStgNo);

  RASSERT(bIs_Stg_Nm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given column number                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Stm_Col(i8 iStmCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (m_nFacets) {
    eSF eFacet = xaFacet[0].m_eFacet;
    RASSERT(SF_NONE    == eFacet ||
            SF_SQY_RSL == eFacet ||
            SF_EXP_COL == eFacet ||
            SF_NMD_COL == eFacet ||
            SF_TBL_COL == eFacet ||
            SF_KEY_COL == eFacet);
  }

  vFacetSprint(SF_EXP_COL, "$%lld"_Stz, iStmCol);

  RASSERT(bIs_Exp_Col());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given string fn                                         */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Str_Fn(Stz* szBolFn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_STR_FN,"%s"_Stz,szBolFn);

  RASSERT(bIs_Str_Fn());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given string                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_String(Stz* szString)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_STR_LIT,"%s"_Stz,szString);

  RASSERT(bIs_String());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-column                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Tbl_Col(char cTblSyn, i8 iTblCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_TBL_COL, "$%c.%lld"_Stz, cTblSyn, iTblCol);

  RASSERT(bIs_Tbl_Col());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-column                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Tbl_Col(Stz* szTblCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet ||
          SF_NMD_COL == xaFacet[0].m_eFacet ||
          SF_EXP_COL == xaFacet[0].m_eFacet ||
          SF_TBL_COL == xaFacet[0].m_eFacet);

  vFacetSprint(SF_TBL_COL, "%s"_Stz, szTblCol);

  RASSERT(bIs_Tbl_Col());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table name                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Tbl_Nm(Stz* szTblNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_TBL_NM,"%s"_Stz,szTblNm);

  RASSERT(bIs_Tbl_Nm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-synonym                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Tbl_Syn(char cTblSyn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_TBL_SYN,"%c"_Stz, cTblSyn);

  RASSERT(bIs_Tbl_Syn());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-synonym                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Tbl_Syn(Stz* szTblSyn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_TBL_SYN,"%s"_Stz,szTblSyn);

  RASSERT(bIs_Tbl_Syn());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given ttable-synonym replicated                         */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Tbl_Syn_rep(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet || SF_TBL_SYN == xaFacet[0].m_eFacet);

  char cTableSyn_actual = cSF_char();
  cTableSyn_actual = StrLib::cToUpper(cTableSyn_actual);

  vFacetReset();
  vFacetSprint(SF_TBL_SYN_rep,"%c"_Stz, cTableSyn_actual);

  RASSERT(bIs_Tbl_Syn_rep());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to THEN                                                    */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Then(Stz* szThen)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_THEN,"%s"_Stz,szThen);

  RASSERT(bIs_Then());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to TYPE                                                    */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Type(Stz* szType)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_TYPE,"%s"_Stz,szType);

  RASSERT(bIs_Type());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given when                                              */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_When(Stz* szWhen)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_WHEN,"%s"_Stz,szWhen);

  RASSERT(bIs_When());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given xk cast                                           */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Xk_Cast(Stz* szXkCast)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_XK_CAST,"%s"_Stz,szXkCast);

  RASSERT(bIs_Xk_Cast());
}

/*----------------------------------------------------------------------------*/
/* concat facets                                                              */
/*----------------------------------------------------------------------------*/
void Symbol:: vFacetAppend(eSF eFacet, Stz* szFacet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vFacetSet(m_nFacets++,eFacet,szFacet);
}

/*----------------------------------------------------------------------------*/
/* concat facets                                                              */
/*----------------------------------------------------------------------------*/
void Symbol:: vFacetConcat(Symbol* pSymbol_arg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  i8 nFacets_arg = pSymbol_arg->m_nFacets;
  for (i8 i_Facet_arg = nFacets_arg - 1; i_Facet_arg >= 0; i_Facet_arg--) {
    Facet* aFacet_arg = pSymbol_arg->aFacet(i_Facet_arg);

    if (aFacet_arg->m_sChrSeq.bSame("%1"_Stz))
      vDebugPoint();

    vFacetRotateSet(aFacet_arg->m_eFacet,aFacet_arg->m_sChrSeq);
  }
}

/*----------------------------------------------------------------------------*/
/* copy facets                                                                */
/*----------------------------------------------------------------------------*/
void Symbol:: vFacetCopy(Symbol* pSymbol_arg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vFacetReset();

  m_nFacets = pSymbol_arg->m_nFacets;
  for (i8 i_Facet = 0; i_Facet < m_nFacets; i_Facet++) {
    xaFacet[i_Facet].m_eFacet  = pSymbol_arg->xaFacet[i_Facet].m_eFacet;
    xaFacet[i_Facet].m_sChrSeq = pSymbol_arg->xaFacet[i_Facet].m_sChrSeq;
  }
}

/*----------------------------------------------------------------------------*/
/* cut facet                                                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vFacetCut(i8 i_Facet_cut)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(i_Facet_cut < m_nFacets);

  for (i8 i_Facet = i_Facet_cut + 1; i_Facet < m_nFacets; i_Facet++) {
    xaFacet[i_Facet - 1].m_eFacet  = xaFacet[i_Facet].m_eFacet;
    xaFacet[i_Facet - 1].m_sChrSeq = xaFacet[i_Facet].m_sChrSeq;
  }

  m_nFacets--;
}

/*----------------------------------------------------------------------------*/
/* push facet                                                                 */
/*----------------------------------------------------------------------------*/
void Symbol::vFacetPush(eSF eFacet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  UNUSED(eFacet);

  m_nFacets++;

  for (i8 i_Facet = nFacets() - 1; i_Facet >= 1; i_Facet--) {
    xaFacet[i_Facet].m_eFacet  = xaFacet[i_Facet - 1].m_eFacet;
    xaFacet[i_Facet].m_sChrSeq = xaFacet[i_Facet - 1].m_sChrSeq;
  }

  vFacetSet(0,eFacet,""_Stz);

  RASSERT(m_nFacets);
}

/*----------------------------------------------------------------------------*/
/* push facet                                                                 */
/*----------------------------------------------------------------------------*/
void Symbol::vFacetPush(eSF eFacet, Stz* szFacet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  UNUSED(eFacet);

  m_nFacets++;

  for (i8 i_Facet = nFacets() - 1; i_Facet >= 1; i_Facet--) {
    xaFacet[i_Facet].m_eFacet  = xaFacet[i_Facet - 1].m_eFacet;
    xaFacet[i_Facet].m_sChrSeq = xaFacet[i_Facet - 1].m_sChrSeq;
  }

  vFacetSet(0,eFacet,szFacet);

  RASSERT(m_nFacets);
}

/*----------------------------------------------------------------------------*/
/* rotate facet to top                                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacetReset(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_nFacets = 0;
  for (i8 i_Facet = 0; i_Facet < C_FACETS; i_Facet++) {
    xaFacet[i_Facet].m_eFacet  = SF_NONE;
    xaFacet[i_Facet].m_sChrSeq = ""_Stz;
  }
}

/*----------------------------------------------------------------------------*/
/* rotate facet to top                                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacetRotate(eSF eFacet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  i8 i_Facet = this->i_Facet(eFacet);
  if (0 == i_Facet)
    return;
  if (C_UNKNOWN == i_Facet) {
    vFacetPush(eFacet);
    RASSERT(m_nFacets);
    return;
  }

  RASSERT(m_nFacets);

  Facet xFacet_tmp;

  xFacet_tmp.m_eFacet  = xaFacet[0].m_eFacet;
  xFacet_tmp.m_sChrSeq = xaFacet[0].m_sChrSeq;

  xaFacet[0].m_eFacet  = xaFacet[i_Facet].m_eFacet;
  xaFacet[0].m_sChrSeq = xaFacet[i_Facet].m_sChrSeq;

  xaFacet[i_Facet].m_eFacet  = xFacet_tmp.m_eFacet;
  xaFacet[i_Facet].m_sChrSeq = xFacet_tmp.m_sChrSeq;
}

/*----------------------------------------------------------------------------*/
/* rotate facet to top                                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacetRotateSet(eSF eFacet, Stz* szFacet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vFacetRotate(eFacet);
  vFacetSet(0, eFacet, szFacet);
}

/*----------------------------------------------------------------------------*/
/* set facet                                                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vFacetSet(i8 i_Facet, eSF eFacet, Stz* szChrSeq)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(i_Facet < m_nFacets);
  xaFacet[i_Facet].m_eFacet  = eFacet;
  xaFacet[i_Facet].m_sChrSeq = szChrSeq;
}

/*----------------------------------------------------------------------------*/
/* sprint facet                                                               */
/*----------------------------------------------------------------------------*/
void Symbol::vFacetSprint(eSF eFacet, Stz* szFormat, ...)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  s256 sChrSeq;

  va_list xArgs;
  va_start(xArgs,szFormat);
  StrLib::vSprint(sChrSeq.azChar(), sChrSeq.nAreaBytes(), xArgs, (char*)szFormat);
  va_end(xArgs);

  vFacetRotate(eFacet);
  RASSERT(m_nFacets);

  vFacetSet(0, eFacet, sChrSeq);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/