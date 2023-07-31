/*============================================================================*/
/* Symbol.cpp -- Symbolic Value                                               */
/*============================================================================*/

#include "Card.hpp"
#include "CardCtl.hpp"
#include "Board.hpp"
#include "CardIter.hpp"

#include "Card_Index.hpp"  // TODO: remove
#include "Index.hpp"       // TODO: remove

//#define X_SET_CROSS_CHECK

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
Symbol::Symbol(Stz* szSymbol, eSF eSymbol)
    : Object(CN_SYMBOL)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szSymbol;
  m_eSymbol = eSymbol;
  m_pProperties = nullptr;

  _checkInitialised_(CN_SYMBOL);

  _postcondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Symbol::Symbol(Symbol* pSymbol)
    : Object(CN_SYMBOL)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSet(pSymbol);

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
/* evaluate symbol form of given arg                                          */
/*----------------------------------------------------------------------------*/
eSF Symbol::eEvalForm(Stz* szArg)
{
  _static_

  /*
  enum eSF // symbol form
  {
    SF_NONE = 0,   // none

    SF_ASNAME,
    SF_DASCNE,
    SF_IDXCOL,
    SF_KEYCOL,
    SF_NUMBER,

    SF_OTHER,

    SF_PRMCOL,
    SF_SASCNM,
    SF_SQRYNO,
    SF_STMCOL,
    SF_TBLCOL,
    SF_TBLSYN,
    SF_XREFNO,

    SF_ENUMS
  };
  */

  if (bIsColumnOrdinal(szArg))      return SF_TBLCOL;
  if (bIsCrossRef(szArg))           return SF_XREFNO;
  if (bIsDisambiguatedColNm(szArg)) return SF_DASCNE;
  if (bIsIndexColumn(szArg))        return SF_IDXCOL;
  if (bIsNumber(szArg))             return SF_NUMBER;
  if (bIsKeyOrdinal(szArg))         return SF_KEYCOL;
  if (bIsParameterOrdinal(szArg))   return SF_PRMCOL;
  if (bIsStreamOrdinal(szArg))      return SF_STMCOL;
  if (bIsSubQueryColumn(szArg))     return SF_SQRYNO;
  if (bIsSyntheticAs(szArg))        return SF_SASCNM;
  if (bIsTableSyn(szArg))           return SF_TBLSYN;

  if (StrLib::bIsSqlName(szArg->azChar()))  return SF_ASNAME;

  return SF_NONE;
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
/* is value a __subQuery like name?                                           */
/*----------------------------------------------------------------------------*/
bool Symbol::b_IsSubQueryName(BASE::Stz *szStz)
{
  _static_

  if (StrLib::bBeginsWith(szStz->azChar(), "__subquery") ||
      StrLib::bBeginsWith(szStz->azChar(), "__sq"))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol an aggregate op?                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsAggOp(Stz* szArg)
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
bool Symbol::bIsBoolFn(Stz* szArg)
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
/* is this symbol a bound?                                                    */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsBound(Stz* szArg)
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
bool Symbol::bIsColNm(Stz* szArg)
{
  _static_

  if ( StrLib::bIsSqlName(szArg->azChar()) &&
       ( (! bIsAggOp(szArg))  &&
         (! bIsBoolFn(szArg)) &&
         (! bIsBound(szArg))  &&
         (! bIsSubQueryOp(szArg)) ) )
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a table-column (i.e. $c.n)                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsColumnOrdinal(Stz* szArg)
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
bool Symbol::bIsComma(Stz* szArg)
{
  _static_

  if (szArg->bSame(","_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a cross-refernece?  (i.e. #n)                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsCrossRef(Stz* szArg)
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
/* is this a symbol for a disambiguated as?  (i.e. Cn_Xyz)                    */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsDisambiguatedColNm(Stz* szArg)
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
/* is this a symbol for an index-column?  (i.e. $In)                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsIndexColumn(Stz* szArg)
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
bool Symbol::bIsKeyOrdinal(Stz* szArg)
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
/* is this a name?                                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsName(Stz* szArg)
{
  _static_

  if (StrLib::bIsSqlName(szArg->azChar()))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a number?                                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsNumber(Stz* szArg)
{
  _static_

  if (StrLib::bIsDecimal(szArg->azChar()))  return true;
  if (StrLib::bIsFloat(szArg->azChar()))    return true;
  if (StrLib::bIsInteger(szArg->azChar()))  return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a numeric operator ?                                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsNumbOp(Stz* szArg)
{
  _static_

  if (szArg->bSame("+"_Stz))     return true;
  if (szArg->bSame("-"_Stz))     return true;
  if (szArg->bSame("*"_Stz))     return true;
  if (szArg->bSame("/"_Stz))     return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this an other?                                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsOther(Stz* szArg)
{
  _static_

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a correlation-parameter?  (i.e. %n)                   */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsParameterOrdinal(Stz* szArg)
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
/* is this symbol a parenthesis?                                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsParenthesis(Stz* szArg)
{
  _static_

  if (szArg->bSame("("_Stz))     return true;
  if (szArg->bSame(")"_Stz))     return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a relational-operator?                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsRelnOp(Stz* szArg)
{
  _static_

  if (RO_NONE != eEvalRelnOp(szArg))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a stream-column?  (i.e. $n)                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsStreamOrdinal(Stz* szArg)
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
/* is this a symbol for a subQuery-operator?                                  */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsSubQueryOp(Stz* szArg)
{
  _static_

  if (szArg->bSame("exists"_Stz))    return true;
  else if (szArg->bSame("in"_Stz))        return true;
  else if (szArg->bSame("notExists"_Stz)) return true;
  else if (szArg->bSame("notIn"_Stz))     return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a subQuery-column?  (i.e. __n)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsSubQueryColumn(Stz* szArg)
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
/* is this a symbol for a synthetic as?  (i.e. __COLn)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsSyntheticAs(Stz* szArg)
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

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol a table synonym?  (i.e. $c)                               */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsTableSyn(Stz* szArg)
{
  _static_

  char acFirst  = szArg->azChar()[0];
  char acSecond = szArg->azChar()[1];
  char acThird  = szArg->azChar()[2];

  if (   '$' == acFirst
         && StrLib::bIsAlpha(acSecond)
         && '\0' == acThird)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a column?  (i.e. $n or $c.n)                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsValueOrdinal(Stz* szArg)
{
  _static_

  if (bIsStreamOrdinal(szArg) || bIsColumnOrdinal(szArg))
    return true;

  return false;
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
eCT Symbol::eColType(void)
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
Stz* Symbol::szColType(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  Stz* sz = Y_szGet(P_Type);

  return sz;
}

/*----------------------------------------------------------------------------*/
/* get type                                                                   */
/*----------------------------------------------------------------------------*/
void Symbol:: vColType(Stz* szColType)
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

       if (m_sSymbol.bSame("<"_Stz))   return RO_LESS_THAN;
  else if (m_sSymbol.bSame("<="_Stz))  return RO_LESS_EQUAL;
  else if (m_sSymbol.bSame("="_Stz))   return RO_EQUAL_TO;
  else if (m_sSymbol.bSame("=="_Stz))  return RO_EQUAL_TO;
  else if (m_sSymbol.bSame("!="_Stz))  return RO_NOT_EQUAL;
  else if (m_sSymbol.bSame(">="_Stz))  return RO_GREATER_EQUAL;
  else if (m_sSymbol.bSame(">"_Stz))   return RO_GREATER_THAN;

  else return RO_NONE;
}

/*----------------------------------------------------------------------------*/
/* evaluate sub-query op                                                      */
/*----------------------------------------------------------------------------*/
eQO Symbol::eEvalSubQryOp(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

       if (m_sSymbol.bSame("exists"_Stz))    return QO_EXISTS_OP;
  else if (m_sSymbol.bSame("notExists"_Stz)) return QO_NOT_EXISTS_OP;
  else if (m_sSymbol.bSame("in"_Stz))        return QO_IN_OP;
  else if (m_sSymbol.bSame("notIn"_Stz))     return QO_NOT_IN_OP;

  else return QO_NONE;
}

/*----------------------------------------------------------------------------*/
/* get ordinal n  (i.e. $n or $In or $Kn or $c.n)                             */
/*----------------------------------------------------------------------------*/
i8 Symbol::iExtractOrdinal(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  i8 i_Col = StrLib::iGleanInt(m_sSymbol.azChar());

  return i_Col;
}

/*----------------------------------------------------------------------------*/
/* get table synonym from $c.n                                                */
/*----------------------------------------------------------------------------*/
char Symbol::cExtractTableSynonym(void)
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
bool Symbol::bHasSymbolicValue(void)
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
/* is this symbol an aggregate op?                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsAggOp(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

       if (m_sSymbol.bSame("avg"_Stz))      return true;
  else if (m_sSymbol.bSame("count"_Stz))    return true;
  else if (m_sSymbol.bSame("countAll"_Stz)) return true;
  else if (m_sSymbol.bSame("max"_Stz))      return true;
  else if (m_sSymbol.bSame("min"_Stz))      return true;
//else if (m_sSymbol.bSame("stddev"_Stz))   return true;
  else if (m_sSymbol.bSame("sum"_Stz))      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a boolean function?                                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsBoolFn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

       if (m_sSymbol.bSame("between"_Stz))         return true;
  else if (m_sSymbol.bSame("inList"_Stz))          return true;
  else if (m_sSymbol.bSame("isNull"_Stz))          return true;
  else if (m_sSymbol.bSame("like2"_Stz))           return true;
  else if (m_sSymbol.bSame("like3"_Stz))           return true;
  else if (m_sSymbol.bSame("matchLeading"_Stz))    return true;
  else if (m_sSymbol.bSame("matchMiddle"_Stz))     return true;
  else if (m_sSymbol.bSame("matchMiddle1"_Stz))    return true;
  else if (m_sSymbol.bSame("matchTrailing"_Stz))   return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a bound?                                                    */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsBound(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

       if (m_sSymbol.bSame("BOUND"_Stz))   return true;
  else if (m_sSymbol.bSame("LBOUND"_Stz))  return true;
  else if (m_sSymbol.bSame("UBOUND"_Stz))  return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a column name?                                                     */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsColNm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if ( StrLib::bIsSqlName(m_sSymbol.azChar()) &&
       ( (! bIsAggOp()) && (! bIsBoolFn()) && (! bIsBound()) && (! bIsSubQueryOp()) ) )
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a table-column (i.e. $c.n)                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsColumnOrdinal(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  char acFirst  = m_sSymbol.azChar()[0];
  char acSecond = m_sSymbol.azChar()[1];
  char acThird  = m_sSymbol.azChar()[2];
  char acFourth = m_sSymbol.azChar()[3];

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
bool Symbol::bIsComma(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (m_sSymbol.bSame(","_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a cross-refernece?  (i.e. #n)                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsCrossRef(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  char acFirst  = m_sSymbol.azChar()[0];
  char acSecond = m_sSymbol.azChar()[1];

  if ('#' == acFirst &&
      StrLib::bIsDigit(acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a comma separator?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsDisambiguatedColNm(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIsDisambiguatedColNm(m_sSymbol.szStz()))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for an index-column?  (i.e. $In)                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsIndexColumn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  char acFirst  = m_sSymbol.azChar()[0];
  char acSecond = m_sSymbol.azChar()[1];
  char acThird  = m_sSymbol.azChar()[2];
  char acFourth = m_sSymbol.azChar()[3];

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
bool Symbol::bIsKeyOrdinal(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  char acFirst  = m_sSymbol.azChar()[0];
  char acSecond = m_sSymbol.azChar()[1];
  char acThird  = m_sSymbol.azChar()[2];
  char acFourth = m_sSymbol.azChar()[3];

  if ('$' == acFirst
      && ('K' == acSecond || 'k' == acSecond)
      && StrLib::bIsDigit(acThird)
      && ! acFourth)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a name?                                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsName(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (StrLib::bIsSqlName(m_sSymbol.azChar()))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a number?                                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsNumber(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (StrLib::bIsDecimal(m_sSymbol.azChar()))  return true;
  if (StrLib::bIsFloat(m_sSymbol.azChar()))    return true;
  if (StrLib::bIsInteger(m_sSymbol.azChar()))  return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a numeric operator ?                                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsNumbOp(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (m_sSymbol.bSame("+"_Stz))     return true;
  if (m_sSymbol.bSame("-"_Stz))     return true;
  if (m_sSymbol.bSame("*"_Stz))     return true;
  if (m_sSymbol.bSame("/"_Stz))     return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol an other ?                                                  */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsOther(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  return bIsOther(m_sSymbol.szStz());
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a correlation-parameter?  (i.e. %n)                   */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsParameterOrdinal(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  char acFirst  = m_sSymbol.azChar()[0];
  char acSecond = m_sSymbol.azChar()[1];
  char acThird  = m_sSymbol.azChar()[2];

  if ('%' == acFirst
      && StrLib::bIsDigit(acSecond)
      && ! acThird)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a parenthesis?                                              */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsParenthesis(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (m_sSymbol.bSame("("_Stz))     return true;
  if (m_sSymbol.bSame(")"_Stz))     return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this symbol a relational-operator?                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsRelnOp(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (RO_NONE != eEvalRelnOp())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a stream-column?  (i.e. $n)                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsStreamOrdinal(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  char acFirst  = m_sSymbol.azChar()[0];
  char acSecond = m_sSymbol.azChar()[1];

  if ('$' == acFirst &&
      StrLib::bIsDigit(acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a subQuery-operator?                                  */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsSubQueryOp(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

       if (m_sSymbol.bSame("exists"_Stz))    return true;
  else if (m_sSymbol.bSame("in"_Stz))        return true;
  else if (m_sSymbol.bSame("notExists"_Stz)) return true;
  else if (m_sSymbol.bSame("notIn"_Stz))     return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a subQuery-column?  (i.e. __n)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsSubQueryColumn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  char acFirst  = m_sSymbol.azChar()[0];
  char acSecond = m_sSymbol.azChar()[1];
  char acThird  = m_sSymbol.azChar()[2];
  char acFourth = m_sSymbol.azChar()[3];

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
/* is this a symbol for a synthetic as?  (i.e. __COLn)                        */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsSyntheticAs(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  char acFirst  = m_sSymbol.azChar()[0];
  char acSecond = m_sSymbol.azChar()[1];
  char acThird  = m_sSymbol.azChar()[2];
  char acFourth = m_sSymbol.azChar()[3];

  if (   'C' == acFirst
      && 'O' == acSecond
      && 'L' == acThird
      && StrLib::bIsDigit(acFourth))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol a table synonym?  (i.e. c)                                */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsTableSyn(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

//  char acFirst  = m_sSymbol.azChar()[0];
//  char acSecond = m_sSymbol.azChar()[1];
//  char acThird  = m_sSymbol.azChar()[2];
//
//  if (   '$' == acFirst
//      && StrLib::bIsAlpha(acSecond)
//      && '\0' == acThird)
//    return true;

  char acFirst  = m_sSymbol.azChar()[0];
  char acSecond = m_sSymbol.azChar()[1];

  if (StrLib::bIsAlpha(acFirst)
      && '\0' == acSecond)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a column?  (i.e. $n or $c.n)                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsValueOrdinal(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIsStreamOrdinal() || bIsColumnOrdinal())
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
void Symbol::vPpyStr(String* pString)
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

  if (*this->szSF__print()->azChar() == cArg)
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

  if (this->szSF__print()->bSame(szArg))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bSame(Symbol* pArg)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (this->szSF__print()->bSame(pArg->szSF__print()))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* set this symbol using given symbol                                         */
/*----------------------------------------------------------------------------*/
void Symbol::vSet(Symbol* pSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = pSymbol->m_sSymbol;
  m_eSymbol = pSymbol->m_eSymbol;

  if (pSymbol->m_pProperties) {
    if (! m_pProperties)
      m_pProperties = new TagSet;
    m_pProperties->vCopy(pSymbol->m_pProperties);
  }
  else
  if (m_pProperties) {
    delete m_pProperties;
    m_pProperties = nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/* set this symbol using given symbol                                         */
/*----------------------------------------------------------------------------*/
void Symbol::vSet(eSF eSymbol, Stz* szSymbol)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szSymbol;
  m_eSymbol = eSymbol;
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given as-name                                           */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_asName(Stz* szAsNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szAsNm;
  m_eSymbol = SF_ASNAME;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIsColNm());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given column-name                                       */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_colNm(Stz* szColNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szColNm;
  m_eSymbol = SF_COLNM;
}


/*----------------------------------------------------------------------------*/
/* set this symbol to given cross-reference no                                */
/*----------------------------------------------------------------------------*/
void Symbol::vSetCrossRef(i8 iCrossRef)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSprint("#%lld"_Stz, iCrossRef);
  m_eSymbol = SF_XREFNO;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIsCrossRef());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given disambiguated-column-name                         */
/*----------------------------------------------------------------------------*/
void Symbol::vSetDisambiguatedColNm(i8 i_Child, Stz* szColNm)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSprint("C%lld_%s"_Stz, i_Child, szColNm);
  m_eSymbol = SF_DASCNE;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIsDisambiguatedColNm());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given index-column                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vSetIndexColOrdinal(i8 iIdxColOdn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSprint("$I%lld"_Stz, iIdxColOdn);
  m_eSymbol = SF_IDXCOL;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIsIndexColumn());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given key-column                                        */
/*----------------------------------------------------------------------------*/
void Symbol::vSetKeyOrdinal(i8 iKeyOdn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSprint("$K%lld"_Stz, iKeyOdn);
  m_eSymbol = SF_KEYCOL;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIsKeyOrdinal());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given number                                             */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_number(i8 iNumber)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

   m_sSymbol = iNumber;
  m_eSymbol = SF_NUMBER;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIsNumber());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given other                                             */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_other(Stz* szOther)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  m_sSymbol = szOther;
  m_eSymbol = SF_OTHER;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIsOther());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given number                                             */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_parameterOrdinal(i8 iNumber)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSprint("%%%lld"_Stz, iNumber);
  m_eSymbol = SF_PRMCOL;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIsNumber());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to a stage                                                 */
/*----------------------------------------------------------------------------*/
void Symbol::vSet_stageNo(i8 iStageNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSprint("S%02lld"_Stz, iStageNo);
  m_eSymbol = SF_OTHER;
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given stream-column                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vSetStreamOrdinal(i8 iStmColOdn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSprint("$%lld"_Stz, iStmColOdn);
  m_eSymbol = SF_STMCOL;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIsStreamOrdinal());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given subquery-number                                   */
/*----------------------------------------------------------------------------*/
void Symbol::vSetSubQueryOrdinal(i8 iSqNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSprint("__sq%02lld"_Stz, iSqNo);
  m_eSymbol = SF_SQRYNO;

#ifdef X_SET_CROSS_CHECK
  //RASSERT(bIsSubQueryColumn());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given synthetic-as                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vSetSyntheticAs(i8 iAsNo)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSprint("COL%lld"_Stz, iAsNo);
  m_eSymbol = SF_SASCNM;

  Y_vSet(P_Synthetic);

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIsSyntheticAs());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-synonym                                     */
/*----------------------------------------------------------------------------*/
void Symbol::vSetTableColSyn(char cTblSyn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSprint("%c"_Stz, cTblSyn);
  m_eSymbol = SF_TBLSYN;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIsTableSyn());
#endif
}

/*----------------------------------------------------------------------------*/
/* set this symbol to given table-column                                      */
/*----------------------------------------------------------------------------*/
void Symbol::vSetTableColOrdinal(char cTblSyn, i8 iTblColOdn)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  vSprint("$%c.%lld"_Stz, cTblSyn, iTblColOdn);
  m_eSymbol = SF_TBLCOL;

#ifdef X_SET_CROSS_CHECK
  RASSERT(bIsColumnOrdinal());
#endif
}

/*----------------------------------------------------------------------------*/
/* get symbol stz                                                             */
/*----------------------------------------------------------------------------*/
Stz* Symbol::szSF__print(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  return m_sSymbol.szStz();
}

/*----------------------------------------------------------------------------*/
/* get symbol stz                                                             */
/*----------------------------------------------------------------------------*/
Stz* Symbol::szSF_any(void)
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
Stz* Symbol::szSF_other(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  return m_sSymbol.szStz();
}

/*----------------------------------------------------------------------------*/
/* get symbol stz                                                             */
/*----------------------------------------------------------------------------*/
Stz* Symbol::szSF_string(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  return m_sSymbol.szStz();
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
Stz* Symbol::szWhatIs(void)
{
  _precondition_(_MAGIC_, CN_SYMBOL, _FOOTER_);

  if (bIsAggOp())            return "AggOp"_Stz;
  if (bIsBoolFn())           return "BoolFn"_Stz;
  if (bIsBound())            return "Bound"_Stz;
  if (bIsColumnOrdinal())    return "ColumnOrdinal"_Stz;
  if (bIsComma())            return "Comma"_Stz;
  if (bIsIndexColumn())      return "IndexColumn"_Stz;
  if (bIsKeyOrdinal())       return "KeyOrdinal"_Stz;
  if (bIsNumber())           return "Number"_Stz;
  if (bIsNumbOp())           return "NumbOp"_Stz;
  if (bIsParameterOrdinal()) return "ParameterOrdinal"_Stz;
  if (bIsParenthesis())      return "Parenthesis"_Stz;
  if (bIsRelnOp())           return "RelnOp"_Stz;
  if (bIsName())             return "Name"_Stz;
  if (bIsStreamOrdinal())    return "StreamOrdinal"_Stz;
  if (bIsSubQueryOp())       return "SubQueryOp"_Stz;
  if (bIsSubQueryColumn())   return "SubQueryColumn"_Stz;
  if (bIsSyntheticAs())      return "SyntheticAs"_Stz;

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
/* sprint symbol                                                              */
/*----------------------------------------------------------------------------*/
void Symbol::vSprint(Stz* szFormat, ...)
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