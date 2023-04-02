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
//Card_Term::Card_Term(Card_Term* p_term_arg)
//: Card(CK_TERM,p_term_arg->szValue())
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
//
//  _checkInitialised_(CN_CARD);
//
//  Y_vAll(p_term_arg);
//
//  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
//}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Term::Card_Term(Stz* szTerm, Stz* szStem)
: Card(CK_TERM,szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  if (szStem)
    pSymVal()->Y_vSet(P_TblCol, szStem);
  
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
/* find sub card with given value and replace                                 */
/*----------------------------------------------------------------------------*/
bool Card_Term::bUpdateTermOfSub(CardIter* pCI, Card* pCard, eCK eSub,
                                   Stz* szValue, Card_Term* p_term_stamp)
{
  _static_

  bool bReplaced = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI, szValue, p_term_stamp, &bReplaced](Card *pCard)
  {
    pCI->G.p_term = _pCard_Term(pCard);

    if (! pCI->G.p_term->bIsValue(szValue))
      return C_CONTINUE_RECURSION;

    pCI->G.p_term->vValue(p_term_stamp->szValue());

    pCI->G.p_term->pSymVal()->Y_vAll(p_term_stamp);

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

  i8 iKeyOdn = Symbol::i_GetColOdn(this->szValue());

  this->pSymVal()->vSetKeyOrdinal(iKeyOdn+nBy);

  if (this->pSymVal()->Y_bHas(P_BumpedBy)) {
    i8 nBumpedBy = this->pSymVal()->Y_iGet(P_BumpedBy);
    this->pSymVal()->Y_vSet(P_BumpedBy,nBumpedBy+nBy);
  }
  else {
    this->pSymVal()->Y_vSet(P_BumpedBy,(i8)nBy);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vBump_StreamColOrdinal(i8 nBy)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 iStmColOdn = Symbol::i_GetColOdn(this->szValue());

  this->pSymVal()->vSetStreamColOrdinal(iStmColOdn+nBy);

  if (this->pSymVal()->Y_bHas(P_BumpedBy)) {
    i8 nBumpedBy = this->pSymVal()->Y_iGet(P_BumpedBy);
    this->pSymVal()->Y_vSet(P_BumpedBy,nBumpedBy+nBy);
  }
  else {
    this->pSymVal()->Y_vSet(P_BumpedBy,(i8)nBy);
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
    ASSERT(pCI->G.p_level && AL_PARTITION == pCI->G.p_level->pSymVal()->Y_iGet(P_Level));
  }

  if (! Symbol::b_IsStreamColOrdinal(szValue()))
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (Card_Expr* p_expr_prior = pCI->pResolvePriorExpr_With_ProjectedCol(this))
    if (Stz* szPrjCol = p_expr_prior->szProjectedCol())
      vValueMark1(szPrjCol, MARK_CHANGED);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vEnsure_tblCol_flip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Symbol::b_IsTableColOrdinal(szValue()))
    return;

  RASSERT(pSymVal()->Y_bHas(P_TblCol));

  vValue(pSymVal()->Y_szGet(P_TblCol));

  pSymVal()->Y_vCut(P_TblCol);
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

  eRO eRelOp = Symbol::e_EvalRelnOp(szValue);

  return eRelOp;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Term::pExpr_For_col(CardIter* pCI, i8 i_Before)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (C_UNKNOWN == i_Before)
    i_Before = pCI->i_Child();

  Card_Expr* p_expr_col = nullptr;

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card *pMain = pCI->G.pDeck->pGetChild(i_Main);

    //pMain->vDump();

    if (Card_Projection* p_projection = pMain->pSub_projection()) {
      i8 nChildren_prj = p_projection->nChildCards();
      for (i8 i_Child = 0; i_Child < nChildren_prj; i_Child++) {
        Card *pChild_prj = p_projection->pGetChild(i_Child);
        if (pChild_prj->bIsCard(CK_EXPR)) {
          Card_Expr* p_expr_prj = _pCard_Expr(pChild_prj);
          if (Card_RpnCalc* p_rpnCalc = p_expr_prj->pSub_rpnCalc()) {
            //p_rpnCalc->vDump();
            if (1 == p_rpnCalc->nChildCards() &&
                p_rpnCalc->szProjectedCol()->bSame(this->szValue())) {
              p_expr_col = p_expr_prj;
              break;
            }
          }
        }
      }

    }
  }

  return p_expr_col;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Term::pExpr_For_colNm(CardIter* pCI, i8 i_Before)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (C_UNKNOWN == i_Before)
    i_Before = pCI->i_Child();

  Card_Expr* p_expr_col = nullptr;

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card *pMain = pCI->G.pDeck->pGetChild(i_Main);

    if (Card_Projection* p_projection = pMain->pSub_projection()) {
      i8 nChildren_prj = p_projection->nChildCards();
      for (i8 i_Child = 0; i_Child < nChildren_prj; i_Child++) {
        Card *pChild_prj = p_projection->pGetChild(i_Child);
        if (pChild_prj->bIsCard(CK_EXPR)) {
          Card_Expr* p_expr_prj = _pCard_Expr(pChild_prj);
          if (Card_As* p_as = p_expr_prj->pSub_as()) {
            //p_as->vDump();
            if (p_as->bIsValue(this->szValue())) {
              p_expr_col = p_expr_prj;
              break;
            }
          }
        }
      }

    }
  }

  return p_expr_col;
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

  Stz* szTerm = this->szValue();

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
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Term::pClone(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return _pCard_Term(pDupCardRecursive());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Term::pClone(Stz* szDiffValue)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Term* p_term_clone = pClone();

  p_term_clone->vValue(szDiffValue);

  return p_term_clone;
}

/*----------------------------------------------------------------------------*/
/* is this term a col ordinal? $n or $c.n                                     */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bIsStmColOdn())
    return true;
  else
  if (bIsTblColOdn())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this term a parameter ordinal? %n                                       */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsCorPrmOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool b = Symbol::b_IsCorrParamOrdinal(this->szValue());

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

  bool b = Symbol::b_IsKeyOrdinal(this->szValue());

  return b;
}

/*----------------------------------------------------------------------------*/
/* is                                                                         */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsSame(Card_Term* p_term_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool b = bIsValue(p_term_arg->szValue());

  return b;
}


/*----------------------------------------------------------------------------*/
/* is this term a stream-column? $n                                           */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsStmColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool b = Symbol::b_IsStreamColOrdinal(this->szValue());

  return b;
}

/*----------------------------------------------------------------------------*/
/* is this term a table-column? $n.c                                          */
/*----------------------------------------------------------------------------*/
bool Card_Term::bIsTblColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Symbol::b_IsTableColOrdinal(szValue()))
    return true;

  if (pSymVal()->Y_bHas(P_TblCol) && Symbol::b_IsTableColOrdinal(pSymVal()->Y_szGet(P_TblCol)))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! pSymVal()->Y_bHas(P_Column)) {
    vPrintCard_bySettings(pRibbon, iDepth);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  s256 sTerm = szValue();
  s32 sType_short = pSymVal()->Y_szGet(P_Type);

  pRibbon->vSprintRight("%s %s"_Stz,sTerm.szStz(),sType_short.szStz());

  //vPrintCard_bySettings(pRibbon, iDepth);
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Term::vFlip(Card_Projection* p_projection_needed)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Expr* p_expr_needed = p_projection_needed->pFind_expr_By_projectedCol(szValue());
  if (p_expr_needed) {
    Stz* szStem = szValue();
    Stz* szBecomes = p_expr_needed->szValue();

    pSymVal()->Y_vSet(P_TblCol, szStem);
    vValue(szBecomes);
  }
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

  else if (Symbol::b_IsRelnOp(szValue))         return TE_RELNOP;

  else if (Symbol::b_IsBoolFn(szValue))         return TE_BOOLFN;

  else return TE_OTHER;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
