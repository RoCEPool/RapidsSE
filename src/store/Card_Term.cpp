/*============================================================================*/
/* Card_Term.cpp -- Term                                                      */
/*============================================================================*/

#include "Card_Term.hpp"
#include "Card_IdxColList.hpp"
#include "Card_IdxCol.hpp"
#include "CardIter.hpp"

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
/* find sub card with given value and replace                                 */
/*----------------------------------------------------------------------------*/
bool Card_Term::bReplaceValueOfSub(CardIter* pCI, Card* pCard, eCK eSub,
                              Stz* szValue, Stz* szWith)
{
  _static_

  bool bReplaced = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI, szValue, szWith, &bReplaced](Card *pCard)
  {
    pCI->G.p_term = _pCard_Term(pCard);

    if (! pCI->G.p_term->bIsValue(szValue))
      return C_CONTINUE_RECURSION;

    pCI->G.p_term->vValue(szWith);
    bReplaced = true;

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCard->bApplyRecursive(eSub, bLambdaFn);

  return bReplaced;
}

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
/* convert term (if stream-column) to table-column                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vConvert_stmColOdn_To_tblColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  ASSERT (   pCI->G.p_MainCard->bIsCard(CK_AGGREG)
          || pCI->G.p_MainCard->bIsCard(CK_FILTER)
          || pCI->G.p_MainCard->bIsCard(CK_GBYAGG)
          || pCI->G.p_MainCard->bIsCard(CK_PROJECT)
          || pCI->G.p_MainCard->bIsCard(CK_WHERE));

  if (CK_AGGREG  == pCI->G.e_MainCard ||
      CK_FILTER  == pCI->G.e_MainCard ||
      CK_GBYAGG  == pCI->G.e_MainCard ||
      CK_PROJECT == pCI->G.e_MainCard) {
    pCI->G.p_level = pCI->G.p_MainCard->pSub_level();
    ASSERT(pCI->G.p_level && AL_PARTITION == pCI->G.p_level->Y_iGet(P_Level));
  }

  if (! Card::bIsStreamColOrdinal(szValue()))
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Projection* p_projection_prior = Card_Projection::pResolvePriorProjection(pCI);
  if (! p_projection_prior)
    return;

  Card_Expr* p_expr= p_projection_prior->pGetExpr(this);
  if (! p_expr)
    return;

  Card_TblColOdn* p_tblColOdn = p_expr->pTblColOdn();
  if (! p_tblColOdn)
    return;

   vSetValueMark1(p_tblColOdn->szValue(), MARK_CHANGED);
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
      vTrace("%s(%lld): unexpected eEvalRelnOp %lld %s\n"_Stz
          ,__func__
          ,(i8)__LINE__
          ,(i8)eRelOp
          ,aRO[eRelOp]);
      RASSERT(0);
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* evaluate relational op                                                     */
/*----------------------------------------------------------------------------*/
eRO Card_Term::eEvalRelOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szValue = this->szValue();

  eRO eRelOp = Card::eEvalRelnOp(szValue);

  return eRelOp;
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
/* get table synonym c (i.e. $$c.n)                                           */
/*----------------------------------------------------------------------------*/
char Card_Term::cGetTableSyn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = this->sz();

  char cSyn = szTerm->azChar()[1];

  return cSyn;
}

/*----------------------------------------------------------------------------*/
/* is board name                                                              */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsBoardNm(STORE::CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szValue_term = szValue();
  if (pCI->G_bIsBoard(szValue_term))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this term a parameter ordinal? %n                                       */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsCorPrmOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool b = Card::bIsCorrParamOrdinal(this->szValue());

  return b;
}

/*----------------------------------------------------------------------------*/
/* is this term the fold expr?                                                */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsFoldExpr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bIsValue("$1"_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this term a key ordinal? $Kn                                            */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsKeyOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool b = Card::bIsKeyOrdinal(this->szValue());

  return b;
}


/*----------------------------------------------------------------------------*/
/* is this term a stream-column? $n                                           */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsStmColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool b = Card::bIsStreamColOrdinal(this->szValue());

  return b;
}

/*----------------------------------------------------------------------------*/
/* is this term a table-column? $n.c                                          */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsTblColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool b = Card::bIsTableColOrdinal(this->szValue());

  return b;
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vReplace_sqCall_With_sqResult(Stz* szStmColOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValue(szStmColOdn);
}

/*----------------------------------------------------------------------------*/
/* evaluate term                                                              */
/*----------------------------------------------------------------------------*/
eTE Card_Term::eTermEval(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szValue = this->szValue();

       if (szValue->bSame("and"_Stz))        return TE_AND;
  else if (szValue->bSame("or"_Stz))         return TE_OR;
  else if (szValue->bSame("not"_Stz))        return TE_NOT;

  else if (szValue->bSame("}"_Stz))          return TE_EXECUTE;
  else if (szValue->bSame("exists"_Stz))     return TE_EXISTS;
  else if (szValue->bSame("in"_Stz))         return TE_IN;
  else if (szValue->bSame("notExists"_Stz))  return TE_NOTEXISTS;
  else if (szValue->bSame("notIn"_Stz))      return TE_NOTIN;

  else if (Card::bIsRelnOp(szValue))         return TE_RELNOP;

  else if (Card::bIsBoolFn(szValue))         return TE_BOOLFN;

  else return TE_OTHER;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
