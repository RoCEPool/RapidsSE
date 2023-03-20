/*============================================================================*/
/* Card_OP.cpp -- Abstract OP                                                 */
/*============================================================================*/

#include "Card_OP.hpp"

#include "Card_Level.hpp"
#include "Card_Table.hpp"
#include "CardIter.hpp"
#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_OP::Card_OP(eCK eCard)
: Card(eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_OP::~Card_OP(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add missing level                                                          */
/*----------------------------------------------------------------------------*/
void Card_OP::vAddLevel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Level* p_level = pSub_level();
  if (p_level)
    return;

  p_level = new Card_Level(AL_PARTITION);
  vPush(p_level);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vAdd_Pred_Under_filter(Card_PRED *p_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Filter* p_filter = this->pSub_filter();
  if (! p_filter) {
    p_filter = _pCard_Filter(this->pAppendNewCardTail(CK_FILTER));
    p_filter->vAppendNewCardTail(CK_BOOLCALC);
  }

  p_filter->vAddPred(p_PRED);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vConvert_rpnCalc_To_boolCalc(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->G.p_boolCalc = pCI->G.p_rpnCalc->pToBoolCalc(pCI);

  vReplaceSubCard(pCI->G.p_rpnCalc,pCI->G.p_boolCalc);

  pCI->G_vDeleteMember(pCI->G.p_rpnCalc);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vConvert_stmColOdn_To_tblColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! (   CK_AGGREG  == pCI->G.e_MainCard
            || CK_FILTER  == pCI->G.e_MainCard
            || CK_GBYAGG  == pCI->G.e_MainCard
            || CK_PROJECT == pCI->G.e_MainCard
            || CK_WHERE   == pCI->G.e_MainCard))
    return;

  if (CK_AGGREG  == pCI->G.e_MainCard ||
      CK_FILTER  == pCI->G.e_MainCard ||
      CK_GBYAGG  == pCI->G.e_MainCard ||
      CK_PROJECT == pCI->G.e_MainCard) {
    pCI->G.p_level = pCI->G.p_MainCard->pSub_level();
    if (pCI->G.p_level && AL_PARTITION != pCI->G.p_level->Y_iGet(P_Level))
      return;
  }

  if (CK_WHERE   == pCI->G.e_MainCard)
    vDebugPoint();

  if (CK_AGGREG  == pCI->G.e_MainCard) pCI->G.p_aggreg->vConvert_stmColOdn_terms_To_tblColOdn(pCI);
  else if (CK_FILTER  == pCI->G.e_MainCard) pCI->G.p_filter->vConvert_stmColOdn_terms_To_tblColOdn(pCI);
  else if (CK_GBYAGG  == pCI->G.e_MainCard) pCI->G.p_gbyAgg->vConvert_stmColOdn_terms_To_tblColOdn(pCI);
  else if (CK_PROJECT == pCI->G.e_MainCard) pCI->G.p_project->vConvert_stmColOdn_terms_To_tblColOdn(pCI);
  else if (CK_WHERE   == pCI->G.e_MainCard) pCI->G.p_where->vConvert_stmColOdn_terms_To_tblColOdn(pCI);
  else NEVER_REACH

}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vConvert_tblColOdn_To_stmColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI](Card* pCard)
  {
    Card_Term* p_term = _pCard_Term(pCard);

    //vTrace("CONVERT %s\n"_Stz,p_term->szValue());

    if (! Card::bIsTableColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    if (Card_Expr* p_expr_col = p_term->pExpr_For_col(pCI)) {
      p_term->vSetValueMark1(p_expr_col->szValue(), MARK_CHANGED);
      return C_CONTINUE_RECURSION;
    }

    if (! (pCI->G.p_MainCard->bIsCard(CK_KEYEXISTS) || pCI->G.p_MainCard->bIsCard(CK_KEYNOTEXISTS)))
      return C_CONTINUE_RECURSION;

    Stz* szStmColOdn;
    s32 sColOfThisTable;

    pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection();
    i8 nExpr_prior = pCI->G.p_projection->nChildCards();
    i8 i_Col = p_term->iGetOdnNo();
    sColOfThisTable.vSprint("$%lld"_Stz,nExpr_prior+i_Col);
    szStmColOdn = sColOfThisTable.szStz();
    p_term->vSetValueMark1(szStmColOdn, MARK_CHANGED);
    return C_CONTINUE_RECURSION;

  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bApplyRecursive(CK_TERM, bLambdaFn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vEarly_project(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(pCI->EarlyProject.m_pProjection_reduced);

  while (pCI->G_bEachMain_reverse()) {

    switch (pCI->G.e_MainCard) {

      // -- -- -- -- -- -- --
      case CK_GBYAGG:
        break;

      case CK_ORDAGG:
        break;

        // -- -- -- -- -- -- --
      case CK_PROJECT:
        pCI->G.p_project->vEarly_project(pCI);
        break;

      // -- -- -- -- -- -- --
      case CK_INDEXJOIN:
        pCI->G.p_indexJoin->vEarly_project(pCI);
        break;

      case CK_INDEXSCAN:
        pCI->G.p_indexScan->vEarly_project(pCI);
        break;

      case CK_SCAN:
        pCI->G.p_scan->vEarly_project(pCI);
        break;

        // -- -- -- -- -- -- --
      default:
        break;
    }

  }

  if (pCI->EarlyProject.m_pProjection_reduced) {
    DELETE_CARD(pCI->EarlyProject.m_pProjection_reduced);
  }

  pCI->vEndTransform();
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* is this op at node level                                                   */
/*----------------------------------------------------------------------------*/
bool Card_OP::bIsNodeLevel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Level* p_level = pSub_level())
    if (AL_NODE == p_level->Y_iGet(P_Level))
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this op at origin level                                                 */
/*----------------------------------------------------------------------------*/
bool Card_OP::bIsOriginLevel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Level* p_level = pSub_level())
    if (AL_ORIGIN == p_level->Y_iGet(P_Level))
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this op at partition level                                              */
/*----------------------------------------------------------------------------*/
bool Card_OP::bIsPartitionLevel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Level* p_level = pSub_level())
    if (AL_PARTITION == p_level->Y_iGet(P_Level))
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* does op needs projection analysis?                                         */
/*----------------------------------------------------------------------------*/
bool Card_OP::bNeedsProjectionAnalysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (bIsCard(CK_TABLE)     ||
      bIsCard(CK_SCAN)      ||
      bIsCard(CK_INDEXSCAN) ||
      bIsCard(CK_INDEXJOIN))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* does op needs projection correction?                                       */
/*----------------------------------------------------------------------------*/
bool Card_OP::bNeedsProjectionCorrection(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (bIsCard(CK_GBYAGG)    ||
      bIsCard(CK_ORDAGG))
    return true;

  return false;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_OP::bPeg_Chosen_index(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_indexList = nullptr;
  pCI->G.p_index = nullptr;

  if (! (pCI->G.p_indexList = pSub_indexList()))
    return false;

  if (! (pCI->G.p_index = pCI->G.p_indexList->pFindChosenIndex()))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_OP::bPeg_Key(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! (pCI->G.p_key = pSub_key()))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_OP::bPeg_Projection(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! (pCI->G.p_projection = pSub_projection()))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* table projection analysis                                                  */
/*----------------------------------------------------------------------------*/
void Card_OP::vProjectionAnalysis(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (!bNeedsProjectionAnalysis() )
    return;

  Card_Projection* p_projection;

  if ((p_projection = pSub_projection()))
    vRemoveDeleteChild(p_projection);

  if (pSub_keyExists() || pSub_keyNotExists()) {
    p_projection = _pCard_Projection(pCI->G.p_projection->pDupCardAndSubCards());
    vAppend(p_projection);
    pCI->G.p_projection = p_projection;
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_tableNm = pSub_tableNm();

  Table* pTable = Card_Table::pAccess_Table(pCI->G.p_tableNm->sz());
  StreamDef* pStreamDef = pTable->pStreamDef();

  if (pCI->G.p_projection)
    p_projection = _pCard_Projection(pCI->G.p_projection->pDupCardAndSubCards());
  else
    p_projection = new Card_Projection;

  //  --  --  --  --  --  --  --  --  --  --  --  --  --

  i8 i_Expr = p_projection->nExpr() + 1;

  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    Card_Expr* p_expr = new Card_Expr;
    p_expr->vValueSprint("$%lld"_Stz,i_Expr++);
    p_projection->vAppend(p_expr);

    Stz* szType = aCT[pColumn->eColumnType()];
    Card_Type *pCard_Type = new Card_Type(szType);
    p_expr->vAppend(pCard_Type);

    Card_As* p_as = new Card_As(pColumn->szColNm());
    p_expr->vAppend(p_as);

    Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
    p_expr->vAppend(p_rpnCalc);

    Card_Term* p_term = new Card_Term;
    p_term->vValueSprint("$%c.%lld"_Stz,cValue(),i_Col+1);
    p_rpnCalc->vAppend(p_term);
  }

  vAppend(p_projection);

  pCI->G.p_projection = p_projection;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
