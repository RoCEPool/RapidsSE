/*============================================================================*/
/* Card_Term.cpp -- Term                                                      */
/*============================================================================*/

#include "Card_Term.hpp"
#include "Card_IdxColList.hpp"
#include "Card_IdxCol.hpp"

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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vBump_KeyOrdinal(i8 nBy)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 iKeyOdn = Card::iGetColOdn(this->szValue());

  this->vSetKeyOrdinal(iKeyOdn+nBy);

  if (this->Y_bHas(P_BumpedBy)) {
    i8 nBumpedBy = this->Y_iGet(P_BumpedBy);
    this->Y_vSet(P_BumpedBy,nBumpedBy+nBy);
  }
  else {
    this->Y_vSet(P_BumpedBy,(i8)nBy);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vBump_StreamColOrdinal(i8 nBy)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 iStmColOdn = Card::iGetColOdn(this->szValue());

  this->vSetStreamColOrdinal(iStmColOdn+nBy);

  if (this->Y_bHas(P_BumpedBy)) {
    i8 nBumpedBy = this->Y_iGet(P_BumpedBy);
    this->Y_vSet(P_BumpedBy,nBumpedBy+nBy);
  }
  else {
    this->Y_vSet(P_BumpedBy,(i8)nBy);
  }
}

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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vMarkIfMatchesAn_idxCol(Card_IdxColList* p_idxColList)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Stz* szTerm = this->sz();
  if (!szTerm->bIsStreamColOrd())
    return;

  i8 i_IdxCol = C_UNKNOWN;

  Card_IdxCol* p_idxCol =
      _pCard_IdxCol(p_idxColList->pFindChild(CK_IDXCOL, szTerm, i_IdxCol));
  if (! p_idxCol)
    return;

  this->vMark1('I');
  this->vMark2(i_IdxCol);
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
  else if (szTerm->bSame("isNull"_Stz))          return TE_BOOLFN;
  else if (szTerm->bSame("like2"_Stz))           return TE_BOOLFN;
  else if (szTerm->bSame("like3"_Stz))           return TE_BOOLFN;
  else if (szTerm->bSame("matchLeading"_Stz))    return TE_BOOLFN;
  else if (szTerm->bSame("matchMiddle"_Stz))     return TE_BOOLFN;
  else if (szTerm->bSame("matchMiddle1"_Stz))    return TE_BOOLFN;
  else if (szTerm->bSame("matchTrailing"_Stz))   return TE_BOOLFN;

  else return TE_OTHER;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
