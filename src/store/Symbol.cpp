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
  vCastReset();
  m_pProperties = nullptr;

  _checkInitialised_(CN_SYMBOL);

  vSetFacet(eFacet, iInt);

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

  vApplyFacet(eFacet, szChrSeq);

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
eSF Symbol::eEval_facet(Stz* szArg)
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
  if (bIs_Rln_Op(szArg))       return SF_RLN_OP;
  if (bIs_Sqy_No(szArg))       return SF_SQY_NO;
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
  if (bIs_List_Cnt(szArg))     return SF_LIST_CNT;

  if (bIs_Rw_Aggregates(szArg)) return SF_RW_AGGREGATES;

  if (bPossibleNm(szArg))      return SF_NMD_COL;

  return SF_NONE;
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-synonym                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vInit_Tbl_Syn(char cTblSyn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vFacetSprint(SF_TBL_SYN,"$T%c"_Stz, cTblSyn);

  RASSERT(bHasFacet(SF_TBL_SYN));
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
/* is this a symbol for a expr-column?  (i.e. $n)                             */
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
  if (bIs_Sqy_No(szArg))      return false;
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
bool Symbol::bIs_List_Cnt(Stz* szChrSeq)
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
  if (bIs_Sqy_No(szChrSeq))      return false;
  if (bIs_Sqy_Op(szChrSeq))      return false;
  if (bIs_Stg_Cast(szChrSeq))    return false;
  if (bIs_Str_Fn(szChrSeq))      return false;
  if (bIs_Then(szChrSeq))        return false;
  if (bIs_Type(szChrSeq))        return false;
  if (bIs_When(szChrSeq))        return false;

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
/* is this symbol a subquery name?                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Nm(Stz* szArg)
{
  _static_

  if (! bPossibleNm(szArg))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is arg a $Qn like name?                                                    */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_No(Stz *szArg)
{
  _static_

  if (! szArg)
    return false;

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
/* is value a stage name?                                                     */
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
/* is this a symbol for a table-column (i.e. $$c.n)                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Tbl_Col(Stz* szArg)
{
  _static_

  char acFirst  = szArg->azChar()[0];
  char acSecond = szArg->azChar()[1];
  char acThird  = szArg->azChar()[2];
  char acFourth = szArg->azChar()[3];
  char acFifth  = szArg->azChar()[4];

  if ('$' == acFirst
      && '$' == acSecond
      && StrLib::bIsAlpha(acThird)
      && '.' == acFourth
      && StrLib::bIsDigit(acFifth))
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
  char acThird  = szArg->azChar()[2];
  char acFourth = szArg->azChar()[3];

  if ('$' == acFirst &&
      'T' == acSecond &&
      StrLib::bIsAlpha(acThird) &&
      '\0' == acFourth)
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
  char acThird  = szArg->azChar()[2];
  char acFourth = szArg->azChar()[3];

  if ('$' == acFirst &&
      'T' == acSecond &&
      StrLib::bIsUpperCase(acThird) &&
      '\0' == acFourth)
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
/* is given arg the rw AGGREGATES ?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Rw_Aggregates(Stz* szArg)
{
  _static_

  if (szArg->bSame("AGGREGATES"_Stz))   return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* test facet for bound                                                       */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_bound(Facet* aFacet)
{
  _static_

  if (SF_BOUND   == aFacet->m_eFacet ||
      SF_IDX_COL == aFacet->m_eFacet ||
      SF_RLN_OP  == aFacet->m_eFacet)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* test facet for rlnConj                                                     */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_rlnConj(Facet* aFacet)
{
  _static_

  if (SF_BOL_FN == aFacet->m_eFacet ||
      SF_RLN_OP == aFacet->m_eFacet ||
      SF_SQY_OP == aFacet->m_eFacet)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* test facet for term                                                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_term(Facet* aFacet)
{
  _static_

  if (
      SF_COL_TYPE    == aFacet->m_eFacet
      ||
      SF_EXP_COL     == aFacet->m_eFacet ||
      SF_KEY_COL     == aFacet->m_eFacet ||
      SF_IDX_COL     == aFacet->m_eFacet ||
      SF_ITM_COL     == aFacet->m_eFacet ||
      SF_NMD_COL     == aFacet->m_eFacet
      ||
      SF_AGG_FN      == aFacet->m_eFacet ||
      SF_BOL_FN      == aFacet->m_eFacet ||
      SF_IVL_FN      == aFacet->m_eFacet ||
      SF_STR_FN      == aFacet->m_eFacet
      ||
      SF_BOL_LIT     == aFacet->m_eFacet ||
      SF_DCM_LIT     == aFacet->m_eFacet ||
      SF_FLT_LIT     == aFacet->m_eFacet ||
      SF_INT_LIT     == aFacet->m_eFacet ||
      SF_STR_LIT     == aFacet->m_eFacet
      ||
      SF_BOL_OP      == aFacet->m_eFacet ||
      SF_MTH_OP      == aFacet->m_eFacet ||
      SF_RLN_OP      == aFacet->m_eFacet
      ||
      SF_INTERVAL_DT == aFacet->m_eFacet ||
      SF_INTERVAL_YM == aFacet->m_eFacet
      ||
      SF_CAST        == aFacet->m_eFacet ||
      SF_CASTTYPE    == aFacet->m_eFacet ||
      SF_TYPE        == aFacet->m_eFacet
      ||
      SF_CASE        == aFacet->m_eFacet ||
      SF_ELSE        == aFacet->m_eFacet ||
      SF_ENDIF       == aFacet->m_eFacet ||
      SF_THEN        == aFacet->m_eFacet ||
      SF_WHEN        == aFacet->m_eFacet
      ||
      SF_BRACE       == aFacet->m_eFacet ||
      SF_LIST_CNT    == aFacet->m_eFacet ||
      SF_NEGATE      == aFacet->m_eFacet ||
      SF_NULL        == aFacet->m_eFacet ||
      SF_PARENTHESIS == aFacet->m_eFacet)
    return true;

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
/* set facet                                                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vApplyFacet(eSF eFacet, Stz* szChrSeq)
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
    case SF_SQY_NM:      vFacet_Sqy_Nm(szChrSeq);      break;
    case SF_SQY_NO:      vFacet_Sqy_No(szChrSeq);      break;
    case SF_SQY_PRM:     vFacet_Sqy_Prm(szChrSeq);     break;
    case SF_SQY_RSL:     vFacet_Sqy_Rsl(szChrSeq);     break;
    case SF_RLN_OP:      vFacet_Rln_Op(szChrSeq);      break;
    case SF_SQY_OP:      vFacet_Sqy_Op(szChrSeq);      break;
    case SF_STG_CAST:    vFacet_Stg_Cast(szChrSeq);    break;
    case SF_STR_FN:      vFacet_Str_Fn(szChrSeq);      break;
    case SF_STR_LIT:     vFacet_Str_Lit(szChrSeq);     break;
    case SF_TBL_COL:     vFacet_Tbl_Col(szChrSeq);     break;
    case SF_TBL_NM:      vFacet_Tbl_Nm(szChrSeq);      break;
    case SF_TBL_SYN:     vFacet_Tbl_Syn(szChrSeq);     break;
    case SF_THEN:        vFacet_Then(szChrSeq);        break;
    case SF_TYPE:        vFacet_Type(szChrSeq);        break;
    case SF_WHEN:        vFacet_When(szChrSeq);        break;
    case SF_XK_CAST:     vFacet_Xk_Cast(szChrSeq);     break;

    case SF_RW_AGGREGATES:  vFacet_Rw_Aggregates(szChrSeq);  break;

    default: NEVER_REACH;
  }

  RASSERT(bHasFacets());
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
void Symbol::vConcatFacet(eSF eFacet, Stz* szFacet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  //vFacetRotateSet(eFacet, szFacet);

  vSetFacet(eFacet, szFacet);
  vRotateFacet(eFacet);
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
void Symbol::vDump(Stz* szTitle)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  s4K sProperties;
  Y_vTo_String(sProperties);

  vTrace("Symbol %s: %p\n"_Stz
         ,szTitle
         ,this
         );

  for (i8 i_Facet = 0; i_Facet < m_nFacets; i_Facet++)
    vTrace("%s%-12s:%lld %s~%lld\n"_Stz
           ,Stz::szTab(1)
           ,aSF[xaFacet[i_Facet].m_eFacet]
           ,(i8)xaFacet[i_Facet].m_eFacet
           ,*szFacet(xaFacet[i_Facet].m_eFacet)->azChar() ? szFacet(xaFacet[i_Facet].m_eFacet) : "??Symbol"_Stz
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
eRO Symbol::eEval_rlnOp(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  eRO eRelnOp = eEvalRelnOp(szFacet(SF_RLN_OP));

  return eRelnOp;
}

/*----------------------------------------------------------------------------*/
/* evaluate sub-query op                                                      */
/*----------------------------------------------------------------------------*/
eQO Symbol::eEval_sqyOp(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  eQO eSubQryOp = eEvalSubQryOp(szFacet(SF_SQY_OP));

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

  if (SF_NONE == eFacet)
    return szFacet(0);

  for (i8 i_Facet = 0; i_Facet < m_nFacets; i_Facet++) {
    if (eFacet == xaFacet[i_Facet].m_eFacet)
      return xaFacet[i_Facet].m_sChrSeq;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* scan for symbol facet                                                      */
/*----------------------------------------------------------------------------*/
Stz* Symbol::szFacet(i8 i_Facet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(i_Facet < nFacets());
  return xaFacet[i_Facet].m_sChrSeq;
}

/*----------------------------------------------------------------------------*/
/* get facet for bound                                                        */
/*----------------------------------------------------------------------------*/
Stz* Symbol::szFacet_bound(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(bIs_bound(&xaFacet[0]));

  return xaFacet[0].m_sChrSeq;
}

/*----------------------------------------------------------------------------*/
/* get facet for rlnConj                                                      */
/*----------------------------------------------------------------------------*/
Stz* Symbol::szFacet_rlnConj(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(bIs_rlnConj(&xaFacet[0]));

  return xaFacet[0].m_sChrSeq;
}

/*----------------------------------------------------------------------------*/
/* get one and only facet                                                     */
/*----------------------------------------------------------------------------*/
Stz* Symbol::szFacet_single(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(1 == nFacets());

  return xaFacet[0].m_sChrSeq;
}

/*----------------------------------------------------------------------------*/
/* get facet for term                                                         */
/*----------------------------------------------------------------------------*/
Stz* Symbol::szFacet_term(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(bIs_term(&xaFacet[0]));

  return xaFacet[0].m_sChrSeq;
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
        '$'     == xaFacet[i_Facet].m_sChrSeq.azChar()[1] &&
        cTblSyn == xaFacet[i_Facet].m_sChrSeq.azChar()[2])
      return i_Facet;
  }
  return C_UNKNOWN;
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
bool Symbol::bHasFacet(eSF eFacet, Stz* szChrSeq)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE != eFacet);

  i8 i_Facet = this->i_Facet(eFacet);
  if (C_UNKNOWN == i_Facet)
    return false;

  if (szFacet(i_Facet)->bSame(szChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* does symbol have given facet?                                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bHasFacet(Stz* szChrSeq)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  for (i8 i_Facet = 0; i_Facet < m_nFacets; i_Facet++) {
    if (szFacet(i_Facet)->bSame(szChrSeq))
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

  if (m_nFacets)
    return true;

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

  RASSERT(SF_TBL_SYN == eFacet(0));

  char cSyn = cTblSyn();

  vFacetSprint(SF_TBL_SYN,"$T%c"_Stz, cSyn + 1);

  RASSERT(bHasFacet(SF_TBL_SYN));
}

/*----------------------------------------------------------------------------*/
/* is the symbol a SF_EXP_COL,SF_KEY_COL,SF_ITM_COL,SF_NMD_COL,SF_TBL_COL     */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Col(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bHasFacet(SF_EXP_COL))  return true;
  if (bHasFacet(SF_KEY_COL))  return true;
  if (bHasFacet(SF_ITM_COL))  return true;
  if (bHasFacet(SF_TBL_COL))  return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a literal?                                                  */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Lit(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bHasFacet(SF_BOL_LIT)) return true;
  if (bHasFacet(SF_DCM_LIT)) return true;
  if (bHasFacet(SF_FLT_LIT)) return true;
  if (bHasFacet(SF_INT_LIT)) return true;

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
/* does this symbol have multiple facets ?                                    */
/*----------------------------------------------------------------------------*/
bool Symbol::bMultiFaceted(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (nFacets() > 1)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bNeedsBrace(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! bHasFacets())
    return false;

       if (szFacet(0)->bSame("between"_Stz))        return true;
  else if (szFacet(0)->bSame("case"_Stz))           return true;
  else if (szFacet(0)->bSame("in"_Stz))             return true;
  else if (szFacet(0)->bSame("matchLeading"_Stz))   return true;
  else if (szFacet(0)->bSame("matchMiddle"_Stz))    return true;
  else if (szFacet(0)->bSame("matchMiddle1"_Stz))   return true;
  else if (szFacet(0)->bSame("matchTrailing"_Stz))  return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* rotate facet to top                                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vRotateFacet(eSF eFacet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  i8 i_Facet = this->i_Facet(eFacet);
  if (0 == i_Facet)
    return;

  RASSERT(m_nFacets);

  Facet xFacet_tmp;

  xFacet_tmp.m_eFacet  = this->eFacet(0);
  xFacet_tmp.m_sChrSeq = szFacet(0);

  xaFacet[0].m_eFacet  = xaFacet[i_Facet].m_eFacet;
  xaFacet[0].m_sChrSeq = xaFacet[i_Facet].m_sChrSeq;

  xaFacet[i_Facet].m_eFacet  = xFacet_tmp.m_eFacet;
  xaFacet[i_Facet].m_sChrSeq = xFacet_tmp.m_sChrSeq;
}

/*----------------------------------------------------------------------------*/
/* set facet                                                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vSetFacet(eSF eFacet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  switch (eFacet) {

    case SF_TBL_SYN_rep: vFacet_Tbl_Syn_rep(); break;

    default: NEVER_REACH;
  }

  RASSERT(bHasFacets());

}

/*----------------------------------------------------------------------------*/
/* set facet                                                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vSetFacet(eSF eFacet, char cChr)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  switch (eFacet) {

    case SF_TBL_SYN: vFacet_Tbl_Syn(cChr); break;

    default: NEVER_REACH;
  }

  RASSERT(bHasFacets());

}

/*----------------------------------------------------------------------------*/
/* set facet                                                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vSetFacet(eSF eFacet, char cChr, i8 iInt)
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
/* set facet                                                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vSetFacet(eSF eFacet, i8 iInt)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  switch (eFacet) {

    case SF_EXP_COL:    vFacet_Exp_Col(iInt);    break;
    case SF_IDX_COL:    vFacet_Idx_Col(iInt);    break;
    case SF_INT_LIT:    vFacet_Int_Lit(iInt);    break;
    case SF_ITM_COL:    vFacet_Itm_Col(iInt);    break;
    case SF_KEY_COL:    vFacet_Key_Col(iInt);    break;
    case SF_NMD_COL:    vFacet_Nmd_Col(iInt);    break;
    case SF_SQY_PRM:    vFacet_Sqy_Prm(iInt);    break;
    case SF_SQY_RSL:    vFacet_Sqy_Rsl(iInt);    break;
    case SF_SQY_NO:     vFacet_Sqy_No(iInt);     break;
    case SF_STG_NO:     vFacet_Stg_Nm(iInt);     break;
    case SF_LIST_CNT:   vFacet_List_Cnt(iInt);   break;

    default: NEVER_REACH;
  }

  RASSERT(bHasFacets());

}

/*----------------------------------------------------------------------------*/
/* set facet                                                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vSetFacet(eSF eFacet, i8 iInt, Symbol* pSymbol)
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
/* set facet                                                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vSetFacet(Facet* aFacet_arg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSetFacet(aFacet_arg->m_eFacet, aFacet_arg->m_sChrSeq);
}

/*----------------------------------------------------------------------------*/
/* set facet                                                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vSetFacet(eSF eFacet, Stz* szFacet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  i8 i_Facet = this->i_Facet(eFacet);
  if (C_UNKNOWN == i_Facet) {
    i_Facet = m_nFacets;
    m_nFacets++;
  }

  vFacetSet(i_Facet, eFacet, szFacet);
}

/*----------------------------------------------------------------------------*/
/* set facets                                                                 */
/*----------------------------------------------------------------------------*/
void Symbol::vSetFacets(Symbol* pSymbol_arg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  i8 nFacets_arg = pSymbol_arg->m_nFacets;
  for (i8 i_Facet_arg = 0; i_Facet_arg < nFacets_arg; i_Facet_arg++) {
    Facet* aFacet_arg = pSymbol_arg->aFacet(i_Facet_arg);

    vSetFacet(aFacet_arg->m_eFacet, aFacet_arg->m_sChrSeq);
  }
}

/*----------------------------------------------------------------------------*/
/* does this symbol only have one facet ?                                     */
/*----------------------------------------------------------------------------*/
bool Symbol::bSingleFaceted(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (nFacets() == 1)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get table synonym from $$c.n (or c)                                        */
/*----------------------------------------------------------------------------*/
char Symbol::cTblSyn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(bHasFacets());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (Stz* szChrSeq = szFacet(SF_TBL_COL)) {
    char acFirst  = szChrSeq->azChar()[0];
    char acSecond = szChrSeq->azChar()[1];
    char acThird  = szChrSeq->azChar()[2];

    if ('$' == acFirst
        && '$' == acSecond
        && StrLib::bIsAlpha(acThird))
      return acThird;
    else
      return acFirst;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (Stz* szChrSeq = szFacet(SF_TBL_SYN)) {
    char acFirst  = szChrSeq->azChar()[0];
    char acSecond = szChrSeq->azChar()[1];
    char acThird  = szChrSeq->azChar()[2];

    if ('$' == acFirst
        && 'T' == acSecond
        && StrLib::bIsAlpha(acThird))
      return acThird;
    else
      return acFirst;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (Stz* szChrSeq = szFacet(SF_TBL_SYN_rep)) {
    char acFirst  = szChrSeq->azChar()[0];
    char acSecond = szChrSeq->azChar()[1];
    char acThird  = szChrSeq->azChar()[2];

    if ('$' == acFirst
        && 'T' == acSecond
        && StrLib::bIsAlpha(acThird))
      return acThird;
    else
      return acFirst;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* get integer                                                                */
/*----------------------------------------------------------------------------*/
i8 Symbol::iTo_integer(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(bHasFacets());

  i8 i = szFacet(0)->iToI8();

  return i;
}

/*----------------------------------------------------------------------------*/
/* get ordinal n  (i.e. $n or $In or $Kn or $c.n)                             */
/*----------------------------------------------------------------------------*/
i8 Symbol::iTo_ordinal(eSF eFacet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(bHasFacets());

  i8 i_Ordinal;

  if (SF_NONE == eFacet)
    i_Ordinal = StrLib::iGleanInt(szFacet(0)->azChar());
  else
    i_Ordinal = StrLib::iGleanInt(szFacet(eFacet)->azChar());

  return i_Ordinal;
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

  if (! nFacets())
    return true;

  bool b = false;
  for (i8 i_Facet = 0; i_Facet < nFacets(); i_Facet++) {
    eSF  eFacet  = this->eFacet(i_Facet);
    Stz* szFacet = this->szFacet(eFacet);

    switch (eFacet) {
      case SF_AGG_FN:      b = bIs_Agg_Fn(szFacet);      break;
      case SF_AS:          b = bIs_As(szFacet);          break;
      case SF_BOL_FN:      b = bIs_Bol_Fn(szFacet);      break;
      case SF_BOL_LIT:     b = bIs_Bol_Lit(szFacet);     break;
      case SF_BOL_OP:      b = bIs_Bol_Op(szFacet);      break;
      case SF_BOUND:       b = bIs_Bound(szFacet);       break;
      case SF_BRACE:       b = bIs_Brace(szFacet);       break;
      case SF_CAST:        b = bIs_Cast(szFacet);        break;
      case SF_CASTTYPE:    b = bIs_CastType(szFacet);    break;
      case SF_COL_TYPE:    b = bIs_Col_Type(szFacet);    break;
      case SF_COMMA:       b = bIs_Comma(szFacet);       break;
      case SF_CROSS:       b = bIs_Cross(szFacet);       break;
      case SF_DCM_LIT:     b = bIs_Dcm_Lit(szFacet);     break;
      case SF_DIRECTION:   b = bIs_Direction(szFacet);   break;
      case SF_EXP_COL:     b = bIs_Exp_Col(szFacet);     break;
      case SF_FLD_NM:      b = bIs_Fld_Nm(szFacet);      break;
      case SF_FLT_LIT:     b = bIs_Flt_Lit(szFacet);     break;
      case SF_GROUPBY:     b = bIs_GroupBy(szFacet);     break;
      case SF_IDX_COL:     b = bIs_Idx_Col(szFacet);     break;
      case SF_IDX_NM:      b = bIs_Idx_Nm(szFacet);      break;
      case SF_INT_LIT:     b = bIs_Int_Lit(szFacet);     break;
      case SF_INTERVAL_DT: b = bIs_Interval_DT(szFacet); break;
      case SF_INTERVAL_YM: b = bIs_Interval_YM(szFacet); break;
      case SF_IVL_FN:      b = bIs_Ivl_Fn(szFacet);      break;
      case SF_ITM_COL:     b = bIs_Itm_Col(szFacet);     break;
      case SF_KEY_COL:     b = bIs_Key_Col(szFacet);     break;
      case SF_LIST_CNT:    b = bIs_List_Cnt(szFacet);    break;
      case SF_MTH_OP:      b = bIs_Mth_Op(szFacet);      break;
      case SF_NMD_COL:     b = bIs_Nmd_Col(szFacet);     break;
      case SF_NOTE:        b = bIs_Note(szFacet);        break;
      case SF_NULL:        b = bIs_Null(szFacet);        break;
      case SF_PARENTHESIS: b = bIs_Parenthesis(szFacet); break;
      case SF_RLN_OP:      b = bIs_Rln_Op(szFacet);      break;
      case SF_SQY_NM:      b = bIs_Sqy_Nm(szFacet);      break;
      case SF_SQY_NO:      b = bIs_Sqy_No(szFacet);      break;
      case SF_SQY_OP:      b = bIs_Sqy_Op(szFacet);      break;
      case SF_SQY_PRM:     b = bIs_Sqy_Prm(szFacet);     break;
      case SF_SQY_RSL:     b = bIs_Sqy_Rsl(szFacet);     break;
      case SF_STG_NO:      b = bIs_Stg_Nm(szFacet);      break;
      case SF_STG_CAST:    b = bIs_Stg_Cast(szFacet);    break;
      case SF_STR_FN:      b = bIs_Str_Fn(szFacet);      break;
      case SF_STR_LIT:     b = bIs_String(szFacet);      break;
      case SF_TBL_COL:     b = bIs_Tbl_Col(szFacet);     break;
      case SF_TBL_NM:      b = bIs_Tbl_Nm(szFacet);      break;
      case SF_TBL_SYN:     b = bIs_Tbl_Syn(szFacet);     break;
      case SF_TBL_SYN_rep: b = bIs_Tbl_Syn_rep(szFacet); break;
      case SF_TYPE:        b = bIs_Type(szFacet);        break;
      case SF_WHEN:        b = bIs_When(szFacet);        break;
      case SF_XK_CAST:     b = bIs_Xk_Cast(szFacet);     break;

      case SF_RW_AGGREGATES: b = bIs_Rw_Aggregates(szFacet);  break;

      default:
        NEVER_REACH
    }
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

  RASSERT(! bHasFacets());

  vSetFacet(SF_AGG_FN,szAggFn);

  RASSERT(bHasFacet(SF_AGG_FN));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given as                                                */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_As(Stz* szAs)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_AS,szAs);

  RASSERT(bHasFacet(SF_AS));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given boolean fn                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Bol_Fn(Stz* szBolFn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_BOL_FN,szBolFn);

  RASSERT(bHasFacet(SF_BOL_FN));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given boolean literal                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Bol_Lit(Stz* szBolLit)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_BOL_LIT,szBolLit);

  RASSERT(bHasFacet(SF_BOL_LIT));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given boolean op                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Bol_Op(Stz* szBolOp)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_BOL_OP,szBolOp);

  RASSERT(bHasFacet(SF_BOL_OP));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given bound                                             */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Bound(Stz* szBound)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_BOUND,szBound);

  RASSERT(bHasFacet(SF_BOUND));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given brace                                             */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Brace(Stz* szBrace)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_BRACE,szBrace);

  RASSERT(bHasFacet(SF_BRACE));
}

/*----------------------------------------------------------------------------*/
/* set this symbol CASE                                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Case(Stz* szCase)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_CASE,szCase);

  RASSERT(bHasFacet(SF_CASE));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given as                                                */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Cast(Stz* szCast)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_CAST,szCast);

  RASSERT(bHasFacet(SF_CAST));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given as                                                */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_CastType(Stz* szCastType)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_CASTTYPE,szCastType);

  RASSERT(bHasFacet(SF_CASTTYPE));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given comma                                             */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Comma(Stz* szComma)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_COMMA,szComma);

  RASSERT(bHasFacet(SF_COMMA));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given cross                                             */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Cross(Stz* szCross)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_CROSS,szCross);

  RASSERT(bHasFacet(SF_CROSS));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given type                                              */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Col_Type(Stz* szColType)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_COL_TYPE,szColType);

  RASSERT(bHasFacet(SF_COL_TYPE));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given decimal literal                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Dcm_Lit(Stz* szDcmLit)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_DCM_LIT,szDcmLit);

  RASSERT(bHasFacet(SF_DCM_LIT));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given direction                                         */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Direction(Stz* szDirection)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_DIRECTION,szDirection);

  RASSERT(bHasFacet(SF_DIRECTION));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to ELSE                                                    */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Else(Stz* szElse)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_ELSE,szElse);

  RASSERT(bHasFacet(SF_ELSE));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to ENDIF                                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_EndIf(Stz* szEndIf)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_ENDIF,szEndIf);

  RASSERT(bHasFacet(SF_ENDIF));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given expr column number                                */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Exp_Col(Stz* szExpCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSetFacet(SF_EXP_COL,szExpCol);
  vRotateFacet(SF_EXP_COL);

  RASSERT(bHasFacet(SF_EXP_COL));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given fold name                                         */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Fld_Nm(Stz* szFldNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_FLD_NM,szFldNm);

  RASSERT(bHasFacet(SF_FLD_NM));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to be a float literal                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Flt_Lit(Stz* szFltLit)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_FLT_LIT,szFltLit);

  RASSERT(bHasFacet(SF_FLT_LIT));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given group-by                                          */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_GroupBy(Stz* szGroupBy)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_GROUPBY,szGroupBy);

  RASSERT(bHasFacet(SF_GROUPBY));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given index-column                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Idx_Col(i8 iIdxCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vFacetSprint(SF_IDX_COL,"$I%lld"_Stz, iIdxCol);

  RASSERT(bHasFacet(SF_IDX_COL));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given index-column                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Idx_Col(Stz* szIdxCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_IDX_COL,szIdxCol);

  RASSERT(bHasFacet(SF_IDX_COL));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to a index name                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Idx_Nm(Stz* szIdxNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_IDX_NM,szIdxNm);

  RASSERT(bHasFacet(SF_IDX_NM));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given integer literal                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Int_Lit(i8 iIntLit)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vFacetSprint(SF_INT_LIT, "%lld"_Stz, iIntLit);

  RASSERT(bHasFacet(SF_INT_LIT));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to be a integer literal                                    */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Int_Lit(Stz* szIntLit)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_INT_LIT,szIntLit);

  RASSERT(bHasFacet(SF_INT_LIT));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given INTERVAL_DT                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Interval_DT(Stz* szInterval_DT)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_INTERVAL_DT,szInterval_DT);

  RASSERT(bHasFacet(SF_INTERVAL_DT));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given INTERVAL_YM                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Interval_YM(Stz* szInterval_YM)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_INTERVAL_YM,szInterval_YM);

  RASSERT(bHasFacet(SF_INTERVAL_YM));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given itemised-column-name                              */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Itm_Col(i8 i_Child)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vFacetSprint(SF_ITM_COL,"C%lld_%s"_Stz, i_Child, szFacet(SF_NMD_COL));
  vRotateFacet(SF_ITM_COL);

  RASSERT(bHasFacet(SF_ITM_COL));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given itemised-column-name                              */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Itm_Col(i8 i_Child, Symbol* pSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vFacetSprint(SF_ITM_COL,"C%lld_%s"_Stz,
               i_Child,
               pSymbol->szFacet(SF_NMD_COL));

  RASSERT(bHasFacet(SF_ITM_COL));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given interval fn                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Ivl_Fn(Stz* szIvlFn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_IVL_FN,szIvlFn);

  RASSERT(bHasFacet(SF_IVL_FN));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given list count                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_List_Cnt(i8 nListCnt)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vFacetSprint(SF_LIST_CNT, "#%lld"_Stz, nListCnt);

  RASSERT(bHasFacet(SF_LIST_CNT));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given list count                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_List_Cnt(Stz* szListCnt)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_LIST_CNT,szListCnt);

  RASSERT(bHasFacet(SF_LIST_CNT));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given key-column                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Key_Col(i8 iKeyCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(0 == nFacets() || SF_KEY_COL == eFacet(0));

  vFacetSprint(SF_KEY_COL, "$K%lld"_Stz, iKeyCol);

  RASSERT(bHasFacet(SF_KEY_COL));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given key-column                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Key_Col(Stz* szKeyCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_KEY_COL,szKeyCol);

  RASSERT(bHasFacet(SF_KEY_COL));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given math op                                           */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Mth_Op(Stz* szMthOp)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_MTH_OP,szMthOp);

  RASSERT(bHasFacet(SF_MTH_OP));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to NEGATE                                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Negate(Stz* szNegate)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_NEGATE,szNegate);

  RASSERT(bHasFacet(SF_NEGATE));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given synthetic-as no                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Nmd_Col(i8 iColNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vFacetSprint(SF_NMD_COL, "COL%lld"_Stz, iColNo);

  RASSERT(bHasFacet(SF_NMD_COL));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given column-name                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Nmd_Col(Stz* szColNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSetFacet(SF_NMD_COL,szColNm);
  vRotateFacet(SF_NMD_COL);

  RASSERT(bHasFacet(SF_NMD_COL));
  RASSERT(!bHasFacet(SF_ITM_COL));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given note                                              */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Note(Stz* szNote)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_NOTE,szNote);

  RASSERT(bHasFacet(SF_NOTE));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to NULL                                                    */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Null(Stz* szNull)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_NULL,szNull);

  RASSERT(bHasFacet(SF_NULL));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given parenthesis                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Parenthesis(Stz* szParenthesis)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_PARENTHESIS,szParenthesis);

  RASSERT(bHasFacet(SF_PARENTHESIS));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given relational op                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Rln_Op(Stz* szRlnOp)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(0 == nFacets() || SF_SQY_OP == eFacet(0));

  vSetFacet(SF_RLN_OP,szRlnOp);
  vRotateFacet(SF_RLN_OP);

  RASSERT(bHasFacet(SF_RLN_OP));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given pipeline name                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Sqy_Nm(Stz* szPpeNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_SQY_NM, szPpeNm);

  RASSERT(bHasFacet(SF_SQY_NM));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given subquery-number                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Sqy_No(i8 iSqyNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vFacetSprint(SF_SQY_NO, "$Q%lld"_Stz, iSqyNo);

  RASSERT(bHasFacet(SF_SQY_NO));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given subquery-number                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Sqy_No(Stz* szSqyNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSetFacet(SF_SQY_NO, szSqyNo);

  RASSERT(bHasFacet(SF_SQY_NO));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given subquery op                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Sqy_Op(Stz* szSqyOp)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(0 == nFacets() || SF_SQY_OP == eFacet(0));

  vFacetSprint(SF_SQY_OP,"%s"_Stz,szSqyOp);
  vRotateFacet(SF_SQY_OP);

  RASSERT(bHasFacet(SF_SQY_OP));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given parameter column                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Sqy_Prm(i8 iOrdinal)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vFacetSprint(SF_SQY_PRM, "%%%lld"_Stz, iOrdinal);

  RASSERT(bHasFacet(SF_SQY_PRM));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given parameter column                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Sqy_Prm(Stz* szSqyPrm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_SQY_PRM, szSqyPrm);

  RASSERT(bHasFacet(SF_SQY_PRM));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given parameter column                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Sqy_Rsl(i8 iOrdinal)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  //RASSERT(! bHasFacets());

  vFacetSprint(SF_SQY_RSL, "$R%lld"_Stz, iOrdinal);
  vRotateFacet(SF_SQY_RSL);

  RASSERT(bHasFacet(SF_SQY_RSL));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given parameter column                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Sqy_Rsl(Stz* szSqyRsl)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  //RASSERT(! bHasFacets());

  vSetFacet(SF_SQY_RSL, szSqyRsl);
  vRotateFacet(SF_SQY_RSL);

  RASSERT(bHasFacet(SF_SQY_RSL));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given stage cast                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Stg_Cast(Stz* szStgCast)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(0 == nFacets() || SF_STG_CAST == eFacet(0));

  vSetFacet(SF_STG_CAST,szStgCast);

  RASSERT(bHasFacet(SF_STG_CAST));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to iven stage name                                         */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Stg_Nm(i8 iStgNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vFacetSprint(SF_STG_NO,"S%02lld"_Stz, iStgNo);

  RASSERT(bHasFacet(SF_STG_NO));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given column number                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Exp_Col(i8 iExpCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (m_nFacets) {
    eSF eFacet = this->eFacet(0);
    RASSERT(SF_SQY_RSL == eFacet ||
            SF_EXP_COL == eFacet ||
            SF_NMD_COL == eFacet ||
            SF_TBL_COL == eFacet ||
            SF_KEY_COL == eFacet);
  }

  vFacetSprint(SF_EXP_COL, "$%lld"_Stz, iExpCol);
  vRotateFacet(SF_EXP_COL);

  RASSERT(bHasFacet(SF_EXP_COL));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given string fn                                         */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Str_Fn(Stz* szBolFn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_STR_FN,szBolFn);

  RASSERT(bHasFacet(SF_STR_FN));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given string                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Str_Lit(Stz* szString)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_STR_LIT,szString);

  RASSERT(bHasFacet(SF_STR_LIT));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-column                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Tbl_Col(char cTblSyn, i8 iTblCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vFacetSprint(SF_TBL_COL, "$$%c.%lld"_Stz, cTblSyn, iTblCol);

  RASSERT(bHasFacet(SF_TBL_COL));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-column                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Tbl_Col(Stz* szTblCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(0 == nFacets() ||
          SF_NMD_COL == eFacet(0) ||
          SF_EXP_COL == eFacet(0) ||
          SF_TBL_COL == eFacet(0));

  vSetFacet(SF_TBL_COL, szTblCol);
  //vRotateFacet(SF_TBL_COL);

  RASSERT(bHasFacet(SF_TBL_COL));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table name                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Tbl_Nm(Stz* szTblNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_TBL_NM,szTblNm);

  RASSERT(bHasFacet(SF_TBL_NM));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-synonym                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Tbl_Syn(char cTblSyn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vFacetSprint(SF_TBL_SYN,"$T%c"_Stz, cTblSyn);

  RASSERT(bHasFacet(SF_TBL_SYN));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-synonym                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Tbl_Syn(Stz* szTblSyn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_TBL_SYN,szTblSyn);

  RASSERT(bHasFacet(SF_TBL_SYN));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given ttable-synonym replicated                         */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Tbl_Syn_rep(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(0 == nFacets() || SF_TBL_SYN == eFacet(0));

  char cTableSyn_actual = cTblSyn();
  cTableSyn_actual = StrLib::cToUpper(cTableSyn_actual);

  vFacetReset();
  vFacetSprint(SF_TBL_SYN_rep,"$T%c"_Stz, cTableSyn_actual);

  RASSERT(bHasFacet(SF_TBL_SYN_rep));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to THEN                                                    */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Then(Stz* szThen)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_THEN,szThen);

  RASSERT(bHasFacet(SF_THEN));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to TYPE                                                    */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Type(Stz* szType)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_TYPE,szType);

  RASSERT(bHasFacet(SF_TYPE));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given when                                              */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_When(Stz* szWhen)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_WHEN,szWhen);

  RASSERT(bHasFacet(SF_WHEN));
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given xk cast                                           */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Xk_Cast(Stz* szXkCast)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_XK_CAST,szXkCast);

  RASSERT(bHasFacet(SF_XK_CAST));
}

/*----------------------------------------------------------------------------*/
/* set facet of this symbol to AGGREGATES                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Rw_Aggregates(Stz* szAggregates)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(! bHasFacets());

  vSetFacet(SF_RW_AGGREGATES,szAggregates);

  RASSERT(bHasFacet(SF_RW_AGGREGATES));
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

    vSetFacet(aFacet_arg->m_eFacet, aFacet_arg->m_sChrSeq);
    vRotateFacet(aFacet_arg->m_eFacet);
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
/* set facet                                                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vFacetSet(i8 i_Facet, eSF eFacet, Stz* szFacet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(i_Facet < m_nFacets);
  xaFacet[i_Facet].m_eFacet  = eFacet;
  xaFacet[i_Facet].m_sChrSeq = szFacet;
}

/*----------------------------------------------------------------------------*/
/* sprint facet                                                               */
/*----------------------------------------------------------------------------*/
void Symbol::vFacetSprint(eSF eFacet, Stz* szFormat, ...)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  s256 sFacet;

  va_list xArgs;
  va_start(xArgs,szFormat);
  sFacet.vSprintVA(szFormat, xArgs);
  va_end(xArgs);

  vSetFacet(eFacet, sFacet);
}

/*----------------------------------------------------------------------------*/
/* sprint facet                                                               */
/*----------------------------------------------------------------------------*/
void Symbol::vFacetSprint(String* pString, Stz* szFormat, ...)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  va_list xArgs;
  va_start(xArgs,szFormat);
  pString->vSprint(szFormat, xArgs);
  va_end(xArgs);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/