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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* evaluate bound                                                              */
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
/* is atom a column ordinal?                                                  */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsOrdinalStreamTerm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = szValue();
  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];

  if ('$' == acFirst && StrLib::bIsDigit(acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is atom an index ordinal?                                                  */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsOrdinalIndexTerm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = szValue();
  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];

  if ('$' == acFirst && ('I' == acSecond || 'i' == acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this atom an separator?                                                 */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsSeparator(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (szValue()->bSame(","_Stz))
    return true;
  else
    return false;
}


/*----------------------------------------------------------------------------*/
/* get ordinal column value                                                   */
/*----------------------------------------------------------------------------*/
i8 Card_Term::iOrdinalStreamColNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_Ord = StrLib::iGleanInt(szValue()->azChar());

  return i_Ord;
}

/*----------------------------------------------------------------------------*/
/* get ordinal index value                                                    */
/*----------------------------------------------------------------------------*/
i8 Card_Term::iOrdinalIndexColNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_Ord = StrLib::iGleanInt(szValue()->azChar());

  return i_Ord;
}

/*----------------------------------------------------------------------------*/
/* evaluate relational op                                                     */
/*----------------------------------------------------------------------------*/
eRO Card_Term::eRelOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = this->sz();

  if (szTerm->bSame("<"_Stz))       return RO_LESS_THAN;
  else if (szTerm->bSame("<="_Stz))      return RO_LESS_EQUAL;
  else if (szTerm->bSame("="_Stz))       return RO_EQUAL_TO;
  else if (szTerm->bSame("=="_Stz))      return RO_EQUAL_TO;
  else if (szTerm->bSame("!="_Stz))      return RO_NOT_EQUAL;
  else if (szTerm->bSame(">="_Stz))      return RO_GREATER_EQUAL;
  else if (szTerm->bSame(">"_Stz))       return RO_GREATER_THAN;

  else return RO_NONE;
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
