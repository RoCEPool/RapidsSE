/*============================================================================*/
/* Card_Term.cpp -- Term                                                      */
/*============================================================================*/

#include "Card_Term.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Term,CK_TERM)
__aNewFunction(Card_Term,CN_CARD)
__vDeleteFunction(Card_Term,CN_CARD)

__AssertFunction(STORE,Card_Term,CN_CARD)
__bIsCard(STORE,Card_Term,CK_TERM)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Term::Card_Term(Stz* szTerm)
: Card(CK_TERM,szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //vValue(szTerm);
  //m_bPrintNm = false;
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Term::Card_Term(Column* pColumn)
: Card(CK_TERM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vValue(pColumn->szColNm());
  //m_bPrintNm = false;
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Term::Card_Term(Token* pTkn)
: Card(CK_TERM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  s4K sString;
  
  if (pTkn->eToken() == TN_STRING) {
    sString.vSprint("'%s'"_Stz,pTkn->szChars());
    vValue(sString);
  }
  else {
    vValue(pTkn->szChars());
  }
  //m_bPrintNm = false;
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Term::~Card_Term(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* form tableColOrdinal from tableSyn and tableColNo                          */
/*----------------------------------------------------------------------------*/
void Card_Term::vTableColOrd(String* psTableColOrdinal, char cTableSyn, i8 iTableColNo)
{
  _static_

  psTableColOrdinal->vSprint("$$%c.%lld"_Stz, cTableSyn, iTableColNo);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* evaluate bound                                                             */
/*----------------------------------------------------------------------------*/
ePB Card_Term::eBound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eRO eRelOp = this->eRelOp();

  switch (eRelOp) {
    case RO_LESS_THAN:     return PB_UBOUND;
    case RO_LESS_EQUAL:    return PB_UBOUND;
    case RO_EQUAL_TO:      return PB_EBOUND;
    case RO_GREATER_EQUAL: return PB_LBOUND;
    case RO_GREATER_THAN:  return PB_LBOUND;

    default:
      vTrace("%s(%lld): unexpected eRelOp %lld %s\n"_Stz
          ,__func__
          ,(i8)__LINE__
          ,(i8)eRelOp
          ,aRO[eRelOp]);
      RASSERT(0);
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* get ordinal index value  (i.e. $In)                                        */
/*----------------------------------------------------------------------------*/
i8 Card_Term::iIndexColNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bIsIndexColOrdinal());

  i8 i_Ord = StrLib::iGleanInt(szValue()->azChar());

  return i_Ord;
}

/*----------------------------------------------------------------------------*/
/* is this atom an aggregate op?                                              */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsAggOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = szValue();

       if (szTerm->bSame("avg"_Stz))      return true;
  else if (szTerm->bSame("count"_Stz))    return true;
  else if (szTerm->bSame("countAll"_Stz)) return true;
  else if (szTerm->bSame("max"_Stz))      return true;
  else if (szTerm->bSame("min"_Stz))      return true;
//else if (szTerm->bSame("stddev"_Stz))   return true;
  else if (szTerm->bSame("sum"_Stz))      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this term a comma separator?                                            */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsCommaSeparator(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (szValue()->bSame(","_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is term an index-column ordinal?  (i.e. $In)                               */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsIndexColOrdinal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = szValue();

  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];
  char acThird  = szTerm->azChar()[2];

  if (   '$' == acFirst
      && ('I' == acSecond || 'i' == acSecond)
      && StrLib::bIsDigit(acThird))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is term a stream-column ordinal?  (i.e. $n)                                */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsStreamColOrdinal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = szValue();

  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];

  if (   '$' == acFirst
      && StrLib::bIsDigit(acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is term an table-column ordinal?  (i.e. $$c.n)                             */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsTableColOrdinal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = szValue();

  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];
  char acThird  = szTerm->azChar()[2];
  char acFourth = szTerm->azChar()[3];
  char acFifth  = szTerm->azChar()[4];

  if (   '$' == acFirst
      && '$' == acSecond
      && StrLib::bIsAlpha(acThird)
      && '.' == acFourth
      && StrLib::bIsDigit(acFifth))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* evaluate relational op                                                     */
/*----------------------------------------------------------------------------*/
eRO Card_Term::eRelOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = this->sz();

  if (szTerm->bSame("<"_Stz))   return RO_LESS_THAN;
  else if (szTerm->bSame("<="_Stz))  return RO_LESS_EQUAL;
  else if (szTerm->bSame("="_Stz))   return RO_EQUAL_TO;
  else if (szTerm->bSame("=="_Stz))  return RO_EQUAL_TO;
  else if (szTerm->bSame("!="_Stz))  return RO_NOT_EQUAL;
  else if (szTerm->bSame(">="_Stz))  return RO_GREATER_EQUAL;
  else if (szTerm->bSame(">"_Stz))   return RO_GREATER_THAN;

  else return RO_NONE;
}

/*----------------------------------------------------------------------------*/
/* get stream column number n  (i.e. $n)                                      */
/*----------------------------------------------------------------------------*/
i8 Card_Term::iStreamColNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bIsStreamColOrdinal());

  i8 i_Col = StrLib::iGleanInt(szValue()->azChar());

  return i_Col;
}

/*----------------------------------------------------------------------------*/
/* get table column number n (i.e. $$c.n)                                     */
/*----------------------------------------------------------------------------*/
i8 Card_Term::iTableColNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bIsTableColOrdinal());

  i8 i_Col = StrLib::iGleanInt(szValue()->azChar());

  return i_Col;
}

/*----------------------------------------------------------------------------*/
/* get table synonym c (i.e. $$c.n)                                           */
/*----------------------------------------------------------------------------*/
char Card_Term::cTableSyn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bIsTableColOrdinal());

  Stz* szTerm = this->sz();

  char cSyn = szTerm->azChar()[2];

  return cSyn;
}

/*----------------------------------------------------------------------------*/
/* evaluate term                                                              */
/*----------------------------------------------------------------------------*/
eTE Card_Term::eTermEval(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = this->sz();

       if (szTerm->bSame("and"_Stz))     return TE_AND;
  else if (szTerm->bSame("as"_Stz))      return TE_AS;
  else if (szTerm->bSame("boolean"_Stz)) return TE_BOOLEAN;
  else if (szTerm->bSame("or"_Stz))      return TE_OR;
  else if (szTerm->bSame("type"_Stz))    return TE_TYPE;

  else if (szTerm->bSame("<"_Stz))       return TE_COMPARE;
  else if (szTerm->bSame("<="_Stz))      return TE_COMPARE;
  else if (szTerm->bSame("="_Stz))       return TE_COMPARE;
  else if (szTerm->bSame("=="_Stz))      return TE_COMPARE;
  else if (szTerm->bSame("!="_Stz))      return TE_COMPARE;
  else if (szTerm->bSame("<>"_Stz))      return TE_COMPARE;
  else if (szTerm->bSame(">="_Stz))      return TE_COMPARE;
  else if (szTerm->bSame(">"_Stz))       return TE_COMPARE;

  else return TE_OTHER;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
