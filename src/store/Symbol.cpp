/*============================================================================*/
/* Symbol.cpp -- Symbolic Value                                               */
/*============================================================================*/

#include "Card.hpp"
#include "CardCtl.hpp"
#include "Board.hpp"
#include "CardIter.hpp"

#include "Card_Index.hpp"  // TODO: remove
#include "Index.hpp"       // TODO: remove

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

  m_eSymbol = SF_NONE;
  m_pProperties = nullptr;

  _checkInitialised_(CN_SYMBOL);

  _postcondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Symbol::Symbol(eSF eSymbol, i8 iInt)
    : Object(CN_SYMBOL)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_eSymbol = SF_NONE;
  m_pProperties = nullptr;

  _checkInitialised_(CN_SYMBOL);

  vSym(eSymbol,iInt);

  _postcondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Symbol::Symbol(eSF eSymbol, Stz* szChrSeq)
    : Object(CN_SYMBOL)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_eSymbol = SF_NONE;
  m_pProperties = nullptr;

  _checkInitialised_(CN_SYMBOL);

  vSym(eSymbol,szChrSeq);

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
  if (bIs_String(szChrSeq))       return SF_STRING;
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

  vSymSprint("%c"_Stz, cTblSyn);
  m_eSymbol = SF_TBL_SYN;

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

  if (szArg->bSame("extract_year"_Stz))         return true;

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
/* is this a symbol for a cross-refernece?  (i.e. #n)                         */
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

  vTrace("Symbol : %%lld %s\n"_Stz,(i8)m_eSymbol,aSF[m_eSymbol]);
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
/* return symbol form                                                         */
/*----------------------------------------------------------------------------*/
eSF Symbol::eSymbol(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  return m_eSymbol;
}

/*----------------------------------------------------------------------------*/
/* evaluate relational op                                                     */
/*----------------------------------------------------------------------------*/
eRO Symbol::eEvalRelnOp(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  eRO eRelnOp = eEvalRelnOp(m_sChrSeq);

  return eRelnOp;
}

/*----------------------------------------------------------------------------*/
/* evaluate sub-query op                                                      */
/*----------------------------------------------------------------------------*/
eQO Symbol::eEvalSubQryOp(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  eQO eSubQryOp = eEvalSubQryOp(m_sChrSeq);

  return eSubQryOp;
}

/*----------------------------------------------------------------------------*/
/* true if symbol is given form                                               */
/*----------------------------------------------------------------------------*/
bool Symbol::bFormIs(eSF eSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (eSymbol == m_eSymbol)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get ordinal n  (i.e. $n or $In or $Kn or $c.n)                             */
/*----------------------------------------------------------------------------*/
i8 Symbol::iSF_ordinal(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  i8 i_Col = StrLib::iGleanInt(m_sChrSeq.azChar());

  return i_Col;
}

/*----------------------------------------------------------------------------*/
/* get table synonym from $c.n                                                */
/*----------------------------------------------------------------------------*/
char Symbol::cSF_tblSyn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  char acFirst  = m_sChrSeq.azChar()[0];
  char acSecond = m_sChrSeq.azChar()[1];
//  char acThird  = m_sChrSeq.azChar()[2];
//  char acFourth = m_sChrSeq.azChar()[3];

  if ('$' == acFirst && StrLib::bIsAlpha(acSecond))
    return acSecond;
  else
    return acFirst;
}

/*----------------------------------------------------------------------------*/
/* does symbol a symbolic value?                                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bHasSymbol(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (* m_sChrSeq.azChar())
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

  RASSERT(SF_TBL_SYN == m_eSymbol);

  char cTblSyn = cSF_char();

  vSymSprint("%c"_Stz, cTblSyn + 1);

  RASSERT(bIs_Tbl_Syn());
}

/*----------------------------------------------------------------------------*/
/* is this symbol an aggregate op?                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Agg_Fn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_AGG_FN != m_eSymbol)
    return false;

  if (bIs_Agg_Fn(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a synthetic as?  (i.e. __COLn)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_As(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_AS != m_eSymbol)
    return false;

  if (bIs_As(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a synthetic as?  (i.e. __COLn)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_As_Nm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_AS_NM != m_eSymbol)
    return false;

  if (bIs_As_Nm(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a synthetic as?  (i.e. __COLn)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_As_Nm_itm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_AS_NM_itm != m_eSymbol)
    return false;

  if (! Y_bHas(P_Itemised))
    return false;

  if (bIs_As_Nm(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a synthetic as?  (i.e. __COLn)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_As_Nm_syn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_AS_NM != m_eSymbol)
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

  if (SF_BOL_FN != m_eSymbol)
    return false;

  if (bIs_Bol_Fn(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a boolean literal?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Bol_Lit(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_BOL_LIT != m_eSymbol)
    return false;

  if (bIs_Bol_Lit(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a boolean operator?                                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Bol_Op(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_BOL_OP != m_eSymbol)
    return false;

  if (bIs_Bol_Op(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a bound?                                                    */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Bound(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_BOUND != m_eSymbol)
    return false;

  if (bIs_Bound(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a bound?                                                    */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Brace(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_BRACE != m_eSymbol)
    return false;

  if (bIs_Brace(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for CASE?                                                 */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Case(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_CASE != m_eSymbol)
    return false;

  if (bIs_Case(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for CAST?                                                 */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Cast(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_CAST != m_eSymbol)
    return false;

  if (bIs_Cast(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for CASTTYPE?                                             */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_CastType(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_CASTTYPE != m_eSymbol)
    return false;

  if (bIs_CastType(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a column name?                                                     */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Col_Nm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_COL_NM != m_eSymbol)
    return false;

  if (bIs_Col_Nm(m_sChrSeq))
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

  if (SF_COL_NO != m_eSymbol)
    return false;

  if (bIs_Col_No(m_sChrSeq)) {
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

  if (SF_COL_TC != m_eSymbol)
    return false;

  if (bIs_Col_Tc(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a table-column (i.e. $c.n)                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Col_Type(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_COL_TYPE != m_eSymbol)
    return false;

  if (bIs_Col_Type(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a comma separator?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Comma(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_COMMA != m_eSymbol)
    return false;

  if (bIs_Comma(m_sChrSeq))
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

  if (SF_CROSS != m_eSymbol)
    return false;

  if (bIs_Cross(m_sChrSeq))
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

  if (SF_DCM_LIT != m_eSymbol)
    return false;

  if (bIs_Dcm_Lit(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a sort direction?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Direction(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_DIRECTION != m_eSymbol)
    return false;

  if (bIs_Direction(m_sChrSeq))
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

  if (SF_ELSE != m_eSymbol)
    return false;

  if (bIs_Else(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol ENDIF?                                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_EndIf(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_ENDIF != m_eSymbol)
    return false;

  if (bIs_EndIf(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a sort direction?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Fld_Nm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_FLD_NM != m_eSymbol)
    return false;

  if (bIs_Fld_Nm(m_sChrSeq))
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

  if (SF_FLT_LIT != m_eSymbol)
    return false;

  if (bIs_Flt_Lit(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol GROUPBY ?                                                   */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_GroupBy(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_GROUPBY != m_eSymbol)
    return false;

  if (bIs_GroupBy(m_sChrSeq))
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

  if (SF_IDX_COL != m_eSymbol)
    return false;

  if (bIs_Idx_Col(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a index name?                                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Idx_Nm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_IDX_NM != m_eSymbol)
    return false;

  if (bIs_Idx_Nm(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a integer literal?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Int_Lit(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_INT_LIT != m_eSymbol)
    return false;

  if (bIs_Int_Lit(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol INTERVAL_DT ?                                               */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Interval_DT(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_INTERVAL_DT != m_eSymbol)
    return false;

  if (bIs_Interval_DT(m_sChrSeq))
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

  if (SF_INTERVAL_YM != m_eSymbol)
    return false;

  if (bIs_Interval_YM(m_sChrSeq))
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

  if (SF_IVL_FN != m_eSymbol)
    return false;

  if (bIs_Ivl_Fn(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a key symbol? (i.e. $Kn)                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Key_Col(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_KEY_COL != m_eSymbol)
    return false;

  if (bIs_Key_Col(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a math operator ?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Mth_Op(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_MTH_OP != m_eSymbol)
    return false;

  if (bIs_Mth_Op(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol NEGATE ?                                                    */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Negate(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_NEGATE != m_eSymbol)
    return false;

  if (bIs_Negate(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a math operator ?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_None(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_NONE != m_eSymbol)
    return false;

  if (bIs_None(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a note                                                             */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Note(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_NOTE != m_eSymbol)
    return false;

  if (bIs_Note(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol NULL ?                                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Null(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_NULL != m_eSymbol)
    return false;

  if (bIs_Null(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a parenthesis?                                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Parenthesis(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_PARENTHESIS != m_eSymbol)
    return false;

  if (bIs_Parenthesis(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a index name?                                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Ppe_Nm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_PPE_NM != m_eSymbol)
    return false;

  if (bIs_Ppe_Nm(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a parameter column?                                                */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Prm_Col(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_PRM_COL != m_eSymbol)
    return false;

  if (bIs_Prm_Col(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a relational-operator?                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Rln_Op(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_RLN_OP != m_eSymbol)
    return false;

  if (bIs_Rln_Op(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a correlation-parameter?  (i.e. %n)                   */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Prm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Sqy_Prm(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a subQuery-operator?                                  */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Nm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_SQY_NM != m_eSymbol)
    return false;

  if (bIs_Sqy_Nm(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a subQuery-operator?                                  */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Op(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_SQY_OP != m_eSymbol)
    return false;

  if (bIs_Sqy_Op(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a subQuery-column?  (i.e. __n)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Rsl(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_SQY_RSL != m_eSymbol)
    return false;

  if (bIs_Sqy_Rsl(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a bound?                                                    */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Stg_Cast(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_STG_CAST != m_eSymbol)
    return false;

  if (bIs_Stg_Cast(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a subQuery-column?  (i.e. __n)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Stg_Nm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_STG_NO != m_eSymbol)
    return false;

  if (bIs_Stg_Nm(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a string function?                                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Str_Fn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_STR_FN != m_eSymbol)
    return false;

  if (bIs_Str_Fn(m_sChrSeq))
    return true;

  return false;
}
/*----------------------------------------------------------------------------*/
/* is this a symbol for a subQuery-column?  (i.e. __n)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_String(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_STRING != m_eSymbol)
    return false;

  if (bIs_String(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a table name?                                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Tbl_Nm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_TBL_NM != m_eSymbol)
    return false;

  if (bIs_Tbl_Nm(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol a table synonym?  (i.e. c)                                */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Tbl_Syn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_TBL_SYN != m_eSymbol)
    return false;

  if (bIs_Tbl_Syn(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol a table synonym?  (i.e. c)                                */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Tbl_Syn_rep(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_TBL_SYN_rep != m_eSymbol)
    return false;

  if (bIs_Tbl_Syn_rep(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol THEN?                                                       */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Then(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_THEN != m_eSymbol)
    return false;

  if (bIs_Then(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol TYPE?                                                       */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Type(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_TYPE != m_eSymbol)
    return false;

  if (bIs_Type(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a parenthesis?                                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_When(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_WHEN != m_eSymbol)
    return false;

  if (bIs_When(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a cross-refernece?  (i.e. #n)                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Xk_Cast(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_XK_CAST != m_eSymbol)
    return false;

  if (bIs_Xk_Cast(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a cross-refernece?  (i.e. #n)                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Xrf_No(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_XRF_NO != m_eSymbol)
    return false;

  if (bIs_Xrf_No(m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* strip itemisation                                                          */
/*----------------------------------------------------------------------------*/
void Symbol::vItemStrip(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (! bIsItemised(m_sChrSeq))
    return;

  vItemStrip(m_sChrSeq);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bNeedsBrace(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

       if (m_sChrSeq.bSame("between"_Stz))        return true;
  else if (m_sChrSeq.bSame("case"_Stz))           return true;
  else if (m_sChrSeq.bSame("in"_Stz))             return true;
  else if (m_sChrSeq.bSame("matchLeading"_Stz))   return true;
  else if (m_sChrSeq.bSame("matchMiddle"_Stz))    return true;
  else if (m_sChrSeq.bSame("matchMiddle1"_Stz))   return true;
  else if (m_sChrSeq.bSame("matchTrailing"_Stz))  return true;
//  else if (m_sChrSeq.bSame("SUBSTRING2"_Stz))     return true;
//  else if (m_sChrSeq.bSame("SUBSTRING3"_Stz))     return true;

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

  if (*m_sChrSeq.azChar() == cArg)
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

  if (m_sChrSeq.bSame(szArg))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bSame(Symbol* pArg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (m_sChrSeq.bSame(pArg->m_sChrSeq))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get symbol int                                                             */
/*----------------------------------------------------------------------------*/
i8 Symbol::iSF_number(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  i8 i = m_sChrSeq.iToI8();

  return i;
}

/*----------------------------------------------------------------------------*/
/* get symbol stz                                                             */
/*----------------------------------------------------------------------------*/
Stz* Symbol::szSF_symbol(eSF eSymbol_xchk)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_NONE != eSymbol_xchk)
    RASSERT(eSymbol_xchk = m_eSymbol);

  return m_sChrSeq.szStz();
}

/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
char Symbol::cSF_char(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  return * m_sChrSeq.azChar();
}

/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSym(eSF eSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  switch (eSymbol) {

    case SF_TBL_SYN_rep: vSet_Tbl_Syn_rep(); break;

    default: NEVER_REACH;
  }

}

/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSym(eSF eSymbol, char cChr)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  switch (eSymbol) {

    case SF_TBL_SYN: vSet_Tbl_Syn(cChr); break;

    default: NEVER_REACH;
  }

}

/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSym(eSF eSymbol, char cChr, i8 iInt)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  switch (eSymbol) {

    case SF_COL_TC:    vSet_Col_Tc(cChr,iInt);    break;

    default: NEVER_REACH;
  }

}

/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSym(eSF eSymbol, i8 iInt)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  switch (eSymbol) {

    case SF_AS_NM_itm:  vSet_As_Nm_itm(iInt);  break;
    case SF_AS_NM_syn:  vSet_As_Nm_syn(iInt);  break;
    case SF_COL_NO:     vSet_Col_No(iInt);     break;
    case SF_COL_NM_dis: vSet_Col_Nm_dis(iInt); break;
    case SF_IDX_COL:    vSet_Idx_Col(iInt);    break;
    case SF_INT_LIT:    vSet_Int_Lit(iInt);    break;
    case SF_KEY_COL:    vSet_Key_Col(iInt);    break;
    case SF_SQY_NM:     vSet_Sqy_Nm(iInt);     break;
    case SF_STG_NO:     vSet_Stg_Nm(iInt);     break;
    case SF_PRM_COL:    vSet_Prm_Col(iInt);    break;
    case SF_XRF_NO:     vSet_Xrf_No(iInt);     break;

    default: NEVER_REACH;
  }

}

/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSym(eSF eSymbol, i8 iInt, Symbol* pSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  switch (eSymbol) {

    case SF_COL_NM_dis: vSet_Col_Nm_dis(iInt,pSymbol); break;

    default: NEVER_REACH;
  }

}

/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSym(eSF eSymbol, Stz* szChrSeq)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  switch (eSymbol) {

    case SF_AGG_FN:      vSet_Agg_Fn(szChrSeq);      break;
    case SF_AS:          vSet_As(szChrSeq);          break;
    case SF_AS_NM:       vSet_As_Nm(szChrSeq);       break;
    case SF_BOL_FN:      vSet_Bol_Fn(szChrSeq);      break;
    case SF_BOL_LIT:     vSet_Bol_Lit(szChrSeq);     break;
    case SF_BOL_OP:      vSet_Bol_Op(szChrSeq);      break;
    case SF_BOUND:       vSet_Bound(szChrSeq);       break;
    case SF_BRACE:       vSet_Brace(szChrSeq);       break;
    case SF_CASE:        vSet_Case(szChrSeq);        break;
    case SF_CAST:        vSet_Cast(szChrSeq);        break;
    case SF_CASTTYPE:    vSet_CastType(szChrSeq);    break;
    case SF_COL_NM:      vSet_Col_Nm(szChrSeq);      break;
    case SF_COL_NM_syn:  vSet_Col_Nm_syn(szChrSeq);  break;
    case SF_COL_NO:      vSet_Col_No(szChrSeq);      break;
    case SF_COL_TC:      vSet_Col_Tc(szChrSeq);      break;
    case SF_COL_TYPE:    vSet_Col_Type(szChrSeq);    break;
    case SF_COMMA:       vSet_Comma(szChrSeq);       break;
    case SF_CROSS:       vSet_Cross(szChrSeq);       break;
    case SF_DCM_LIT:     vSet_Dcm_Lit(szChrSeq);     break;
    case SF_DIRECTION:   vSet_Direction(szChrSeq);   break;
    case SF_ELSE:        vSet_Else(szChrSeq);        break;
    case SF_ENDIF:       vSet_EndIf(szChrSeq);       break;
    case SF_FLD_NM:      vSet_Fld_Nm(szChrSeq);      break;
    case SF_FLT_LIT:     vSet_Flt_Lit(szChrSeq);     break;
    case SF_GROUPBY:     vSet_GroupBy(szChrSeq);     break;
    case SF_IDX_COL:     vSet_Idx_Col(szChrSeq);     break;
    case SF_IDX_NM:      vSet_Idx_Nm(szChrSeq);      break;
    case SF_INT_LIT:     vSet_Int_Lit(szChrSeq);     break;
    case SF_IVL_FN:      vSet_Ivl_Fn(szChrSeq);      break;
    case SF_INTERVAL_DT: vSet_Interval_DT(szChrSeq); break;
    case SF_INTERVAL_YM: vSet_Interval_YM(szChrSeq); break;
    case SF_KEY_COL:     vSet_Key_Col(szChrSeq);     break;
    case SF_NEGATE:      vSet_Negate(szChrSeq);      break;
    case SF_NOTE:        vSet_Note(szChrSeq);        break;
    case SF_NULL:        vSet_Null(szChrSeq);        break;
    case SF_MTH_OP:      vSet_Mth_Op(szChrSeq);      break;
    case SF_PARENTHESIS: vSet_Parenthesis(szChrSeq); break;
    case SF_PPE_NM:      vSet_Ppe_Nm(szChrSeq);      break;
    case SF_PRM_COL:     vSet_Prm_Col(szChrSeq);     break;
    case SF_RLN_OP:      vSet_Rln_Op(szChrSeq);      break;
    case SF_SQY_OP:      vSet_Sqy_Op(szChrSeq);      break;
    case SF_STG_CAST:    vSet_Stg_Cast(szChrSeq);    break;
    case SF_STR_FN:      vSet_Str_Fn(szChrSeq);      break;
    case SF_STRING:      vSet_String(szChrSeq);      break;
    case SF_TBL_NM:      vSet_Tbl_Nm(szChrSeq);      break;
    case SF_TBL_SYN:     vSet_Tbl_Syn(szChrSeq);     break;
    case SF_THEN:        vSet_Then(szChrSeq);        break;
    case SF_TYPE:        vSet_Type(szChrSeq);        break;
    case SF_WHEN:        vSet_When(szChrSeq);        break;
    case SF_XK_CAST:     vSet_Xk_Cast(szChrSeq);     break;
    case SF_XRF_NO:      vSet_Xrf_No(szChrSeq);      break;

    default: NEVER_REACH;
  }

}

/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSym(Symbol* pSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (SF_COL_NO == pSymbol->m_eSymbol) {
    vSet_Col_No(pSymbol->m_sChrSeq);
  }
  else {
    m_sChrSeq = pSymbol->m_sChrSeq;
    m_eSymbol = pSymbol->m_eSymbol;
  }

  if (SF_COL_NO == m_eSymbol)
    RASSERT(Symbol::bIs_Col_No(m_sChrSeq));

  if (SF_COL_TC == m_eSymbol)
    RASSERT(Symbol::bIs_Col_Tc(m_sChrSeq));

}


/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSymPpy(Symbol* pSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sChrSeq = pSymbol->m_sChrSeq;
  m_eSymbol = pSymbol->m_eSymbol;

  if (SF_COL_NO == m_eSymbol)
    RASSERT(Symbol::bIs_Col_No(m_sChrSeq));

  if (SF_COL_TC == m_eSymbol)
    RASSERT(Symbol::bIs_Col_Tc(m_sChrSeq));

  Y_vAll(pSymbol);
}


/*----------------------------------------------------------------------------*/
/* validate symbol properties                                                 */
/*----------------------------------------------------------------------------*/
bool Symbol::bTest_mustHave_ppy(Stz* szPpy, ...)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  bool bMustHave = true;

  va_list xArgs;

  va_start(xArgs, szPpy);
  Stz* szPpy_mustHave = szPpy;
  while (nullptr != szPpy_mustHave) {

    bMustHave = Y_bHas(szPpy_mustHave);

    if (! bMustHave)
      break;

    szPpy_mustHave = (Stz*)va_arg(xArgs, Stz*);
  }

  va_end(xArgs);

  return bMustHave;
}

/*----------------------------------------------------------------------------*/
/* validate symbol                                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bValidate(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  bool b = false;

  switch (m_eSymbol) {
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
    case SF_SQY_OP:      b = bIs_Sqy_Op();      break;
    case SF_STG_NO:      b = bIs_Stg_Nm();      break;
    case SF_STG_CAST:    b = bIs_Stg_Cast();    break;
    case SF_STR_FN:      b = bIs_Str_Fn();      break;
    case SF_STRING:      b = bIs_String();      break;
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
void Symbol::vSet_Agg_Fn(Stz* szAggFn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szAggFn;
  m_eSymbol = SF_AGG_FN;

  RASSERT(bIs_Agg_Fn());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given as                                                */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_As(Stz* szAs)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szAs;
  m_eSymbol = SF_AS;

  RASSERT(bIs_As());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given as-name                                           */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_As_Nm(Stz* szAsNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol || SF_AS_NM == m_eSymbol);

  m_sChrSeq = szAsNm;
  m_eSymbol = SF_AS_NM;
  Y_vCut(P_Synthetic);

  RASSERT(bIs_As_Nm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given synthetic-as no                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_As_Nm_itm(i8 iAsNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol || SF_AS_NM == m_eSymbol);

  vSymSprint("C%lld_%s"_Stz, iAsNo, m_sChrSeq.szStz());
  m_eSymbol = SF_AS_NM_itm;
  Y_vSet(P_Itemised);

  RASSERT(bIs_As_Nm_itm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given synthetic-as no                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_As_Nm_syn(i8 iAsNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  vSymSprint("COL%lld"_Stz, iAsNo);
  m_eSymbol = SF_AS_NM;
  Y_vSet(P_Synthetic);

  RASSERT(bIs_As_Nm_syn());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given boolean fn                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Bol_Fn(Stz* szBolFn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szBolFn;
  m_eSymbol = SF_BOL_FN;

  RASSERT(bIs_Bol_Fn());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given boolean literal                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Bol_Lit(Stz* szBolLit)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szBolLit;
  m_eSymbol = SF_BOL_LIT;

  RASSERT(bIs_Bol_Lit());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given boolean op                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Bol_Op(Stz* szBolOp)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szBolOp;
  m_eSymbol = SF_BOL_OP;

  RASSERT(bIs_Bol_Op());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given bound                                             */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Bound(Stz* szBound)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szBound;
  m_eSymbol = SF_BOUND;

  RASSERT(bIs_Bound());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given brace                                             */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Brace(Stz* szBrace)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szBrace;
  m_eSymbol = SF_BRACE;

  RASSERT(bIs_Brace());
}

/*----------------------------------------------------------------------------*/
/* set this symbol CASE                                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Case(Stz* szCase)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szCase;
  m_eSymbol = SF_CASE;

  RASSERT(bIs_Case());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given as                                                */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Cast(Stz* szCast)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szCast;
  m_eSymbol = SF_CAST;

  RASSERT(bIs_Cast());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given as                                                */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_CastType(Stz* szCastType)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szCastType;
  m_eSymbol = SF_CASTTYPE;

  RASSERT(bIs_CastType());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given column-name                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Col_Nm(Stz* szColNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol || SF_COL_NO == m_eSymbol);

  m_sChrSeq = szColNm;
  m_eSymbol = SF_COL_NM;

  Y_vCut(P_DisamCol);

  RASSERT(bIs_Col_Nm());
  RASSERT(! bIs_Col_Nm_dis());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given disambiguated-column-name                         */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Col_Nm_dis(i8 i_Child)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol || SF_COL_NM == m_eSymbol);

  vSymSprint("C%lld_%s"_Stz, i_Child, m_sChrSeq.szStz());
  m_eSymbol = SF_COL_NM;

  Y_vSet(P_DisamCol);

  RASSERT(bIs_Col_Nm_dis());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given disambiguated-column-name                         */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Col_Nm_dis(i8 i_Child, Symbol* pSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  vSymSprint("C%lld_%s"_Stz, i_Child, pSymbol->m_sChrSeq.szStz());
  m_eSymbol = SF_COL_NM;

  Y_vSet(P_DisamCol);

  RASSERT(bIs_Col_Nm_dis());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given disambiguated-column-name                         */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Col_Nm_syn(Stz* szColNm_syn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szColNm_syn;
  m_eSymbol = SF_COL_NM;

  Y_vSet(P_Synthetic);

  RASSERT(bIs_Col_Nm_syn());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given column number                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Col_No(i8 iStmColOdn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol || SF_COL_NO == m_eSymbol || SF_KEY_COL == m_eSymbol);

  if (bIs_Col_Tc()) {
    Y_vSet(P_StmCol, szSF_symbol(SF_COL_TC));
    Y_vSet(P_WasStmCol, szSF_symbol(SF_COL_TC));
  }
  else
  if (bIs_Key_Col()) {
    Y_vSet(P_WasKeyCol, szSF_symbol(SF_KEY_COL));
  }

  vSymSprint("$%lld"_Stz, iStmColOdn);
  m_eSymbol = SF_COL_NO;

  RASSERT(bIs_Col_No());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given column nuber                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Col_No(Stz* szColNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol   || SF_COL_NO == m_eSymbol  ||
          SF_COL_TC == m_eSymbol ||

          SF_KEY_COL == m_eSymbol ||  // todo: these need to be investigated
          SF_PARENTHESIS == m_eSymbol ||
          SF_PRM_COL == m_eSymbol ||
          SF_XRF_NO == m_eSymbol
          );

  if (bIs_Col_Tc()) {
    Y_vSet(P_StmCol, szSF_symbol(SF_COL_TC));
    Y_vSet(P_WasStmCol, szSF_symbol(SF_COL_TC));
  }
  m_sChrSeq = szColNo;
  m_eSymbol = SF_COL_NO;

  RASSERT(bIs_Col_No());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-column                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Col_Tc(char cTblSyn, i8 iTblColOdn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  vSymSprint("$%c.%lld"_Stz, cTblSyn, iTblColOdn);
  m_eSymbol = SF_COL_TC;

  RASSERT(bIs_Col_Tc());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-column                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Col_Tc(Stz* szColTc)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol || SF_COL_NM == m_eSymbol || SF_COL_TC == m_eSymbol);

  m_sChrSeq = szColTc;
  m_eSymbol = SF_COL_TC;

  RASSERT(bIs_Col_Tc());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given comma                                             */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Comma(Stz* szComma)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szComma;
  m_eSymbol = SF_COMMA;

  RASSERT(bIs_Comma());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given cross                                             */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Cross(Stz* szCross)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szCross;
  m_eSymbol = SF_CROSS;

  RASSERT(bIs_Cross());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given type                                              */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Col_Type(Stz* szColType)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szColType;
  m_eSymbol = SF_COL_TYPE;

  RASSERT(bIs_Col_Type());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given decimal literal                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Dcm_Lit(Stz* szDcmLit)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szDcmLit;
  m_eSymbol = SF_DCM_LIT;

  RASSERT(bIs_Dcm_Lit());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given direction                                         */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Direction(Stz* szColNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szColNm;
  m_eSymbol = SF_DIRECTION;

  RASSERT(bIs_Direction());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to ELSE                                                    */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Else(Stz* szElse)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szElse;
  m_eSymbol = SF_ELSE;

  RASSERT(bIs_Else());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to ENDIF                                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_EndIf(Stz* szEndIf)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szEndIf;
  m_eSymbol = SF_ENDIF;

  RASSERT(bIs_EndIf());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given fold name                                         */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Fld_Nm(Stz* szFldNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szFldNm;
  m_eSymbol = SF_FLD_NM;

  RASSERT(bIs_Fld_Nm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to be a float literal                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Flt_Lit(Stz* szFltLit)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szFltLit;
  m_eSymbol = SF_FLT_LIT;

  RASSERT(bIs_Flt_Lit());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given group-by                                          */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_GroupBy(Stz* szGroupBy)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szGroupBy;
  m_eSymbol = SF_GROUPBY;

  RASSERT(bIs_GroupBy());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given index-column                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Idx_Col(i8 iIdxColOdn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  vSymSprint("$I%lld"_Stz, iIdxColOdn);
  m_eSymbol = SF_IDX_COL;

  RASSERT(bIs_Idx_Col());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given index-column                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Idx_Col(Stz* szIdxCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szIdxCol;
  m_eSymbol = SF_IDX_COL;

  RASSERT(bIs_Idx_Col());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to a index name                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Idx_Nm(Stz* szIdxNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szIdxNm;
  m_eSymbol = SF_IDX_NM;

  RASSERT(bIs_Idx_Nm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given integer literal                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Int_Lit(i8 iIIntLIt)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = iIIntLIt;
  m_eSymbol = SF_INT_LIT;

  RASSERT(bIs_Int_Lit());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to be a integer literal                                    */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Int_Lit(Stz* szIntLit)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szIntLit;
  m_eSymbol = SF_INT_LIT;

  RASSERT(bIs_Int_Lit());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given INTERVAL_DT                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Interval_DT(Stz* szInterval_DT)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szInterval_DT;
  m_eSymbol = SF_INTERVAL_DT;

  RASSERT(bIs_Interval_DT());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given INTERVAL_YM                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Interval_YM(Stz* szInterval_YM)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szInterval_YM;
  m_eSymbol = SF_INTERVAL_YM;

  RASSERT(bIs_Interval_YM());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given interval fn                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Ivl_Fn(Stz* szIvlFn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szIvlFn;
  m_eSymbol = SF_IVL_FN;

  RASSERT(bIs_Ivl_Fn());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given key-column                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Key_Col(i8 iKeyOdn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol || SF_KEY_COL == m_eSymbol);

  vSymSprint("$K%lld"_Stz, iKeyOdn);
  m_eSymbol = SF_KEY_COL;

  RASSERT(bIs_Key_Col());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given key-column                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Key_Col(Stz* szKeyCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szKeyCol;
  m_eSymbol = SF_KEY_COL;

  RASSERT(bIs_Key_Col());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given math op                                           */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Mth_Op(Stz* szMthOp)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szMthOp;
  m_eSymbol = SF_MTH_OP;

  RASSERT(bIs_Mth_Op());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to NEGATE                                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Negate(Stz* szNull)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szNull;
  m_eSymbol = SF_NEGATE;

  RASSERT(bIs_Negate());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given note                                              */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Note(Stz* szNote)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szNote;
  m_eSymbol = SF_NOTE;

  RASSERT(bIs_Note());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to NULL                                                    */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Null(Stz* szNull)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szNull;
  m_eSymbol = SF_NULL;

  RASSERT(bIs_Null());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given parenthesis                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Parenthesis(Stz* szParenthesis)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szParenthesis;
  m_eSymbol = SF_PARENTHESIS;

  RASSERT(bIs_Parenthesis());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given pipeline name                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Ppe_Nm(Stz* szPpeNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szPpeNm;
  m_eSymbol = SF_PPE_NM;

  RASSERT(bIs_Ppe_Nm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given parameter column                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Prm_Col(i8 iOrdinal)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  vSymSprint("%%%lld"_Stz, iOrdinal);
  m_eSymbol = SF_PRM_COL;

  RASSERT(bIs_Sqy_Prm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given parameter column                                  */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Prm_Col(Stz* szPrmCol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szPrmCol;
  m_eSymbol = SF_PRM_COL;

  RASSERT(bIs_Sqy_Prm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given relational op                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Rln_Op(Stz* szRlnOp)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol || SF_SQY_OP == m_eSymbol);

  m_sChrSeq = szRlnOp;
  m_eSymbol = SF_RLN_OP;

  RASSERT(bIs_Rln_Op());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given subquery-number                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Sqy_Nm(i8 iSqyNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  vSymSprint("__sq%02lld"_Stz, iSqyNo);
  m_eSymbol = SF_SQY_NM;

  RASSERT(bIs_Sqy_Nm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given subquery op                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Sqy_Op(Stz* szSqyOp)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol || SF_SQY_OP == m_eSymbol);

  m_sChrSeq = szSqyOp;
  m_eSymbol = SF_SQY_OP;

  RASSERT(bIs_Sqy_Op());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given stage cast                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Stg_Cast(Stz* szStgCast)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol || SF_STG_CAST == m_eSymbol);

  m_sChrSeq = szStgCast;
  m_eSymbol = SF_STG_CAST;

  RASSERT(bIs_Stg_Cast());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to iven stage name                                         */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Stg_Nm(i8 iStgNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  vSymSprint("S%02lld"_Stz, iStgNo);
  m_eSymbol = SF_STG_NO;

  RASSERT(bIs_Stg_Nm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given string fn                                         */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Str_Fn(Stz* szBolFn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szBolFn;
  m_eSymbol = SF_STR_FN;

  RASSERT(bIs_Str_Fn());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given string                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_String(Stz* szString)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szString;
  m_eSymbol = SF_STRING;

  RASSERT(bIs_String());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table name                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Tbl_Nm(Stz* szTblNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szTblNm;
  m_eSymbol = SF_TBL_NM;

  RASSERT(bIs_Tbl_Nm());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-synonym                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Tbl_Syn(char cTblSyn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  vSymSprint("%c"_Stz, cTblSyn);
  m_eSymbol = SF_TBL_SYN;

  RASSERT(bIs_Tbl_Syn());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-synonym                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Tbl_Syn(Stz* szTblSyn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szTblSyn;
  m_eSymbol = SF_TBL_SYN;

  RASSERT(bIs_Tbl_Syn());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given ttable-synonym replicated                         */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Tbl_Syn_rep(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol || SF_TBL_SYN == m_eSymbol);

  char cTableSyn_actual = cSF_char();
  cTableSyn_actual = StrLib::cToUpper(cTableSyn_actual);

  vSymSprint("%c"_Stz, cTableSyn_actual);
  m_eSymbol = SF_TBL_SYN_rep;

  RASSERT(bIs_Tbl_Syn_rep());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to THEN                                                    */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Then(Stz* szThen)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szThen;
  m_eSymbol = SF_THEN;

  RASSERT(bIs_Then());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to TYPE                                                    */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Type(Stz* szType)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szType;
  m_eSymbol = SF_TYPE;

  RASSERT(bIs_Type());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given when                                              */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_When(Stz* szWhen)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szWhen;
  m_eSymbol = SF_WHEN;

  RASSERT(bIs_When());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given xk cast                                           */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Xk_Cast(Stz* szXkCast)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szXkCast;
  m_eSymbol = SF_XK_CAST;

  RASSERT(bIs_Xk_Cast());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given cross-reference no                                */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Xrf_No(i8 iXrefNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  vSymSprint("#%lld"_Stz, iXrefNo);
  m_eSymbol = SF_XRF_NO;

  RASSERT(bIs_Xrf_No());
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given cross-reference no                                */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Xrf_No(Stz* szXrefNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  RASSERT(SF_NONE == m_eSymbol);

  m_sChrSeq = szXrefNo;
  m_eSymbol = SF_XRF_NO;

  RASSERT(bIs_Xrf_No());
}

/*----------------------------------------------------------------------------*/
/* sprint symbol                                                              */
/*----------------------------------------------------------------------------*/
void Symbol::vSymSprint(Stz* szFormat, ...)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  s256 sSymbol;

  va_list xArgs;
  va_start(xArgs,szFormat);
  StrLib::vSprint(sSymbol.azChar(), sSymbol.nAreaBytes(), xArgs, (char*)szFormat);
  va_end(xArgs);

  m_sChrSeq = sSymbol;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/