/*============================================================================*/
/* Symbol.cpp -- Symbolic Value                                               */
/*============================================================================*/

#include "Card.hpp"

#include "SketchTkn.hpp"

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

  m_nFacets = 0;
  for (i8 i_Facet = 0; i_Facet < C_FACETS; i_Facet++) {
    xaFacet[i_Facet].m_eFacet = SF_NONE;
  }
  m_pProperties = nullptr;

  _checkInitialised_(CN_SYMBOL);

  _postcondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Symbol::Symbol(eSF eFacet, i8 iInt)
    : Object(CN_SYMBOL)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_nFacets = 0;
  for (i8 i_Facet = 0; i_Facet < C_FACETS; i_Facet++) {
    xaFacet[i_Facet].m_eFacet = SF_NONE;
  }
  m_pProperties = nullptr;

  _checkInitialised_(CN_SYMBOL);

  vFacet(eFacet, iInt);

  _postcondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Symbol::Symbol(eSF eFacet, Stz* szChrSeq)
    : Object(CN_SYMBOL)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_nFacets = 0;
  for (i8 i_Facet = 0; i_Facet < C_FACETS; i_Facet++) {
    xaFacet[i_Facet].m_eFacet = SF_NONE;
  }
  m_pProperties = nullptr;

  _checkInitialised_(CN_SYMBOL);

  vFacet(eFacet, szChrSeq);

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
/* determine the ordinal difference between seq 1 and 2                       */
/*----------------------------------------------------------------------------*/
i8 Symbol::iOrdDiff(Stz* szSeq1, Stz* szSeq2)
{
  _static_

  char* acSeq1 = szSeq1->azChar();
  char* acSeq2 = szSeq2->azChar();
  while (*acSeq1 && ! StrLib::bIsDigit(*acSeq1) && *acSeq1 == *acSeq2) {
    acSeq1++;
    acSeq2++;
  }

  if (StrLib::bIsDigit(*acSeq1) &&  StrLib::bIsDigit(*acSeq2)) {
    i8 iSeq1 = StrLib::iGleanInt(acSeq1);
    i8 iSeq2 = StrLib::iGleanInt(acSeq2);

    if (iSeq2 >= iSeq1) {
      i8 iDiff = iSeq2 - iSeq1;
      return iDiff;
    }
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* evaluate sub-query op                                                      */
/*----------------------------------------------------------------------------*/
eQO Symbol::eEvalSubQryOp(Stz* szChrSeq)
{
  _static_

       if (szChrSeq->bSame("exists"_Stz))    return QO_EXISTS_OP;
  else if (szChrSeq->bSame("notExists"_Stz)) return QO_NOT_EXISTS_OP;
  else if (szChrSeq->bSame("in"_Stz))        return QO_IN_OP;
  else if (szChrSeq->bSame("notIn"_Stz))     return QO_NOT_IN_OP;

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
eSF Symbol::eEvalSymbol(Stz* szChrSeq)
{
  _static_

  if (bIs_Agg_Fn(szChrSeq))       return SF_AGG_FN;
  if (bIs_As(szChrSeq))           return SF_AS;
  if (bIs_Bol_Fn(szChrSeq))       return SF_BOL_FN;
  if (bIs_Bol_Lit(szChrSeq))      return SF_BOL_LIT;
  if (bIs_Bol_Op(szChrSeq))       return SF_BOL_OP;
  if (bIs_Bound(szChrSeq))        return SF_BOUND;
  if (bIs_Brace(szChrSeq))        return SF_BRACE;
  if (bIs_Case(szChrSeq))         return SF_CASE;
  if (bIs_Cast(szChrSeq))         return SF_CAST;
  if (bIs_CastType(szChrSeq))     return SF_CASTTYPE;
  if (bIs_Col_No(szChrSeq))       return SF_COL_NO;
  if (bIs_Col_Tc(szChrSeq))       return SF_COL_TC;
  if (bIs_Col_Type(szChrSeq))     return SF_COL_TYPE;
  if (bIs_Comma(szChrSeq))        return SF_COMMA;
  if (bIs_Cross(szChrSeq))        return SF_CROSS;
  if (bIs_Dcm_Lit(szChrSeq))      return SF_DCM_LIT;
  if (bIs_Direction(szChrSeq))    return SF_DIRECTION;
  if (bIs_Else(szChrSeq))         return SF_ELSE;
  if (bIs_EndIf(szChrSeq))        return SF_ENDIF;
  if (bIs_Flt_Lit(szChrSeq))      return SF_FLT_LIT;
  if (bIs_GroupBy(szChrSeq))      return SF_GROUPBY;
  if (bIs_Idx_Col(szChrSeq))      return SF_IDX_COL;
  if (bIs_Int_Lit(szChrSeq))      return SF_INT_LIT;
  if (bIs_Interval_DT(szChrSeq))  return SF_INTERVAL_DT;
  if (bIs_Interval_YM(szChrSeq))  return SF_INTERVAL_YM;
  if (bIs_Ivl_Fn(szChrSeq))       return SF_IVL_FN;
  if (bIs_Key_Col(szChrSeq))      return SF_KEY_COL;
  if (bIs_Mth_Op(szChrSeq))       return SF_MTH_OP;
  if (bIs_Negate(szChrSeq))       return SF_NEGATE;
  if (bIs_Null(szChrSeq))         return SF_NULL;
  if (bIs_Parenthesis(szChrSeq))  return SF_PARENTHESIS;
  if (bIs_Prm_Col(szChrSeq))      return SF_PRM_COL;
  if (bIs_Rln_Op(szChrSeq))       return SF_RLN_OP;
  if (bIs_Sqy_Prm(szChrSeq))      return SF_PRM_COL;
  if (bIs_Sqy_Op(szChrSeq))       return SF_SQY_OP;
  if (bIs_Sqy_Rsl(szChrSeq))      return SF_SQY_NM;
  if (bIs_Stg_Cast(szChrSeq))     return SF_STG_CAST;
  if (bIs_Str_Fn(szChrSeq))       return SF_STR_FN;
  if (bIs_String(szChrSeq))       return SF_STR_LIT;
  if (bIs_Then(szChrSeq))         return SF_THEN;
  if (bIs_Type(szChrSeq))         return SF_TYPE;
  if (bIs_When(szChrSeq))         return SF_WHEN;
  if (bIs_Xrf_No(szChrSeq))       return SF_XRF_NO;

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

       if (szArg->bSame("avg"_Stz))      return true;
  else if (szArg->bSame("count"_Stz))    return true;
  else if (szArg->bSame("countAll"_Stz)) return true;
  else if (szArg->bSame("max"_Stz))      return true;
  else if (szArg->bSame("min"_Stz))      return true;
//else if (szArg->bSame("stddev"_Stz))   return true;
  else if (szArg->bSame("sum"_Stz))      return true;

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
/* is this a column name?                                                     */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_As_Nm(Stz* szArg)
{
  _static_

  if (! bIs_Col_Nm(szArg))
    return false;

  return true;
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
/* is this a column name?                                                     */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Col_Nm(Stz* szArg)
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
  if (bIs_Sqy_Op(szArg))      return false;
  if (bIs_Stg_Cast(szArg))    return false;
  if (bIs_Str_Fn(szArg))      return false;
  if (bIs_Then(szArg))        return false;
  if (bIs_Type(szArg))        return false;
  if (bIs_When(szArg))        return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a stream-column?  (i.e. $n)                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Col_No(Stz* szArg)
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
/* is this a symbol for a table-column (i.e. $c.n)                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Col_Tc(Stz* szArg)
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
/* is this a symbol for a parameter-column?  (i.e. %n)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Prm_Col(Stz* szArg)
{
  _static_

  char acFirst  = szArg->azChar()[0];
  char acSecond = szArg->azChar()[1];

  if ('%' == acFirst &&
      StrLib::bIsDigit(acSecond))
    return true;
  else
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
/* is value a __subQuery like name?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Crl_Key(BASE::Stz *szChrSeq)
{
  _static_

  if (StrLib::bBeginsWith(szChrSeq->azChar(), C_SQCRLKEY->azChar()))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is value a __subQuery like name?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Nm(BASE::Stz *szChrSeq)
{
  _static_

  if (StrLib::bBeginsWith(szChrSeq->azChar(), "__subquery") ||
      StrLib::bBeginsWith(szChrSeq->azChar(), "__sq"))
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
/* is this a symbol for a subQuery-column?  (i.e. __n)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Rsl(Stz* szArg)
{
  _static_

  char acFirst  = szArg->azChar()[0];
  char acSecond = szArg->azChar()[1];
  char acThird  = szArg->azChar()[2];
  char acFourth = szArg->azChar()[3];

  if ('_' == acFirst
      && '_' == acSecond
      && StrLib::bIsDigit(acThird))
    return true;

  if ('_' == acFirst
      && '_' == acSecond
      && 'S' == acThird
      && 'Q' == acFourth)
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
bool Symbol::bIs_Stg_Nm(BASE::Stz *szChrSeq)
{
  _static_

  char acFirst  = szChrSeq->azChar()[0];
  char acSecond = szChrSeq->azChar()[1];

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
/* is this a symbol a table synonym?  (i.e. $c)                               */
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
/* is this a symbol a table synonym?  (i.e. $c)                               */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Tbl_Syn_rep(Stz* szArg)
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
/* is this a symbol for a cross-reference?  (i.e. [n])                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Xrf_No(Stz* szArg)
{
  _static_

  char acFirst  = szArg->azChar()[0];
  char acSecond = szArg->azChar()[1];

  if ('#' == acFirst &&
      StrLib::bIsDigit(acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this an itemised column name?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsItemised(Stz* szArg)
{
  _static_

  char* azCrs = szArg->azChar();

  if ('C' != *azCrs++)
    return false;

  if (!StrLib::bIsDigit(*azCrs))
    return false;

  while(StrLib::bIsDigit(*azCrs))
    azCrs++;

  if ('_' != *azCrs)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* strip disambiguation                                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vItemStrip(String* psArg)
{
  _static_

  char* azCrs = psArg->azStr();

  RASSERT(*azCrs++ == 'C');

  RASSERT(StrLib::bIsDigit(*azCrs));

  while(StrLib::bIsDigit(*azCrs))
    azCrs++;

  RASSERT(*azCrs++ == '_');

  psArg->vCopy((Stz*)azCrs);
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

  if (bIs_Col_No(szChrSeq))   return false;
  if (bIs_Col_Tc(szChrSeq))   return false;
  if (bIs_Sqy_Prm(szChrSeq))  return false;
  if (bIs_Sqy_Rsl(szChrSeq))  return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* find eCT by name                                                           */
/*----------------------------------------------------------------------------*/
eCT Symbol::eTo_eCT_str(Stz* szType)
{
  _static_

  for (i8 i_Type = 0; i_Type < CT_ENUMS; i_Type++)
    if (aCT[i_Type]->bSame(szType))
      return (eCT)i_Type;

  return CT_NONE;
}

/*----------------------------------------------------------------------------*/
/* find eCT by name                                                           */
/*----------------------------------------------------------------------------*/
eCT Symbol::eTo_eCT_char(Stz* szType)
{
  _static_

  for (i8 i_Type = 0; i_Type < CT_ENUMS; i_Type++)
    if (szCT_char[i_Type]->bSame(szType))
      return (eCT)i_Type;

  return CT_NONE;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get column type                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vDump(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  s4K sProperties;
  vPpy_To_String(sProperties);

  vTrace("Symbol : %lld %s: %s {%s} %s\n"_Stz
         , (i8)xaFacet[0].m_eFacet
         , aSF[xaFacet[0].m_eFacet]

         , *szSF_chrSeq(SF_NONE)->azChar() ? szSF_chrSeq(SF_NONE) : "??Symbol"_Stz
         , sProperties.szStz()
         , szWhatIs()
  );
}

/*----------------------------------------------------------------------------*/
/* evaluate relational op                                                     */
/*----------------------------------------------------------------------------*/
eRO Symbol::eEvalRelnOp(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  eRO eRelnOp = eEvalRelnOp(xaFacet[0].m_sChrSeq);

  return eRelnOp;
}

/*----------------------------------------------------------------------------*/
/* evaluate sub-query op                                                      */
/*----------------------------------------------------------------------------*/
eQO Symbol::eEvalSubQryOp(void)
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

  for (i8 i_Facet = 0; i_Facet < C_FACETS; i_Facet++) {
    if (eFacet == xaFacet[i_Facet].m_eFacet)
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

}

/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet(eSF eFacet, char cChr, i8 iInt)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  switch (eFacet) {

    case SF_COL_TC:    vFacet_Col_Tc(cChr,iInt);    break;

    default: NEVER_REACH;
  }

}

/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet(eSF eFacet, i8 iInt)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  switch (eFacet) {

    case SF_AS_NM_itm:  vFacet_As_Nm_itm(iInt);  break;
    case SF_AS_NM_syn:  vFacet_As_Nm_syn(iInt);  break;
    case SF_COL_NO:     vFacet_Col_No(iInt);     break;
    case SF_COL_NM_dis: vFacet_Col_Nm_dis(iInt); break;
    case SF_IDX_COL:    vFacet_Idx_Col(iInt);    break;
    case SF_INT_LIT:    vFacet_Int_Lit(iInt);    break;
    case SF_KEY_COL:    vFacet_Key_Col(iInt);    break;
    case SF_SQY_NM:     vFacet_Sqy_Nm(iInt);     break;
    case SF_STG_NO:     vFacet_Stg_Nm(iInt);     break;
    case SF_PRM_COL:    vFacet_Prm_Col(iInt);    break;
    case SF_XRF_NO:     vFacet_Xrf_No(iInt);     break;

    default: NEVER_REACH;
  }

}

/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet(eSF eFacet, i8 iInt, Symbol* pSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  switch (eFacet) {

    case SF_COL_NM_dis: vFacet_Col_Nm_dis(iInt,pSymbol); break;

    default: NEVER_REACH;
  }

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
    case SF_AS_NM:       vFacet_As_Nm(szChrSeq);       break;
    case SF_BOL_FN:      vFacet_Bol_Fn(szChrSeq);      break;
    case SF_BOL_LIT:     vFacet_Bol_Lit(szChrSeq);     break;
    case SF_BOL_OP:      vFacet_Bol_Op(szChrSeq);      break;
    case SF_BOUND:       vFacet_Bound(szChrSeq);       break;
    case SF_BRACE:       vFacet_Brace(szChrSeq);       break;
    case SF_CASE:        vFacet_Case(szChrSeq);        break;
    case SF_CAST:        vFacet_Cast(szChrSeq);        break;
    case SF_CASTTYPE:    vFacet_CastType(szChrSeq);    break;
    case SF_COL_NM:      vFacet_Col_Nm(szChrSeq);      break;
    case SF_COL_NM_syn:  vFacet_Col_Nm_syn(szChrSeq);  break;
    case SF_COL_NO:      vFacet_Col_No(szChrSeq);      break;
    case SF_COL_TC:      vFacet_Col_Tc(szChrSeq);      break;
    case SF_COL_TYPE:    vFacet_Col_Type(szChrSeq);    break;
    case SF_COMMA:       vFacet_Comma(szChrSeq);       break;
    case SF_CROSS:       vFacet_Cross(szChrSeq);       break;
    case SF_DCM_LIT:     vFacet_Dcm_Lit(szChrSeq);     break;
    case SF_DIRECTION:   vFacet_Direction(szChrSeq);   break;
    case SF_ELSE:        vFacet_Else(szChrSeq);        break;
    case SF_ENDIF:       vFacet_EndIf(szChrSeq);       break;
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
    case SF_NEGATE:      vFacet_Negate(szChrSeq);      break;
    case SF_NOTE:        vFacet_Note(szChrSeq);        break;
    case SF_NULL:        vFacet_Null(szChrSeq);        break;
    case SF_MTH_OP:      vFacet_Mth_Op(szChrSeq);      break;
    case SF_PARENTHESIS: vFacet_Parenthesis(szChrSeq); break;
    case SF_PPE_NM:      vFacet_Ppe_Nm(szChrSeq);      break;
    case SF_PRM_COL:     vFacet_Prm_Col(szChrSeq);     break;
    case SF_RLN_OP:      vFacet_Rln_Op(szChrSeq);      break;
    case SF_SQY_OP:      vFacet_Sqy_Op(szChrSeq);      break;
    case SF_STG_CAST:    vFacet_Stg_Cast(szChrSeq);    break;
    case SF_STR_FN:      vFacet_Str_Fn(szChrSeq);      break;
    case SF_STR_LIT:     vFacet_String(szChrSeq);      break;
    case SF_TBL_NM:      vFacet_Tbl_Nm(szChrSeq);      break;
    case SF_TBL_SYN:     vFacet_Tbl_Syn(szChrSeq);     break;
    case SF_THEN:        vFacet_Then(szChrSeq);        break;
    case SF_TYPE:        vFacet_Type(szChrSeq);        break;
    case SF_WHEN:        vFacet_When(szChrSeq);        break;
    case SF_XK_CAST:     vFacet_Xk_Cast(szChrSeq);     break;
    case SF_XRF_NO:      vFacet_Xrf_No(szChrSeq);      break;

    default: NEVER_REACH;
  }

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
/* true if symbol is given form                                               */
/*----------------------------------------------------------------------------*/
bool Symbol::bFormIs(eSF eSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (eSymbol == xaFacet[0].m_eFacet)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* does symbol a symbolic value?                                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bHasSymbol(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (* xaFacet[0].m_sChrSeq.azChar())
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
/* increment table synonym                                                    */
/*----------------------------------------------------------------------------*/
void Symbol::vInc_Tbl_Syn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

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
/* is this a symbol for a synthetic as?  (i.e. __COLn)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_As_Nm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_AS_NM != xaFacet[0].m_eFacet)
    return false;

  if (bIs_As_Nm(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a synthetic as?  (i.e. __COLn)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_As_Nm_itm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_AS_NM_itm != xaFacet[0].m_eFacet)
    return false;

  if (! Y_bHas(P_Itemised))
    return false;

  if (bIs_As_Nm(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a synthetic as?  (i.e. __COLn)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_As_Nm_syn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_AS_NM != xaFacet[0].m_eFacet)
    return false;

  if (! Y_bHas(P_Synthetic))
    return false;

  return true;
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
/* is this a column name?                                                     */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Col_Nm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_COL_NM != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Col_Nm(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a comma separator?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Col_Nm_dis(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! bIs_Col_Nm())
    return false;

  if (! Y_bHas(P_DisamCol))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a comma separator?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Col_Nm_syn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! bIs_Col_Nm())
    return false;

  if (! Y_bHas(P_Synthetic))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a stream-column?  (i.e. $n)                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Col_No(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_COL_NO != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Col_No(xaFacet[0].m_sChrSeq)) {
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a table-column (i.e. $c.n)                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Col_Tc(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_COL_TC != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Col_Tc(xaFacet[0].m_sChrSeq))
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
/* is this a parameter column?                                                */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Prm_Col(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_PRM_COL != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Prm_Col(xaFacet[0].m_sChrSeq))
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
/* is this a symbol for a cross-refernece?  (i.e. #n)                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Xrf_No(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_XRF_NO != xaFacet[0].m_eFacet)
    return false;

  if (bIs_Xrf_No(xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* strip itemisation                                                          */
/*----------------------------------------------------------------------------*/
void Symbol::vItemStrip(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! bIsItemised(xaFacet[0].m_sChrSeq))
    return;

  vItemStrip(xaFacet[0].m_sChrSeq);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bNeedsBrace(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

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
/* convert properties to string                                               */
/*----------------------------------------------------------------------------*/
void Symbol::vPpy_To_String(String* pString)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (m_pProperties)
    m_pProperties->vToString(pString);
  else
    pString->vCopy("nil"_Stz);
}

/*----------------------------------------------------------------------------*/
/* does this card have same symbol as given argument?                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bSame(char cArg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (*xaFacet[0].m_sChrSeq.azChar() == cArg)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bSame(Stz* szArg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (xaFacet[0].m_sChrSeq.bSame(szArg))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bSame(Symbol* pArg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (xaFacet[0].m_sChrSeq.bSame(pArg->xaFacet[0].m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
char Symbol::cSF_char(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  return * xaFacet[0].m_sChrSeq.azChar();
}

/*----------------------------------------------------------------------------*/
/* get symbol stz                                                             */
/*----------------------------------------------------------------------------*/
Stz* Symbol::szSF_chrSeq(eSF eSymbol_xchk)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_NONE != eSymbol_xchk)
    RASSERT(eSymbol_xchk = xaFacet[0].m_eFacet);

  return xaFacet[0].m_sChrSeq.szStz();
}

/*----------------------------------------------------------------------------*/
/* get symbol int                                                             */
/*----------------------------------------------------------------------------*/
i8 Symbol::iSF_number(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  i8 i = xaFacet[0].m_sChrSeq.iToI8();

  return i;
}

/*----------------------------------------------------------------------------*/
/* get ordinal n  (i.e. $n or $In or $Kn or $c.n)                             */
/*----------------------------------------------------------------------------*/
i8 Symbol::iSF_ordinal(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  i8 i_Col = StrLib::iGleanInt(xaFacet[0].m_sChrSeq.azChar());

  return i_Col;
}

/*----------------------------------------------------------------------------*/
/* get table synonym from $c.n                                                */
/*----------------------------------------------------------------------------*/
char Symbol::cSF_tblSyn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

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
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSymbol(Symbol* pSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_COL_NO == pSymbol->xaFacet[0].m_eFacet) {
    vFacet_Col_No(pSymbol->xaFacet[0].m_sChrSeq);
  }
  else {
    xaFacet[0].m_sChrSeq = pSymbol->xaFacet[0].m_sChrSeq;
    xaFacet[0].m_eFacet = pSymbol->xaFacet[0].m_eFacet;
  }

  if (SF_COL_NO == xaFacet[0].m_eFacet)
    RASSERT(Symbol::bIs_Col_No(xaFacet[0].m_sChrSeq));

  if (SF_COL_TC == xaFacet[0].m_eFacet)
    RASSERT(Symbol::bIs_Col_Tc(xaFacet[0].m_sChrSeq));

}


/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSymbolPpy(Symbol* pSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  xaFacet[0].m_sChrSeq = pSymbol->xaFacet[0].m_sChrSeq;
  xaFacet[0].m_eFacet = pSymbol->xaFacet[0].m_eFacet;

  if (SF_COL_NO == xaFacet[0].m_eFacet)
    RASSERT(Symbol::bIs_Col_No(xaFacet[0].m_sChrSeq));

  if (SF_COL_TC == xaFacet[0].m_eFacet)
    RASSERT(Symbol::bIs_Col_Tc(xaFacet[0].m_sChrSeq));

  Y_vAll(pSymbol);
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
    case SF_AS_NM:       b = bIs_As_Nm();       break;
    case SF_BOL_FN:      b = bIs_Bol_Fn();      break;
    case SF_BOL_LIT:     b = bIs_Bol_Lit();     break;
    case SF_BOL_OP:      b = bIs_Bol_Op();      break;
    case SF_BOUND:       b = bIs_Bound();       break;
    case SF_BRACE:       b = bIs_Brace();       break;
    case SF_CAST:        b = bIs_Cast();        break;
    case SF_CASTTYPE:    b = bIs_CastType();    break;
    case SF_COL_NM:      b = bIs_Col_Nm();      break;
    case SF_COL_NM_syn:  b = bIs_Col_Nm_syn();  break;
    case SF_COL_NO:      b = bIs_Col_No();      break;
    case SF_COL_TC:      b = bIs_Col_Tc();      break;
    case SF_COL_TYPE:    b = bIs_Col_Type();    break;
    case SF_COMMA:       b = bIs_Comma();       break;
    case SF_CROSS:       b = bIs_Cross();       break;
    case SF_DCM_LIT:     b = bIs_Dcm_Lit();     break;
    case SF_DIRECTION:   b = bIs_Direction();   break;
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
    case SF_NOTE:        b = bIs_Note();        break;
    case SF_NULL:        b = bIs_Null();        break;
    case SF_MTH_OP:      b = bIs_Mth_Op();      break;
    case SF_PARENTHESIS: b = bIs_Parenthesis(); break;
    case SF_PPE_NM:      b = bIs_Ppe_Nm();      break;
    case SF_PRM_COL:     b = bIs_Prm_Col();     break;
    case SF_RLN_OP:      b = bIs_Rln_Op();      break;
    case SF_SQY_NM:      b = bIs_Sqy_Nm();      break;
    case SF_SQY_OP:      b = bIs_Sqy_Op();      break;
    case SF_STG_NO:      b = bIs_Stg_Nm();      break;
    case SF_STG_CAST:    b = bIs_Stg_Cast();    break;
    case SF_STR_FN:      b = bIs_Str_Fn();      break;
    case SF_STR_LIT: b = bIs_String();      break;
    case SF_TBL_NM:      b = bIs_Tbl_Nm();      break;
    case SF_TBL_SYN:     b = bIs_Tbl_Syn();     break;
    case SF_TBL_SYN_rep: b = bIs_Tbl_Syn_rep(); break;
    case SF_TYPE:        b = bIs_Type();        break;
    case SF_WHEN:        b = bIs_When();        break;
    case SF_XK_CAST:     b = bIs_Xk_Cast();     break;
    case SF_XRF_NO:      b = bIs_Xrf_No();      break;

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
  if (bIs_As_Nm_syn())    return "SyntheticAs"_Stz;
  if (bIs_Bol_Fn())       return "BoolFn"_Stz;
  if (bIs_Bound())        return "Bound"_Stz;
  if (bIs_Col_No())       return "StreamOrdinal"_Stz;
  if (bIs_Col_Tc())       return "ColumnOrdinal"_Stz;
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
/* get column type                                                            */
/*----------------------------------------------------------------------------*/
eCT Symbol::Y_eColType(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  Stz* szType = Y_szColType();
  if (! szType)
    return CT_NONE;

  eCT eColumnType = eTo_eCT_char(szType);

  return eColumnType;
}

/*----------------------------------------------------------------------------*/
/* get type                                                                   */
/*----------------------------------------------------------------------------*/
Stz* Symbol::Y_szColType(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  Stz* sz = Y_szGet(P_Type);

  return sz;
}

/*----------------------------------------------------------------------------*/
/* get type                                                                   */
/*----------------------------------------------------------------------------*/
void Symbol:: Y_vColType(Stz* szColType)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  Y_vSet(P_Type,szColType);
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
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
Ppy Symbol::Y_xGet(i8 i_Ppy)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  Ppy xPpy;

  if (! m_pProperties)
    return xPpy;

  xPpy = m_pProperties->szValue(i_Ppy);

  return xPpy;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
Ppy Symbol::Y_xGet(Stz* szPpyNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  Ppy xPpy;

  if (! m_pProperties)
    return xPpy;

  xPpy = m_pProperties->szValue(szPpyNm);

  return xPpy;
}


/*----------------------------------------------------------------------------*/
/* has given property ?                                                       */
/*----------------------------------------------------------------------------*/
bool Symbol::Y_bHas(Stz* szPpyNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! m_pProperties)
    return false;

  return m_pProperties->bHasName(szPpyNm);
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

  if (szPpyNm->bSame(P_Type)) {
    //RASSERT(!szPpy->bSame(""_Stz));
    RASSERT(!szPpy->bSame("!"_Stz));
  }
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
/* sprint card property                                                       */
/*----------------------------------------------------------------------------*/
void Symbol::Y_vSetSprint(Stz* szPpyNm, Stz* szFormat, ...)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  s256 sPpy;

  va_list xArgs;
  va_start(xArgs,szFormat);
  StrLib::vSprint(sPpy.azChar(), sPpy.nAreaBytes(), xArgs, (char*)szFormat);
  va_end(xArgs);

  Y_vSet(szPpyNm, sPpy);


  if (szPpyNm->bSame(P_Type))
    RASSERT(! sPpy.bSame("!"_Stz));
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
/* set this symbol to given as-name                                           */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_As_Nm(Stz* szAsNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet || SF_AS_NM == xaFacet[0].m_eFacet);

  vFacetSprint(SF_AS_NM,"%s"_Stz,szAsNm);

  Y_vCut(P_Synthetic);

  RASSERT(bIs_As_Nm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given synthetic-as no                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_As_Nm_itm(i8 iAsNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet || SF_AS_NM == xaFacet[0].m_eFacet);

  vFacetSprint(SF_AS_NM_itm,"C%lld_%s"_Stz, iAsNo, xaFacet[0].m_sChrSeq.szStz());
  Y_vSet(P_Itemised);

  RASSERT(bIs_As_Nm_itm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given synthetic-as no                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_As_Nm_syn(i8 iAsNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_AS_NM,"COL%lld"_Stz, iAsNo);
  Y_vSet(P_Synthetic);

  RASSERT(bIs_As_Nm_syn());
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
/* set this symbol to given column-name                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Col_Nm(Stz* szColNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet || SF_COL_NO == xaFacet[0].m_eFacet);

  vFacetSprint(SF_COL_NM,"%s"_Stz,szColNm);

  Y_vCut(P_DisamCol);

  RASSERT(bIs_Col_Nm());
  RASSERT(! bIs_Col_Nm_dis());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given disambiguated-column-name                         */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Col_Nm_dis(i8 i_Child)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet || SF_COL_NM == xaFacet[0].m_eFacet);

  vFacetSprint(SF_COL_NM,"C%lld_%s"_Stz, i_Child, xaFacet[0].m_sChrSeq.szStz());

  Y_vSet(P_DisamCol);

  RASSERT(bIs_Col_Nm_dis());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given disambiguated-column-name                         */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Col_Nm_dis(i8 i_Child, Symbol* pSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_COL_NM,"C%lld_%s"_Stz, i_Child, pSymbol->xaFacet[0].m_sChrSeq.szStz());

  Y_vSet(P_DisamCol);

  RASSERT(bIs_Col_Nm_dis());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given disambiguated-column-name                         */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Col_Nm_syn(Stz* szColNm_syn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_COL_NM,"%s"_Stz,szColNm_syn);

  Y_vSet(P_Synthetic);

  RASSERT(bIs_Col_Nm_syn());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given column number                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Col_No(i8 iStmColOdn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet || SF_COL_NO == xaFacet[0].m_eFacet || SF_KEY_COL == xaFacet[0].m_eFacet);

  if (bIs_Col_Tc()) {
     Y_vSet(P_TblCol, szSF_chrSeq(SF_COL_TC));
  }
  else
  if (bIs_Key_Col()) {
    Y_vSet(P_KeyCol, szSF_chrSeq(SF_KEY_COL));
  }

  vFacetSprint(SF_COL_NO,"$%lld"_Stz, iStmColOdn);

  RASSERT(bIs_Col_No());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given column nuber                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Col_No(Stz* szColNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet || SF_COL_NO == xaFacet[0].m_eFacet ||
          SF_COL_TC == xaFacet[0].m_eFacet ||

          SF_KEY_COL == xaFacet[0].m_eFacet ||  // todo: these need to be investigated
          SF_PARENTHESIS == xaFacet[0].m_eFacet ||
          SF_PRM_COL == xaFacet[0].m_eFacet ||
          SF_XRF_NO == xaFacet[0].m_eFacet
          );

  if (bIs_Col_Tc()) {
    Y_vSet(P_TblCol, szSF_chrSeq(SF_COL_TC));
  }

  vFacetSprint(SF_COL_NO,"%s"_Stz,szColNo);

  RASSERT(bIs_Col_No());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-column                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Col_Tc(char cTblSyn, i8 iTblColOdn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_COL_TC,"$%c.%lld"_Stz, cTblSyn, iTblColOdn);

  RASSERT(bIs_Col_Tc());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-column                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Col_Tc(Stz* szColTc)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet || SF_COL_NM == xaFacet[0].m_eFacet || SF_COL_TC == xaFacet[0].m_eFacet);

  vFacetSprint(SF_COL_TC,"%s"_Stz,szColTc);

  RASSERT(bIs_Col_Tc());
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
void Symbol::vFacet_Idx_Col(i8 iIdxColOdn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_IDX_COL,"$I%lld"_Stz, iIdxColOdn);

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
/* set this symbol to given key-column                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Key_Col(i8 iKeyOdn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet || SF_KEY_COL == xaFacet[0].m_eFacet);

  vFacetSprint(SF_KEY_COL,"$K%lld"_Stz, iKeyOdn);

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
/* set this symbol to given parameter column                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Prm_Col(i8 iOrdinal)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_PRM_COL,"%%%lld"_Stz, iOrdinal);

  RASSERT(bIs_Sqy_Prm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given parameter column                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Prm_Col(Stz* szPrmCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_PRM_COL,"%s"_Stz,szPrmCol);

  RASSERT(bIs_Sqy_Prm());
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

  vFacetSprint(SF_SQY_NM,"__sq%02lld"_Stz, iSqyNo);

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
/* set this symbol to given cross-reference no                                */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Xrf_No(i8 iXrefNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_XRF_NO,"#%lld"_Stz, iXrefNo);

  RASSERT(bIs_Xrf_No());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given cross-reference no                                */
/*----------------------------------------------------------------------------*/
void Symbol::vFacet_Xrf_No(Stz* szXrefNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == xaFacet[0].m_eFacet);

  vFacetSprint(SF_XRF_NO,"%s"_Stz,szXrefNo);

  RASSERT(bIs_Xrf_No());
}

/*----------------------------------------------------------------------------*/
/* copy facets                                                                */
/*----------------------------------------------------------------------------*/
void Symbol:: vFacetCopy(Symbol* pSymbol_arg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_nFacets = pSymbol_arg->m_nFacets;
  for (i8 i_Facet = 0; i_Facet < m_nFacets; i_Facet++) {
    xaFacet[i_Facet].m_eFacet  = pSymbol_arg->xaFacet[i_Facet].m_eFacet;
    xaFacet[i_Facet].m_sChrSeq = pSymbol_arg->xaFacet[i_Facet].m_sChrSeq;
  }
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

  xaFacet[0].m_eFacet  = eFacet;
  xaFacet[0].m_sChrSeq = ""_Stz;

}

/*----------------------------------------------------------------------------*/
/* rotate facet to top                                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vFacetRotate(eSF eFacet)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  UNUSED(eFacet);

  i8 i_Facet = this->i_Facet(eFacet);
  if (C_UNKNOWN == i_Facet) {
    vFacetPush(eFacet);
    return;
  }

  Facet xFacet_tmp;

  xFacet_tmp.m_eFacet  = xaFacet[0].m_eFacet;
  xFacet_tmp.m_sChrSeq = xaFacet[0].m_sChrSeq;

  xaFacet[0].m_eFacet  = xaFacet[i_Facet].m_eFacet;
  xaFacet[0].m_sChrSeq = xaFacet[i_Facet].m_sChrSeq;

  xaFacet[i_Facet].m_eFacet  = xFacet_tmp.m_eFacet;
  xaFacet[i_Facet].m_sChrSeq = xFacet_tmp.m_sChrSeq;
}

/*----------------------------------------------------------------------------*/
/* sprint facet                                                               */
/*----------------------------------------------------------------------------*/
void Symbol::vFacetSprint(eSF eFacet, Stz* szFormat, ...)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);


  s256 sSymbol;

  va_list xArgs;
  va_start(xArgs,szFormat);
  StrLib::vSprint(sSymbol.azChar(), sSymbol.nAreaBytes(), xArgs, (char*)szFormat);
  va_end(xArgs);

  vFacetRotate(eFacet);
  xaFacet[0].m_eFacet = eFacet;
  xaFacet[0].m_sChrSeq = sSymbol;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/