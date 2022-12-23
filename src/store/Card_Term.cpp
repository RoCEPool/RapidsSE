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

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Term::Card_Term(void)
    : Card(CK_TERM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Term::Card_Term(Stz* szTerm)
: Card(CK_TERM,szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* evaluate bound                                                             */
/*----------------------------------------------------------------------------*/
ePB Card_Term::eEvalBound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eRO eRelOp = this->eEvalRelOp();

  switch (eRelOp) {
    case RO_LESS_THAN:     return PB_UBOUND;
    case RO_LESS_EQUAL:    return PB_UBOUND;
    case RO_EQUAL_TO:      return PB_EBOUND;
    case RO_GREATER_EQUAL: return PB_LBOUND;
    case RO_GREATER_THAN:  return PB_LBOUND;

    default:
      vTrace("%s(%lld): unexpected eEvalRelOp %lld %s\n"_Stz
          ,__func__
          ,(i8)__LINE__
          ,(i8)eRelOp
          ,aRO[eRelOp]);
      RASSERT(0);
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* get ordinal number                                                         */
/*----------------------------------------------------------------------------*/
i8 Card_Term::iGetOdnNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_Ord = StrLib::iGleanInt(this->szValue()->azChar());

  return i_Ord;
}

/*----------------------------------------------------------------------------*/
/* evaluate relational op                                                     */
/*----------------------------------------------------------------------------*/
eRO Card_Term::eEvalRelOp(void)
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
/* get table synonym c (i.e. $$c.n)                                           */
/*----------------------------------------------------------------------------*/
char Card_Term::cGetTableSyn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

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

  if (szTerm->bSame("and"_Stz))        return TE_AND;
  else if (szTerm->bSame("or"_Stz))         return TE_OR;
  else if (szTerm->bSame("not"_Stz))        return TE_NOT;

  else if (szTerm->bSame("}"_Stz))          return TE_EXECUTE;
  else if (szTerm->bSame("exists"_Stz))     return TE_EXISTS;
  else if (szTerm->bSame("in"_Stz))         return TE_IN;
  else if (szTerm->bSame("notExists"_Stz))  return TE_NOTEXISTS;
  else if (szTerm->bSame("notIn"_Stz))      return TE_NOTIN;

  else if (szTerm->bSame("<"_Stz))          return TE_COMPARE;
  else if (szTerm->bSame("<="_Stz))         return TE_COMPARE;
  else if (szTerm->bSame("="_Stz))          return TE_COMPARE;
  else if (szTerm->bSame("=="_Stz))         return TE_COMPARE;
  else if (szTerm->bSame("!="_Stz))         return TE_COMPARE;
  else if (szTerm->bSame("<>"_Stz))         return TE_COMPARE;
  else if (szTerm->bSame(">="_Stz))         return TE_COMPARE;
  else if (szTerm->bSame(">"_Stz))          return TE_COMPARE;

  else if (szTerm->bSame("between"_Stz))         return TE_BOOLFN;
  else if (szTerm->bSame("matchLeading"_Stz))    return TE_BOOLFN;
  else if (szTerm->bSame("matchMiddle"_Stz))     return TE_BOOLFN;
  else if (szTerm->bSame("matchMiddle1"_Stz))    return TE_BOOLFN;
  else if (szTerm->bSame("matchTrailing"_Stz))   return TE_BOOLFN;

  else return TE_OTHER;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
