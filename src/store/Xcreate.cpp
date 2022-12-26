/*============================================================================*/
/* Xcreate.cpp --                                                       */
/*============================================================================*/

#include "Xcreate.hpp"

#include "Table.hpp"
#include "PtnDef.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Xcreate)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xcreate::Xcreate(eCN eClass)
: Xverify(eClass)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xcreate::~Xcreate(void) noexcept(false)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create a bound given cmpPred                                               */
/*----------------------------------------------------------------------------*/
Card_Bound* Xcreate::C_pBound_CreateFrom_cmpPred(Card_CmpPred* p_cmpPred,
                                                   char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Term* p_term;

  Card_Bound* p_bound = C_pBound_New(p_cmpPred->szValue());

  Card_Term* pTerm_0 = _pCard_Term(p_cmpPred->pGetChild(0));
  Card_Term* pTerm_1 = _pCard_Term(p_cmpPred->pGetChild(1));

  if (Card::bIsTableColOrdinal(pTerm_0->szValue())) {
    char cTblSyn_term_0 = Card::cTableColSyn(pTerm_0->szValue());
    if (cTblSyn_term_0 == cTblSyn) {
      p_term = new Card_Term("$I1"_Stz);
      p_bound->vAppend(p_term);
      p_term = new Card_Term(pTerm_1->sz());
      p_bound->vAppend(p_term);
    }
    else
    if (Card::bIsTableColOrdinal(pTerm_1->szValue())) {
      char cTblSyn_term_1 = Card::cTableColSyn(pTerm_1->szValue());
      if (cTblSyn_term_1 == cTblSyn) {
        p_term = new Card_Term("$I1"_Stz);
        p_bound->vAppend(p_term);
        p_term = new Card_Term(pTerm_0->sz());
        p_bound->vAppend(p_term);
      } else {
        NEVER_REACH
      }
    }
  }
  else
  if (Card::bIsTableColOrdinal(pTerm_1->szValue())) {
    char cTblSyn_term_1 = Card::cTableColSyn(pTerm_1->szValue());
    if (cTblSyn_term_1 == cTblSyn) {
      p_term = new Card_Term("$I1"_Stz);
      p_bound->vAppend(p_term);
      p_term = new Card_Term(pTerm_0->sz());
      p_bound->vAppend(p_term);
    }
    else {
      NEVER_REACH
    }
  }
  else {
    NEVER_REACH
  }

  p_term = new Card_Term(p_cmpPred->sz());
  p_bound->vAppend(p_term);

  return p_bound;
}

/*----------------------------------------------------------------------------*/
/* create a bound given cmpPred                                               */
/*----------------------------------------------------------------------------*/
Card_Bound* Xcreate::C_pBound_New(Stz* szRelnOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Stz* szBound = Card::szBound(szRelnOp);
  Card_Term* p_term_bound = new Card_Term(szBound);

  if (szRelnOp->bSame(">"_Stz) || szRelnOp->bSame(">="_Stz)) {
   Card_LBound* p_lBound =  new Card_LBound;
   p_lBound->vAppend(p_term_bound);
   return p_lBound;
  }

  if (szRelnOp->bSame("<"_Stz) || szRelnOp->bSame("<="_Stz)) {
    Card_UBound* p_uBound =  new Card_UBound;
    p_uBound->vAppend(p_term_bound);
    return p_uBound;
  }

  if (szRelnOp->bSame("=="_Stz)) {
    Card_EBound* p_eBound =  new Card_EBound;
    p_eBound->vAppend(p_term_bound);
    return p_eBound;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* create a bound given pred                                                  */
/*----------------------------------------------------------------------------*/
Card_Bound* Xcreate::C_pBound_CreateFrom_PRED(Card_PRED* p_PRED, char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // todo: hack

  Card_CmpPred* p_cmpPred = _pCard_CmpPred(p_PRED);

  Card_Bound* p_bound = C_pBound_CreateFrom_cmpPred(p_cmpPred,cTblSyn);

  return p_bound;
}


/*----------------------------------------------------------------------------*/
/* create a bounds given tblColOdn                                            */
/*----------------------------------------------------------------------------*/
Card_Bounds* Xcreate::C_pBounds_CreateFrom_PRED(Card_PRED* p_PRED,
                                                char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Bound* p_bound;
  Card_CmpPred* p_cmpPred;

  Card_Bounds* p_bounds = new Card_Bounds;

  if (p_PRED->bIsCard(CK_CMPPRED)) {
    p_bound = C_pBound_CreateFrom_PRED(p_PRED,cTblSyn);
    p_bounds->vAppend(p_bound);
  }
  else
  if (p_PRED->bIsCard(CK_ANDPRED)) {
    p_cmpPred = _pCard_CmpPred(p_PRED->pGetChild(0));
    p_bound = C_pBound_CreateFrom_PRED(p_cmpPred,cTblSyn);
    p_bounds->vAppend(p_bound);

    p_cmpPred = _pCard_CmpPred(p_PRED->pGetChild(1));
    p_bound = C_pBound_CreateFrom_PRED(p_cmpPred,cTblSyn);
    p_bounds->vAppend(p_bound);
  }
  else {
    NEVER_REACH
  }

  return p_bounds;
}
/*----------------------------------------------------------------------------*/
//Card_Bounds* Xcreate::C_pBounds_CreateFrom_PRED(Card_PRED* p_PRED,
//                                                char cTblSyn)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
//
//  // todo: hack
//
//  Card_CmpPred* p_cmpPred = _pCard_CmpPred(p_PRED);
//
//  Card_Bounds* p_bounds = C_pBounds_CreateFrom_cmpPred(p_cmpPred,cTblSyn);
//
//  return p_bounds;
//}

/*----------------------------------------------------------------------------*/
/* create a bounds given tblColOdn                                            */
/*----------------------------------------------------------------------------*/
Card_Bounds* Xcreate::C_pBounds_Create_Eq_ForGivenCol(Stz* szTblColOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Bounds* p_bounds = new Card_Bounds;

  Card_Term* p_term = new Card_Term("BOUND"_Stz);
  p_bounds->vAppend(p_term);

  p_term = new Card_Term("$I1"_Stz);
  p_bounds->vAppend(p_term);
  p_term = new Card_Term(szTblColOdn);
  p_bounds->vAppend(p_term);
  p_term = new Card_Term("=="_Stz);
  p_bounds->vAppend(p_term);

  return p_bounds;
}

/*----------------------------------------------------------------------------*/
/* create a bounds given tblColOdn                                            */
/*----------------------------------------------------------------------------*/
Card_Bounds* Xcreate::C_pBounds_CreateFrom_cmpPred(Card_CmpPred* p_cmpPred,
                                                   char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Term* p_term;
  Stz* szBound;

  Card_Bounds* p_bounds = new Card_Bounds;


  Card_Term* pTerm_0 = _pCard_Term(p_cmpPred->pGetChild(0));
  Card_Term* pTerm_1 = _pCard_Term(p_cmpPred->pGetChild(1));

  if (Card::bIsTableColOrdinal(pTerm_0->szValue())) {
    char cTblSyn_term_0 = Card::cTableColSyn(pTerm_0->szValue());
    if (cTblSyn_term_0 == cTblSyn) {
      szBound = Card::szBound(pTerm_1->sz());
      p_term = new Card_Term(szBound);
      p_bounds->vAppend(p_term);
      p_term = new Card_Term("$I1"_Stz);
      p_bounds->vAppend(p_term);
      p_term = new Card_Term(pTerm_1->sz());
      p_bounds->vAppend(p_term);
    }
    else
    if (Card::bIsTableColOrdinal(pTerm_1->szValue())) {
      char cTblSyn_term_1 = Card::cTableColSyn(pTerm_0->szValue());
      if (cTblSyn_term_1 == cTblSyn) {
        szBound = Card::szBound(pTerm_1->sz());
        p_term = new Card_Term(szBound);
        p_bounds->vAppend(p_term);
        p_term = new Card_Term("$I1"_Stz);
        p_bounds->vAppend(p_term);
        p_term = new Card_Term(pTerm_0->sz());
        p_bounds->vAppend(p_term);
      } else {
        NEVER_REACH
      }
    }
  }
  else
  if (Card::bIsTableColOrdinal(pTerm_1->szValue())) {
    char cTblSyn_term_1 = Card::cTableColSyn(pTerm_1->szValue());
    if (cTblSyn_term_1 == cTblSyn) {
      szBound = Card::szBound(pTerm_1->sz());
      p_term = new Card_Term(szBound);
      p_bounds->vAppend(p_term);
      p_term = new Card_Term("$I1"_Stz);
      p_bounds->vAppend(p_term);
      p_term = new Card_Term(pTerm_0->sz());
      p_bounds->vAppend(p_term);
    }
    else {
      NEVER_REACH
    }
  }
  else {
    NEVER_REACH
  }

  p_term = new Card_Term(p_cmpPred->sz());
  p_bounds->vAppend(p_term);

  return p_bounds;
}

/*----------------------------------------------------------------------------*/
/* create bounds under table from filter                                      */
/*----------------------------------------------------------------------------*/
void Xcreate::C_vBounds_DeriveFrom_filter(Card_Table* p_table,
                                          Card_Filter* p_filter,
                                          bool bParamAllowedInBounds)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_BoolCalc* p_boolCalc = p_filter->pSub_boolCalc();
  RASSERT(1 == p_boolCalc->nChildCards());

  Card_PRED* p_pred_ofFilter_ROOT = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  // TODO: this is a hack to put bounds under one and only one index

  Card_IndexList* p_indexList = p_table->pSub_indexList();
  Card_Index* p_index = p_table->pSub_index();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this,p_table,p_index,bParamAllowedInBounds](Card_PRED* p_pred_Of_filter, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred_Of_filter->bIsCard(CK_CMPPRED))
      return C_CONTINUE_RECURSION;

    Card_CmpPred* p_cmpPred = _pCard_CmpPred(p_pred_Of_filter);

    //if (!T_bCmpPred_MaybeBounds(p_cmpPred,p_table,bParamAllowedInBounds))
    if ( ! p_cmpPred->bMaybeBounds_v2(p_table,bParamAllowedInBounds) )
      return C_CONTINUE_RECURSION;

    // - - - -

    Card_Bounds* p_bounds = p_table->pSub_bounds();
    if (! p_bounds) {
      p_bounds = new Card_Bounds;
      p_index->vAppend(p_bounds);
    }

    char cTblSyn = p_table->cValue();
    Card_Bound* p_bound = C_pBound_CreateFrom_cmpPred(p_cmpPred,cTblSyn);
    p_bounds->vAppend(p_bound);

    p_cmpPred->vMark1(MARK_UP);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ofFilter_ROOT->bPredRecursive(bLambdaFn);

}
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_StmColList* Xcreate::C_pStmColList_DeriveFrom_ELIST(Card_ELIST* p_ELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_StmColList* p_stmColList = new Card_StmColList;

  CardIter xCI(p_ELIST);
  while (Card_Expr* p_expr = _pCard_Expr(xCI.pEachChild(CK_EXPR))) {

    Card *p_type = p_expr->pFindSubCard(CK_TYPE);
    Card *p_as = p_expr->pFindSubCard(CK_AS);

    Card_StmCol* pCard_StreamCol = new Card_StmCol;
    p_stmColList->vAppend(pCard_StreamCol);

    if (p_as)
      p_as = _pCard_As(p_as->pDupCardAndSubCards());
    else {
      s32 sAs;
      sAs.vSprint("$%lld"_Stz,xCI.i_Child() + 1);
      p_as = new Card_As(sAs);
    }

    pCard_StreamCol->vAppend(p_as);
    pCard_StreamCol->vAppend(p_type->pDupCardAndSubCards());
  }

  return p_stmColList;
}

/*----------------------------------------------------------------------------*/
/* convert <rpnCalc> to <boolCalc>                                            */
/*----------------------------------------------------------------------------*/
Card_BoolCalc* Xcreate::C_pConvert_rpnCalc_To_boolCalc(Card_RpnCalc* p_rpnCalc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_BoolCalc* p_boolCalc = new Card_BoolCalc;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // does the rpn calc have a when in it?

  if (p_rpnCalc->bHasWhen()) {

    Card_BolPred*p_bolPred = new Card_BolPred;
    p_bolPred->vTransferChildren(p_rpnCalc);

    p_boolCalc->vAppend(p_bolPred);

    return p_boolCalc;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Stz* szSqyNm;
  Card_AndPred* p_andPred = nullptr;
  Card_BolPred* p_bolPred = nullptr;
  Card_CmpPred* p_cmpPred = nullptr;
  Card_NotPred* p_notPred = nullptr;
  Card_OrPred*  p_orPred  = nullptr;
  Card_RoPred*  p_roPred  = nullptr;

  Card*  p_childA  = nullptr;
  Card*  p_childB  = nullptr;

  bool bSawSqy = false;

  Card_Term*    p_subQueryNm = nullptr;

  Card_RpnTerms*  p_TermSeries = new Card_RpnTerms;

  while (p_rpnCalc->nChildCards()) {
    Card_Term* p_term_next = _pCard_Term(p_rpnCalc->pPop());

    eTE eTerm_next = p_term_next->eTermEval();

    //vTrace("%s %s\n"_Stz, aTE[eTerm_next], p_term_next->szValue());

    switch (eTerm_next) {

      case TE_AND:
        // <boolCalc> = <and>/<boolCalc>
        p_childB = p_boolCalc->pPlopChild();
        p_childA = p_boolCalc->pPlopChild();

        p_andPred = new Card_AndPred;
        p_andPred->vAppend(p_childA);
        p_andPred->vAppend(p_childB);

        p_boolCalc->vAppend(p_andPred);
        p_andPred = nullptr;
        p_childA  = nullptr;
        p_childB  = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolCalc->vDump(0,"AND"_Stz);
        break;

      case TE_OR:
        // <boolCalc> = <or>/<boolCalc>
        p_childB = p_boolCalc->pPlopChild();
        p_childA = p_boolCalc->pPlopChild();

        p_orPred = new Card_OrPred;
        p_orPred->vAppend(p_childA);
        p_orPred->vAppend(p_childB);

        p_boolCalc->vAppend(p_orPred);
        p_orPred = nullptr;
        p_childA = nullptr;
        p_childB = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolCalc->vDump(0,"OR"_Stz);
        break;

      case TE_NOT:
        p_childA = p_boolCalc->pPlopChild();

        p_notPred = new Card_NotPred;
        p_notPred->vAppend(p_childA);

        p_boolCalc->vAppend(p_notPred);
        p_notPred = nullptr;
        p_childA  = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolCalc->vDump(0,"NOT"_Stz);
        break;

      case TE_COMPARE:
        // <boolCalc> = <cmpPred><termSeries><boolCalc>
        RASSERT(p_TermSeries->nChildCards() > 0);
        p_cmpPred = new Card_CmpPred;
        p_cmpPred->vValue(p_term_next->sz());
        p_cmpPred->vTransferChildren(p_TermSeries);
        if (p_cmpPred->bContains_Sqy_Call())
          p_cmpPred->vCard(CK_ROPRED);
        //p_boolCalc->vPush(p_cmpPred);
        p_boolCalc->vAppend(p_cmpPred);
        p_cmpPred = nullptr;
        DELETE_CARD(p_term_next);
        bSawSqy = false;
        //p_boolCalc->vDump(0,"COMPARE"_Stz);
        break;

      case TE_EXISTS:
      case TE_NOTEXISTS:
        // <boolCalc> = <exists><boolCalc>
        p_roPred = new Card_RoPred;
        p_roPred->vValue(p_term_next->sz());
        p_roPred->vTransferChildren(p_TermSeries);
        p_boolCalc->vAppend(p_roPred);
        p_roPred = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolCalc->vDump(0,"EXISTS"_Stz);
        break;

      case TE_IN:
       // <boolCalc> = <in><boolCalc>
        if (p_TermSeries->bIsInList()) {
          p_bolPred = new Card_BolPred;
          p_bolPred->vValue(p_term_next->sz());
          p_bolPred->vTransferChildren(p_TermSeries);
          p_boolCalc->vAppend(p_bolPred);
          p_bolPred = nullptr;
          DELETE_CARD(p_term_next);
          //p_boolCalc->vDump(0,"BOOLFN/IN"_Stz);
        }
        else {
          p_roPred = new Card_RoPred;
          p_roPred->vValue(p_term_next->sz());
          p_roPred->vTransferChildren(p_TermSeries);
          p_boolCalc->vAppend(p_roPred);
          p_roPred = nullptr;
          DELETE_CARD(p_term_next);
          //p_boolCalc->vDump(0,"IN"_Stz);
        }
        break;

      case TE_NOTIN:
        // <boolCalc> = <in><boolCalc>
        if (p_TermSeries->bIsInList()) {
          p_bolPred = new Card_BolPred;
          p_bolPred->vValue("in"_Stz);
          p_bolPred->vTransferChildren(p_TermSeries);
          p_boolCalc->vAppend(p_bolPred);
          p_bolPred = nullptr;
          DELETE_CARD(p_term_next);

          p_childA = p_boolCalc->pPlopChild();
          p_notPred = new Card_NotPred;
          p_notPred->vAppend(p_childA);
          p_boolCalc->vAppend(p_notPred);
          p_notPred = nullptr;
          p_childA  = nullptr;
          //p_boolCalc->vDump(0,"BOOLFN/NOTIN"_Stz);
        }
        else {
          p_roPred = new Card_RoPred;
          p_roPred->vValue(p_term_next->sz());
          p_roPred->vTransferChildren(p_TermSeries);
          p_boolCalc->vAppend(p_roPred);
          p_roPred = nullptr;
          DELETE_CARD(p_term_next);
          //p_boolCalc->vDump(0,"NOTIN"_Stz);
        }
        break;

      case TE_BOOLFN:
        p_bolPred = new Card_BolPred;
        p_bolPred->vValue(p_term_next->sz());
        p_bolPred->vTransferChildren(p_TermSeries);
        p_boolCalc->vAppend(p_bolPred);
        p_bolPred = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolCalc->vDump(0,"BOOLFN"_Stz);
        break;

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // <termSeries>

      case TE_EXECUTE:
        DELETE_CARD(p_term_next);
        break;

      case TE_OTHER:
        if (StrLib::bBeginsWith(p_term_next->szValue()->azChar(), "__"))
          bSawSqy = true;
        p_TermSeries->vAppend(p_term_next);
        //p_TermSeries->vDump(0,"TERM_SERIES"_Stz);
        break;

      default:
        NEVER_REACH;
        break;
    }
  }

  if (p_TermSeries->nItems()) {
    RASSERT(0 == p_boolCalc->nChildCards());
    p_boolCalc->vDump(0,"boolCalc"_Stz);
    p_TermSeries->vDump(0,"termSeries"_Stz);

    p_bolPred = new Card_BolPred;
    p_bolPred->vTransferChildren(p_TermSeries);

    p_boolCalc->vAppend(p_bolPred);
  }

  RASSERT(0 == p_TermSeries->nChildCards());
  delete p_TermSeries;

  //p_boolCalc->vDump();

  return p_boolCalc;
}

/*----------------------------------------------------------------------------*/
/* create a filter from/duplicating given PRED                                */
/*----------------------------------------------------------------------------*/
Card_Filter* Xcreate::C_pFilter_From_PRED(Card_PRED* p_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Filter* p_filter = new Card_Filter;

  Card_BoolCalc* p_boolCalc = new Card_BoolCalc;
  p_filter->vAppend(p_boolCalc);

  p_boolCalc->vAppend(p_PRED->pDupCardAndSubCards());

  return p_filter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
