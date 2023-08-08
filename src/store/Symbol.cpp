/*============================================================================*/
/* Symbol.cpp -- Symbolic Value                                               */
/*============================================================================*/

#include "Card.hpp"
#include "CardCtl.hpp"
#include "Board.hpp"
#include "CardIter.hpp"

#include "Card_Index.hpp"  // TODO: remove
#include "Index.hpp"       // TODO: remove

#define X_SET_CROSS_CHECK

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

  switch (eSymbol) {

    case SF_COL_NO  : vSet_Col_No(iInt);   break;
    case SF_IDX_COL : vSet_Idx_Col(iInt);  break;
    case SF_KEY_COL : vSet_Key_Col(iInt);  break;
    case SF_NUMBER  : vSet_Number(iInt);   break;
    case SF_PRM_COL : vSet_Prm_Col(iInt);  break;
    case SF_SQY_NO  : vSet_Sqy_No(iInt);   break;

    default: NEVER_REACH;
  }

  _postcondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Symbol::Symbol(eSF eSymbol, Stz* szStz)
    : Object(CN_SYMBOL)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szStz;
  m_eSymbol = eSymbol;
  m_pProperties = nullptr;

  _checkInitialised_(CN_SYMBOL);

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
eQO Symbol::eEvalSubQryOp(Stz* szSymbol)
{
  _static_

       if (szSymbol->bSame("exists"_Stz))    return QO_EXISTS_OP;
  else if (szSymbol->bSame("notExists"_Stz)) return QO_NOT_EXISTS_OP;
  else if (szSymbol->bSame("in"_Stz))        return QO_IN_OP;
  else if (szSymbol->bSame("notIn"_Stz))     return QO_NOT_IN_OP;

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
eSF Symbol::eEvalSymbol(Stz* szSymbol)
{
  _static_

  /*
  enum eSF // symbol form
  {
    SF_NONE = 0,   // none

    SF_AS_NM,
    SF_COL_NM_dis,
    SF_IDX_COL,
    SF_KEY_COL,
    SF_NUMBER,

    SF_OTHER,

    SF_PRM_COL,
    SF_AS_NM_syn,
    SF_SQY_NO,
    SF_COL_TC,
    SF_TBLCOL,
    SF_TBL_SYN,
    SF_XRF_NO,

    SF_ENUMS
  };
  */

  if (bIs_Agg_Fn(szSymbol))              return SF_AGG_FN;
  if (bIs_Col_Tc(szSymbol))      return SF_COL_TC;
  if (bIs_Xrf_No(szSymbol))           return SF_XRF_NO;
  if (bIs_Direction(szSymbol))          return SF_DIRECT;
  if (bIs_Col_Nm_dis(szSymbol))        return SF_COL_NM_dis;
  if (bIs_Idx_Col(szSymbol))        return SF_IDX_COL;
  if (bIs_Number(szSymbol))             return SF_NUMBER;
  if (bIs_Key_Col(szSymbol))         return SF_KEY_COL;
  if (bIs_Sqy_Prm(szSymbol))   return SF_PRM_COL;
  if (bIs_Col_No(szSymbol))      return SF_COL_NO;
  if (bIs_Sqy_Crl_Key(szSymbol))       return SF_SQY_KEY;
  if (bIs_Sqy_Rsl(szSymbol))     return SF_SQY_NO;
  if (bIs_Col_Nm_syn(szSymbol))        return SF_AS_NM_syn;
  if (bIs_Tbl_Syn(szSymbol))           return SF_TBL_SYN;

  if (StrLib::bIsSqlName(szSymbol->azChar()))  return SF_AS_NM;

  return SF_OTHER;
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-synonym                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vInit_Tbl_Syn(char cTblSyn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSymSprint("%c"_Stz, cTblSyn);
  m_eSymbol = SF_TBL_SYN;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Tbl_Syn());
#endif
}



/*----------------------------------------------------------------------------*/
/* is this symbol an aggregate op?                                            */
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
/* is this symbol a boolean function?                                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Bol_Fn(Stz* szArg)
{
  _static_

       if (szArg->bSame("between"_Stz))         return true;
  else if (szArg->bSame("inList"_Stz))          return true;
  else if (szArg->bSame("isNull"_Stz))          return true;
  else if (szArg->bSame("like2"_Stz))           return true;
  else if (szArg->bSame("like3"_Stz))           return true;
  else if (szArg->bSame("matchLeading"_Stz))    return true;
  else if (szArg->bSame("matchMiddle"_Stz))     return true;
  else if (szArg->bSame("matchMiddle1"_Stz))    return true;
  else if (szArg->bSame("matchTrailing"_Stz))   return true;

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
/* is this a column name?                                                     */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Col_Nm(Stz* szArg)
{
  _static_

  if (! StrLib::bIsSqlName(szArg->azChar()))
    return false;

  if (bIs_Agg_Fn(szArg))       return false;
  if (bIs_Bol_Fn(szArg))      return false;
  if (bIs_Bol_Op(szArg))      return false;
  if (bIs_Bound(szArg))       return false;
  if (bIs_Direction(szArg))   return false;
  if (bIs_Sqy_Op(szArg))  return false;

  return true;
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
/* is this a symbol for a disambiguated as?  (i.e. Cn_Xyz)                    */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Col_Nm_dis(Stz* szArg)
{
  _static_

  char* acCursor_arg = szArg->azChar();

  if ('C' != *acCursor_arg++)
    return false;

  i8 nDigits = 0;
  while (StrLib::bIsDigit(*acCursor_arg++))
    nDigits++;
  if (! nDigits)
    return false;

  if ('_' != *acCursor_arg++)
    return false;

  if (StrLib::bIsSqlName(acCursor_arg))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a synthetic as?  (i.e. __COLn)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Col_Nm_syn(Stz* szArg)
{
  _static_

  char acFirst  = szArg->azChar()[0];
  char acSecond = szArg->azChar()[1];
  char acThird  = szArg->azChar()[2];
  char acFourth = szArg->azChar()[3];
  char acFifth  = szArg->azChar()[4];
  char acSixth  = szArg->azChar()[5];

  if (   '_' == acFirst
         && '_' == acSecond
         && 'C' == acThird
         && 'O' == acFourth
         && 'L' == acFifth
         && StrLib::bIsDigit(acSixth))
    return true;

  if (   'C' == acFirst
         && 'O' == acSecond
         && 'L' == acThird
         && StrLib::bIsDigit(acFourth))
    return true;

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

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a number?                                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Number(Stz* szArg)
{
  _static_

  if (StrLib::bIsDecimal(szArg->azChar()))  return true;
  if (StrLib::bIsFloat(szArg->azChar()))    return true;
  if (StrLib::bIsInteger(szArg->azChar()))  return true;

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
/* is value a __subQuery like name?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Crl_Key(BASE::Stz *szStz)
{
  _static_

  if (StrLib::bBeginsWith(szStz->azChar(), "_SQCRLKEY_"))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is value a __subQuery like name?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Nm(BASE::Stz *szStz)
{
  _static_

  if (StrLib::bBeginsWith(szStz->azChar(), "__subquery") ||
      StrLib::bBeginsWith(szStz->azChar(), "__sq"))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is value a __subQuery like name?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_No(BASE::Stz *szStz)
{
  _static_

  if (StrLib::bBeginsWith(szStz->azChar(), "__subquery") ||
      StrLib::bBeginsWith(szStz->azChar(), "__sq"))
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
/* is value a __subQuery like name?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Stg_Nm(BASE::Stz *szStz)
{
  _static_

  char acFirst  = szStz->azChar()[0];
  char acSecond = szStz->azChar()[1];

  if ('S' == acFirst
      && StrLib::bIsDigit(acSecond))
    return true;

  return false;
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
/* strip disambiguation                                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vStripDisamb(String* psArg)
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
eCT Symbol::Y_eColType(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  Stz* szType = Y_szGet(P_Type);
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
/* evaluate relational op                                                     */
/*----------------------------------------------------------------------------*/
eRO Symbol::eEvalRelnOp(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  eRO eRelnOp = eEvalRelnOp(m_sSymbol);

  return eRelnOp;
}

/*----------------------------------------------------------------------------*/
/* evaluate sub-query op                                                      */
/*----------------------------------------------------------------------------*/
eQO Symbol::eEvalSubQryOp(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  eQO eSubQryOp = eEvalSubQryOp(m_sSymbol);

  return eSubQryOp;
}

/*----------------------------------------------------------------------------*/
/* get ordinal n  (i.e. $n or $In or $Kn or $c.n)                             */
/*----------------------------------------------------------------------------*/
i8 Symbol::iSF_ordinal(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  i8 i_Col = StrLib::iGleanInt(m_sSymbol.azChar());

  return i_Col;
}

/*----------------------------------------------------------------------------*/
/* get table synonym from $c.n                                                */
/*----------------------------------------------------------------------------*/
char Symbol::cSF_tblSyn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  char acFirst  = m_sSymbol.azChar()[0];
  char acSecond = m_sSymbol.azChar()[1];
  char acThird  = m_sSymbol.azChar()[2];
  char acFourth = m_sSymbol.azChar()[3];

  RASSERT('$' == acFirst);
  RASSERT(StrLib::bIsAlpha(acSecond));
  RASSERT('.' == acThird);
  RASSERT(StrLib::bIsDigit(acFourth));

  return acSecond;
}

/*----------------------------------------------------------------------------*/
/* does symbol a symbolic value?                                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bHasSymbol(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (* m_sSymbol.azChar())
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

  char cTblSyn = cSF_char();

  vSet_Tbl_Syn(cTblSyn + 1);
}

/*----------------------------------------------------------------------------*/
/* is this symbol an aggregate op?                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Agg_Fn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Agg_Fn(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a synthetic as?  (i.e. __COLn)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_As_Nm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Col_Nm(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a synthetic as?  (i.e. __COLn)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_As_Nm_syn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Col_Nm_syn(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a boolean function?                                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Bol_Fn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Bol_Fn(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a boolean operator?                                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Bol_Op(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Bol_Op(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a bound?                                                    */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Bound(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Bound(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a column name?                                                     */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Col_Nm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Col_Nm(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a comma separator?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Col_Nm_dis(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Col_Nm_dis(m_sSymbol))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a stream-column?  (i.e. $n)                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Col_No(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Col_No(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a table-column (i.e. $c.n)                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Col_Tc(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Col_Tc(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a comma separator?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Comma(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Comma(m_sSymbol))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a sort direction?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Direction(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Direction(m_sSymbol))
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

  if (bIs_Idx_Col(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a key symbol? (i.e. $Kn)                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Key_Col(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Key_Col(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a math operator ?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Mth_Op(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Mth_Op(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a number?                                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Number(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Number(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a parenthesis?                                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Parenthesis(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Parenthesis(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a relational-operator?                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Rln_Op(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Rln_Op(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a correlation-parameter?  (i.e. %n)                   */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Prm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Sqy_Prm(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a subQuery-operator?                                  */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_No(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Sqy_No(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a subQuery-operator?                                  */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Op(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Sqy_Op(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a subQuery-column?  (i.e. __n)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Sqy_Rsl(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Sqy_Rsl(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a subQuery-column?  (i.e. __n)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Stg_Nm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Stg_Nm(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol a table synonym?  (i.e. c)                                */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Tbl_Syn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Tbl_Syn(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol a table synonym?  (i.e. c)                                */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Tbl_Syn_rep(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Tbl_Syn_rep(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a cross-refernece?  (i.e. #n)                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bIs_Xrf_No(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIs_Xrf_No(m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bNeedsBrace(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

       if (m_sSymbol.bSame("between"_Stz))        return true;
  else if (m_sSymbol.bSame("case"_Stz))           return true;
  else if (m_sSymbol.bSame("in"_Stz))             return true;
  else if (m_sSymbol.bSame("matchLeading"_Stz))   return true;
  else if (m_sSymbol.bSame("matchMiddle"_Stz))    return true;
  else if (m_sSymbol.bSame("matchMiddle1"_Stz))   return true;
  else if (m_sSymbol.bSame("matchTrailing"_Stz))  return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* convert properties to string                                               */
/*----------------------------------------------------------------------------*/
void Symbol::vPpy_To_String(String* pString)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (m_pProperties)
    m_pProperties->vToString(pString->szStz());
  else
    pString->vCopy("nil"_Stz);
}

/*----------------------------------------------------------------------------*/
/* does this card have same symbol as given argument?                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bSame(char cArg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (*m_sSymbol.azChar() == cArg)
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

  if (m_sSymbol.bSame(szArg))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bSame(Symbol* pArg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (m_sSymbol.bSame(pArg->m_sSymbol))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get symbol int                                                             */
/*----------------------------------------------------------------------------*/
i8 Symbol::iSF_number(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  i8 i = m_sSymbol.iToI8();

  return i;
}

/*----------------------------------------------------------------------------*/
/* get symbol stz                                                             */
/*----------------------------------------------------------------------------*/
Stz* Symbol::szSF_symbol(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  return m_sSymbol.szStz();
}

/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
char Symbol::cSF_char(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  return * m_sSymbol.azChar();
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

    case SF_AS_NM_syn: vSet_As_Nm_syn(iInt); break;
    case SF_COL_NO:    vSet_Col_No(iInt);    break;
    case SF_KEY_COL:   vSet_Key_Col(iInt);   break;
    case SF_NUMBER:    vSet_Number(iInt);    break;
    case SF_SQY_NO:    vSet_Sqy_No(iInt);    break;
    case SF_STG_NO:
      vSet_Stg_Nm(iInt);    break;
    case SF_XRF_NO:    vSet_Xrf_No(iInt);    break;

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
void Symbol::vSym(eSF eSymbol, Stz* szStz)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  switch (eSymbol) {

    case SF_AS_NM:   vSet_As_Nm(szStz);  break;
    case SF_COL_NM:  vSet_Col_Nm(szStz); break;
    case SF_COL_NO:  vSet_Col_No(szStz); break;
    case SF_DIRECT:
      vSet_Direction(szStz); break;
    case SF_OTHER:   vSet_Other(szStz);  break;
    case SF_SQY_KEY: vSet_Other(szStz);  break;

    default: NEVER_REACH;
  }

}

/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSym(Symbol* pSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = pSymbol->m_sSymbol;
  m_eSymbol = pSymbol->m_eSymbol;
}


/*----------------------------------------------------------------------------*/
/* get symbol char                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSymPpy(Symbol* pSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = pSymbol->m_sSymbol;
  m_eSymbol = pSymbol->m_eSymbol;

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
/* what is this symbol?                                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vSym(Stz* szSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szSymbol;
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
  if (bIs_Number())       return "Number"_Stz;
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
  StrLib::vSprint(sPpy.azChar(), xArgs, (char*)szFormat);
  va_end(xArgs);

  Y_vSet(szPpyNm, sPpy);


  if (szPpyNm->bSame(P_Type))
    RASSERT(! sPpy.bSame("!"_Stz));
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set this symbol to given as-name                                           */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Agg_Fn(Stz* szAggFn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szAggFn;
  m_eSymbol = SF_AGG_FN;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Agg_Fn());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given as-name                                           */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_As_Nm(Stz* szAsNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szAsNm;
  m_eSymbol = SF_AS_NM;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_As_Nm());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given as-name                                           */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Bol_Fn(Stz* szBolFn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szBolFn;
  m_eSymbol = SF_BOL_FN;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Bol_Fn());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given as-name                                           */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Bol_Op(Stz* szBolOp)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szBolOp;
  m_eSymbol = SF_BOL_OP;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Bol_Op());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given as-name                                           */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Bound(Stz* szBound)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szBound;
  m_eSymbol = SF_BOUND;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Bound());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given column-name                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Col_Nm(Stz* szColNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szColNm;
  m_eSymbol = SF_COL_NM;
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given column-name                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Col_No(Stz* szColNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szColNm;
  m_eSymbol = SF_COL_NO;
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given as-name                                           */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Comma(Stz* szComma)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szComma;
  m_eSymbol = SF_COMMA;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Comma());
#endif
}


/*----------------------------------------------------------------------------*/
/* set this symbol to given disambiguated-column-name                         */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Col_Nm_dis(i8 i_Child, Symbol* pSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSymSprint("C%lld_%s"_Stz, i_Child, pSymbol->m_sSymbol.szStz());
  m_eSymbol = SF_COL_NM_dis;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Col_Nm_dis());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given stream-column                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Col_No(i8 iStmColOdn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSymSprint("$%lld"_Stz, iStmColOdn);
  m_eSymbol = SF_COL_TC;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Col_No());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-column                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Col_Tc(char cTblSyn, i8 iTblColOdn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSymSprint("$%c.%lld"_Stz, cTblSyn, iTblColOdn);
  m_eSymbol = SF_COL_TC;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Col_Tc());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given column-name                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Direction(Stz* szColNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szColNm;
  m_eSymbol = SF_DIRECT;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Direction());
#endif

}

/*----------------------------------------------------------------------------*/
/* set this symbol to given index-column                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Idx_Col(i8 iIdxColOdn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSymSprint("$I%lld"_Stz, iIdxColOdn);
  m_eSymbol = SF_IDX_COL;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Idx_Col());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given key-column                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Key_Col(i8 iKeyOdn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSymSprint("$K%lld"_Stz, iKeyOdn);
  m_eSymbol = SF_KEY_COL;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Key_Col());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given as-name                                           */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Mth_Op(Stz* szMthOp)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szMthOp;
  m_eSymbol = SF_MTH_OP;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Bol_Op());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given number                                             */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Number(i8 iNumber)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = iNumber;
  m_eSymbol = SF_NUMBER;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Number());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given column-name                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Other(Stz* szOther)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szOther;
  m_eSymbol = SF_OTHER;
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given as-name                                           */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Parenthesis(Stz* szParenthesis)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szParenthesis;
  m_eSymbol = SF_PARENTHESIS;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Parenthesis());
#endif
}



/*----------------------------------------------------------------------------*/
/* set this symbol to given number                                             */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Prm_Col(i8 iOrdinal)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSymSprint("%%%lld"_Stz, iOrdinal);
  m_eSymbol = SF_PRM_COL;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Sqy_Prm());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given as-name                                           */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Rln_Op(Stz* szRlnOp)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szRlnOp;
  m_eSymbol = SF_RLN_OP;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Rln_Op());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to a stage                                                 */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Stg_Nm(i8 iStageNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSymSprint("S%02lld"_Stz, iStageNo);
  m_eSymbol = SF_OTHER;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Stg_Nm());
#endif

}

/*----------------------------------------------------------------------------*/
/* set this symbol to given subquery-number                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Sqy_No(i8 iSqNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSymSprint("__sq%02lld"_Stz, iSqNo);
  m_eSymbol = SF_SQY_NO;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Sqy_No());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given as-name                                           */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Sqy_Op(Stz* szSqyOp)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szSqyOp;
  m_eSymbol = SF_SQY_OP;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Rln_Op());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given synthetic-as                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_As_Nm_syn(i8 iAsNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSymSprint("COL%lld"_Stz, iAsNo);
  m_eSymbol = SF_AS_NM_syn;

  Y_vSet(P_Synthetic);

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_As_Nm_syn());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-synonym                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Tbl_Syn(char cTblSyn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSymSprint("%c"_Stz, cTblSyn);
  m_eSymbol = SF_TBL_SYN;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Tbl_Syn());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-synonym                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Tbl_Syn_rep(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  char cTableSyn_actual = cSF_char();
  cTableSyn_actual = StrLib::cToUpper(cTableSyn_actual);

  vSymSprint("%c"_Stz, cTableSyn_actual);
  m_eSymbol = SF_TBL_SYN;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Tbl_Syn_rep());
#endif
}


/*----------------------------------------------------------------------------*/
/* set this symbol to given cross-reference no                                */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_Xrf_No(i8 iXrefNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSymSprint("#%lld"_Stz, iXrefNo);
  m_eSymbol = SF_XRF_NO;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIs_Xrf_No());
#endif
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
  StrLib::vSprint(sSymbol.azChar(), xArgs, (char*)szFormat);
  va_end(xArgs);

  m_sSymbol = sSymbol;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/