/*============================================================================*/
/* Symbol.cpp -- Symbolic Value                                               */
/*============================================================================*/

#include "Card.hpp"
#include "CardCtl.hpp"
#include "Board.hpp"
#include "CardIter.hpp"

#include "Card_Index.hpp"  // TODO: remove
#include "Index.hpp"       // TODO: remove

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Symbol::Symbol(void)
{
  m_pProperties = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Symbol::Symbol(Stz* szArg)
{
  m_sSymbol = szArg;
  m_pProperties = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::operator =(Stz* szArg)
{
  m_sSymbol = szArg;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::operator =(Symbol* pSymbol)
{
  m_sSymbol = pSymbol->m_sSymbol;

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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::operator =(Symbol& rxSymbol)
{
  m_sSymbol = rxSymbol.m_sSymbol;

  if (rxSymbol.m_pProperties) {
    if (! m_pProperties)
      m_pProperties = new TagSet;
    m_pProperties->vCopy(rxSymbol.m_pProperties);
  }
  else
  if (m_pProperties) {
    delete m_pProperties;
    m_pProperties = nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/* auto conversion operator : as Stz*                                         */
/*----------------------------------------------------------------------------*/
Symbol::operator Stz*(void)
{

  return m_sSymbol.szStz();
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* evaluate relational op                                                     */
/*----------------------------------------------------------------------------*/
eRO Symbol::eEvalRelnOp(void)
{
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
  i8 i_Col = StrLib::iGleanInt(m_sSymbol.azChar());

  return i_Col;
}

/*----------------------------------------------------------------------------*/
/* get table synonym from $c.n                                                */
/*----------------------------------------------------------------------------*/
char Symbol::cExtractTableSynonym(void)
{
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
/* is this symbol an aggregate op?                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsAggOp(void)
{

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
/* is this symbol a comma separator?                                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsComma(void)
{
  if (m_sSymbol.bSame(","_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for an index-column?  (i.e. $In)                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsIndexColumn(void)
{
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
/* is this a symbol for a correlation-parameter?  (i.e. %n)                   */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsParameterOrdinal(void)
{
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
/* is this symbol a relational-operator?                                      */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsRelnOp(void)
{
  if (RO_NONE != eEvalRelnOp())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a symbol for a stream-column?  (i.e. $n)                           */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsStreamOrdinal(void)
{
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
/* is this a symbol for a table-column (i.e. $c.n)                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsColumnOrdinal(void)
{
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
/* is this a symbol for a column?  (i.e. $n or $c.n)                          */
/*----------------------------------------------------------------------------*/
bool Symbol::bIsValueOrdinal(void)
{
  if (bIsStreamOrdinal() || bIsColumnOrdinal())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bNeedsBrace(void)
{
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
/* does this card have same symbol as given argument?                         */
/*----------------------------------------------------------------------------*/
bool Symbol::bSameAs(char cArg)
{
  if (*this->szSymbol()->azChar() == cArg)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bSameAs(Stz* szArg)
{
  if (this->szSymbol()->bSame(szArg))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Symbol::bSameAs(Symbol* pArg)
{
  if (this->szSymbol()->bSame(pArg->szSymbol()))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSet(Symbol* pSymbol)
{

  m_sSymbol = pSymbol->m_sSymbol;

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
/* sprint symbol                                                              */
/*----------------------------------------------------------------------------*/
void Symbol::vSprint(Stz* szFormat, ...)
{
  s256 sSymbol;

  va_list xArgs;
  va_start(xArgs,szFormat);
  StrLib::vSprint(sSymbol.azChar(), xArgs, (char*)szFormat);
  va_end(xArgs);

  m_sSymbol = sSymbol;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSetIndexColOrdinal(i8 iIdxColOdn)
{
  vSprint("$I%lld"_Stz, iIdxColOdn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSetKeyOrdinal(i8 iKeyOdn)
{
  vSprint("$K%lld"_Stz, iKeyOdn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSetStreamOrdinal(i8 iStmColOdn)
{
  vSprint("$%lld"_Stz, iStmColOdn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSetSubQueryOrdinal(i8 iSqNo)
{
  vSprint("__sq%02lld"_Stz, iSqNo);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSetTableColSyn(char cTblSyn)
{
  //((char*) m_sSymbol.azChar())[1] = cTblSyn;

  vSprint("%c"_Stz, cTblSyn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Symbol::vSetTableColOrdinal(char cTblSyn, i8 iTblColOdn)
{
  vSprint("$%c.%lld"_Stz, cTblSyn, iTblColOdn);
}

/*----------------------------------------------------------------------------*/
/* get type                                                                   */
/*----------------------------------------------------------------------------*/
Stz* Symbol::szType(void)
{
  Stz* sz = Y_szGet(P_Type);

  return sz;
}

/*----------------------------------------------------------------------------*/
/* get type                                                                   */
/*----------------------------------------------------------------------------*/
void Symbol:: vType(Stz* szType)
{
  Y_vSet(P_Type,szType);
}


/*----------------------------------------------------------------------------*/
/* get symbol                                                                 */
/*----------------------------------------------------------------------------*/
Stz* Symbol::szSymbol(void)
{
  return m_sSymbol.szStz();
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
void Symbol::Y_vAll(Card* pCard_arg)
{

  if (!pCard_arg->pSymbol()->m_pProperties) {
    if (m_pProperties) {
      delete m_pProperties;
      m_pProperties = nullptr;
    }
    return;
  }

  if (! m_pProperties)
    m_pProperties = new TagSet;

  m_pProperties->vCopy(pCard_arg->pSymbol()->m_pProperties);
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
void Symbol::Y_vAll(TagSet* pProperties)
{

  if (! m_pProperties)
    m_pProperties = new TagSet;

  m_pProperties->vCopy(pProperties);
}

/*----------------------------------------------------------------------------*/
/* remove given property                                                      */
/*----------------------------------------------------------------------------*/
void Symbol::Y_vCut(Stz* szPpyNm)
{

  if (! m_pProperties)
    return;

  m_pProperties->vDelete(szPpyNm);
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
i8 Symbol::Y_iGet(Stz* szPpyNm)
{

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

  if (! m_pProperties)
    return C_UNKNOWN;

  i8 iPpy = m_pProperties->iValue(i_Ppy);

  return iPpy;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
Stz* Symbol::Y_szGet(Stz* szPpyNm)
{

  if (! m_pProperties)
    return nullptr;

  Stz* szPpy = m_pProperties->szValue(szPpyNm);

  return szPpy;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
Stz* Symbol::Y_szGet(i8 i_Ppy)
{

  if (! m_pProperties)
    return nullptr;

  Stz* szPpy = m_pProperties->szValue(i_Ppy);

  return szPpy;
}

/*----------------------------------------------------------------------------*/
/* has given property ?                                                       */
/*----------------------------------------------------------------------------*/
bool Symbol::Y_bHas(Stz* szPpyNm)
{

  if (! m_pProperties)
    return false;

  return m_pProperties->bHasName(szPpyNm);
}

/*----------------------------------------------------------------------------*/
/* get number of properties                                                   */
/*----------------------------------------------------------------------------*/
i8 Symbol::Y_nPpy(void)
{

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

  if (! m_pProperties)
    m_pProperties = new TagSet;

  m_pProperties->vReplace(szPpyNm, "y"_Stz);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Symbol::Y_vSet(Stz* szPpyNm, i8 iPpy)
{

  if (! m_pProperties)
    m_pProperties = new TagSet;

  m_pProperties->vReplace(szPpyNm, iPpy);
}

/*----------------------------------------------------------------------------*/
/* set given property                                                         */
/*----------------------------------------------------------------------------*/
void Symbol::Y_vSet(Stz* szPpyNm, Stz* szPpy)
{
  if (! m_pProperties)
    m_pProperties = new TagSet;

  m_pProperties->vReplace(szPpyNm, szPpy);

  if (szPpyNm->bSame(P_Type)) {
    RASSERT(!szPpy->bSame(""_Stz));
    RASSERT(!szPpy->bSame("!"_Stz));
  }
}

/*----------------------------------------------------------------------------*/
/* set property from given symbol                                             */
/*----------------------------------------------------------------------------*/
void Symbol::Y_vSet(Stz* szPpyNm, Symbol* pSymbol)
{
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

  s256 sPpy;

  va_list xArgs;
  va_start(xArgs,szFormat);
  StrLib::vSprint(sPpy.azChar(), xArgs, (char*)szFormat);
  va_end(xArgs);

  Y_vSet(szPpyNm, sPpy);


  if (szPpyNm->bSame(P_Type))
    RASSERT(! sPpy.bSame("!"_Stz));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/